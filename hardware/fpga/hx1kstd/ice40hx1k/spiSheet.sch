EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
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
AR Path="/5E5F2441/5E610BE8" Ref="U5"  Part="1" 
AR Path="/5E9ED365/5E610BE8" Ref="U5"  Part="1" 
F 0 "U5" H 2300 1219 50  0000 C CNN
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
AR Path="/5E5F2441/5E610BEE" Ref="C10"  Part="1" 
AR Path="/5E9ED365/5E610BEE" Ref="C10"  Part="1" 
F 0 "C10" H 2415 2746 50  0000 L CNN
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
AR Path="/5E5F2441/5E610BF4" Ref="#PWR013"  Part="1" 
AR Path="/5E9ED365/5E610BF4" Ref="#PWR013"  Part="1" 
F 0 "#PWR013" H 1950 2400 50  0001 C CNN
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
SPI_OUT
Text GLabel 1700 2100 0    50   Input ~ 0
SPI_IN
Text GLabel 2900 2000 2    50   Input ~ 0
SPI_SCK
Text GLabel 2900 1900 2    50   Input ~ 0
SPI_CS
$Comp
L power:GND #PWR?
U 1 1 5E610C07
P 2300 2850
AR Path="/5E610C07" Ref="#PWR?"  Part="1" 
AR Path="/5E5F2441/5E610C07" Ref="#PWR014"  Part="1" 
AR Path="/5E9ED365/5E610C07" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 2300 2600 50  0001 C CNN
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
AR Path="/5E5F2441/5E610C0D" Ref="#PWR012"  Part="1" 
AR Path="/5E9ED365/5E610C0D" Ref="#PWR012"  Part="1" 
F 0 "#PWR012" H 1950 1100 50  0001 C CNN
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
AR Path="/5E5F2441/5E62B581" Ref="R7"  Part="1" 
AR Path="/5E9ED365/5E62B581" Ref="R7"  Part="1" 
F 0 "R7" H 5120 1896 50  0000 L CNN
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
AR Path="/5E5F2441/5E62B587" Ref="R8"  Part="1" 
AR Path="/5E9ED365/5E62B587" Ref="R8"  Part="1" 
F 0 "R8" H 5320 1896 50  0000 L CNN
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
AR Path="/5E5F2441/5E62B58D" Ref="R9"  Part="1" 
AR Path="/5E9ED365/5E62B58D" Ref="R9"  Part="1" 
F 0 "R9" H 5470 1896 50  0000 L CNN
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
AR Path="/5E5F2441/5E62B593" Ref="R10"  Part="1" 
AR Path="/5E9ED365/5E62B593" Ref="R10"  Part="1" 
F 0 "R10" H 5620 1896 50  0000 L CNN
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
AR Path="/5E5F2441/5E62B599" Ref="R5"  Part="1" 
AR Path="/5E9ED365/5E62B599" Ref="R5"  Part="1" 
F 0 "R5" H 4870 1896 50  0000 L CNN
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
AR Path="/5E5F2441/5E62B59F" Ref="R6"  Part="1" 
AR Path="/5E9ED365/5E62B59F" Ref="R6"  Part="1" 
F 0 "R6" H 4970 1896 50  0000 L CNN
F 1 "10kOhm" H 4970 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4830 1850 50  0001 C CNN
F 3 "~" H 4900 1850 50  0001 C CNN
	1    4900 1850
	1    0    0    -1  
$EndComp
Text GLabel 4600 2350 0    50   Input ~ 0
SPI_IN
Text GLabel 4600 2150 0    50   Input ~ 0
CDONE
Text GLabel 4600 2250 0    50   Input ~ 0
CRESET
Text GLabel 5750 2350 2    50   Input ~ 0
SPI_OUT
Text GLabel 5750 2250 2    50   Input ~ 0
SPI_SCK
Text GLabel 5750 2150 2    50   Input ~ 0
SPI_CS
$Comp
L power:+3V3 #PWR?
U 1 1 5E62B5AB
P 5200 1450
AR Path="/5E62B5AB" Ref="#PWR?"  Part="1" 
AR Path="/5E5F2441/5E62B5AB" Ref="#PWR015"  Part="1" 
AR Path="/5E9ED365/5E62B5AB" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 5200 1300 50  0001 C CNN
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
Text GLabel 6250 4200 0    50   Input ~ 0
SPI_OUT
Text GLabel 6250 4300 0    50   Input ~ 0
SPI_SCK
Text GLabel 6250 4500 0    50   Input ~ 0
SPI_CS
Text GLabel 6250 4400 0    50   Input ~ 0
SPI_IN
Text GLabel 6250 4000 0    50   Input ~ 0
CDONE
Text GLabel 6250 4100 0    50   Input ~ 0
CRESET
$Comp
L power:+3V3 #PWR016
U 1 1 5E63A01C
P 4700 3700
F 0 "#PWR016" H 4700 3550 50  0001 C CNN
F 1 "+3V3" H 4715 3873 50  0000 C CNN
F 2 "" H 4700 3700 50  0001 C CNN
F 3 "" H 4700 3700 50  0001 C CNN
	1    4700 3700
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5E63A022
P 5250 3200
F 0 "#PWR017" H 5250 2950 50  0001 C CNN
F 1 "GND" H 5255 3027 50  0000 C CNN
F 2 "" H 5250 3200 50  0001 C CNN
F 3 "" H 5250 3200 50  0001 C CNN
	1    5250 3200
	1    0    0    1   
$EndComp
Wire Wire Line
	5250 3400 5250 3200
Wire Wire Line
	4700 3700 4700 3500
Wire Wire Line
	6250 3400 5250 3400
Wire Wire Line
	5250 3400 5250 3600
Wire Wire Line
	5250 3600 6250 3600
Connection ~ 5250 3400
Wire Wire Line
	6250 5300 5250 5300
Wire Wire Line
	5250 5300 5250 5000
Connection ~ 5250 3600
Wire Wire Line
	6250 5000 5250 5000
Connection ~ 5250 5000
Wire Wire Line
	5250 5000 5250 4600
Wire Wire Line
	6250 4600 5250 4600
Connection ~ 5250 4600
Wire Wire Line
	5250 4600 5250 3900
Wire Wire Line
	6250 3900 5250 3900
Connection ~ 5250 3900
Wire Wire Line
	5250 3900 5250 3600
Wire Wire Line
	6250 3500 4700 3500
Text GLabel 6250 3700 0    50   Input ~ 0
uart_tx
Text GLabel 6250 3800 0    50   Input ~ 0
uart_rx
NoConn ~ 6250 4900
$Comp
L Connector:Conn_01x20_Male J1
U 1 1 5EA4F382
P 6450 4400
F 0 "J1" H 6558 5481 50  0000 C CNN
F 1 "Conn_01x20_Male" H 6558 5390 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Vertical" H 6450 4400 50  0001 C CNN
F 3 "~" H 6450 4400 50  0001 C CNN
	1    6450 4400
	-1   0    0    1   
$EndComp
NoConn ~ 6250 4700
NoConn ~ 6250 4800
NoConn ~ 6250 5100
NoConn ~ 6250 5200
$EndSCHEMATC
