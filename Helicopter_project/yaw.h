/*******************************************************************************
 *
 * yaw.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions for controlling the yaw of the helicopter.
 *
 ******************************************************************************/

#ifndef YAW_H_
#define YAW_H_

#include <flight_mode.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "buffer.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#define ANGLE_PRECISION 10
#define TICKS_PER_REVOLUTION 448
#define PHASE_A GPIO_PIN_0
#define PHASE_B GPIO_PIN_1
#define YAW_REFERENCE_PIN GPIO_PIN_4
#define MAX_YAW 360
#define MIN_YAW -360

extern int32_t count;

int32_t calc_yaw_angle(void);
void yaw_int_handler(void);
int32_t update_desired_yaw(int16_t degreesChange);
int32_t get_desired_yaw(void);
int32_t yaw_error(void);
void init_yaw_GPIO(void);
void reset_desired_yaw(void);

#endif /* YAW_H_ */
