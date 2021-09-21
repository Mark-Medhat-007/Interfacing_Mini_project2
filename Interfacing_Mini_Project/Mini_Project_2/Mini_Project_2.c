/*
 * Mini_Project_2.c
 *
 *  Created on: Sep 20, 2021
 *      Author: marcm
 */



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


/**************************Global Variables************************************/
unsigned char sec_counter1=0;
unsigned char sec_counter2=0;
unsigned char min_counter1=0;
unsigned char min_counter2=0;
unsigned char hr_counter1=0;
unsigned char hr_counter2=0;



/**************************Timer 1 Initiation function*************************/
void Timer1_ctc_Init(void)
{
	TCNT1 = 0;
	OCR1A = 1000;                           // putting compare value in register as 1clock =1ms
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);  //CS10 and CS12 to selecting prescaler 1024
	TCCR1A=(1<<FOC1A)|(1<<FOC1B);
	TIMSK=(1<<OCIE1A);			         //enabling interrupt timer module
}


/**************************Timer 1 ISR*************************/
ISR(TIMER1_COMPA_vect){
	sec_counter1++;
	if(sec_counter1==10){
		sec_counter1=0;
		sec_counter2++;
	}
	if (sec_counter2==6){
		sec_counter2=0;
		min_counter1++;
	}
	if (min_counter1==10){
			min_counter1=0;
			min_counter2++;
		}
	if (min_counter2==6){
				min_counter2=0;
				hr_counter1++;
		}

	if (hr_counter1==10){
					hr_counter1=0;
					hr_counter2++;
			}
	if (hr_counter2==10){
						sec_counter1=0;
						sec_counter2=0;
						min_counter1=0;
						min_counter2=0;
						hr_counter1=0;
						hr_counter2=0;

				}
}


/**************************Interrupt 1 ISR************************************/
ISR(INT1_vect){
	TCCR1B&=~(1<<CS10);
	TCCR1B&=~(1<<CS12);
}



/**************************Interrupt 0 ISR************************************/
ISR(INT0_vect){
	sec_counter1=0;
	sec_counter2=0;
	min_counter1=0;
	min_counter2=0;
	hr_counter1=0;
	hr_counter2=0;
	TCNT1 = 0;
}


/**************************Interrupt 2 ISR************************************/
ISR(INT2_vect){
	TCCR1B |=(1<<CS10)|(1<<CS12);
}


/**************************7-segments display function *********************************/
void display(void){
	PORTA=1;
	PORTC = (PORTC & 0xF0) | (sec_counter1 & 0x0F);
	_delay_ms(5);

	PORTA=2;
	PORTC = (PORTC & 0xF0) | (sec_counter2 & 0x0F);
	_delay_ms(5);

	PORTA=4;
	PORTC = (PORTC & 0xF0) | (min_counter1 & 0x0F);
	_delay_ms(5);

	PORTA=8;
	PORTC = (PORTC & 0xF0) | (min_counter2 & 0x0F);
	_delay_ms(5);

	PORTA=16;
	PORTC = (PORTC & 0xF0) | (hr_counter1 & 0x0F);
	_delay_ms(5);

	PORTA=32;
	PORTC = (PORTC & 0xF0) | (hr_counter2 & 0x0F);
	_delay_ms(5);

}


/**************************Interrupt 1 Initiation function*********************/
void INT1_init(void){
	DDRD &=~(1<<3);
	GICR  |= (1<<INT1);
	MCUCR |= (1<<ISC11)|(1<<ISC10);
}


/**************************Interrupt 0 Initiation function*********************/
void INT0_init(void){
	DDRD &=~(1<<2);
	GICR  |= (1<<INT0);
	MCUCR |= (1<<ISC01);
	PORTD|=(1<<2);
}


/**************************Interrupt 2 Initiation function*********************/
void INT2_init(void){
	DDRB &=~(1<<2);
	GICR  |= (1<<INT2);
	MCUCSR &= ~(1<<ISC2);
}



/**************************MAIN************************************************/
int main(void){
	SREG=(1<<7);			//enable I-bit
	DDRC=0x0F;				//making first 4 pins in in Port c  OUTput pins
	PORTC=0;				//initiating pins with LOW
	DDRA=0;					//Making port A input pins
	PORTA=0x3F;				// Able the internal PULL UP
	INT1_init();
	INT0_init();
	INT2_init();
	Timer1_ctc_Init();
	while(1){
		display();
	}
}

