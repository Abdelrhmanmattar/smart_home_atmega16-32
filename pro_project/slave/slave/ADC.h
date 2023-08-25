/*
 * ADC.h
 *
 * Created: 3/31/2023 12:44:55 AM
 *  Author: abdelrhman mattar
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <stdio.h>
void ADC_vinit(int8_t mux);
unsigned short ADC_u16Read(void);
#endif /* ADC_H_ */