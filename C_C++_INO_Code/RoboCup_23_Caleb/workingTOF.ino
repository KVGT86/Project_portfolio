int distance = 0;
int strength = 0;
int MAdirpin = 0;
int MAsteppin = 1;
boolean receiveComplete = false;

void getTFminiData(int* distance, int* strength, boolean* complete) {
  static char i = 0;
  char j = 0;
  int checksum = 0; 
  static int rx[9];
  if(Serial2.available()) {  
    rx[i] = Serial2.read();
    if(rx[0] != 0x59) {
      i = 0;
    } else if(i == 1 && rx[1] != 0x59) {
      i = 0;
    } else if(i == 8) {
      for(j = 0; j < 8; j++) {
        checksum += rx[j];
      }
      if(rx[8] == (checksum % 256)) {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
        *complete = true;
      }
      i = 0;
    } else {
      i++;
    } 
  }  
}

void setup() {
  Serial2.begin(115200);
  pinMode(MAdirpin,OUTPUT);
  pinMode(MAsteppin,OUTPUT);  
  Serial.begin(115200);
}

void loop() {
  int j;
  int e = 1;
  digitalWrite(MAdirpin,LOW);
  for(j=0;j<=400;j++)            //Move 1000 steps
  {
    getTFminiData(&distance, &strength, &receiveComplete);
    if(receiveComplete) {
      receiveComplete = false;
      Serial.print(distance);
      Serial.print("cm\t");
      Serial.print("strength: ");
      Serial.println(strength);
    }
    digitalWrite(MAsteppin,LOW);
    delayMicroseconds(20);
    digitalWrite(MAsteppin,HIGH);
    delay(1);
  }
  delay(100);
  digitalWrite(MAdirpin,HIGH);
  for(j=0;j<=400;j++)            //Move 1000 steps
  {
    getTFminiData(&distance, &strength, &receiveComplete);
    if(receiveComplete) {
      receiveComplete = false;
      Serial.print(distance);
      Serial.print("cm\t");
      Serial.print("strength: ");
      Serial.println(strength);
    }
    digitalWrite(MAsteppin,LOW);
    delayMicroseconds(20);
    digitalWrite(MAsteppin,HIGH);
    delay(1);
  }
  delay(100);
}
