//************************************
//         IMU.h    
//************************************
#ifndef IMU_H_
#define IMU_H_

#include <Arduino.h>
#include "sensors.h"
// SET THIS TO REAL VALUES

void IMU_init();
void GetHeading(void);
void printEvent(sensors_event_t* event);
#endif /* IMU_H_ */
