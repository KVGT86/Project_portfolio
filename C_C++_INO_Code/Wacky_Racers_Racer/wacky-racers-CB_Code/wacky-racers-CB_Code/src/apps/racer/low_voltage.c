#include "sleep.h"
#include "mcu_sleep.h"
#include "cpu.h"
#include "irq.h"
#include "target.h"
#include "low_voltage.h"
#include "adc.h"
#include <stdio.h>
#include "usb_serial.h"
#include "delay.h"
#include "led_racer.h"
#include "radio.h"
#include "pio.h"
#include "ledbuffer.h"
#include "nrf24.h"
#include "motors.h"
#include "button_debounce.h"

// Refer to pwm pins
extern pwm_t pwm_AIN1;
extern pwm_t pwm_BIN1;

// Define the buffer
static PowerADCBuffer adc_buffer = { .head = 0, .tail = 0, .count = 0 };

// Define the ADC device
adc_t adc_device;

// ADC configuration
static const adc_cfg_t adc_cfg = {
    .bits = 10,                  // 10-bit resolution
    .channel = ADC_CHANNEL_4,    // Channel to read
    .clock_speed_kHz = 1000,     // ADC clock speed
    .trigger = ADC_TRIGGER_SW    // Software trigger
};

// Initialize the ADC
void power_init(void) {
    // Initialize the ADC with the given configuration
    adc_device = adc_init(&adc_cfg);
    if (!adc_device) {
        // Handle ADC initialization failure
        while(1) {
            printf("Failed to initialize ADC\n");
        }
    }

    // Fill the buffer initially to ensure accurate averaging
    power_fill_buffer_initial();
}

// Function to fill the buffer with ADC data
void power_fill_buffer(void) {
    uint16_t adc_value;
    // Read a new ADC value
    if (adc_read(adc_device, &adc_value, sizeof(adc_value)) == sizeof(adc_value)) {
        adc_buffer.data[adc_buffer.head] = adc_value;
        adc_buffer.head = (adc_buffer.head + 1) % CIRCULAR_BUFFER_SIZE;
        if (adc_buffer.count < CIRCULAR_BUFFER_SIZE) {
            adc_buffer.count++;
        } else {
            adc_buffer.tail = (adc_buffer.tail + 1) % CIRCULAR_BUFFER_SIZE;
        }
    } else {
        // Handle ADC read failure
        printf("Failed to read ADC value\n");
    }
}

// Function to fill the entire buffer initially
void power_fill_buffer_initial(void) {
    uint16_t adc_value;
    for (uint8_t i = 0; i < CIRCULAR_BUFFER_SIZE; i++) {
        // Read a new ADC value
        if (adc_read(adc_device, &adc_value, sizeof(adc_value)) == sizeof(adc_value)) {
            adc_buffer.data[i] = adc_value;
            adc_buffer.count++;
        } else {
            // Handle ADC read failure
            printf("Failed to read ADC value\n");
        } 
    }
    adc_buffer.head = 0;
    adc_buffer.tail = 0;
}

// Calculate the average of the ADC buffer
uint16_t power_adc_get_average(void) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < adc_buffer.count; i++) {
        sum += adc_buffer.data[i];
    }
    if (adc_buffer.count != 0) {
        return (uint16_t)(sum / adc_buffer.count);
    } else {
        return 0;
    }
}

// extern adc_t adc_device;

// static const mcu_sleep_wakeup_cfg_t wakeup_cfg = {
//     .pio = BUTTON_PIO,
//     .active_high = true // Adjust this based on your button configuration
// };

uint16_t lowPowerFlash_counter = 0;
// void check_and_sleep(ledbuffer_t* ledBuffer, pwm_t buzzer, nrf24_t* radio)
void check_and_sleep(nrf24_t* nrf, ledbuffer_t* ledBuffer, uint8_t* powerLEDState)
{
    // Check the ADC output
    uint16_t adc_output = power_adc_get_average();
    printf("ADC: %d\n", adc_output);
    // If ADC output is less than threshold, put MCU to sleep
    if (adc_output < THRESHOLD || debounceSwitch(BUTTON_PIO))
    {
        printf("sleep\n");
        pio_output_high(LED_ACTIVE_PIO);
        pio_output_high(LED_STATUS_PIO);

        ledOff(ledBuffer);
        *powerLEDState = 0;
        nrf24_power_down(nrf);
        pwm_power_down();

        while (!debounceSwitch(BUTTON_PIO)) {
            power_fill_buffer();
            if (lowPowerFlash_counter % 10 == 0){
                pio_output_low(LED_ERROR_PIO);
                delay_ms(100);
                pio_output_high(LED_ERROR_PIO);

            }
            delay_ms(100);
            lowPowerFlash_counter ++;

        }
        *powerLEDState = 1;
        pio_output_high(LED_ERROR_PIO);
        nrf24_power_up(nrf);
        init_pwm();
    }
}

