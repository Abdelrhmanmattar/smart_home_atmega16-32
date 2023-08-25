/*
 * uart_messages.h
 *
 * Created: 8/24/2023 2:53:19 PM
 *  Author: abdelrhman mattar
 */ 


#ifndef UART_MESSAGES_H_
#define UART_MESSAGES_H_

#include <avr/io.h>
#include <stdbool.h>
#define F_CPU 8000000ul
#include <util/delay.h>
#include "UART.h"
#include "servo.h"
#include "ADC.h"
#include "data_config.h"
#include "EEPROM_driver.h"
#include "glo.h"


void set_temp(uint8_t te)
{
	char x=UART_RECIEVE8();
	temp=(uint8_t)x;
	EEPROM_write(0x100,temp);
	_delay_ms(200);
}
void con_on(uint8_t t)
{
	condition_on=true;
}
void con_off(uint8_t t )
{
	condition_on=false;
	CLR_BIT(PORTD,3);
}

void door_on(uint8_t t)
{
	servo_speed_OCR1A(90);
}
void door_off(uint8_t t)
{
	servo_speed_OCR1A(0);
}

void gar_on(uint8_t t)
{
	servo_speed_OCR1B(90);
}
void gar_off(uint8_t t)
{
	servo_speed_OCR1B(0);
}


void tv_on(uint8_t t)
{
	SET_BIT(PORTD,2);
}
void tv_off(uint8_t t)
{
	CLR_BIT(PORTD,2);
}


void set_room(uint8_t room_num)
{
	if (room_num < 18)
	{
		room_num-=10;
		SET_BIT(PORTB,room_num);
	}
	else if (room_num < 26)
	{
		room_num-=18;
		CLR_BIT(PORTB,room_num);
	}
	else
	{
	}
}



#endif /* UART_MESSAGES_H_ */