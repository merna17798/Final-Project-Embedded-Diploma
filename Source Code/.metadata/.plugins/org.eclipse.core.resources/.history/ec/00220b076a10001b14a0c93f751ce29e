/*
 * timer.c
 *
 *  Created on: Oct 9, 2020
 *      Author: user
 */


#include "timer.h"

static volatile void(*g_callBackPTr)(void) = NULL_PTR;

ISR (TIMER1_COMPA_vect)
{
	if (g_callBackPTr != NULL_PTR)
	{
		(*g_callBackPTr)();
	}

}

void Timer1_init(const Timer1_config * config_Ptr)
{
	SREG |= (1 << 7); // I_bit

	TCCR1A |= (1 << FOC1A) | (1 << FOC1B);  // non_PWM mode

	//TCCR1B = (1 << CS10) | (1 << CS12) | (1 << WGM12);   // pre_scalar = 8 MHZ / 1028 (interrupt every .128 second)  // compare mode

	TCCR1B = (TCCR1B & 0xF8) | (config_Ptr->clock); // pre_scalar
	TCCR1B = (TCCR1B & 0xF7) | (compare_mode_bit << WGM12); // Normal or compare mode

	TCNT1 = config_Ptr->initial_value;  // initial value
	OCR1A = config_Ptr->compare_value;  // compare value

	TIMSK = (TIMSK & 0xEF) | (compare_interrupt << OCIE1A); // module interrupt enable (compare mode)
	TIMSK = (TIMSK & 0xFD) | (normal_interrupt << TOIE1); // module interrupt enable (Normal mode)
}

void Timer1_set_CallBack(void(*a_ptr)(void))
{
	g_callBackPTr = a_ptr;
}
