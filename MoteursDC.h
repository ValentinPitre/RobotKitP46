#ifndef Moteur_DC_H
#define Moteur_DC_H

#include <Arduino.h>
#include <stdbool.h>
#include <math.h>

#define Port1A 0x00
#define Port1B 0x01
#define Port2A 0x02
#define Port2B 0x03
#define Port3A 0x04
#define Port3B 0x05
#define Port4A 0x06
#define Port4B 0x07

typedef struct
{
  uint8_t pin_PWM; 
  uint8_t pin_H1;
  uint8_t pin_H2;
} DC_port_type;



class MotorDC
{
public:
  //Constructeur
	MotorDC();
	MotorDC(uint8_t port);
	void run(int pwm);
	virtual void stop(void);
protected:
;
	uint8_t _Slot;
	uint8_t _Port;
	uint8_t _PIN_PWM; 
	uint8_t _PIN_H1;
	uint8_t _PIN_H2;
};

class Pince: public MotorDC
{
public:
  //Constructeur
  Pince();
  Pince(uint8_t port);
  void open(int pwm=200);
  void close(int pwm=150);
};

class Bras: public MotorDC
{
public:
  //Constructeur
  Bras();
  Bras(uint8_t port);
  virtual void stop();
  void up(int pwm=200);
  void down(int pwm=100);
  uint8_t getState(void);
private:
  uint8_t _State;
};
#endif

