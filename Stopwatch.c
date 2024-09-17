/*
 * Stopwatch.c
 *
 *  Created on: Sep 10, 2024
 *      Author: Eng Omar Badwy
 */
/**********************************************
 *                                            *
 * Project ----> Stopwatch with Dual Mode .   *
 * By Omar Mohamed Suliman Mohamed.           *
 *                                            *
 *********************************************/
/* Include Libraries */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Define Push Button for increasing and decreasing in case of Hours or Minutes or Seconds. */
volatile uint8_t BUTTON_INC_HR; /* Connected to PB1 with internal pull up resistor */
volatile uint8_t BUTTON_DEC_HR; /*  Connected to PB0 with internal pull up resistor */
volatile uint8_t BUTTON_INC_MIN;  /* Connected to PB4 with internal pull up resistor */
volatile uint8_t BUTTON_DEC_MIN; /* Connected to PB3 with internal pull up resistor */
volatile uint8_t BUTTON_INC_SEC;  /* Connected to PB6 with internal pull up resistor */
volatile uint8_t BUTTON_DEC_SEC;  /* Connected to PB5 with internal pull up resistor */

/* Define hours, minutes, seconds and flag. */
volatile uint8_t Flag = 0;
volatile uint8_t SECONDS = 0;
volatile uint8_t MINUTES = 0;
volatile uint8_t HOURS = 0;
/*****************************************
 * Functions Prototyping / declarations. *
 *****************************************/
void Timer1_CTC_Init(void);
void INT0_Init_FallingEdge(void);
void INT1_Init_RisingEdge(void);
void INT2_Init_FallingEdge(void);
/********************************
 * Interrupt Service Routines.  *
 ********************************/
ISR(TIMER1_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(INT2_vect);
/******************
 * Main Function  *
 ******************/
int main(void){
	unsigned char c_ButtonState;
	unsigned char pre_ButtonState = 1;
	DDRB &= ~(1 << PB0) & ~(1 << PB1) & ~(1 << PB3) & ~(1 << PB2) & ~(1 << PB5) & ~(1 << PB6); /* make the PIN 0,1,3,4,5,6 in port B as input*/
	PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4) | (1 << PB5)| (1 << PB6); /*Enable internal pull up resistor in pin 0,1,3,4,5,6*/
	PORTD |= (1 << PD4); /*At the begging the Grean led is on because the start is up counter*/
	DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD0); /*make PD4 and PD5 and PD0 output pins*/
	/*make PB7 input pin*/
	DDRB &= ~(1 << PB7);
	PORTB |= (1 << PB7);
	DDRC |= 0x0F; /*make first 4-pins in PORTC as output for 7447 decoder*/
	PORTC &= 0xF0;/* put 0 value as initial in first 4-pins in PORTC */
	DDRA |= 0x3F; /* define first 6-pins in PORTA as output as the enable/disable pins for the six 7-segments */
	PORTA &= 0xC0; /* put 0 value as initial in first 6-pins in PORTA */
	/* Calling Functions */
	Timer1_CTC_Init();
	INT0_Init_FallingEdge();
	INT1_Init_RisingEdge();
	INT2_Init_FallingEdge();
	while (1){
			c_ButtonState = PINB & (1 << PB7);
			/* Check if the button state has changed from not pressed to pressed */
			if (c_ButtonState == 0 && pre_ButtonState == (1 << PB7)){
				Flag ^= 1; // Toggle the Flag
				PORTD ^= (1 << PD4);
				PORTD ^= (1 << PD5);
				_delay_ms(5);
			}
			// Update the previous button state
			pre_ButtonState = c_ButtonState;
			/*Check the decrement button for seconds*/
			if(!(PINB & (1 << PB5))) {
				if(BUTTON_INC_SEC == 0 && SECONDS > 0) {
					SECONDS--;
					BUTTON_DEC_SEC = 1;
				}
			}else{
				BUTTON_DEC_SEC = 0;
			}
			/*Check the increment button for seconds*/
			if(!(PINB & (1 << PB6))) {
				if (BUTTON_INC_SEC == 0) {
					SECONDS++;
					BUTTON_INC_SEC = 1;
				}
			}else{
				BUTTON_INC_SEC = 0;
			}
			/*Check the decrement button for minutes*/
			if (!(PINB & (1 << PB3))) {
				if (BUTTON_DEC_MIN == 0 && MINUTES > 0) {
					MINUTES--;
					BUTTON_DEC_MIN = 1;
				}
			}else{
				BUTTON_DEC_MIN = 0;
			}
			/*Check the increment button for Minutes*/
			if(!(PINB & (1 << PB4))){
				if(BUTTON_INC_MIN == 0){
					MINUTES++;
					BUTTON_INC_MIN = 1;
				}
			}else{
				BUTTON_INC_MIN = 0;
			}
			/*Check the decrement button for hours*/
			if(!(PINB & (1 << PB0))) {
				if(BUTTON_DEC_HR == 0 && HOURS > 0) {
					HOURS--;
					BUTTON_DEC_HR = 1;
				}
			}else{
				BUTTON_DEC_HR = 0;
			}
			/*Check the increment button for hours*/
			if (!(PINB & (1 << PB1))) {
				if (BUTTON_INC_HR == 0) {
					HOURS++;
					BUTTON_INC_HR = 1;
				}
			}else{
				BUTTON_INC_HR = 0;
			}
			/*First 7-Segment*/
			PORTA = 0x20; /* enable first 7-segment for first digit in seconds */
			PORTC = (PORTC & 0xF0) | ((SECONDS % 10) & 0x0F);
			_delay_ms(2);

			/*Second 7-segment*/
			/*enable second 7-segment for second digit in seconds*/
			PORTA = 0x10; /*enable second 7-segment for second digit in seconds*/
			PORTC = (PORTC & 0xF0) | ((SECONDS / 10) & 0x0F);
			_delay_ms(2);

			/*Third 7-Segment*/
			PORTA = 0x08; /*enable third 7-segment for first digit in minutes*/
			PORTC = (PORTC & 0xF0) | ((MINUTES % 10) & 0x0F);
			_delay_ms(2);

			/*Fourth 7-segment*/
			PORTA = 0x04; /*enable fourth 7-segment for second digit in minutes*/
			PORTC = (PORTC & 0xF0) | ((MINUTES / 10) & 0x0F);
			_delay_ms(2);

			/*Fifth 7-segment*/
			PORTA = 0x02; /*enable Fifth 7-segment for first digit in hours*/
			PORTC = (PORTC & 0xF0) | ((HOURS % 10) & 0x0F);
			_delay_ms(2);

			/*Sixth 7-segment*/
			PORTA = 0x01; /*enable sixth 7-segment for second digit in hours*/
			PORTC = (PORTC & 0xF0) | ((HOURS / 10) & 0x0F);
			_delay_ms(2);
		}
}
/******************************
 *  Functions Implementation  *
 ******************************/
/* Timer1 Compare Match A ISR to update the time */
ISR(TIMER1_COMPA_vect) {
	if(!Flag){
		PORTD |= (1 << PD4); /*Grean LED for count up is on*/
		PORTD &= ~(1 << PD5); /*Yellow LED for down count is off*/
		PORTD &= ~(1 << PD0); /* Buzzer off */
		SECONDS++;
		if(SECONDS == 60){
			MINUTES++;
			SECONDS = 0;
		}
		if(MINUTES == 60){
			HOURS++;
			MINUTES = 0;
		}
		if(HOURS == 100){
			HOURS = 0;
			MINUTES = 0;
			SECONDS = 0;
		}
	}else{
		PORTD |= (1 << PD5); /* turn on the count down led */
		PORTD &= ~(1 << PD4); /* turn off the count up led */
		if (SECONDS == 0){
			if (MINUTES == 0){
				if (HOURS == 0){
					PORTD |= (1 << PD0); /* If everything is zero, buzzer will turn on */
				}else{
					HOURS--;
					MINUTES = 59;
					SECONDS = 59;
				}
			}else{
				MINUTES--;
				SECONDS = 59;
			}
		}else{
			SECONDS--;
		}
	}
}
/* Initialize Timer1 in CTC mode */
void Timer1_CTC_Init(void){
	/* F_CPU=16 MHz , prescaler=1024
	 * timer_frequency=15625 , T(timer)=64 micro sec
	 * the compare match value for 1sec is 15625
	 */
	TCNT1 = 0;  /* Set Timer1 Initial count to 0 */
	OCR1A = 15624; /* Set compare value for 1 second interval (Assume Prescaler = 1024) */
	TCCR1A = 0;  /* Clear TCCR1A */
	TCCR1A = (1 << FOC1A); /*Force Output Compare for Compare unit A*/
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);  /* Set CTC mode and 1024 Prescaler */
	TIMSK |= (1 << OCIE1A); /* Enable Timer1 Compare A Interrupt */
	SREG |= (1 << 7); /* Enable global interrupts */
}
/* External Interrupt INT0 ISR to reset the stopwatch */
ISR(INT0_vect){
	/*Reset the timer*/
	TCNT1 = 0;
	SECONDS = 0;
	MINUTES = 0;
	HOURS = 0;
}
/* Initialize INT0 for Falling Edge detection */
void INT0_Init_FallingEdge(void){
	PORTD |= (1 << PD2);     /* Enable pull-up resistor for PD2 */
	MCUCR |= (1 << ISC01);   /* Set Falling Edge detection for INT0 */
	MCUCR &= ~(1 << ISC00);
	GICR |= (1 << INT0);     /* Enable INT0 */
}
/* External Interrupt INT1 ISR to reset the stopwatch */
ISR(INT1_vect){
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);/* Stop Timer1 */
}
/* Initialize INT1 for Rising Edge detection */
void INT1_Init_RisingEdge(void){
	TCNT1 = 0; /* Set Timer1 Initial count to zero */
	MCUCR |= (1 << ISC10) | (1 << ISC11); /* Set Rising Edge detection for INT1 */
	GICR |= (1 << INT1);  /* Enable INT1 */
}
/* External Interrupt INT2 ISR to reset the stopwatch */
ISR(INT2_vect){
	TCNT1 = 0; /* Reset Timer1 counter */
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); /* Restart Timer1 */
	PORTD &= ~(1<<PD0); /* Turn off buzzer */
}
/* Initialize INT2 for Falling Edge detection */
void INT2_Init_FallingEdge(void){
	MCUCSR &= ~(1 << ISC2); /* Set Falling Edge detection for INT2 */
	GICR |= (1 << INT2);    /* Enable INT2 */
}
/************************************************ "Thank You" ************************************************/
