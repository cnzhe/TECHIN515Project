#include <Wire.h>  // Include the Wire library for I2C
#include "Adafruit_VL53L0X.h" // Include the VL53L0X distance sensor library
#include "HX711.h" // Include the HX711 library for the load cell

// VL53L0X Setup
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// HX711 Setup
const int LOADCELL_DOUT_PIN = 7; // Use GPIO7 for the DOUT pin of HX711
const int LOADCELL_SCK_PIN = 8; // Use GPIO8 for the SCK pin of HX711
HX711 scale;

void setup() {
  Serial.begin(115200);

  // Wait for the serial port to connect. Needed for native USB
  while (!Serial) {
    delay(1);
  }

  // Initialize I2C for VL53L0X
  Wire.begin(4, 3); // SDA on GPIO4 (D3), SCL on GPIO3 (D2)
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

  // Initialize
  Serial.println("Initializing...");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(231.85); // calibration factor
  scale.tare(); // Reset the scale to 0

  Serial.println("Setup complete.");
}

void loop() {
  // VL53L0X measurement
  VL53L0X_RangingMeasurementData_t measure;
  Serial.println("Reading a measurement... ");
  lox.rangingTest(&measure, false); // true for debug data

  if (measure.RangeStatus != 4) {  // Phase failures have incorrect data
    Serial.print("Distance (mm): "); 
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.print("Distance (mm): "); 
    Serial.println("Out of range");
  }

  // HX711 measurement
  float weight = scale.get_units();
  if (weight < 0) {
    weight = 0.0; // Adjust negative readings to zero
  }
  Serial.print("Weight (g): ");
  Serial.println(weight, 1); // Print the weight in grams

  delay(1000); // Delay a second between measurements
}
