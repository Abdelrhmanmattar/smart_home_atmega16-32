/*
 * UART.c
 *
 * Created: 6/30/2023 3:52:48 AM
 *  Author: abdelrhman mattar
 */ 
#define  F_CPU 8000000UL
#include <avr/io.h>
#include "UART.h"
#include "std_macros.h"
#include <util/delay.h>

void UART_Vinit(unsigned int baudrate)
{
	//SET VALUE OF UBRR REGISTER
	short int data=(F_CPU>>4)/baudrate;
	data--;
	UBRRH = (unsigned char)(data >> 8);
	UBRRL = (unsigned char) data;
	
	
	SET_BIT(UCSRB ,RXEN);
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXCIE);
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
}


void UART_SEND_CHAR(char DATA)
{
	uint8_t wait = 250;
	while(READ_BIT(UCSRA,UDRE) == 0)
	{
		if (wait<=1)
		{
			return;
		}
		wait--;
	}
	UDR = DATA;
}



void UART_SEND_STRING(char *DATA)
{
	while(*DATA != '\0')
	{
		UART_SEND_CHAR(*DATA);
		DATA++;
		_delay_ms(50);
	}
}


char UART_RECIEVE8()
{	
	uint8_t wait=250;
	while(UDR==0xff);
	while( READ_BIT(UCSRA ,RXC) ==0)
	{
		if (wait <=1)return 0;
		wait--;
	}
	return (char)UDR;
}


