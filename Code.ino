#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10); // CS pin on GPIO 10

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

  // Set MCP2515 to loopback mode
  mcp2515.setLoopbackMode();

  // Initialize CAN message
  canMsg.can_id = 0x100;
  canMsg.can_dlc = 8;
  for (int i = 0; i < 8; i++) {
    canMsg.data[i] = 0xFF;
  }
}

void loop() {
  // Send CAN message
  if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.println("Message Sent Successfully");
  } else {
    Serial.println("Error Sending Message");
  }

  // Check for received CAN messages
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
