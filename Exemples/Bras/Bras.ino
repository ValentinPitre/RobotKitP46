#include <MoteursDC.h>  //Déclaration librairie
//  /!\ Brancher le moteur correctement: borne - du moteur sur - du driver et borne + sur +
 
Bras MyBras(Port3B); 
/*  Info des port disponibles
  Port1A  Port1B  Port2A  Port2B  Port3A  Port3B  Port4A  Port4B
 */
 
void setup() {
  Serial.begin(9600);
  Serial.println("Start exemple...");
}

void loop() {
  MyBras.up(200); //Montée du bras à la puissance 200 (puissance entre 0 et +255)
  delay(1000);
  MyBras.stop();   //Arret du bras
  delay(1000);
  MyBras.down(100);  //Descente du bras à la puissance 100 (puissance entre 0 et +255)
  delay(1000);
  MyBras.stop();   //Arret du bras
  delay(1000);


  MyBras.up(); //Montée du bras à la puissance par défaut 200  
  delay(1000);
  MyBras.down(); //Descente du bras à la puissance par défaut 100
  delay(1000);
  MyBras.stop();   //Arret du bras
  delay(1000);


}
