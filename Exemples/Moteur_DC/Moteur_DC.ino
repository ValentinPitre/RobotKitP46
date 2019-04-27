#include <MoteursDC.h>  //Déclaration librairie
//  /!\ Brancher le moteur correctement: borne - du moteur sur - du driver et borne + sur +
 
MotorDC MyMotor(Port2B); 
/*  Info des port disponibles
  Port1A  Port1B  Port2A  Port2B  Port3A  Port3B  Port4A  Port4B
 */
 
void setup() {
  Serial.begin(9600);
  Serial.println("Start exemple...");
}

void loop() {
  MyMotor.run(200) //Le moteur tourne à la puissance 200 (puissance entre-255 et +255)
  delay(1000);
  MyMotor.stop();   //Arret du moteur
  delay(1000);
  MyMotor.run(-200);
  delay(1000);
    MyMotor.stop();   //Arret du moteur
  delay(1000);
}
