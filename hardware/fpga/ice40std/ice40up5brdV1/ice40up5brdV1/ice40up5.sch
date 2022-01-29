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
L FPGA_Lattice:ICE40UP5K-SG48ITR U1
U 1 1 60A6AE69
P 3150 3800
F 0 "U1" H 3480 3853 50  0000 L CNN
F 1 "ICE40UP5K-SG48ITR" H 3480 3762 50  0000 L CNN
F 2 "Package_DFN_QFN:QFN-48-1EP_7x7mm_P0.5mm_EP5.6x5.6mm" H 3150 2450 50  0001 C CNN
F 3 "http://www.latticesemi.com/Products/FPGAandCPLD/iCE40Ultra" H 2750 4800 50  0001 C CNN
	1    3150 3800
	1    0    0    -1  
$EndComp
$Comp
L FPGA_Lattice:ICE40UP5K-SG48ITR U1
U 3 1 60A6E562
P 7450 3500
F 0 "U1" H 7780 3603 50  0000 L CNN
F 1 "ICE40UP5K-SG48ITR" H 7780 3512 50  0000 L CNN
F 2 "Package_DFN_QFN:QFN-48-1EP_7x7mm_P0.5mm_EP5.6x5.6mm" H 7450 2150 50  0001 C CNN
F 3 "http://www.latticesemi.com/Products/FPGAandCPLD/iCE40Ultra" H 7050 4500 50  0001 C CNN
	3    7450 3500
	1    0    0    -1  
$EndComp
Text GLabel 2750 3200 0    50   Input ~ 0
ale
Text GLabel 2750 3500 0    50   Input ~ 0
re
Text GLabel 2750 3400 0    50   Input ~ 0
we
Text GLabel 2750 3600 0    50   Input ~ 0
cle
Text GLabel 2750 3700 0    50   Input ~ 0
rfrx0_false
Text GLabel 2750 3800 0    50   Input ~ 0
rfrx0_true
Text GLabel 2750 3900 0    50   Input ~ 0
d1
Text GLabel 2750 4100 0    50   Input ~ 0
d2
Text GLabel 2750 4200 0    50   Input ~ 0
d3
Text GLabel 2750 4000 0    50   Input ~ 0
d4
Text GLabel 2750 4400 0    50   Input ~ 0
d5
Text GLabel 2750 4600 0    50   Input ~ 0
d6
Text GLabel 2750 4700 0    50   Input ~ 0
d7
Text GLabel 7050 3900 0    50   Input ~ 0
a1
Text GLabel 7050 4000 0    50   Input ~ 0
b1
Text GLabel 7050 3800 0    50   Input ~ 0
a2
Text GLabel 7050 3600 0    50   Input ~ 0
b2
$Comp
L power:+3V3 #PWR01
U 1 1 60B33280
P 3150 2500
F 0 "#PWR01" H 3150 2350 50  0001 C CNN
F 1 "+3V3" H 3165 2673 50  0000 C CNN
F 2 "" H 3150 2500 50  0001 C CNN
F 3 "" H 3150 2500 50  0001 C CNN
	1    3150 2500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR04
U 1 1 60B336AB
P 7450 2600
F 0 "#PWR04" H 7450 2450 50  0001 C CNN
F 1 "+3V3" H 7465 2773 50  0000 C CNN
F 2 "" H 7450 2600 50  0001 C CNN
F 3 "" H 7450 2600 50  0001 C CNN
	1    7450 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2600 7450 2800
Wire Wire Line
	3150 2500 3150 2700
$Comp
L Device:C C1
U 1 1 60B44B94
P 5000 1950
F 0 "C1" H 5115 1996 50  0000 L CNN
F 1 "0.1uF" H 5115 1905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5038 1800 50  0001 C CNN
F 3 "~" H 5000 1950 50  0001 C CNN
	1    5000 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 60B45B6D
P 5600 1950
F 0 "C2" H 5715 1996 50  0000 L CNN
F 1 "0.1uF" H 5715 1905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5638 1800 50  0001 C CNN
F 3 "~" H 5600 1950 50  0001 C CNN
	1    5600 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR03
U 1 1 60B45F7E
P 5300 1350
F 0 "#PWR03" H 5300 1200 50  0001 C CNN
F 1 "+3V3" H 5315 1523 50  0000 C CNN
F 2 "" H 5300 1350 50  0001 C CNN
F 3 "" H 5300 1350 50  0001 C CNN
	1    5300 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 60B462D1
P 5250 2600
F 0 "#PWR02" H 5250 2350 50  0001 C CNN
F 1 "GND" H 5255 2427 50  0000 C CNN
F 2 "" H 5250 2600 50  0001 C CNN
F 3 "" H 5250 2600 50  0001 C CNN
	1    5250 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 1800 5000 1600
Wire Wire Line
	5000 1600 5300 1600
Wire Wire Line
	5600 1600 5600 1800
Wire Wire Line
	5300 1350 5300 1600
Connection ~ 5300 1600
Wire Wire Line
	5300 1600 5600 1600
Wire Wire Line
	5000 2100 5000 2400
Wire Wire Line
	5000 2400 5250 2400
Wire Wire Line
	5600 2400 5600 2100
Wire Wire Line
	5250 2400 5250 2600
Connection ~ 5250 2400
Wire Wire Line
	5250 2400 5600 2400
NoConn ~ 2750 3300
Text GLabel 2750 4800 0    50   Input ~ 0
l1
Text GLabel 2750 4500 0    50   Input ~ 0
l2
Text GLabel 2750 4300 0    50   Input ~ 0
l3
Text GLabel 6950 3300 0    50   Input ~ 0
btn1
Wire Wire Line
	6950 3300 7050 3300
Text GLabel 6950 3700 0    50   Input ~ 0
btn2
Wire Wire Line
	6950 3700 7050 3700
Text GLabel 6950 3500 0    50   Input ~ 0
btn3
Wire Wire Line
	6950 3500 7050 3500
NoConn ~ 7050 3400
$EndSCHEMATC
