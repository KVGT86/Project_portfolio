#include <pio.h>
#include "target.h"
#include "led_racer.h"
#include "ledbuffer.h"
#include "ledtape.h" 
#include "usb_serial.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "sysclock.h"
#include <stdlib.h>

void init_leds(void)
{
    // configuring led pins
    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LED_ACTIVE_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LED_ERROR_PIO, PIO_OUTPUT_HIGH);
    
    // using green led as power led
}

#define NUM_LEDS 21
Color off = {0,0,0};

// Function to display the wave pattern by writing to the LED strip
void color_map_to_strip(Color color_map[], ledbuffer_t* buffer) {
    for (int i = 0; i < buffer->leds; i++) {
        ledbuffer_set(buffer, i, color_map[i].r, color_map[i].g, color_map[i].b);
    }
    ledbuffer_write(buffer);
}

void ledOff(ledbuffer_t* buffer) {
    Color blank_map [buffer->leds];
    for (int i = 0; i < buffer->leds; ++i) {
        blank_map[i] = off;
    }
    color_map_to_strip(blank_map, buffer);
}

void init_color_map(ledbuffer_t* buffer, Color color_map[]) {
    for (int i = 0; i < buffer->leds; ++i) {
        color_map[i] = off;
    }
    // color_map_to_strip(color_map, buffer);
}

// void waveLED(ledbuffer_t* buffer) {
//     for (int i = 1; i < buffer->leds; i++)
//     {
//         //delay_ms(100);
//         if (i > 0) {
//             ledbuffer_set(buffer, i-1, 0, 0, 0);
//         }
//         ledbuffer_set(buffer, i, 213, 0, 255);
//         if (i < buffer->leds - 1) {
//             ledbuffer_set(buffer, i+1, 0, 0, 0);
//         }
//         ledbuffer_write(buffer);
//     }
//     ledbuffer_set(buffer, buffer->leds-1, 0, 0, 0);
// }

/************************************************************************
    Chroma Effect
*************************************************************************/

Color interpolate(Color color1, Color color2, double ratio) {
    Color result;
        result.r = (uint8_t)(color1.r + (int)(color2.r - color1.r) * ratio);
        result.g = (uint8_t)(color1.g + (int)(color2.g - color1.g) * ratio);
        result.b = (uint8_t)(color1.b + (int)(color2.b - color1.b) * ratio);
    return result;
}

void createGradient(Color *colors, int numColors, int numLeds, Color *gradient) {
    // Calculate the step size for interpolation
    float stepSize = (float)(numColors - 1) / (numLeds - 1);
    
    // Iterate through each LED
    for (int i = 0; i < numLeds; ++i) {
        // Calculate the corresponding position in the colors array
        float t = i * stepSize;
        int colorIndex = (int)t;  // Round down to the nearest integer
        
        // Handle the case when colorIndex is at the last index
        if (colorIndex >= numColors - 1) {
            gradient[i] = colors[numColors - 1]; // Set the last color directly
        } else {
            // Calculate the interpolation parameter within the segment
            float tInSegment = t - colorIndex;
            
            // Interpolate between the current color and the next color
            gradient[i] = interpolate(colors[colorIndex], colors[colorIndex + 1], tInSegment);
        }
    }
}

sysclock_clocks_t start_time_gradient = 0;
// Function to shift the gradient along the LED strip
void shiftGradient(Color *gradient, int numLEDs, uint16_t delay) {
    Color temp = gradient[numLEDs - 1];
    if (sysclock_millis_elapsed(start_time_gradient, delay)) {
        for (int i = numLEDs - 1; i > 0; --i) {
            gradient[i] = gradient[i - 1];
        }
        gradient[0] = temp;
        start_time_gradient = sysclock_clocks();
    }

}

/************************************************************************
    Static Effect
*************************************************************************/

uint32_t start_tick_static = 0;
// Function to set each LED to a random color from a specified color list
void setRandomColors(Color *colors, int numColors, int numLeds, Color *ledColors, uint16_t delay) {
    // Seed the random number generator
    
    // Iterate through each LED
    if (sysclock_millis_elapsed(start_tick_static, delay)) {
        for (int i = 0; i < numLeds; ++i) {
            // Generate a random index to select a color from the list
            int randomIndex = rand() % numColors;
            // Assign the LED with the color at the random index
            ledColors[i] = colors[randomIndex];
        }
        start_tick_static = sysclock_clocks();
    }
}




/************************************************************************
    PROGRESS BAR Effect
*************************************************************************/


void loadStrip_init(LoadStripContext *StripContext, Color *color_map, ledbuffer_t *ledBuffer, Color color, uint8_t percentage_start, uint8_t percentage_finish, uint16_t delay) {
    StripContext->state = STATE_INIT;
    StripContext->start_tick = sysclock_clocks();
    StripContext->i = 0;
    StripContext->j = 0;
    StripContext->isFilled = false;
    // Ensure the finish percentage does not exceed 100%
    if (percentage_finish > 100) {
        printf("ERROR: LED LOAD STRIP FUNCTION. FINISH > 100\n");
        percentage_finish = 100;
    }

    // Ensure the start percentage does not exceed the finish percentage
    if (percentage_start > percentage_finish) {
        printf("ERROR: LED LOAD STRIP FUNCTION. START > FINISH PERCENTAGE\n");
        percentage_start = 0;
        percentage_finish = 100;
    }

    StripContext->ledsLoaded = ledBuffer->leds * (percentage_start / 100.0);
    StripContext->ledFinish = ledBuffer->leds * (percentage_finish / 100.0);
   
    // Pre-fill the strip up to the start position if percentage_start > 0
    for (int i = 0; i < StripContext->ledsLoaded; ++i) {
        color_map[i] = color;
    }
    StripContext->isFilled = true;
    color_map_to_strip(color_map, ledBuffer);
}

uint8_t loadStrip_run(LoadStripContext *StripContext, Color *color_map, ledbuffer_t *ledBuffer, Color color, uint8_t percentage_start, uint8_t percentage_finish, uint16_t delay) {
    switch (StripContext->state) {
        case STATE_INIT:
            StripContext->state = STATE_ANIMATE;
            StripContext->i = StripContext->ledsLoaded;
            StripContext->j = ledBuffer->leds - 1;
            StripContext->start_tick = sysclock_clocks();
            break;
        
        case STATE_PREFILL:
            for (int i = 0; i < ledBuffer->leds; ++i) {
                if (i < StripContext->i) {
                    color_map[i] = color;
                } else {
                    color_map[i] = off;
                }
            }
            color_map_to_strip(color_map, ledBuffer);
            StripContext->state = STATE_ANIMATE;
            StripContext->isFilled = true;
        break;

        case STATE_ANIMATE:
            if (!StripContext->isFilled) {
                StripContext->state = STATE_PREFILL;
                break;
            }
            if (StripContext->i < StripContext->ledFinish) {
                if (sysclock_millis_elapsed(StripContext->start_tick, delay)) {
                    if (StripContext->j > StripContext->i) {
                        color_map[StripContext->j] = color;
                        if (StripContext->j < ledBuffer->leds - 1) {
                            color_map[StripContext->j + 1] = off;
                        }
                        color_map_to_strip(color_map, ledBuffer);
                        StripContext->j--;

                    } else {
                        color_map[StripContext->i] = color;
                        color_map_to_strip(color_map, ledBuffer);
                        StripContext->i++;
                        StripContext->j = ledBuffer->leds - 1;
                    }
                    StripContext->start_tick = sysclock_clocks(); // Reset the delay timer

                }
            } else {
                StripContext->state = STATE_DONE;
            }
            StripContext->isFilled = false;
            break;

        case STATE_DONE:
            // Reset or perform other tasks
            return true;
            break;
    }
    return false;
}


void revStrip_run(LoadStripContext *StripContext, Color *color_map, ledbuffer_t *ledBuffer, Color color, int16_t leds_on) { 
    leds_on = abs(leds_on);       
    if (leds_on > 100) {
        printf("ERROR: LED REV STRIP FUNCTION. REV > 100\n");
        leds_on = 100;
    }

    // Ensure the start percentage does not exceed the finish percentage
    if (leds_on < 0) {
        printf("ERROR: LED REV STRIP FUNCTION. REV < 0\n");
        leds_on = 0;
    }

    StripContext->ledsLoaded = ledBuffer->leds * (leds_on / 100.0);
    for (int i = 0; i < ledBuffer->leds; ++i) {
        if (i < StripContext->ledsLoaded) {
            color_map[i] = color;
        } else {
            color_map[i] = off;
        }
        
    }
    color_map_to_strip(color_map, ledBuffer);
}

/************************************************************************
    Example Useage
*************************************************************************/


// void my_periodic_task(void) {
//     // Code to run every millisecond

//     printf("every 1ms\n");
// }


// int main(void) {
//     usb_serial_stdio_init ();
//     srand(time(NULL));

//     /*Init non blocking delay*/
//     sysclock_init();
//     sysclock_callback(my_periodic_task);


//     ledbuffer_t* ledBuffer = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);
//     ledOff(ledBuffer);
    

//     /*Set up a List of colors to use (RGB)*/
//     Color colors[] = {
//         {255, 0, 0},
//         {255, 0, 255},
//         {0, 255, 0}, 
//         {0, 0, 255},
//         {255, 119, 0}, 
//         {255, 255, 255},
//     };
//     int numColors = sizeof(colors) / sizeof(colors[0]);
//     Color color_map[ledBuffer->leds];
//     init_color_map(ledBuffer, color_map);

//     /*Chroma/Rainbow Effect*/
//     Color gradient[ledBuffer->leds];
//     createGradient(colors, numColors, ledBuffer->leds, gradient);
//     int shift = 0;

//     /*Static TV effect*/
//     setRandomColors(colors, numColors, ledBuffer->leds, color_map, 50);


//     /*Progress Bar*/
//     LoadStripContext loadStripContext;
//     LoadStripContext revStripContext;
//     uint8_t loadProgress_start = 0;
//     uint8_t loadProgress_finish = 10;
//     Color purple = {255,0,255};
//     loadStrip_init(&loadStripContext, color_map, ledBuffer, purple, loadProgress_start, loadProgress_finish, 50);

//     int i =0;
//     while(1) {
//         /*This ones kinda shit*/
//         // waveLED(ledBuffer);

//         /*Static Effect*/
//         // setRandomColors(colors, numColors, ledBuffer->leds, color_map, 50);
//         // color_map_to_strip(color_map, ledBuffer);

//         /*Chroma Effect*/
//         color_map_to_strip(gradient, ledBuffer);
//         shiftGradient(gradient, ledBuffer->leds, 100);


//         /*Progress Bar*/
//         // init_color_map(ledBuffer, color_map);
//         // if(loadStrip_run(&loadStripContext, color_map, ledBuffer, colors[i], loadProgress_start, loadProgress_finish, 25)) {
//         //     loadProgress_start += 10;
//         //     loadProgress_finish = loadProgress_start + 10;
//         //     if (loadProgress_start > 100) {
//         //         loadProgress_start = 0;
//         //         loadProgress_finish = loadProgress_start + 10;
//         //         i++;
//         //     }
//         //     loadStrip_init(&loadStripContext, color_map, ledBuffer, colors[i], loadProgress_start, loadProgress_finish, 50);
//         //     if (i > numColors) {
//         //         i =0;
//         //     }


//         // }
//     }
// }
