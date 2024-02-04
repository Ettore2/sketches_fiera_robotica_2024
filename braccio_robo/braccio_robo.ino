#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //default address 0x40
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define PIN_PINZA 15
#define PIN_BASE 10
#define PIN_COLLO 14
#define PIN_TORSO0 11
#define PIN_TORSO1 12
#define PIN_TORSO2 13
#define PIN_SENSORE_CARICO 3
#define PIN_SENSORE_SCARICO 2

#define ID_PINZA 0
#define ID_BASE 1
#define ID_COLLO 2
#define ID_TORSO0 3
#define ID_TORSO1 4
#define ID_TORSO2 5

#define PINZA_CHIUSA 10
#define PINZA_APERTA 180
#define SENSORE_COPERTO 0
#define SENSORE_SCOPERTO 1

#define DEFAULT_MOVE_DELAY 20

int moveDelay;
int currPos [6];
int newPos [6];
int posCarico[] = {65,136,133,161};
int posCaricoAlzata[] = {65,127,132,164};
int posDritto[] = {90,90,90,90};
int posRest[] = {90,115,110,130};
int posMid[] = {140,115,110,130};
int posScarico[] = {160,110,140,175};



void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);

  moveDelay = DEFAULT_MOVE_DELAY;
  
  currPos[ID_PINZA] = 180;
  currPos[ID_BASE] = 90;
  currPos[ID_COLLO] = 90;
  currPos[ID_TORSO0] = 90;
  currPos[ID_TORSO1] = 90;
  currPos[ID_TORSO2] = 90;
   pwm.setPWM(PIN_BASE, 0, getMap(90,PIN_BASE));
   pwm.setPWM(PIN_PINZA, 0, getMap(180,PIN_PINZA));
   //pwm.setPWM(PIN_PINZA, 0, 500);
   //pwm.setPWM(collo, 0, 5);//non usare il collo!!!!
   pwm.setPWM(PIN_TORSO0, 0, getMap(90,PIN_TORSO0));
   pwm.setPWM(PIN_TORSO1, 0, getMap(90,PIN_TORSO1));
   pwm.setPWM(PIN_TORSO2, 0, getMap(90,PIN_TORSO2));
   delay(700);

   danza();
   
   delay(700);
   
   //prendi();
   //lascia();
   goToVector(posRest);
   //goToVector(posCaricoAlzata);
   //goToVector(posCarico);
   //delay(700);
   //goToVector(posCaricoAlzata);

   pinMode(PIN_SENSORE_CARICO,INPUT);
   pinMode(PIN_SENSORE_SCARICO,INPUT);


  //delay(1000000000);
  //Serial.println("caio");
}


void loop() {

//if(false)
  if((digitalRead(PIN_SENSORE_CARICO) == SENSORE_COPERTO) && (digitalRead(PIN_SENSORE_SCARICO) == SENSORE_SCOPERTO)){
   goToVector(posCaricoAlzata);
   goToVector(posCarico);
   prendi();
   delay(700);
   goToVector(posCaricoAlzata);
   delay(300);
   goToVector(posRest);
   goToVector(posMid);
   goToVector(posScarico);
   lascia();
   delay(700);
   goToVector(posRest);
  }

  

   delay(600);
    
}

//funzioni-----------------------------------------------------------------------------


int getMap(int val, int motore){
  //return map(val,0,180,SERVOMIN,SERVOMAX);
  
  if(motore == PIN_BASE){
    return map(val,0,180,120,530);
  }
  if(motore == PIN_PINZA){
    return map(val,0,180,100,350);//vero massimo = 500
  }
  if(motore == PIN_COLLO){
    return map(val,0,180,SERVOMIN,SERVOMAX);
  }
  if(motore == PIN_TORSO0){
    return map(val,0,180,120,530);
  }
  if(motore == PIN_TORSO1){
    return map(val,0,180,100,470);
  }
  if(motore == PIN_TORSO2){
    return map(val,0,180,100,470);
  }

  
  return map(val,0,180,SERVOMIN,SERVOMAX);

  
}

void goToComplete(int base,int torso0, int torso1, int torso2, int pinza){
  while(currPos[ID_BASE] != base || currPos[ID_TORSO0] != torso0
  || currPos[ID_TORSO1] != torso1 || currPos[ID_TORSO2] != torso2 || currPos[ID_PINZA] != pinza){
    if(currPos[ID_BASE] > base){
      currPos[ID_BASE] -=1;
    }
    if(currPos[ID_BASE] < base){
      currPos[ID_BASE] +=1;
    }
    
    if(currPos[ID_TORSO0] > torso0){
      currPos[ID_TORSO0] -=1;
    }
    if(currPos[ID_TORSO0] < torso0){
      currPos[ID_TORSO0] +=1;
    }
    
    if(currPos[ID_TORSO1] > torso1){
      currPos[ID_TORSO1] -=1;
    }
    if(currPos[ID_TORSO1] < torso1){
      currPos[ID_TORSO1] +=1;
    }
    
    if(currPos[ID_TORSO2] > torso2){
      currPos[ID_TORSO2] -=1;
    }
    if(currPos[ID_TORSO2] < torso2){
      currPos[ID_TORSO2] +=1;
    }
    
    if(currPos[ID_PINZA] > pinza){
      currPos[ID_PINZA] -=1;
    }
    if(currPos[ID_PINZA] < pinza){
      currPos[ID_PINZA] +=1;
    }

    updateEnginesPos();
    delay(moveDelay);
    //Serial.println("sposto motori");
    
  }
}
void goTo(int base,int torso0, int torso1, int torso2){
  goToComplete(base, torso0, torso1, torso2, currPos[ID_PINZA]);
}
void goToVector(int vector4[4]){
  goTo(vector4[0],vector4[1],vector4[2],vector4[3]);
}

void setEnginePos(int pinMotore,int valDeg){
  pwm.setPWM(pinMotore, 0, getMap(valDeg,pinMotore));
  
}

void updateEnginesPos(){
  setEnginePos(PIN_BASE, currPos[ID_BASE]);
  setEnginePos(PIN_TORSO0, currPos[ID_TORSO0]);
  setEnginePos(PIN_TORSO1, currPos[ID_TORSO1]);
  setEnginePos(PIN_TORSO2, currPos[ID_TORSO2]);
  setEnginePos(PIN_PINZA, currPos[ID_PINZA]);
}
void prendi(){
  setEnginePos(PIN_PINZA,PINZA_CHIUSA);
  currPos[ID_PINZA] = PINZA_CHIUSA;
}
void lascia(){
  setEnginePos(PIN_PINZA,PINZA_APERTA);
  currPos[ID_PINZA] = PINZA_APERTA;
}
void danza(){
  //danza
  moveDelay = 2;

  //si abbassa
  goTo(90,180,0,90);
  delay(50);
  goTo(90,180,90,0);
  delay(50);
  goTo(90,180,90,90);
  delay(50);

  //si alza
  goTo(90,70,70,70);
  delay(50);
  goTo(90,90,90,90);
  delay(50);

  //ruota fisarmonica
  goTo(0,135,0,180);
  delay(50);
  goTo(180,135,180,0);
  delay(50);

    //foglio d icarta
    for(int i = 0; i < 2; i++){
    goTo(0,120,60,30);
    delay(50);
    goTo(90,60,120,150);
    delay(50);
    goTo(180,120,60,30);
    delay(50);
    goTo(90,60,120,150);
    delay(50);
    goTo(0,120,60,30);
      
    }
  
  moveDelay = 10;//importante
  goToVector(posRest);
  moveDelay = 20;//importante
}
