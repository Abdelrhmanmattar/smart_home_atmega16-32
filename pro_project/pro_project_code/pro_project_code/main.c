/*
* pro_project_code.c
*
* Created: 8/17/2023 3:28:42 PM
* Author : abdelrhman mattar
*/


#include "PASS.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define F_CPU 8000000ul
#include <util/delay.h>
#include <avr/interrupt.h>
#include "std_macros.h"
#include "lcd.h"
#include "keypad_driver.h"
#include "EEPROM_driver.h"
#include "timer_0.h"
#include "UART.h"

volatile unsigned long long timer_count = 0;
char x;
char arr_pass[4], GUEST_PASS[4];

void take_password(char *arr);
bool array_equal(char *arr1, uint16_t arr2, uint8_t size);
void login_sys(void);
void condition_menu(void);
void admin_menu(void);
void motor_door_garage(MOTOR M , USER user);
void rooms(USER user);
void guest_menu(void);

char *rooms_names[8]={"room","room","room","room","room","room","room","room"};

void give_rooms_names(char * arr[],uint8_t size)
{
	uint8_t i;
	uint8_t number = EEPROM_read(ADDRESS_ROOMS);
	for(i=0;i<size;i++)
	{
		if (READ_BIT(number,i)==1) 
		{
		}
		else
		{
			arr[i]=NULL;
			UART_SEND_CHAR(ROOM1_OFF+i);
		}
	}
}

int main(void)
{
	//////////////////////////////////////////////////////////////////////////
	//start
	SET_BIT(DDRA,0);
	SET_BIT(DDRA,1);
	SET_BIT(DDRA,2);
	
	CLR_BIT(PORTA,0);
	CLR_BIT(PORTA,1);
	CLR_BIT(PORTA,2);

	lcd_init();
	keypad_vInit();
	UART_Vinit(9600);

	Timer0_Vinit(Normal_Mode, PRESCALAR_1024, Dis_Connect, 0);
	sei();
	give_rooms_names(rooms_names,8);
	
	lcd_string("Home System");
	movecurse(2, 1);
	lcd_string("Starting...");
	_delay_ms(500);
	lcd_cmd(clearscreen);
	_delay_ms(10);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//if it first time or not
	unsigned char first_not = EEPROM_read(First_or_not);
	if (first_not == 0xff)
	{
		// SET ADMIN PASSWORD
		lcd_string("Set Admin");
		movecurse(2, 1);
		lcd_string("pass:");
		take_password(arr_pass);
		lcd_string("...");
		lcd_cmd(clearscreen);
		_delay_ms(10);

		// SET GUEST PASSWORD
		lcd_string("Set GUEST");
		movecurse(2, 1);
		lcd_string("pass:");
		take_password(GUEST_PASS);
		lcd_string("...");
		lcd_cmd(clearscreen);
		_delay_ms(10);

		for (uint8_t number_ = 0; number_ < pass_length; number_++)
		{
			EEPROM_write(ADDRESS_ADMIN + number_, arr_pass[number_]);
			_delay_ms(5);
			EEPROM_write(ADDRESS_GUEST + number_, GUEST_PASS[number_]);
		}
		EEPROM_write(First_or_not, 0x01);
		lcd_string("ADMIN SAVED");
		movecurse(2, 1);
		lcd_string("GUEST SAVED");
		_delay_ms(2000);
		lcd_cmd(clearscreen);
		_delay_ms(10);
	}
	//////////////////////////////////////////////////////////////////////////

	// LOG IN SYSTEM
	login_sys();

	while (1)
	{
		if (x=='0')
		{
			SET_BIT(PORTA,0);
			CLR_BIT(PORTA,1);
			CLR_BIT(PORTA,2);
			admin_menu();
		}
		else if (x=='1')
		{
			SET_BIT(PORTA,1);
			CLR_BIT(PORTA,0);
			CLR_BIT(PORTA,2);
			guest_menu();
		}
		else
		{
			CLR_BIT(PORTA,0);
			CLR_BIT(PORTA,1);
			CLR_BIT(PORTA,2);
		}
		
	}
}

ISR(TIMER0_OVF_vect)
{
	timer_count++;
	if ((timer_count * 32) > timer_time_wait)
	{
		timer_count = 0;
		lcd_cmd(clearscreen);
		lcd_string("timeout");
		x=NOTPRESSED;
		_delay_ms(1000);
		main();
	}
}

void take_password(char *arr)
{
	int ind = 0;
	timer_count = 0;
	char press = NOTPRESSED;
	Timer0_overflow_enable();
	while (ind < pass_length)
	{
		press = keypad_u8check_press();
		if (press != NOTPRESSED)
		{
			arr[ind] = press;
			timer_count = 0;
			lcd_char(press);
			_delay_ms(200);
			movecurse(2, 6 + ind);
			lcd_char(password_mark);
			ind++;
			_delay_ms(5);
		}
	}
	Timer0_overflow_disable();
}

bool array_equal(char *arr1, uint16_t arr2, uint8_t size)
{
	uint8_t x;
	for (x = 0; x < size; x++)
	{
		if ((unsigned char)(*(arr1 + x)) != EEPROM_read(arr2 + x))
		{
			return false;
		}
	}
	return true;
}

void login_sys(void)
{
	// LOG IN SYSTEM

	while (1)
	{
		lcd_cmd(clearscreen);
		_delay_ms(10);
		lcd_string("WHICH MODE?");
		movecurse(2, 1);
		lcd_string("0-ADMIN 1-GUEST");
		timer_count=0;
		Timer0_overflow_enable();

		x = NOTPRESSED;
		while (x == NOTPRESSED)
		{
			x = keypad_u8check_press();
		}
		timer_count=0;
		Timer0_overflow_disable();
		
		_delay_ms(300);
		if (x == '0')
		{
			uint8_t try;
			for (try = 0; try < pass_tries; try++)
			{
				lcd_cmd(clearscreen);
				_delay_ms(300);
				lcd_string("enter Admin");
				movecurse(2, 1);
				lcd_string("pass:");
				take_password(arr_pass);
				if (array_equal(arr_pass, ADDRESS_ADMIN, 4))
				{
					lcd_cmd(clearscreen);
					_delay_ms(10);
					lcd_string("RIGHT PASSWORD");
					_delay_ms(1000);
					break;
				}
				else
				{
					lcd_cmd(clearscreen);
					char arr[3];
					sprintf(arr, "%d", pass_tries - try - 1);
					lcd_string(arr);
					lcd_string("try left");
					_delay_ms(1000);
					lcd_cmd(clearscreen);
					_delay_ms(100);
				}
			}
			if (try == pass_tries)
			{
				lcd_cmd(clearscreen);
				_delay_ms(10);
				lcd_string("block 20 s");
				SET_BIT(PORTA,2);
				CLR_BIT(PORTA,1);
				CLR_BIT(PORTA,0);
				_delay_ms(20000);
			}
			else
			break;
		}
		else if (x == '1')
		{

			uint8_t try;
			for (try = 0; try < pass_tries; try++)
			{
				lcd_cmd(clearscreen);
				_delay_ms(300);
				lcd_string("enter GUEST");
				movecurse(2, 1);
				lcd_string("pass:");
				take_password(GUEST_PASS);
				if (array_equal(GUEST_PASS, ADDRESS_GUEST, 4))
				{
					lcd_cmd(clearscreen);
					_delay_ms(10);
					lcd_string("RIGHT PASSWORD");
					_delay_ms(1000);
					lcd_cmd(clearscreen);
					_delay_ms(10);
					break;
				}
				else
				{
					lcd_cmd(clearscreen);
					_delay_ms(10);
					char arr[3];
					sprintf(arr, "%d", pass_tries - try - 1);
					lcd_string(arr);
					lcd_string("try left");
					_delay_ms(1000);
					lcd_cmd(clearscreen);
					_delay_ms(100);
				}
			}
			if (try == pass_tries)
			{
				lcd_cmd(clearscreen);
				_delay_ms(10);
				lcd_string("block 20 s");
				SET_BIT(PORTA,2);
				CLR_BIT(PORTA,1);
				CLR_BIT(PORTA,0);
				_delay_ms(20000);
			}
			else
			break;
		}
		else
		{
		}
	}
}

void guest_menu(void)
{
	lcd_cmd(clearscreen);
	_delay_ms(10);
	lcd_string("1-TV 2-ROOMS");
	movecurse(2,1);
	lcd_string("3-RET");
	
	char press=NOTPRESSED;
	timer_count=0;
	Timer0_overflow_enable();
	
	while(press==NOTPRESSED || press<'1' || press>'3' )
	{
		press=keypad_u8check_press();
	}
	Timer0_overflow_disable();
	_delay_ms(300);
	
	if (press=='3')main();
	else if (press=='1')motor_door_garage(TV,guest);
	else if (press=='2')rooms(guest);
	else{}
}
void admin_menu(void)
{
	//////////////////////////////////////////////////////////////////////////
	//                                   devices                           //
	lcd_cmd(clearscreen);
	_delay_ms(10);
	lcd_string("1-CON 2-GAR 3-TV");
	movecurse(2,1);
	lcd_string("4-DOOR 5-ROOMS");
	
	char press=NOTPRESSED;
	timer_count=0;
	Timer0_overflow_enable();
	
	while(press==NOTPRESSED || press<'1' || press>'5' )
	{
		press=keypad_u8check_press();
	}
	Timer0_overflow_disable();
	_delay_ms(300);
	
	if (press=='1')
	{
		condition_menu();
	}
	else if (press=='2')
	{
		motor_door_garage(GARAGE,admin);
	}
	else if(press=='3')
	{
		motor_door_garage(TV,admin);
	}
	else if (press=='4')
	{
		motor_door_garage(DOOR,admin);
	}
	else if (press =='5')
	{
		rooms(admin);
	}
}
void condition_menu(void)
{
	lcd_cmd(clearscreen);
	_delay_ms(10);
	lcd_string("1-set temp 2-ret");
	movecurse(2,1);
	lcd_string("3-on 4-off");
	
	char press=NOTPRESSED;
	timer_count=0;
	Timer0_overflow_enable();
	
	while(press==NOTPRESSED || press<'1' || press>'4' )
	{
		press=keypad_u8check_press();
	}
	Timer0_overflow_disable();
	_delay_ms(300);
 
 	if (press=='3')
	{
		UART_SEND_CHAR(ON_COND);
		_delay_ms(50);
		lcd_cmd(clearscreen);
		_delay_ms(10);
		lcd_string("ok...");
		lcd_cmd(clearscreen);
		_delay_ms(10);
		condition_menu();
	}
	else if (press=='4')
	{
		
		UART_SEND_CHAR(OFF_COND);
		_delay_ms(50);
		lcd_cmd(clearscreen);
		_delay_ms(10);
		lcd_string("ok...");
		lcd_cmd(clearscreen);
		_delay_ms(10);
		condition_menu();
	}
	else if (press=='2')
	{
		admin_menu();
	}
	else if (press=='1')
	{
		lcd_cmd(clearscreen);
		_delay_ms(10);
		lcd_string("temp=__C");
		movecurse(1,6);
		char temp_array[3]={NOTPRESSED,NOTPRESSED,'\0'};
		uint16_t temp;
		for(int8_t i = 0;i<2;i++)
		{
			timer_count=0;
			Timer0_overflow_enable();
			
			while(temp_array[i]==NOTPRESSED || temp_array[i]<'0' || temp_array[i]>'9' )
			{
				temp_array[i]=keypad_u8check_press();
			}
			_delay_ms(300);
			lcd_char(temp_array[i]);
			Timer0_overflow_disable();
		}
		_delay_ms(500);
		temp=(temp_array[0]-'0')*10 + (temp_array[1]-'0');
		UART_SEND_CHAR(READ_TEMP);
		UART_SEND_CHAR((char)temp);
		_delay_ms(50);
		EEPROM_write(0x88,temp);
		
		lcd_cmd(clearscreen);
		_delay_ms(10);
		lcd_string("ok...");
		lcd_cmd(clearscreen);
		_delay_ms(10);
		condition_menu();
	}
	else
	{
		condition_menu();
	}
	
	
}


void motor_door_garage(MOTOR M , USER user)
{
	lcd_cmd(clearscreen);
	_delay_ms(10);
	lcd_string("1-open");
	movecurse(2,1);
	lcd_string("2-close 3-ret");
	
	char press=NOTPRESSED;
	timer_count=0;
	Timer0_overflow_enable();
	
	while(press==NOTPRESSED || press<'1' || press>'3' )
	{
		press=keypad_u8check_press();
	}
	Timer0_overflow_disable();
	_delay_ms(300);
	if(press=='1')
	{
		UART_SEND_CHAR(DOOR_OPEN+M);
		motor_door_garage(M,user);
	}
	else if(press=='2')
	{
		UART_SEND_CHAR(DOOR_CLOSE+M);
		motor_door_garage(M,user);
	}
	else if (press=='3')
	{
		if(user==admin)admin_menu();
		else if(user==guest)guest_menu();
	}
	else
	{
		
	}
}

void rooms(USER user)
{
	lcd_cmd(clearscreen);
	_delay_ms(10);
	uint8_t z=0;
	lcd_string("1-ON 2-OFF 3-RET");
	movecurse(2,1);
	
	
	if(user) lcd_string("4-EDIT_ROOMS");

	char press=NOTPRESSED;
	char press2=NOTPRESSED;
	char new_room[10]={0};
	timer_count=0;
	Timer0_overflow_enable();
	while(press==NOTPRESSED || press<'1' || press>'4')
	{
		press=keypad_u8check_press();
	}
	Timer0_overflow_disable();
	_delay_ms(300);
	lcd_cmd(clearscreen);
	_delay_ms(10);
	
	
	if(press =='4' && user==admin)
	{
		uint8_t edit_rooms=0;
		lcd_string("EDIT ROOMS:");
		movecurse(2,1);
		while (1)
		{
			press2=NOTPRESSED;
			timer_count=0;
			Timer0_overflow_enable();
			while(press2==NOTPRESSED)
			{
				press2=keypad_u8check_press();
			}
			if (press2=='A')
			{
				break;
			}
			else if(press2>='0' && press2<='7')
			{
				timer_count=0;
				lcd_char(press2);
				edit_rooms|=1<<(press2-'0');
			}
			_delay_ms(300);
		}
		Timer0_overflow_disable();
		EEPROM_write(ADDRESS_ROOMS,edit_rooms);
		give_rooms_names(rooms_names,8);
		lcd_cmd(clearscreen);
		_delay_ms(10);
		lcd_string("saved");
		_delay_ms(1000);
		lcd_cmd(clearscreen);
		rooms(user);
	}
	else if(press =='1')
	{
		on_room:
		lcd_cmd(clearscreen);
		_delay_ms(10);
		press2=NOTPRESSED;
		//memset(new_room, 0, sizeof(new_room));
		lcd_string("ON ROOM:");
		movecurse(2,1);

		timer_count = 0;
		press2 = NOTPRESSED;
		Timer0_overflow_enable();
		while (press2 == NOTPRESSED)
		{
			press2 = keypad_u8check_press();
		}
		Timer0_overflow_disable();
		_delay_ms(300);
		if (press2 == 'A')
		{
			rooms(user);
		}
		else if(press2>='0' && press2<='7')
		{
			if (rooms_names[press2-'0']!=NULL)
			{
				UART_SEND_CHAR(ROOM1_ON+press2-'0');
				_delay_ms(50);
				lcd_cmd(clearscreen);
				_delay_ms(10);
				lcd_string("ok...");
				_delay_ms(500);
				lcd_cmd(clearscreen);
				_delay_ms(10);
				goto on_room;
			}
			else
			{
				lcd_string("no room");
				_delay_ms(1000);
				goto on_room;
			}
		}
	}
	else if (press=='2')
	{
		off_room:
		lcd_cmd(clearscreen);
		_delay_ms(10);
		press2=NOTPRESSED;
	    //memset(new_room, 0, sizeof(new_room));
		lcd_string("OFF ROOM:");
		movecurse(2,1);

		timer_count = 0;
		press2 = NOTPRESSED;
		Timer0_overflow_enable();
		while (press2 == NOTPRESSED)
		{
			press2 = keypad_u8check_press();
		}
		Timer0_overflow_disable();
		_delay_ms(300);
		if (press2 == 'A')
		{
			rooms(user);
		}
		else if(press2>='0' && press2<='7')
		{
			if (rooms_names[press2-'0']!=NULL)
			{
				UART_SEND_CHAR(ROOM1_OFF+press2-'0');
				_delay_ms(50);
				lcd_cmd(clearscreen);
				_delay_ms(10);
				lcd_string("ok...");
				_delay_ms(500);
				lcd_cmd(clearscreen);
				_delay_ms(10);
				goto off_room;
			}
			else
			{
				lcd_string("no room");
				_delay_ms(1000);
				goto off_room;
			}
		}
	}
	else if (press=='3')
	{
		if(user==admin)admin_menu();
		else if(user==guest)guest_menu();
	}
	else{}
}