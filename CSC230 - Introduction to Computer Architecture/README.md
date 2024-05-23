# CSC230-Intro to computer architecture
UVIC CSC 230 2024

The following are projects from CSC 230

# Assignment 1

Assignment was broken into 3 parts to be done using assembly language (AVR). 

The first was to write an assembly program that identifies the edit distance between two binary numbers.

The second was to reset the right-most contiguous sequence of bits in a binary number.

The third was to do the addition of two packed BCDs numbers.

------------
# Assignment 2
Assignment was to be completed using assembly language (AVR)

Assignment was broken into 5 tasks:

a) Write the function set_leds

b) Write the functions fast_leds and slow_leds

c) Write the function leds_with_speed

d) Write the function encode_letter

e) Write the function display_message

The parameter to set_leds determines which of the Arduino board’s six LEDs to turn on

fast/slow_leds runs set_leds and determines how long the lights stay on for

leds_with_speed determines if the fast or slow version will be used based on if the two top-most bits are set or unset

encode_letter decodes letters and their light configuration into the binary value corresponding to the lights in the arduino board. the configuration is stored in memory in the following format

.db "A", "..oo..", 1 Where the first item "A" is the letter the second item "..oo.." is the light configuration of that letter (. = light off, o = light on) and the 3rd item is how long it should stay on for (1 for 1s 2 for 0.25s )

display_message ties it all together by using labels in the format WORD07: .db "THE", 0 and displaying the corresponding message with the light display (in the chosen example the word is "THE")

The flow of the program is that it parses through each character in the word and compares it to the possible configurations in memory. If a comparison is succesful, the program parses through the light configuration in .s and os and transforms it into binary that the program can work with. The program then checks if it's a 1 or 2 and sets the 2 highest bits accordingly. Then displays the light with the correct speed and move onto to the next letter repeating untill the last chracter. 

------------
# Assignment 3
Asignment 3 made use of timers and interrupt handlers to display messages onto a 2x16 LCD display using the AVR asemply language

the message is displayed by pressing the up or down button on the board which cycles through the provided AVAILABLE_CHARSET: .db "0123456789abcdef_", in memory. The left and right buttons were used to move to the next column where another character could be placed.
on the second row the  first letter of corresponding direction was to be written on the LCD display when the specific button was pressed.

------------
# Assignment 4
A C program divided into 5 parts:

The first part was to create a function Led_State() which turns on lights based on two accepted parameters:

1. The number of an LCD
   
2.A number indicating the state to which that LCD must be put (with a zero value meaning “off”, and all other values meaning “on”).

The second part was to create a function called SOS() which contains the following:

uint8_t light[]: an array of 8-bit values indicate the LED pattern. An array value indicates LEDs on or off by bits set or cleared, with bit 0 the state for LED #0, bit 1 the state for LED #1, etc.

int duration[]: an array of ints (i.e., 16-bit values) representating the duration in milliseconds for an LED pattern.

int length: the number of elements in light[] and the number of elements in duration[].

The function must then call led_states() using these values to display the message.

The third part is a function glow() which uses timers to implement a duty cycle for an LED in ordr to simulate brightness.

The fourth part is the function pulse_glow() which is similar to glow() but it uses two interrupt timers to create a varying duty cycle which simulates the light pulsing.

The fifth part is to implement something similar to SOS() but the display is not provided and has to follow a pattern shown by the instructor.
