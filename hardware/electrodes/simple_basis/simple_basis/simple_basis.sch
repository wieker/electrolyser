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
$Comp
L Connector:Conn_01x04_Male J4
U 1 1 5F84C5E9
P 8250 3100
F 0 "J4" H 8358 3381 50  0000 C CNN
F 1 "Conn_01x04_Male" H 8358 3290 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8250 3100 50  0001 C CNN
F 3 "~" H 8250 3100 50  0001 C CNN
	1    8250 3100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5F84CE07
P 7200 3100
F 0 "J3" H 7308 3381 50  0000 C CNN
F 1 "Conn_01x04_Male" H 7308 3290 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7200 3100 50  0001 C CNN
F 3 "~" H 7200 3100 50  0001 C CNN
	1    7200 3100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5F84DC28
P 3800 3000
F 0 "J1" H 3908 3281 50  0000 C CNN
F 1 "Conn_01x04_Male" H 3908 3190 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3800 3000 50  0001 C CNN
F 3 "~" H 3800 3000 50  0001 C CNN
	1    3800 3000
	1    0    0    -1  
$EndComp
$Comp
L electrolyser_electrodes:Metallic_Electrode J2
U 1 1 5F84D1DE
P 4850 3050
F 0 "J2" H 4930 3092 50  0000 L CNN
F 1 "Metallic_Electrode" H 4930 3001 50  0000 L CNN
F 2 "electrodes:Electrode_ThroughHole_V0.1" H 4850 3050 50  0001 C CNN
F 3 "~" H 4850 3050 50  0001 C CNN
	1    4850 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2900 4000 3000
Connection ~ 4000 3000
Wire Wire Line
	4000 3000 4000 3100
Connection ~ 4000 3100
Wire Wire Line
	4000 3100 4000 3200
Wire Wire Line
	4000 3000 4300 3000
Wire Wire Line
	4300 3000 4300 3050
Wire Wire Line
	4300 3050 4650 3050
Wire Wire Line
	7400 3000 7400 3100
Wire Wire Line
	7400 3100 7400 3200
Connection ~ 7400 3100
Connection ~ 7400 3200
Wire Wire Line
	7400 3200 7400 3300
Wire Wire Line
	7400 3300 7400 3800
Wire Wire Line
	7400 3800 8450 3800
Wire Wire Line
	8450 3800 8450 3300
Connection ~ 7400 3300
Connection ~ 8450 3100
Wire Wire Line
	8450 3100 8450 3000
Connection ~ 8450 3200
Wire Wire Line
	8450 3200 8450 3100
Connection ~ 8450 3300
Wire Wire Line
	8450 3300 8450 3200
$EndSCHEMATC
