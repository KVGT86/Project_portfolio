//************************************
//         sensors.h     
//************************************
#ifndef SENSORS_H_
#define SENSORS_H_

#include <Arduino.h>

#include <stdint.h>
#include "ultrasonic.hpp"
#include "motors.h"

// Read ultrasonic value
void read_ultrasonic();
void sensor_init();
uint8_t PassDir();

extern int direction; 
static uint16_t distances[US_NUM]; 
#endif /* SENSORS_H_ */
