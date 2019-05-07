#ifndef Sensor_H
#define Sensor_H

#include <Arduino.h>
#include <stdbool.h>
//#include <L3G.h>
#include <ADS1015.h>


#define REQUEST_FINDECOURSE1 0x01
#define REQUEST_FINDECOURSE2 0x02
#define REQUEST_ULTRASONIC_MM 0x03
#define REQUEST_ULTRASONIC_CM 0x04
#define REQUEST_LINESENSOR 0x05


//********************************
//    Capteur Ultrasons
//********************************

class CapteurUltrasons
{
public:
  //Constructeur
  CapteurUltrasons();
  CapteurUltrasons(uint8_t address);

  uint8_t DistanceCm(void);
  uint8_t DistanceMm(void);
private:
  int8_t _Address;
  uint8_t _DistanceCm;
  uint8_t _DistanceMm;
};


//********************************
//    Fin de courses
//********************************

class FinDeCourse
{
public:
  //Constructeur
  FinDeCourse();
  FinDeCourse(uint8_t address,int8_t number);
  int8_t State(void);
private:
  int8_t _Address;
  int8_t _State;
  int8_t _Number;
};

//********************************
//    Capteur de lignes
//********************************

class CapteurDeLignes
{
public:
  //Constructeur
  CapteurDeLignes();
  CapteurDeLignes(uint8_t address);
  int8_t State(void);
private:
  int8_t _Address;
  uint8_t _Number;
  int8_t _State;
};

//********************************
//    Capteur de distance SHARP
//********************************
class CapteurSharp
{
public:
  //Constructeur
  CapteurSharp();
  CapteurSharp(uint8_t address, int8_t number);
  ~CapteurSharp();
  float DistanceCm(void);
  float DistanceMm(void);
private:
  ADS1015* _Sharp;
  int8_t _Address;
  uint8_t _Number;
  float _Distance;
};


/*
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
*/

#endif