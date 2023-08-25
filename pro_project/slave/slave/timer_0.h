/*
 * timer_0.h
 *
 * Created: 7/5/2023 8:01:53 PM
 *  Author: abdelrhman mattar
 */ 


#ifndef TIMER_0_H_
#define TIMER_0_H_
typedef enum {
	Normal_Mode=0,
	PWM,
	CTC_Mode,
	Fast_PWM,
}
TIMER0_MODES;


typedef enum {
	STOP=0,
	PRESCALAR_1,
	PRESCALAR_8,
	PRESCALAR_64,
	PRESCALAR_256,
	PRESCALAR_1024,
	EXTERNAL_FALLING_EDGE,
	EXTERNAL_RISINGING_EDGE,
}
TIMER0_PRESCALAR;


typedef enum{
	Dis_Connect=0,
	Togle=1,
	Clear=2,
	Set=3,
}
OC_MODE;

void Timer0_overflow_enable();
void Timer0_overflow_disable();
void Timer0_Vinit(TIMER0_MODES t_mode , TIMER0_PRESCALAR scale ,OC_MODE mode, uint8_t timer_data);
void Timer0_OC_enable();
void Timer0_OC_disable();










#endif /* TIMER_0_H_ */