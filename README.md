# Arduino CAN Communication with MCP2515

This project demonstrates basic CAN (Controller Area Network) communication using an Arduino and the MCP2515 CAN controller. It includes setup, sending, and receiving CAN messages.

## Components Used

- Arduino
- MCP2515 CAN Controller Module
- SPI Interface

## Setup

1. **Connect the MCP2515 to the Arduino:**
   - SCLK to pin 12
   - MISO to pin 13
   - MOSI to pin 11
   - CS to pin 10
   - VCC to 5V
   - GND to GND

2. **Install Libraries:**
   - Install `SPI` and `mcp2515` libraries in the Arduino IDE.

## Code Overview

### Libraries
```cpp
#include <SPI.h>
#include <mcp2515.h>
```

### Global Variables
```cpp
struct can_frame canMsg;
MCP2515 mcp2515(10); // CS pin on GPIO 10
```

### Setup Function
```cpp
void setup() {
  Serial.begin(115200);
  SPI.begin(12, 13, 11, 10); // SCLK, MISO, MOSI, CS

  mcp2515.reset();
  if (mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ) == MCP2515::ERROR_OK) {
    Serial.println("MCP2515 Initialized Successfully");
  } else {
    Serial.println("Error Initializing MCP2515");
    while(1); // Stop execution if initialization fails
  }

  mcp2515.setLoopbackMode();

  canMsg.can_id = 0x100;
  canMsg.can_dlc = 8;
  for (int i = 0; i < 8; i++) {
    canMsg.data[i] = 0xFF;
  }
}
```

### Loop Function
```cpp
void loop() {
  if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.println("Message Sent Successfully");
  } else {
    Serial.println("Error Sending Message");
  }

  struct can_frame rxMsg;
  if (mcp2515.readMessage(&rxMsg) == MCP2515::ERROR_OK) {
    Serial.print("Received Message with ID: ");
    Serial.println(rxMsg.can_id, HEX);
    Serial.print("Data: ");
    for (int i = 0; i < rxMsg.can_dlc; i++) {
      Serial.print(rxMsg.data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  delay(1000);
}
```

## How It Works

1. **Initialization**: Sets up SPI communication and initializes the MCP2515 in loopback mode.
2. **Sending CAN Message**: Sends a CAN message with ID 0x100 and 8 data bytes set to 0xFF.
3. **Receiving CAN Message**: Checks for received CAN messages and prints them to the serial monitor.

