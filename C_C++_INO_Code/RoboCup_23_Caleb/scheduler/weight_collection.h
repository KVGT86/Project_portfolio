
/************************************
 *        weight_collection.h       *
 *************************************/

 /* This header is for functions and tasks for
  *  finding and collecting weights  */

#ifndef WEIGHT_COLLECTION_H_
#define WEIGHT_COLLECTION_H_

//Libraries

#include <Adafruit_TCS34725.h>



//** GLOBALS **
//Pins
const int HALL_PIN_L = 21;
const int HALL_PIN_M = 2;
const int HALL_PIN_R = 2; 
const int MAG_PIN_L = 14;
const int MAG_PIN_M = 24;
const int MAG_PIN_R = 25;

//Constants
const uint8_t MAG_PWM = 254; //(PWM 0 - 255)
const uint16_t MAG_PWM_FREQ = 10000; 




//Prototypes
void weight_collection_initialisation();
void weight_detection();
void colour_detection();
void electromagnet_control();
void collection_control();

#endif /* WEIGHT_COLLECTION_H_ */
