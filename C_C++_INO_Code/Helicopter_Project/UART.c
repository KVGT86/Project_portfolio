/*******************************************************************************
 *
 * UART.c
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
 *
 * References: 
 * Adapted from P.J. Bones UCECE (uartDemo.c)
 *
 ******************************************************************************/

#include "UART.h"

char uartStr[MAX_STR_LEN + 1];

//*****************************************************************************
// Initialises the UART outputs 
//*****************************************************************************
void initialise_USB_UART (void)
{
    // Enable GPIO port A which is used for UART0 pins.
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);

    // Select the alternate (UART) function for these pins.
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    GPIOPinConfigure (GPIO_PA0_U0RX);
    GPIOPinConfigure (GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
    UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}

//*****************************************************************************
// Sends contents of the buffer to UART 
//*****************************************************************************
void UART_Send (char *uartBuff)
{
    while (*uartBuff)
    {
        UARTCharPut(UART_USB_BASE, *uartBuff);
        uartBuff++;
    }
}

//*****************************************************************************
// Generates strings to send to UART. These are sent via a function call to UART_send
//*****************************************************************************
void UART_print_func(void)
{
    int32_t yaw = calc_yaw_angle();
    int32_t desiredYaw = get_desired_yaw();
    int32_t altitude = get_altitude();
    int32_t desiredAltitude = get_desired_alt();
    int32_t tailDC = get_tail_dc();
    int32_t mainDC = get_main_dc();
    const char* flightState = get_flight_state_string();

    // Send break
    usprintf(uartStr, "*************** \r\n \0");
    UART_Send(uartStr);

    // Send yaw
    usprintf(uartStr, "Yaw: %03d [%03d] deg  \r\n \0", yaw, desiredYaw);
    UART_Send(uartStr);

    // Send altitude
    usprintf(uartStr, "Alt: %02d%c [%02d%c]  \r\n \0", altitude, 37, desiredAltitude, 37);
    UART_Send(uartStr);

    // Send tail PWM duty cycle
    usprintf(uartStr, "Tail: %02d %c    \r\n \0", tailDC, 37);
    UART_Send(uartStr);

    // Send main PWM duty cycle
    usprintf(uartStr, "Main: %02d %c    \r\n \0", mainDC, 37);
    UART_Send(uartStr);

    // Send current flight state
    usprintf(uartStr, "State: %s.       \r\n \0", flightState);
    UART_Send(uartStr);
}

/* END OF SOURCE FILE */
