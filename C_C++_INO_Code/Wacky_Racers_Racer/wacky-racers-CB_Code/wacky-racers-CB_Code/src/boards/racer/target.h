/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a racer!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000

/* USB  */
#define USB_VBUS_PIO PA29_PIO
#define USB_CURRENT_MA 500

// /* LEDs  */

#define LED_ERROR_PIO PA27_PIO      //RED
#define LED_STATUS_PIO PA26_PIO     //GREEN
#define LED_ACTIVE_PIO PA25_PIO     //BLUE

/* General  */
#define APPENDAGE_PIO PA1_PIO
#define SERVO_PWM_PIO PA2_PIO
#define BUMPER_PIO PA23_PIO
#define VA_REF PB2_PIO  //idek why greyed
#define VBATT_MEAS_PIO PB0_PIO

/* Button  */
#define BUTTON_PIO PA28_PIO

// /* H-bridges   */
#define MOTOR_AIN1_PIO PA0_PIO    //PWMH0 PWMAH AIN1
#define MOTOR_AIN2_PIO PA19_PIO   //PWML0 PWMAL AIN2
#define MOTOR_BIN1_PIO PA1_PIO    //PWMH1 PWMBH BIN1
#define MOTOR_BIN2_PIO PA20_PIO   //PWML1 PWMBL BIN2

#define N_SLEEP_HBRIDGE PA18_PIO        //HBRIDGE nSLEEP PIN
#define N_FAULT PA31_PIO                //HBRIDGE nFAULT PIN

/* Radio  */
#define RADIO_CS_PIO PA30_PIO
#define RADIO_CE_PIO PA15_PIO
#define RADIO_IRQ_PIO PA16_PIO

/* Radio channel switches*/
#define RADIO_SWITCH_1 PA11_PIO
#define RADIO_SWITCH_2 PA10_PIO
#define RADIO_SWITCH_3 PA9_PIO
#define RADIO_SWITCH_4 PA8_PIO

/* LED tape  */
#define LEDTAPE_PIO PA24_PIO

#endif /* TARGET_H  */
