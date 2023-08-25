/*
 * ADC.c
 *
 * Created: 3/31/2023 12:45:11 AM
 *  Author: abdelrhman mattar
 */ 

#include <avr/io.h>
#include "std_macros.h"
#include "ADC.h"
void ADC_vinit(int8_t mux)
{
	//configure VREF
	SET_BIT(ADMUX,REFS0);
	//SET_BIT(ADMUX,REFS1);
	//enable ADC
	SET_BIT(ADCSRA,ADEN);
	mux=mux | 0b11100000;
	//set mux mode
	ADMUX |= 0b00011111;
	ADMUX =ADMUX&mux;
	
	// SET Prescaler
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
}


unsigned short ADC_u16Read(void)
{
	unsigned short read_val;
	SET_BIT(ADCSRA,ADSC);
	while(READ_BIT(ADCSRA,ADSC));
	read_val=(ADCL);
	read_val|=(ADCH<<8);
	return read_val ;
}