#ifndef LPF_H
#define LPF_H

// Low-pass filter data structure
typedef struct {
    float alpha;      // Filter coefficient (between 0 and 1)
    int prev_filtered_value; // Filtered output
} LowPassFilter;

// Function to initialize the low-pass filter
void low_pass_filter_init(LowPassFilter *filter, float alpha);

// Function to apply the low-pass filter to a new input value
int low_pass_filter_apply(LowPassFilter *filter, int input_value);

#endif /* LOW_PASS_FILTER_H */
