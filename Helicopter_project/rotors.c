/*******************************************************************************
 *
 * rotors.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the main and tail rotors.
 *
 * References:
 * Parts of this module were taken from Week 3 Lab (pwmGen.c)
 *
 ******************************************************************************/

#include "rotors.h"

static int32_t mainDC;
static int32_t tailDC;

//*****************************************************************************
// Initialises the PWM outputs for the main motor
//*****************************************************************************
void init_main_pwm (void)
{
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the PWM output
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

//*****************************************************************************
// InitialisesPWM outputs for the tail motor
//*****************************************************************************
void init_tail_pwm (void)
{
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the PWM output
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);

}

//*****************************************************************************
// Sets the main PWM according to a given duty cycle
//*****************************************************************************
void set_main_pwm (int32_t dutyCycle)
{
    if (dutyCycle > PWM_MAIN_MAX_DUTY) {
        dutyCycle = PWM_MAIN_MAX_DUTY;

    } else if (dutyCycle < PWM_MAIN_MIN_DUTY) {
        dutyCycle = 0;
    }
    int32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQUENCY;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, period * dutyCycle / 100);
    mainDC = dutyCycle;
}


//*****************************************************************************
// Sets the tail PWM according to a given duty cycle
//*****************************************************************************
void set_tail_pwm (int32_t dutyCycle)
{
    if (dutyCycle > PWM_TAIL_MAX_DUTY) {
        dutyCycle = PWM_TAIL_MAX_DUTY;

    } else if (dutyCycle < PWM_TAIL_MIN_DUTY) {
        dutyCycle = 0;
    }
    int32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQUENCY;

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, period * dutyCycle / 100);
    tailDC = dutyCycle;
}


//*****************************************************************************
// Initialises the main and tail motors
//*****************************************************************************
void init_motors(void)
{
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
    init_main_pwm();
    init_tail_pwm();
}


//*****************************************************************************
// Stops the tail motor
//*****************************************************************************
void stop_tail_pwm()
{
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
    set_tail_pwm(0);
}

//*****************************************************************************
// Stops the main motor
//*****************************************************************************
void stop_main_pwm()
{
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
    set_main_pwm(0);
}

//*****************************************************************************
// Starts the main motors
//*****************************************************************************
void start_main_pwm()
{
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    set_main_pwm(PWM_MAIN_MIN_DUTY);
}

//*****************************************************************************
// Starts the tail motors
//*****************************************************************************
void start_tail_pwm()
{
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
    set_tail_pwm(PWM_TAIL_MIN_DUTY);
}

//*****************************************************************************
// Returns the current duty cycle of the main motor
//*****************************************************************************
int32_t get_main_dc()
{
    return mainDC;
}

//*****************************************************************************
// Returns the current duty cycle of the tail motor
//*****************************************************************************
int32_t get_tail_dc()
{
    return tailDC;
}

/* END OF SOURCE FILE */
