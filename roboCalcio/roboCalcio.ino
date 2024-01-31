#include <SoftwareSerial.h>
#include <Servo.h>

#define pinruotaSinistra 22
#define pinruotaDestra 23
/* Definitione dei valori dei valori che possono assumere servo a rotazione continua */
#define ruotaStopValore 90
#define ruotaSinistraAvantiValore 180 // 180 per i servo AR-3606HB,   0 per i servo SM-S4303R a 110 va bene
#define ruotaSinistraIndietroValore 0 //   0 per i servo AR-3606HB, 180 per i servo SM-S4303R
#define ruotaDestraAvantiValore 0     //   0 per i servo AR-3606HB, 180 per i servo SM-S4303R
#define ruotaDestraIndietroValore 180 // 180 per i servo AR-3606HB,   0 per i servo SM-S4303R


/* Creazione oggetti per la libreria Servo */
Servo ruotaSinistra;                    /*  Valores from 0 to 180  */
Servo ruotaDestra;                      /*  Valores from 0 to 180  */

SoftwareSerial bluetooth(12,11);
const int ledPin = 13; // Built in LED in Arduino board
int dxV, sxV;
bool updateV;

void setup() {
  // cose bluethoot
  bluetooth.begin(9600);
  Serial.begin(9600);
  updateV = false;
  
  //cose ruote
  ruotaSinistra.attach(pinruotaSinistra);
  ruotaDestra.attach(pinruotaDestra);
  ruotaSinistra.write(ruotaStopValore);
  ruotaDestra.write(ruotaStopValore);
  
}

void loop() {
  
  // prendo messaggio
  if (bluetooth.available() > 1){ // Check if there is data coming
    //(importante aspettare di avere 2 info prima di leggere 2 volte)
    
    //Serial.println("checked for msg");//debug
    sxV = bluetooth.read();
    dxV = bluetooth.read();
    updateV = true;
    
    //Serial.println(sxV);//debug
    //Serial.println(dxV);//debug
  }
  
  //Serial.println("letto da bluetooth");//debug

  // elaboro messaggio
  if(updateV){
    ruotaSinistra.write(sxV);
    ruotaDestra.write(dxV);
    updateV = false;
  }

  //Serial.println("loop");//debug
}
