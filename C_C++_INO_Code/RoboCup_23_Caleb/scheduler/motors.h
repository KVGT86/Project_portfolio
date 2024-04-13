//************************************
//         motors.h    
//************************************
#ifndef MOTORS_H_
#define MOTORS_H_

#include <Arduino.h>
#include "sensors.h"
// SET THIS TO REAL VALUES
#define LEFT_MOTOR_ADDRESS 1      //Pin corresponding to the left dc motor
#define RIGHT_MOTOR_ADDRESS 1     //Pin corresponding to the right dc motor
#define MIN_SPEED_CAP 1           //Set the minimum speed value that can be written to the motors
#define MAX_SPEED_CAP 1           //Set the maximum speed value that can be written to the motors

void check_speed_limits();
void set_motor();
void set_right_motor();
void set_left_motor();
void motor_init();
void GetDir();
void set_target();
#endif /* MOTORS_H_ */
