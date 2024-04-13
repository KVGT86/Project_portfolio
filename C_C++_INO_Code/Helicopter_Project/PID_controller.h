/*******************************************************************************
 *
 * PID_controller.h
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

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include <flight_mode.h>
#include <stdint.h>
#include <stdbool.h>
#include "rotors.h"
#include "switch.h"
#include "altitude.h"

int32_t mainDC;
int32_t tailDC;

void update_altitude(void);
void update_yaw(void);

#endif /* PID_CONTROLLER_H_ */
