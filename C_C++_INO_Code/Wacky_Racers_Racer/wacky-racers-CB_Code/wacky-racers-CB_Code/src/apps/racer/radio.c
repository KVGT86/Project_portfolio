#include "Radio.h"
#include <stdio.h>
#include "usb_serial.h"
#include "pacer.h"
#include "pio.h"
#include "delay.h"
#include <string.h>
#include "motors.h" 
#include "target.h"

uint8_t BUMPER_STATE = 0;
uint8_t BUMPER_LATCH = 0;

nrf24_t* init_Radio(){
    spi_cfg_t spi_cfg =
    {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CS_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8
    };
    nrf24_cfg_t nrf24_cfg =
    {
        .channel = RADIO_CHANNEL,
        .address = RADIO_ADDRESS,
        .payload_size = RADIO_PAYLOAD_SIZE,
        .ce_pio = RADIO_CE_PIO,
        .irq_pio = RADIO_IRQ_PIO,
        .spi = spi_cfg,
    };
    nrf24_t *nrf;
    nrf = nrf24_init (&nrf24_cfg);
    
    return nrf;
}

void bumper_state(void)
{
    if (pio_input_get(BUMPER_PIO) != 1) {
        BUMPER_STATE = 1;
    } else {
        BUMPER_STATE = 0;
    }
}

void auto_Radio(nrf24_t *nrf, char* transmitBuffer, char* recieveBuffer) {
    char buzzerString[RADIO_PAYLOAD_SIZE];
    
    if (BUMPER_STATE){
        sprintf(transmitBuffer, "ON");
        printf("ATTEMPT TO SEND BUMP \n\r");
        if(nrf24_write(nrf, transmitBuffer, RADIO_PAYLOAD_SIZE)) {
            nrf24_listen(nrf);
            printf("TX: %s\n\r", transmitBuffer);

        } else {
            nrf24_read(nrf, recieveBuffer, RADIO_PAYLOAD_SIZE);
            process_motor_command(recieveBuffer);
        }
    } else {   
        nrf24_read(nrf, recieveBuffer, RADIO_PAYLOAD_SIZE);
        process_motor_command(recieveBuffer);
        printf("RX: %s\n\r", recieveBuffer);
    }
}

uint8_t current_channel = RADIO_CHANNEL;
uint8_t new_channel = RADIO_CHANNEL;
void channel_Radio(nrf24_t *nrf) {
    if (pio_input_get(RADIO_SWITCH_1) && !pio_input_get(RADIO_SWITCH_2) && !pio_input_get(RADIO_SWITCH_3) && !pio_input_get(RADIO_SWITCH_4)) {
        new_channel = 1;
    } else if (!pio_input_get(RADIO_SWITCH_1) && pio_input_get(RADIO_SWITCH_2) && !pio_input_get(RADIO_SWITCH_3) && !pio_input_get(RADIO_SWITCH_4)) {
        new_channel = 2;
    } else if (!pio_input_get(RADIO_SWITCH_1) && !pio_input_get(RADIO_SWITCH_2) && pio_input_get(RADIO_SWITCH_3) && !pio_input_get(RADIO_SWITCH_4)) {
        new_channel = 3;
    } else if (!pio_input_get(RADIO_SWITCH_1) && !pio_input_get(RADIO_SWITCH_2) && !pio_input_get(RADIO_SWITCH_3) && pio_input_get(RADIO_SWITCH_4)) {
        new_channel = 5;
    } 
    // printf("which channel %d\n\n", current_channel);

    if (new_channel != current_channel) {
        current_channel = new_channel;
        printf("Channel Selected: %d\n\r", current_channel);
        if(!nrf24_set_channel (nrf,  current_channel)) {
            printf("ERROR: CHANNEL NOT SET\n\r");
        }
    }

}  
