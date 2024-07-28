#ifndef ADC_POWER_H
#define ADC_POWER_H

#include <stdint.h>
#include "adc.h"
#include "radio.h"
#include "led_racer.h"

#define CIRCULAR_BUFFER_SIZE 255
#define THRESHOLD 525

typedef struct {
    uint16_t data[CIRCULAR_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} PowerADCBuffer;

void power_init(void);
void power_fill_buffer(void);
void power_fill_buffer_initial(void);

uint16_t power_adc_get_average(void);
void check_and_sleep(nrf24_t* nrf, ledbuffer_t* ledBuffer, uint8_t* powerLEDState);

#endif // ADC_POWER_H

