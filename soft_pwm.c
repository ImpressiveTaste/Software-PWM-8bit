#include <xc.h>
#include "soft_pwm.h"

#define _XTAL_FREQ 16000000  // System clock frequency

SoftwarePWM pwms[MAX_PWM_CHANNELS];  // Array to manage multiple PWM channels

void PWM_Init(void) {
    for (uint8_t i = 0; i < MAX_PWM_CHANNELS; i++) {
        pwms[i].active = false;
        pwms[i].pin_state = false;
        pwms[i].complementary_state = false;
    }
    
    // Configure Timer1 (or another timer) for PWM control
    T1CON = 0x00;  // Stop Timer1, set prescaler
    TMR1 = 0;      // Clear Timer register
    PIR1bits.TMR1IF = 0;  // Clear interrupt flag
    PIE1bits.TMR1IE = 1;  // Enable Timer1 interrupt
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.GIE = 1;   // Enable global interrupts
}

void PWM_SetupComplementary(uint8_t channel, uint8_t primary_pin, uint8_t complementary_pin, uint16_t frequency, uint8_t duty_cycle, uint16_t dead_time_rise, uint16_t dead_time_fall) {
    if (channel >= MAX_PWM_CHANNELS) return;

    pwms[channel].primary_pin = primary_pin;
    pwms[channel].complementary_pin = complementary_pin;
    pwms[channel].dead_time_rise = dead_time_rise;
    pwms[channel].dead_time_fall = dead_time_fall;
    
    // Set both pins as outputs
    TRISB &= ~(1 << primary_pin);          // Set primary pin as output
    TRISB &= ~(1 << complementary_pin);    // Set complementary pin as output
    
    // Set both pins low initially
    LATB &= ~(1 << primary_pin);
    LATB &= ~(1 << complementary_pin);
    
    // Configure frequency and duty cycle
    PWM_SetFrequency(channel, frequency);
    PWM_SetDutyCycle(channel, duty_cycle);
}

void PWM_SetDutyCycle(uint8_t channel, uint8_t duty_cycle) {
    if (channel >= MAX_PWM_CHANNELS || duty_cycle > 100) return;
    pwms[channel].on_time = (pwms[channel].period * duty_cycle) / 100;
}

void PWM_SetFrequency(uint8_t channel, uint16_t frequency) {
    if (channel >= MAX_PWM_CHANNELS) return;
    pwms[channel].period = (_XTAL_FREQ / (4 * frequency)) - 1;
}

void PWM_Start(uint8_t channel) {
    if (channel >= MAX_PWM_CHANNELS) return;
    pwms[channel].active = true;
    T1CONbits.TMR1ON = 1;  // Start Timer1
}

void PWM_Stop(uint8_t channel) {
    if (channel >= MAX_PWM_CHANNELS) return;
    pwms[channel].active = false;
    LATB &= ~(1 << pwms[channel].primary_pin);       // Ensure the primary pin is low
    LATB &= ~(1 << pwms[channel].complementary_pin); // Ensure the complementary pin is low
}

void __interrupt() ISR(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;  // Clear Timer1 interrupt flag
        
        for (uint8_t i = 0; i < MAX_PWM_CHANNELS; i++) {
            if (pwms[i].active) {
                if (pwms[i].pin_state) {
                    // Turn off primary and wait for dead time rise before turning on complementary
                    LATB &= ~(1 << pwms[i].primary_pin);
                    TMR1 = pwms[i].dead_time_rise;  // Apply rising dead time
                    pwms[i].pin_state = false;
                } else {
                    if (pwms[i].complementary_state) {
                        // Turn off complementary and wait for dead time fall before turning on primary
                        LATB &= ~(1 << pwms[i].complementary_pin);
                        TMR1 = pwms[i].dead_time_fall;  // Apply falling dead time
                        pwms[i].complementary_state = false;
                    } else {
                        // Turn on primary, and turn off complementary
                        LATB |= (1 << pwms[i].primary_pin);
                        TMR1 = pwms[i].on_time;
                        pwms[i].pin_state = true;
                    }
                    pwms[i].complementary_state = !pwms[i].complementary_state;
                }
            }
        }
    }
}
