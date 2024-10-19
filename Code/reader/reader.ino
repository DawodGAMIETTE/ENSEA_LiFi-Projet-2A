/**
 * @file reader.ino
 * @brief This file contains the implementation for reading analog voltage from pin A2,
 *        converting it to bits, and printing the corresponding character if the voltage
 *        remains stable for a specified threshold time.
 */

#define PIN_A2 A2

// Global variables
unsigned long previousMillis = 0; ///< Stores the last time the voltage was checked
const long interval = 250; ///< Measurement interval in milliseconds
const long threshold = 1000; ///< Time threshold for conversion to character
float previousVoltage = 0; ///< Stores the previous voltage value
int lb[8] = {0}; ///< Array to store the bits
int index = 0; ///< Current index in the array
bool conversionNeeded = false; ///< Indicates if a conversion is needed

void setup() {
  Serial.begin(9600);
}

/**
 * @brief Main loop that reads the voltage, converts it to bits, and prints the character
 *        if the voltage remains stable for the threshold time.
 */
void loop() {
  unsigned long currentMillis = millis();

  // Measure the voltage on pin A2
  float voltage = (analogRead(PIN_A2) * 5.0) / 1024;

  // Convert the voltage to bits according to the specifications
  if (voltage < 1) { // Case where V is close to 0 V
    lb[index] = 1;
    index++;
  } else {
    lb[index] = 0;
    index++;
  }

  // Check if the voltage has remained constant for the threshold time
  if (abs(voltage - previousVoltage) < 0.01) {
    if (!conversionNeeded) {
      previousMillis = currentMillis;
      conversionNeeded = true;
    } else if (currentMillis - previousMillis >= threshold) {
      conversionNeeded = false;
      convertAndPrint();
      // Reset all values in lb to 0
      memset(lb, 0, sizeof(lb));
      index = 0;
    }
  } else {
    conversionNeeded = false;
  }

  previousVoltage = voltage;

  delay(interval);
}

/**
 * @brief Converts the bits in the lb array to a decimal value and prints the corresponding character.
 */
void convertAndPrint() {
  int decimal = 0;
  for (int i = 0; i < 8; i++) {
    decimal += lb[i] * (1 << (7 - i));
  }
  char character = char(decimal);
  Serial.println("");
  Serial.println(decimal);
  Serial.println(character);
}