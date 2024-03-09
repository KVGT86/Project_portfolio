/*******************************************************************************
 *
 * altitude.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions for controlling the altitude of the helicopter.
 *
 * References:
 * Parts of this module were taken from Week 4 Lab (ADCdemo1.c)
 *
 ******************************************************************************/

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

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

#define VREF 3.3
#define VMIN 1.0
#define MAX_ADC 4095
#define SAMPLE_RATE_HZ 80
#define BUFFER_SIZE 10
#define MAX_ALTITUDE 100
#define MIN_ALTITUDE 0

extern buff_t g_inBuffer;
extern int32_t g_ulSampCnt;

int32_t calc_avg_ADC(void);
int32_t calc_altitude(void);
int32_t set_ref_ADC(void);
int32_t altitude_error(void);
int32_t update_desired_altitude(int16_t change);
void reset_desired_altitude(void);
int32_t get_desired_alt(void);
int32_t get_altitude(void);
void set_altitude(int32_t newAltitude);
void init_ADC(void);
void ADC_int_handler(void);

#endif /* ALTITUDE_H_ */
