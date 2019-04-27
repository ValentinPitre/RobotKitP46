#include "Sensors_kit.h"
#include <Wire.h>
#include <avr/interrupt.h>

//********************************
//		Capteur Ultrasons
//********************************
CapteurUltrasons::CapteurUltrasons(){}
CapteurUltrasons::CapteurUltrasons(int8_t address)
{
	_Address=address;
	_Distance=0;
	_Last_mesurement_time=millis()-DELAY_BETWEEN_MESUREMENT;
	Distance();
}

void CapteurUltrasons::Distance(void)
{
  	//Wire.begin()
  	if ((millis()-_Last_mesurement_time) >= DELAY_BETWEEN_MESUREMENT)
  	{
  		_Distance=0;
	  	while(_Distance==0)
	  	{
	  		Wire.beginTransmission(_Address); // transmit to device 
	  		Wire.write(REQUEST_ULTRASON);        // sends byte
	    	Wire.endTransmission();    // stop transmitting

	  		Wire.requestFrom(_Address, 2); // request 2 byte from slave device address 0x34 
			_Distance = Wire.read();
			_Distance = _Distance << 8 | Wire.read();

			_Last_mesurement_time=millis();
	  	}
  	}
}

float CapteurUltrasons::DistanceCm(void)
{
	Distance();
	return float(_Distance)/10.0;
}

uint16_t CapteurUltrasons::DistanceMm(void)
{
	Distance();
	return _Distance;
}


//********************************
//		Fin de courses
//********************************
FinDeCourse::FinDeCourse(){}
FinDeCourse::FinDeCourse(int8_t address,int8_t number)
{
	_Address=address;
	_Number = number;
	_State=0xff;
	_Last_mesurement_time=millis()-DELAY_BETWEEN_MESUREMENT;

	State();
}

int8_t FinDeCourse::State(void)
{
	//Wire.begin()
  	if ((millis()-_Last_mesurement_time) >= DELAY_BETWEEN_MESUREMENT)
  	{
  		_State=0xff;
	  	while(_State==0xff)
	  	{
	  		Wire.beginTransmission(_Address); // transmit to device 
	  		if (_Number==1)
	  			Wire.write(REQUEST_FINDECOURSE1);        // sends byte
	  		else
	  			Wire.write(REQUEST_FINDECOURSE2);        // sends byte
	    	Wire.endTransmission();    // stop transmitting

	  		Wire.requestFrom(_Address, 1); // request 1 byte from slave device address 0x34 
			_State = Wire.read();
			_Last_mesurement_time=millis();
	  	}
  	}
  	return _State;
}


//********************************
//		Capteur de lignes
//********************************

CapteurDeLignes::CapteurDeLignes(int8_t address)
{
	_Address=address;
	_State=0xff;
	_Last_mesurement_time=millis()-DELAY_BETWEEN_MESUREMENT;

	State();
}

int8_t CapteurDeLignes::State(void)
{
	//Wire.begin()
  	if ((millis()-_Last_mesurement_time) >= DELAY_BETWEEN_MESUREMENT)
  	{
  		_State=0xff;
	  	while(_State==0xff)
	  	{
	  		Wire.beginTransmission(_Address); // transmit to device 
	  		Wire.write(REQUEST_CAPTEURLIGNES);        // sends byte
	    	Wire.endTransmission();    // stop transmitting

	  		Wire.requestFrom(_Address, 1); // request 1 byte from slave device address 0x34 
			_State = Wire.read();
			_Last_mesurement_time=millis();
	  	}
  	}
  	return _State;
}

/////////////////////////////////////////////////


/*
Gyroscope::Gyroscope()
{
	gyro = new L3G();
	gyro->init();
	gyro->enableDefault();
  	gyro->writeReg(0x23, 0b00010000);   // FS = 00 (+/- 2000 dps full scale)

  	init();

}

void Gyroscope::init(void)
{
	for(int i=0;i<20;i++)
 	{
      	gyro->read();
      	_x_offset+=(float)gyro->g.x;
      	_y_offset+=(float)gyro->g.y;
      	_z_offset+=(float)gyro->g.z;
     	delay(10);
  	}
  	_x_offset/=20;
  	_y_offset/=20;
  	_z_offset/=20;

  	_anglex=0;
  	_angley=0;
  	_anglez=0;
}

void Gyroscope::init_interruption(void)
{
	cli(); // Désactive l'interruption globale
	bitClear (TCCR2A, WGM20); // WGM20 = 0
	bitClear (TCCR2A, WGM21); // WGM21 = 0 
	TCCR2B = 0b00000110; // Clock / 256 soit 16 micro-s et WGM22 = 0
	TIMSK2 = 0b00000001; // Interruption locale autorisée par TOIE2
	sei(); // Active l'interruption globale
}


//à appeler toutes les 20 millisecondes
void Gyroscope::Measure(void)
{
	gyro->read();

 	_x=(float)gyro->g.x;
 	if((_x>_x_offset+50) || (_x<_x_offset-50)) //élimination du bruit
	{
		_x=(_x-_x_offset)/55;
		_anglex+=_x/50;
	}

	_y=(float)gyro->g.y;
 	if((_y>_y_offset+50) || (_y<_y_offset-50)) //élimination du bruit
	{
		_y=(_y-_y_offset)/55;
		_angley+=_y/50;
	}

	_z=(float)gyro->g.z;
 	if((_z>_z_offset+50) || (_z<_z_offset-50)) //élimination du bruit
	{
		_z=(_z-_z_offset)/55;
		_anglez+=_z/50;
	}
}

float Gyroscope::angleX(void){return _anglex;}
float Gyroscope::angleY(void){return _angley;}
float Gyroscope::angleZ(void){return _anglez;}

/* UTILISATION DU GYRO 

//Setup Interruption
cli(); // Désactive l'interruption globale
bitClear (TCCR2A, WGM20); // WGM20 = 0
bitClear (TCCR2A, WGM21); // WGM21 = 0 
TCCR2B = 0b00000110; // Clock / 256 soit 16 micro-s et WGM22 = 0
TIMSK2 = 0b00000001; // Interruption locale autorisée par TOIE2
sei(); // Active l'interruption globale

byte varCompteur = 0; // La variable compteur
 
// Routine d'interruption
ISR(TIMER2_OVF_vect) {
  TCNT2 = 256 - 250; // 250 x 16 µS = 4 ms
  if (varCompteur++ > 125) { // 5 * 4 ms = 20 ms
    varCompteur = 0;
    Mygyro.Measure();
  }
}

*/

