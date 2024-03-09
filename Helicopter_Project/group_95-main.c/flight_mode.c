/*******************************************************************************
 *
 * flight_mode.c
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

#include <flight_mode.h>

extern int8_t flightState = LANDED;

//*****************************************************************************
// Implements a finite state machine to control the current flight state of the helicopter.
//*****************************************************************************
void update_flight_mode(void)
{
    switch(flightState) {

        case LANDED: {
            
            stop_main_pwm();
            stop_tail_pwm();
            set_altitude(0); // Ensure that ORBIT screen & UART displays 0% altitude when landed.

            if (check_switch() == SW1_UP) {
                reset_desired_yaw();
                reset_desired_altitude();
                update_desired_altitude(TAKEOFF_ALTITUDE);
                flightState = TAKEOFF;
            }
            break;
        }
        case TAKEOFF: {

            start_main_pwm();
            start_tail_pwm();

            update_altitude();
            update_yaw();

            // Changes state to FLYING if helicopter is hovering at the take-off altitude & it is facing the reference yaw
            if ((yaw_ref_found() == true) && (reached_desired_alt() == true)) {
                flightState = FLYING;
            } else {
                // Continue rotating until yawRef is found
                set_tail_pwm(FINDING_YAW_REF_SPEED);
            }
            break;
        }
        case FLYING: {

            if (check_button(UP) == PUSHED) {
             update_desired_altitude(ALTITUDE_STEP_PERCENT); // Increase altitude by 10 degrees

            } else if (check_button(DOWN) == PUSHED) {
             update_desired_altitude(-ALTITUDE_STEP_PERCENT); // Decrease altitude by 10 degrees

            } else if (check_button(LEFT) == PUSHED) {
                update_desired_yaw(YAW_STEP_DEGREES); // 15 degrees counter-clockwise rotation

            } else if (check_button(RIGHT) == PUSHED) {
                update_desired_yaw(-YAW_STEP_DEGREES); // 15 degrees clockwise rotation
            }

            update_altitude();
            update_yaw();

            if (check_switch() == SW1_DOWN) {
                reset_desired_yaw();
                update_desired_yaw(-calc_yaw_angle()); // Set desired yaw position to reference point before landing
                update_yaw();
                flightState = LANDING;
            }
            break;
        }
        case LANDING: {

            update_yaw();

            if (yaw_ref_found() == true) { // Once helicopter is facing the yaw reference position, land the helicopter.
                set_main_pwm(LANDING_DC);
            }
            if (calc_altitude() < 1) {
                flightState = LANDED;
            }
            break;
        }
    }
}

//*****************************************************************************
// Returns the current flight state of the helicopter
//*****************************************************************************
int8_t get_flight_state(void)
{
    return flightState;
}

//*****************************************************************************
// Returns the current flight state as a string to be printed on UART
//*****************************************************************************
const char* get_flight_state_string(void)
{
    const char* flightStateString = flightStateStrings[flightState];
    return flightStateString;
}

//*****************************************************************************
// Returns a boolean to verify that the yaw reference point has been found by the helicopter
//*****************************************************************************
bool yaw_ref_found(void)
{
    if (((calc_yaw_angle() <= 1) && (calc_yaw_angle() >= -1))) {
        return true;
    } else {
        return false;
    }
}

//*****************************************************************************
// Returns a boolean to verify that the helicopter has reached the current desired altitude
//*****************************************************************************
bool reached_desired_alt()
{
    if ((calc_altitude() <= get_desired_alt() + 1) && (calc_altitude() >= (get_desired_alt() - 1))) {
        return true;
    } else {
        return false;
    }
}

/* END OF SOURCE FILE */
