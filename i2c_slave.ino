#include <Wire.h>  // For I2C communication
#include <avr/sleep.h> // For managing sleep modes

// Constants
const uint8_t I2C_ADDRESS = 0x42; // Change to your desired I2C address
const uint8_t ANALOG_PIN = PIN_PA1; // Analog pin to read from (PA1 as an example)

// Global variables
volatile bool dataReady = false; // Flag to indicate data processing is required
int lastHeading = 0; // Store the last calculated heading

void setup() {
  // Configure analog pin as input
  pinMode(ANALOG_PIN, INPUT);

  // Initialize I2C as a slave
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(onI2CRequest);
  Wire.onReceive(onI2CReceive);

  // Configure sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Enable interrupts globally
  sei();
}

void loop() {
  // Check if data needs processing
  if (dataReady) {
    dataReady = false; // Reset the flag

    // Read the analog value
    int analogValue = analogRead(ANALOG_PIN);

    // Convert analog value to wind direction heading
    lastHeading = calculateWindDirection(analogValue);
  }

  // Enter sleep mode
  enterSleep();
}

void enterSleep() {
  // Ensure interrupts are enabled before sleeping
  sei();
  sleep_cpu();

  // Execution resumes here after waking up
}

void onI2CReceive(int numBytes) {
  // Wake up and set flag for processing
  dataReady = true;
}

void onI2CRequest() {
  // Send the last calculated heading to the I2C master
  Wire.write(lastHeading & 0xFF);       // Send the low byte
  Wire.write((lastHeading >> 8) & 0xFF); // Send the high byte
}

int calculateWindDirection(int analogValue) {
  // Convert the 8-bit analog value from the R-2R ladder to a heading (0-359 degrees)
  // Each step corresponds to 360 / 8 = 45 degrees
  int directionIndex = analogValue / 32; // Divide by 32 to map 0-255 to 0-7
  return directionIndex * 45; // Convert index to heading
}
