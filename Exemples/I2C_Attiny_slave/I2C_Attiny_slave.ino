//Programme à televerser en Arduino as ISP sur les cartes capteurs
//Permet de reçevoir l'information en I2C sur la donnée à envoyer en I2C
//Renvoie sur requète I2C la donnée du capteur
//Renvoie 0xff si requète inconnue

#include <TinyWireS.h> 

#define I2C_SLAVE_ADDRESS 0x34 // Address of the slave 

#define REQUEST_FINDECOURSE1 0x01
#define REQUEST_FINDECOURSE2 0x02
#define REQUEST_ULTRASONIC_CM 0x04
#define REQUEST_LINESENSOR 0x05

#define LineSensor1 PB4
#define LineSensor2 PB3

#define FdC1 PB4
#define FdC2 PB3

#define SR04_TRIG PB3
#define SR04_ECHO PB4

uint8_t capteur;
uint8_t DataRequested=0xff;
uint8_t SentData=0xff; 

void setup()
{
    TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
    TinyWireS.onReceive(receiveEvent); 
    TinyWireS.onRequest(requestEvent);
 
    // Turn on LED when program starts
    pinMode(PB3, INPUT_PULLUP); //Par défaut en INPUT_PULLUP pour protéger l'ATTINY si un Fin de course est connecté
    pinMode(PB4, INPUT_PULLUP); //Par défaut en INPUT_PULLUP pour protéger l'ATTINY si un Fin de course est connecté
}
 
void loop()
{
    // This needs to be here
    TinyWireS_stop_check();
}
 
// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
    if (DataRequested==REQUEST_LINESENSOR)
        SentData=Line_Sensor();
    else if (DataRequested==REQUEST_ULTRASONIC_CM)
        SentData=Ultrasonic_Sensor_cm();
    else if (DataRequested==REQUEST_FINDECOURSE1)
        SentData=Fdc_Sensor(1);
    else if (DataRequested==REQUEST_FINDECOURSE2)
        SentData=Fdc_Sensor(2);
    else
        SentData=0xff;

    TinyWireS.send(SentData);   //Send byte
    DataRequested=0xff; //Reset Request
    
    //TinyWireS.send(DataRequested);   //For Debug
}

// Gets called when the ATtiny receives an i2c byte
void receiveEvent()
{
    DataRequested = TinyWireS.receive();    //Recieve byte
}



uint8_t Line_Sensor(){
  pinMode(LineSensor1, INPUT);
  pinMode(LineSensor2, INPUT);
  byte value = ((digitalRead(LineSensor1)<<1) + digitalRead(LineSensor2));
  pinMode(LineSensor1, INPUT_PULLUP);
  pinMode(LineSensor2, INPUT_PULLUP);
  return value;
}

uint8_t Fdc_Sensor(int number){
    if (number==1)
        return digitalRead(FdC1);
    else if (number==2)
        return digitalRead(FdC2);
    return 0xff;
}


#define MEASURE_TIMEOUT  20000UL // 20ms = ~3,40m à 340m/s
#define SOUND_SPEED  340.0/1000 // Vitesse du son dans l'air en mm/us

int Ultrasonic_Sensor(){
    long duree, distance;
    pinMode(SR04_TRIG, OUTPUT);
    pinMode(SR04_ECHO, INPUT); 
  
    digitalWrite(SR04_TRIG, LOW);  
    delayMicroseconds(2); 
    digitalWrite(SR04_TRIG, HIGH);
    delayMicroseconds(10); //Trig envois pendant 10ms 
    digitalWrite(SR04_TRIG, LOW);

    // On calcul le temps pour l'aller retour du signal
    duree = pulseIn(SR04_ECHO, HIGH,MEASURE_TIMEOUT);
    distance = duree / 2.0 * SOUND_SPEED;
    
    pinMode(SR04_TRIG, INPUT_PULLUP);
    pinMode(SR04_ECHO, INPUT_PULLUP);
    return distance;
}

uint8_t Ultrasonic_Sensor_cm(){
  int distance =  Ultrasonic_Sensor()/10;
  distance = distance > 254 ? 254 : distance; //On borne à 254 cm, , 255 étant un message d'erreur
  return distance;
}
