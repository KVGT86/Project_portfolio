#include "pwm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pio.h"
#include "panic.h"
#include "target.h"

#ifndef DARSTADLY_STUFF_H
#define DARSTADLY_STUFF_H


#define PWM_FREQ_HZ 100
#define SERVO_PWM_FREQ_HZ 50  // Define the frequency of the PWM signal in Hertz
#define PWM_DUTY 50      // Define the duty cycle of the PWM signal as a percentage
#define OFF_STATE 5     // Define the off state of the servo as a percentage
#define ON_STATE 10       // Define the on state of the servo as a percentage
#define SERVO_STATE_DUMP  0
#define SERVO_STATE_HOLD  1

// Configuration structure for the PWM signal used to control the servo
static const pwm_cfg_t pwm_servo_cfg =
{
    .pio = SERVO_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (SERVO_PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (SERVO_PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};


// Function to initialize the PWM signal for the servo
void init_servo_pwm(void);

void run_servo(void);

void reset_servo(void);

// Function to run the servo based on the input buffer
// void check_run_servo(char* buf);

#endif // DARSTADLY_STUFF_H