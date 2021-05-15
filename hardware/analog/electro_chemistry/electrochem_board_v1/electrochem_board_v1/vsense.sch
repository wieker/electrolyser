EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 4250 3350 0    50   Input ~ 0
Anode
$Comp
L power:GND #PWR05
U 1 1 60A0E701
P 4950 4650
F 0 "#PWR05" H 4950 4400 50  0001 C CNN
F 1 "GND" H 4955 4477 50  0000 C CNN
F 2 "" H 4950 4650 50  0001 C CNN
F 3 "" H 4950 4650 50  0001 C CNN
	1    4950 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 60A0E91D
P 4950 3800
F 0 "R1" H 5020 3846 50  0000 L CNN
F 1 "10kOhm" H 5020 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4880 3800 50  0001 C CNN
F 3 "~" H 4950 3800 50  0001 C CNN
	1    4950 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 60A0EAED
P 4950 4300
F 0 "R2" H 5020 4346 50  0000 L CNN
F 1 "10kOhm" H 5020 4255 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4880 4300 50  0001 C CNN
F 3 "~" H 4950 4300 50  0001 C CNN
	1    4950 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3350 4950 3350
Wire Wire Line
	4950 3350 4950 3650
Wire Wire Line
	4950 3950 4950 4050
Wire Wire Line
	4950 4450 4950 4650
Text GLabel 4250 4050 0    50   Input ~ 0
vsense
Wire Wire Line
	4250 4050 4950 4050
Connection ~ 4950 4050
Wire Wire Line
	4950 4050 4950 4150
$EndSCHEMATC
