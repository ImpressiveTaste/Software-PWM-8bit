#ifndef SOFT_PWM_H
#define SOFT_PWM_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_PWM_CHANNELS 4  // Maximum number of independent PWM channels

typedef struct {
    uint16_t period;        // PWM period (derived from frequency)
    uint16_t on_time;       // ON time (calculated from duty cycle)
    uint8_t primary_pin;    // Primary PWM pin
    uint8_t complementary_pin;  // Complementary pin (inverse signal)
    uint16_t dead_time_rise; // Dead time between turning off primary and turning on complementary
    uint16_t dead_time_fall; // Dead time between turning off complementary and turning on primary
    bool active;            // Is this PWM channel active?
    bool pin_state;         // Current state of the primary pin (high/low)
    bool complementary_state;  // Current state of the complementary pin
} SoftwarePWM;

// Initialize the PWM system
void PWM_Init(void);

// Setup the PWM with primary pin, complementary pin, frequency, duty cycle, and separate dead times
void PWM_SetupComplementary(uint8_t channel, uint8_t primary_pin, uint8_t complementary_pin, uint16_t frequency, uint8_t duty_cycle, uint16_t dead_time_rise, uint16_t dead_time_fall);

// Set the duty cycle (0-100%) for a channel
void PWM_SetDutyCycle(uint8_t channel, uint8_t duty_cycle);

// Set the frequency for a channel (in Hz)
void PWM_SetFrequency(uint8_t channel, uint16_t frequency);

// Start the PWM on a given channel
void PWM_Start(uint8_t channel);

// Stop the PWM on a given channel
void PWM_Stop(uint8_t channel);

// Timer Interrupt handler to manage PWM channels
void PWM_Update(void);

#endif  // SOFT_PWM_H
