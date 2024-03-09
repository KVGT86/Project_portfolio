#include <Servo.h>

Servo myservoA;      // create servo object to control a servo
Servo myservoB;      // create servo object to control a servo

enum PinAssignments {
  encoder1PinA = 2,
  encoder1PinB = 3,
  
  encoder2PinA = 4,
  encoder2PinB = 5,
};

volatile signed int encoderPos1 = 0;
unsigned int lastReportedPos1 = 1;
volatile signed int encoderPos2 = 0;
unsigned int lastReportedPos2 = 1;

boolean A_set1 = false;
boolean B_set1 = false;
boolean A_set2 = false;
boolean B_set2 = false;

void setup() 
{
  pinMode(encoder1PinA, INPUT);       //Set encoder pins as inputs
  pinMode(encoder1PinB, INPUT); 
  pinMode(encoder2PinA, INPUT); 
  pinMode(encoder2PinB, INPUT); 

  attachInterrupt(digitalPinToInterrupt(2), doEncoder1A, CHANGE);  //Set up an interrupt for each encoder
  attachInterrupt(digitalPinToInterrupt(4), doEncoder2A, CHANGE);
  
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  myservoA.attach(7);  // attaches the servo pin 3 to the servo object
  myservoB.attach(8);  // attaches the servo pin 2 to the servo object
  Serial.begin(9600);                       //Set up serial communications
}


void loop()
{ 
  
  //If there has been a change in value of either encoder then print the 
  //  encoder values to the serial port
  if ((lastReportedPos1 != encoderPos1)||(lastReportedPos2 != encoderPos2)) 
  {
    Serial.print("Index:");
    Serial.print(encoderPos1, DEC);
    Serial.print(":");
    Serial.print(encoderPos2 % 360, DEC);
    Serial.println();
  }
  myservoA.writeMicroseconds(1900);
  
}

// Interrupt on A changing state
void doEncoder1A(){
  // Test transition
  A_set1 = digitalRead(encoder1PinA) == HIGH;
  // and adjust counter + if A leads B
  encoderPos1 += (A_set1 != B_set1) ? +1 : -1;
  
  B_set1 = digitalRead(encoder1PinB) == HIGH;
  // and adjust counter + if B follows A
  encoderPos1 += (A_set1 == B_set1) ? +1 : -1;
}


// Interrupt on A changing state
void doEncoder2A(){
  // Test transition
  A_set2 = digitalRead(encoder2PinA) == HIGH;
  // and adjust counter + if A leads B
  encoderPos2 += (A_set2 != B_set2) ? +1 : -1;
  
   B_set2 = digitalRead(encoder2PinB) == HIGH;
  // and adjust counter + if B follows A
  encoderPos2 += (A_set2 == B_set2) ? +1 : -1;
}
