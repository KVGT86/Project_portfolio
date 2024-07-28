//Standard C/C+ libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//User defined libs
#include "darstadly_stuff.h"
#include "radio.h"
#include "motors.h"
#include "sleep_commands.h"
#include "usb_commands.h"
#include "led_racer.h"
#include "low_voltage.h"

//SAM4S spefic libs
#include "sys.h"
#include "pacer.h"
#include "pio.h"
#include "panic.h"
#include "target.h"
#include "sysclock.h"

#define PACER_RATE 50
#define LED_FLASH_RATE 2

#define RADIO_TASK_PERIOD 2
#define BUMPER_TASK_PERIOD 7
#define LED_FLASH_PERIOD 173
#define LED_TASK_PERIOD 3
#define RADIO_CHANNEL_PERIOD 43
#define VOLTAGE_CHECK_PERIOD 101
#define SERVO_PERIOD 750
#define FILL_BUFFER_PERIOD 17

volatile uint64_t ticks = 0;
uint8_t powerLEDState = 1;

void radio_tasks(nrf24_t *nrf)
{
    char transmitBuffer[RADIO_PAYLOAD_SIZE];
    char recieveBuffer[RADIO_PAYLOAD_SIZE];

    auto_Radio(nrf, transmitBuffer, recieveBuffer);
}

void bumper_tasks(void)
{
    bumper_state();
}

void led_flash_tasks(void)
{
    if (powerLEDState != 0) {
        pio_output_toggle(LED_STATUS_PIO);  
    };
}

void led_strip_tasks(Color* gradient,  ledbuffer_t* ledBuffer)
{
    color_map_to_strip(gradient, ledBuffer);
    shiftGradient(gradient, ledBuffer->leds, 10);
}

void check_channel_tasks(nrf24_t* nrf)
{
    channel_Radio(nrf);
}

void adc_voltage_tasks(nrf24_t* nrf, ledbuffer_t* ledBuffer, uint8_t* powerLEDState)
{
    check_and_sleep(nrf, ledBuffer, powerLEDState);
}

void update_ticks(void)
{
    ticks++;
}

int main (void)
{
    //Library defined inits
    usb_serial_stdio_init();

    sysclock_init();
    sysclock_callback(update_ticks);
    

    //USer defined inits
    nrf24_t *nrf = init_Radio();
    init_pwm();
    init_leds();
    power_init();
    init_servo_pwm();

    // /* Radio Channel Select.  */
    // /* Configure button PIO as input with pullup.  */
    pio_config_set (BUTTON_PIO, PIO_PULLUP);
    pio_config_set (RADIO_SWITCH_1, PIO_PULLUP);
    pio_config_set (RADIO_SWITCH_2, PIO_PULLUP);
    pio_config_set (RADIO_SWITCH_3, PIO_PULLUP);
    pio_config_set (RADIO_SWITCH_4, PIO_PULLUP);


    /*Set up a List of colors to use (RGB)*/
    Color colors[] = {
        {255, 0, 0},
        {255, 0, 255},
        {0, 255, 0}, 
        {0, 0, 255},
        {255, 119, 0}, 
        {255, 255, 255},
    };

    ledbuffer_t* ledBuffer = ledbuffer_init(LEDTAPE_PIO, 22);
    int numColors = sizeof(colors) / sizeof(colors[0]);
    Color color_map[ledBuffer->leds];
    init_color_map(ledBuffer, color_map);
    Color gradient[ledBuffer->leds];
    createGradient(colors, numColors, ledBuffer->leds, gradient);

    while (1)
    {
        if (ticks % RADIO_TASK_PERIOD == 0)
        {
            radio_tasks(nrf);
        }
        if (ticks % BUMPER_TASK_PERIOD == 0)
        {
            bumper_state();
        }
        if (ticks % LED_FLASH_PERIOD == 0)
        {
            led_flash_tasks();
        }
        if (ticks % LED_TASK_PERIOD == 0)
        {
            led_strip_tasks(gradient, ledBuffer);
        }
        if (ticks % RADIO_CHANNEL_PERIOD == 0)
        {
            check_channel_tasks(nrf);
        }
        if (ticks % VOLTAGE_CHECK_PERIOD == 0) 
        {
            adc_voltage_tasks(nrf, ledBuffer, &powerLEDState);
        }
        if (ticks % FILL_BUFFER_PERIOD == 0)
        {
            power_fill_buffer();
        }
    }
}
