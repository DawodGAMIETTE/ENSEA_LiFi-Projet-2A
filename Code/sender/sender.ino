/**
 * @file sender.ino
 * @brief This file contains the implementation for scanning and connecting to a BLE receiver, and sending messages.
 */

#include <ArduinoBLE.h>

const char* receiverUUID = "19b10000-e8f2-537e-4f6c-d104768a1214";

void setup() {
  BLE.begin();
  BLE.scanForUuid(receiverUUID);
}

/**
 * @brief Main loop that checks for available BLE devices and controls the LED if the receiver is found.
 */
void loop() {
  BLEDevice receiver = BLE.available();

  if (receiver && receiver.localName() == "RECEPTEUR") {
    BLE.stopScan();
    controlLed(receiver);
    BLE.scanForUuid(receiverUUID);
  }
}

/**
 * @brief Connects to the receiver and sends a message to control the LED.
 * @param receiver The BLE device representing the receiver.
 */
void controlLed(BLEDevice receiver) {
  if (!receiver.connect() || !receiver.discoverAttributes()) {
    receiver.disconnect(); // Disconnect if connection or attribute discovery fails
    return;
  }

  BLECharacteristic senderCharacteristic = receiver.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

  if (!senderCharacteristic || !senderCharacteristic.canWrite()) {
    receiver.disconnect(); // Disconnect if the characteristic is not found or not writable
    return;
  }

  while (receiver.connected()) {
    senderCharacteristic.writeValue((uint8_t*)"Hello", 5);
    delay(1000);
  }
}