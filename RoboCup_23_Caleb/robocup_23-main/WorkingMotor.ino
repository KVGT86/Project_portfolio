#include <Servo.h>

Servo myservoA,myservoB,myservoC,myservoD;      // create servo object to control a servo
int speed = 1050;
int state = 1; // 1 forward, -1 backward
void setup()// In Con65
{   
  Serial.begin(115200);
  myservoA.attach(0);  // attaches the servo  to the servo object useing pin 3  # 0 -> Right backwards
  myservoB.attach(1);  // attaches the servo  to the servo object useing pin 3  # 1 -> Left Forward
  myservoC.attach(4);  // attaches the servo  to the servo object useing pin 3
  myservoD.attach(5);  // attaches the servo  to the servo object useing pin 3
}

void loop() 
{ 
  myservoA.writeMicroseconds(speed);
  myservoB.writeMicroseconds(3000-speed);
  if (state == 1){
    speed++;
  } else if (state == -1){
    speed--;
  }
  if (speed < 1100){
    state = 1;
  }
  else if (speed > 1900){
    state = -1;
  }
  Serial.println(speed);
  delay(10);
} 
