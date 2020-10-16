/*
 * main.c
 *
 *  Created on: 16 Oct 2020
 *      Author: pc
 */
#include "external_eeprom.h"
#include "uart.h"
enum READY{
	READY1=10,
	READY2=20,
	READY3=30,
	READY4=40
};
static void Recive_Write(enum READY R){
	uint8 i,x[7];
	while(UART_recieveByte()!=R){};
	for(i=0;i<5;i++){
		UART_sendByte(R);
		x[i]=UART_recieveByte();
		EEPROM_writeByte((0x000|i),x[i]);
		_delay_ms(100);
	}
}
static void Read_Send(enum READY R){
	uint8 i,x[7];
	while(UART_recieveByte()!=R){};
	for(i=0;i<5;i++){
		UART_sendByte(R);
		EEPROM_readByte((0x000|i),&x[i]);
		UART_sendByte(x[i]);
		_delay_ms(100);
	}
}

int main (void){
	UART_init();
	EEPROM_init();

	Recive_Write(READY1);
	while(1){


		Read_Send(READY2);
		Recive_Write(READY3);
		Recive_Write(READY1);
	}
}
