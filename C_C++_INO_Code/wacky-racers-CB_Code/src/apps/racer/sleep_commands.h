#include "mcu_sleep.h" 
#include "adc.h"
#include "target.h"
#include "delay.h"
#include "panic.h"

#ifndef SLEEP_COMMANDS_H
#define SLEEP_COMMANDS_H


/*********DEFINES**********/
// Define the reference voltage (in volts) for the system
#define V_REF 3.3
// Define the maximum value that can be represented by the 12-bit ADC
#define ADC_SIZE 4095 // 2^(n-1) where n=12 as SAM4S has a 12-bit ADC
// Define the voltage cutoff (in volts) for the system
#define V_CUTOFF 5
// Define the voltage tolerance (in volts) for the system
#define TOLERANCE 25 // Gives a 0.2V tolerance


/*********STRUCTS***********/
// Configuration structure for the MCU sleep mode
static const mcu_sleep_cfg_t mcu_sleep_mode =
{
   .mode = MCU_SLEEP_MODE_SLEEP// Set the sleep mode to the specified state
};

// Configuration structure for the ADC to measure V_BATT
static const adc_cfg_t vbatt_adc_cfg =
{
   .bits = 12, // 12-bit ADC resolution
   .channels = BIT(ADC_CHANNEL_4), // Select ADC channel 4 for V_BATT measurement
   .trigger = ADC_TRIGGER_SW, // Software-triggered ADC conversion
   .clock_speed_kHz = 1000 // Set the ADC clock speed to 1000 kHz
};

// Configuration structure for the ADC to measure 3.3V reference (not used in this implementation)
// static const adc_cfg_t 3v3_ref_cfg_t =
// {
//    .bits = 12,
//    .channels = BIT(ADC_CHANNEL_6),
//    .trigger = ADC_TRIGGER_SW,
//    .clock_speed_kHz = 1000
// };


/*********FUNCTION PROTOTYPES*********/
// Function to initialize the ADC
void init_adc(void);
// Function to check the voltage level
void check_voltage(void);
//Shits down mcu and non critical periphals
void mcu_low_voltage(void);

#endif //SLEEP_COMMAND_H