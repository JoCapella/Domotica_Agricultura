#include <Wire.h>
#include <BMP280_DEV.h>
#include "ATH20.h"

//Variables globales

#define I2C_SD 21                                          //I2C SDA Pin
#define I2C_SCL 22                                         //I2C SCL Pin

BMP280_DEV bmp;                                            //BMP280 Object Definition
ATH20 ATH;                                                 //AHT20 Object Definition

float temperature;                                         //What we are measuring with the BMP Sensor
float humi;                                                //What we are measuring with the AHT Sensor

int PinLight = 33;                                         //Pin where TEMT6000 is connected (ADC:0-4095)
int LightValue = 0;                                        //Light sensor value
int valueMax = 1100;
int valueMin = 0;
int percentLight = 50;

void setup()
{
  Serial.begin(115200);                                    //SerialPort 115200bauds
  Serial.println(F("*****Prueba de todos los sensores.*****\n"));
  
  pinMode(PinLight, INPUT);

  bmp.begin(0x76);                                         //Begin I2C communication on 0x76 address, which is the BMP280 one
  bmp.setTimeStandby(TIME_STANDBY_500MS);                  //Set the standby time to 0.5 seconds
  bmp.startNormalConversion();                             //Start continuous measurement in NORMAL_MODE

  ATH.begin();                                             //Begin I2C communication on default 0x38 address, which is the AHT20 one
  
} 
void loop()
{
  printLight();
  print_Temp();
  printHum();
  
  delay(1500);

}
void printLight()
{
  LightValue = analogRead(PinLight);                       //Updates light sensor
  percentLight = map(LightValue, valueMin, valueMax, 0, 100);
  Serial.print("Porcentaje de luz: ");
  Serial.print(percentLight);
  Serial.println("%");                              //Prints light sensor value
}
void print_Temp()
{
  bmp.getCurrentTemperature(temperature);

  Serial.print("Temperatura = ");
  Serial.print(temperature);
  Serial.println(" ºC;  ");
}
void printHum()
{
    ATH.getHumidity(&humi);
    Serial.print("Humidity: ");
    Serial.print(humi * 100);
    Serial.println("%");
}
