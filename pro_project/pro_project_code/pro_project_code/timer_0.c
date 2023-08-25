/*
 * timer_0.c
 *
 * Created: 7/5/2023 8:02:08 PM
 *  Author: abdelrhman mattar
 */ 
#include <avr/io.h>
#include "timer_0.h"
#include "std_macros.h"


void Timer0_overflow_enable()
{
	SET_BIT(TIMSK,TOIE0);
}
void Timer0_overflow_disable()
{
	CLR_BIT(TIMSK,TOIE0);
}


void Timer0_OC_enable()
{
	SET_BIT(TIMSK,OCIE0);
}
void Timer0_OC_disable()
{
	CLR_BIT(TIMSK,OCIE0);
}

void Timer0_Vinit(TIMER0_MODES t_mode , TIMER0_PRESCALAR scale, OC_MODE mode ,uint8_t timer_data)
{
	//TIMER MODE
	if(t_mode == Normal_Mode)TCNT0 = timer_data;
	else OCR0=timer_data;
		
	switch (t_mode)
	{
		case Normal_Mode:
		CLR_BIT(TCCR0 , WGM00);
		CLR_BIT(TCCR0 , WGM01);
		break;
		
		case PWM:
		SET_BIT(TCCR0 , WGM00);
		CLR_BIT(TCCR0 , WGM01);
		break;
		
		case  CTC_Mode:
		CLR_BIT(TCCR0 , WGM00);
		SET_BIT(TCCR0 , WGM01);
		break;
		
		case Fast_PWM:
		SET_BIT(TCCR0 , WGM00);
		SET_BIT(TCCR0 , WGM01);
		SET_BIT(DDRB,3);
		break;
	}
	
	//OC-mode
	CLR_BIT(TCCR0,COM00);
	CLR_BIT(TCCR0,COM01);
	TCCR0 |=(mode<<4);
	
	//PRESCALAR
	TCCR0 &= 0XF8;
	TCCR0|= scale;
		
}