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
L Device:C C16
U 1 1 5EA60079
P 7500 2950
F 0 "C16" H 7615 2996 50  0000 L CNN
F 1 "0.1uF" H 7615 2905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 7538 2800 50  0001 C CNN
F 3 "~" H 7500 2950 50  0001 C CNN
	1    7500 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C17
U 1 1 5EA6062B
P 8200 2950
F 0 "C17" H 8315 2996 50  0000 L CNN
F 1 "4.7uF" H 8315 2905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8238 2800 50  0001 C CNN
F 3 "~" H 8200 2950 50  0001 C CNN
	1    8200 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR057
U 1 1 5EA60B6E
P 7850 3850
F 0 "#PWR057" H 7850 3600 50  0001 C CNN
F 1 "GND" H 7855 3677 50  0000 C CNN
F 2 "" H 7850 3850 50  0001 C CNN
F 3 "" H 7850 3850 50  0001 C CNN
	1    7850 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR056
U 1 1 5EA60E78
P 7850 2150
F 0 "#PWR056" H 7850 2000 50  0001 C CNN
F 1 "+3V3" H 7865 2323 50  0000 C CNN
F 2 "" H 7850 2150 50  0001 C CNN
F 3 "" H 7850 2150 50  0001 C CNN
	1    7850 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 2800 7500 2500
Wire Wire Line
	8200 2500 8200 2800
Wire Wire Line
	7850 2150 7850 2500
Connection ~ 7850 2500
Wire Wire Line
	7850 2500 8200 2500
Wire Wire Line
	7500 3100 7500 3500
Wire Wire Line
	7500 3500 7850 3500
Wire Wire Line
	8200 3500 8200 3100
Wire Wire Line
	7850 3850 7850 3500
Connection ~ 7850 3500
Wire Wire Line
	7850 3500 8200 3500
$Comp
L power:PWR_FLAG #FLG06
U 1 1 5EA61D74
P 7850 3250
F 0 "#FLG06" H 7850 3325 50  0001 C CNN
F 1 "PWR_FLAG" H 7850 3423 50  0000 C CNN
F 2 "" H 7850 3250 50  0001 C CNN
F 3 "~" H 7850 3250 50  0001 C CNN
	1    7850 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3500 7850 3250
$Comp
L digital:MT29F1G08ABAEAWP U6
U 1 1 60A46341
P 4450 3850
F 0 "U6" H 4450 5431 50  0000 C CNN
F 1 "MT29F1G08ABAEAWP" H 4450 5340 50  0000 C CNN
F 2 "Package_SO:TSOP-I-48_18.4x12mm_P0.5mm" H 5500 2500 50  0001 C CNN
F 3 "https://www.mouser.ee/datasheet/2/671/micron_technology_mict-s-a0000866752-1-1759126.pdf" H 4150 4300 50  0001 C CNN
	1    4450 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR054
U 1 1 60A607CF
P 4400 5650
F 0 "#PWR054" H 4400 5400 50  0001 C CNN
F 1 "GND" H 4405 5477 50  0000 C CNN
F 2 "" H 4400 5650 50  0001 C CNN
F 3 "" H 4400 5650 50  0001 C CNN
	1    4400 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 5500 4400 5500
Wire Wire Line
	4400 5500 4400 5650
Wire Wire Line
	4400 5500 4450 5500
Wire Wire Line
	4450 5500 4450 5250
Connection ~ 4400 5500
Wire Wire Line
	4350 5250 4350 5500
$Comp
L power:+3V3 #PWR055
U 1 1 60A63A01
P 4550 1850
F 0 "#PWR055" H 4550 1700 50  0001 C CNN
F 1 "+3V3" H 4565 2023 50  0000 C CNN
F 2 "" H 4550 1850 50  0001 C CNN
F 3 "" H 4550 1850 50  0001 C CNN
	1    4550 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2450 4450 2150
Wire Wire Line
	4450 2150 4550 2150
Wire Wire Line
	4550 2150 4550 1850
Wire Wire Line
	4550 2150 4600 2150
Wire Wire Line
	4600 2150 4600 2450
Connection ~ 4550 2150
Text GLabel 3850 4950 0    50   Input ~ 0
d0
Text GLabel 3850 4850 0    50   Input ~ 0
d1
Text GLabel 3850 4750 0    50   Input ~ 0
d2
Text GLabel 3850 4650 0    50   Input ~ 0
d3
Text GLabel 3850 4550 0    50   Input ~ 0
d4
Text GLabel 3850 4450 0    50   Input ~ 0
d5
Text GLabel 3850 4350 0    50   Input ~ 0
d6
Text GLabel 3850 4250 0    50   Input ~ 0
d7
Text GLabel 5050 4400 2    50   Input ~ 0
WP
Text GLabel 5050 4600 2    50   Input ~ 0
ale
Text GLabel 5050 4700 2    50   Input ~ 0
ce
Text GLabel 5050 4800 2    50   Input ~ 0
we
Text GLabel 5050 4900 2    50   Input ~ 0
re
Text GLabel 7000 2500 0    50   Input ~ 0
WP
Wire Wire Line
	7000 2500 7500 2500
Connection ~ 7500 2500
Wire Wire Line
	7500 2500 7850 2500
Text GLabel 5050 4500 2    50   Input ~ 0
cle
$EndSCHEMATC
