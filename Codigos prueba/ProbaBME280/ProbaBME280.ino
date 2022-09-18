#include <Wire.h>
#include <BMP280_DEV.h>

#define I2C_SD 21
#define I2C_SCL 22

BMP280_DEV bmp;

float temperature;//, humidity, pressure, altitude;

unsigned long delayTime;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));
  

  bmp.begin(0x76);
  bmp.setTimeStandby(TIME_STANDBY_05MS);     // Set the standby time to 2 seconds
  bmp.startNormalConversion();

  Serial.println("-- Default Test --");

  Serial.println();
}

void loop() {
  printValues();
  delay(1500);
}

void printValues() {

  bmp.getCurrentTemperature(temperature/*, pressure, altitude*/);

  Serial.print("Temperatura = ");
  Serial.print(temperature);
  Serial.println(" *C");
  Serial.println();
}
