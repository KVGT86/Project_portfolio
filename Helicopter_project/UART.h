/*******************************************************************************
 *
 * UART.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the serial communication via UART.
 * Adapted from P.J. Bones UCECE (uartDemo.c)
 *
 ******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "altitude.h"
#include "yaw.h"
#include "flight_mode.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include <stdio.h>
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "stdlib.h"

/*
 * Constants
 */
#define MAX_STR_LEN 32
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX

/*
 * Globals
 */
char uartStr[MAX_STR_LEN + 1];

/*
 * Function Prototypes
 */
void initialise_USB_UART (void);
void UART_Send (char *uartBuff);
void UART_print_func(void);

#endif /* UART_H_ */
