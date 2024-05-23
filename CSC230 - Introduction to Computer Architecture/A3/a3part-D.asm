;
; a3part-A.asm
;
; Part A of assignment #3
;
;
; Student name: Victor Mendes
; Student ID: V00905409
; Date of completed work: 19/03/2024
;
; **********************************
; Code provided for Assignment #3
;
; Author: Mike Zastre (2022-Nov-05)
;
; This skeleton of an assembly-language program is provided to help you 
; begin with the programming tasks for A#3. As with A#2 and A#1, there are
; "DO NOT TOUCH" sections. You are *not* to modify the lines within these
; sections. The only exceptions are for specific changes announced on
; Brightspace or in written permission from the course instruction.
; *** Unapproved changes could result in incorrect code execution
; during assignment evaluation, along with an assignment grade of zero. ***
;


; =============================================
; ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
; =============================================
;
; In this "DO NOT TOUCH" section are:
; 
; (1) assembler direction setting up the interrupt-vector table
;
; (2) "includes" for the LCD display
;
; (3) some definitions of constants that may be used later in
;     the program
;
; (4) code for initial setup of the Analog-to-Digital Converter
;     (in the same manner in which it was set up for Lab #4)
;
; (5) Code for setting up three timers (timers 1, 3, and 4).
;
; After all this initial code, your own solutions's code may start
;

.cseg
.org 0
	jmp reset

; Actual .org details for this an other interrupt vectors can be
; obtained from main ATmega2560 data sheet
;
.org 0x22
	jmp timer1

; This included for completeness. Because timer3 is used to
; drive updates of the LCD display, and because LCD routines
; *cannot* be called from within an interrupt handler, we
; will need to use a polling loop for timer3.
;
; .org 0x40
;	jmp timer3

.org 0x54
	jmp timer4

.include "m2560def.inc"
.include "lcd.asm"

.cseg
#define CLOCK 16.0e6
#define DELAY1 0.01
#define DELAY3 0.1
#define DELAY4 0.5

#define BUTTON_RIGHT_MASK 0b00000001	
#define BUTTON_UP_MASK    0b00000010
#define BUTTON_DOWN_MASK  0b00000100
#define BUTTON_LEFT_MASK  0b00001000

#define BUTTON_RIGHT_ADC  0x032
#define BUTTON_UP_ADC     0x0b0   ; was 0x0c3
#define BUTTON_DOWN_ADC   0x160   ; was 0x17c
#define BUTTON_LEFT_ADC   0x22b
#define BUTTON_SELECT_ADC 0x316

.equ PRESCALE_DIV=1024   ; w.r.t. clock, CS[2:0] = 0b101

; TIMER1 is a 16-bit timer. If the Output Compare value is
; larger than what can be stored in 16 bits, then either
; the PRESCALE needs to be larger, or the DELAY has to be
; shorter, or both.
.equ TOP1=int(0.5+(CLOCK/PRESCALE_DIV*DELAY1))
.if TOP1>65535
.error "TOP1 is out of range"
.endif

; TIMER3 is a 16-bit timer. If the Output Compare value is
; larger than what can be stored in 16 bits, then either
; the PRESCALE needs to be larger, or the DELAY has to be
; shorter, or both.
.equ TOP3=int(0.5+(CLOCK/PRESCALE_DIV*DELAY3))
.if TOP3>65535
.error "TOP3 is out of range"
.endif

; TIMER4 is a 16-bit timer. If the Output Compare value is
; larger than what can be stored in 16 bits, then either
; the PRESCALE needs to be larger, or the DELAY has to be
; shorter, or both.
.equ TOP4=int(0.5+(CLOCK/PRESCALE_DIV*DELAY4))
.if TOP4>65535
.error "TOP4 is out of range"
.endif

reset:
; ***************************************************
; **** BEGINNING OF FIRST "STUDENT CODE" SECTION ****
; ***************************************************

.def DATAH=r25  ;DATAH:DATAL  store 10 bits data from ADC
.def DATAL=r24
.def BOUNDARY_H=r1  ;hold high byte value of the threshold for button
.def BOUNDARY_L=r0

.equ ADCSRA_BTN=0x7A
.equ ADCSRB_BTN=0x7B
.equ ADMUX_BTN=0x7C
.equ ADCL_BTN=0x78
.equ ADCH_BTN=0x79

; Anything that needs initialization before interrupts
; start must be placed here.

; ***************************************************
; ******* END OF FIRST "STUDENT CODE" SECTION *******
; ***************************************************

; =============================================
; ====  START OF "DO NOT TOUCH" SECTION    ====
; =============================================

	; initialize the ADC converter (which is needed
	; to read buttons on shield). Note that we'll
	; use the interrupt handler for timer 1 to
	; read the buttons (i.e., every 10 ms)
	;
	ldi temp, (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)
	sts ADCSRA, temp
	ldi temp, (1 << REFS0)
	sts ADMUX, r16

	; Timer 1 is for sampling the buttons at 10 ms intervals.
	; We will use an interrupt handler for this timer.
	ldi r17, high(TOP1)
	ldi r16, low(TOP1)
	sts OCR1AH, r17
	sts OCR1AL, r16
	clr r16
	sts TCCR1A, r16
	ldi r16, (1 << WGM12) | (1 << CS12) | (1 << CS10)
	sts TCCR1B, r16
	ldi r16, (1 << OCIE1A)
	sts TIMSK1, r16

	; Timer 3 is for updating the LCD display. We are
	; *not* able to call LCD routines from within an 
	; interrupt handler, so this timer must be used
	; in a polling loop.
	ldi r17, high(TOP3)
	ldi r16, low(TOP3)
	sts OCR3AH, r17
	sts OCR3AL, r16
	clr r16
	sts TCCR3A, r16
	ldi r16, (1 << WGM32) | (1 << CS32) | (1 << CS30)
	sts TCCR3B, r16
	; Notice that the code for enabling the Timer 3
	; interrupt is missing at this point.

	; Timer 4 is for updating the contents to be displayed
	; on the top line of the LCD.
	ldi r17, high(TOP4)
	ldi r16, low(TOP4)
	sts OCR4AH, r17
	sts OCR4AL, r16
	clr r16
	sts TCCR4A, r16
	ldi r16, (1 << WGM42) | (1 << CS42) | (1 << CS40)
	sts TCCR4B, r16
	ldi r16, (1 << OCIE4A)
	sts TIMSK4, r16

	sei

; =============================================
; ====    END OF "DO NOT TOUCH" SECTION    ====
; =============================================

; ****************************************************
; **** BEGINNING OF SECOND "STUDENT CODE" SECTION ****
; ****************************************************

	;initialize LCD display and reset CURRENT_CHARSET_INDEX and CURRENT_CHAR_INDEX
start:
	rcall lcd_init
	clr r16
	sts CURRENT_CHARSET_INDEX, r16
	sts	CURRENT_CHAR_INDEX, r16

	;The following code borrows segments from the timer and LCD labs to set up timer functions and utilize the lcd functions
timer3:
	in r19, TIFR3
	sbrs r19, OCF3A
	rjmp timer3

	
	ldi temp, 1<<OCF3A
	out TIFR3, temp

	; grab the value in BIP and LBP and check if it is 1 indicating that the button is being pressed
	lds r22, LAST_BUTTON_PRESSED
	lds r18, BUTTON_IS_PRESSED
	cpi r18, 1
	brsh star ; go to the star if the button is pressed otherwise continue to display a dash

	; code taken from LED lab to display a character (-) loops back to top of timer for polling
dash:
	ldi r16, 2 ;row
	ldi r17, 15 ;column
	push r16
	push r17
	rcall lcd_gotoxy
	pop r17
	pop r16

	ldi r16, '-'
	push r16
	rcall lcd_putchar
	pop r16

	rjmp checkl  ; jumps to code segment checking which button was last pressed

; code taken from LED lab to display a character (*) loops back to top of timer for polling
star:
	ldi r16, 2 ;row
	ldi r17, 15 ;column
	push r16
	push r17
	rcall lcd_gotoxy
	pop r17
	pop r16

	ldi r16, '*'
	push r16
	rcall lcd_putchar
	pop r16

	ldi r16, 2 ;row
	ldi r17, 0 ;column
	push r16
	push r17
	rcall lcd_gotoxy
	pop r17
	pop r16


checkl:

	cpi r22, 1 ; compare LAST_BUTTON_PRESSED with 1 (select button - not in use)
	breq timer3 ; display nothing and return to start of loop

	cpi r22, 2 ; compare LAST_BUTTON_PRESSED with 2 (Left button)
	breq LEFT ; jump to the code section responsible for displaying an L

	cpi r22, 3 ; compare LAST_BUTTON_PRESSED with 3 (down button)
	breq DOWN ;jump to the code section responsible for displaying an D

	cpi r22, 4 ; compare LAST_BUTTON_PRESSED with 4 (up button)
	breq UP ; jump to the code section responsible for displaying an U

	cpi r22, 5 ; compare LAST_BUTTON_PRESSED with 5 (right button)
	breq RIGHT ;jump to the code section responsible for displaying a

	rjmp timer3

	; code segment responsible for displaying an L
LEFT:
	ldi r16, 'L'
	push r16
	rcall lcd_putchar
	pop r16

	;calls function clear to clear non L LCD spaces
	ldi r17, 3
	rcall CLEAR

	rjmp timer3

	; code segment responsible for displaying a D and displaying row 0 content
DOWN:
	; calls function clear to clear non D LCD spaces
	ldi r17, 1
	rcall CLEAR

	ldi r16, 'D'
	push r16
	rcall lcd_putchar
	pop r16
	
	; calls function clear to clear non D LCD spaces
	ldi r17, 2
	rcall CLEAR

	;check if button is being pressed
	cpi r18, 1
	brne SKIPD	;skip the following code if it is not being pressed
	ldi r16, 0 ;row
	lds r17, CURRENT_CHAR_INDEX ; use CURRENT_CHAR_INDEX as the column
	push r16
	push r17
	rcall lcd_gotoxy
	pop r17
	pop r16

	; send the content of TOP_LINE_CONTENT into lcd_putchar
	lds r16, TOP_LINE_CONTENT
	push r16
	rcall lcd_putchar
	pop r16

SKIPD:
	rjmp timer3

	;code segment responsible for displaying an U and displaying row 0 content
UP:
	;calls function clear to clear non U LCD spaces=
	ldi r17, 2
	rcall CLEAR

	ldi r16, 'U'
	push r16
	rcall lcd_putchar
	pop r16
	
	;check if button is being pressed
	ldi r17, 1
	;calls function clear to clear non R LCD spaces
	rcall CLEAR

	cpi r18, 1
	brne SKIPU	;skip the following code if it is not being pressed
	ldi r16, 0 ;row
	lds r17, CURRENT_CHAR_INDEX ;column
	push r16
	push r17
	rcall lcd_gotoxy
	pop r17
	pop r16

	; send the content of TOP_LINE_CONTENT into lcd_putchar
	lds r16, TOP_LINE_CONTENT
	push r16
	rcall lcd_putchar
	pop r16

SKIPU:
	rjmp timer3

	;code segment responsible for displaying an R
RIGHT:
	;calls function clear to clear non R LCD spaces
	ldi r17, 3
	rcall CLEAR

	ldi r16, 'R'
	push r16
	rcall lcd_putchar
	pop r16
	rjmp timer3

	;function that clears the LCD spaces not being pressed. Loops based on the value in r17 and places empty on LCD slots
CLEAR: 
	push r16
lloop:	
	dec r17

	ldi r16, ' '
	push r16
	rcall lcd_putchar
	pop r16
	
	cpi r17,0
	brne lloop
	
	pop r16
	ret

stop:
	rjmp stop


timer1:
	;The following code borrows segments from the timer and button labs to set up timer functions and determine button presses

	;protecting registers
	push r16
	push r17
	push r24


	lds	r16, ADCSRA	

	; bit 6 =1 ADSC (ADC Start Conversion bit), remain 1 if conversion not done
	; ADSC changed to 0 if conversion is done
	ori r16, 0x40 ; 0x40 = 0b01000000
	sts	ADCSRA, r16

	; wait for it to complete, check for bit 6, the ADSC bit
wait:	
		lds r16, ADCSRA_BTN
		andi r16, 0x40
		brne wait

		; read the value, use XH:XL to store the 10-bit result
		lds DATAL, ADCL_BTN
		lds DATAH, ADCH_BTN

		ldi r16, low(BUTTON_SELECT_ADC);
		mov BOUNDARY_L, r16
		ldi r16, high(BUTTON_SELECT_ADC)
		mov BOUNDARY_H, r16
		cp DATAL, BOUNDARY_L
		cpc DATAH, BOUNDARY_H
		brsh ButtonN ;if button value higher than BOUNDARY then no button is pressed so branch to that portion of the code			
		ldi r17, 1 ;if button value low than BOUNDARY then button is pressed so set BUTTON_IS_PRESSED to 1
		sts BUTTON_IS_PRESSED ,r17 ;load 1 into BUTTON_IS_PRESSED indicating button is pressed
		
		

		;check if right button. load BUTTON_RIGHT_ADC into boundary for comparison
		ldi r16, low(BUTTON_RIGHT_ADC);
		mov BOUNDARY_L, r16
		ldi r16, high(BUTTON_RIGHT_ADC)
		mov BOUNDARY_H, r16
		; if DATAH:DATAL < BOUNDARY_H:BOUNDARY_L
		;     r17=1  "right" button is pressed
		; else
		;     r17=0
		cp DATAL, BOUNDARY_L
		cpc DATAH, BOUNDARY_H
		brsh ButtonU ;if button value higher than BOUNDARY then check next button threshold		
		ldi r17, 5
		sts LAST_BUTTON_PRESSED ,r17 ; load 5 into LAST_BUTTON_PRESSED if right is being pressed indicating button is pressed
		rjmp end
		
ButtonU:
		ldi r16, low(BUTTON_UP_ADC);
		mov BOUNDARY_L, r16
		ldi r16, high(BUTTON_UP_ADC)
		mov BOUNDARY_H, r16
		cp DATAL, BOUNDARY_L
		cpc DATAH, BOUNDARY_H
		brsh ButtonD ;if button value higher than BOUNDARY then check next button threshold				

		ldi r17, 4
		sts LAST_BUTTON_PRESSED ,r17 ; load 2 into LAST_BUTTON_PRESSED if up is being pressed indicating button is pressed
		rjmp end

ButtonD:
		ldi r16, low(BUTTON_DOWN_ADC);
		mov BOUNDARY_L, r16
		ldi r16, high(BUTTON_DOWN_ADC)
		mov BOUNDARY_H, r16
		cp DATAL, BOUNDARY_L
		cpc DATAH, BOUNDARY_H
		brsh ButtonL ;if button value higher than BOUNDARY then check next button threshold				
		
		ldi r17, 3
		sts LAST_BUTTON_PRESSED ,r17 ; load 3 into LAST_BUTTON_PRESSED if down is being pressed indicating button is pressed
		rjmp end

ButtonL:
		ldi r16, low(BUTTON_LEFT_ADC);
		mov BOUNDARY_L, r16
		ldi r16, high(BUTTON_LEFT_ADC)
		mov BOUNDARY_H, r16
		cp DATAL, BOUNDARY_L
		cpc DATAH, BOUNDARY_H
		brsh end ;if button value higher than BOUNDARY then exit out.
		ldi r17, 2
		sts LAST_BUTTON_PRESSED ,r17 ; load 2 into LAST_BUTTON_PRESSED if left is being pressed indicating button is pressed
		rjmp end

	; code segment setting BUTTON_IS_PRESSED to 0 if nothing was pressed
ButtonN:
		ldi r17,0
		sts BUTTON_IS_PRESSED ,r17

end:
		;restoring registers
		pop r24
		pop r17
		pop r16
		reti

;timer3
;
; Note: There is no "timer3" interrupt handler as you must use
; timer3 in a polling style (i.e. it is used to drive the refreshing
; of the LCD display, but LCD functions cannot be called/used from
; within an interrupt handler).


timer4:
	; protecting registers
	push r18
	push r17
	push r16
	push r19
	push r20

	clr r19

	;loading AVAILABLE_CHARSET address into Z
	ldi ZH, high(AVAILABLE_CHARSET<<1)
	ldi ZL, low(AVAILABLE_CHARSET<<1) 

	lds r17, BUTTON_IS_PRESSED
	cpi r17, 1
	brne TEMP4END ; exit if no button is being pressed

	;check which button is being pressed
	lds r18, LAST_BUTTON_PRESSED
	cpi r18, 3 ; check if down is being pressed
	breq T4DOWN ; go to code segment responsible for down behaviour

	cpi r18, 2 ; check if left is being pressed
	breq T4LEFT ;go to code segment responsible for left behaviour

	cpi r18, 5 ; check if right is being pressed
	breq T4RIGHT ;go to code segment responsible for right behaviour
	
	cpi r18, 4 ; cehck if up being pressed
	breq T4UP ; go to code segment responsible for up behaviour
	brne TEMP4END ; exit if neither is pressed

	;code section responsible for for down behaviour
T4DOWN:
	; store Z  address bytes into r16 and r18
	mov r16, ZH
	mov r18, ZL

	; store CURRENT_CHARSET_INDEX into r17
	lds r17, CURRENT_CHARSET_INDEX

	;if CURRENT_CHARSET_INDEX = 0 then do not decriment CURRENT_CHARSET_INDEX value to prevent underflow
	cpse r17, r19
	dec r17

	;add CURRENT_CHARSET_INDEX into Z address values
	add r18, r17
	adc r16, r19

	;move the value back into Z
	mov ZH, r16
	mov ZL, r18

	; load the data in the current Z address into r18 then load that value into TOP_LINE_CONTENT also update CURRENT_CHARSET_INDEX since it was decrimented
	lpm r18, Z
	sts CURRENT_CHARSET_INDEX ,r17
	sts TOP_LINE_CONTENT ,r18
	
	ldi r16, 0

TEMP4END:
	rjmp T4END
	

;code section responsible for up behaviour
T4UP:
	; store Z  address bytes into r16 and r18
	mov r16, ZH
	mov r18, ZL

	; store CURRENT_CHARSET_INDEX into r17
	lds r17, CURRENT_CHARSET_INDEX

	;increment CURRENT_CHARSET_INDEX
	inc r17
	clr r19

	;add CURRENT_CHARSET_INDEX into Z address values
	add r18, r17
	adc r16, r19

	;move the value back into Z
	mov ZH, r16
	mov ZL, r18

	; load the data in the current Z address into r18 then load that value into TOP_LINE_CONTENT also update CURRENT_CHARSET_INDEX since it was incremented
	lpm r18, Z
	cpi r18, 0x00 ;check if the value in r18 is NULL if yes it is the end of the string so the values of TOP_LINE_CONTENT and CURRENT_CHARSET_INDEX should not be updated in the memory
	breq ENDU
	sts CURRENT_CHARSET_INDEX ,r17
	sts TOP_LINE_CONTENT ,r18

ENDU:
	ldi r16, 0
	rjmp T4END

	;code section responsible for left behaviour
T4LEFT:
	lds r17, CURRENT_CHAR_INDEX ;load  CURRENT_CHAR_INDEX into r17
	cp r17, r19 ;check if CURRENT_CHAR_INDEX is 15 and skip to end if yes indicating that we have reached the last LCD slot
	breq T4END
	dec r17 ; decriment CURRENT_CHAR_INDEX
	sts CURRENT_CHAR_INDEX, r17 ; load new CURRENT_CHAR_INDEX into CURRENT_CHAR_INDEX
	clr r19
	sts CURRENT_CHARSET_INDEX ,r19 ; reset position of CURRENT_CHARSET_INDEX so it starts from the beggining in the new slot
	ldi ZH, high(AVAILABLE_CHARSET<<1)
	ldi ZL, low(AVAILABLE_CHARSET<<1) 
	lpm r19, Z+
	sts TOP_LINE_CONTENT, r19 ; resets TOP_LINE_CONTENT
	clr r19
	rjmp T4END

	;code section responsible for right behaviour
T4RIGHT:
	lds r17, CURRENT_CHAR_INDEX ;load  CURRENT_CHAR_INDEX into r17 
	ldi r19, 0x0F ; load 15 to r19
	cp r17, r19 ;check if CURRENT_CHAR_INDEX is 15 and skip to end if yes indicating that we have reached the last LCD slot
	breq T4END
	inc r17 ; increment CURRENT_CHAR_INDEX
	sts CURRENT_CHAR_INDEX, r17 ; load new CURRENT_CHAR_INDEX value into CURRENT_CHAR_INDEX
	clr r19
	sts CURRENT_CHARSET_INDEX ,r19 ; reset position of CURRENT_CHARSET_INDEX so it starts from the beggining in the new slot
	ldi ZH, high(AVAILABLE_CHARSET<<1) ;
	ldi ZL, low(AVAILABLE_CHARSET<<1) 
	lpm r19, Z+
	sts TOP_LINE_CONTENT, r19 ; resets TOP_LINE_CONTENT
	clr r19

T4END:
	;restoring registers
	pop r20
	pop r19
	pop r16
	pop r17
	pop r18

	reti


; ****************************************************
; ******* END OF SECOND "STUDENT CODE" SECTION *******
; ****************************************************


; =============================================
; ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
; =============================================

; r17:r16 -- word 1
; r19:r18 -- word 2
; word 1 < word 2? return -1 in r25
; word 1 > word 2? return 1 in r25
; word 1 == word 2? return 0 in r25
;
compare_words:
	; if high bytes are different, look at lower bytes
	cp r17, r19
	breq compare_words_lower_byte

	; since high bytes are different, use these to
	; determine result
	;
	; if C is set from previous cp, it means r17 < r19
	; 
	; preload r25 with 1 with the assume r17 > r19
	ldi r25, 1
	brcs compare_words_is_less_than
	rjmp compare_words_exit

compare_words_is_less_than:
	ldi r25, -1
	rjmp compare_words_exit

compare_words_lower_byte:
	clr r25
	cp r16, r18
	breq compare_words_exit

	ldi r25, 1
	brcs compare_words_is_less_than  ; re-use what we already wrote...

compare_words_exit:
	ret

.cseg
AVAILABLE_CHARSET: .db "0123456789abcdef_", 0


.dseg

BUTTON_IS_PRESSED: .byte 1			; updated by timer1 interrupt, used by LCD update loop
LAST_BUTTON_PRESSED: .byte 1        ; updated by timer1 interrupt, used by LCD update loop

TOP_LINE_CONTENT: .byte 16			; updated by timer4 interrupt, used by LCD update loop
CURRENT_CHARSET_INDEX: .byte 16		; updated by timer4 interrupt, used by LCD update loop
CURRENT_CHAR_INDEX: .byte 1			; ; updated by timer4 interrupt, used by LCD update loop


; =============================================
; ======= END OF "DO NOT TOUCH" SECTION =======
; =============================================


; ***************************************************
; **** BEGINNING OF THIRD "STUDENT CODE" SECTION ****
; ***************************************************

.dseg

; If you should need additional memory for storage of state,
; then place it within the section. However, the items here
; must not be simply a way to replace or ignore the memory
; locations provided up above.


; ***************************************************
; ******* END OF THIRD "STUDENT CODE" SECTION *******
; ***************************************************
