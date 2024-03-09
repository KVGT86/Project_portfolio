/*******************************************************************************
 *
 * buffer.h
 *
 * ENCE361 Helicopter Project
 * Group 95
 *
 * Written by:
 *  - Sam Morphew   <smo214@uclive.ac.nz>   27906959
 *  - Caleb Barnes  <cba145@uclive.ac.nz>   98688440
 *
 * Description:
 * This module contains the functions relating to the circular buffer.
 *
 * References:
 * Parts of this module were taken from Week 4 Lab files (circBufT.c).
 *
 ******************************************************************************/

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include "stdlib.h"

typedef struct {
    uint32_t size;
    uint32_t writeIndex;
    uint32_t readIndex;
    uint32_t *data;
} buff_t;

uint32_t *init_buff(buff_t *buffer, uint32_t size);
void write_buff(buff_t *buffer, uint32_t entry);
uint32_t read_buff(buff_t *buffer);
void flush_buff(buff_t *buffer);

#endif /* BUFFER_H_ */
