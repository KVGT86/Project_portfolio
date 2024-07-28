#include "usb_commands.h"
#include "motors.h" // not sure if needs

extern pwm_t pwm_AIN1;
extern pwm_t pwm_BIN1;

void prompt_command (void)
{
    printf (">Enter desired duties: ");
    fflush (stdout);
}

void process_command (void)
{
    pio_output_high(N_SLEEP_HBRIDGE);
    char buf[256];
        if (fgets(buf, sizeof(buf), stdin)) {
            int duty1, duty2;
            char direction1, direction2;
            // sscanf returns the number of input items successfully matched
            if (sscanf(buf, "%c %d %c %d", &direction1, &duty1, &direction2, &duty2) == 4) {
                
                if (duty1 > MAX_DUTY) duty1 = MAX_DUTY;
                if (duty2 > MAX_DUTY) duty2 = MAX_DUTY;
                if (duty1 < MIN_DUTY) duty1 = MIN_DUTY;
                if (duty2 < MIN_DUTY) duty2 = MIN_DUTY;

                switch (direction1) 
                {
                
                //Motor one 
                case '+': //forward
                    pio_output_low(MOTOR_AIN2_PIO);                                 // Drive AIN2 Low
                    pwm_duty_set(pwm_AIN1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, duty1));   // Drive AIN1 PWM
                    break;

                case '-': //reverse
                    pio_output_high(MOTOR_AIN2_PIO);
                    pwm_duty_set(pwm_AIN1, PWM_DUTY_DIVISOR(PWM_FREQ_HZ, (100 - duty1)));
                    break;

                default:
                    printf("Invalid operator: %c\n", direction1);
                }

                switch (direction2) {
                
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
                    printf("Invalid operator: %c\n", direction2);
                }

            } else {
                printf("Invalid input\n");
            }

        prompt_command ();
        }
    
}

void init_usb_commands (void)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        printf("Initalising %d\n", i);
        fflush(stdout);
        delay_ms(50);
    };
    // prompt_command();
}