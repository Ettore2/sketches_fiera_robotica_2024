/*
 * Classe: 5F
 */
 
#include <Servo.h>

/*Sensori*/
#define pinruotaSinistra 23
#define pinruotaDestra 22
#define trigPinSinistra 6
#define echoPinSinistra 7
#define trigPinDestra 4
#define echoPinDestra 5
#define trigPinCentro 2
#define echoPinCentro 3

#define pinTatticaScan 9
#define pinTattica180 11
#define pinTattica3 13 // puggato, se non lo collego ha nulla restituisce HIGH


//distanza rilevamento
#define distanzaRilevamento 35 //50

/* Definitione dei valori dei valori che possono assumere servo a rotazione continua */
/*
#define ruotaStopValore 90
#define ruotaSinistraAvantiValore 130 // 180 per i servo AR-3606HB,   0 per i servo SM-S4303R a 110 va bene
#define ruotaSinistraIndietroValore 0 //   0 per i servo AR-3606HB, 180 per i servo SM-S4303R
#define ruotaDestraAvantiValore 50     //   0 per i servo AR-3606HB, 180 per i servo SM-S4303R
#define ruotaDestraIndietroValore 110 // 180 per i servo AR-3606HB,   0 per i servo SM-S4303R
*/

#define ruotaStopValore 90
#define ruotaSinistraAvantiValore 0 // 180 per i servo AR-3606HB,   0 per i servo SM-S4303R a 110 va bene
#define ruotaSinistraIndietroValore 180 //   0 per i servo AR-3606HB, 180 per i servo SM-S4303R
#define ruotaDestraAvantiValore 180     //   0 per i servo AR-3606HB, 180 per i servo SM-S4303R
#define ruotaDestraIndietroValore 0 // 180 per i servo AR-3606HB,   0 per i servo SM-S4303R


//tattiche
#define tatticaScan 0 //riga su se stesso epr cercare il nemico e poi lo punta
#define tattica180 1 //riga di 180° e poi va all'indietro
#define tattica3 2 //riga di 180° e poi va all'indietro

int tatticaUsata;

/* Creazione oggetti per la libreria Servo */
Servo ruotaSinistra;                    /*  Valores from 0 to 180  */
Servo ruotaDestra;                      /*  Valores from 0 to 180  */

//default delay
#define defaultDelay 10

long durationSin;
long durationDes;
long durationCen;
int cmDestra;
int cmSinistra;
int cmCentro;


void setup() {
  Serial.begin(9800);
  
  ruotaSinistra.attach(pinruotaSinistra);
  ruotaDestra.attach(pinruotaDestra);
  ruotaSinistra.write(ruotaStopValore);
  ruotaDestra.write(ruotaStopValore);

  pinMode(trigPinCentro, OUTPUT);
  pinMode(trigPinDestra, OUTPUT);
  pinMode(trigPinSinistra, OUTPUT);
  pinMode(echoPinCentro, INPUT);
  pinMode(echoPinDestra, INPUT);
  pinMode(echoPinSinistra, INPUT);
  
  pinMode(pinTatticaScan, INPUT);
  pinMode(pinTattica180, INPUT);
  pinMode(pinTattica3, INPUT);

  
  
  delay(5000);        //DELAY INIZIALE

  //togliere--------------------------
      ruotaSinistra.write(ruotaSinistraAvantiValore);
      ruotaDestra.write(ruotaDestraAvantiValore);
      delay(1000);        //DELAY INIZIALE
      ruotaSinistra.write(ruotaStopValore);
      ruotaDestra.write(ruotaStopValore);
      delay(10000);        //DELAY INIZIALE
  //togliere--------------------------

 //codice per implemntare l'input della tattica tramite il settaggio di dei pin della scheda
 //(pare che i pin scollegati siano  a high)
  if(digitalRead(pinTatticaScan) == LOW){
    tatticaUsata = tatticaScan;
    //Serial.print("tattica 1");
  }else if(digitalRead(pinTattica180) == LOW){
    tatticaUsata = tattica180;
    //Serial.print("tattica 2");
  }else{
    tatticaUsata = tattica180;//(possibile implementazione di 3° tattica)
    //Serial.print("tattica 3");
  }



  if(tatticaUsata == tatticaScan){
    do{//sistemare rilevamento nemico
      ruotaSinistra.write(ruotaSinistraAvantiValore);
      ruotaDestra.write(ruotaDestraIndietroValore);
      delay(35);
      ruotaSinistra.write(ruotaStopValore);
      ruotaDestra.write(ruotaStopValore);
      
  
      leggiDestra();
      leggiSinistra();
      leggiCentro();
    } while (cmCentro > distanzaRilevamento && cmSinistra > distanzaRilevamento && cmDestra > distanzaRilevamento);

    if(cmCentro < distanzaRilevamento){
        gira180();
    }else if(cmDestra < distanzaRilevamento){
        giraSinistra();
    }else{
        giraDestra();
    }
  
  }else if(tatticaUsata == tattica180){
    gira180();
  }

  

}

void loop() {

  leggiDestra();
  leggiSinistra();
  if(cmDestra < distanzaRilevamento){
    giraSinistra();
  }else if(cmSinistra < distanzaRilevamento){
    giraDestra();
  }

}

void leggiDestra(){

  digitalWrite(trigPinDestra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDestra, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDestra, LOW);

  durationDes = pulseIn(echoPinDestra, HIGH);
  cmDestra = microsecondsToCentimeters(durationDes); 

}


void leggiSinistra(){

  digitalWrite(trigPinSinistra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSinistra, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSinistra, LOW);

  durationSin = pulseIn(echoPinSinistra, HIGH);
  cmSinistra = microsecondsToCentimeters(durationSin);
  //Serial.println(cmSinistra);

}

void leggiCentro(){

  // lettura sensore centrale
  digitalWrite(trigPinCentro, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinCentro, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinCentro, LOW);

  durationCen = pulseIn(echoPinCentro, HIGH);
  cmCentro = microsecondsToCentimeters(durationCen); 
}

void giraDestra(){  //curva di 90 gradi
  
  ruotaDestra.write(ruotaStopValore);
  ruotaSinistra.write(ruotaStopValore);
  delay(100);
  ruotaDestra.write(ruotaDestraAvantiValore);
  ruotaSinistra.write(ruotaSinistraIndietroValore);
  delay(260);   //167
  ruotaDestra.write(ruotaDestraIndietroValore); 
}

void gira180(){  //curva di 180 gradi
  
  ruotaDestra.write(ruotaStopValore);
  ruotaSinistra.write(ruotaStopValore);
  delay(100);
  ruotaDestra.write(ruotaDestraAvantiValore);
  ruotaSinistra.write(ruotaSinistraIndietroValore);
  delay(520);
  ruotaDestra.write(ruotaDestraIndietroValore); 
}

void giraSinistra(){    //curva di 90 gradi

  ruotaSinistra.write(ruotaStopValore);
  ruotaDestra.write(ruotaStopValore);
  delay(100);
  ruotaSinistra.write(ruotaSinistraAvantiValore);
  ruotaDestra.write(ruotaDestraIndietroValore);
  delay(260); //167
  ruotaSinistra.write(ruotaSinistraIndietroValore);
}

void vaiAvanti(){

  ruotaDestra.write(ruotaDestraAvantiValore);
  ruotaSinistra.write(ruotaSinistraAvantiValore);
  
}

void vaiIndietro(){

  ruotaDestra.write(ruotaDestraIndietroValore);
  ruotaSinistra.write(ruotaSinistraIndietroValore);
  
}


long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
