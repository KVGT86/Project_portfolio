#include <Wire.h>

int incomingByte = 0; // for incoming serial data
int Rx1 = 7;
int Tx1 = 8;
int SwitchIn = 19;
int valx1 = 0;
int valx2 = 0;
void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
  pinMode(Rx1, INPUT);
  pinMode(Tx1, INPUT);
  Wire.begin();        // join i2c bus (address optional for master)
}

void loop() {
  Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8

  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
}
//  valx1 = digitalRead(Rx1);   // read the input pin
//  valx2 = digitalRead(Tx1);
//  Serial.print("RX1:");
//  Serial.print(valx1);
//  Serial.print(", TX1:");
//  Serial.println(valx2);
//  delay(10);
//}
