#ifndef RADIO_H
#define RADIO_H
#include "nrf24.h"

#define RADIO_CHANNEL 5
// #define RADIO_ADDRESS 0xB00B1E5420LL
#define RADIO_ADDRESS 0x0123456789LL
#define RADIO_PAYLOAD_SIZE 32

nrf24_t* init_Radio(void);
// void auto_Radio(nrf24_t *nrf, char* transmitBuffer, char* recieveBuffer, int64_t clockCycles);
void auto_Radio(nrf24_t *nrf, char* transmitBuffer, char* recieveBuffer);
void channel_Radio(nrf24_t *nrf);
void bumper_state(void);

#endif

// //radio libraries
// #include "nrf24.h"
// #include "spi.h"
// #include "pio.h"
// #include "target.h"
// #include "motors.h"

// #ifndef RADIO_H
// #define RADIO_H

// // #define RADIO_CHANNEL 1
// // #define RADIO_ADDRESS 0xB00B135420LL
// // #define RADIO_PAYLOAD_SIZE 32

// #define RADIO_CHANNEL 5
// #define RADIO_ADDRESS 0x0123456789LL
// #define RADIO_PAYLOAD_SIZE 32

// const spi_cfg_t spi_cfg =
// {
//     .channel = 0,
//     .clock_speed_kHz = 1000,
//     .cs = RADIO_CS_PIO,
//     .mode = SPI_MODE_0,
//     .cs_mode = SPI_CS_MODE_FRAME,
//     .bits = 8
// };

// nrf24_cfg_t nrf24_cfg =
// {
//     .channel = RADIO_CHANNEL,
//     .address = RADIO_ADDRESS,
//     .payload_size = RADIO_PAYLOAD_SIZE,
//     .ce_pio = RADIO_CE_PIO,
//     .irq_pio = RADIO_IRQ_PIO,
//     .spi = spi_cfg,
// };

// void radio_init(void);

// void radio_tx(void);

// void radio_rx(void);

// void bumper_state(void);

// #endif //RADIO_H