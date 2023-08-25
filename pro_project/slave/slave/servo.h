/*
 * servo.h
 *
 * Created: 7/15/2023 6:26:45 PM
 *  Author: abdelrhman mattar
 */ 


#ifndef SERVO_H_
#define SERVO_H_
#include "std_macros.h"
#include <avr/io.h>
void servo_int()
{
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1B,WGM13);
	CLR_BIT(TCCR1B,CS11);
	
	OCR1A=500;
	OCR1B=500;
	ICR1=19999;
	SET_BIT(TCCR1A,WGM11);
	SET_BIT(TCCR1A,COM1A1);
	SET_BIT(TCCR1A,COM1B1);
}

void servo_speed_OCR1A(double i)
{
	OCR1A=500.0 + (i*10.555);
	SET_BIT(TCCR1B,CS11);
}

void servo_speed_OCR1B(double i)
{
	OCR1B=500.0 + (i*11);
	SET_BIT(TCCR1B,CS11);
}

void change(int x)
{
	ICR1=x;
}




#endif /* SERVO_H_ */