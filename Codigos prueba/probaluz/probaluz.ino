int sense01 = 33;//Pin analógico
int val01 = 0;//Valor del sensor de luz
int valueMax = 1100;
int valueMin = 0;
int percentLight = 50;
void setup()
{
  Serial.begin(115200);//Inicializa el puerto serie
  pinMode(sense01, INPUT);
}

void loop()
{
  val01 = analogRead(sense01);
  Serial.print(val01);
  percentLight = map(val01, valueMin, valueMax, 0, 100);
  Serial.print("\t");
  Serial.print("Porcentaje de luz: ");
  Serial.print(percentLight);
  Serial.println("%");
  delay(1500);

}
