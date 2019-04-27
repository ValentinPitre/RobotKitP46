#include "Driver_moteur.h"
#include <Arduino.h>
#include <stdbool.h>
//Maping de la carte
Encoder_port_type encoder_Port[4] =
{
  { 18,     31,     12,     34,     35},
  { 19,     38,     8,      37,     36},
  { 3,      49,     9,      43,     42},
  { 2,      A1,     5,      A4,     A5}
};

//Constructeur
MoteurEncodeur::MoteurEncodeur(){}
MoteurEncodeur::MoteurEncodeur(uint8_t slot)
{
  _Slot = slot;
  _PIN_A = encoder_Port[slot-1].pin_A;
  _PIN_B = encoder_Port[slot-1].pin_B;
  _PIN_PWM = encoder_Port[slot-1].pin_PWM;
  _PIN_H1 = encoder_Port[slot-1].pin_H1;
  _PIN_H2 = encoder_Port[slot-1].pin_H2;

  uint8_t pin_int[4] = {5,4,1,0};
  _IntNum = pin_int[slot-1];

  pinMode(_PIN_A, INPUT_PULLUP);
  pinMode(_PIN_B, INPUT_PULLUP);
  pinMode(_PIN_H1, OUTPUT);
  pinMode(_PIN_H2, OUTPUT);

  //Réglage PWM 8kHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  encodeur.pulsePos = 0;
  encodeur.previousPwm = 500;
  encodeur.mode = DIRECT_MODE;
  encodeur.pulseEncoder = 11;
  encodeur.ratio = 20.4;
  encodeur.precision = 360/ (encodeur.ratio * encodeur.pulseEncoder);
  encodeur.maxRPM = 300;

  encodeur.PID_pos.P = 1.8;
  encodeur.PID_pos.I = 0;
  encodeur.PID_pos.D = 1.2;

  encodeur.PID_speed.P = 0.18;
  encodeur.PID_speed.I = 0;
  encodeur.PID_speed.D = 0;

  setMotorPwm(0);
  setPulsePos(0);
  _Last_speed_mesurement_time = millis();
}


//***************************************************
//			Lecture des attributs
//***************************************************
uint8_t MoteurEncodeur::getPortNum(void){  return _Slot;}
uint8_t MoteurEncodeur::getIntNum(void){  return _IntNum;}
uint8_t MoteurEncodeur::getPinA(void){  return _PIN_A;}
uint8_t MoteurEncodeur::getPinB(void){  return _PIN_B;}

long MoteurEncodeur::getPulsePos(void){  return encodeur.pulsePos;}
float MoteurEncodeur::getCurrentrentSpeed(void){  return encodeur.currentSpeed;}
long MoteurEncodeur::getCurrentPos(void){  return encodeur.currentPos;}
int16_t MoteurEncodeur::getCurrentPwm(void){  return encodeur.currentPwm;}


//***************************************************
//			Paramètres du moteur
//***************************************************
void MoteurEncodeur::setRatio(int16_t RatioValue)
{  
  encodeur.ratio = RatioValue;
  encodeur.precision = 360/ (encodeur.ratio * encodeur.pulseEncoder);
}
void MoteurEncodeur::setMaxRPM(int16_t MaxRPM){  encodeur.maxRPM = MaxRPM;}


//***************************************************
//			Opération sur la position du moteur 
//***************************************************
void MoteurEncodeur::setPulse(int16_t pulseValue)
{
  encodeur.pulseEncoder = pulseValue;
  encodeur.precision = 360/ (encodeur.ratio * encodeur.pulseEncoder);
}
void MoteurEncodeur::setPulsePos(long pulsePos){  encodeur.pulsePos = pulsePos;}
void MoteurEncodeur::pulsePosPlus(void){  encodeur.pulsePos++;}
void MoteurEncodeur::pulsePosMinus(void){  encodeur.pulsePos--;}
void MoteurEncodeur::updateCurPos(void){  encodeur.currentPos = (long)((encodeur.pulsePos/(encodeur.pulseEncoder * encodeur.ratio)) * 360);}

//***************************************************
//			Opération sur la vitesse du moteur
//***************************************************
void MoteurEncodeur::setCurrentSpeed(float speed)
{  
  speed = constrain(speed,-encodeur.maxRPM,encodeur.maxRPM); //Limitation des valeurs
  encodeur.currentSpeed = speed;
}


void MoteurEncodeur::updateSpeed(void)
{
  if((millis() - _Last_speed_mesurement_time) > SAMPLING_TIME/2) //On échantillonne
  {
    uint16_t dt = millis() - _Last_speed_mesurement_time;
    long cur_pos = getPulsePos();
    setCurrentSpeed(((cur_pos - encodeur.previousPos) *(1000.0/dt)*60.0) /(encodeur.pulseEncoder * encodeur.ratio));
    encodeur.previousPos = cur_pos;
    _Last_speed_mesurement_time = millis();
  }
}


void MoteurEncodeur::runSpeed(float speed)
{
  encodeur.mode = PID_MODE;
  encodeur.motionState = SPEED_ENSLAVEMENT;
  encodeur.targetSpeed = constrain(speed,-encodeur.maxRPM,encodeur.maxRPM);
  _Lock_flag = false;
}


void MoteurEncodeur::setSpeed(float speed)
{
  encodeur.motionState = SPEED_ENSLAVEMENT;
  encodeur.targetSpeed = constrain(speed,-encodeur.maxRPM,encodeur.maxRPM);
  _Lock_flag = false;
}



//***************************************************
//			Opération sur pwm du moteur
//***************************************************
void MoteurEncodeur::setTarPWM(int16_t pwm_value)
{
  encodeur.mode = PWM_MODE;
  encodeur.targetPwm = constrain(pwm_value,-255,255);;
}


void MoteurEncodeur::setMotorPwm(int16_t pwm)
{
  pwm = constrain(pwm,-255,255);
  if(encodeur.previousPwm != pwm)
    encodeur.previousPwm = pwm;
  else
    return;
    
  encodeur.currentPwm = pwm;

	//En fonction du sens de rotation
  if(pwm < 0)		//En arrière
  {
    digitalWrite(MoteurEncodeur::_PIN_H1, LOW);	//Changement de sens du pont en H
    delayMicroseconds(5);
    digitalWrite(MoteurEncodeur::_PIN_H2, HIGH);
    analogWrite(MoteurEncodeur::_PIN_PWM, abs(pwm)); //Activation PWM pour réguler la vitesse
  }
  else				//En avant
  {
    digitalWrite(MoteurEncodeur::_PIN_H1, HIGH); //Changement de sens du pont en H
    delayMicroseconds(5);
    digitalWrite(MoteurEncodeur::_PIN_H2, LOW);
    analogWrite(MoteurEncodeur::_PIN_PWM, abs(pwm)); //Activation PWM pour réguler la vitesse
  }
}
 
  void MoteurEncodeur::stop()
  {
  	setTarPWM(0);
  	loop();
  }
//***************************************************
//			Choix du mode de régulation
//***************************************************

void MoteurEncodeur::setMotionMode(int16_t motionMode){  encodeur.mode = motionMode;}

void MoteurEncodeur::pwmMove(void)
{
  if(millis() - _Last_position_mesurement_time > SAMPLING_TIME)	//Echantillonnage
  {
    _Last_position_mesurement_time = millis();
    encodeur.currentPwm = round(0.8 * encodeur.currentPwm + 0.2 * encodeur.targetPwm); //Augmentation progressive
    if((abs(encodeur.currentPwm) <= 10) && (encodeur.targetPwm == 0)) //résolution problème pour consigne à 0
      encodeur.currentPwm = 0;
  }
  setMotorPwm(encodeur.currentPwm);
}

void MoteurEncodeur::encoderMove(void) 
{
  if(millis() - _Last_position_mesurement_time > SAMPLING_TIME) //Echantillonnage
  {
    int16_t pwm_encoder = 0;
    _Last_position_mesurement_time = millis();

    if(encodeur.motionState == POSITION_ENSLAVEMENT)
      pwm_encoder = pidPositionToPwm();
    else if(encodeur.motionState == SPEED_ENSLAVEMENT)
      pwm_encoder = speedWithoutPos();

    encodeur.currentPwm = pwm_encoder;
  }
  setMotorPwm(encodeur.currentPwm);
}






//***************************************************
//			Régulation en vitesse PID
//***************************************************
void MoteurEncodeur::setSpeedPid(float p,float i,float d)
{
  encodeur.PID_speed.P = p;
  encodeur.PID_speed.I = i;
  encodeur.PID_speed.D = d;
}

int16_t MoteurEncodeur::speedWithoutPos(void)
{
  if(_Lock_flag == true)
    encodeur.PID_speed.Output = 0;
  else
  {
	  float speed_error=encodeur.currentSpeed - encodeur.targetSpeed;
	  float speed_command=encodeur.PID_speed.P *speed_error;

	  speed_command = constrain(speed_command,-25,25);
	  encodeur.PID_speed.Output = constrain(_Encoder_output - speed_command,-255,255);

	  if((_Lock_flag == false) && (encodeur.targetSpeed == 0) && (abs(speed_command) < 15))
	  {
	    encodeur.PID_speed.Output = 0;
	    _Lock_flag = true;
	  }
  }
  _Encoder_output = encodeur.PID_speed.Output;
  return _Encoder_output;
}


//***************************************************
//			Régulation en position PID
//***************************************************
void MoteurEncodeur::setPosPid(float p,float i,float d)
{
  encodeur.PID_pos.P = p;
  encodeur.PID_pos.I = i;
  encodeur.PID_pos.D = d;
}

void MoteurEncodeur::moveTo(long position,float speed)
{
  encodeur.targetPos = encodeur.currentPos + position;
  encodeur.targetSpeed = constrain(speed,-encodeur.maxRPM,encodeur.maxRPM);

  encodeur.mode = PID_MODE;
  encodeur.motionState = POSITION_ENSLAVEMENT;
  _Lock_flag = false;

  if(position > 0) //On compare la position actuelle avec la position recherchée
    _Direction = 1; //On va en avant
  else
    _Direction = -1; //On va en arrière
}

int16_t MoteurEncodeur::pidPositionToPwm(void)
{
  float cur_pos = 0;
  float next_speed = 0;
  float pos_error = 0;
  float speed_error = 0;
  float d_component = 0;
  float regulator = 0;
  //Calcul erreur
  pos_error = encodeur.targetPos - encodeur.currentPos;;


  if((_Lock_flag == false) && ((_Direction == 1 && pos_error < 0) || (_Direction == -1 && pos_error > 0)))
  {
    //Calcul régulateur
    d_component = encodeur.currentSpeed;
    regulator = encodeur.PID_pos.D * d_component;
    //Application de la consigne
    encodeur.PID_pos.Output = -regulator;
    _Encoder_output = encodeur.PID_pos.Output;
    encodeur.currentPwm = _Encoder_output;

    _Lock_flag = true;   //Position dépassée 
    return _Encoder_output;
  }
      
  //A grande distance de la position cible et en fonction de la vitesse voulue, on régule en vitesse
  //Plus on va vite, plus on aura besoin de ralentir avant.
  if((_Lock_flag == false) && (abs(pos_error) >= encodeur.targetSpeed * DECELERATION_DISTANCE)) 
  {
    //Calcul régulateur P
    speed_error = encodeur.currentSpeed - encodeur.targetSpeed * (pos_error/abs(pos_error));
    regulator = constrain(encodeur.PID_speed.P * speed_error,-25,25);
    //Application de la consigne
    encodeur.PID_speed.Output = constrain(_Encoder_output - regulator,-255,255);  
    _Encoder_output = encodeur.PID_speed.Output;
  }

  //Dès que l'on se rapproche, on régule en position
  else
  {
  	//On régule jusqu'à ce qu'on soit dans la bande d'erreur
    if((_Lock_flag == false) && (abs(pos_error) > encodeur.precision)) //On est hors de la bande l'erreur
    {
      //Atténueation parabolique de la vitesse 
      next_speed = (pos_error/abs(pos_error)) * sqrt(abs(encodeur.targetSpeed * (pos_error-encodeur.precision)/DECELERATION_DISTANCE));
      
      //Calcul régulateur PD
      d_component = encodeur.currentSpeed - next_speed;
      regulator = constrain(encodeur.PID_pos.D * d_component,-20,20);
      //Application de la consigne
      encodeur.PID_pos.Output = _Encoder_output - regulator;
      if(pos_error >= 0) //On restreint en fonction du sens de rotation en ne laissant pas la pwm passer entre -25 et -25 pour éviter le bloquage
        encodeur.PID_pos.Output = constrain(encodeur.PID_pos.Output,25,255);
      else
        encodeur.PID_pos.Output = constrain(encodeur.PID_pos.Output,-255,-25 );
      _Encoder_output = encodeur.PID_pos.Output;
    }

    //On est dans la la bande d'erreur ou la position a déjà été atteinte, 
    //on continue à réguler si jamais on force manuellement la rotation de la roue, elle reviendra à sa position 
    else
    {
      _Lock_flag = true; //Position atteinte
	  //Calcul régulateur PD
      d_component = encodeur.currentSpeed;
      regulator = constrain(encodeur.PID_pos.D * d_component,-20,20);
      encodeur.PID_pos.Output = (pos_error * encodeur.PID_pos.P)- regulator;
      //Application de la consigne
      encodeur.PID_pos.Output = constrain(encodeur.PID_pos.Output,-255,255);
      _Encoder_output = encodeur.PID_pos.Output;
    }
  }
  return _Encoder_output;
}



//***************************************************
//			Autres
//***************************************************
boolean MoteurEncodeur::isTarPosReached(void)
{
  return _Lock_flag;
}

void MoteurEncodeur::loop(void)
{
  updateCurPos();
  updateSpeed();

  if(encodeur.mode == PID_MODE)
    encoderMove();
  else if(encodeur.mode == PWM_MODE)
    pwmMove();
}
