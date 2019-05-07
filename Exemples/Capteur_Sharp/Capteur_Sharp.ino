#include <Arduino.h>
#include <Wire.h>
#include <Sensors_kit.h>

CapteurSharp MyCapteur1(0x48,1); //Déclaration du capteur adress 0x48 port 1
CapteurSharp MyCapteur2(0x48,2); 
/*
	Tips on address: the address of ADS1015 can be changed by connecting ADDR pin to different PINs:
	- ADDR to Gnd: address = 0b01001000 = 0x48 = 72
	- ADDR to VDD: address = 0b01001001 = 0x49 = 73
	- ADDR to SDA: address = 0b01001010 = 0x4A = 74
	- ADDR to SCL: address = 0b01001011 = 0x4B = 75
*/


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Start...");
}

void loop() {
  Serial.print("Distance Sharp1 en cm: ");
  Serial.println(MyCapteur1.DistanceCm());
  Serial.print("Distance Sharp2 en mm: ");
  Serial.println(MyCapteur2.DistanceMm());
  Serial.println("");
  delay(500);
}
