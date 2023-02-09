#include <Wire.h>
#include <SD.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>

// Initialize the RTC object
RTC_DS3231 rtc;

// Initialize the BME280 and BMP280 sensors
Adafruit_BME280 bme;
Adafruit_BMP280 bmp;

// Create a string for the filename
char filename[] = "data.txt";

void setup() {
  Serial.begin(9600);
  Serial.println("BMP280 and BME280 Calibration Test");

  // Check if the RTC is connected and working
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the BME280 and BMP280 sensors are connected and working
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Initialize the SD card
  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed");
    while (1);
  }

  // Check if the file exists, and if not, create a new file and write the header
  if (!SD.exists(filename)) {
    File file = SD.open(filename, FILE_WRITE);
    file.println("Timestamp, Pressure (BME), Pressure (BMP)");
    file.close();
  }
}

void loop() {
  // Get the current date and time
  DateTime now = rtc.now();

  // Read the sensor values
  float pressureBME = bme.readPressure() / 100.0F;
  float pressureBMP = bmp.readPressure() / 100.0F;

  // Write the sensor values to the SD card
  File file = SD.open(filename, FILE_WRITE);
  file.print(now.unixtime());
  file.print(",");
  file.print(pressureBME);
  file.print(",");
  file.println(pressureBMP);
  file.close();

  
    // Write the sensor values to the Serial
  Serial.print(now.unixtime());
  Serial.print(",");
  Serial.print(pressureBME);
  Serial.print(",");
  Serial.println(pressureBMP);
   
  // Wait for 1 minute before reading the sensors again
  delay(60000);
}
