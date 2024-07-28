#ifndef BUTTON_DEBOUNCE_H
#define BUTTON_DEBOUNCE_H
#include <stdbool.h> // Include for bool type
#include <stdint.h>  // Include for uint16_t type
#include "pio.h"

bool debounceSwitch(pio_t button);

#endif /* LOW_PASS_FILTER_H */
