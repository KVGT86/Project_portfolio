#include "motors.h"

// Ben said better to declare global variables in .c files 
// instead of .h files and refer to them as externs in main.c
pwm_t pwm_AIN1;
pwm_t pwm_BIN1;

void init_pwm(void)
{   
    // Configuring NSLEEP pin as active high output
    pio_config_set(N_SLEEP_HBRIDGE, PIO_OUTPUT_HIGH);
    // pio_output_low(N_SLEEP_HBRIDGE);

    // Setting NSLEEP PIN HIGH
    pio_output_high(N_SLEEP_HBRIDGE);
    /*INITIALISE A AND B CHANNELS*/
    init_pwm_a();
    init_pwm_b();
    /*START BOTH CHANNELS*/
    pwm_channels_start(pwm_channel_mask(pwm_AIN1) | pwm_channel_mask(pwm_BIN1));
}

void init_pwm_a(void)
{
    /*A CHANNEL*/
    pio_config_set(MOTOR_AIN2_PIO, PIO_OUTPUT_LOW);

    pwm_AIN1 = pwm_init(&pwm_AIN1_cfg);
    if (! pwm_AIN1) panic(LED_ERROR_PIO, 1);
}

void init_pwm_b(void)
{
    /*B CHANNEL*/
    pio_config_set(MOTOR_BIN2_PIO, PIO_OUTPUT_LOW);

    pwm_BIN1 = pwm_init(&pwm_BIN1_cfg);
    if (! pwm_BIN1) panic(LED_ERROR_PIO, 1);
}

void pwm_power_down(void)
{
    pwm_channels_stop(pwm_channel_mask(pwm_AIN1) | pwm_channel_mask(pwm_BIN1));
    pio_output_low(N_SLEEP_HBRIDGE);
    pio_output_low(MOTOR_AIN2_PIO);
    pio_output_low(MOTOR_BIN2_PIO);
}

void process_motor_command (char *buf)
{
    // // Setting NSLEEP PIN HIGH
    // pio_output_high(N_SLEEP_HBRIDGE);
    int duty1, duty2, servo_mode;
    char direction1, direction2, servo;

    // sscanf returns the number of input items successfully matched [MAY HAVE TO CHANGE '+' AND'-' TO LOGICS (and therefore %c -> %d)]
    if (sscanf(buf, "%c %d %c %d %c %d", &direction1, &duty1, &direction2, &duty2, &servo, &servo_mode) == 6) {
        
        // if (duty1 > MAX_DUTY) duty1 = MAX_DUTY;
        // if (duty2 > MAX_DUTY) duty2 = MAX_DUTY;
        // if (duty1 < MIN_DUTY) duty1 = MIN_DUTY;
        // if (duty2 < MIN_DUTY) duty2 = MIN_DUTY;

        printf ("%d | ", duty1);
        printf ("%d | ", duty2);
        printf ("%d\n\r", servo_mode);

        switch (direction1) 
        {
        //Motor one 
        case '+': //forward
            pio_output_low(MOTOR_AIN2_PIO);
            pwm_duty_set(pwm_AIN1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, duty1));
            break;

        case '-': //reverse
            pio_output_high(MOTOR_AIN2_PIO);
            pwm_duty_set(pwm_AIN1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, (100 - duty1)));
            break;

        default:
            break;
        };

        switch(direction2) {
        //Motor two 
        case '+': //forward
            pio_output_low(MOTOR_BIN2_PIO);
            pwm_duty_set(pwm_BIN1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, duty2));
            break;

        case '-': //reverse
            pio_output_high(MOTOR_BIN2_PIO);
            pwm_duty_set(pwm_BIN1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, (100 - duty2)));
            break;

        default:
            break;
        };

        switch(servo_mode) {
        //Servo motor
        case 40: //off
            printf("servo mode = %d\n\n", servo_mode);
            printf("Not running servo\n");
            reset_servo();
            break;

        case 100: //on
            printf("servo mode = %d\n\n", servo_mode);
            run_servo();
            break;

        default:
            break;
        };
    };  
}