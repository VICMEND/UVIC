# AVR LCD Buttons

**Language:** AVR assembly

**Build environment:** Windows with Microchip Studio (formerly Atmel Studio), targeting an ATmega2560 at 16 MHz with the course LCD keypad shield attached. The source includes `m2560def.inc` and talks to the ADC and a timer interrupt. There is no desktop makefile.

Sample the shield buttons with the analog-to-digital converter on a timer, and show which button was pressed on the 2x16 character display.

## Breakdown

`a3part-D.asm` is the whole program. Up and down cycle a character from the charset stored as `AVAILABLE_CHARSET: .db "0123456789abcdef_"`. Left and right move the cursor to the next column. The second row shows the first letter of the direction that was pressed.

## Usage

Build `a3part-D.asm` in Microchip Studio for the ATmega2560 and flash it to a Mega that has the LCD keypad shield connected. Press left, right, up, and down on the shield. The display updates from the timer interrupt that reads the ADC.
