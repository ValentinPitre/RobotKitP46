#ifndef Moteur_encodeur_H
#define Moteur_encodeur_H

#include <Arduino.h>
#include <stdbool.h>
#include <avr/interrupt.h>


#define DIRECT_MODE       0x00
#define PID_MODE          0x01
#define PWM_MODE          0x02

#define POSITION_ENSLAVEMENT        0x00
#define SPEED_ENSLAVEMENT     0x01

#define ENCODER_POS_DEADBAND           1.6
#define DECELERATION_DISTANCE    6 

#define SAMPLING_TIME 40

typedef struct
{
  float P, I, D;
  float Setpoint, Output, Integral, differential, last_error;
} PID_internal;

typedef struct
{
  uint8_t mode;
  uint8_t motionState;

  int16_t pulseEncoder;
  long pulsePos;
  float ratio;
  float precision;
  int16_t maxRPM;
  
  int16_t currentPwm;
  int16_t targetPwm;
  int16_t previousPwm;

  float currentSpeed;
  float targetSpeed;
  float previousSpeed;
  
  long currentPos;
  long targetPos;
  long previousPos;

  PID_internal  PID_speed;
  PID_internal  PID_pos;
}Me_Encoder_type;

typedef struct
{
  uint8_t pin_A;
  uint8_t pin_B;
  uint8_t pin_PWM; 
  uint8_t pin_H1;
  uint8_t pin_H2;
} Encoder_port_type;



class MoteurEncodeur 
{
public:
  //Constructeur
  MoteurEncodeur();
  MoteurEncodeur(uint8_t slot);

  //Lecture des attributs
  uint8_t getPortNum(void);
  uint8_t getIntNum(void);  
  uint8_t getPinA(void);
  uint8_t getPinB(void);

  long getPulsePos(void);
  float getCurrentrentSpeed(void);
  long getCurrentPos(void);
  int16_t getCurrentPwm(void);

  //Paramètres du moteur
  void setRatio(int16_t RatioValue);
  void setMaxRPM(int16_t MaxRPM);

  //Opération sur la position du moteur 
  void setPulse(int16_t pulseValue);
  void setPulsePos(long pulse_pos);  
  void pulsePosPlus(void);
  void pulsePosMinus(void);
  void updateCurPos(void);

  //Opération sur la vitesse du moteur
  void setCurrentSpeed(float speed);
  void updateSpeed(void);
  void runSpeed(float speed);
  void setSpeed(float speed);  

  //Opération sur la pwm du moteur
  void setTarPWM(int16_t pwm_value);
  void setMotorPwm(int16_t pwm);  
  void stop();
  //Choix du mode de régulation
  void setMotionMode(int16_t motionMode);
  void pwmMove(void); //Sans PID
  void encoderMove(void); // Avec PID

  //Régulation en vitesse PID
  void setSpeedPid(float p,float i,float d);
  int16_t speedWithoutPos(void);

  //Régulation en position PID
  void setPosPid(float p,float i,float d);  
  void moveTo(long position,float speed = 100);
  int16_t pidPositionToPwm(void);


  //Autres
  boolean isTarPosReached(void); //Flag de mouvement terminé
  void loop(void);  //Boucle moteur à appeler pour régulation/mouvement

private:
   volatile Me_Encoder_type encodeur;
   boolean _Lock_flag;
   int8_t _Direction;
   uint8_t _PIN_A;
   uint8_t _PIN_B;
   uint8_t _PIN_PWM;
   uint8_t _PIN_H1;
   uint8_t _PIN_H2;
   uint8_t _IntNum;
   uint8_t _Slot;
   int16_t _Encoder_output;
   long _Last_speed_mesurement_time;
   long _Last_position_mesurement_time;
};
#endif


