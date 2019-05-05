//Programme permettant de communiquer avec les cartes des capteurs I2C pour lire les données des capteurs
//Destiné à l'arduino en tant que Master


#include <Wire.h>
#define I2C_SLAVE 0x34  //Mettre l'adresse de la carte ICI

char data = 0xff; //Default data

void setup()
{
 Wire.begin(); // join i2c bus (address optional for master)
 Serial.begin(9600); // start serial
 Serial.println("go");
}

void loop()
{
  if (Serial.available()) //On attend la consigne de l'utilisateur (chiffre entre 1 et 5)
  {
    data = Serial.read()- '0';
  }
  Wire.beginTransmission(I2C_SLAVE); // transmit to device
  Wire.write(data);              // sends request command
  Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(I2C_SLAVE, 1); // request 1 byte from slave device address 0x34 
  if(Wire.available()) // slave may send less than requested
   {
   byte i = Wire.read(); // receive a byte
   Serial.println(i,HEX); // print the byte
   } 
  
delay(50);
}
