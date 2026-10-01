# AVR Bit and BCD Operations

**Language:** AVR assembly

**Build environment:** Windows with Microchip Studio (formerly Atmel Studio), targeting an ATmega2560 at 16 MHz. Each file includes `m2560def.inc`. These run on the board or in the studio simulator. There is no desktop makefile.

Three short programs on bytes in registers.

## Breakdown

- `edit-distance.asm` — count how many bits differ between two values
- `reset-rightmost.asm` — clear the rightmost contiguous run of set bits
- `bcd-addition.asm` — add two packed-BCD numbers and keep the carry

## Usage

Open one `.asm` file as its own Microchip Studio project for the ATmega2560, build it, and run it in the simulator. Inputs and results live in registers, so read them from the register view after the program stops. Repeat for the other two files.
