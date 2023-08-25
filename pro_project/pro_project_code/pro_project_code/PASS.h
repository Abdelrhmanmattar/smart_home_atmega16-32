/*
 * PASS.h
 *
 * Created: 8/17/2023 5:01:50 PM
 *  Author: abdelrhman mattar
 */ 


#ifndef PASS_H_
#define PASS_H_

//////////////////////////////////////////////////////////////////////////
//door or garage
typedef enum
{
	DOOR=0,
	GARAGE=2,
	TV=4,
	
} MOTOR;

typedef enum
{
	admin=1,
	guest=0,
	
} USER;

//password macros
#define pass_length 4
#define First_or_not (uint16_t)0x099
#define ADDRESS_ADMIN (uint16_t)0X100
#define ADDRESS_GUEST (uint16_t)0X104
#define password_mark '*'
#define pass_tries 3

//////////////////////////////////////////////////////////////////////////
//timer macros
#define timer_time_wait 20000


//////////////////////////////////////////////////////////////////////////
//control rooms
#define ADDRESS_ROOMS (uint16_t)0X108




//////////////////////////////////////////////////////////////////////////
//uart message

#define READ_TEMP 1
#define ON_COND 2
#define OFF_COND 3

#define DOOR_OPEN 4
#define DOOR_CLOSE 5

#define GARAGE_OPEN 6
#define GARAGE_CLOSE 7

#define TV_OPEN 8
#define TV_CLOSE 9

//////////////////////////////////////////////////////////////////////////
//           ROOMS

#define ROOM1_ON 10
#define ROOM2_ON 11
#define ROOM3_ON 12
#define ROOM4_ON 13
#define ROOM5_ON 14
#define ROOM6_ON 15
#define ROOM7_ON 16
#define ROOM8_ON 17

#define ROOM1_OFF 18
#define ROOM2_OFF 19
#define ROOM3_OFF 20
#define ROOM4_OFF 21
#define ROOM5_OFF 22
#define ROOM6_OFF 23
#define ROOM7_OFF 24
#define ROOM8_OFF 25




#endif /* PASS_H_ */