#include "pio.h"
#include "delay.h"
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "adxl345.h"
#include "LPF.h"
#include "button_debounce.h"
#include <stdio.h>

LowPassFilter xfilter;
LowPassFilter yfilter;
LowPassFilter zfilter;

struct calibrationValues_t {
    int16_t forwardMax;
    int16_t backwardMax;
    int16_t leftMax;
    int16_t rightMax;
    int16_t ymin;
    int16_t xmin;
    int16_t zmin;
    bool calibration_Finished;
};

struct calibrationValues_t calibrationValues = {
    .forwardMax = 0,
    .backwardMax = 0,
    .leftMax = 0,
    .rightMax = 0,
    .ymin = 0,
    .xmin = 0,
    .zmin = 0,
    .calibration_Finished = false
};

#define PACER_RATE 80 //Number of times per second it prints to terminal
#define PACER_DIVIDER 20
#define ZEROPOSITIONPWM 50
static adxl345_t *adxl345; // Static variable to hold the ADXL345 instance

static twi_cfg_t adxl345_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz I2C
    .slave_addr = 0
};

enum axis_t{
    CA_zero=0,
    CA_forward,
    CA_backward,
    CA_left,
    CA_right,
};

void calibrate(enum axis_t* currentAxis, int* count)
{
    (*count)++;
    pacer_wait ();
    int16_t accel[3];
    adxl345_accel_read (adxl345, accel);
    int16_t filtered_x = low_pass_filter_apply(&xfilter, accel[0]);
    int16_t filtered_y = low_pass_filter_apply(&yfilter, accel[1]);
    switch(*currentAxis)
    {
        case CA_zero:
            pio_output_low(BLUE_LED);
            pio_output_low(RED_LED);
            pio_output_low(GREEN_LED);
            if (*count % PACER_DIVIDER == 0)
            {
                printf("Waiting for calibrated zero position: %d %d\n", filtered_x, filtered_y);
            }
            if (debounceSwitch())
            {
                calibrationValues.xmin = filtered_x;
                calibrationValues.ymin = filtered_y;
                *currentAxis = CA_forward;
            } 
            break;
        case CA_forward:
            pio_output_low(BLUE_LED);
            pio_output_high(RED_LED);
            pio_output_high(GREEN_LED);
            if (*count % PACER_DIVIDER == 0)
            {
                printf("Waiting for calibrated forward position, x = %d\n", accel[0]);
            }
            if (debounceSwitch())
            {
                calibrationValues.forwardMax = accel[0];
                *currentAxis = CA_backward;
            }
            break; 
        case CA_backward:
            pio_output_high(BLUE_LED);
            pio_output_high(RED_LED);
            pio_output_low(GREEN_LED);
            if (*count % PACER_DIVIDER == 0)
            {
                printf("Waiting for calibrated backward position, x = %d\n", filtered_x);
            }
            if (debounceSwitch())
            {
                calibrationValues.backwardMax = filtered_x;
                *currentAxis = CA_left;
            }
            break;
        case CA_left:
            pio_output_high(BLUE_LED);
            pio_output_low(RED_LED);
            pio_output_high(GREEN_LED);
            if (*count % PACER_DIVIDER == 0)
            {
                printf("Waiting for calibrated left position\n");
            }
            if (debounceSwitch())
            {
                calibrationValues.leftMax = filtered_y;
                *currentAxis = CA_right;
            }
            break;
        case CA_right:
            pio_output_low(BLUE_LED);
            pio_output_low(RED_LED);
            pio_output_low(GREEN_LED);
            if (*count % PACER_DIVIDER == 0)
            {
                printf("Waiting for calibrated right position\n");
            }
            if (debounceSwitch())
            {
                calibrationValues.rightMax = filtered_y;
                calibrationValues.calibration_Finished = true;
            } 
            break;
    }
}

void convertAccelerometerToPWM(int16_t (*accelerometerReadings)[3])
{
    int16_t speedAxis = (*accelerometerReadings)[0];

    if (speedAxis == 0)
    {
        (*accelerometerReadings)[0] = 50;
    }
    else if (((speedAxis * calibrationValues.forwardMax) > 0) && speedAxis > 0) //User is leaning forward and recorded value is a positive
    {
        (*accelerometerReadings)[0] = 50 + (speedAxis * 50 / calibrationValues.forwardMax); //Convert acceleration to a percentage between 50 and 100
    }
    else if (((speedAxis * calibrationValues.forwardMax) > 0) && speedAxis < 0) //User is leaning forward and recorded value is a negative
    {
        (*accelerometerReadings)[0] = 50 - (speedAxis * 50 / -calibrationValues.forwardMax); //Convert acceleration to a percentage between 50 and 100
    }
    else if (((speedAxis * calibrationValues.forwardMax) < 0) && speedAxis < 0) //User is leaning backward and recorded value is negative => forward is positive direction
    {
        (*accelerometerReadings)[0] = 50 + (speedAxis * 50 / -calibrationValues.backwardMax);
    }
    else if (((speedAxis * calibrationValues.forwardMax) < 0) && speedAxis > 0) //User is leaning backward and recorded value is positive => forward is negative direction
    {
        (*accelerometerReadings)[0] = 50 - (speedAxis * 50 / calibrationValues.backwardMax);
    }

    //Make a 5% deadzone around midpoint and make absolute limits on percentages
    if (((*accelerometerReadings)[0] > 50) && ((*accelerometerReadings)[0] < 55))
    {
        (*accelerometerReadings)[0] = 50;
    }
    else if (((*accelerometerReadings)[0] < 50) && ((*accelerometerReadings)[0] > 45))
    {
        (*accelerometerReadings)[0] = 50;
    }
    else if ((*accelerometerReadings)[0] < 0)
    {
        (*accelerometerReadings)[0] = 0;
    }
    else if ((*accelerometerReadings)[0] > 100)
    {
        (*accelerometerReadings)[0] = 100;
    }
}

int main (void)
{
    low_pass_filter_init(&xfilter, 0.1);
    low_pass_filter_init(&yfilter, 0.1);
    low_pass_filter_init(&zfilter, 0.1);

    pio_config_set (GREEN_LED, PIO_OUTPUT_LOW);
    pio_config_set (BLUE_LED, PIO_OUTPUT_LOW);
    pio_config_set (RED_LED, PIO_OUTPUT_LOW);

    /* Configure button PIO as input with pullup.  */
    pio_config_set (BUTTON_PIO, PIO_PULLUP);

    twi_t adxl345_twi; //Create a I2c definition
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    // Initialise the TWI (I2C) bus for the ADXL345
    adxl345_twi = twi_init (&adxl345_twi_cfg); //Send a command over I2C

    if (! adxl345_twi)
        {
            while(1)
            {
                printf("Error while initialising I2C bus (Source: Acceloromter)");
                pacer_wait ();
                pio_output_toggle (LED_ERROR_PIO);
            }
        }

    // Initialise the ADXL345
    adxl345 = adxl345_init (adxl345_twi, ADXL345_ADDRESS); //Sends command which sets datamode, check for address in response

    if (!adxl345)
        {
            while(1)
            {
                printf("Error while initialising Acceloromter");
                pacer_wait ();
                pio_output_toggle (LED_ERROR_PIO);
            }
        }
    int count = 0;
    pacer_init (PACER_RATE); //Ticks PACER_RATE times a second
    enum axis_t currentAxis = 0;
    calibrationValues.calibration_Finished = false;
    while(!calibrationValues.calibration_Finished)
    {
        calibrate(&currentAxis, &count);
    }

    pio_output_high(RED_LED);
    pio_output_high(BLUE_LED);
    pio_output_high(GREEN_LED);


    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Read in the accelerometer data.  */
        if (! adxl345_is_ready (adxl345))
        {
            printf ("Waiting for accelerometer to be ready...");
        }
        else
        {
            int16_t accel[3];
            if (adxl345_accel_read (adxl345, accel))
            {
                int16_t filtered_x = low_pass_filter_apply(&xfilter, accel[0]);
                int16_t filtered_y = low_pass_filter_apply(&yfilter, accel[1]);
                int16_t filtered_values[3];
                filtered_values[0] = filtered_x;
                filtered_values[1] = filtered_y;
                filtered_values[2] = 0;
                convertAccelerometerToPWM(&filtered_values);
                printf("XPWM: %d\n", filtered_values[0]);
            }
            else
            {
                printf ("ERROR: failed to read acceleration\n");
            }
        }
    }
}