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


//costanti di stato
#define vicolo 1
#define destra 2
#define sinistra 3
#define dritto 0
#define aggiustare 4

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
int contatoreAggiusta;

int STATO;

#define numStorico 5    //quanti valori andrà a memorizzare
int storicoDestra[numStorico];  //valori presi da destra
int storicoSinistra[numStorico];  //valori presi da sinistra


void setup() {
  STATO = dritto;
  ruotaSinistra.attach(pinruotaSinistra);
  ruotaDestra.attach(pinruotaDestra);
  ruotaSinistra.write(ruotaStopValore);
  ruotaDestra.write(ruotaStopValore);
  contatoreAggiusta=0;

  delay(1000);
  
  pinMode(trigPinCentro, OUTPUT);
  pinMode(trigPinDestra, OUTPUT);
  pinMode(trigPinSinistra, OUTPUT);
  pinMode(echoPinCentro, INPUT);
  pinMode(echoPinDestra, INPUT);
  pinMode(echoPinSinistra, INPUT);

  for (int i = 0; i < numStorico; i++)
  {
    leggiSinistra();
  }
  for (int i = 0; i < numStorico; i++)
  {
    leggiDestra();
  }
  
  

  
}

void loop() {

  leggiDestra();
  leggiCentro();
  leggiSinistra();
  contatoreAggiusta++;


  delay(10);
  
  if(cmSinistra < 12 && cmCentro < 12 && cmDestra < 12){
    STATO = vicolo;
  }else if(cmSinistra > cmCentro && cmSinistra > cmDestra){
    if (controlloErroreSinistra())
    {
      STATO = sinistra;
    }else{
      STATO = dritto; 
    }
  }else if(cmDestra > cmCentro && cmDestra > cmSinistra){
    if (controlloErroreDestra)
    {
      STATO = destra;
    }else{
      STATO = dritto; 
    }
  }else{
    STATO = dritto;
  }
  if(contatoreAggiusta>20){

    contatoreAggiusta = 0;
    //STATO = aggiustare;
    aggiusta();
    
  }
  
  switch (STATO)
  {
  case vicolo:
      //TODO: tornare in dietro
    break;
  
  case sinistra:
  case destra:
    delay(350);   //400  //ti centri rispetto alla curva
    leggiSinistra();
    leggiDestra();

    if(cmSinistra > cmDestra){
      giraSinistra();
    }else{
      giraDestra();
    }
    break;

  /*case aggiustare:
    aggiusta();
   break;*/  

  case dritto:
    vaiAvanti();
    break;
  }


}
/*
void leggiDestra(){
  // lettura sensore destro
  digitalWrite(trigPinDestra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDestra, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDestra, LOW);

  durationDes = pulseIn(echoPinDestra, HIGH);
  cmDestra = microsecondsToCentimeters(durationDes);
  //funzione aggiunta nel caso di errore nella scansione
  if(cmDestra >= 500){
      cmDestra = 0;
  }
}
*/
void leggiDestra(){

  digitalWrite(trigPinDestra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDestra, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDestra, LOW);

  durationDes = pulseIn(echoPinDestra, HIGH);
  cmDestra = microsecondsToCentimeters(durationDes); 

  salvaStoricoDestra(cmDestra);

}

bool controlloErroreDestra(){

  for (int i = 0; i < numStorico; i++)
  {
    
    if(storicoDestra[i] < 25){
      return false;
    }

  }
  
  return true;

}

bool controlloErroreSinistra(){

  for (int i = 0; i < numStorico; i++)
  {
    if(storicoSinistra[i] < 25){
      return false;
    }
  }
  return true;

}

/*
void leggiSinistra(){
  // scrittura sensori
  digitalWrite(trigPinSinistra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSinistra, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSinistra, LOW);

  durationSin = pulseIn(echoPinSinistra, HIGH);
  cmSinistra = microsecondsToCentimeters(durationSin);
  //funzione aggiunta nel caso di errore nella scansione
  if(cmSinistra >= 500){
      cmSinistra = 0;
  }
}
*/


void leggiSinistra(){

  digitalWrite(trigPinSinistra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSinistra, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSinistra, LOW);

  durationSin = pulseIn(echoPinSinistra, HIGH);
  cmSinistra = microsecondsToCentimeters(durationSin);

  salvaStoricoSinistra(cmSinistra);

}
/*
void leggiCentro(){
  // lettura sensore centrale
  digitalWrite(trigPinCentro, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinCentro, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinCentro, LOW);

  durationCen = pulseIn(echoPinCentro, HIGH);
  cmCentro = microsecondsToCentimeters(durationCen); 
  //funzione aggiunta nel caso di errore nella scansione
  if(cmCentro >= 500){
    cmCentro = 0;
  }
}
*/


void leggiCentro(){

do
{
    // lettura sensore centrale
  digitalWrite(trigPinCentro, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinCentro, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinCentro, LOW);

  durationCen = pulseIn(echoPinCentro, HIGH);
  cmCentro = microsecondsToCentimeters(durationCen); 
} while (cmCentro > 500);
}

void giraDestra(){
  //ruotaSinistra.write(ruotaStopValore);
  //ruotaDestra.write(ruotaStopValore);
  //delay(100);
  ruotaDestra.write(ruotaDestraAvantiValore);
  ruotaSinistra.write(ruotaSinistraIndietroValore);
  delay(260);   //167
  ruotaSinistra.write(ruotaSinistraAvantiValore);
  delay(800);
  //aggiusta();

    
  
}

void giraSinistra(){
  //ruotaSinistra.write(ruotaStopValore);
  //ruotaDestra.write(ruotaStopValore);
  //delay(100);
  ruotaSinistra.write(ruotaSinistraAvantiValore);
  ruotaDestra.write(ruotaDestraIndietroValore);
  delay(260); //167
  ruotaDestra.write(ruotaDestraAvantiValore);
  delay(800);
  //aggiusta();
}

void vaiAvanti(){

  ruotaDestra.write(ruotaDestraAvantiValore);
  ruotaSinistra.write(ruotaSinistraAvantiValore);
  
}

void vaiIndietro(){

  ruotaDestra.write(ruotaDestraIndietroValore);
  ruotaSinistra.write(ruotaSinistraIndietroValore);
  
}

void aggiusta(){

  leggiSinistra();
  leggiDestra();
  int diffDex = cmDestra-cmSinistra;
  int diffSex = cmSinistra-cmDestra;
  if(diffDex < 20 && diffSex < 20){
    if(diffSex>2){
      ruotaDestra.write(ruotaStopValore);
      delay(17 * diffSex);
      ruotaDestra.write(ruotaDestraAvantiValore);
    }else if(diffDex>2){
      ruotaSinistra.write(ruotaStopValore);
      delay(17 * diffDex);
      ruotaSinistra.write(ruotaSinistraAvantiValore);
    }
  }
}


void salvaStoricoDestra(int valore)
{

  for (int i = numStorico-1; i > 0; i--)
  {
    storicoDestra[i] = storicoDestra[i - 1];
  }
  storicoDestra[0] = valore;
}
void salvaStoricoSinistra(int valore)
{

  for (int i = numStorico-1; i > 0; i--)
  {
    storicoSinistra[i] = storicoSinistra[i - 1];
  }
  storicoSinistra[0] = valore;
}


long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
