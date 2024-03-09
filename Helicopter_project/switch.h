/*******************************************************************************
 *
 * switch.c
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to SW1 (used for changing flight modes).
 *
 ******************************************************************************/
#ifndef SWITCH_H_
#define SWITCH_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "flight_mode.h"

#define SW1_PERIPH      SYSCTL_PERIPH_GPIOA
#define SW1_PORT_BASE   GPIO_PORTA_BASE
#define SW1_PIN         GPIO_PIN_7

enum switchStates {SW1_NOT_CHANGED = 0, SW1_UP, SW1_DOWN};
typedef enum switchStates switchState_t;

void switch_init(void);
void update_switch(void);
switchState_t check_switch(void);

#endif /* SWITCH_H_ */
