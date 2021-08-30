#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "remote.h"
#include "rckeys.h"
#include "eeprom.h"

uint8_t app_1,app_2,app_3,app_4,cmd=0;

int main()
{

DDRC|=((1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3));  //Make Portc as Output (Devices are connected here)

	//Give Some Time for other hardware to start
	_delay_loop_2(0);

	
	LCDInit(LS_BLINK);//Initialize the LCD Subsystem
	
	LCDClear();
	
	InitRemote();//Initialize the Remote Subsystem
	
	/*EEPROM bits have to set to either FF or 00. In default they will be set to FF
	but its better to check them using Extreme electronics USB avr burning software.
	If the values are other than those, set them to 00 or FF. 
	00 - OFF
	ff - ON

	I have written the EEPROM routines using atmega8 datasheet. Refer EEPROM section 
	in datasheet for more information regarding reading and writing of operations
	in EEPROM.
	*/
	app_1 = EEPROM_read(0);    //Reading from internal EEPROM
	app_2 = EEPROM_read(1);
	app_3 = EEPROM_read(2);
	app_4 = EEPROM_read(3);

	while(1)
	{
	switch(cmd)
		{
		case RC_1:
			app_1=~app_1;   //toggling the state of appliance
			if(app_1==0)
			PORTC|=(1<<PC0);    //Turn Off the appliance_1
			else
			PORTC&=(~(1<<PC0));		//Turn On the appliance_1
	
			EEPROM_write(0,app_1); //saving the state of appliance
			break;
		
		case RC_2:
			app_2=~app_2;   //toggling the state of appliance
			if(app_2==0)
			PORTC|=(1<<PC1);    //Turn Off the appliance_2
			else
			PORTC&=(~(1<<PC1));		//Turn On the appliance_2
			
			EEPROM_write(1,app_2); //saving the state of appliance
			break;
		
		case RC_3:
			app_3=~app_3;   //toggling the state of appliance
			if(app_3==0)
			PORTC|=(1<<PC2);    //Turn Off the appliance_3
			else
			PORTC&=(~(1<<PC2));		//Turn On the appliance_3
			
			EEPROM_write(2,app_3); //saving the state of appliance
			break;

		case RC_4:
			app_4=~app_4;   //toggling the state of appliance
			if(app_4==0)
			PORTC|=(1<<PC3);    //Turn Off the appliance_4
			else
			PORTC&=(~(1<<PC3));		//Turn On the appliance_4
			
			EEPROM_write(3,app_4); //saving the state of appliance
			break;

		case RC_POWER:  //turn off all the appliances
			app_1=0;
			app_2=0;
			app_3=0;
			app_4=0;
			EEPROM_write(0,app_1);
			EEPROM_write(1,app_2);
			EEPROM_write(2,app_3);
			EEPROM_write(3,app_4);
			break;
		}
			
			if(app_1==0)
			{LCDWriteStringXY(1,0,"A1-OFF");}
			else
			{LCDWriteStringXY(1,0,"A1-ON ");}
			
			if(app_2==0)
			{LCDWriteStringXY(9,0,"A2-OFF");}
			else
			{LCDWriteStringXY(9,0,"A2-ON ");}
						
			if(app_3==0)
			{LCDWriteStringXY(1,1,"A3-OFF");}
			else
			{LCDWriteStringXY(1,1,"A3-ON ");}
			

		
			if(app_4==0)
			{LCDWriteStringXY(9,1,"A4-OFF");}
			else
			{LCDWriteStringXY(9,1,"A4-ON ");}
			
			uint8_t cmd;  //dont cut and paste on the top
			cmd=GetRemoteCmd(1);	
			/*Get Remote Command. Command is taken in last because the
				the past device status has to be recovered first!   
			*/
	}


}
