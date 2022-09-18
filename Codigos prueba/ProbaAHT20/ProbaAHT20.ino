#include <Wire.h>
#include "ATH20.h"

ATH20 ATH;

void setup()
{
  Serial.begin(115200);
  Serial.println("ATH20 DEMO");
  ATH.begin();
}

void loop()
{
  float humi;
  int ret = ATH.getHumidity(&humi);

  if (ret)    // GET DATA OK
  {
    Serial.print("Humidity: ");
    Serial.print(humi * 100);
    Serial.println("%");
  }
  else        // GET DATA FAIL
  {
    Serial.println("GET DATA FROM ATH20 FAIL");
  }

  delay(1500);
}
