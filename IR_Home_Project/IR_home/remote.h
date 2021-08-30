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

#ifndef _REMOTE_H
 #define _REMOTE_H
 /*____________________________________________________________________________________________*/

//Constants
//*********

//States
#define IR_VALIDATE_LEAD_HIGH 0
#define IR_VALIDATE_LEAD_LOW 1
#define IR_RECEIVE_BITS 3
#define IR_WAIT_STOP_BIT 4

//Others
#define TOL 0.1			//Tollerence for timming
#define QMAX 8			//Size of the Remote command buffer
#define RC_NONE 255		//This val is returned by GetRemoteCmd when no key is pressed


//Functions
//*********

void ResetIR();
void InitRemote();
unsigned char GetRemoteCmd(char wait);

 /*____________________________________________________________________________________________*/
#endif
