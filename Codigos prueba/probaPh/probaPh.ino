int PinPh = 33;
int PinPhValue = 0;
double Po = 0;
float sensorMax = 3125.00; 
float sensorMin = 2670.00;
void setup()
{
  Serial.begin(115200);//Inicializa el puerto serie
  pinMode(PinPh, INPUT);

}
void loop()
{
  PinPhValue = analogRead(PinPh);
  Serial.println(PinPhValue);
  
  Po = fmap(PinPhValue, sensorMax, sensorMin, 6.00, 8.00);
  Serial.print("\tPH: ");
  Serial.print(Po); 

  Serial.println("");
  delay(2000);

}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
