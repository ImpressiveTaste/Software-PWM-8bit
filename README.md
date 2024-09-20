
# Software PWM Library for Microchip 8-bit Devices

## Overview

This library provides software-based PWM generation for Microchip 8-bit devices. It allows you to create multiple independent PWM channels, each with configurable frequency, duty cycle, and dead time. Additionally, it supports **complementary PWM** for motor control applications, including the ability to set separate dead times for rising and falling transitions between the primary and complementary pins.

## Features

- **Multiple PWM channels** (up to 4 by default, configurable)
- **Configurable frequency** and **duty cycle** for each PWM channel
- **Complementary PWM output** with a primary and complementary pin
- **Dead time control** for both rising and falling transitions
- **Non-blocking execution** using timer interrupts

## Requirements

- A Microchip 8-bit microcontroller (e.g., PIC16F, PIC18F series)
- Configured clock frequency (`_XTAL_FREQ` set to match your device's clock speed)
- Timer1 or another available timer for managing PWM timing

## Installation

1. **Clone the repository** or **download** the files directly:

   ```bash
   git clone https://github.com/your-username/soft-pwm-library.git
   ```

2. Add the following files to your MPLAB project:
   - `soft_pwm.c`
   - `soft_pwm.h`

3. Ensure your project is configured with the correct clock frequency by defining `_XTAL_FREQ` in your project.

## Usage

### 1. Initialization

First, initialize the PWM system in your main code:

```c
#include "soft_pwm.h"

void main(void) {
    PWM_Init();  // Initialize the PWM system
    // Your code here
}
```

### 2. Configuring a PWM Channel

To configure a PWM channel with primary and complementary outputs:

```c
// Example: Channel 0, RB5 as primary, RB6 as complementary, 1kHz, 50% duty, 100 cycle rise dead-time, 200 cycle fall dead-time
PWM_SetupComplementary(0, 5, 6, 1000, 50, 100, 200);
```

Parameters:
- `channel`: PWM channel index (0 to MAX_PWM_CHANNELS - 1)
- `primary_pin`: Pin number for the primary PWM signal
- `complementary_pin`: Pin number for the complementary PWM signal
- `frequency`: PWM frequency in Hz
- `duty_cycle`: Duty cycle as a percentage (0–100%)
- `dead_time_rise`: Dead time between turning off the primary pin and turning on the complementary pin
- `dead_time_fall`: Dead time between turning off the complementary pin and turning on the primary pin

### 3. Starting and Stopping PWM

Start PWM generation on a channel:

```c
PWM_Start(0);  // Start PWM on channel 0
```

Stop PWM generation on a channel:

```c
PWM_Stop(0);  // Stop PWM on channel 0
```

### 4. Adjusting Duty Cycle and Frequency

You can dynamically adjust the duty cycle and frequency for any PWM channel:

```c
PWM_SetDutyCycle(0, 75);  // Set duty cycle to 75% on channel 0
PWM_SetFrequency(0, 2000);  // Set frequency to 2kHz on channel 0
```

## Example

Here’s a full example of setting up complementary PWM on two pins with custom dead times:

```c
#include "soft_pwm.h"

void main(void) {
    PWM_Init();  // Initialize the PWM system
    
    // Setup complementary PWM on RB5 (primary) and RB6 (complementary)
    PWM_SetupComplementary(0, 5, 6, 1000, 50, 100, 200);  // 1kHz, 50% duty, 100 rise dead-time, 200 fall dead-time
    
    // Start the PWM on channel 0
    PWM_Start(0);
    
    while (1) {
        // Main loop
    }
}
```

## Configuration

You can configure the library by modifying the following in `soft_pwm.h`:

- `MAX_PWM_CHANNELS`: Set the maximum number of independent PWM channels (default: 4)
