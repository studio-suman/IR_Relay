#ifndef _EEPROM_H
 #define _EEPROM_H
 /*____________________________________________________________________________________________*/



//Functions
//*********

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);

 /*____________________________________________________________________________________________*/
#endif
