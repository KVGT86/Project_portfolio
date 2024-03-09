// Name: Main Game Program
//Purpose: Utilizes all modules to create a Rock, Paper ans Scissors
//Course: ENCE260
//Authors: Max Sigley and Caleb Barnes (Group324)
//Date: 15/10/2022


#include "system.h"
#include "led.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "game_logic.h"
#include "game_display.h"
#include "navswitch.h"
#include "stdbool.h"
#include "ir_uart.h"
#include <avr/io.h>


int main(void)
{
    /**System Initialisation*/
    system_init ();
    led_init ();
    pacer_init(500);
    clear_display();
    navswitch_init();
    led_set(LED1, 0);
    ir_uart_init();

    /**Variable Initialisation*/
    uint8_t opponent_choice;
    uint8_t state = 0; //
    uint8_t current_column = 0;
    uint8_t choice = 1; // 1 is paper, 2 is scissors, 3 is rock.
    uint8_t final_choice;
    uint8_t result;
    uint16_t time = 0;
    bool sent = false;

    while (1)
    {
        /**Update*/
        pacer_wait();
        navswitch_update();

        if (state == 0) { // Displaying options

            if (navswitch_push_event_p(NAVSWITCH_NORTH)) { // scroll forwards through choices.

               if (choice == 3) {

                    choice = 1;

                } else {

                    choice++;
                } 
            }

            if (navswitch_push_event_p(NAVSWITCH_SOUTH)) { // scroll backwards throughchoices.

                if (choice == 1) {

                    choice = 3;

                } else {

                    choice--;
                }
            }
            
            show_choice(choice, current_column);
            
            if (navswitch_push_event_p(NAVSWITCH_PUSH)){ //confirms selection and progresses onto next state

                final_choice = choice;
                clear_display();
                state = 1;

            }

        } else if (state == 1) { // Sending and receiving options

            if (ir_uart_write_ready_p()) { // Waits untill ir is ready to send

                ir_uart_putc(final_choice);
                sent = true;
            }

            if (ir_uart_read_ready_p() && sent) { //receives opponents choice

                opponent_choice = ir_uart_getc();

                if ((opponent_choice == 1) || (opponent_choice == 2) || (opponent_choice == 3)) { //filters IR inputs and progresses onto next state

                    state = 2;
                    time = 0;
                }
            }
  
        } else if (state == 2) { // Displaying winner

            result = test_winner(choice, opponent_choice);
            show_result(result,current_column);
            
            time ++;
            if (time > 2000) { //shows result for 2000ms

                sent = false;
                ir_uart_putc('!'); // Makes sure ir does not interfer with next loop ("!"" gets filtered out)
                state = 0;
                time = 0;
            }


        }

        current_column++;
        if (current_column > 5) { //increments columns to update

            current_column = 0;
        }

    }
}