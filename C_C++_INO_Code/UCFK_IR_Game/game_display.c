// Name: Game Display Module
//Purpose: Creates maps for wanted display sprites and diplays on LED Matrix
//Course: ENCE260
//Authors: Max Sigley and Caleb Barnes (Group324)
//Date: 15/10/2022

#include "system.h"
#include "font5x7_1.h"
#include "pio.h"

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/** Defines LED maps*/
static uint8_t paper_map[] = {0x00, 0x3E, 0x3E, 0x3E, 0x00};
static uint8_t scissor_map[] = {0x33, 0x34, 0x08, 0x34, 0x33};
static uint8_t rock_map[] = {0x1C, 0x3E, 0X3E, 0X3E, 0X1C};
static uint8_t win_map[] = {0x01, 0x02, 0x04, 0x28, 0x10};
static uint8_t lose_map[] = {0x22, 0x14, 0x08, 0x14, 0x22};
static uint8_t draw_map[] = {0x7F, 0x2C, 0x2F, 0x2C, 0x2F};


/** Clears the LED matrix by setting columns to high */
void clear_display(void) {

    for (int i = 0; i<7; i++) {

        pio_config_set(rows[i], PIO_OUTPUT_HIGH);
    }
    for (int i = 0; i<5; i++) {

        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
}


/** Helper function that displays one column of the matrix */
static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    static uint8_t column_tracker = 0;
    pio_output_high(cols[column_tracker]);
    
    for (int current_row = 0; current_row < 7; current_row++) {

        if ((row_pattern >> current_row) & 1) {

            pio_output_low(rows[current_row]);

        } else {

            pio_output_high(rows[current_row]);

        }
    }

    pio_output_low(cols[current_column]);
    column_tracker = current_column;
}


/** Loops over the columns displaying the appropriate symbol */
void show_choice(uint8_t choice, uint8_t current_column) {

    switch (choice) {
        case 1:

            display_column(paper_map[current_column], current_column);
            break;

        case 2:

            display_column(scissor_map[current_column], current_column);
            break;

        case 3:

            display_column(rock_map[current_column], current_column);
            break;
    }
}


/** Loops over the columns displaying the appropriate symbol for win-lose senario */
void show_result(uint8_t result, uint8_t current_column) {

    switch (result) {
        case 1:

            display_column(win_map[current_column], current_column);
            break;

        case 0:

            display_column(lose_map[current_column], current_column);
            break;

        case 2:

            display_column(draw_map[current_column], current_column);
            break;

    }
}
