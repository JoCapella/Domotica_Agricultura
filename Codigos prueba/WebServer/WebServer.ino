#include <WiFi.h>
#include <Wire.h>
#include <BMP280_DEV.h>
#include "ATH20.h"

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "RUUUIIIIDOO";
const char* password = "GodFas44!";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;
String header;                                             //Variable para guardar el HTTP request
String estadoSalida = "off";
const int salida = 2;

//#define I2C_SD 33                                          //I2C SDA Pin
//#define I2C_SCL 32                                         //I2C SCL Pin

BMP280_DEV bmp;                                            //BMP280 Object Definition
ATH20 ATH;                                                 //AHT20 Object Definition

float temperature, humidity, pressure, altitude;           //What we are measuring with the BMP Sensor
float humi, temp;                                          //What we are measuring with the AHT Sensor

int PinLight = 33;                                          //Pin where TEMT6000 is connected (ADC:0-4095)
int LightValue = 0;                                        //Light sensor value

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8' />"
                "<title>CARDANO</title>"
                "</head>"
                "<body>"
                "<center>"
                "<h1>CARDANO</h1>"
                "<p><a href='/on'><button style='height:50px;width:100px'>Agarramela</button></a></p>"
                "<p><a href='/off'><button style='height:50px;width:100px'>Conlamano</button></a></p>"
                "</center>"
                "</body>"
                "</html>";


//---------------------------SETUP--------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("");

  bmp.begin(0x76);                                         //Begin I2C communication on 0x76 address, which is the BMP280 one
  bmp.setTimeStandby(TIME_STANDBY_500MS);                  //Set the standby time to 0.5 seconds
  bmp.setSeaLevelPressure(1020.87);                        //Some calibration
  bmp.startNormalConversion();                             //Start continuous measurement in NORMAL_MODE

  ATH.begin();                                             //Begin I2C communication on default 0x38 address, which is the AHT20 one

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    //para usar con ip fija
    //IPAddress ip(192,168,1,180);
    //IPAddress gateway(192,168,1,1);
    //IPAddress subnet(255,255,255,0);
    //WiFi.config(ip, gateway, subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    server.begin(); // iniciamos el servidor
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop() {
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          //
    String currentLine = "";                //
    while (client.connected())
    { // loop mientras el cliente está conectado
      if (client.available())
      { // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n')
        { // si el byte es un caracter de salto de linea
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Muestra la página web
            printLight();
            print_Temp_Press_Alt();
            print_Temp_Hum();
            updatepagina();
            client.println(pagina);

            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void updatepagina()
{
  pagina =      "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8' />"
                "<title>ESP32 WEBSERVER</title>"
                "</head>"
                "<body>"
                "<center>"
                "<h1>ESP32 SENSORES</h1>"
                "<h2>TEMPERATURA</h2>"
                "<h3>BMP280: " + String(temperature, 2) + "*C &nbsp AHT20:" + String(temp, 2) + "*C</h3>"
                "<h2>HUMEDAD</h2>"
                "<h3>AHT: " + String(humi*100, 2) + "%</h3>"
                "<h2>PRESION ATM</h2>"
                "<h3>BMP280: " + String(pressure, 2) + "hPa</h3>"
                "<h2>ALTITUD</h2>"
                "<h3>BMP280: " + String(altitude, 2) + "m</h3>"
                "<h2>NIVEL DE LUZ</h2>"
                "<h3>TMT6000: " + String(LightValue) + "%</h3>"
                "</center>"
                "</body>"
                "</html>";
}

void printLight()
{
  LightValue = analogRead(PinLight);                       //Updates light sensor
  LightValue = map(LightValue, 0, 400, 0, 100);
  if(LightValue > 100)
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
