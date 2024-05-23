/* a4.c
 * CSC Fall 2022
 * 
 * Student name:
 * Student UVic ID:
 * Date of completed work:
 *
 *
 * Code provided for Assignment #4
 *
 * Author: Mike Zastre (2022-Nov-22)
 *
 * This skeleton of a C language program is provided to help you
 * begin the programming tasks for A#4. As with the previous
 * assignments, there are "DO NOT TOUCH" sections. You are *not* to
 * modify the lines within these section.
 *
 * You are also NOT to introduce any new program-or file-scope
 * variables (i.e., ALL of your variables must be local variables).
 * YOU MAY, however, read from and write to the existing program- and
 * file-scope variables. Note: "global" variables are program-
 * and file-scope variables.
 *
 * UNAPPROVED CHANGES to "DO NOT TOUCH" sections could result in
 * either incorrect code execution during assignment evaluation, or
 * perhaps even code that cannot be compiled.  The resulting mark may
 * be zero.
 */


/* =============================================
 * ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
 * =============================================
 */

#define __DELAY_BACKWARD_COMPATIBLE__ 1
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY1 0.000001
#define DELAY3 0.01

#define PRESCALE_DIV1 8
#define PRESCALE_DIV3 64
#define TOP1 ((int)(0.5 + (F_CPU/PRESCALE_DIV1*DELAY1))) 
#define TOP3 ((int)(0.5 + (F_CPU/PRESCALE_DIV3*DELAY3)))

#define PWM_PERIOD ((long int)500)

volatile long int count = 0;
volatile long int slow_count = 0;


ISR(TIMER1_COMPA_vect) {
	count++;
}


ISR(TIMER3_COMPA_vect) {
	slow_count += 5;
}

/* =======================================
 * ==== END OF "DO NOT TOUCH" SECTION ====
 * =======================================
 */


/* *********************************************
 * **** BEGINNING OF "STUDENT CODE" SECTION ****
 * *********************************************
 */

void led_state(uint8_t LED, uint8_t state) {
	switch (LED) {
		case 0: // if LED 0 is passed to function 
		if(state == 0){
			PORTL &= 0b01111111;
			}else{
			PORTL |= 0b10000000;
		}
		break;
		case 1: //if LED 1 is passed to function 
		if(state == 0){
			PORTL &= 0b11011111;
			}else{
			PORTL |= 0b00100000;
		}
		break;
		case 2: //if LED 2 is passed to function 
		if(state == 0){
			PORTL &= 0b11110111;
			}else{
			PORTL |= 0b00001000;
		}
		break;
		case 3: //if LED 3 is passed to function 
		if(state == 0){
			PORTL &= 0b11111101;
			} else{
			PORTL |= 0b00000010;
		}
		break;
		default: //if incorrect led is passed to function 
		PORTL = 0b00000000;
		break;
	}
}


void SOS() {
    uint8_t light[] = {
        0x1, 0, 0x1, 0, 0x1, 0,
        0xf, 0, 0xf, 0, 0xf, 0,
        0x1, 0, 0x1, 0, 0x1, 0,
        0x0
    };

    int duration[] = {
        100, 250, 100, 250, 100, 500,
        250, 250, 250, 250, 250, 500,
        100, 250, 100, 250, 100, 250,
        250
    };

	int length = 19;
	
	//outer for loop cycles through all element in the two arrays
	for(int i =0; i < length; i++){
		uint8_t mask = 0x01;
	
	/* inner for loop does a bitwise operation with each element in the array
	  with a bit mask corresponding to each bit and turns on or off the light depending on the result
	*/
		for (int j = 0; j<4; j++){
			if (light[i] & mask){
				led_state(j,1);
				}else{
				led_state(j,0);
			}
			mask = mask*2;
		}
		_delay_ms(duration[i]);
	}
	
}


void glow(uint8_t LED, float brightness) {
	
	/*infinite loop checks if the count is less than PWM_PERIOD multiplied by the duty cycle
	  and if its less than PWM_PERIOD in that order
	  if count is less than PWM_PERIOD*duty cycle turn on light
	  if count is less than PWM_PERIOD turn off light
	  if count is greater than PWM_PERIOD reset count
	*/
	while(1){
		if(count < brightness*PWM_PERIOD){
			led_state(LED,1);
		} else if (count < PWM_PERIOD){
			led_state(LED,0);
		} else {
			count = 0;
			led_state(LED,1);
		}
	}

}

void pulse_glow(uint8_t LED) {

	/*infinite loop checks if the count is less than slow_count divided by a factor of 10
	  Glow up logic is identical to the one in glow() except using count_slow*0.1 as the threshold
	  Glow down logic is identical to the Glow up part except instead of the comparing to count_slow as it increases to PWM_PERIOD we compare to (PWM_PERIOD-count_slow*0.1)
	*/
	while(1){
		count =0;
		slow_count = 0;
		while(slow_count*0.1 < PWM_PERIOD){
			if(count < slow_count*0.1){
				led_state(LED,1);
				} else if (count < PWM_PERIOD){
				led_state(LED,0);
				} else {
				count = 0;
				led_state(LED,1);
			}
		}
		count =0;
		slow_count = 0;

		while(slow_count*0.1 < PWM_PERIOD){
			if(count < PWM_PERIOD - slow_count*0.1){
				led_state(LED,1);
				} else if (count < PWM_PERIOD){
				led_state(LED,0);
				} else {
				count = 0;
				led_state(LED,1);
			}
		}
		
	}
}


void light_show() {

// code is identical to SOS() except with different arrays	 
uint8_t light[] = {
		 0xf, 0, 0xf, 0, 0xf, 0,
		 0x6, 0, 0x9, 0, 0xf, 0,
		 0xf, 0, 0xf, 0, 0x9, 0,
		 0x6, 0, 0x8, 0xC, 0x6,
		 0x3, 0x1, 0x3, 0x6, 0xC,
		 0x8, 0xC, 0x6, 0x3, 0x1,
		 0x3, 0x6,0, 0xf, 0, 0xf,
		 0, 0x6, 0, 0x6, 0
	 };

	 int duration[] = {
		 250, 100, 250, 100, 250, 100,
		 100, 100, 100, 100, 250, 100,
		 250, 100, 250, 100, 100, 100,
		 100, 100, 100, 100, 100,
		 100, 100, 100, 100, 100,
		 100, 100, 100, 100, 100,
		 100, 100, 100, 250, 100, 250,
		 100, 250, 100, 250, 100
	 };


	 int length = 44;
	 

	 
	for(int i =0; i < length; i++){
		 uint8_t mask = 0x01;
		 for (int j = 0; j<4; j++){
			 if (light[i] & mask){
				 led_state(j,1);
				 }else{
				 led_state(j,0);
			 }
			 mask = mask*2;
		 }
		 _delay_ms(duration[i]);
	 }
	
}



/* ***************************************************
 * **** END OF FIRST "STUDENT CODE" SECTION **********
 * ***************************************************
 */


/* =============================================
 * ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
 * =============================================
 */

int main() {
    /* Turn off global interrupts while setting up timers. */

	cli();

	/* Set up timer 1, i.e., an interrupt every 1 microsecond. */
	OCR1A = TOP1;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);
    /* Next two lines provide a prescaler value of 8. */
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);

	/* Set up timer 3, i.e., an interrupt every 10 milliseconds. */
	OCR3A = TOP3;
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3B |= (1 << WGM32);
    /* Next line provides a prescaler value of 64. */
	TCCR3B |= (1 << CS31);
	TIMSK3 |= (1 << OCIE3A);


	/* Turn on global interrupts */
	sei();

/* =======================================
 * ==== END OF "DO NOT TOUCH" SECTION ====
 * =======================================
 */


/* *********************************************
 * **** BEGINNING OF "STUDENT CODE" SECTION ****
 * *********************************************
 */

 //This code could be used to test your work for part A.
/*
	led_state(0, 1);
	_delay_ms(1000);
	led_state(2, 1);
	_delay_ms(1000);
	led_state(1, 1);
	_delay_ms(1000);
	led_state(2, 0);
	_delay_ms(1000);
	led_state(0, 0);
	_delay_ms(1000);
	led_state(1, 0);
	_delay_ms(1000);

// This code could be used to test your work for part B.

	//SOS();
*/

//This code could be used to test your work for part C.

	//glow(2, 0.01);




// This code could be used to test your work for part D.

//pulse_glow(3);



// This code could be used to test your work for the bonus part.

	light_show();
 

/* ****************************************************
 * **** END OF SECOND "STUDENT CODE" SECTION **********
 * ****************************************************
 */
}
