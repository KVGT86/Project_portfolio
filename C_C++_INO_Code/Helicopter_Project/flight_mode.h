/*******************************************************************************
 *
 * flight_mode.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the finite state machine for controlling the flight mode of the helicopter.
 *
 ******************************************************************************/

#ifndef FLIGHT_MODE_H_
#define FLIGHT_MODE_H_

#include <PID_controller.h>
#include "switch.h"
#include "buttons.h"
#include "rotors.h"
#include "altitude.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "yaw.h"

enum flightState {LANDED = 0, TAKEOFF, FLYING, LANDING};
static const char* flightStateStrings[] = {
    "LANDED",
    "TAKEOFF",
    "FLYING",
    "LANDING"
};

#define FINDING_YAW_REF_SPEED 70
#define TAKEOFF_ALTITUDE 10
#define ALTITUDE_STEP_PERCENT 10
#define YAW_STEP_DEGREES 15
#define LANDING_DC 15

void update_flight_mode(void);
int8_t get_flight_state(void);
const char* get_flight_state_string(void);
bool yaw_ref_found(void);
bool reached_desired_alt(void);

#endif /* FLIGHT_MODE_H_ */
