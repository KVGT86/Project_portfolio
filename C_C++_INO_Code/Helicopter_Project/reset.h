/*******************************************************************************
 *
 * reset.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the virtual reset.
 *
 ******************************************************************************/

#ifndef RESET_H_
#define RESET_H_

#include "buttons.h"
#include <stdio.h>
#include <stdint.h>
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"

void reset_int(void);
void init_reset(void);

#endif /* RESET_H_ */
