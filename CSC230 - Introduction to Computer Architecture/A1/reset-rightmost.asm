; reset-rightmost.asm
; CSC 230: Fall 2022
;
; Code provided for Assignment #1
;
; Mike Zastre (2022-Sept-22)

; This skeleton of an assembly-language program is provided to help you
; begin with the programming task for A#1, part (b). In this and other
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
; Your task: You are to take the bit sequence stored in R16,
; and to reset the rightmost contiguous sequence of set
; by storing this new value in R25. For example, given
; the bit sequence 0b01011100, resetting the right-most
; contigous sequence of set bits will produce 0b01000000.
; As another example, given the bit sequence 0b10110110,
; the result will be 0b10110000.
;
; Your solution must work, of course, for bit sequences other
; than those provided in the example. (How does your
; algorithm handle a value with no set bits? with all set bits?)

; ANY SIGNIFICANT IDEAS YOU FIND ON THE WEB THAT HAVE HELPED
; YOU DEVELOP YOUR SOLUTION MUST BE CITED AS A COMMENT (THAT
; IS, WHAT THE IDEA IS, PLUS THE URL).

    .cseg
    .org 0

; ==== END OF "DO NOT TOUCH" SECTION ==========

	ldi R16, 0b01011100
	; ldi R16, 0b10110110
	


	; THE RESULT **MUST** END UP IN R25

; **** BEGINNING OF "STUDENT CODE" SECTION **** 

	mov r25, r16 ; store r16 in r25
	cpi r25, 0x00 ; compare r25 and 0
	breq reset_rightmost_stop ; if r25 = 0 then skip to end
	

first0s:
	inc r17 ; increment r17 (the counter)
	lsr r25 ; shift bits to right saving shifted bit in the carry
	brcc first0s ; if shifted bit is a 0 loop back until locating a 1

found1s:

	inc r17 ; increase counter
	lsr r25 ; shift bit to right saving shifted bit in the carry
	brcs found1s ; loop this section until the previous operation finds a 0 signifying an end to the right most contigiuous set bits.

shiftback:
	lsl r25 ; shift bits to the left by 1
	dec r17 ; decriment the counter
	cpi r17, 0x00 ; check if counter r17 is 0
	brne shiftback ; loop until all right shifts are undone and contigious bits have been reset


; **** END OF "STUDENT CODE" SECTION ********** 



; ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
reset_rightmost_stop:
    rjmp reset_rightmost_stop


; ==== END OF "DO NOT TOUCH" SECTION ==========
