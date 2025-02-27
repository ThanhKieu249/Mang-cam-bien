#include "SRF05.h"

const int trigger = 5;
const int echo    = 18;

SRF05 SRF(trigger, echo);


void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("SRF05_LIB_VERSION: ");
  Serial.println(SRF05_LIB_VERSION);
  Serial.println();

  SRF.setCorrectionFactor(1.035);
  SRF.setModeAverage(10);
}


void loop()
{
  float distance = SRF.getMillimeter() / 10.0;
  Serial.print("Khoảng cách: ");
  Serial.print(distance, 2);
  Serial.println(" cm");
  delay(1000);
}