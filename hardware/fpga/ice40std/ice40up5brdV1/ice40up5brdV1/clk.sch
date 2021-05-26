EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 9
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
L Oscillator:ASE-xxxMHz X1
U 1 1 60AB6E27
P 3650 3800
F 0 "X1" H 3994 3846 50  0000 L CNN
F 1 "ASE-xxxMHz" H 3994 3755 50  0000 L CNN
F 2 "Oscillator:Oscillator_SMD_Abracon_ASE-4Pin_3.2x2.5mm" H 4350 3450 50  0001 C CNN
F 3 "http://www.abracon.com/Oscillators/ASV.pdf" H 3550 3800 50  0001 C CNN
	1    3650 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 60AB7231
P 5250 3800
F 0 "C9" H 5365 3846 50  0000 L CNN
F 1 "0.01uF" H 5365 3755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5288 3650 50  0001 C CNN
F 3 "~" H 5250 3800 50  0001 C CNN
	1    5250 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 60AB7989
P 4500 4600
F 0 "#PWR024" H 4500 4350 50  0001 C CNN
F 1 "GND" H 4505 4427 50  0000 C CNN
F 2 "" H 4500 4600 50  0001 C CNN
F 3 "" H 4500 4600 50  0001 C CNN
	1    4500 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4100 3650 4300
Wire Wire Line
	3650 4300 4500 4300
Wire Wire Line
	4500 4300 4500 4600
Wire Wire Line
	4500 4300 5250 4300
Wire Wire Line
	5250 4300 5250 3950
Connection ~ 4500 4300
$Comp
L power:+3V3 #PWR023
U 1 1 60AB7CF6
P 4500 2900
F 0 "#PWR023" H 4500 2750 50  0001 C CNN
F 1 "+3V3" H 4515 3073 50  0000 C CNN
F 2 "" H 4500 2900 50  0001 C CNN
F 3 "" H 4500 2900 50  0001 C CNN
	1    4500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 3500 3650 3300
Wire Wire Line
	3650 3300 4500 3300
Wire Wire Line
	4500 3300 4500 2900
Wire Wire Line
	4500 3300 5250 3300
Wire Wire Line
	5250 3300 5250 3650
Connection ~ 4500 3300
Wire Wire Line
	3350 3800 3100 3800
Wire Wire Line
	3100 3800 3100 3300
Wire Wire Line
	3100 3300 3650 3300
Connection ~ 3650 3300
Text GLabel 3950 3800 2    50   Input ~ 0
clk
$EndSCHEMATC
