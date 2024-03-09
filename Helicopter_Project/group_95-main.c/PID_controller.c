/*******************************************************************************
 *
 * PID_controller.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the yaw and altitude PID controllers.
 *
 ******************************************************************************/

#include <PID_controller.h>

#define KP_ALTITUDE 1.8
#define KD_ALTITUDE 0
#define KI_ALTITUDE 0.01
#define KP_YAW 6
#define KD_YAW 0.2
#define KI_YAW 0.01

#define ALT_HOVER 50
#define YAW_HOVER 30 // To counteract the torque from the main rotor

int32_t prevAltError = 0;
int32_t prevYawError = 0;

//*****************************************************************************
// PID controller to adjust the duty cycle of the main motor according to the current error
//*****************************************************************************
void update_altitude(void)
{
    int32_t error = altitude_error();
    int32_t derivativeError = 0;
    int32_t accumIntegralError = 0;

    // Calculated the error accumulated over time
    accumIntegralError += error;
    // Calculate the difference between current error and previous error
    derivativeError = (error - prevAltError);

    mainDC = (error * KP_ALTITUDE ) + KI_ALTITUDE * (accumIntegralError) + (derivativeError * KD_ALTITUDE) + ALT_HOVER;

    prevAltError = error;

    if (mainDC > PWM_MAIN_MAX_DUTY) {
       mainDC = PWM_MAIN_MAX_DUTY;
   } else if (mainDC < PWM_MAIN_MIN_DUTY) {
       mainDC = PWM_MAIN_MIN_DUTY;
   }
      set_main_pwm(mainDC);
}

//*****************************************************************************
// PID controller to update the duty cycle of the tail motor according to the current error
//*****************************************************************************
void update_yaw(void)
{
    int32_t error = yaw_error();
    int32_t derivativeError = 0;
    int32_t accumIntegralError = 0;

    // Calculate the accumulated error over time 
    accumIntegralError += error;
    // Calculate the difference between the current error and previous error 
    derivativeError = (error - prevYawError);

    tailDC = (error * KP_YAW ) + KI_YAW * (accumIntegralError) + (derivativeError * KD_YAW) + YAW_HOVER;

    prevYawError = error;

    if (tailDC > PWM_TAIL_MAX_DUTY) {
      tailDC = PWM_TAIL_MAX_DUTY;
    } else if (tailDC < PWM_TAIL_MIN_DUTY) {
      tailDC = PWM_TAIL_MIN_DUTY;
    }
     set_tail_pwm(tailDC);
}

/* END OF SOURCE FILE */