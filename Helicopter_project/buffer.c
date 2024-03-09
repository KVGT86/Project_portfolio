/*******************************************************************************
 *
 * buffer.c
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

#include "buffer.h"

//*****************************************************************************
// Initialises the circular buffer
//*****************************************************************************
uint32_t *init_buff(buff_t *buffer, uint32_t size)
{
    buffer->writeIndex = 0;
    buffer->readIndex = 0;
    buffer->size = size;
    buffer->data = (uint32_t *) calloc (size, sizeof(uint32_t));

    return buffer->data;
}

//*****************************************************************************
// Writes to the circular buffer
//*****************************************************************************
void write_buff(buff_t *buffer, uint32_t entry)
{
    buffer->data[buffer->writeIndex] = entry;
    buffer->writeIndex++;

    if (buffer->writeIndex >= buffer->size) {
       buffer->writeIndex = 0;
    }
}

//*****************************************************************************
// Reads the circular buffer
//*****************************************************************************
uint32_t read_buff(buff_t *buffer) {

    uint32_t entry;

    entry = buffer->data[buffer->readIndex];
    buffer->readIndex++;

    if (buffer->readIndex >= buffer->size) {
       buffer->readIndex = 0;
    }
    return entry;
}

//*****************************************************************************
// Empties the circular buffer
//*****************************************************************************
void flush_buff(buff_t *buffer)
{
    buffer->writeIndex = 0;
    buffer->readIndex = 0;
    buffer->size = 0;
    free(buffer->data);
    buffer->data = NULL;
}

/* END OF SOURCE FILE */