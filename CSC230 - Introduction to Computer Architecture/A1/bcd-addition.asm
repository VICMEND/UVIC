; bcd-addition.asm
; CSC 230: Fall 2022
;
; Code provided for Assignment #1
;
; Mike Zastre (2022-Sept-22)

; This skeleton of an assembly-language program is provided to help you
; begin with the programming task for A#1, part (c). In this and other
; files provided through the semester, you will see lines of code
; indicating "DO NOT TOUCH" sections. You are *not* to modify the
; lines within these sections. The only exceptions are for specific
; changes announced on conneX or in written permission from the course
; instructor. *** Unapproved changes could result in incorrect code
; execution during assignment evaluation, along with an assignment grade
; of zero. ****
;
; In a more positive vein, you are expected to place your code with the
; area marked "STUDENT CODE" sections.

; ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
; Your task: Two packed-BCD numbers are provided in R16
; and R17. You are to add the two numbers together, such
; the the rightmost two BCD "digits" are stored in R25
; while the carry value (0 or 1) is stored R24.
;
; For example, we know that 94 + 9 equals 103. If
; the digits are encoded as BCD, we would have
;   *  0x94 in R16
;   *  0x09 in R17
; with the result of the addition being:
;   * 0x03 in R25
;   * 0x01 in R24
;
; Similarly, we know than 35 + 49 equals 84. If 
; the digits are encoded as BCD, we would have
;   * 0x35 in R16
;   * 0x49 in R17
; with the result of the addition being:
;   * 0x84 in R25
;   * 0x00 in R24
;

; ANY SIGNIFICANT IDEAS YOU FIND ON THE WEB THAT HAVE HELPED
; YOU DEVELOP YOUR SOLUTION MUST BE CITED AS A COMMENT (THAT
; IS, WHAT THE IDEA IS, PLUS THE URL).



    .cseg
    .org 0

	; Some test cases below for you to try. And as usual
	; your solution is expected to work with values other
	; than those provided here.
	;
	; Your code will always be tested with legal BCD
	; values in r16 and r17 (i.e. no need for error checking).

	; 94 + 9 = 03, carry = 1
	 ;ldi r16, 0x94
	 ;ldi r17, 0x09

	; 86 + 79 = 65, carry = 1
	;ldi r16, 0x86
	;ldi r17, 0x79

	; 35 + 49 = 84, carry = 0
	;ldi r16, 0x35
	;ldi r17, 0x49

	; 32 + 41 = 73, carry = 0
	ldi r16, 0x32
	ldi r17, 0x41

; ==== END OF "DO NOT TOUCH" SECTION ==========

; **** BEGINNING OF "STUDENT CODE" SECTION **** 
	
	mov r18, r16 ; temporary storage for r16 value
	mov r19, r17 ; temporary storage for r17 value

	cbr r18, 0xF0 ; clear upper nibble
	cbr r19, 0xF0 ; clear up nibble

	add r18, r19 ; add r18 and r19

	cpi r18, 0x0A ; check if the addition of r18 and r19 exceeded 10
	
	
	brlo skip ; if less than 10 jump to skip

	subi r18, 0x0A ; subtract 10 from r18 to correct for overflow
	ldi r20, 0x01 ; set r20 to 1

skip:
	clc ; clear carry flag from previous cpi
	mov r25, r18 ; copy r18 to r25
	
	swap r16 ; swap upper nibble with low nibble
	swap r17 ; swap upper nibble with low nibble
	
	cbr r16, 0xF0 ;clear upper nibble after the swap
	cbr r17, 0xF0 ;clear upper nibble after the swap

	cpi r20, 0x00 ; check if r20 is 0 signifiying that the previous adition did not result in a carry

	breq skipset ; if r20 is set to 0 then skip setting the carry flag

	sec ; set carry for the addition.

skipset:

	adc r16, r17 ; add r17 and r16 with carry

	cpi r16, 0x0A ; check if r16 is greater than 10 if not skip adjustments

	brlo combine; skip adjustments

	subi r16, 0x0A ; subtract 10 from r16
	ldi r24, 0x01 ; set r24 as the carry flag because of overflow

combine:
	
	swap r16 ; put the nibble in the correct place
	add r25, r16 ; add the two correct nibbles together
	


; **** END OF "STUDENT CODE" SECTION ********** 

; ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
bcd_addition_end:
	rjmp bcd_addition_end



; ==== END OF "DO NOT TOUCH" SECTION ==========
