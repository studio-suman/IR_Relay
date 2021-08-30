/******************************************************************************

                          eXtreme Electronics xAPI(TM)
						  ----------------------------
xAPI is a Powerful but easy to use C library to program the xBoard(TM)
series of AVR development board. The library has support for commonly use tasks
like:-

*LCD interfacing
*MUXED 7 segment displays.
*Remote Control
*Serial Communication
*DC Motor Controls with Speed Control
*Analog Measurement for Sensor Interface.
*Temperature Measurement.
*I2C Communication.
*EEPROM Interface
*Real Time Clock (RTC Interface)

The APIs are highly documented and easy to use even by a beginner.

For More Info Log On to 
www.eXtremeElectronics.co.in

Copyright 2008-2009 eXtreme Electronics India

                                 Remote Control Core
						        ---------------------
This module is used for interfacing with Standard NEC Protocol Remote Control.
For More information please see supplied tutorials and videos.

IR remote interfacing library for AVR series of microcontrollers.
Target MCU ATmege8 @ 8MHz/12MHz/16MHz


Sensor: TSOP1738 IR receiver module must be connected to INT0 Pin.
		This is PIN4 in ATmega8

		 -----
		|  _  |
		| | | |
		| | | |
		-------
		| |  |
	  	| |  | 
		| |  |
	(GND) |  (To PIN4)
         (5V)	

		**********************
		*TSOP 1738 Front View*
		**********************



Resource Usage:
-Timer1
-INT0 (PD2)

Please give credit to www.eXtremeElectronics.co.in if you use
it in your projects and find it useful

                                     NOTICE
									--------
NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN 
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.


WRITTEN BY:
AVINASH GUPTA
me@avinashgupta.com


*******************************************************************************/

#include <avr/interrupt.h>
#include <util/delay.h>

#include "remote.h"


//******************************************************
//               Environment Checkup
//               -------------------
//Test if user has correct clock & MCU setting.
//******************************************************

#ifndef F_CPU
#error "IR Remote Lib : F_CPU not defined"
#endif

#if ((F_CPU !=8000000) && (F_CPU !=12000000) && (F_CPU !=16000000))
#error "IR Remote Lib : Unsupported CPU frequency"
#error "IR remote Lib : Pls use F_CPU = 8MHz,12Mhz or 16MHz"
#endif

//Now check the CPU
#ifndef __AVR_ATmega8__
#error "IR Remote Lib : This version is only for ATmega8"
#error "IR Remote Lib : Pls see www.eXtremeElectronics.co.in for other CPU models"
#endif

#if F_CPU==8000000
	#define TIMER_COMP_VAL 80
		#elif F_CPU==12000000
			#define TIMER_COMP_VAL 120
				#elif F_CPU==16000000
					#define TIMER_COMP_VAL 160
					#endif


//Globals
volatile unsigned int Time;		//Main timer, stores time in 10us,
									//Updated by ISR(TIMER0_COMP)
volatile unsigned char BitNo;		//Pos of next BIT
volatile unsigned char ByteNo;		//Pos of current Byte

volatile unsigned char IrData[4];	//The four data Bytes of Ir Packet
									//2-Byte Address 2-Byte Data
volatile unsigned char IrCmdQ[QMAX];//Final Command Received (Buffer)

volatile unsigned char PrevCmd;		//Used for repeat

//Variables used for start repeating only after a key is pressed for certain time

volatile unsigned char Repeat;		//1=yes 0=no
volatile unsigned char RCount;		//Repeat count  

volatile char QFront=-1,QEnd=-1;

volatile unsigned char State;		//State of receiver

volatile unsigned char Edge;		//Edge of interrupt [ RISING=1 OR FALLING=0 ]

volatile unsigned int stop;





/**********************************************************************************************/
/*				F U N C T I O N S   S T A R T S												  */
/**********************************************************************************************/

void InitRemote()
{

	
	char i;
	for(i=0;i<4;i++) IrData[i]=0;

	stop=0;
	State=IR_VALIDATE_LEAD_HIGH;
	Edge=0;
	Repeat=0;

	//Setup Timer1
	//------------
	TCCR1B|=((1<<CS10)|(1<<WGM12));	//Prescaler : Fcpu   Mode : CTC

	TIMSK|=(1<<OCIE1A);	//Enable Output Compare Interrupt

	OCR1A=TIMER_COMP_VAL;			//Set Compare Value 


	//Set Up INT0
	//------------
	MCUCR|=(1<<ISC01);	//INT ON Falling Edge

	GICR|=(1<<INT0);	//Enable INT0

	//Enable Interrupts
	//-------------------

	SREG|=(1<<7);
}	

ISR(TIMER1_COMPA_vect)
{
	Time++;
}
ISR(INT0_vect)
{

	GICR&=(~(1<<INT0));	//Disable INT1
	sei();
	
	if(stop) return;
	unsigned int TempTime=Time;
	Time=0;
	TCNT0=0;
	switch(State)
	{
	case IR_VALIDATE_LEAD_HIGH:
	{
			if(Edge)
			{
				//Rising
				if((TempTime>(900-(900*TOL))) && (TempTime<(900+(900*TOL))))
				{

					//Lead High Correct
					State=IR_VALIDATE_LEAD_LOW;
					//INT ON FALLING EDGE
					MCUCR&=(~((1<<ISC01)|(1<<ISC00)));
					MCUCR|=(1<<ISC01);
					Edge=0;
				}
				else
				{
				/*
				stop=1;
				LCD_Clear();
				LCD_Write_String("Err in LeadHigh");
				LCD_GotoXY(0,1);
				LCD_Write_Int(TempTime,-1);
				*/

				ResetIR();
			
				}
			}
			else
			{
				//Falling
				MCUCR|=((1<<ISC01)|(1<<ISC00));	//Set INT on Rising Edge 
				Edge=1;
			}
		break;
	}
	case IR_VALIDATE_LEAD_LOW:
	{
		if((TempTime>(450-(450*TOL))) && (TempTime<(450+(450*TOL))))
		{
		//Got a valid leader
		State=IR_RECEIVE_BITS;
		BitNo=0;
		ByteNo=0;
		MCUCR|=((1<<ISC01)|(1<<ISC00));	//Set INT on Rising Edge
		Edge=1;

		
		}
		else if((TempTime>200) && (TempTime<245))
		{
			if(Repeat)
			{
			//Got a repeat pulse
			if((QEnd==(QMAX-1) && QFront==0)||((QEnd+1)==QFront))
						{
							QFront++;
							if(QFront==(QMAX)) 
								QFront=0;
						}

						if(QEnd==(QMAX-1))
							QEnd=0;
						else
							QEnd++;

						IrCmdQ[QEnd]=PrevCmd;

					if(QFront==-1) QFront=0;
			}
			else
			{
				RCount++;
				if(RCount==4) Repeat=1;
			}

				ResetIR();
		}else
		{/*
		stop=1;
				LCD_Clear();
				LCD_Write_String("Err in LeadLow");
				LCD_GotoXY(0,1);
				LCD_Write_Int(TempTime,-1);
		 */
		ResetIR();
		
		}
	break;
	}
	case IR_RECEIVE_BITS:
	{
		if(Edge)
		{
			//Rising
			if((TempTime>50) && (TempTime<69))
			{
				//Correct Beg of BIT found
				//INT ON FALLING EDGE
					MCUCR&=(~((1<<ISC01)|(1<<ISC00)));
					MCUCR|=(1<<ISC01);
					Edge=0;
			}
			else
			{
			/*	stop=1;
				LCD_Clear();
				LCD_Write_String("Err in Bit Beg");
				LCD_Write_StringXY(1,0,"Bit");
				LCD_Write_Int(BitNo,-1);
				LCD_Write_String("Byt");
				LCD_Write_Int(ByteNo,-1);
				LCD_GotoXY(13,1);
				LCD_Write_Int(TempTime,-1);*/
				ResetIR();
			}
		}
		else
		{
			//Falling
			if((TempTime>41) && (TempTime<58))
			{
				//We got a '0' here
				BitNo++;
				if(BitNo==8)
				{
					BitNo=0;
					ByteNo++;
					if(ByteNo==4)
					{
		
						State=IR_WAIT_STOP_BIT;

					}
				}
				MCUCR|=((1<<ISC01)|(1<<ISC00));	//Set INT on Rising Edge 
				Edge=1;
			}else if((TempTime>(169-(169*TOL))) && (TempTime<(169+(169*TOL))))
			{
				//We Have got a '1' here
				IrData[ByteNo]|=(1<<BitNo);
				BitNo++;
				if(BitNo==8)
				{
					BitNo=0;
					ByteNo++;
					if(ByteNo==4)
					{
						
						State=IR_WAIT_STOP_BIT;
					
					}
				}
				MCUCR|=((1<<ISC01)|(1<<ISC00));	//Set INT on Rising Edge 
				Edge=1;

			}else
			{
			/*Invalid Bit
				stop=1;
				LCD_Clear();
				LCD_Write_String("Err in Bit Low/Sp");
				LCD_Write_StringXY(1,0,"Bit");
				LCD_Write_Int(BitNo,-1);
				LCD_Write_String("Byt");
				LCD_Write_Int(ByteNo,-1);
				LCD_GotoXY(13,1);
				LCD_Write_Int(TempTime,-1);
			*/

			ResetIR();
			}
		}
	break;
	}
	case IR_WAIT_STOP_BIT:
	{
		if(Edge)
		{
			//Check for integrity
			if(IrData[2]==((unsigned char)~IrData[3]))
			{ 
				//Now We Have Got a packet
				//Add its Cmd to Queue
				
					//Step1:Check of Q full
					if((QEnd==(QMAX-1) && QFront==0)||((QEnd+1)==QFront))
					{
						QFront++;
						if(QFront==(QMAX)) 
							QFront=0;
					}

					if(QEnd==(QMAX-1))
						QEnd=0;
					else
						QEnd++;

					IrCmdQ[QEnd]=IrData[2];
					PrevCmd=IrData[2];

					if(QFront==-1) QFront=0;
				//Prevent repeating immediatly
				Repeat=0;//It will be enabled after 4 repeat pulses
				RCount=0;


				ResetIR();
			}
			
			
		}
	}
	break;
	}
	GICR|=(1<<INT0);	//Enable INT1
}

void ResetIR()
{
	char i;
	for(i=0;i<4;i++) IrData[i]=0;
	State=IR_VALIDATE_LEAD_HIGH;
	//INT ON FALLING EDGE
	MCUCR&=(~((1<<ISC01)|(1<<ISC00)));
	MCUCR|=(1<<ISC01);
	Edge=0;
}


unsigned char GetRemoteCmd(char wait)
{
	unsigned char cmd;

	if(wait)
		while(QFront==-1);
	else
		if(QFront==-1) return (RC_NONE);

	cmd=IrCmdQ[QFront];

	if(QFront==QEnd) 
		QFront=QEnd=-1;
	else
	{
		if(QFront==(QMAX-1))
			QFront=0;
		else
			QFront++;
	}

	return cmd;
}

	

				

					





