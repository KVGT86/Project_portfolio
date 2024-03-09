/*******************************************************************************
 *
 * display.c
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

#include "display.h"

//*****************************************************************************
// Initialises the display
//*****************************************************************************
void init_display(void)
{
    OLEDInitialise ();
}

//*****************************************************************************
// Displays a blank screen
//*****************************************************************************
void display_blank(void)
{
    int32_t i = 0;
    for (i = 0; i <= 3; ++i) {
        OLEDStringDraw ("                ", 0, i);
    }
}

//*****************************************************************************
// Displays the altitude percentage and yaw angle
//*****************************************************************************
void display_flight(int32_t mainDC, int32_t tailDC, int32_t yawAngle, int32_t altitude)
{
    uint32_t yawDec = abs(yawAngle % 10);

    // Displays altitude
    usnprintf(text_buffer, sizeof(text_buffer), "Altitude: %02d %c", altitude, 37);
    OLEDStringDraw (text_buffer, 0, 0);

    // Displays the yaw angle relative to starting position
    usnprintf(text_buffer, sizeof(text_buffer), "Yaw: %3d.%1d deg", yawAngle, yawDec);
    OLEDStringDraw (text_buffer, 0, 1);

    // Displays PWM output of MAIN ROTOR
    usnprintf(text_buffer, sizeof(text_buffer), "Main PWM: %02d %c", mainDC, 37);
    OLEDStringDraw (text_buffer, 0, 2);

    // Displays PWM output of TAIL ROTOR
    usnprintf(text_buffer, sizeof(text_buffer), "Tail PWM: %02d %c", tailDC, 37);
    OLEDStringDraw (text_buffer, 0, 3);
}

/* END OF SOURCE FILE */
