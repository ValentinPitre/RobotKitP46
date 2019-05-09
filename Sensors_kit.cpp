#include <Sensors_kit.h>
#include <Wire.h>
#include <avr/interrupt.h>

//********************************
//		Capteur Ultrasons
//********************************
CapteurUltrasons::CapteurUltrasons(){}
CapteurUltrasons::CapteurUltrasons(uint8_t address)
{
	_Address=address;
	_DistanceCm=0xff;
	_DistanceMm=0xff;
	Wire.begin();
}

uint8_t CapteurUltrasons::DistanceMm(void)
{
  	//Wire.begin()
	Wire.beginTransmission(_Address); // transmit to device 
	Wire.write(REQUEST_ULTRASONIC_MM);        // sends byte
	Wire.endTransmission();    // stop transmitting

	Wire.requestFrom(_Address, 1); // request 2 byte from slave device address 0x34 
	if(Wire.available())
   		_DistanceMm = Wire.read();
	else
		return 0xff;

  	return _DistanceMm;
}

uint8_t CapteurUltrasons::DistanceCm(void)
{
  	//Wire.begin()
	Wire.beginTransmission(_Address); // transmit to device 
	Wire.write(REQUEST_ULTRASONIC_CM);        // sends byte
	Wire.endTransmission();    // stop transmitting

	Wire.requestFrom(_Address, 1); // request 2 byte from slave device address 0x34 
	if(Wire.available())
   		_DistanceCm = Wire.read();
	else
		return 0xff;

  	return _DistanceCm;
}



//********************************
//		Fin de courses
//********************************
FinDeCourse::FinDeCourse(){}
FinDeCourse::FinDeCourse(uint8_t address,int8_t number)
{
	_Address=address;
	_Number = number;
	_State=0xff;
	Wire.begin();
}

int8_t FinDeCourse::State(void)
{
	//Wire.begin()
	Wire.beginTransmission(_Address); // transmit to device 
	if (_Number==1)
	 	Wire.write(REQUEST_FINDECOURSE1);        // sends byte
	else
		Wire.write(REQUEST_FINDECOURSE2);        // sends byte
	Wire.endTransmission();    // stop transmitting

	Wire.requestFrom(_Address, 1); // request 1 byte from slave device address 0x34 
	if(Wire.available())
		_State = Wire.read();
	else
		return 0xff;
  	
  	return _State;
}


//********************************
//		Capteur de lignes
//********************************
CapteurDeLignes::CapteurDeLignes(){};
CapteurDeLignes::CapteurDeLignes(uint8_t address)
{
	_Address=address;
	_State=0xff;
	Wire.begin();
}

int8_t CapteurDeLignes::State(void)
{
	//Wire.begin()
	Wire.beginTransmission(_Address); // transmit to device 
	Wire.write(REQUEST_LINESENSOR);        // sends byte
	Wire.endTransmission();    // stop transmitting

	Wire.requestFrom(_Address, 1); // request 1 byte from slave device address 0x34 
	if(Wire.available())
		_State = Wire.read();
	else
		return 0xff;
  	return _State;
}


//********************************
//		Capteur de distance Sharp
//********************************
CapteurSharp::CapteurSharp(){};
CapteurSharp::CapteurSharp(uint8_t address,int8_t number)
{
	_Address=address;
	_Distance=0;
	_Number = number;

	_Sharp = new ADS1015(_Address);
	_Sharp->begin();
	_Sharp->setGain(GAIN_TWOTHIRDS);

}

CapteurSharp::~CapteurSharp(void)
{
	delete _Sharp;
}

int CapteurSharp::Distance(void)
{
	_Distance= _Sharp->readADC_SingleEnded(_Number-1);
  	_Distance= _Distance*3;
  	_Distance= map(_Distance,0,5000,0,1024);
  	return _Distance;
}
float CapteurSharp::DistanceCm(void)
{
	_Distance= _Sharp->readADC_SingleEnded(_Number-1);
  	_Distance= _Distance*3;
  	_Distance= map(_Distance,0,5000,0,1024);
  	_Distance= 2970.4* (1.0/_Distance) - 1.7651;
  	return _Distance;
}

float CapteurSharp::DistanceMm(void)
{
  	return DistanceCm()*10;
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

