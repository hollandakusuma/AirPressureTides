#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <RTClib.h>

RTC_DS3231 rtc;
Adafruit_BME280 bme;
Adafruit_BMP280 bmp;
File dataFile;

const int chipSelect = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("BMP280 & BME280 Air Pressure Observation");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed");
    while (1);
  }

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  DateTime now = rtc.now();
  
  float tempBME = bme.readTemperature();
  float pressureBME = bme.readPressure() / 100.0F;
  float humidityBME = bme.readHumidity();

  float tempBMP = bmp.readTemperature();
  float pressureBMP = bmp.readPressure() / 100.0F;

  pressureBME = pressureBME + 2.283;
  pressureBMP = 0.9892 * pressureBMP + 11.006;

  if (!dataFile) {
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.print("Time, Pressure (BME280), Pressure (BMP280)\n");
  }

  dataFile.print(now.hour());
  dataFile.print(":");
  dataFile.print(now.minute());
  dataFile.print(":");
  dataFile.print(now.second());
  dataFile.print(",");
  dataFile.print(pressureBME);
  dataFile.print(",");
  dataFile.println(pressureBMP);
  dataFile.close();
  
  delay(60000);
}
