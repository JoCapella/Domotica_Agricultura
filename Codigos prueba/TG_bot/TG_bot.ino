#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
// Replace with your network credentials
const char* ssid = "RUUUIIIIDOO";
const char* password = "GodFas44!";
// Initialize Telegram BOT
#define BOTtoken "2025063291:AAEU_mpiVdW2JaCbJcGjMtvjHeJDI3e7k-I" // your Bot Token (Get from Botfather)
#define CHAT_ID "298656787"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
const int ledPin = 2;
bool ledState = LOW;
// Handle new receive messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;
    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    if (text == "/Hola")
    {
      bot.sendMessage(chat_id, "Hola q ase jeje", "");
    }
    if (text == "/Cardano")
    {
      bot.sendMessage(chat_id, "Me la trincas con la mano", "");
    }
    if (text == "/Shiba")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/temperatura")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/humedad")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/presion")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/altitud")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/luz")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/state")
    {
      if (digitalRead(ledPin)) {
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else {
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}
void setup() {
  Serial.begin(115200);
#ifdef ESP8266
  client.setInsecure();
#endif
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
/*
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

  //Serial.println();
  }

  void print_Temp_Hum()
  {
  ATH.getSensor(&humi, &temp);
  Serial.print("Humidity: ");
  Serial.print(humi * 100);
  Serial.print("%\t TemperatureAHT20: ");
  Serial.println(temp);
  }
*/
/*void handleNewMessages(int numNewMessages)
  {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = String(bot.messages[i].chat_id);

    if (chat_id != CHAT_ID)
    {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;
    if (text == "/start")
    {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }
    bool sendMessage(String chat_id, String text, String parse_mode = "");
    if (text == "/Que_pasa")
    {
      print_Temp_Press_Alt();
      printLight();
      print_Temp_Hum();
      String enviar_ =  "Tenemos una temperatura exquisita de " + String(temperature, 2) + "ºC\n" +
                        "Así como una humedad relativa del " + String(humi, 2) + "%\n" +
                        "Por si te interesa, una presión atmosférica de " + String(pressure, 2) + "hPa" +
                        "Tambien una altitud de " + String(altitude, 2) + "m\n" +
                        "La luz tiene un nivel del " + String(humi, 2) + "%\n";

      bot.sendMessage(chat_id, enviar_, "");
    }
    if (text == "/Hola")
    {
      bot.sendMessage(chat_id, "Hola q ase jeje", "");
    }
    if (text == "/Cardano")
    {
      bot.sendMessage(chat_id, "Me la trincas con la mano", "");
    }
    if (text == "/Shiba")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/temperatura")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/humedad")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/presion")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/altitud")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
    if (text == "/luz")
    {
      bot.sendMessage(chat_id, "TO THE MOON🚀🚀🚀!!", "");
    }
  }
  }
*/
