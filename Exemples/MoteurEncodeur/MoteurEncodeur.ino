#include <Driver_moteur.h>  //Déclaration librairie
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

MoteurEncodeur Motor(2); //Déclaration du moteur sur le Port 2 (port allant de 1 à 4)

////////////////////////////////////////////////////////////////////
//    Fonctions essentielles au bon fonctionnement des moteurs    //
////////////////////////////////////////////////////////////////////

//Une fonction d'interruption par moteur
void isr_process_encoder(void) //Déclaration de l'interruption de l'encodeur du oteur
{
      if(digitalRead(Motor.getPinB()) == 0){
            Motor.pulsePosMinus();  //Le moteur tourne dans le sens inverse, on soustrait la pulsation
      }else{
            Motor.pulsePosPlus(); //Le moteur tourne en avant, on ajoute la pulsation la pulsation
      }
}

void _delay(float milliseconds){  //Fonction de delay bis pour actualiser l'encodeur et le régulateur PID
    long endTime = millis() + milliseconds;
    while(millis() < endTime)_loop();
}

void _loop(){ //Fonction loop bis pour actualiser l'encodeur
    Motor.loop();  
    //Ajouter les autres moteurs  
}


////////////////////
//    Programme   //
////////////////////

void setup() {
  attachInterrupt(Motor.getIntNum(), isr_process_encoder, RISING);  //On attache l'interruption, le moteur est prêt
  //Ajouter les interruptions des autres moteurs

  //Si on utilise un autre moteur que celui fournit (voir datasheet du moteur:
  //  Motor.setRatio(20.4);     //Ratio du moteur
  //  Motor.setMaxRPM(300);     //Vitesse maximale
  //  Motor.setPulse(11);       //Pulsation par tour d'encodeur
  
  //Pour modifier les paramètres des PID par défaut
  //  Motor.setPosPid(1.8,0,1.2);   //Paramètres du PID en position
  //  Motor.setSpeedPid(0.18,0,0);  //Paramètres du PID en vitesse
}

void loop() {
    Motor.setTarPWM(100);         //On active la moteur à la puissance 100 (min=0 max=255)
    delay(1000);                  //Pendant 1s sans PID
    
    Motor.runSpeed(200);          //On demande au moteur d'avancer à la vitesse 200 rpm
    _delay(1000);                  //Pendant 1 seconde avec le PID

    Motor.moveTo(-360,abs(200));  //On demande au moteur de reculer de 360 degrès à la vitesse 200 rpm
    while(not Motor.isTarPosReached()){_loop();} //La fonction n'est pas blocante, on attend donc que le moteur ai atteint sa position

    Motor.stop();                 //Arret du moteur
}
