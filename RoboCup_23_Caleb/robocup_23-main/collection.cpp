#include "collection.hpp"

void initialisation() 
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
  int8_t sensorValue = digitalRead(HALL_PIN); //Read the sensor 

  if (sensorValue != PREVIOUS_STATE) {
    //If the sensor detects a change in magnentic field 
    Serial.print1n("Target Caputred")
    PREVIOUS_STATE = sensorValue;
    CAPTURED_FLAG = true;
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
