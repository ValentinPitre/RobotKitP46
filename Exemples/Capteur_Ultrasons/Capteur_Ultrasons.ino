#include <Arduino.h>
#include <Wire.h>
#include <Sensors_kit.h>

CapteurUltrasons MyCapteur(0x03); //Déclaration du capteur à l'adresse 0x03

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Start...");
}

void loop() {
  Serial.print("Distance en cm: ");
  Serial.println(MyCapteur.DistanceCm());
  Serial.print("Distance en mm: ");
  Serial.println(MyCapteur.DistanceMm());
  Serial.println("");
  delay(500);
}
