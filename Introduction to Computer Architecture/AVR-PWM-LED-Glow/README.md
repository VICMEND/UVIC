# AVR PWM LED Glow

**Language:** C for the AVR ATmega2560 (`F_CPU` is 16 MHz)

**Build environment:** Windows with Microchip Studio (formerly Atmel Studio) and the AVR GCC toolchain. The source uses `avr/io.h`, hardware timers, and interrupts. It runs on the Arduino Mega or in the studio simulator. There is no desktop makefile.

Control the board LEDs from hardware timers. The program plays stored blink patterns, holds an LED at a chosen brightness with pulse-width modulation, and pulses an LED so it fades in and out.

## Breakdown

`a4.c` contains five parts, called in order from `main`:

1. `led_state` turns one LED on or off from an LED number and a zero or non-zero state
2. `sos` plays a stored blink pattern. `light[]` is the bit mask for each step and `duration[]` is how long that step stays on, in milliseconds
3. `glow` holds an LED at a fixed brightness by changing the PWM duty cycle
4. `pulse_glow` uses two interrupt timers to vary that duty cycle, so the LED fades in and out
5. A second stored pattern, in the same style as `sos`, matching the pattern from the lab

## Usage

Create an AVR GCC project in Microchip Studio for the ATmega2560, add `a4.c`, build, and flash the board. Watch the LEDs: a blink pattern, a steady glow, then a pulse. Leave the `DO NOT TOUCH` sections in the file as they are; the lab skeleton depends on them.
