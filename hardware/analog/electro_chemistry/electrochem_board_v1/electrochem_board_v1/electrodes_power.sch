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
L electrodes:Metallic_Electrode J3
U 1 1 60A1085B
P 2950 2750
F 0 "J3" H 3030 2792 50  0000 L CNN
F 1 "Metallic_Electrode" H 3030 2701 50  0000 L CNN
F 2 "electrodes:Electrode_ThroughHole_V0.1" H 2950 2750 50  0001 C CNN
F 3 "~" H 2950 2750 50  0001 C CNN
	1    2950 2750
	1    0    0    -1  
$EndComp
$Comp
L electrodes:Metallic_Electrode J4
U 1 1 60A10B7F
P 4950 2850
F 0 "J4" V 4914 2762 50  0000 R CNN
F 1 "Metallic_Electrode" V 4823 2762 50  0000 R CNN
F 2 "electrodes:Electrode_ThroughHole_V0.1" H 4950 2850 50  0001 C CNN
F 3 "~" H 4950 2850 50  0001 C CNN
	1    4950 2850
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 60A10F93
P 4950 3400
F 0 "#PWR04" H 4950 3150 50  0001 C CNN
F 1 "GND" H 4955 3227 50  0000 C CNN
F 2 "" H 4950 3400 50  0001 C CNN
F 3 "" H 4950 3400 50  0001 C CNN
	1    4950 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3050 4950 3400
Text GLabel 2500 2750 0    50   Input ~ 0
Anode
Wire Wire Line
	2500 2750 2750 2750
$EndSCHEMATC
