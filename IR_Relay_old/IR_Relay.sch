EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 7650 7500 0    50   Italic 0
IR Relay Circuit
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U?
U 1 1 6039C861
P 4300 3100
F 0 "U?" H 3656 3146 50  0000 R CNN
F 1 "ATmega328-PU" H 3656 3055 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 4300 3100 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 4300 3100 50  0001 C CNN
	1    4300 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1600 4300 1150
Wire Wire Line
	4300 1150 5500 1150
Wire Wire Line
	4400 1600 5500 1600
Wire Wire Line
	5500 1150 5500 1600
$EndSCHEMATC
