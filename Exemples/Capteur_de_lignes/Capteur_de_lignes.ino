#include <Arduino.h>
#include <Wire.h>
#include <Sensors_kit.h>

CapteurDeLignes MyCapteur(0x03); //Déclaration du capteur à l'adresse 0x03

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Start...");
}

void loop() {
  Serial.print("Etat du capteur: ");
  byte etat = MyCapteur.State(); //le 1er bit représente la valeur du capteur droit, le 2eme bit le capteur gauche. 1=Noir, 0=Blanc 
  Serial.println(etat); 
  switch (etat){
    case(0):  //0b00000000 
      Serial.println("Gauche=Blanc    Droite=Blanc");
      break;
    case(1):  //0b00000001
      Serial.println("Gauche=Blanc    Droite=Noir");
      break;
    case(2):  //0b00000010
      Serial.println("Gauche=Noir    Droite=Blanc");
      break;
    case(3):  //0b00000011
      Serial.println("Gauche=Noir    Droite=Noir");
      break;
  }
  Serial.println("");
  delay(500);
}
