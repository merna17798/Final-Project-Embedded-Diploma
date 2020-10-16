/*
 * main.c
 *
 *  Created on: 16 Oct 2020
 *      Author: pc
 */
#include "external_eeprom.h"
#include "uart.h"
#define NUMBER_OF_OVERFLOWS_PER_HALF_MINUTE 916

/* global variable contain the ticks count of the timer */
unsigned char g_tick = 0;

ISR(TIMER0_OVF_vect)
{
	g_tick++;
	if(g_tick == NUMBER_OF_OVERFLOWS_PER_HALF_MINUTE)
	{
		PORTC = PORTC ^ (1<<PC0); //toggle led every 0.5 second
		g_tick = 0; //clear the tick counter again to count a new 0.5 second
	}
}

/* Description :
 * For clock=8Mhz and prescale F_CPU/1024 every count will take 1ms
 * so put initial timer counter = 0  0 --> 255 (256ms per overflow)
 * so we need timer to overflow 2 times to get a 0.5 second
 */
void timer0_init_normal_mode(void)
{
	TCNT0 = 0; //Set Timer initial value to 0
	TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
}
void timer0_stop(void){
	TCNT0 = 0; //Set Timer initial value to 0
}

enum READY{
	READY1=10,
	READY2=20,
	READY3=30,
	READY4=40,
	READY5=50,
	READY6=60
};
static void Recive_Write(enum READY R){
	uint8 i,x[7];
	uint8 recieve=UART_recieveByte();
	while(recieve!=R){};
	for(i=0;i<5;i++){
		UART_sendByte(R);
		x[i]=UART_recieveByte();
		EEPROM_writeByte((0x000|i),x[i]);
		_delay_ms(100);
	}
}

int main (void){
	UART_init();
	EEPROM_init();
	uint8 i,x[7];
	uint8 recieve=UART_recieveByte();
	if(recieve==READY1){
		while(recieve!=READY1){};
		for(i=0;i<5;i++){
			UART_sendByte(READY1);
			x[i]=UART_recieveByte();
			EEPROM_writeByte((0x000|i),x[i]);
			_delay_ms(100);
		}
	}
	else if (recieve==READY4){
		_delay_ms(100);
		SET_BIT(DDRA,0);
		SET_BIT(PORTA,0);
		_delay_ms(90000);
		CLEAR_BIT(PORTA,0);
		Recive_Write(READY1);
	}

	while(1){
		uint8 RECIEVE=UART_recieveByte();
		if(RECIEVE==READY2){
			while(RECIEVE!=READY2){};
			for(i=0;i<5;i++){
				UART_sendByte(READY2);
				EEPROM_readByte((0x000|i),&x[i]);
				UART_sendByte(x[i]);
				_delay_ms(100);
			}
			Recive_Write(READY3);
			Recive_Write(READY1);
		}
		else if(RECIEVE==READY5){
			/* configure pin PC0 and PC1 as output pins */
			DDRB |= 0x03;
			_delay_ms(1000);
			// Rotate the motor --> clock wise
			PORTB |= (1<<PB1);
			PORTB &= (~(1<<PB0));
			_delay_ms(40000);

			// Rotate the motor --> anti-clock wise
			PORTB |= (1<<PB0);
			PORTB &= (~(1<<PB1));
			_delay_ms(40000);
			PORTB=0XFC;
			}
			PORTB=0XFC;
	}
}
