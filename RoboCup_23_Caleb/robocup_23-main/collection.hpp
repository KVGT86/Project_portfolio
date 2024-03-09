#include <Arduino.h>
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


//Flags
int PREVIOUS_STATE = LOW; 
bool MAGS_ON = true;
bool INIT_FLAG = true;
bool CAPTURE_FLAG = true;

//Prototypes
void initialisation();
void weight_detection();
void color_detection();
void electromagnet_control();
