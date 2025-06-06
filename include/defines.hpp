#pragma once

// Nordic UART Service UUIDs
// Used for the app to recognize the device as a valid moonboard
// See more at https://developer.nordicsemi.com/nRF_Connect_SDK/doc/v1.7-branch/nrf/libraries/bluetooth_services/services/nus.html
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define HOLD_AMOUNT  200
#define OUTPUT_PIN   16
#define MOONBOARD_MAX_CONNECTIONS 10
#define MOONBOARD_DEVICE_NAME "Moonboard CABA Bucarelli"

