/*
 * main.c
 *
 *  Created on: 16 Oct 2020
 *      Author: pc
 */
#include "external_eeprom.h"
#include "uart.h"
#include "timer0.h"

/*
 *this enum have the orders that will be sent to the other Micro-controller
 */

enum READY{
	READY1=10,
	READY2=20,
	READY3=30,
	READY4=40,
	READY5=50,
	READY6=60, READY7=70
};

/*
 * Number of overflows per second as the used mode is the normal mode
 * */

#define NUMBER_OF_OVERFLOWS_PER_SECOND 32

/*volatile to avoid implicit casting of compiler*/

volatile uint8 g_tick = 0;
volatile uint8 second;

/* Description :
 * For clock = 8Mhz and prescaler F_CPU/1024 every count will take 128 microsecond
 * so put initial timer counter=0  0 --> 255 (32ms per overflow)
 * so we need timer to overflow 31 times to get a 1 second
 * This is the ISR of timer0 overflow mode
 */

ISR(TIMER0_OVF_vect)
{
	g_tick++;
	if (g_tick == NUMBER_OF_OVERFLOWS_PER_SECOND)
	{
		second++;
		g_tick = 0; //clear the tick counter again to count a new 0.5 second
	}
}

/*
 * Motor Rotates clockwise
 */

void motor_open(void){
	/* configure pin PC0 and PC1 as output pins */
	DDRB |= 0x03;
	_delay_ms(1000);
	// Rotate the motor --> clock wise
	PORTB |= (1<<PB1);
	PORTB &= (~(1<<PB0));
}

/*
 * Motor rotates anti-clockwise
 */

void motor_close(void){
	/* configure pin PC0 and PC1 as output pins */
	DDRB |= 0x03;
	_delay_ms(1000);
	// Rotate the motor --> anti-clock wise
	PORTB |= (1<<PB0);
	PORTB &= (~(1<<PB1));
}

/*
 * This Function recieve specific flag to store the sent array in E2PROM memory
 */

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


/*
 * START OF MAIN FUNCTION
 */

int main (void){

	/*
	 * initialise UART DRIVER
	 */

	UART_init();

	/*
	 * initialise EEPROM DRIVER
	 */

	EEPROM_init();

	/*
	 * Variable Declaration
	 */

	uint8 i,x[7];

	Recive_Write(READY1);

	while(1){

		/*
		 * Variable Declaration
		 */

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


		else if(RECIEVE==READY6){
			while(RECIEVE!=READY6){};
			for(i=0;i<5;i++){
				UART_sendByte(READY6);
				EEPROM_readByte((0x000|i),&x[i]);
				UART_sendByte(x[i]);
				_delay_ms(100);
			}

			uint8 recieve=UART_recieveByte();

			/*
			 * ALARM SETED FOR FOUR SECONDS
			 */

			if (recieve==READY4){
				_delay_ms(100);
				SET_BIT(DDRA,0);
				SET_BIT(PORTA,0);
				timer0_init_normal_mode(T0_F_CPU_1024, 0);
				while (second != 4)
					;
				second = 0;
				CLEAR_BIT(PORTA,0);

			}

			else if (recieve==READY7){
				for(i=0;i<5;i++){
					UART_sendByte(READY7);
					x[i]=UART_recieveByte();
					EEPROM_writeByte((0x000|i),x[i]);
					_delay_ms(100);
				}
			motor_open();
			timer0_init_normal_mode(T0_F_CPU_1024, 0);
			while (second != 2)
				;
			second=0;

			motor_close();
			while (second != 2)
				;
			second = 0;
			PORTB=0XFC;
			timer0_deinit();
			}

	}
}
}
