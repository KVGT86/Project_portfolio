/*
Test out the low cost ultrasound sensor, ISR based code on  learn is better post by ethan in forums. Soldering on one ultrasound board causing one sensor not to work
*/
#include <Servo.h>
Servo driveL; 
Servo driveR; 

const int AtrigPin = 3;
const int AechoPin = 2;

const int BtrigPin = 5;
const int BechoPin = 4;

const int CtrigPin = 30;
const int CechoPin = 31;

const int DtrigPin = 32;
const int DechoPin = 33;
 
void setup() 
{  
  pinMode(AtrigPin, OUTPUT);            //Setup ultrasound pins
  pinMode(AechoPin, INPUT);

  pinMode(BtrigPin, OUTPUT);            //Setup ultrasound pins
  pinMode(BechoPin, INPUT);

  pinMode(CtrigPin, OUTPUT);            //Setup ultrasound pins
  pinMode(CechoPin, INPUT);

  pinMode(DtrigPin, OUTPUT);            //Setup ultrasound pins
  pinMode(DechoPin, INPUT);

  driveL.attach(0);
  driveR.attach(1); 
  
  Serial.begin(9600);                  // initialize serial communication:
}
 
void loop()
{
  long durationA,durationB,durationC,durationD,Acm,Bcm,Ccm,Dcm;
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(AtrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(AtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(AtrigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  durationA = pulseIn(AechoPin, HIGH);

  digitalWrite(BtrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(BtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(BtrigPin, LOW);
  durationB = pulseIn(BechoPin, HIGH);
  
  digitalWrite(CtrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(CtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(CtrigPin, LOW);
  durationC = pulseIn(CechoPin, HIGH);

  digitalWrite(DtrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(DtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(DtrigPin, LOW);
  durationD = pulseIn(DechoPin, HIGH);
 
  // convert the time into a distance
  Acm = microsecondsToCentimeters(durationA);
  Bcm = microsecondsToCentimeters(durationB);
  Ccm = microsecondsToCentimeters(durationC);
  Dcm = microsecondsToCentimeters(durationD);
  Serial.print(Acm);
  Serial.print(" ");
  Serial.print(Bcm);
  Serial.print(" ");
  Serial.print(Ccm);
  Serial.print(" ");
  Serial.print(Dcm);
  Serial.println();
  delay(50);


//   if ((Acm > 16 && Acm < 50) && (Bcm > 16 && Bcm < 50)) {
//     driveL.writeMicroseconds(1050); //Forwards
//     driveR.writeMicroseconds(1950);    
//   } else if (Acm > 16 && Acm < 50) {
//     //turn right
//     Serial.print("turn right");
//     driveL.writeMicroseconds(1050); //pivot right
//     driveR.writeMicroseconds(1050);
//   } else if (Bcm > 16 && Bcm < 50){
//     //turn left
//     Serial.print("turn left");
//     driveL.writeMicroseconds(1950); //pivot left
//     driveR.writeMicroseconds(1950);
//   } else {
//     driveL.writeMicroseconds(1050); //Forwards
//     driveR.writeMicroseconds(1950);
//   }
}
 
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
} 
