/*
 * slave.c
 *
 * Created: 8/21/2023 2:47:45 PM
 * Author : abdelrhman mattar
 */ 

#include "uart_messages.h"
bool condition_on=false;
uint8_t temp=25;

int main(void)
{
	temp=EEPROM_read(0x100);
	DDRB=0XFF;
	DDRD|=0B00111100;
	
	ADC_vinit(0);
	UART_Vinit(9600);
	servo_int();
	
	char x;	
	
	void (*ptr[10])(uint8_t)={set_temp,con_on,con_off,door_on,door_off,gar_on,gar_off,tv_on,tv_off,set_room};

    /* Replace with your application code */
    while (1) 
    {
		if (condition_on && (ADC_u16Read()>>1)>temp)
			SET_BIT(PORTD,3);
		else
			CLR_BIT(PORTD,3);

		x=UART_RECIEVE8();
		if (x>0 && x<10)
			ptr[x-1](x);
		else if (x>=10 && x<=25)
			ptr[9](x);
		else{}
    }
}