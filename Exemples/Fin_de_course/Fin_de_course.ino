#include <Arduino.h>
#include <Wire.h>
#include <Sensors_kit.h>

FinDeCourse MyCapteur1(0x03,1); //Déclaration du capteur 1 à l'adresse 0x03
FinDeCourse MyCapteur1(0x03,2); //Déclaration du capteur 2 à l'adresse 0x03

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Start...");
}

void loop() {
  Serial.print("Etat du capteur 1: ");
  Serial.println(MyCapteur1.State()); //=1 si enclenché, 0 sinon
  Serial.print("Etat du capteur 2: ");
  Serial.println(MyCapteur2.State()); //=1 si enclenché, 0 sinon
  delay(100);
}
