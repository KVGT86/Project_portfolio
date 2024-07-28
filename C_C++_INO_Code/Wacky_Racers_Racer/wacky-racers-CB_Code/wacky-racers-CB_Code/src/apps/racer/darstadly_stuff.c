#include "darstadly_stuff.h"

// Define a variable to hold the PWM handle for the servo
pwm_t pwm_servo;


// Initialize the servo PWM
void init_servo_pwm(void)
{
    // Initialize the PWM handle for the servo with the specified configuration
    pwm_servo = pwm_init(&pwm_servo_cfg);
    // Check if the PWM handle was successfully initialized
    if (!pwm_servo) {
        // If not, trigger a panic with an error LED and a delay of 1 second
        panic(LED_ERROR_PIO, 1);
    }

    // Start the PWM for the servo
    pwm_start(pwm_servo);
    // Set the duty cycle for the PWM to the OFF state
    pwm_duty_set(pwm_servo, PWM_DUTY_DIVISOR(SERVO_PWM_FREQ_HZ, OFF_STATE));
}

void run_servo(void)
{ 
    pwm_duty_set(pwm_servo, PWM_DUTY_DIVISOR(SERVO_PWM_FREQ_HZ, ON_STATE));
}

void reset_servo(void)
{
    pwm_duty_set(pwm_servo, PWM_DUTY_DIVISOR(SERVO_PWM_FREQ_HZ, OFF_STATE));
}

// // Run the servo based on a trigger character
// void check_run_servo(nrf24_t *nrf)
// {
//     // Define a variable to hold the trigger character
//     int trigger; // SERVO STATE

//     // Try to read a character from the input buffer
//     if (sscanf(buf, "%d", &trigger) == 1) {
//         // Switch on the trigger character
//         switch(trigger) {
//             // If the trigger character is "ON"
//             case SERVO_STATE_DUMP:
//                 // Set the duty cycle for the PWM to the ON state
//                 pwm_duty_set(pwm_servo, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, ON_STATE));
//                 break;
//             // If the trigger character is "OFF"
//             case SERVO_STATE_HOLD:
//                 // Do nothing
//                 break;
//             // If the trigger character is not recognized
//             default:
//                 // Do nothing
//                 break;
//         };
//     };
// }