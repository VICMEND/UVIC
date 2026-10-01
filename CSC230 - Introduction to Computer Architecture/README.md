# CSC 230 — Introduction to Computer Architecture

UVIC CSC 230, 2022. Programs for the ATmega2560 (Arduino Mega), first in AVR assembly and then in C. The same work is also split into standalone folders under `Introduction to Computer Architecture`.

**Build environment:** Windows with Microchip Studio (formerly Atmel Studio), targeting an ATmega2560 clocked at 16 MHz. Assembly files include `m2560def.inc`. The C file uses the AVR libc headers (`avr/io.h`, timers, and interrupts).

These programs run on the board, or in the Microchip Studio simulator. They are not desktop Linux programs, and this folder has no makefile.

Open the `.asm` or `.c` file as the project source, build, and flash the board (or start the simulator). The LCD-shield labs expect the course LCD keypad shield wired to the Mega.

---

## Assignment 1 — Bit and BCD operations

**Language:** AVR assembly

Three short programs that work on bytes in registers.

**Breakdown**

- `A1/edit-distance.asm` — count how many bits differ between two values
- `A1/reset-rightmost.asm` — clear the rightmost contiguous run of set bits
- `A1/bcd-addition.asm` — add two packed-BCD numbers and keep the carry

**Usage**

Build and debug each file on its own in Microchip Studio. Each one is a standalone program with its inputs in registers, so the result is checked in the simulator’s register view.

---

## Assignment 2 — LED signalling

**Language:** AVR assembly

Spell a word on the board’s six LEDs. Each letter has a stored on/off pattern and a fast or slow timing.

The program is a set of functions that stack:

- `set_leds` turns on a subset of the six LEDs from a bit mask
- `fast_leds` and `slow_leds` hold that pattern for a short or long delay
- `leds_with_speed` picks the fast or slow delay from the two top bits of the pattern
- `encode_letter` looks up a character in a table such as `.db "A", "..oo..", 1` (`.` is off, `o` is on, and the number is the speed)
- `display_message` walks a string such as `WORD07: .db "THE", 0`, encodes each letter, and flashes it

**Breakdown**

- `A2/a2-signalling.asm` — LED helpers, the letter table, and the message display

**Usage**

Build `a2-signalling.asm` for the ATmega2560 and run it on the board or in the simulator. The message string near the bottom of the file is what the LEDs spell.

---

## Assignment 3 — LCD and buttons

**Language:** AVR assembly

Sample the LCD-shield buttons with the analog-to-digital converter on a timer, and show the result on the 2x16 character display.

The up and down buttons cycle a character from `AVAILABLE_CHARSET: .db "0123456789abcdef_"`. Left and right move the cursor to the next column. The second row shows the first letter of the direction that was pressed.

**Breakdown**

- `A3/a3part-D.asm` — ADC button sampling, timer interrupt, and LCD output

**Usage**

Build for the ATmega2560 and run it with the LCD keypad shield attached. Press the shield buttons to move the cursor and change the character.

---

## Assignment 4 — PWM LED glow

**Language:** C for the AVR ATmega2560 (`F_CPU` is 16 MHz)

Control the board LEDs from hardware timers.

**Breakdown**

`A4/a4.c` contains five parts:

1. `led_state` turns one LED on or off
2. `sos` plays a stored blink pattern from parallel `light[]` and `duration[]` arrays
3. `glow` holds an LED at a chosen brightness with pulse-width modulation
4. `pulse_glow` varies that duty cycle with two timers so the LED fades in and out
5. A second pattern display, in the same style as `sos`, matching the pattern from the lab

**Usage**

Create an AVR GCC project in Microchip Studio for the ATmega2560, add `a4.c`, build, and flash the board. The `main` function calls the LED routines in sequence.
