#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//---------------------WiFi Credentials -----------------------

const char* ssid     = "RUUUIIIIDOO";
const char* password = "GodFas44!";
const char* mqtt_server = "192.168.1.5";
const char* mqtt_username = "admin"; // MQTT username
const char* mqtt_password = "Password1"; // MQTT password
const char* clientID = "ESP32client2";
const char* main_and_only_topic = "global/measurements";

//---------------------GLOBAL VARIABLES-------------------------

int PinPh       = 33;
int PinPhValue  = 0;
double Po       = 0.00;
float sensorMax = 3125.00; 
float sensorMin = 2670.00;

WiFiClient ESP32client1;
PubSubClient client(ESP32client1);
long lastMsg = 0;
char msg[50];
int value = 0;
char json_msg[256];

//--------------------------FUNCTIONS-------------------------------
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void printPh()
{
  PinPhValue = analogRead(PinPh);
  Serial.println(PinPhValue);
  Po = fmap(PinPhValue, sensorMax, sensorMin, 6.00, 8.00);
  Serial.print("\tPH: ");
  Serial.print(Po);
  Serial.println("");
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
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID, mqtt_username, mqtt_password))
    {
      Serial.println("connected");
    }
    else
    {
      //This gives us why didnt connected
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}
//---------------------------SETUP--------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.println("");

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
  if (now - lastMsg > 3500)
  {
    lastMsg = now;

    printPh();

    DynamicJsonDocument doc(1024);

    doc["Ph"] = Po;

    serializeJson(doc, json_msg);

    client.publish("main_and_only_topic", json_msg);

    Serial.println();
  }
}

