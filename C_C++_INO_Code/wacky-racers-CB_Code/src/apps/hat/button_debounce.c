#include "button_debounce.h"
#include "pio.h"

#define DEBOUNCE_DELAY 10 // Adjust as needed for your application

bool debounceSwitch(void) {
    static uint16_t state = 0;  // Static variable to store previous state
    state = (state << 1) | !pio_input_get(BUTTON_PIO) | 0xE000;  // Simulate reading input
    if (state == 0xF000) {
        // If the state is stable for 4 consecutive reads
        return true;  // Button is pressed
    }
    return false;  // Button is not pressed
}