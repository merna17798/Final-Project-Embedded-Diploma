/*
 * timer.h
 *
 *  Created on: Oct 9, 2020
 *      Author: user
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

#define COMPARE_MODE

#ifdef COMPARE_MODE
	#define compare_mode_bit 1     // bit (WGM12) in TCCR1B register (timer mode)
	#define compare_interrupt 1   // bit (OCIE1A) in TIMSK register (module interrupt enable)
	#define normal_interrupt 0   // bit (TOIE1) in TIMSK register (module interrupt enable)
#endif

#ifdef NORMAL_MODE
	#define compare_mode_bit 0     // bit (WGM12) in TCCR1B register (timer mode)
	#define compare_interrupt 0   // bit (OCIE1A) in TIMSK register (module interrupt enable)
	#define normal_interrupt 1 	 // bit (TOIE1) in TIMSK register (module interrupt enable)
#endif


typedef enum
{
	NO_CLK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}timer1_Clock;


typedef struct
{
	timer1_Clock clock;
	uint16 initial_value;
	uint16 compare_value;

}Timer1_config;

void Timer1_init(const Timer1_config * config_Ptr);
void Timer1_set_CallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
