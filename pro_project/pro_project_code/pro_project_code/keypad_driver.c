
 #include "keypad_driver.h"
 void keypad_vInit()
 {
	 dio_setport('C',0X0F);
	dio_writeport('C',0XF0);	  
 }
 char keypad_u8check_press()
 {
	 char arr[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'A','0','=','+'}};
	 char row,coloumn,x;
	 char returnval=NOTPRESSED;
	 for(row=0;row<4;row++)
	 {
		 dio_writepin('C',0,1);
		 dio_writepin('C',1,1);
		 dio_writepin('C',2,1);
		 dio_writepin('C',3,1);
		 dio_writepin('C',row,0);
		 
		 for(coloumn=0;coloumn<4;coloumn++)
		 {
			 x=dio_readpin('C',(coloumn+4));
			 if(x==0)
			 {
				 returnval=arr[row][coloumn];
				 break;
			 }
		 }
		 if(x==0)
		 {
			 break;
		 }
	 }
	 return returnval ;	 
 }