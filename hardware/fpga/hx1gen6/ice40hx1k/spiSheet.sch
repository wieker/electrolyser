EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 8
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
L Memory_Flash:AT25SF081-SSHD-X U?
U 1 1 5E610BE8
P 2300 1900
AR Path="/5E610BE8" Ref="U?"  Part="1" 
AR Path="/5E5F2441/5E610BE8" Ref="U?"  Part="1" 
AR Path="/5E9ED365/5E610BE8" Ref="U1"  Part="1" 
F 0 "U1" H 2300 1219 50  0000 C CNN
F 1 "AT25SF081-SSHD-X" H 2300 1310 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 2300 1300 50  0001 C CNN
F 3 "https://www.adestotech.com/wp-content/uploads/DS-AT25SF081_045.pdf" H 2300 1900 50  0001 C CNN
	1    2300 1900
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5E610BEE
P 2300 2700
AR Path="/5E610BEE" Ref="C?"  Part="1" 
AR Path="/5E5F2441/5E610BEE" Ref="C?"  Part="1" 
AR Path="/5E9ED365/5E610BEE" Ref="C1"  Part="1" 
F 0 "C1" H 2415 2746 50  0000 L CNN
F 1 "0.1uF" H 2415 2655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2338 2550 50  0001 C CNN
F 3 "~" H 2300 2700 50  0001 C CNN
	1    2300 2700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E610BF4
P 1950 2550
AR Path="/5E610BF4" Ref="#PWR?"  Part="1" 
AR Path="/5E5F2441/5E610BF4" Ref="#PWR?"  Part="1" 
AR Path="/5E9ED365/5E610BF4" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 1950 2400 50  0001 C CNN
F 1 "+3V3" H 1965 2723 50  0000 C CNN
F 2 "" H 1950 2550 50  0001 C CNN
F 3 "" H 1950 2550 50  0001 C CNN
	1    1950 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2550 2300 2550
Wire Wire Line
	2300 2550 2300 2400
Connection ~ 2300 2550
Wire Wire Line
	2900 1700 3100 1700
Wire Wire Line
	3100 1700 3100 1800
Wire Wire Line
	3100 2550 2300 2550
Wire Wire Line
	2900 1800 3100 1800
Connection ~ 3100 1800
Wire Wire Line
	3100 1800 3100 2550
Text GLabel 2900 2100 2    50   Input ~ 0
fpga_spi_out
Text GLabel 1700 2100 0    50   Input ~ 0
fpga_spi_in
Text GLabel 2900 2000 2    50   Input ~ 0
spi_clk
Text GLabel 2900 1900 2    50   Input ~ 0
spi_cs
$Comp
L power:GND #PWR?
U 1 1 5E610C07
P 2300 2850
AR Path="/5E610C07" Ref="#PWR?"  Part="1" 
AR Path="/5E5F2441/5E610C07" Ref="#PWR?"  Part="1" 
AR Path="/5E9ED365/5E610C07" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 2300 2600 50  0001 C CNN
F 1 "GND" H 2305 2677 50  0000 C CNN
F 2 "" H 2300 2850 50  0001 C CNN
F 3 "" H 2300 2850 50  0001 C CNN
	1    2300 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E610C0D
P 1950 1350
AR Path="/5E610C0D" Ref="#PWR?"  Part="1" 
AR Path="/5E5F2441/5E610C0D" Ref="#PWR?"  Part="1" 
AR Path="/5E9ED365/5E610C0D" Ref="#PWR01"  Part="1" 
F 0 "#PWR01" H 1950 1100 50  0001 C CNN
F 1 "GND" H 1955 1177 50  0000 C CNN
F 2 "" H 1950 1350 50  0001 C CNN
F 3 "" H 1950 1350 50  0001 C CNN
	1    1950 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1400 2300 1200
Wire Wire Line
	2300 1200 1950 1200
Wire Wire Line
	1950 1200 1950 1350
$Comp
L Device:R R?
U 1 1 5E62B581
P 5050 1850
AR Path="/5E62B581" Ref="R?"  Part="1" 
AR Path="/5E5F2441/5E62B581" Ref="R?"  Part="1" 
AR Path="/5E9ED365/5E62B581" Ref="R3"  Part="1" 
F 0 "R3" H 5120 1896 50  0000 L CNN
F 1 "10kOhm" H 5120 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4980 1850 50  0001 C CNN
F 3 "~" H 5050 1850 50  0001 C CNN
	1    5050 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E62B587
P 5250 1850
AR Path="/5E62B587" Ref="R?"  Part="1" 
AR Path="/5E5F2441/5E62B587" Ref="R?"  Part="1" 
AR Path="/5E9ED365/5E62B587" Ref="R4"  Part="1" 
F 0 "R4" H 5320 1896 50  0000 L CNN
F 1 "10kOhm" H 5320 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5180 1850 50  0001 C CNN
F 3 "~" H 5250 1850 50  0001 C CNN
	1    5250 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E62B58D
P 5400 1850
AR Path="/5E62B58D" Ref="R?"  Part="1" 
AR Path="/5E5F2441/5E62B58D" Ref="R?"  Part="1" 
AR Path="/5E9ED365/5E62B58D" Ref="R5"  Part="1" 
F 0 "R5" H 5470 1896 50  0000 L CNN
F 1 "10kOhm" H 5470 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5330 1850 50  0001 C CNN
F 3 "~" H 5400 1850 50  0001 C CNN
	1    5400 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E62B593
P 5550 1850
AR Path="/5E62B593" Ref="R?"  Part="1" 
AR Path="/5E5F2441/5E62B593" Ref="R?"  Part="1" 
AR Path="/5E9ED365/5E62B593" Ref="R6"  Part="1" 
F 0 "R6" H 5620 1896 50  0000 L CNN
F 1 "10kOhm" H 5620 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5480 1850 50  0001 C CNN
F 3 "~" H 5550 1850 50  0001 C CNN
	1    5550 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E62B599
P 4800 1850
AR Path="/5E62B599" Ref="R?"  Part="1" 
AR Path="/5E5F2441/5E62B599" Ref="R?"  Part="1" 
AR Path="/5E9ED365/5E62B599" Ref="R1"  Part="1" 
F 0 "R1" H 4870 1896 50  0000 L CNN
F 1 "10kOhm" H 4870 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4730 1850 50  0001 C CNN
F 3 "~" H 4800 1850 50  0001 C CNN
	1    4800 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E62B59F
P 4900 1850
AR Path="/5E62B59F" Ref="R?"  Part="1" 
AR Path="/5E5F2441/5E62B59F" Ref="R?"  Part="1" 
AR Path="/5E9ED365/5E62B59F" Ref="R2"  Part="1" 
F 0 "R2" H 4970 1896 50  0000 L CNN
F 1 "10kOhm" H 4970 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4830 1850 50  0001 C CNN
F 3 "~" H 4900 1850 50  0001 C CNN
	1    4900 1850
	1    0    0    -1  
$EndComp
Text GLabel 4600 2350 0    50   Input ~ 0
fpga_spi_in
Text GLabel 4600 2150 0    50   Input ~ 0
cdone
Text GLabel 4600 2250 0    50   Input ~ 0
creset
Text GLabel 5750 2350 2    50   Input ~ 0
fpga_spi_out
Text GLabel 5750 2250 2    50   Input ~ 0
spi_clk
Text GLabel 5750 2150 2    50   Input ~ 0
spi_cs
$Comp
L power:+3V3 #PWR?
U 1 1 5E62B5AB
P 5200 1450
AR Path="/5E62B5AB" Ref="#PWR?"  Part="1" 
AR Path="/5E5F2441/5E62B5AB" Ref="#PWR?"  Part="1" 
AR Path="/5E9ED365/5E62B5AB" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 5200 1300 50  0001 C CNN
F 1 "+3V3" H 5215 1623 50  0000 C CNN
F 2 "" H 5200 1450 50  0001 C CNN
F 3 "" H 5200 1450 50  0001 C CNN
	1    5200 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1700 4900 1700
Connection ~ 4900 1700
Wire Wire Line
	4900 1700 5050 1700
Connection ~ 5050 1700
Wire Wire Line
	5050 1700 5200 1700
Connection ~ 5250 1700
Wire Wire Line
	5250 1700 5400 1700
Connection ~ 5400 1700
Wire Wire Line
	5400 1700 5550 1700
Wire Wire Line
	5200 1450 5200 1700
Connection ~ 5200 1700
Wire Wire Line
	5200 1700 5250 1700
Wire Wire Line
	4600 2150 4800 2150
Wire Wire Line
	4800 2150 4800 2000
Wire Wire Line
	4600 2250 4900 2250
Wire Wire Line
	4900 2250 4900 2000
Wire Wire Line
	4600 2350 5050 2350
Wire Wire Line
	5050 2350 5050 2000
Wire Wire Line
	5250 2000 5250 2350
Wire Wire Line
	5250 2350 5750 2350
Wire Wire Line
	5400 2000 5400 2250
Wire Wire Line
	5400 2250 5750 2250
Wire Wire Line
	5550 2000 5550 2150
Wire Wire Line
	5550 2150 5750 2150
$EndSCHEMATC
