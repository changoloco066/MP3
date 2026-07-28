#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hola, ESP32-S3 vivo!");
}

void loop() {
  Serial.println("tick");
  delay(1000);
}