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
L Connector:Conn_01x20_Male J2
U 1 1 61A42A54
P 4700 3450
F 0 "J2" H 4672 3332 50  0000 R CNN
F 1 "Conn_01x20_Male" H 4672 3423 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Horizontal" H 4700 3450 50  0001 C CNN
F 3 "~" H 4700 3450 50  0001 C CNN
	1    4700 3450
	-1   0    0    1   
$EndComp
Text GLabel 4500 2650 0    50   Input ~ 0
IO1_1A
Text GLabel 4500 2750 0    50   Input ~ 0
IO2_1A
Text GLabel 4500 3150 0    50   Input ~ 0
IO7_3A
Text GLabel 4500 3250 0    50   Input ~ 0
IO8_3B
Text GLabel 4500 3650 0    50   Input ~ 0
IO12_6A
Text GLabel 4500 3750 0    50   Input ~ 0
IO13_6B
Text GLabel 4500 4150 0    50   Input ~ 0
IO18_8A
Text GLabel 4500 4250 0    50   Input ~ 0
IO19_8B
$Comp
L Connector:Conn_01x20_Male J3
U 1 1 61A4ACA4
P 6300 3350
F 0 "J3" H 6408 4431 50  0000 C CNN
F 1 "Conn_01x20_Male" H 6408 4340 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Horizontal" H 6300 3350 50  0001 C CNN
F 3 "~" H 6300 3350 50  0001 C CNN
	1    6300 3350
	1    0    0    -1  
$EndComp
Text GLabel 6500 2950 2    50   Input ~ 0
pwm_tx_1
Text GLabel 6500 3150 2    50   Input ~ 0
pwm_tx_2
Text GLabel 6500 3350 2    50   Input ~ 0
pwm_tx_3
Text GLabel 6500 3550 2    50   Input ~ 0
pwm_tx_4
$Comp
L power:GND #PWR?
U 1 1 61A8D367
P 3450 4750
F 0 "#PWR?" H 3450 4500 50  0001 C CNN
F 1 "GND" H 3455 4577 50  0000 C CNN
F 2 "" H 3450 4750 50  0001 C CNN
F 3 "" H 3450 4750 50  0001 C CNN
	1    3450 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 4350 3450 4350
Wire Wire Line
	3450 4350 3450 4750
Wire Wire Line
	4500 2550 3450 2550
Wire Wire Line
	3450 2550 3450 2850
Connection ~ 3450 4350
Wire Wire Line
	4500 4050 3450 4050
Connection ~ 3450 4050
Wire Wire Line
	3450 4050 3450 4350
Wire Wire Line
	4500 3850 3450 3850
Connection ~ 3450 3850
Wire Wire Line
	3450 3850 3450 4050
Wire Wire Line
	4500 2850 3450 2850
Connection ~ 3450 2850
Wire Wire Line
	3450 2850 3450 3050
Wire Wire Line
	4500 3050 3450 3050
Connection ~ 3450 3050
Wire Wire Line
	3450 3050 3450 3350
Wire Wire Line
	4500 3350 3450 3350
Connection ~ 3450 3350
Wire Wire Line
	3450 3350 3450 3550
Wire Wire Line
	4500 3550 3450 3550
Connection ~ 3450 3550
Wire Wire Line
	3450 3550 3450 3850
$Comp
L power:+3V3 #PWR?
U 1 1 61A90947
P 3900 2150
F 0 "#PWR?" H 3900 2000 50  0001 C CNN
F 1 "+3V3" H 3915 2323 50  0000 C CNN
F 2 "" H 3900 2150 50  0001 C CNN
F 3 "" H 3900 2150 50  0001 C CNN
	1    3900 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2150 3900 2450
Wire Wire Line
	3900 2450 4500 2450
Wire Wire Line
	3900 2450 3900 2950
Wire Wire Line
	3900 3950 4500 3950
Connection ~ 3900 2450
Wire Wire Line
	4500 3450 3900 3450
Connection ~ 3900 3450
Wire Wire Line
	3900 3450 3900 3950
Wire Wire Line
	4500 2950 3900 2950
Connection ~ 3900 2950
Wire Wire Line
	3900 2950 3900 3450
$Comp
L power:GND #PWR?
U 1 1 61A9605D
P 7250 4900
F 0 "#PWR?" H 7250 4650 50  0001 C CNN
F 1 "GND" H 7255 4727 50  0000 C CNN
F 2 "" H 7250 4900 50  0001 C CNN
F 3 "" H 7250 4900 50  0001 C CNN
	1    7250 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2450 7250 2450
Wire Wire Line
	7250 2450 7250 2550
Wire Wire Line
	6500 4350 7250 4350
Connection ~ 7250 4350
Wire Wire Line
	7250 4350 7250 4900
Wire Wire Line
	6500 4250 7250 4250
Connection ~ 7250 4250
Wire Wire Line
	7250 4250 7250 4350
Wire Wire Line
	6500 4150 7250 4150
Connection ~ 7250 4150
Wire Wire Line
	7250 4150 7250 4250
Wire Wire Line
	6500 4050 7250 4050
Connection ~ 7250 4050
Wire Wire Line
	7250 4050 7250 4150
Wire Wire Line
	6500 3950 7250 3950
Connection ~ 7250 3950
Wire Wire Line
	7250 3950 7250 4050
Wire Wire Line
	6500 3850 7250 3850
Connection ~ 7250 3850
Wire Wire Line
	7250 3850 7250 3950
Wire Wire Line
	6500 3750 7250 3750
Connection ~ 7250 3750
Wire Wire Line
	7250 3750 7250 3850
Wire Wire Line
	6500 3650 7250 3650
Connection ~ 7250 3650
Wire Wire Line
	7250 3650 7250 3750
Wire Wire Line
	6500 3450 7250 3450
Connection ~ 7250 3450
Wire Wire Line
	7250 3450 7250 3650
Wire Wire Line
	6500 3250 7250 3250
Connection ~ 7250 3250
Wire Wire Line
	7250 3250 7250 3450
Wire Wire Line
	6500 3050 7250 3050
Connection ~ 7250 3050
Wire Wire Line
	7250 3050 7250 3250
Wire Wire Line
	6500 2850 7250 2850
Connection ~ 7250 2850
Wire Wire Line
	7250 2850 7250 3050
Wire Wire Line
	6500 2750 7250 2750
Connection ~ 7250 2750
Wire Wire Line
	7250 2750 7250 2850
Wire Wire Line
	6500 2650 7250 2650
Connection ~ 7250 2650
Wire Wire Line
	7250 2650 7250 2750
Wire Wire Line
	6500 2550 7250 2550
Connection ~ 7250 2550
Wire Wire Line
	7250 2550 7250 2650
$EndSCHEMATC
