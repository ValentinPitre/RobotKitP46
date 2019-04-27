// Connection capteur sur  Arduino
//    SCL => SCL (A5 sur UNO) (21 sur MEGA)
//    SDA => SDA (A4 sur UNO) (20 sur MEGA)
//    GND => GND
//    CDD => 5V
#include <Arduino.h>
#include <Wire.h>
#include "Sensors_kit.h"

Gyroscope MyGyro;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Start...");
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
}
