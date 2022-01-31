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
L bgt:Connector_Generic_Conn_01x20 J1
U 1 1 61F5F9B7
P 1600 3250
F 0 "J1" H 1518 2025 50  0000 C CNN
F 1 "Connector_Generic_Conn_01x20" H 1518 2116 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Vertical" H 1600 3250 50  0001 C CNN
F 3 "" H 1600 3250 50  0001 C CNN
	1    1600 3250
	-1   0    0    1   
$EndComp
$Comp
L bgt:Connector_Generic_Conn_01x02 J2
U 1 1 61F613C8
P 5350 2200
F 0 "J2" H 5430 2192 50  0000 L CNN
F 1 "Connector_Generic_Conn_01x02" H 5430 2101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5350 2200 50  0001 C CNN
F 3 "" H 5350 2200 50  0001 C CNN
	1    5350 2200
	1    0    0    -1  
$EndComp
Text GLabel 1800 3350 2    50   Input ~ 0
rfrx0_true
Text GLabel 1800 3550 2    50   Input ~ 0
rfrx0_false
Text GLabel 1800 4150 2    50   Input ~ 0
3V3
Text GLabel 1800 3150 2    50   Input ~ 0
GND
Text GLabel 1800 3450 2    50   Input ~ 0
GND
Text GLabel 1800 3650 2    50   Input ~ 0
GND
NoConn ~ 1800 4050
NoConn ~ 1800 3950
NoConn ~ 1800 3850
NoConn ~ 1800 3750
NoConn ~ 1800 3250
NoConn ~ 1800 3050
NoConn ~ 1800 2250
NoConn ~ 1800 2350
NoConn ~ 1800 2450
NoConn ~ 1800 2550
NoConn ~ 1800 2650
NoConn ~ 1800 2750
NoConn ~ 1800 2850
NoConn ~ 1800 2950
Text GLabel 5150 2300 0    50   Input ~ 0
GND
Text GLabel 2950 3750 0    50   Input ~ 0
rfrx0_false
Text GLabel 2950 3400 0    50   Input ~ 0
rfrx0_true
Text GLabel 2950 3100 0    50   Input ~ 0
3V3
Text GLabel 2950 4100 0    50   Input ~ 0
GND
$Comp
L Device:R R1
U 1 1 61F84AD6
P 3150 3250
F 0 "R1" H 3220 3296 50  0000 L CNN
F 1 "R" H 3220 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" V 3080 3250 50  0001 C CNN
F 3 "~" H 3150 3250 50  0001 C CNN
	1    3150 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 61F84D92
P 3300 3550
F 0 "R2" H 3370 3596 50  0000 L CNN
F 1 "R" H 3370 3505 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" V 3230 3550 50  0001 C CNN
F 3 "~" H 3300 3550 50  0001 C CNN
	1    3300 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 61F84FE9
P 3450 3850
F 0 "R3" H 3520 3896 50  0000 L CNN
F 1 "R" H 3520 3805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" V 3380 3850 50  0001 C CNN
F 3 "~" H 3450 3850 50  0001 C CNN
	1    3450 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 61F8539C
P 3450 4150
F 0 "R4" H 3520 4196 50  0000 L CNN
F 1 "R" H 3520 4105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P5.08mm_Vertical" V 3380 4150 50  0001 C CNN
F 3 "~" H 3450 4150 50  0001 C CNN
	1    3450 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3700 3450 3700
Wire Wire Line
	3450 4000 3250 4000
Wire Wire Line
	3250 4000 3250 3850
Wire Wire Line
	3250 3850 3100 3850
Wire Wire Line
	3100 3850 3100 3750
Wire Wire Line
	3100 3750 2950 3750
Connection ~ 3450 4000
Wire Wire Line
	2950 4100 3150 4100
Wire Wire Line
	3150 4100 3150 4450
Wire Wire Line
	3450 4450 3450 4300
Wire Wire Line
	3150 4450 3450 4450
Wire Wire Line
	2950 3100 3150 3100
Wire Wire Line
	2950 3400 3150 3400
Wire Wire Line
	3300 3400 3150 3400
Connection ~ 3150 3400
Wire Wire Line
	3300 3400 3600 3400
Wire Wire Line
	3600 3400 3600 3200
Wire Wire Line
	3900 3200 3900 3150
Wire Wire Line
	4700 2200 5150 2200
Wire Wire Line
	3600 3200 3900 3200
Connection ~ 3300 3400
Text GLabel 3900 3150 2    50   Input ~ 0
bpf_lvds
Text GLabel 4700 2200 0    50   Input ~ 0
bpf_ant
Text GLabel 4900 3150 0    50   Input ~ 0
bpf_lvds
$Comp
L Device:C C1
U 1 1 61F6D266
P 5250 3150
F 0 "C1" V 4998 3150 50  0000 C CNN
F 1 "C" V 5089 3150 50  0000 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5288 3000 50  0001 C CNN
F 3 "~" H 5250 3150 50  0001 C CNN
	1    5250 3150
	0    1    1    0   
$EndComp
$Comp
L Device:L L1
U 1 1 61F6D5EA
P 5900 3150
F 0 "L1" V 5719 3150 50  0000 C CNN
F 1 "L" V 5810 3150 50  0000 C CNN
F 2 "Inductor_SMD:L_1206_3216Metric" H 5900 3150 50  0001 C CNN
F 3 "~" H 5900 3150 50  0001 C CNN
	1    5900 3150
	0    1    1    0   
$EndComp
$Comp
L Device:L L2
U 1 1 61F6DBD3
P 6500 3500
F 0 "L2" H 6553 3546 50  0000 L CNN
F 1 "L" H 6553 3455 50  0000 L CNN
F 2 "Inductor_SMD:L_1206_3216Metric" H 6500 3500 50  0001 C CNN
F 3 "~" H 6500 3500 50  0001 C CNN
	1    6500 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 61F6DFD0
P 6900 3500
F 0 "C2" H 7015 3546 50  0000 L CNN
F 1 "C" H 7015 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 6938 3350 50  0001 C CNN
F 3 "~" H 6900 3500 50  0001 C CNN
	1    6900 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 61F6E415
P 7400 3150
F 0 "C3" V 7148 3150 50  0000 C CNN
F 1 "C" V 7239 3150 50  0000 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 7438 3000 50  0001 C CNN
F 3 "~" H 7400 3150 50  0001 C CNN
	1    7400 3150
	0    1    1    0   
$EndComp
$Comp
L Device:L L3
U 1 1 61F6E880
P 8100 3150
F 0 "L3" V 7919 3150 50  0000 C CNN
F 1 "L" V 8010 3150 50  0000 C CNN
F 2 "Inductor_SMD:L_1206_3216Metric" H 8100 3150 50  0001 C CNN
F 3 "~" H 8100 3150 50  0001 C CNN
	1    8100 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 3150 5100 3150
Wire Wire Line
	5400 3150 5750 3150
Wire Wire Line
	6050 3150 6500 3150
Wire Wire Line
	7550 3150 7950 3150
Wire Wire Line
	6500 3350 6500 3150
Connection ~ 6500 3150
Wire Wire Line
	6500 3150 6900 3150
Wire Wire Line
	6900 3350 6900 3150
Connection ~ 6900 3150
Wire Wire Line
	6900 3150 7250 3150
$Comp
L power:GND #PWR01
U 1 1 61F71582
P 6500 4000
F 0 "#PWR01" H 6500 3750 50  0001 C CNN
F 1 "GND" H 6505 3827 50  0000 C CNN
F 2 "" H 6500 4000 50  0001 C CNN
F 3 "" H 6500 4000 50  0001 C CNN
	1    6500 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 61F7180C
P 6900 4000
F 0 "#PWR02" H 6900 3750 50  0001 C CNN
F 1 "GND" H 6905 3827 50  0000 C CNN
F 2 "" H 6900 4000 50  0001 C CNN
F 3 "" H 6900 4000 50  0001 C CNN
	1    6900 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4000 6900 3650
Wire Wire Line
	6500 3650 6500 4000
Text GLabel 8550 3150 2    50   Input ~ 0
bpf_ant
Wire Wire Line
	8250 3150 8550 3150
$Comp
L power:GND #PWR0101
U 1 1 61F8878F
P 4550 4850
F 0 "#PWR0101" H 4550 4600 50  0001 C CNN
F 1 "GND" H 4555 4677 50  0000 C CNN
F 2 "" H 4550 4850 50  0001 C CNN
F 3 "" H 4550 4850 50  0001 C CNN
	1    4550 4850
	1    0    0    -1  
$EndComp
Text GLabel 4950 4400 2    50   Input ~ 0
GND
Wire Wire Line
	4550 4400 4950 4400
Wire Wire Line
	4550 4400 4550 4850
$EndSCHEMATC
