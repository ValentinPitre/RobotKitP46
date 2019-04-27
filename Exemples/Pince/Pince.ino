#include <MoteursDC.h>  //Déclaration librairie
//  /!\ Brancher le moteur correctement: borne - du moteur sur - du driver et borne + sur +
 
Pince MyPince(Port2B); 
/*  Info des port disponibles
  Port1A  Port1B  Port2A  Port2B  Port3A  Port3B  Port4A  Port4B
 */
 
void setup() {
  Serial.begin(9600);
  Serial.println("Start exemple...");
}

void loop() {
  MyPince.open(200); //Ouverture de la pince à la puissance 200 (puissance entre 0 et +255)
  delay(1000);
  MyPince.stop();   //Arret du moteur
  delay(1000);
  MyPince.close(100);  //Fermeture de la pince à la puissance 100 (puissance entre 0 et +255)
  delay(1000);
  MyPince.stop();   //Arret du moteur
  delay(1000);



  MyPince.open(); //Ouverture de la pince à la puissance par défaut 200
  delay(1000);
  MyPince.close();  //Fermeture de la pince à la puissance par défaut 150 
  delay(1000);
  MyPince.stop();   //Arret du moteur
  delay(1000);
}
