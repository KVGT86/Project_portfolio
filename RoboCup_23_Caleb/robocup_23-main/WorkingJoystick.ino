#define joyX A2
#define joyY A3
int xValue, yValue;
void setup() {
  Serial.begin(115200);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
 
  //print the values with to plot or view
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
  delay(100);
}