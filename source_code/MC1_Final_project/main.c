/*
 * main.c
 *
 *  Created on: 11 Oct 2020
 *      Author: pc
 */
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
enum READY{
	READY1=10,
	READY2=20,
	READY3=30,
	READY4=40
};

static void get_pass(char arr[50],uint8 sent_pass[7]){
	uint8 key, i,j;
	LCD_sendCommand(CLEAR_COMMAND);
	LCD_displayStringRowColumn(0,0,arr);
	LCD_goToRowColumn(1,0);
	/* if any switch pressed for more than 500 ms it counts more than one press */
	for (i=0;i<5;i++){
		key = KeyPad_getPressedKey(); /* get the pressed key number */
		_delay_ms(2000); /* Press time  */
		LCD_displayCharacter('*'); /* display the pressed keypad switch */
		sent_pass[i]=key;
	}
	key = KeyPad_getPressedKey(); /* get the pressed key number */
	_delay_ms(2000); /* Press time  */
	while (key!=13){
		LCD_sendCommand(CLEAR_COMMAND);
    	LCD_displayStringRowColumn(0,0,"Err, press on/c");
    	key = KeyPad_getPressedKey(); /* get the pressed key number */
    	_delay_ms(2000); /* Press time  */
    	j++;

	}
	if (key==13){
		LCD_sendCommand(CLEAR_COMMAND);
	}
}
static uint8 compare(uint8 new_pass[7], uint8 match_pass[7] ){
	uint8 i,j=0;
	for (i=0;i<5;i++){

		if (match_pass[i]==new_pass[i]){
			j++;
		}

	}

	if(j==5){
		LCD_displayStringRowColumn(0,0,"matched!");
	}
	else{
		LCD_displayStringRowColumn(0,0,"Not matched!");
	}

	return j;
}
static void F_compare(uint8 new_pass[7], uint8 match_pass[7],char arr[50],enum READY R){
	uint8 i,j=0,z=1;
	j=compare(new_pass,match_pass);

	while(j!=5){
    	get_pass(arr,match_pass);
    	j=compare(new_pass,match_pass);
    	z++;
		if(z==3){
			//UART_sendByte(READY4);
			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"Stranger!");
			_delay_ms(10000);
		}

	}
	if (j==5) {

    	UART_sendByte(R);
    	for(i=0;i<5;i++){
    		while(UART_recieveByte()!=R){};
    		UART_sendByte(new_pass[i]);

    	}
	}

}
int main(void)
{
	uint8 i,key
	,new_pass[7],match_pass[7],old_pass[7],match_pass2[7];
	LCD_init();
	UART_init();
	get_pass("Enter your PW:", new_pass);
	_delay_ms(10);
	get_pass("reEnter your PW:",match_pass);
	F_compare(new_pass,match_pass,"reEnter your PW:",READY1);


    while(1)
    {

		LCD_displayStringRowColumn(0,0,"- open");
		LCD_displayStringRowColumn(1,0,"+ New password");
		_delay_ms(1000);
		key = KeyPad_getPressedKey(); /* get the pressed key number */
		_delay_ms(2000); /* Press time  */
		if (key =='+'){
			get_pass("Enter old PW:", old_pass);
			UART_sendByte(READY2);
			for(i=0;i<5;i++){
				while(UART_recieveByte()!=READY2){};
				match_pass2[i]=UART_recieveByte();
				_delay_ms(100);
			}
			//Check_Stranger(match_pass2,old_pass,"reEnter old PW:");
			F_compare(match_pass2,old_pass,"reEnter old PW:",READY3);
			get_pass("Enter new PW:", new_pass);
			_delay_ms(10);
			get_pass("reEnter new PW:",match_pass);
			F_compare(new_pass,match_pass,"reEnter new PW:",READY1);
		}
		LCD_displayStringRowColumn(0,0,"Choose option");
		_delay_ms(1000);
    }

}
