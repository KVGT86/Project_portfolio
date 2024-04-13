/*******************************************************************************
 *
 * display.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the ORBIT display.
 *
 * References:
 * Adapted from P.J. Bones UCECE (OLEDTest.c)
 *
 ******************************************************************************/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
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

static char text_buffer[17];

void init_display(void);
void display_blank(void);
void display_flight(int32_t mainDC, int32_t tailDC, int32_t yawAngle, int32_t altitude);

#endif /* DISPLAY_H_ */
