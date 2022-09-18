#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <BMP280_DEV.h>
#include "ATH20.h"
#include <ArduinoJson.h>
//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "RUUUIIIIDOO";
const char* password = "GodFas44!";
const char* mqtt_server = "192.168.1.78";

//---------------------VARIABLES GLOBALES-------------------------
BMP280_DEV bmp;                                            //BMP280 Object Definition
ATH20 ATH;                                                 //AHT20 Object Definition

float temperature, pressure, altitude;                     //What we are measuring with the BMP Sensor
float humi, temp;                                          //What we are measuring with the AHT Sensor
char tempString[8], temp2String[8], humiString[8], pressString[8], altString[8], lightString[8];

int PinLight = 33;                                         //Pin where TEMT6000 is connected (ADC:0-4095)
int LightValue = 0;                                        //Light sensor value

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char json_msg[256];


// LED Pin
const int ledPin = 13;

//---------------------------SETUP--------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("");

  bmp.begin(0x76);                                         //Begin I2C communication on 0x76 address, which is the BMP280 one
  bmp.setTimeStandby(TIME_STANDBY_500MS);                  //Set the standby time to 0.5 seconds
  bmp.setSeaLevelPressure(1020.87);                        //Some calibration
  bmp.startNormalConversion();                             //Start continuous measurement in NORMAL_MODE

  ATH.begin();                                             //Begin I2C communication on default 0x38 address, which is the AHT20 one

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);

}

//----------------------------LOOP----------------------------------

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000)
  {
    lastMsg = now;

    printLight();
    print_Temp_Press_Alt();
    print_Temp_Hum();

    DynamicJsonDocument doc(1024);

    doc["Temperature"][0]     = temperature;
    doc["Temperature"][1]     = temp;
    doc["Humidity"]           = humi;
    doc["Pressure"]           = pressure;
    doc["Light"]              = LightValue;

    serializeJson(doc, json_msg);

    client.publish("test/topic", json_msg);
    
    Serial.println();
    /*
      printLight();
      print_Temp_Press_Alt();
      print_Temp_Hum();

      dtostrf(temperature, 1, 2, tempString);
      dtostrf(temp, 1, 2, temp2String);
      dtostrf(humi, 1, 2, humiString);
      dtostrf(pressure, 1, 2, pressString);
      dtostrf(altitude, 1, 2, altString);
      dtostrf(LightValue, 1, 2, lightString);


      client.publish("test/topic", tempString);
      client.publish("test/topic", temp2String);
      client.publish("test/topic", humiString);
      client.publish("test/topic", pressString);
      client.publish("test/topic", altString);
      client.publish("test/topic", lightString);
    */
  }
}

void printLight()
{
  LightValue = analogRead(PinLight);                       //Updates light sensor
  LightValue = map(LightValue, 0, 400, 0, 100);
  if (LightValue > 100)
    LightValue = 100;
  Serial.println(LightValue);                              //Prints light sensor value
}

void print_Temp_Press_Alt()
{
  bmp.getCurrentMeasurements(temperature, pressure, altitude);

  Serial.print("TemperatureBMP280 = ");
  Serial.print(temperature);
  Serial.print(" *C;  ");


  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.print(" hPa;  ");

  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
}

void print_Temp_Hum()
{
  ATH.getSensor(&humi, &temp);
  Serial.print("Humidity: ");
  humi *= 100;
  Serial.print(humi);
  Serial.print("%\t TemperatureAHT20: ");
  Serial.println(temp);
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
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

void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "test/topic")
  {
    Serial.print("Changing output to ");
    if (messageTemp == "on")
    {
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if (messageTemp == "off")
    {
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("test/topic");
    } else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
