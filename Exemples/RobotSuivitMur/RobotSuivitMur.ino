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
CapteurSharp MyCapteurG(0x48,1);
CapteurSharp MyCapteurD(0x48,2); 

//Fonction delay bis
void _delay(float seconds) {
  long endTime = millis() + 1000*seconds;
  while(millis() < endTime)_loop();
}


void setup() {
  //Activation des interruptions
  attachInterrupt(Motor2.getIntNum(), isr_process_encoder2, RISING);
  attachInterrupt(Motor3.getIntNum(), isr_process_encoder3, RISING);

  longer_mur('g',120,25);
}

void loop() {delay(1000);}


int dist_IR(char capt)
{
  if (capt == 'g') return MyCapteurG.Distance();
  else if (capt == 'd') return MyCapteurD.Distance();
  return -1;
}


// Cette fonction permet de longer un mur, on privilégiera une régulation sur la valeur brute du capteur qui offre une régulation plus rapide qu'en passant pas des valeurs en cm grâce à la forma caractéristique de la courbe
 void longer_mur(char mur, int vitesse, int distance_avec_mur)
{
  distance_longer  =  2976.4 / (distance_avec_mur + 1.7651);  //renvoie la valeur entre 0 et 1024 que nous devons avoir sur le capteur

  int P,I=0,D;
  float Kp=0.3,Ki=0,Kd=20; //On fixe les paramètres du PID

  int capteur = dist_IR(mur);
  while (capteur < 110) //On avance jusqu'à détecter un mur
  {
    Robot.move('a',vitesse);
    _delay(0.05);
    capteur = dist_IR(mur);
  }

  //Calcul des erreurs
  int capteur_old = capteur;
  int diff_capteur = distance_longer - capteur;
  int diff_capteur_old = diff_capteur;

  while (abs(capteur-capteur_old) < 110 and capteur > 90) //Tant que l'on détecte un mur
  {
    //Calcul des coeff du PID
    P = Kp * diff_capteur;
    I += Ki * diff_capteur/1000;
    D = Kd * (diff_capteur - diff_capteur_old)/10;

    //On applique a consigne
    if (mur == 'g')
      Robot.move((-vitesse + P + I + D),(vitesse + P + I + D));
    else if (mur == 'd')
      Robot.move((-vitesse - P - I - D),(vitesse - P - I - D));

    //Echantillonnage 
    _delay(0.02); 
    //Actualisation des erreurs
    capteur_old = capteur;
    capteur = dist_IR(mur);
    diff_capteur_old = diff_capteur;
    diff_capteur = distance_longer - capteur;
  }
  Robot.stop();
  _delay(1);
}
  
