EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 9
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
L power:GND #PWR01
U 1 1 60A0449C
P 4000 5000
F 0 "#PWR01" H 4000 4750 50  0001 C CNN
F 1 "GND" H 4005 4827 50  0000 C CNN
F 2 "" H 4000 5000 50  0001 C CNN
F 3 "" H 4000 5000 50  0001 C CNN
	1    4000 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4700 4000 4700
Wire Wire Line
	4000 4700 4000 5000
Text GLabel 3700 4600 2    50   Input ~ 0
vsense
Text GLabel 3700 4500 2    50   Input ~ 0
asense
Text GLabel 3700 4400 2    50   Input ~ 0
pwr_ctl
$Comp
L Connector:Conn_01x06_Male J1
U 1 1 60A0AD14
P 3500 4500
F 0 "J1" H 3608 4881 50  0000 C CNN
F 1 "Conn_01x06_Male" H 3608 4790 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 3500 4500 50  0001 C CNN
F 3 "~" H 3500 4500 50  0001 C CNN
	1    3500 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR02
U 1 1 60A0B9A9
P 4550 3950
F 0 "#PWR02" H 4550 3800 50  0001 C CNN
F 1 "+3V3" H 4565 4123 50  0000 C CNN
F 2 "" H 4550 3950 50  0001 C CNN
F 3 "" H 4550 3950 50  0001 C CNN
	1    4550 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4800 4400 4800
Wire Wire Line
	4550 4800 4550 3950
$Comp
L power:PWR_FLAG #FLG02
U 1 1 60A0BFC0
P 4400 4800
F 0 "#FLG02" H 4400 4875 50  0001 C CNN
F 1 "PWR_FLAG" H 4400 4973 50  0000 C CNN
F 2 "" H 4400 4800 50  0001 C CNN
F 3 "~" H 4400 4800 50  0001 C CNN
	1    4400 4800
	1    0    0    -1  
$EndComp
Connection ~ 4400 4800
Wire Wire Line
	4400 4800 4550 4800
$Comp
L power:PWR_FLAG #FLG01
U 1 1 60A0C34D
P 4000 4700
F 0 "#FLG01" H 4000 4775 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 4873 50  0000 C CNN
F 2 "" H 4000 4700 50  0001 C CNN
F 3 "~" H 4000 4700 50  0001 C CNN
	1    4000 4700
	1    0    0    -1  
$EndComp
Connection ~ 4000 4700
Text GLabel 3700 4300 2    50   Input ~ 0
led
$EndSCHEMATC
