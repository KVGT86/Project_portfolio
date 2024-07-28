#include "sleep_commands.h"

// ADC object for reading VBAT voltage
adc_t vbatt_adc;
// adc_t 3v3_ref_adc; // Not used in this implementation
// uint16_t 3v3_digital; // Not used in this implementation
uint16_t digital_5V; // Digital representation of 5V reference voltage
uint16_t vbatt_digital; // Digital representation of VBAT voltage


/**
 * Initializes the ADC module for reading VBAT voltage.
 */
void init_adc(void) 
{
    // Initialize the ADC object for VBAT voltage reading       
    vbatt_adc = adc_init(&vbatt_adc_cfg);
    if(!vbatt_adc) 
        panic(LED_ERROR_PIO, 1);

    //we shouldnt need a 3v3 reference just a calculated value
    // 3v3_ref_adc = adc_init(&3v3_ref_cfg_t);
    // if(!3v3_ref_adc) panic(LED_ERROR_PIO, 1);
    
    // Calculate the digital representation of 5V reference voltage
    digital_5V = (V_CUTOFF / V_REF) * (ADC_SIZE / 10);
};

/**
 * Checks the VBAT voltage and takes action if it's low.
 */
void check_voltage(void)
{
    printf("entering check voltage cct\n\n");
    // uint16_t 3v3_ref_data[1];
    uint16_t vbatt_data[1]; // Stores the ADC reading of VBAT voltage

    //Read VBATT voltage using ADC (Imported data at xx_data[0] of buffs)
    // adc_read(3v3_ref_adc, 3v3_ref_data, sizeof(3v3_ref_data));
    adc_read(vbatt_adc, vbatt_data, sizeof(vbatt_data));


    /*
    Digital value = (Analog input voltage)/ VAREF) * (2n-1)
    Where n = number of bits of ADC digital output.
    For example, for 12-bit ADC, VIN=1V, VAREF=3.3 V
    Digital value = (1 V/3.3 V) *4095 = 1240d = 4D8h
    */
    // 3v3_digital = (3v3_ref_data[0] / V_REF) * (ADC_SIZE / 10)
    vbatt_digital = (vbatt_data[0] / V_REF) * (ADC_SIZE / 10);
    printf("vbatt digital = %d\n", vbatt_digital);

    // If the VBAT voltage is low, enter low-power mode
    // while ((vbatt_digital - digital_5V) <= TOLERANCE) {
    //     mcu_low_voltage();
    // };
};

/**
 * Enters low-power mode by shutting down the ADC, setting the sleep state, and flashing an error LED.
 */
void mcu_low_voltage(void)
{
    // Disable the H-bridge
    pio_output_low(N_SLEEP_HBRIDGE);
    // Shut down the ADC
    adc_shutdown(vbatt_adc);
    // Flash the error LED
    pio_output_toggle(LED_ERROR_PIO); 
    // Enter sleep mode
    mcu_sleep(&mcu_sleep_mode);
    // Wait for 500ms
    delay_ms(500);
};