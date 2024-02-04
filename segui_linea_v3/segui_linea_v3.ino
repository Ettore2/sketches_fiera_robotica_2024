/*
 * Classe: 5F e 5G
 */
 
#include <Servo.h>

/*Sensori*/
#define pinlineaSinistra 12
#define pinlineaDestra 8
#define pinlineaCentro 10
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

/* valori sensore guardalinea */
#define lineaTrovata 1
#define lineaNonTrovata 0
#define curvaDx 1
#define curvaSx -1
#define curvaNone 0
#define cicliPerFuoriPista 30

/* Variabili lettura sensori linea*/
long inputDestra;
long inputSinistra;
long inputCentro;
int ultimaCurva, cicliNoLinee;

/* Default delay */
#define defaultDelay        10

void setup() {
  /* Define dei pin degli oggetti servo */
  ruotaSinistra.attach(pinruotaSinistra);
  ruotaDestra.attach(pinruotaDestra);
  Serial.begin(9600);
  pinMode(pinlineaSinistra, INPUT);
  pinMode(pinlineaDestra, INPUT);
  pinMode(pinlineaCentro, INPUT);
  ultimaCurva = curvaNone;
  cicliNoLinee = 0;
  
  ruotaSinistra.write(ruotaStopValore);
  ruotaDestra.write(ruotaStopValore);
  delay(1000);//tenere prima di settare la velocità delle ruote
  
  ruotaSinistra.write(ruotaSinistraAvantiValore);
  ruotaDestra.write(ruotaDestraAvantiValore);
}

void loop() {

  inputDestra = digitalRead(pinlineaDestra);
  inputCentro = digitalRead(pinlineaCentro);
  inputSinistra = digitalRead(pinlineaSinistra);

  if(inputCentro == lineaTrovata && inputSinistra == lineaTrovata && inputDestra == lineaTrovata){
    
  }
  else if (inputSinistra == lineaTrovata && inputCentro == lineaTrovata) {
    //giraSinistra90();//sul circuito di prova è più veloce se usa solo il codice di correzione di qunad ovai fuori pista
    ultimaCurva = curvaSx;
    cicliNoLinee = 0;
  } else if (inputDestra == lineaTrovata && inputCentro == lineaTrovata) {
    //giraDestra90();//sul circuito di prova è più veloce se usa solo il codice di correzione di qunad ovai fuori pista
    ultimaCurva = curvaDx;
    cicliNoLinee = 0;
  } else if(inputSinistra == lineaTrovata){
    giraSinistra();
    ultimaCurva = curvaSx;
    cicliNoLinee = 0;
  }else if(inputDestra == lineaTrovata){
    giraDestra();
    ultimaCurva = curvaDx;
    cicliNoLinee = 0;
  }else if(inputCentro == lineaNonTrovata && inputSinistra == lineaNonTrovata && inputDestra == lineaNonTrovata){//nessun nero -> fuori percorso
    if(cicliNoLinee >= cicliPerFuoriPista){
      //frenataDebug();//debug
      if(ultimaCurva == curvaSx){
        ruotaSinistra.write(ruotaSinistraIndietroValore);
      }
      if(ultimaCurva == curvaDx){
        ruotaDestra.write(ruotaDestraIndietroValore);
      }
      while(digitalRead(pinlineaDestra) == lineaNonTrovata && digitalRead(pinlineaCentro) == lineaNonTrovata && digitalRead(pinlineaSinistra) == lineaNonTrovata){
      //while(digitalRead(pinlineaCentro) == lineaNonTrovata){
        delay(10);
      }
      ruotaSinistra.write(ruotaSinistraAvantiValore);
      ruotaDestra.write(ruotaDestraAvantiValore);
      cicliNoLinee = 0;
    }else{
      cicliNoLinee ++;
    }
  }
  
  delay(5);//necessario per contare quanti loop prima di considerarsi fuori pista
  //(da questo delay dipende la costante "cicliPerFuoriPista" ed i delay delle funzioni
  //giraSinistra() e giraDestra()
}


void giraSinistra(){
  ruotaSinistra.write(ruotaStopValore);
  delay(10);
  ruotaSinistra.write(ruotaSinistraAvantiValore);
}
void giraDestra(){
  ruotaDestra.write(ruotaStopValore);
  delay(10);
  ruotaDestra.write(ruotaDestraAvantiValore);
}

void giraSinistra90(){
  ruotaSinistra.write(ruotaStopValore);//se si mette in retro diventa scattoso ed instabile
  while(digitalRead(pinlineaDestra) == lineaNonTrovata){
  delay(5);
  }
  ruotaSinistra.write(ruotaSinistraAvantiValore);
}
void giraDestra90(){
  ruotaDestra.write(ruotaStopValore);//se si mette in retro diventa scattoso ed instabile
  while(digitalRead(pinlineaSinistra) == lineaNonTrovata){
  delay(5);
  }
  ruotaDestra.write(ruotaDestraAvantiValore);
}

void frenataDebug(){
      ruotaSinistra.write(ruotaStopValore);//debug
      ruotaDestra.write(ruotaStopValore);//debug
      delay(1500);//debug
      ruotaSinistra.write(ruotaSinistraAvantiValore);//debug
      ruotaDestra.write(ruotaDestraAvantiValore);//debug
}
