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
L Amplifier_Current:INA181 U1
U 1 1 6096FFE7
P 4750 3350
F 0 "U1" H 5094 3396 50  0000 L CNN
F 1 "INA181" H 5094 3305 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 4800 3400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina181.pdf" H 4900 3500 50  0001 C CNN
	1    4750 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 609D8FAA
P 4000 2700
F 0 "R3" V 3793 2700 50  0000 C CNN
F 1 "0.001Ohm" V 3884 2700 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3930 2700 50  0001 C CNN
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
Text GLabel 5050 3350 2    50   Input ~ 0
asense
$Comp
L power:GND #PWR07
U 1 1 60A075D5
P 4700 4050
F 0 "#PWR07" H 4700 3800 50  0001 C CNN
F 1 "GND" H 4705 3877 50  0000 C CNN
F 2 "" H 4700 4050 50  0001 C CNN
F 3 "" H 4700 4050 50  0001 C CNN
	1    4700 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 3650 4650 4000
Wire Wire Line
	4650 4000 4700 4000
Wire Wire Line
	4700 4000 4700 4050
Wire Wire Line
	4850 3650 4850 4000
Wire Wire Line
	4850 4000 4700 4000
Connection ~ 4700 4000
$Comp
L power:+3V3 #PWR06
U 1 1 60A07A39
P 4650 2900
F 0 "#PWR06" H 4650 2750 50  0001 C CNN
F 1 "+3V3" H 4665 3073 50  0000 C CNN
F 2 "" H 4650 2900 50  0001 C CNN
F 3 "" H 4650 2900 50  0001 C CNN
	1    4650 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2900 4650 3050
$Comp
L Device:C C1
U 1 1 60A081FF
P 5950 3400
F 0 "C1" H 6065 3446 50  0000 L CNN
F 1 "0.1uF" H 6065 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5988 3250 50  0001 C CNN
F 3 "~" H 5950 3400 50  0001 C CNN
	1    5950 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 60A089EA
P 5950 3700
F 0 "#PWR09" H 5950 3450 50  0001 C CNN
F 1 "GND" H 5955 3527 50  0000 C CNN
F 2 "" H 5950 3700 50  0001 C CNN
F 3 "" H 5950 3700 50  0001 C CNN
	1    5950 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR08
U 1 1 60A09244
P 5950 3050
F 0 "#PWR08" H 5950 2900 50  0001 C CNN
F 1 "+3V3" H 5965 3223 50  0000 C CNN
F 2 "" H 5950 3050 50  0001 C CNN
F 3 "" H 5950 3050 50  0001 C CNN
	1    5950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3050 5950 3250
Wire Wire Line
	5950 3550 5950 3700
Text GLabel 3200 2700 0    50   Input ~ 0
5VPSU
Wire Wire Line
	3200 2700 3850 2700
Connection ~ 3850 2700
Text GLabel 4400 2300 1    50   Input ~ 0
Anode
Wire Wire Line
	4150 2700 4400 2700
Wire Wire Line
	4400 2700 4400 2300
Connection ~ 4150 2700
$EndSCHEMATC
