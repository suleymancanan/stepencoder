//This is an example of how you would control 1 stepper

#include <AccelStepper.h>

//int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorSpeed = 5000; //maximum steps per second (about 5rps / at 8 microsteps)
int motorAccel = 500; //steps/second/second to accelerate
/*
int motorDirPin = 5; //digital pin 2
int motorStepPin = 2; //digital pin 3
const int Enable_PIN = 3;
*/
/*
const unsigned char ttable[6][4] = {
  {0x3 , 0x2, 0x1,  0x0}, {0x23, 0x0, 0x1,  0x0},
  {0x13, 0x2, 0x0,  0x0}, {0x3 , 0x5, 0x4,  0x0},
  {0x3 , 0x3, 0x4, 0x10}, {0x3 , 0x5, 0x3, 0x20}
};
*/
/*const unsigned char ttable[7][4] = {
  {0x0, 0x2, 0x4,  0x0}, {0x3, 0x0, 0x1, 0x10},
  {0x3, 0x2, 0x0,  0x0}, {0x3, 0x2, 0x1,  0x0},
  {0x6, 0x0, 0x4,  0x0}, {0x6, 0x5, 0x0, 0x10},
  {0x6, 0x5, 0x4,  0x0},
};*/
const unsigned char ttable[7][4] = {
{0x0, 0x2, 0x4, 0x0}, {0x3, 0x0, 0x1, 0x10},
{0x3, 0x2, 0x0, 0x0}, {0x3, 0x2, 0x1, 0x0},
{0x6, 0x0, 0x4, 0x0}, {0x6, 0x5, 0x0, 0x20},
{0x6, 0x5, 0x4, 0x0},
};

#define DT 2
#define CLK 3
#define SW 4
#define DIR_CCW 0x10
#define DIR_CW 0x20
int motorStepPin = 8; //digital pin 3
int motorDirPin = 9; //digital pin 2
const int Enable_PIN = 10;
AccelStepper stepper(1, motorStepPin, motorDirPin);
volatile unsigned char state = 0;



void setup(){
  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);
  pinMode(Enable_PIN, OUTPUT);
  digitalWrite(Enable_PIN, LOW);
  stepper.moveTo(8000); //move 32000 steps (should be 10 rev)
  Serial.begin(9600); 
  pinMode(DT, INPUT);   
  pinMode(CLK, INPUT);  
  pinMode(SW, INPUT);  
  digitalWrite(SW, HIGH);
}

void loop(){
 int counter;
 int scale;
 //byte DialPos;
 //byte Last_DialPos;
 unsigned char result;
  /* Reset the counter */
 counter = 0;
 scale=0;
 
  while(1)
  {
    /* Read the status of the dial */
    unsigned char pinstate = (digitalRead(CLK) << 1) | digitalRead(DT);
    state = ttable[state & 0xf][pinstate];
    result=state&0x30;
    if(result==DIR_CCW) counter++;
        if(result==DIR_CW) counter--;
    
    /*
    if (DialPos == 3 && Last_DialPos == 1)
    {
      counter--;
    }
    */
    //Serial.println(counter);
 
      if(!digitalRead(SW)){
        scale++;counter=0;
      while (!digitalRead(SW)) {}  // wait til switch is released
            delay(10);
            Serial.println(scale);
      }
     //Serial.println("Switch pressed!");
  
    /* Save the state of the encoder */
   // Last_DialPos = DialPos;
    //stepper.moveTo(-stepper.currentPosition());
    //if(counter==0) 
    //stepper.moveTo(stepper.currentPosition());
    //else
    stepper.moveTo(counter*scale);
    stepper.run();
  }
}
