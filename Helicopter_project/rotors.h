/*******************************************************************************
 *
 * rotors.h
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

#ifndef ROTORS_H_
#define ROTORS_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "driverlib/pin_map.h"
#include "buttons.h"

// PWM configuration
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER         4

#define PWM_MAIN_MIN_DUTY   2
#define PWM_TAIL_MIN_DUTY   2
#define PWM_MAIN_MAX_DUTY   98
#define PWM_TAIL_MAX_DUTY   50 // Max duty cycle set to 50% to avoid erratic movements
#define PWM_FREQUENCY      250

/* MAIN ROTOR MOTOR (module 0, PWM 7) */
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

/* TAIL ROTOR MOTOR (module 1, PWM 5) */
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

void init_motors(void);
void set_main_pwm (int32_t dutyCycle);
void set_tail_pwm (int32_t dutyCycle);
void init_main_pwm (void);
void init_tail_pwm (void);
void stop_tail_pwm(void);
void stop_main_pwm(void);
void start_main_pwm(void);
void start_tail_pwm(void);
int32_t get_main_dc(void);
int32_t get_tail_dc(void);

#endif /* ROTORS_H_ */
