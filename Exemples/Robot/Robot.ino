#include <MyRobot.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>



//Encoder Motor
MoteurEncodeur MotorG(2);
MoteurEncodeur MotorD(3);
MyRobot Robot(&MotorG,&MotorD,6.1,20.5);


void isr_process_encoder2(void)
{
      if(digitalRead(MotorD.getPinB()) == 0){
            MotorD.pulsePosMinus();
      }else{
            MotorD.pulsePosPlus();
      }
      //Serial.println(MotorD.getCurrentPos());
}

void isr_process_encoder3(void)
{
      if(digitalRead(MotorG.getPinB()) == 0){
            MotorG.pulsePosMinus();
      }else{
            MotorG.pulsePosPlus();
      }
      //Serial.println(MotorG.getCurrentPos());
}


double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;



void setup(){
    //Serial.begin(9600);
    attachInterrupt(MotorD.getIntNum(), isr_process_encoder3, RISING);
    attachInterrupt(MotorG.getIntNum(), isr_process_encoder2, RISING);
    Robot.setBras(Port3A);
}

void loop(){
    Robot.Bluetooth();    
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    MotorD.loop();    
    MotorG.loop();
}
