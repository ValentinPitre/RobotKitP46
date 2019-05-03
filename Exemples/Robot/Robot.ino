#include <Arduino.h>
#include <MyRobot.h>
#include <Wire.h>

// Déclaration moteur 1
MoteurEncodeur Motor1(Port1);
void isr_process_encoder1(void)
{
  if(digitalRead(Motor1.getPinB()) == 0){
    Motor1.pulsePosMinus();
  }else{
   Motor1.pulsePosPlus();}
}


// Déclaration moteur 2
MoteurEncodeur Motor2(Port2);
void isr_process_encoder2(void)
{
  if(digitalRead(Motor2.getPinB()) == 0){
   Motor2.pulsePosMinus();
  }else{
   Motor2.pulsePosPlus();}
}


// Déclaration moteur 3
MoteurEncodeur Motor3(Port3);
void isr_process_encoder3(void)
{
  if(digitalRead(Motor3.getPinB()) == 0){
   Motor3.pulsePosMinus();
  }else{
   Motor3.pulsePosPlus();}
}


// Déclaration moteur 4
MoteurEncodeur Motor4(Port4);
void isr_process_encoder4(void)
{
  if(digitalRead(Motor4.getPinB()) == 0){
   Motor4.pulsePosMinus();
  }else{
   Motor4.pulsePosPlus();}
}

//Actualisation de l'encodeur
void _loop()
{
  Motor1.loop();
  Motor2.loop();
  Motor3.loop();
  Motor4.loop();
}

//Déclaration du robot
MyRobot Robot(&Motor2,&Motor3,6.1/*Diametre roue*/,20.5/*Largeur entre les roues*/);

//Fonction delay bis
void _delay(float seconds) {
  long endTime = millis() + 1000*seconds;
  while(millis() < endTime)_loop();
}


void setup() {
  //Activation des interruptions
  attachInterrupt(Motor1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Motor2.getIntNum(), isr_process_encoder2, RISING);
  attachInterrupt(Motor3.getIntNum(), isr_process_encoder3, RISING);
  attachInterrupt(Motor4.getIntNum(), isr_process_encoder4, RISING);
}

void loop() {
  Robot.avancer(200); //Avancer le robot à la vitesse 200 rpm
  _delay(1);
  Robot.reculer(200); //Reculer le robot à la vitesse 200 rpm
  _delay(1);
  Robot.gauche(200);  //Tourner à gauche le robot à la vitesse 200 rpm
  _delay(1);
  Robot.droite(200);  //Tourner à droite le robot à la vitesse 200 rpm
  _delay(1);
  Robot.stop();       //Arreter le robot

  
  Robot.avancerCm(200,10);  //Avancer le robot à la vitesse 200 rpm de 10cm
  while ((Motor3.isTarPosReached() && Motor2.isTarPosReached()) == true) {
    _delay(0.1);
  }
  Robot.reculerCm(200,10); //Reculer le robot à la vitesse 200 rpm de 10cm
  while ((Motor3.isTarPosReached() && Motor2.isTarPosReached()) == true) {
    _delay(0.1);
  }
  Robot.gaucheAngle(200,90); //Tourner à gauche le robot à la vitesse 200 rpm et de 90 degrés
  while ((Motor3.isTarPosReached() && Motor2.isTarPosReached()) == true) {
    _delay(0.1);
  }
  Robot.droiteAngle(200,90); //Tourner à droite le robot à la vitesse 200 rpm et de 90 degrés
  while ((Motor3.isTarPosReached() && Motor2.isTarPosReached()) == true) {
    _delay(0.1);
  }
  Robot.move(100,-200); //Tourner la roue gauche à 100 rpm et la droite à -200 rpm
  _delay(1);
  Robot.stop();
 }
