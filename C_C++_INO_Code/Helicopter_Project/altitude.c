/*******************************************************************************
 *
 * altitude.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions for controlling the altitude of the helicopter.
 *
 * References:
 * Parts of this module were taken from Week 4 Lab (ADCdemo1.c)
 *
 ******************************************************************************/

#include "altitude.h"
#include "buffer.h"

static int32_t ADCMeanVal;
static int32_t refADC;
static int32_t desiredAltitude;
static int32_t altitude;

//*****************************************************************************
// Returns the current desired altitude.
//*****************************************************************************
int32_t get_desired_alt(void)
{
    return desiredAltitude;
}

//*****************************************************************************
// Calculates the rounded mean average ADC value by reading the circular buffer.
//*****************************************************************************
int32_t calc_avg_ADC(void)
{
    int32_t sum = 0;
    uint16_t i;

    for (i = 0; i < BUFFER_SIZE; i++) {
        sum = sum + read_buff(&g_inBuffer);
    }
    ADCMeanVal = (((2 * sum + BUFFER_SIZE) / 2) / BUFFER_SIZE);
    return ADCMeanVal;
}

//*****************************************************************************
// Sets the reference ADC reading as the current ADC reading.
// Called upon initialisation of the programme.
//*****************************************************************************
int32_t set_ref_ADC(void)
{
    refADC = calc_avg_ADC();
    return refADC;
}

//*****************************************************************************
// Calculates the percentage altitude
//*****************************************************************************
int32_t calc_altitude(void)
{
    int32_t ADCMax = refADC + (MAX_ADC / 3.3);
    int32_t ADCMin = refADC;
    altitude = (((refADC - calc_avg_ADC()) * 100) / (ADCMax - ADCMin));

    return altitude;
}

//*****************************************************************************
// Calculates the current altitude error
//*****************************************************************************
int32_t altitude_error(void)
{
    return desiredAltitude - calc_altitude();
}

//*****************************************************************************
// Increments the desired altitude by a specified amount.
//*****************************************************************************
int32_t update_desired_altitude(int16_t change)
{
    desiredAltitude = desiredAltitude + change;

    // Ensure that the altitude cannot exceed 100% or go below 0%
    if (desiredAltitude > MAX_ALTITUDE) {
        desiredAltitude = MAX_ALTITUDE;
    }
    if (desiredAltitude < MIN_ALTITUDE) {
        desiredAltitude = MIN_ALTITUDE;
    }
    return desiredAltitude;
}

//*****************************************************************************
// Returns the current altitude.
//*****************************************************************************
int32_t get_altitude(void)
{
    return altitude;
}

//*****************************************************************************
// Sets the altitude to a given value
//*****************************************************************************
void set_altitude(int32_t newAltitude)
{
    altitude = newAltitude;
}

//*****************************************************************************
// Resets the desired altitude to 0% (ground level).
//*****************************************************************************
void reset_desired_altitude(void)
{
    desiredAltitude = 0;
}

//*****************************************************************************
// Initialises the ADC GPIO and interrupt handler for the ISR
//*****************************************************************************
void init_ADC(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 3);

    ADCIntRegister (ADC0_BASE, 3, ADC_int_handler);

    ADCIntEnable(ADC0_BASE, 3);
}

//*****************************************************************************
// Interrupt service routine for the ADC interrupt.
// Writes the current reading at the ADC pin to the circular buffer.
//*****************************************************************************
void ADC_int_handler(void)
{
    uint32_t ulValue;
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    write_buff(&g_inBuffer, ulValue);
    ADCIntClear(ADC0_BASE, 3);
}

/* END OF SOURCE FILE */
