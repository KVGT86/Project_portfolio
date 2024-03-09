/*******************************************************************************
 *
 * main.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module includes a round-robin kernel with time-triggered events to schedule events involved in the control of the helicopter.
 *
 ******************************************************************************/

#include <flight_mode.h>
#include <PID_controller.h>
#include "switch.h"
#include "buffer.h"
#include "buttons.h"
#include "display.h"
#include "altitude.h"
#include "yaw.h"
#include "rotors.h"
#include "reset.h"
#include "UART.h"

#define BUFFER_SIZE         10
#define SAMPLE_RATE_HZ      80
#define TICKS_PER_SECOND    SAMPLE_RATE_HZ / 3
#define SIX_HZ_TICKS        TICKS_PER_SECOND / 6
#define TEN_HZ_TICKS        TICKS_PER_SECOND / 10
#define FOUR_HZ_TICKS       TICKS_PER_SECOND / 4

buff_t g_inBuffer;

bool flagTenHz = false;
bool flagSixHz = false;
bool flagFourHz = false;

//*****************************************************************************
// Interrupt handler for the SysTick ISR
//*****************************************************************************
void sys_tick_int_handler(void)
{
    static uint32_t SixHzCounter = 0;
    static uint32_t TenHzCounter = 0;
    static uint32_t FourHzCounter = 0;

    ADCProcessorTrigger(ADC0_BASE, 3);

    // Increment every sysTick interrupt
    SixHzCounter++;
    TenHzCounter++;
    FourHzCounter++;

    if (SixHzCounter >= SIX_HZ_TICKS) {
        SixHzCounter = 0;
        flagSixHz = true;
    }
    if (TenHzCounter >= TEN_HZ_TICKS) {
        TenHzCounter = 0;
        flagTenHz = true;
    }
    if (FourHzCounter >= FOUR_HZ_TICKS) {
        FourHzCounter = 0;
        flagFourHz = true;
    }
}

//*****************************************************************************
// Initialises the SysTick interrupt handler
//*****************************************************************************
void init_sys_tick(void)
{
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlDelay(100);
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    SysTickIntRegister(sys_tick_int_handler);
    SysTickIntEnable();
    SysTickEnable();
}

//*****************************************************************************
// Main function containing programme loop and scheduler
//*****************************************************************************
int main(void)
{
    init_sys_tick();
    init_ADC();
    init_display();
    init_yaw_GPIO();
    init_buff(&g_inBuffer, BUFFER_SIZE);
    init_motors();
    init_buttons();
    switch_init();
    init_reset();
    initialise_USB_UART();

    IntMasterEnable();

    SysCtlDelay (SysCtlClockGet() / 3); // 1 second delay to allow time for buffer to empty

    set_ref_ADC();
    display_blank(); // Initialises the ORBIT display

    // Begin round-robin kernel with time-triggered events
    while (1)
    {
        // PID controller and button/switch refresh events will be updated at a rate of 10Hz
        if (flagTenHz) {
            update_flight_mode();
            update_switch();
            update_buttons();
            flagTenHz = false;
        }
        // Display refreshed at a rate of 6Hz
        if (flagSixHz) {
            display_flight(get_main_dc(), get_tail_dc(), calc_yaw_angle(), calc_altitude());
            flagSixHz = false;
        }
        // UART will be updated at a rate of 4Hz
        if (flagFourHz) {
            UART_print_func();
            flagFourHz = false;
        }
    }
}

/* END OF SOURCE FILE */
