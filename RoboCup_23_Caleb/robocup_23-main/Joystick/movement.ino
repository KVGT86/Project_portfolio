#include <Servo.h>
Servo driveL; 
Servo driveR; 
#define joyX A4
#define joyY A5
int xValue = 0;
int yValue = 0;
 
void setup()

{
    Serial.begin(9600);
    driveL.attach(0);
    driveR.attach(1); 

    // Serial.println("Enter values between -450 to 450");
}

void loop() {
  // driveL.writeMicroseconds(1050); //Forwards
  // driveR.writeMicroseconds(1950);
  // delay(2000);
  // driveL.writeMicroseconds(1500); //Stop
  // driveR.writeMicroseconds(1500);
  // delay(2000);
  // driveL.writeMicroseconds(1950); //Reverse
  // driveR.writeMicroseconds(1050);
  // delay(2000);
  // driveL.writeMicroseconds(1500); //turn right
  // driveR.writeMicroseconds(1050);
  // delay(2000);
  // driveL.writeMicroseconds(1050); //pivot right
  // driveR.writeMicroseconds(1050);
  // delay(2000);
  // driveL.writeMicroseconds(1950); //turn left
  // driveR.writeMicroseconds(1500);
  // delay(2000);
  // driveL.writeMicroseconds(1950); //pivot left
  // driveR.writeMicroseconds(1950);
  // delay(2000);

  //for joystick if you want it lachlan

  // Nothing 673 710        Up 41 708       Down 1023 688       Left 670 61         Right 670 1023

  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  Serial.print(xValue); 
  Serial.print(" ");
  Serial.println(yValue);
 
      // int speed = Serial.parseInt();  //Receive Value from serial monitor

  if (xValue < 300) {
    driveL.writeMicroseconds(1050); //Forwards
    driveR.writeMicroseconds(1950);
  } else if (xValue > 800) {
    driveL.writeMicroseconds(1950); //Reverse
    driveR.writeMicroseconds(1050);
  } else if (yValue < 300) {
    driveL.writeMicroseconds(1950); //pivot left
    driveR.writeMicroseconds(1950);
  } else if (yValue > 800) {
    driveL.writeMicroseconds(1050); //pivot right
    driveR.writeMicroseconds(1050);
  } else {
    driveL.writeMicroseconds(1500); 
    driveR.writeMicroseconds(1500); 
  } 
  delay(50);
}