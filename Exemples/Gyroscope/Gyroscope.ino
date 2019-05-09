// Connection capteur sur  Arduino
//    SCL => SCL (A5 sur UNO) (21 sur MEGA)
//    SDA => SDA (A4 sur UNO) (20 sur MEGA)
//    GND => GND
//    CDD => 5V

//Ce code est le code de test du gyroscope. 
//Il utilise la librairie du constructeur à laquelle nous rajoutons la partie traitement des données
//La librairie nous renvoie l'accélération et nous en calculons l'angle
//Mais les résultats sont peu précis et nécessitent un programme d'interruption sur le timer2  (mais incomatible avec la librairie Wire qui utilise la même ressource) ou l'utilisation du pin INT1 du capteur
//N'étant donc pas compatible avec notre kit, nous écartons ce capteur

#include <Arduino.h>
#include <Wire.h>
#include "Sensors_kit.h"
#include "L3G.h"
L3G gyro;

int  _x_offset=0;
int  _anglex=0;
int  _x=0;
void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }
  gyro.enableDefault();
  gyro.writeReg(0x23, 0b00010000);   // FS = 00 (+/- 2000 dps full scale)

  for(int i=0;i<20;i++)
  {
        gyro.read();
        _x_offset+=(float)gyro.g.x;
      delay(10);
  }
  _x_offset/=20;
  _anglex=0;

}

void loop() {
  gyro.read();

  _x=(float)gyro.g.x;
  if((_x>_x_offset+50) || (_x<_x_offset-50)) //élimination du bruit
  {
    _x=(_x-_x_offset)/55;
    _anglex+=_x/50;
  }

  Serial.print("Angle x: ");
  Serial.println(_anglex);
  delay(20);
}
/*
//Le code censé fonctionner avec la librairie

Gyroscope MyGyro;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Start...");
  MyGyro.init();
}

void loop() {
  Serial.print("Angle X: ");
  Serial.println(MyGyro.angleX());
  Serial.print("Angle Y: ");
  Serial.println(MyGyro.angleY());
  Serial.print("Angle Z: ");
  Serial.println(MyGyro.angleZ());
  delay(20);
  MyGyro.Measure();
}*/
