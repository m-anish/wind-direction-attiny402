#include <Wire.h>  // For I2C communication

// Constants
const uint8_t SLAVE_I2C_ADDRESS = 0x42; // Address of the slave I2C device

void setup() {
  // Initialize Serial for debugging (optional)
  Serial.begin(9600);

  // Initialize I2C as a master
  Wire.begin();
}

void loop() {
  int heading = readWindDirection();
  
  // Debugging: Print the received heading
  Serial.print("Wind Direction Heading: ");
  Serial.println(heading);

  // Add a delay between requests (adjust as needed)
  delay(500);
}

// Function to read the wind direction from the slave
int readWindDirection() {
  Wire.requestFrom(SLAVE_I2C_ADDRESS, 2); // Request 2 bytes from the slave

  int heading = 0;
  if (Wire.available() == 2) {
    heading = Wire.read();        // Read the low byte
    heading |= (Wire.read() << 8); // Read the high byte and combine
  }

  return heading;
}
