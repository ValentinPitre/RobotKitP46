#ifndef MyRobot_H
#define MyRobot_H


#include <Arduino.h>
#include "MoteursDC.h"
#include "Sensors_kit.h"

#include "Driver_moteur.h"


class MyRobot
{
public:
  //Constructeur
  MyRobot(uint8_t portMotG,uint8_t portMotD, float diametre, float largeur);
  MyRobot(MoteurEncodeur* MotG,MoteurEncodeur* MotD, float diametre, float largeur);
  ~MyRobot(void);
  void setMoteurGauche(uint8_t port);
  void setMoteurGauche(MoteurEncodeur* moteur);
  void setMoteurDroite(uint8_t port);
  void setMoteurDroite(MoteurEncodeur* moteur);
  void setCapteurBrasBas(uint8_t address, uint8_t numb);
  void setCapteurBrasHaut(uint8_t address, uint8_t numb);
  void setBras(uint8_t Port);
  void setPince(uint8_t Port);
  void setDiametreRoues(float diametre);
  void setLargeurRobot(float diametre);

  void avancer(int speed);
  void gauche(int speed);
  void droite(int speed);
  void reculer(int speed);
  void stop(void);

  void avancerCm(int speed, float distance);
  void gaucheAngle(int speed, int angle);
  void droiteAngle(int speed, int angle);
  void reculerCm(int speed, float distance);

  void move(int speed_g, int speed_d);
  void moveDegres(int direction,long degres, int speed);
  void WaitTarPosReached(void);

  void BrasHaut(int pwm=150);
  void BrasBas(int pwm=100);

  void OuvrirPince(int pwm=150);
  void FermerPince(int pwm=100);

  void Bluetooth(void);

private:

	int _Diametre_roue;
	int _Largeur_Robot;
 
	MoteurEncodeur* Moteur_g;
	MoteurEncodeur* Moteur_d;

	FinDeCourse* FdcHaut;
	FinDeCourse* FdcBas;

	Pince* MyPince;
	Bras* MyBras;
};




#endif