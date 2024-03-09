/*******************************************************************************
 *
 * reset.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the virtual reset.
 *
 ******************************************************************************/

#include "reset.h"

#define RESET_PORT_BASE GPIO_PORTA_BASE
#define RESET_PIN GPIO_PIN_6
#define RESET_PERIPH SYSCTL_PERIPH_GPIOA

//*****************************************************************************
// Interrupt handler for virtual reset
//*****************************************************************************
void reset_int(void)
{
    GPIOIntClear(RESET_PORT_BASE, RESET_PIN);
    SysCtlReset();
}

//*****************************************************************************
// Initialises the virtual reset interrupt handler
//*****************************************************************************
void init_reset(void)
{
    SysCtlPeripheralEnable (RESET_PERIPH);

    GPIOPinTypeGPIOInput(RESET_PORT_BASE, RESET_PIN);
    GPIOPadConfigSet(RESET_PORT_BASE, RESET_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIODirModeSet(RESET_PORT_BASE, RESET_PIN, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(RESET_PORT_BASE, RESET_PIN, GPIO_FALLING_EDGE);
    GPIOIntRegister(RESET_PORT_BASE, reset_int);
    GPIOIntEnable(RESET_PORT_BASE, RESET_PIN);
}

/* END OF SOURCE FILE */