#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <BMP280_DEV.h>
#include "ATH20.h"
#include <ArduinoJson.h>

//---------------------WiFi Credentials -----------------------

const char* ssid     = "RUUUIIIIDOO";
const char* password = "GodFas44!";
const char* mqtt_server = "192.168.1.5";
const char* mqtt_username = "admin"; // MQTT username
const char* mqtt_password = "Password1"; // MQTT password
const char* clientID = "ESP32client1";
const char* main_and_only_topic = "global/measurements";

//---------------------GLOBAL VARIABLES-------------------------

BMP280_DEV bmp;                                            //BMP280 Object Definition
ATH20 ATH;                                                 //AHT20 Object Definition

float temperature, pressure, altitude;                     //What we are measuring with the BMP Sensor
float humi, temp;                                          //What we are measuring with the AHT Sensor

int PinLight = 33;                                         //Pin where TEMT6000 is connected (ADC:0-4095)
int LightValue = 0;                                        //Light sensor value
int valueMax = 1100;
int valueMin = 0;
int percentLight = 50;


WiFiClient ESP32client1;
PubSubClient client(ESP32client1);
long lastMsg = 0;
char msg[50];
int value = 0;
char json_msg[256];

//--------------------------FUNCTIONS-------------------------------
void printLight()
{
  LightValue = analogRead(PinLight);                          //Updates light sensor
  percentLight = map(LightValue, valueMin, valueMax, 0, 100); //Maps it to make it percentage
}

void print_Temp()
{
  bmp.getCurrentTemperature(temperature);
}

void print_Hum()
{
  ATH.getHumidity(&humi);
  humi *= 100;
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttConection()
{
  while (!client.connected())
  {
    Serial.print("Intento de conexion MQTT...");
    // Attempt to connect
    if (client.connect(clientID, mqtt_username, mqtt_password))
    {
      Serial.println("connected");
    }
    else
    {
      //Error por el cual no conecta
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Probar de nuevo en 5 segundos");

      delay(5000);
    }
  }
}
//---------------------------SETUP--------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("");

  bmp.begin(0x76);                                         //Begin I2C communication on 0x76 address, which is the BMP280 one
  bmp.setTimeStandby(TIME_STANDBY_500MS);                  //Set the standby time to 0.5 seconds
  bmp.startNormalConversion();                             //Start continuous measurement in NORMAL_MODE

  ATH.begin();                                             //Begin I2C communication on default 0x38 address, which is the AHT20 one

  setup_wifi();
  client.setServer(mqtt_server, 1883);

}


//----------------------------LOOP----------------------------------

void loop()
{
  //Whenever the mqtt client/broker disconects, we attempt to connect again
  if (!client.connected())
  {
    mqttConection();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000)
  {
    lastMsg = now;

    print_Temp();
    print_Hum();
    printLight();
    
    DynamicJsonDocument doc(1024);

    doc["Temperature"]        = temperature;
    doc["Humidity"]           = int(humi);
    doc["Light"]              = percentLight;

    serializeJson(doc, json_msg);

    client.publish("main_and_only_topic", json_msg);

  }
}

