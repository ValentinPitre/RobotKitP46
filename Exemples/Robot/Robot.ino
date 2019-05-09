#include <Arduino.h>
#include <MyRobot.h>
#include <Wire.h>


// Déclaration moteur 2
MoteurEncodeur Motor2(2);
void isr_process_encoder2(void)
{
  if(digitalRead(Motor2.getPinB()) == 1){
   Motor2.pulsePosMinus();
  }else{
   Motor2.pulsePosPlus();}
}


// Déclaration moteur 3
MoteurEncodeur Motor3(3);
void isr_process_encoder3(void)
{
  if(digitalRead(Motor3.getPinB()) == 1){
   Motor3.pulsePosMinus();
  }else{
   Motor3.pulsePosPlus();}
}


//Actualisation de l'encodeur
void _loop()
{
  Motor2.loop();
  Motor3.loop();
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
  attachInterrupt(Motor2.getIntNum(), isr_process_encoder2, RISING);
  attachInterrupt(Motor3.getIntNum(), isr_process_encoder3, RISING);
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
  Robot.WaitTarPosReached(); //Attendre la fin du mouvement
  Robot.reculerCm(200,10); //Reculer le robot à la vitesse 200 rpm de 10cm
  Robot.WaitTarPosReached(); //Attendre la fin du mouvement
  Robot.gaucheAngle(200,90); //Tourner à gauche le robot à la vitesse 200 rpm et de 90 degrés
  Robot.WaitTarPosReached(); //Attendre la fin du mouvement
  Robot.droiteAngle(200,90); //Tourner à droite le robot à la vitesse 200 rpm et de 90 degrés
  Robot.WaitTarPosReached(); //Attendre la fin du mouvement
  Robot.move(100,-200); //Tourner la roue gauche à 100 rpm et la droite à -200 rpm
  _delay(1);
  Robot.stop();
 }
