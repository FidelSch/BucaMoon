# Moonboard LED Controller

An ESP32-based Bluetooth LED controller for Moonboard climbing walls. This project allows you to control WS2811/WS2812 LED strips via Bluetooth Low Energy (BLE), fully compatible with the official Moonboard mobile app.

## What is a Moonboard?

A Moonboard is a standardized climbing training wall with a specific hold layout. LED controllers like this project illuminate holds to display climbing problems from the Moonboard app, making it easy to train with thousands of community-created routes.

## Features

- **Full Moonboard App Integration**: Works seamlessly with the official iOS/Android Moonboard app via BLE
- **Nordic UART Service (NUS)**: Uses standard BLE UART protocol for communication
- **200 LED Support**: Controls up to 200 individually addressable LEDs (one per hold)
- **WS2811/WS2812 Compatible**: Works with common LED strip types
- **Multi-client Support**: Can handle up to 10 simultaneous BLE connections
- **Problem Display**: Shows starting holds, moves, and finish holds with different colors
- **ESP32 Platform**: Runs on affordable ESP32 microcontrollers

## Hardware Requirements

### Required Components

- **ESP32 Board**: NodeMCU ESP32-S (tested) or similar ESP32 development board
  - Alternative: LILYGO T-Display (also supported)
- **LED Strip**: WS2811 or WS2812B addressable LED strip
  - Quantity: 200 LEDs (one per Moonboard hold)
  - Recommended: 12V WS2811 for longer strips, 5V WS2812B for shorter runs
- **Power Supply**:
  - 5V/10A (50W) for WS2812B LEDs, or
  - 12V/5A (60W) for WS2811 LEDs
  - Note: At full brightness, 200 LEDs can draw ~12A at 5V
- **Level Shifter** (recommended): 3.3V to 5V logic level converter for reliable data signal
- **Wiring**: Appropriate gauge wire for power distribution
- **Optional**: Capacitor (1000µF) across power supply for LED protection

### Wiring Diagram

```
ESP32 Pin 16 ──→ [Level Shifter] ──→ LED Strip Data In
                                      LED Strip 5V/12V ──→ Power Supply (+)
ESP32 GND ────────────────────────→ LED Strip GND ──────→ Power Supply (-)
```

> [!IMPORTANT]
> Always connect ESP32 GND to LED strip GND and power supply GND (common ground).

## Software Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- [Visual Studio Code](https://code.visualstudio.com/) (if using PlatformIO)

## Installation

### Step 1: Install Development Environment

**Option A: PlatformIO (Recommended)**

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)
3. Restart VS Code

**Option B: Arduino IDE**

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Add ESP32 board support:
   - File → Preferences → Additional Board Manager URLs
   - Add: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install

### Step 2: Clone the Repository

```bash
git clone https://github.com/FidelSch/BucaMoon
cd BucaMoon
```

### Step 3: Configure Your Setup

Edit [include/defines.hpp](include/defines.hpp) to customize:

```cpp
#define HOLD_AMOUNT  200           // Number of LEDs (holds)
#define OUTPUT_PIN   16            // GPIO pin connected to LED data
#define MOONBOARD_DEVICE_NAME "Moonboard CABA Bucarelli"  // BLE device name
```

### Step 4: Build and Upload

**Using PlatformIO:**

1. Open the project folder in VS Code
2. Select your board environment:
   - `env:nodemcu` for NodeMCU ESP32-S
   - `env:lilygo-t-display` for LILYGO T-Display
3. Click the PlatformIO upload button (→) or use the command palette:
   - `Ctrl+Shift+P` → "PlatformIO: Upload"

**Using Arduino IDE:**

1. Open `src/Server.ino`
2. Install required library: NeoPixelBus by Makuna
   - Sketch → Include Library → Manage Libraries → Search "NeoPixelBus"
3. Select your board: Tools → Board → ESP32 → NodeMCU-32S
4. Select your port: Tools → Port → (your ESP32 port)
5. Click Upload

### Step 5: Verify Operation

1. Open Serial Monitor (115200 baud)
2. You should see the device initialize and start BLE advertising
3. The LED strip should light up briefly on startup

## Usage

1. **Power on** the ESP32 with the LED strip connected
2. **Open** the Moonboard app on your phone
3. **Enable Bluetooth** on your phone
4. **Connect** to your device (should appear as "Moonboard CABA Bucarelli" or your custom name)
5. **Select a problem** from the app - the LEDs will automatically display the route

### LED Color Coding

- **Green**: Starting holds
- **Blue**: Movement holds
- **Red**: Finish holds

## Configuration Options

### Change LED Pin

Edit `OUTPUT_PIN` in [include/defines.hpp](include/defines.hpp):

```cpp
#define OUTPUT_PIN   16  // Change to your GPIO pin
```

### Change Number of LEDs

Edit `HOLD_AMOUNT` in [include/defines.hpp](include/defines.hpp):

```cpp
#define HOLD_AMOUNT  200  // Adjust based on your setup
```

### Change Device Name

Edit `MOONBOARD_DEVICE_NAME` in [include/defines.hpp](include/defines.hpp):

```cpp
#define MOONBOARD_DEVICE_NAME "My Moonboard"
```

### Adjust LED Strip Type

If using a different LED type, edit [include/Strip.hpp](include/Strip.hpp):

```cpp
// For WS2812B on GPIO3:
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> m_strip;
```

See [NeoPixelBus documentation](https://github.com/Makuna/NeoPixelBus/wiki) for other LED types.

## Troubleshooting

### LEDs don't light up

- Check power supply voltage and amperage
- Verify common ground between ESP32, LEDs, and power supply
- Test with a simple LED blink sketch first
- Try adding a level shifter for the data line

### App can't find device

- Ensure Bluetooth is enabled on your phone
- Check Serial Monitor for BLE initialization messages
- Verify device name matches in app and code
- Try resetting the ESP32

### Random LED colors or flickering

- Check data line connection and quality
- Add a 330Ω resistor on the data line
- Ensure power supply can provide enough current
- Reduce LED brightness in code if power is limited

### Compilation errors

- Ensure NeoPixelBus library is installed
- Select the correct board in platformio.ini or Arduino IDE
- Update PlatformIO platform if needed: `pio pkg update`

## Project Structure

```
BucaMoon/
├── include/
│   ├── defines.hpp              # Configuration constants
│   ├── Hold.hpp                 # Hold state definitions
│   ├── MoonboardCallbacks.hpp   # BLE callback handlers
│   ├── MoonboardServer.hpp      # BLE server interface
│   ├── Problem.hpp              # Problem data structures
│   └── Strip.hpp                # LED strip controller
├── src/
│   ├── MoonboardCallbacks.cpp   # BLE event handling
│   ├── MoonboardServer.cpp      # BLE server implementation
│   ├── Problem.cpp              # Problem parsing
│   ├── Server.ino               # Main application entry
│   └── Strip.cpp                # LED control logic
├── platformio.ini               # PlatformIO configuration
└── README.md                    # This file
```

## Technical Notes

- **BLE Protocol**: Uses Nordic UART Service (NUS) with standard UUIDs
- **LED Library**: NeoPixelBus by Makuna (more efficient than FastLED/Adafruit on ESP32)
- **LED Method**: WS2811 timing on configurable GPIO pin
- **FreeRTOS**: Uses ESP32's built-in task scheduler
- **Memory**: Optimized for 200 LED array storage

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Acknowledgments

- Based on the [Moonboard](https://www.moonboard.com/) climbing training system
- Uses [NeoPixelBus library](https://github.com/Makuna/NeoPixelBus) by Makuna
- Nordic UART Service specification
