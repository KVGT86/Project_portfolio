// Name: Game Display Header file
//Purpose: Creates maps for wanted display sprites and diplays on LED Matrix (header file)
//Course: ENCE260
//Authors: Max Sigley and Caleb Barnes (Group324)
//Date: 15/10/2022
#ifndef GAME_DISPLAY 
#define GAME_DISPLAY

#include "system.h"

/** displays a column of the desired choice, eg show_choice(1, 3); */
void show_choice(uint8_t choice, uint8_t current_column);

/** displays a column of the result, eg showResult(0, 2); */
void show_result(uint8_t result, uint8_t current_column);

/** wipes the display */
void clear_display(void);

#endif //GAME_DISPLAY