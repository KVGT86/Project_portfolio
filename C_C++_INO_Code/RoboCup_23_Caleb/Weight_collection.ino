#include <Wire.h>
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//** GLOBALS **\\
//Pins
const int HALL_PIN = 1; 
const int MAG_PIN_1 = 2;
const int MAG_PIN_2 = 3;

//Constants
const uint8_t MAG_PWM = 229; //(PWM 0 - 255)
const uint16_t MAG_PWM_FREQ = 10000; 
uint16_t START_TIME = 0; 
const uint16_t DURATION = 2000; 
const float TOLERANCE = 0.1;
bool INIT_FLAG = true;

//Flags
int PREVIOUS_STATE = LOW; 
bool MAGS_ON = true;

void setup() 
{
  Serial.begin(9600);
  
  //Colour sensor initialisation
  Serial.println("Color View Test!");

  if (tcs.begin()) {
    
    Serial.println("Found sensor");
    
  } else {
    
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // infinite loop
  }

  //Hall Effect sensor inputs
  pinMode(HALL_PIN, INPUT);

  //Electromagnet Pin
  pinMode(MAG_PIN_1, OUTPUT);
  pinMode(MAG_PIN_2, OUTPUT);
  analogWrite(MAG_PIN_1, MAG_PWM_FREQ); //PWM Frequency Set
  analogWrite(MAG_PIN_2, MAG_PWM_FREQ); //PWM Frequency Set
}

void weight_detection() 
{
  // Read the state of the Hall effect sensor
  int sensorValue = digitalRead(HALL_PIN);

  if (sensorValue == LOW && PREVIOUS_STATE == HIGH) {
    
    Serial.println("Metal object detected");
    PREVIOUS_STATE = LOW; // Update the previous state
    START_TIME = millis(); // Record the start time
  
  } else if (sensorValue == HIGH && PREVIOUS_STATE == LOW) {
  
    Serial.println("No metal object detected."); // Metal removed
    PREVIOUS_STATE = HIGH; // Update the previous state
    
    unsigned long elapsedTime = millis() - START_TIME; // Calculate the elapsed time
    
    if (elapsedTime >= DURATION) {
   
      Serial.println("Collection Success"); 
    }
  }
}

void color_detection()
{
  uint8_t count = 0;
  uint16_t clear, red, green, blue, pClear, pRed, pGreen, pBlue;
  uint16_t currentColour[] = {clear, red, green, blue};
  uint16_t firstColour[] = {pClear, pRed, pGreen, pBlue};

  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  if (INIT_FLAG == true) {
    
    tcs.getRawData(&pRed, &pGreen, &pBlue, &pClear);
  }

  tcs.setInterrupt(true);  // turn off LED
  
  for (int i = 0; i < 0; ++i) {
  
    if (TOLERANCE >= abs(currentColour[i] - firstColour[i])) {
      
      ++count;
    }
    
    if (count == 4) {
        
      MAGS_ON = false;
    }
    
    count = 0;
  }
}

void electromagnet_control()
{
  if (MAGS_ON == true) {

    analogWrite(MAG_PIN_1, MAG_PWM);
    analogWrite(MAG_PIN_2, MAG_PWM);
    
  } else if (MAGS_ON == false) {

    analogWrite(MAG_PIN_1, 0);
    analogWrite(MAG_PIN_2, 0);
  }
}

void loop() 
{
  //have to set init flag to false
}
