#include "MyRobot.h"

#include <math.h>
#include <Arduino.h>

MyRobot::MyRobot(MoteurEncodeur* MotG,MoteurEncodeur* MotD, float diametre, float largeur){
	_Diametre_roue=diametre;
	_Largeur_Robot=largeur;

	Moteur_g= MotG;
	Moteur_d= MotD;

	FdcHaut=NULL;
	FdcBas=NULL;

	MyPince=NULL;
	MyBras=NULL;
	Serial.begin(9600);
	Serial3.begin(9600);
}

MyRobot::MyRobot(uint8_t portMotG,uint8_t portMotD, float diametre, float largeur){
	_Diametre_roue=diametre;
	_Largeur_Robot=largeur;

	Moteur_g= new MoteurEncodeur(portMotG);
	Moteur_d= new MoteurEncodeur(portMotD);

	FdcHaut=NULL;
	FdcBas=NULL;

	MyPince=NULL;
	MyBras=NULL;
	Serial.begin(9600);
	Serial3.begin(9600);
}
MyRobot::~MyRobot()
{
	delete Moteur_d;
	delete Moteur_g;

	delete FdcHaut;
	delete FdcBas;

	delete MyPince;
	delete MyBras;
}

void MyRobot::setMoteurGauche(uint8_t port)
{
	if (Moteur_g == NULL)
		delete Moteur_g;
	Moteur_g= new MoteurEncodeur(port);
}
void MyRobot::setMoteurGauche(MoteurEncodeur* moteur)
{
	if (Moteur_g == NULL)
		delete Moteur_g;
	Moteur_g= moteur;
}

void MyRobot::setMoteurDroite(uint8_t port)
{
	if (Moteur_d == NULL)
		delete Moteur_d;
	Moteur_d=new MoteurEncodeur(port);
}
void MyRobot::setMoteurDroite(MoteurEncodeur* moteur)
{
	if (Moteur_d == NULL)
		delete Moteur_d;
	Moteur_d=moteur;
}


void MyRobot::setCapteurBrasBas(uint8_t address,uint8_t  numb)
{
	FdcBas = new FinDeCourse(address,numb);
}
void MyRobot::setCapteurBrasHaut(uint8_t address, uint8_t numb)
{
	FdcHaut = new FinDeCourse(address,numb);
}
void MyRobot::setBras(uint8_t Port)
{
	MyBras = new Bras(Port);
}
void MyRobot::setPince(uint8_t Port)
{
	MyPince = new Pince(Port);
}
void MyRobot::setDiametreRoues(float diametre)
{
	_Diametre_roue=diametre;
}
void MyRobot::setLargeurRobot(float largeur)
{
	_Largeur_Robot=largeur;
}

void MyRobot::avancer(int speed){	move(speed,-speed);	}
void MyRobot::gauche(int speed){	move(speed,speed);	}
void MyRobot::droite(int speed){	move(-speed,-speed);	}
void MyRobot::reculer(int speed){	move(-speed,speed);	}
void MyRobot::stop(void)
{
	Moteur_g->stop();
    Moteur_d->stop();
}

void MyRobot::avancerCm(int speed, float distance)
{
	float angle = distance * 360 / (_Diametre_roue * PI); 
	moveDegres('a',angle,speed);
}

void MyRobot::gaucheAngle(int speed, int angle)
{
	float dist = angle*(PI/180) * _Largeur_Robot/2;
	float angle_moteur = dist * 360 / (_Diametre_roue * PI); 
	moveDegres('g',angle_moteur,speed);
}

void MyRobot::droiteAngle(int speed, int angle)
{
	float dist = angle*(PI/180) * _Largeur_Robot/2;
	float angle_moteur = dist * 360 / (_Diametre_roue * PI); 
	moveDegres('d',angle_moteur,speed);
}

void MyRobot::reculerCm(int speed, float distance)
{
	float angle = distance * 360 / (_Diametre_roue * PI); 
	moveDegres('r',angle,speed);
}

void MyRobot::move(int speed_g, int speed_d)
{
      Moteur_g->runSpeed(speed_g);
      Moteur_d->runSpeed(speed_d);
}
void MyRobot::moveDegres(int direction,long degres, int speed)
{
    speed = abs(speed);
    if(direction == 'a')
    {
        Moteur_g->moveTo(degres,(float)speed);
        Moteur_d->moveTo(-degres,(float)speed);
    }
    else if(direction == 'r')
    {
        Moteur_g->moveTo(-degres,(float)speed);
        Moteur_d->moveTo(degres,(float)speed);
    }
    else if(direction == 'g')
    {
        Moteur_g->moveTo(degres,(float)speed);
        Moteur_d->moveTo(degres,(float)speed);
    }
    else if(direction == 'd')
    {
        Moteur_g->moveTo(-degres,(float)speed);
        Moteur_d->moveTo(-degres,(float)speed);
    }

    while(not (Moteur_g->isTarPosReached() && Moteur_d->isTarPosReached()))
    {
     	Moteur_g->loop();    
    	Moteur_d->loop();
    }
    stop();

    long endTime = millis() + 100;
    while(millis() < endTime)
    {
    	Moteur_g->loop();    
    	Moteur_d->loop();
    }
    
}

  void MyRobot::BrasHaut(int pwm)
  {
  	if(MyBras!=NULL && FdcHaut!=NULL)
  	{
	  	MyBras->up(pwm);
	  	while( not(FdcHaut->State()==1))
	  	{
	  		delay(20);
	  	}
	  	MyBras->stop();
	}
  }
  void MyRobot::BrasBas(int pwm)
  {
  	if(MyBras!=NULL && FdcBas!=NULL)
  	{
	  	MyBras->down(pwm);
	  	while( not(FdcBas->State()==1))
	  	{
	  		delay(20);
	  	}
	  	MyBras->stop();
	}
  }

  void MyRobot::OuvrirPince(int pwm=150)
  {
  	MyPince->open(pwm);
  }
  void MyRobot::FermerPince(int pwm=100)
  {
  	MyPince->close(pwm);
  }

  void MyRobot::Bluetooth(void)
  {
  	if(Serial3.available())
  	{
  		uint8_t octet= Serial3.read();

  		Serial.write(octet);
  		uint8_t commande = octet & 0x0f;
  		int vitesse = (int)(255.0*(octet >> 4)/10);

  		switch(commande){
  			case 1:
  				stop();
  				MyPince->stop();
  				MyBras->stop();
  				break;
   			case 2:
   				avancer(vitesse);
  				break;
   			case 3:
   				reculer(vitesse);
  				break;
  			case 4:
  				gauche(vitesse);
  				break;
  			case 5:
  				droite(vitesse);
  				break;
  			case 6:
  				if(MyBras!=NULL)
  					MyBras->up();
  				break;
   			case 7:
   				if(MyBras!=NULL)
   					MyBras->down();
  				break;
  			case 8:
  				if(MyPince!=NULL)
  					MyPince->open();
  				break;
   			case 9:
   				if(MyPince!=NULL)
   					MyPince->close();
  				break;
  		}
  	}
  }