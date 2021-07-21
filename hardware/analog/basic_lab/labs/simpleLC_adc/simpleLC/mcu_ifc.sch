EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L Connector:Conn_01x20_Female J3
U 1 1 60F88D36
P 3650 3400
F 0 "J3" H 3678 3376 50  0000 L CNN
F 1 "Conn_01x20_Female" H 3678 3285 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x20_P2.54mm_Horizontal" H 3650 3400 50  0001 C CNN
F 3 "~" H 3650 3400 50  0001 C CNN
	1    3650 3400
	1    0    0    -1  
$EndComp
Text GLabel 3450 3500 0    50   Input ~ 0
pwm
Text GLabel 3450 3600 0    50   Input ~ 0
adc
Text GLabel 2000 2900 0    50   Input ~ 0
pwm
NoConn ~ 2000 2900
$Comp
L power:GND #PWR?
U 1 1 60F8C25B
P 2800 4750
F 0 "#PWR?" H 2800 4500 50  0001 C CNN
F 1 "GND" H 2805 4577 50  0000 C CNN
F 2 "" H 2800 4750 50  0001 C CNN
F 3 "" H 2800 4750 50  0001 C CNN
	1    2800 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4750 2800 4400
Wire Wire Line
	2800 4200 3450 4200
Wire Wire Line
	3450 4400 2800 4400
Connection ~ 2800 4400
Wire Wire Line
	2800 4400 2800 4200
$Comp
L power:+3V3 #PWR?
U 1 1 60F8CBA5
P 2950 4000
F 0 "#PWR?" H 2950 3850 50  0001 C CNN
F 1 "+3V3" H 2965 4173 50  0000 C CNN
F 2 "" H 2950 4000 50  0001 C CNN
F 3 "" H 2950 4000 50  0001 C CNN
	1    2950 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4000 2950 4300
Wire Wire Line
	2950 4300 3450 4300
NoConn ~ 3450 2500
NoConn ~ 3450 2600
NoConn ~ 3450 2700
NoConn ~ 3450 2800
NoConn ~ 3450 2900
NoConn ~ 3450 3000
NoConn ~ 3450 3100
NoConn ~ 3450 3200
NoConn ~ 3450 3300
NoConn ~ 3450 3400
NoConn ~ 3450 3700
NoConn ~ 3450 3800
NoConn ~ 3450 3900
NoConn ~ 3450 4000
NoConn ~ 3450 4100
$EndSCHEMATC
