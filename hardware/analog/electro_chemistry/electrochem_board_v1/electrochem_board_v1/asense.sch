EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Amplifier_Current:INA181 U?
U 1 1 6096FFE7
P 4750 3350
F 0 "U?" H 5094 3396 50  0000 L CNN
F 1 "INA181" H 5094 3305 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 4800 3400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina181.pdf" H 4900 3500 50  0001 C CNN
	1    4750 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 609D8FAA
P 4000 2700
F 0 "R?" V 3793 2700 50  0000 C CNN
F 1 "R" V 3884 2700 50  0000 C CNN
F 2 "" V 3930 2700 50  0001 C CNN
F 3 "~" H 4000 2700 50  0001 C CNN
	1    4000 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 2700 4150 3450
Wire Wire Line
	4150 3450 4450 3450
Wire Wire Line
	4450 3250 3850 3250
Wire Wire Line
	3850 3250 3850 2700
$EndSCHEMATC
