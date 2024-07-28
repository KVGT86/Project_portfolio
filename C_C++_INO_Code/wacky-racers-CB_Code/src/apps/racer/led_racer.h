#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "target.h"
#include "ledbuffer.h"
#include "sysclock.h"

#ifndef LED_RACER_H
#define LED_RACER_H

void init_leds(void);

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef enum {
    STATE_INIT,
    STATE_PREFILL,
    STATE_ANIMATE,
    STATE_DONE
} LoadStripState;

typedef struct {
    LoadStripState state;
    sysclock_clocks_t start_tick;
    int i;
    int j;
    uint8_t ledsLoaded;
    uint8_t ledFinish;
    uint8_t isFilled;
    uint16_t previous_led_finish;
} LoadStripContext;

typedef struct {
    LoadStripState state;
    sysclock_clocks_t start_tick;
    int i;
    int j;
    uint8_t ledsLoaded;
    uint8_t ledFinish;
    uint8_t isFilled;

} RevStripContext;


void color_map_to_strip(Color color_map[], ledbuffer_t* buffer);
void ledOff(ledbuffer_t* buffer);
void init_color_map(ledbuffer_t* buffer, Color color_map[]);
void waveLED(ledbuffer_t* buffer);
Color interpolate(Color color1, Color color2, double ratio);
void createGradient(Color *colors, int numColors, int numLeds, Color *gradient);
void shiftGradient(Color *gradient, int numLEDs, uint16_t delay);
void setRandomColors(Color *colors, int numColors, int numLeds, Color *ledColors, uint16_t delay);
void loadStrip_init(LoadStripContext *StripContext, Color *color_map, ledbuffer_t *ledBuffer, Color color, uint8_t percentage_start, uint8_t percentage_finish, uint16_t delay);
uint8_t loadStrip_run(LoadStripContext *StripContext, Color *color_map, ledbuffer_t *ledBuffer, Color color, uint8_t percentage_start, uint8_t percentage_finish, uint16_t delay);
void revStrip_run(LoadStripContext *StripContext, Color *color_map, ledbuffer_t *ledBuffer, Color color, int16_t leds_on);


#endif //LED_RACER_H
