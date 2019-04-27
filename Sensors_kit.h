#ifndef Sensor_H
#define Sensor_H

#include <Arduino.h>
#include <stdbool.h>
#include <L3G.h>
#define DELAY_BETWEEN_MESUREMENT   10

#define REQUEST_FINDECOURSE1 0x01
#define REQUEST_FINDECOURSE2 0x02
#define REQUEST_ULTRASON 0x03
#define REQUEST_CAPTEURLIGNES 0x04


//********************************
//    Capteur Ultrasons
//********************************

class CapteurUltrasons
{
public:
  //Constructeur
  CapteurUltrasons();
  CapteurUltrasons(int8_t address);

  void Distance(void);
  float DistanceCm(void);
  uint16_t DistanceMm(void);
private:
  int8_t _Address;
  uint16_t _Distance;
  long _Last_mesurement_time;
};


//********************************
//    Fin de courses
//********************************

class FinDeCourse
{
public:
  //Constructeur
  FinDeCourse();
  FinDeCourse(int8_t address,int8_t number);
  int8_t State(void);
private:
  int8_t _Address;
  int8_t _State;
  int8_t _Number;
  long _Last_mesurement_time;
};

//********************************
//    Capteur de lignes
//********************************

class CapteurDeLignes
{
public:
  //Constructeur
  CapteurDeLignes();
  CapteurDeLignes(int8_t address);
  int8_t State(void);
private:
  int8_t _Address;
  uint8_t _Number;
  int8_t _State;
  long _Last_mesurement_time;
};

//********************************
//    Capteur de distance SHARP
//********************************

//********************************
//    Gyroscope
//********************************
class Gyroscope
{
public:
  //Constructeur
  Gyroscope(void);
  void init(void);
  void init_interruption(void);
  void Measure(void);
  float angleX(void);
  float angleY(void);
  float angleZ(void);

private:
  L3G *gyro;
  float _x;
  float _x_offset;
  float _anglex;
  float _y;
  float _y_offset;
  float _angley;
  float _z;
  float _z_offset;
  float _anglez;

};

#endif