#include <BH1750.h>
#include <Wire.h>
//SDA PIN 21, SCL PIN 22
BH1750 lightMeter(0x23);

void setup() {

  Serial.begin(9600);

  Wire.begin();
  
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
    delay(1000);
  }
}