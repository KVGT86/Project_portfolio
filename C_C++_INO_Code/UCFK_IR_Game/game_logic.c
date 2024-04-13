//Name: Game logic module.
//Purpose: Takes the two choices and outputs the winner
//Course: ENCE260
//Authors: Max Sigley and Caleb Barnes (Group324)
//Date: 15/10/2022

#include <stdint.h>
#include "game_logic.h"


/** Choices: 1 is paper, 2 is scissors, 3 is rock */
uint8_t test_winner(uint8_t choice, uint8_t opponent_choice) {

    /*Initialises Variables*/
    uint8_t test_result;

    if (choice == opponent_choice) {

        test_result = 2;
    } 

    if ((choice == 1) && (opponent_choice == 2)) { 

        test_result = 0;
    }

    if ((choice == 1) && (opponent_choice == 3)) {

        test_result = 1;
    }

    if ((choice == 2) && (opponent_choice == 1)) {

        test_result = 1;
    }

    if ((choice == 2) && (opponent_choice == 3)) {

        test_result = 0;
    }

    if ((choice == 3) && (opponent_choice == 2)) {

        test_result = 1;
    }

    if ((choice == 3) && (opponent_choice == 1)) {

        test_result = 0;
    }

    return test_result;
    
}

/** return values: 0 is lose, 1 is win, 2 is tie */