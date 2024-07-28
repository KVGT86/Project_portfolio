#include "LPF.h"

// Function to initialize the low-pass filter
void low_pass_filter_init(LowPassFilter *filter, float alpha) {
    // Initialize filter parameters
    filter->alpha = alpha;
    filter->prev_filtered_value = 0;
}

// Function to apply the low-pass filter to a new input value
int low_pass_filter_apply(LowPassFilter *filter, int input_value) {
    // Update the filtered value using a weighted average of the input and the previous filtered value
    filter->prev_filtered_value = (1 - filter->alpha) * filter->prev_filtered_value + filter->alpha * input_value;
    return filter->prev_filtered_value;
}
