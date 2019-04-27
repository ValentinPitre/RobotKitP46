#include "MoteursDC.h"

#include <Arduino.h>
#include <stdbool.h>

DC_port_type Struct_Port_DC[8] =
{
  {11,33,32},{12,35,34},{7,40,41},{8,36,37},{6,47,48},{9,42,43}, {4,A3,A2}, {5,A5,A4}
};

//**************************
//		MoteurDC 
//**************************

MotorDC::MotorDC(){}
MotorDC::MotorDC(uint8_t port)
{
	_Port = port;
	_PIN_PWM = Struct_Port_DC[port].pin_PWM;
	_PIN_H1 = Struct_Port_DC[port].pin_H1;
	_PIN_H2 = Struct_Port_DC[port].pin_H2;

	pinMode(_PIN_H1, OUTPUT);
	pinMode(_PIN_H2, OUTPUT);

	stop();
}


void MotorDC::run(int pwm)
{
	pwm=constrain(pwm,-255,255);
	Serial.println(pwm);
	if (pwm<0)
	{
	    digitalWrite(_PIN_H1, LOW);	//Changement de sens du pont en H
	    delayMicroseconds(5);
	    digitalWrite(_PIN_H2, HIGH);
	    analogWrite(_PIN_PWM, abs(pwm)); //Activation PWM pour réguler la vitesse
	} 
	else if (pwm>0)
	{
	    digitalWrite(_PIN_H1, HIGH);	//Changement de sens du pont en H
	    delayMicroseconds(5);
	    digitalWrite(_PIN_H2, LOW);
	    analogWrite(_PIN_PWM, abs(pwm)); //Activation PWM pour réguler la vitesse
	}
	else
	{
		digitalWrite(_PIN_H1,LOW);	//Changement de sens du pont en H
	    delayMicroseconds(5);
	    digitalWrite(_PIN_H2, LOW);
	    analogWrite(_PIN_PWM, 0);
	}
}

void MotorDC::stop(void)
{
	run(0);
}

//**************************
//		Pince 
//**************************

Pince::Pince(){}
Pince::Pince(uint8_t port):MotorDC(port){}

void Pince::open(int pwm)
{
	run(-pwm);
}
void Pince::close(int pwm)
{
	run(pwm);
}

//**************************
//		Bras
//**************************
Bras::Bras(){}
Bras::Bras(uint8_t port):MotorDC(port){}

void Bras::up(int pwm)
{
	run(pwm);
}
void Bras::down(int pwm)
{
	run(-pwm);
}