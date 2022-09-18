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
double voltage  = 0.00;
float Ph        = 0.00;

WiFiClient ESP32client1;
PubSubClient client(ESP32client1);
long lastMsg = 0;
char msg[50];
int value = 0;
char json_msg[256];


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
  if (now - lastMsg > 5000)
  {
    lastMsg = now;

    printPh();

    DynamicJsonDocument doc(1024);

    doc["Ph"] = Ph;

    serializeJson(doc, json_msg);

    client.publish("main_and_only_topic", json_msg);

    Serial.println();
  }
}

void printPh()
{
  PinPhValue = analogRead(PinPh);
  Serial.println(PinPhValue);  
  voltage = (5.00 / 5912.00) * PinPhValue; 
  Ph = 7 + ((2.5 - voltage) / 0.1839);
  Serial.print(Ph);
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
