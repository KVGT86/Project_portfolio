#include <Arduino.h>

#include <stdint.h>
#include "ultrasonic.hpp"
#include "sensors.h"
#include "motors.h"

bool DIRECTION_FLAG = true;

int direction = 1;
void sensor_init() {
    usAddToArray(US_TRIG_0, US_ECHO_0, 0); 
    usAddToArray(US_TRIG_1, US_ECHO_1, 1); 
    usAddToArray(US_TRIG_2, US_ECHO_2, 2); 
    usAddToArray(US_TRIG_3, US_ECHO_3, 3); 
    
}


void read_ultrasonic() {

    uint16_t low_left,low_right,high_left,high_right;       //try move outside of everything (to top) !!!!!!!
    uint16_t obstacle_range = 350;  //was 500
    // uint16_t min_seperation = 50;

    Serial.printf("\n");
    // Ping array and wait - set wait around 100ms
    usPingArray(usArray, US_NUM); 
    delay(100);
    // Waited long enough, calculate the distances
    usCalcArray(usArray, US_NUM, distances); 

    for (int i = 0; i < US_NUM; ++i) {
      if (distances[i] == 0) {
        distances[i] = 1000;
      }
      if (i==0) {
        Serial.printf("Low Left ");
      } 
      else if (i==1) {
        Serial.printf("High Left ");
      }
      else if (i==2) {
        Serial.printf("Low Right ");
      }
      else if (i==3) {
        Serial.printf("High Right ");
      }

      Serial.printf("%d\t", distances[i]); 
        
    }
    Serial.printf("\n");

    low_left = distances[0];
    high_left = distances[1];
    low_right = distances[2];
    high_right = distances[3];

    if ((high_left < 80) || (high_right < 80)) {                                   //Something tall very very close
      direction = 3;
    } else if ((low_left < 80) && (low_right < 80)) {                                   //something low very very close
      direction = 3;    //turn away instead?
    } else if ((high_left > obstacle_range && high_right > obstacle_range && low_left > 160 && low_left < 500 && low_right > 160 && low_right < 500) || (high_left < obstacle_range && (high_left - low_left) >= 50 && low_left > 160 && high_right < obstacle_range && (high_right - low_right) >= 50 && low_right > 160)) {              //weight detected by all sensors  
      direction = 1;
    } else if ((high_left > obstacle_range && low_left > 160 && low_left < 500) || (high_left < obstacle_range && (high_left - low_left) >= 50 && low_left > 160)) {               //weight detected by left sensors  
      direction = 2;
    } else if ((high_right > obstacle_range && low_right > 160 && low_right < 500) || (high_right < obstacle_range && (high_right - low_right) >= 50 && low_right > 160)) {            //weight detected by right sensors       
      direction = 0;
    } else if (high_left < 250 && high_right < 250) {                   //obstacle getting fairly fucking close, turn sharp (left,right alternate) //was 250
      if (DIRECTION_FLAG) {                                           //Think this is working ok. if happening when unexpected, can have a counter like button debouncing - but unlikely both sensors misread?
        direction = 4;
        DIRECTION_FLAG = false; 
      } else {
        direction = 5;
        DIRECTION_FLAG = true;
      }
    } else if (high_left < obstacle_range && (high_left - low_left) < 50) {                          //tall obstruction detcted by left sensors
      direction = 0;
    } else if (high_right < obstacle_range && (high_right - low_right) < 50) {                       //tall obstruction detcted by right sensors
      direction = 2;
    } else {                                                                              //clear ahead
      direction = 1;
    }
  Serial.println(direction);
}
uint8_t PassDir(){
  return direction;
}


