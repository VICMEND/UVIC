; a2-signalling.asm
; CSC 230: Fall 2022
;
; Student name:
; Student ID:
; Date of completed work:
;
; *******************************
; Code provided for Assignment #2
;
; Author: Mike Zastre (2022-Oct-15)
;
 
; This skeleton of an assembly-language program is provided to help you
; begin with the programming tasks for A#2. As with A#1, there are "DO
; NOT TOUCH" sections. You are *not* to modify the lines within these
; sections. The only exceptions are for specific changes changes
; announced on Brightspace or in written permission from the course
; instructor. *** Unapproved changes could result in incorrect code
; execution during assignment evaluation, along with an assignment grade
; of zero. ****

.include "m2560def.inc"
.cseg
.org 0

; ***************************************************
; **** BEGINNING OF FIRST "STUDENT CODE" SECTION ****
; ***************************************************

	; initializion code will need to appear in this
    ; section



; ***************************************************
; **** END OF FIRST "STUDENT CODE" SECTION **********
; ***************************************************

; ---------------------------------------------------
; ---- TESTING SECTIONS OF THE CODE -----------------
; ---- TO BE USED AS FUNCTIONS ARE COMPLETED. -------
; ---------------------------------------------------
; ---- YOU CAN SELECT WHICH TEST IS INVOKED ---------
; ---- BY MODIFY THE rjmp INSTRUCTION BELOW. --------
; -----------------------------------------------------

	rjmp test_part_e
	; Test code


test_part_a:
	ldi r16, 0b00100001
	rcall set_leds
	rcall delay_long

	clr r16
	rcall set_leds
	rcall delay_long

	ldi r16, 0b00111000
	rcall set_leds
	rcall delay_short

	clr r16
	rcall set_leds
	rcall delay_long

	ldi r16, 0b00100001
	rcall set_leds
	rcall delay_long

	clr r16
	rcall set_leds

	rjmp end


test_part_b:
	ldi r17, 0b00101010
	rcall slow_leds
	ldi r17, 0b00010101
	rcall slow_leds
	ldi r17, 0b00101010
	rcall slow_leds
	ldi r17, 0b00010101
	rcall slow_leds

	rcall delay_long
	rcall delay_long

	ldi r17, 0b00101010
	rcall fast_leds
	ldi r17, 0b00010101
	rcall fast_leds
	ldi r17, 0b00101010
	rcall fast_leds
	ldi r17, 0b00010101
	rcall fast_leds
	ldi r17, 0b00101010
	rcall fast_leds
	ldi r17, 0b00010101
	rcall fast_leds
	ldi r17, 0b00101010
	rcall fast_leds
	ldi r17, 0b00010101
	rcall fast_leds

	rjmp end

test_part_c:
	ldi r16, 0b11111000
	push r16
	rcall leds_with_speed
	pop r16

	ldi r16, 0b11011100
	push r16
	rcall leds_with_speed
	pop r16

	ldi r20, 0b00100000
test_part_c_loop:
	push r20
	rcall leds_with_speed
	pop r20
	lsr r20
	brne test_part_c_loop

	rjmp end


test_part_d:
	ldi r21, 'E'
	push r21
	rcall encode_letter
	pop r21
	push r25
	rcall leds_with_speed
	pop r25

	rcall delay_long

	ldi r21, 'A'
	push r21
	rcall encode_letter
	pop r21
	push r25
	rcall leds_with_speed
	pop r25

	rcall delay_long


	ldi r21, 'M'
	push r21
	rcall encode_letter
	pop r21
	push r25
	rcall leds_with_speed
	pop r25

	rcall delay_long

	ldi r21, 'H'
	push r21
	rcall encode_letter
	pop r21
	push r25
	rcall leds_with_speed
	pop r25

	rcall delay_long

	rjmp end


test_part_e:
	ldi r25, HIGH(WORD00 << 1)
	ldi r24, LOW(WORD00 << 1)
	rcall display_message
	rjmp end

end:
    rjmp end






; ****************************************************
; **** BEGINNING OF SECOND "STUDENT CODE" SECTION ****
; ****************************************************

set_leds:
	; preserve registers
	push r17
	push r18
	push r19

	;clear registers going into ports
	clr r17
	clr r18

	; check if corresponding bits are on or off
	
	mov r19, r16 ; place r16 value into r19
	andi r19, 0x01 ;compare r19 with bit mask to check if bit is set
	sbrc r19, 0 ;skip next instruction if bit was not set
	sbr r17, 128 ; set bit 7 of r17

	mov r19, r16 ;reset r19
	andi r19, 0x02 ;compare r19 with corresponding bit mask to check if bit is set
	sbrc r19, 1 ;skip next instruction if bit was not set
	sbr r17, 32 ; set bit 5 of r17

	mov r19, r16 ;reset r19
	andi r19, 0x04 ;compare r19 with corresponding bit mask to check if bit is set
	sbrc r19, 2 ;skip next instruction if bit was not set
	sbr r17, 8; set bit 3 of r17

	mov r19, r16 ;reset r19
	andi r19, 0x08 ;compare r19 with corresponding bit mask to check if bit is set
	sbrc r19, 3 ;skip next instruction if bit was not set
	sbr r17, 2; set bit 1 of r17

	mov r19, r16 ;reset r19
	andi r19, 0x10 ;compare r19 with corresponding bit mask to check if bit is set
	sbrc r19, 4 ;skip next instruction if bit was not set
	sbr r18, 8; set bit 3 of r18

	mov r19, r16 ;reset r19
	andi r19, 0x20 ;compare r19 with corresponding bit mask to check if bit is set
	sbrc r19, 5 ;skip next instruction if bit was not set
	sbr r18, 2; set bit 1 of r18

end_a:
	sts PORTL, r17 ; set configuration of LEDs 3,4,5,6
	out PORTB, r18 ; set configuration of LEDs 1,2

	;restore registers
	pop r19
	pop r18
	pop r17

	ret


slow_leds:
	
	;preserve registers
	push r16
	push r17

	; call set_leds with a long delay after
	mov r16,r17
	rcall set_leds
	rcall delay_long
	
	;turn off all lights 
	ldi r16, 0x00
	sts PORTL, r16
	out PORTB, r16
	
	;restore registers
	pop r17
	pop r16

	ret


fast_leds:
	;preserve register values
	push r16
	push r17

	;call set_leds with a short delay after
	mov r16,r17
	rcall set_leds
	rcall delay_short
	
	;turn off all lights
	ldi r16, 0x00
	sts PORTL, r16
	out PORTB, r16
	
	;restore registers
	pop r17
	pop r16

	ret


leds_with_speed:
	
	;preserve registers
	push ZL
	push ZH
	push r17
	push r18

	; store stack pointer in Z
	in ZH, SPH
	in ZL, SPL

	; determine if slow_leds or fast_leds should be called
	ldd r17, Z+8 ; load the value into r17
	mov r18, r17 ; use r18 as temporary storage for r17
	andi r18, 0xC0 ; perform and operation with mask 0b11000000 to check if fast or slow leds should be called
	breq go_fast ; if bits 6 and 7 were off go the fast light configuration, go to slow lights otherwise
	
	rcall slow_leds 
	
	;restore registers
	pop r18
	pop r17
	pop ZH
	pop ZL
	ret

go_fast:
	;Use fast lights configuration, restore registers and exit function.
	rcall fast_leds
	pop r18
	pop r17
	pop ZH
	pop ZL
	ret


; Note -- this function will only ever be tested
; with upper-case letters, but it is a good idea
; to anticipate some errors when programming (i.e. by
; accidentally putting in lower-case letters). Therefore
; the loop does explicitly check if the hyphen/dash occurs,
; in which case it terminates with a code not found
; for any legal letter.

encode_letter:
	;clear r25 and preserve register values
	clr r25
	push ZL
	push ZH
	push r17
	push r18
	push r19
	
	;load stack pointer
	in ZH, SPH
	in ZL, SPL

	;load relevant values to be checked
	ldd r17, Z+9
	ldi r19, 0x2e;

	;load position of PATTERNS into Z
	ldi ZH, high(PATTERNS<<1)
	ldi ZL, low(PATTERNS<<1) 
	 
	 ;loop through patterns to find corresponding letter
compare:	
	lpm r18, Z+ ;  load the value in the address pointed to by Z
	cpi r18, 0x2d ; check if r18 is holding the hyphen
	breq endp ; exit program if it is the hyphen
	cp r17, r18 ; compare letter in r17 with letter in r18
	brne compare ; loop if not found
	 
	 ; go through the symbols in PATTERNS and set the bits in r25 accordingly

	lpm r18, Z+ ;load into r18 what the Z register points to
	cpse r18, r19 ;checks if r18 is reading a o or . (if reading . skip next instruction)
	sbr r25, 32 ; set bit 5

	; repeat previous step to check if bits 0 through 4 should be set
	lpm r18, Z+
	cpse r18, r19
	sbr r25, 16 ; set bit 4


	lpm r18, Z+
	cpse r18, r19
	sbr r25, 8 ; set bit 3


	lpm r18, Z+
	cpse r18, r19
	sbr r25, 4 ; set bit 2
	 

	lpm r18, Z+
	cpse r18, r19
	sbr r25, 2 ; set bit 1


	lpm r18, Z+
	cpse r18, r19
	sbr r25, 1 ;set bit 0

	; check the speed bits (bits 6 and 7)
	lpm r18, Z+ ;load into r18 what the Z register points to in mem
	cpi r18, 0x02 ;compare r18 to 2 (if 2 then it's fast and bits 6 and 7 remain cleared)
	breq endp ; skip setting bits if found a 2 (set bits 6 and 7 otherwise)
	sbr r25, 64 ; set bit 6 
	sbr r25, 128; set bit 7

endp:
	;restore registers
	pop r19
	pop r18
	pop r17
	pop ZH
	pop ZL
	ret


display_message:
	;preserve register values
	push ZH
	push ZL
	push r18

	;move r25 and r24 to the z register
	mov ZH, r25
	mov ZL, r24


word_repeat:
	
	lpm r18,Z+ ; save value in the memory address located in Z into r18
	tst r18 ;check if r18 is 0 (End of string)
	breq fin ;end program if end

	push r18 ; push r18 to stack for encode_letter to use
	rcall encode_letter ; encode the letter
	pop r18 ; restore r18

	push r25 ; push r25 generated by encode_letter into the stack for leds_with_speed to use
	rcall leds_with_speed ; turn the lights on given the configuration
	pop r25 ; restore r25
	
	;small delay similar to the one in the youtube video
	rcall delay_short
	rcall delay_short
	tst r18 ; this is to use the following branch function
    brne word_repeat ; loop back to the next letter

fin:
	;restore register values and exit function
	pop r18
	pop ZL
	pop ZH
	ret


; ****************************************************
; **** END OF SECOND "STUDENT CODE" SECTION **********
; ****************************************************




; =============================================
; ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
; =============================================

; about one second
delay_long:
	push r16

	ldi r16, 14
delay_long_loop:
	rcall delay
	dec r16
	brne delay_long_loop

	pop r16
	ret


; about 0.25 of a second
delay_short:
	push r16
	ldi r16, 4
delay_short_loop:
	rcall delay
	dec r16
	brne delay_short_loop

	pop r16
	ret

; When wanting about a 1/5th of a second delay, all other
; code must call this function
;
delay:
	rcall delay_busywait
	ret


; This function is ONLY called from "delay", and
; never directly from other code. Really this is
; nothing other than a specially-tuned triply-nested
; loop. It provides the delay it does by virtue of
; running on a mega2560 processor.
;
delay_busywait:
	push r16
	push r17
	push r18

	ldi r16, 0x08
delay_busywait_loop1:
	dec r16
	breq delay_busywait_exit

	ldi r17, 0xff
delay_busywait_loop2:
	dec r17
	breq delay_busywait_loop1

	ldi r18, 0xff
delay_busywait_loop3:
	dec r18
	breq delay_busywait_loop2
	rjmp delay_busywait_loop3

delay_busywait_exit:
	pop r18
	pop r17
	pop r16
	ret


; Some tables
;.cseg ;change if not working
;.org 0x600

PATTERNS:
	; LED pattern shown from left to right: "." means off, "o" means
    ; on, 1 means long/slow, while 2 means short/fast.
	.db "A", "..oo..", 1
	.db "B", ".o..o.", 2
	.db "C", "o.o...", 1
	.db "D", ".....o", 1
	.db "E", "oooooo", 1
	.db "F", ".oooo.", 2
	.db "G", "oo..oo", 2
	.db "H", "..oo..", 2
	.db "I", ".o..o.", 1
	.db "J", ".....o", 2
	.db "K", "....oo", 2
	.db "L", "o.o.o.", 1
	.db "M", "oooooo", 2
	.db "N", "oo....", 1
	.db "O", ".oooo.", 1
	.db "P", "o.oo.o", 1
	.db "Q", "o.oo.o", 2
	.db "R", "oo..oo", 1
	.db "S", "....oo", 1
	.db "T", "..oo..", 1
	.db "U", "o.....", 1
	.db "V", "o.o.o.", 2
	.db "W", "o.o...", 2
	.db "X", "oo....", 2
	.db "Y", "..oo..", 2
	.db "Z", "o.....", 2
	.db "-", "o...oo", 1   ; Just in case!

WORD00: .db "HELLOWORLD", 0, 0
WORD01: .db "THE", 0
WORD02: .db "QUICK", 0
WORD03: .db "BROWN", 0
WORD04: .db "FOX", 0
WORD05: .db "JUMPED", 0, 0
WORD06: .db "OVER", 0, 0
WORD07: .db "THE", 0
WORD08: .db "LAZY", 0, 0
WORD09: .db "DOG", 0

; =======================================
; ==== END OF "DO NOT TOUCH" SECTION ====
; =======================================

