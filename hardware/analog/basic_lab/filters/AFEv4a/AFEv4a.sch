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
F 2 "" H 1600 3250 50  0001 C CNN
F 3 "" H 1600 3250 50  0001 C CNN
	1    1600 3250
	-1   0    0    1   
$EndComp
$Comp
L bgt:Connector_Generic_Conn_01x02 J2
U 1 1 61F613C8
P 4550 3150
F 0 "J2" H 4630 3142 50  0000 L CNN
F 1 "Connector_Generic_Conn_01x02" H 4630 3051 50  0000 L CNN
F 2 "" H 4550 3150 50  0001 C CNN
F 3 "" H 4550 3150 50  0001 C CNN
	1    4550 3150
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
Text GLabel 4350 3250 0    50   Input ~ 0
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
F 2 "" V 3080 3250 50  0001 C CNN
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
F 2 "" V 3230 3550 50  0001 C CNN
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
F 2 "" V 3380 3850 50  0001 C CNN
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
F 2 "" V 3380 4150 50  0001 C CNN
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
	3900 3150 4350 3150
Wire Wire Line
	3600 3200 3900 3200
Connection ~ 3300 3400
$EndSCHEMATC
