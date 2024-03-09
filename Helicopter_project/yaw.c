/*******************************************************************************
 *
 * yaw.c
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
#include "yaw.h"

uint32_t prevPhase = 0;
int32_t count = 0;
static int32_t desiredYaw;
static int32_t yaw;

//*****************************************************************************
// Calculates the yaw angle
//*****************************************************************************
int32_t calc_yaw_angle(void)
{
    yaw = ((count * 360 * ANGLE_PRECISION) / TICKS_PER_REVOLUTION) / (ANGLE_PRECISION);

    if (yaw <= MIN_YAW || yaw >= MAX_YAW) {
        yaw = 0;
        count = 0;
    }

    return yaw;
}

//*****************************************************************************
// Resets the desired yaw back to 0
//*****************************************************************************
void reset_desired_yaw(void)
{
    desiredYaw = 0;
}

//*****************************************************************************
// The interrupt service routine for finding the yaw reference point.
//*****************************************************************************
void yaw_ref_ISR(void)
{
    if ((get_flight_state() == TAKEOFF) || (get_flight_state() == LANDING)) {
        // Reset yaw values to 0 once reference point has been found
        count = 0;
        desiredYaw = 0;
    }
    
    GPIOIntClear(GPIO_PORTC_BASE, YAW_REFERENCE_PIN);
}

//*****************************************************************************
// Updates the desired yaw by a specified amount.
//*****************************************************************************
int32_t update_desired_yaw(int16_t degreesChange)
{
    desiredYaw = desiredYaw + degreesChange;
    if (desiredYaw > MAX_YAW) {
        desiredYaw = desiredYaw - MAX_YAW;
    }

    if (desiredYaw < MIN_YAW) {
        desiredYaw = desiredYaw + MAX_YAW;
    }

    return desiredYaw;
}

//*****************************************************************************
// Returns the current desired yaw 
//*****************************************************************************
int32_t get_desired_yaw(void)
{
    return desiredYaw;
}

//*****************************************************************************
// Returns the current yaw error
//*****************************************************************************
int32_t yaw_error(void)
{
    int32_t yawValue = calc_yaw_angle();

    return desiredYaw - yawValue;
}

//*****************************************************************************
// The interrupt handler for the for quadrature encoder interrupt.
//*****************************************************************************
void yaw_int_handler(void)
{
    uint8_t currentPhase = GPIOPinRead(GPIO_PORTB_BASE, PHASE_A | PHASE_B);

    switch (currentPhase)
    {
        case(0b00):
            if (prevPhase == 0b10)
            {
                count --;
            } else if (prevPhase == 0b01)
            {
                count ++;
            }
        break;

        case(0b01):
            if (prevPhase == 0b00)
            {
                count --;
            } else if (prevPhase == 0b11)
            {
                count ++;
            }
        break;

        case(0b10):
            if (prevPhase == 0b11)
            {
                count --;
            } else if (prevPhase == 0b00)
            {
                count ++;
            }
        break;

        case(0b11):
            if (prevPhase == 0b01)
            {
                count --;
            } else if (prevPhase == 0b10)
            {
                count ++;
            }
        break;

    }
    prevPhase = currentPhase;
    GPIOIntClear(GPIO_PORTB_BASE, PHASE_A | PHASE_B);
}

//*****************************************************************************
// Initialises the GPIO for interrupt handling of the quadrature encoder
//*****************************************************************************
void init_yaw_GPIO(void)
{
    /* YAW A & B INTERRUPTS */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, PHASE_A | PHASE_B);
    GPIOIntRegister(GPIO_PORTB_BASE, yaw_int_handler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, PHASE_A | PHASE_B, GPIO_BOTH_EDGES);
    prevPhase = GPIOPinRead(GPIO_PORTB_BASE, PHASE_A | PHASE_B);
    GPIOIntEnable(GPIO_PORTB_BASE, PHASE_A | PHASE_B);

    /* YAW REFERENCE SIGNAL INTERRUPT */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, YAW_REFERENCE_PIN);
    GPIOIntRegister(GPIO_PORTC_BASE, yaw_ref_ISR);
    // Interrupt to be triggered by falling edges, as the yaw reference signal is active LOW.
    GPIOIntTypeSet(GPIO_PORTC_BASE, YAW_REFERENCE_PIN, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTC_BASE, YAW_REFERENCE_PIN);
}

/* END OF SOURCE FILE */
