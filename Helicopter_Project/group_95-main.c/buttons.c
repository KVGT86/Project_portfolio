/*******************************************************************************
 *
 * buttons.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the buttons on the ORBIT Launchpad / Tiva board.
 *
 * References:
 * This module was taken from Week 3 Lab (buttons4.c)
 *
 ******************************************************************************/

#include "buttons.h"

static bool buttonState[NUM_BUTS];
static uint8_t buttonCount[NUM_BUTS];
static bool buttonFlag[NUM_BUTS];
static bool buttonNormal[NUM_BUTS];

//*****************************************************************************
// Initialises GPIO for buttons
//*****************************************************************************
void init_buttons (void)
{
    int i;
    SysCtlPeripheralEnable (UP_BUT_PERIPH);
    GPIOPinTypeGPIOInput (UP_BUT_PORT_BASE, UP_BUT_PIN);
    GPIOPadConfigSet (UP_BUT_PORT_BASE, UP_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    buttonNormal[UP] = UP_BUT_NORMAL;

    SysCtlPeripheralEnable (DOWN_BUT_PERIPH);
    GPIOPinTypeGPIOInput (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN);
    GPIOPadConfigSet (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    buttonNormal[DOWN] = DOWN_BUT_NORMAL;

    SysCtlPeripheralEnable (LEFT_BUT_PERIPH);
    GPIOPinTypeGPIOInput (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN);
    GPIOPadConfigSet (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    buttonNormal[LEFT] = LEFT_BUT_NORMAL;

    SysCtlPeripheralEnable (RIGHT_BUT_PERIPH);
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= GPIO_PIN_0;
    GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    GPIOPinTypeGPIOInput (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN);
    GPIOPadConfigSet (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    buttonNormal[RIGHT] = RIGHT_BUT_NORMAL;

    for (i = 0; i < NUM_BUTS; i++) {

        buttonState[i] = buttonNormal[i];
        buttonCount[i] = 0;
        buttonFlag[i] = false;
    }
}

//*****************************************************************************
// Polls the buttons
//*****************************************************************************
void update_buttons(void)
{
    bool buttonValue[NUM_BUTS];
    int i;

    buttonValue[UP] = (GPIOPinRead (UP_BUT_PORT_BASE, UP_BUT_PIN) == UP_BUT_PIN);
    buttonValue[DOWN] = (GPIOPinRead (DOWN_BUT_PORT_BASE, DOWN_BUT_PIN) == DOWN_BUT_PIN);
    buttonValue[LEFT] = (GPIOPinRead (LEFT_BUT_PORT_BASE, LEFT_BUT_PIN) == LEFT_BUT_PIN);
    buttonValue[RIGHT] = (GPIOPinRead (RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN) == RIGHT_BUT_PIN);

    for (i = 0; i < NUM_BUTS; i++) {
        if (buttonValue[i] != buttonState[i]) {
            buttonCount[i]++;
            if (buttonCount[i] >= NUM_BUT_POLLS) {
                buttonState[i] = buttonValue[i];
                buttonFlag[i] = true;
                buttonCount[i] = 0;
            }
        } else {
            buttonCount[i] = 0;
        }
    }
}

//*****************************************************************************
// Checks the current state of the buttons
//*****************************************************************************
uint8_t check_button(uint8_t butName)
{
    if (buttonFlag[butName])
    {
        buttonFlag[butName] = false;

        if (buttonState[butName] == buttonNormal[butName]) {

            return RELEASED;
        }
        else {
            return PUSHED;
        }
    }
    return NO_CHANGE;
}

/* END OF SOURCE FILE */
