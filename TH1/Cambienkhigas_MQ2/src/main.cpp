#include<Arduino.h>

#define MQ2_PIN 34  

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("MQ-2 Gas Sensor Test");
}

void loop() {
    int raw_adc = analogRead(MQ2_PIN);  
    float voltage = raw_adc * (3.3 / 4095);  

    Serial.print("ADC: ");
    Serial.println(raw_adc);

    delay(1000);
}
