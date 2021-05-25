EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
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
	7500 2500 7600 2500
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
L power:PWR_FLAG #FLG05
U 1 1 5EA619BD
P 7600 2350
F 0 "#FLG05" H 7600 2425 50  0001 C CNN
F 1 "PWR_FLAG" H 7600 2523 50  0000 C CNN
F 2 "" H 7600 2350 50  0001 C CNN
F 3 "~" H 7600 2350 50  0001 C CNN
	1    7600 2350
	1    0    0    -1  
$EndComp
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
Wire Wire Line
	7600 2350 7600 2500
Connection ~ 7600 2500
Wire Wire Line
	7600 2500 7850 2500
$Comp
L Connector:Conn_01x20_Male J4
U 1 1 5EA6E6FC
P 1700 4600
F 0 "J4" H 1808 5681 50  0000 C CNN
F 1 "Conn_01x20_Male" H 1808 5590 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x20_P2.54mm_Horizontal" H 1700 4600 50  0001 C CNN
F 3 "~" H 1700 4600 50  0001 C CNN
	1    1700 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR053
U 1 1 5EA812AE
P 2350 3300
F 0 "#PWR053" H 2350 3150 50  0001 C CNN
F 1 "+3V3" H 2365 3473 50  0000 C CNN
F 2 "" H 2350 3300 50  0001 C CNN
F 3 "" H 2350 3300 50  0001 C CNN
	1    2350 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR052
U 1 1 5EA84AA8
P 2200 5950
F 0 "#PWR052" H 2200 5700 50  0001 C CNN
F 1 "GND" H 2205 5777 50  0000 C CNN
F 2 "" H 2200 5950 50  0001 C CNN
F 3 "" H 2200 5950 50  0001 C CNN
	1    2200 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3700 1900 3700
Wire Wire Line
	1900 4000 2200 4000
Connection ~ 2200 4000
Wire Wire Line
	2200 4000 2200 3700
Wire Wire Line
	1900 5100 2200 5100
Connection ~ 2200 5100
Wire Wire Line
	2200 5100 2200 4900
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
D0
Text GLabel 3850 4850 0    50   Input ~ 0
D1
Text GLabel 3850 4750 0    50   Input ~ 0
D2
Text GLabel 3850 4650 0    50   Input ~ 0
D3
Text GLabel 3850 4550 0    50   Input ~ 0
D4
Text GLabel 3850 4450 0    50   Input ~ 0
D5
Text GLabel 3850 4350 0    50   Input ~ 0
D6
Text GLabel 3850 4250 0    50   Input ~ 0
D7
Text GLabel 5050 4400 2    50   Input ~ 0
WP
Text GLabel 5050 4500 2    50   Input ~ 0
CLE
Text GLabel 5050 4600 2    50   Input ~ 0
ALE
Text GLabel 5050 4700 2    50   Input ~ 0
CE
Text GLabel 5050 4800 2    50   Input ~ 0
WE
Text GLabel 5050 4900 2    50   Input ~ 0
RE
Wire Wire Line
	2200 5100 2200 5950
Wire Wire Line
	1900 5600 2350 5600
Wire Wire Line
	2350 3300 2350 5600
Wire Wire Line
	1900 4900 2200 4900
Connection ~ 2200 4900
Wire Wire Line
	2200 4900 2200 4600
Wire Wire Line
	1900 4600 2200 4600
Connection ~ 2200 4600
Wire Wire Line
	2200 4000 2200 4300
Wire Wire Line
	1900 4300 2200 4300
Connection ~ 2200 4300
Wire Wire Line
	2200 4300 2200 4600
Text GLabel 1900 5200 2    50   Input ~ 0
CLE
Text GLabel 1900 5000 2    50   Input ~ 0
CE
Text GLabel 1900 5300 2    50   Input ~ 0
WE
Text GLabel 1900 5400 2    50   Input ~ 0
RE
Text GLabel 1900 5500 2    50   Input ~ 0
ALE
Text GLabel 1900 4800 2    50   Input ~ 0
D0
Text GLabel 1900 4700 2    50   Input ~ 0
D1
Text GLabel 1900 4500 2    50   Input ~ 0
D2
Text GLabel 1900 4400 2    50   Input ~ 0
D3
Text GLabel 1900 4200 2    50   Input ~ 0
D4
Text GLabel 1900 4100 2    50   Input ~ 0
D5
Text GLabel 1900 3900 2    50   Input ~ 0
D6
Text GLabel 1900 3800 2    50   Input ~ 0
D7
Text GLabel 7000 2500 0    50   Input ~ 0
WP
Wire Wire Line
	7000 2500 7500 2500
Connection ~ 7500 2500
$EndSCHEMATC
