/************************************
 *        weight_collection.cpp       *
 *************************************/

 /* This is for functions and tasks for
  *  finding and collecting weights  */

#include "weight_collection.h"

bool CAPTURED_FLAG[3] = {false,false,false};
float COLOUR_TOLERANCE = 0.1;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//Flags
int PREVIOUS_HALL_STATE_ARR[] = {LOW, LOW, LOW}; 
bool WEIGHT_CAPTURE_FLAG[] = {false, false, false};
bool ELEMAGS_ON = true;
bool INIT_COLOR_FLAG = true;

void weight_collection_initialisation() 
{
  //Colour sensor initialisation
  // Serial.println("Color View Test!");

  // if (tcs.begin()) {
    
  //   Serial.println("Found sensor");
    
  // } else {
    
  //   Serial.println("No TCS34725 found ... check your connections");
  //   while (1); // infinite loop
  // }

  //Hall Effect sensor inputs
  pinMode(HALL_PIN_L, INPUT);
  pinMode(HALL_PIN_M, INPUT);
  pinMode(HALL_PIN_R, INPUT);

  //Electromagnet Pin
  pinMode(MAG_PIN_L, OUTPUT);
  pinMode(MAG_PIN_M, OUTPUT);
  pinMode(MAG_PIN_R, OUTPUT);
  analogWrite(MAG_PIN_L, MAG_PWM_FREQ); //PWM Frequency Set
  analogWrite(MAG_PIN_M, MAG_PWM_FREQ); //PWM Frequency Set
  analogWrite(MAG_PIN_R, MAG_PWM_FREQ); 
}


void weight_detection() 
{
  uint8_t sensorValueArr[3] = {digitalRead(HALL_PIN_L),digitalRead(HALL_PIN_M),digitalRead(HALL_PIN_R)}; //Read the sensors 

  for (int i = 0; i <= 2; i++) {
     if (sensorValueArr[i] != PREVIOUS_HALL_STATE_ARR[i]) {
      //If the sensor detects a change in magnentic field 
      PREVIOUS_HALL_STATE_ARR[i] = sensorValueArr[i];
      CAPTURED_FLAG[i] = true;
    }
  }
}

void colour_detection()
{
  uint8_t count = 0;
  uint16_t clear, red, green, blue, pClear, pRed, pGreen, pBlue;
  uint16_t currentColour[] = {clear, red, green, blue};
  uint16_t firstColour[] = {pClear, pRed, pGreen, pBlue};

  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  if (INIT_COLOR_FLAG == true) {
    
    tcs.getRawData(&pRed, &pGreen, &pBlue, &pClear);
  }

  tcs.setInterrupt(true);  // turn off LED
  
  for (int i = 0; i < 5; i++) {
  
    if (COLOUR_TOLERANCE >= abs(currentColour[i] - firstColour[i])) {
      
      ++count;
    }
    
    if (count == 4) {
        
      ELEMAGS_ON = false;
    }
    
    count = 0;
  }
}

void electromagnet_control()
{
  if (ELEMAGS_ON == true) {

    analogWrite(MAG_PIN_L, MAG_PWM);
    analogWrite(MAG_PIN_M, MAG_PWM);
    analogWrite(MAG_PIN_R, MAG_PWM);
    
  } else if (ELEMAGS_ON == false) {

    analogWrite(MAG_PIN_L, 0);
    analogWrite(MAG_PIN_M, 0);
    analogWrite(MAG_PIN_R, 0);
  }
}

void collection_control() {
  electromagnet_control();
  weight_detection();
}
