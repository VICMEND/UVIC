# AVR LED Signalling

**Language:** AVR assembly

**Build environment:** Windows with Microchip Studio (formerly Atmel Studio), targeting an ATmega2560 at 16 MHz. The source includes `m2560def.inc`. It runs on the Arduino Mega or in the studio simulator. There is no desktop makefile.

Spell a word on six LEDs. Each letter has a stored on/off pattern and a fast or slow timing, and the program walks through a string such as `HELLOWORLD`, flashing that pattern for each character.

## Breakdown

`a2-signalling.asm` is the whole program:

- `set_leds` turns on a subset of the six LEDs from a bit mask
- `fast_leds` and `slow_leds` hold that pattern for a short or long delay
- `leds_with_speed` picks the delay from the two top bits of the pattern
- `encode_letter` looks up a character in a table such as `.db "A", "..oo..", 1` (`.` is off, `o` is on, and the number selects the speed)
- `display_message` walks a stored string, encodes each letter, and flashes it

## Usage

Create an AVR assembly project in Microchip Studio for the ATmega2560, add `a2-signalling.asm`, build, and flash the board or start the simulator. The word that gets spelled is the string label near the bottom of the file (for example `WORD07: .db "THE", 0`). Change that string to display a different word.
