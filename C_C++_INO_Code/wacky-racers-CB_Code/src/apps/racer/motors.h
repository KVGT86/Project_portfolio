#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pwm.h"
#include "pio.h"
#include "panic.h"
#include "target.h"
#include "sys.h"
#include "darstadly_stuff.h"

#ifndef MOTORS_H
#define MOTORS_H


#define PWM_FREQ_HZ 100
#define PWM_DUTY 50
#define MAX_DUTY 95
#define MIN_DUTY 5

// Initialising all pwm config variables
static const pwm_cfg_t pwm_AIN1_cfg =
{
    .pio = MOTOR_AIN1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm_BIN1_cfg =
{
    .pio = MOTOR_BIN1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

// Initialising pwm drivers
void init_pwm(void);
void init_pwm_a(void);
void init_pwm_b(void);
void pwm_power_down(void);

void process_motor_command (char *buf);

#endif //MOTORS_H