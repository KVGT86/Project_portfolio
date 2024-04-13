/*******************************************************************************
 *
 * switch.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to SW1 (used for changing flight modes).
 *
 ******************************************************************************/
#include "switch.h"

bool switchPos;
bool isSwitchPosChanged;

//*****************************************************************************
// Initialises the GPIO for Switch 1 
//*****************************************************************************
void switch_init(void)
{
    SysCtlPeripheralEnable (SW1_PERIPH);
    GPIOPinTypeGPIOInput (SW1_PORT_BASE, SW1_PIN);
    GPIOPadConfigSet (SW1_PORT_BASE, SW1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    switchPos = GPIOPinRead(SW1_PORT_BASE, SW1_PIN) == SW1_PIN;
}

//*****************************************************************************
// Update the position of the switch to detemine whether it has changed position.
//*****************************************************************************
void update_switch(void)
{
    bool newSwitchPosition  = (GPIOPinRead (SW1_PORT_BASE, SW1_PIN) == SW1_PIN);

    // Checks if the switch has changed position
    if (newSwitchPosition != switchPos) {
        isSwitchPosChanged = true;
    }

    switchPos = newSwitchPosition;
}

//*****************************************************************************
// Checks the current position of the switch if the position has been changed
//*****************************************************************************
switchState_t check_switch(void)
{
    if ((isSwitchPosChanged == true)) {
        if (switchPos == 1) {
            return SW1_UP;
        } else {
            return SW1_DOWN;
        }
    } else {
        return SW1_NOT_CHANGED;
    }

}

/* END OF SOURCE FILE */
