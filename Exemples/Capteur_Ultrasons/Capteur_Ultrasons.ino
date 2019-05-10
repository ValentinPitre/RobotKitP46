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
  Serial.println(MyCapteur.DistanceCm()); //Valeur sur 1 octet entre 0 et 254 (255 étant un message d'erreur)
  Serial.println("");
  delay(500);
}
