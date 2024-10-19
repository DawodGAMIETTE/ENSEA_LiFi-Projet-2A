/**
 * @file receiver.ino
 * @brief This file contains the implementation for receiving and processing BLE messages.
 */

#include <ArduinoBLE.h>

// BLE service and characteristic
BLEService receiverService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic receiverCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLEWrite, 20);

/**
 * @brief Initializes the serial communication and BLE module.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(A1, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("Starting BLE module failed!");
    while (1);
  }

  BLE.setLocalName("RECEPTEUR");
  BLE.setAdvertisedService(receiverService);
  receiverService.addCharacteristic(receiverCharacteristic);
  BLE.addService(receiverService);
  BLE.advertise();

  Serial.println("BLE peripheral running...");
}

/**
 * @brief Main loop that handles BLE connections and processes received messages.
 */
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (receiverCharacteristic.written()) {
        // Read the received message
        String receivedMessage((char*)receiverCharacteristic.value(), receiverCharacteristic.valueLength());
        Serial.println("Message reçu: " + receivedMessage);

        // Process each character in the received message
        for (char myChar : receivedMessage) {
          for (int i = 7; i >= 0; i--) {
            byte bitValue = bitRead(myChar, i); // Read each bit of the character
            Serial.print(bitValue, BIN);
            analogWrite(A1, bitValue ? 255 : 0);
            delay(250);
          }
          analogWrite(A1, 0); // Turn off the output
          delay(1000);
          Serial.println();
        }
      }
      delay(1000);
    }

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}