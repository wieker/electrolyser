EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
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
L Connector:Conn_01x20_Female J1
U 1 1 5E9E9075
P 2050 3500
F 0 "J1" H 1942 2275 50  0000 C CNN
F 1 "Conn_01x20_Female" H 1942 2366 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Horizontal" H 2050 3500 50  0001 C CNN
F 3 "~" H 2050 3500 50  0001 C CNN
	1    2050 3500
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E9ECCA0
P 3350 1800
AR Path="/5E9E6BE4/5E9ECCA0" Ref="#PWR?"  Part="1" 
AR Path="/5E9E8D09/5E9ECCA0" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 3350 1650 50  0001 C CNN
F 1 "+3V3" H 3365 1973 50  0000 C CNN
F 2 "" H 3350 1800 50  0001 C CNN
F 3 "" H 3350 1800 50  0001 C CNN
	1    3350 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5E9EE351
P 3000 4900
F 0 "#PWR03" H 3000 4650 50  0001 C CNN
F 1 "GND" H 3005 4727 50  0000 C CNN
F 2 "" H 3000 4900 50  0001 C CNN
F 3 "" H 3000 4900 50  0001 C CNN
	1    3000 4900
	1    0    0    -1  
$EndComp
Text GLabel 2250 2900 2    50   Input ~ 0
uart_tx
Text GLabel 2250 2800 2    50   Input ~ 0
uart_rx
Text GLabel 2250 3300 2    50   Input ~ 0
spi_mosi
Text GLabel 2250 3400 2    50   Input ~ 0
spi_sck
Text GLabel 2250 3500 2    50   Input ~ 0
spi_miso
Text GLabel 2250 3600 2    50   Input ~ 0
spi_ss
Text GLabel 2250 3200 2    50   Input ~ 0
fpga_reset
Text GLabel 2250 3100 2    50   Input ~ 0
fpga_cdone
Text GLabel 2250 3800 2    50   Input ~ 0
uart_fpga_tx_2
Text GLabel 2250 3900 2    50   Input ~ 0
uart_fpga_rx_2
Text GLabel 2250 4200 2    50   Input ~ 0
nrf_rx
Text GLabel 2250 4300 2    50   Input ~ 0
nrf_tx
$Comp
L Device:C C?
U 1 1 5E9F19A3
P 4600 2700
AR Path="/5E9E6BB8/5E9F19A3" Ref="C?"  Part="1" 
AR Path="/5E9E8D09/5E9F19A3" Ref="C4"  Part="1" 
F 0 "C4" H 4715 2746 50  0000 L CNN
F 1 "0.1uF" H 4715 2655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4638 2550 50  0001 C CNN
F 3 "~" H 4600 2700 50  0001 C CNN
	1    4600 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5E9F19A9
P 4050 2700
AR Path="/5E9E6BB8/5E9F19A9" Ref="C?"  Part="1" 
AR Path="/5E9E8D09/5E9F19A9" Ref="C2"  Part="1" 
F 0 "C2" H 4165 2746 50  0000 L CNN
F 1 "4.7uF" H 4165 2655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4088 2550 50  0001 C CNN
F 3 "~" H 4050 2700 50  0001 C CNN
	1    4050 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E9F19AF
P 4350 3350
AR Path="/5E9E6BB8/5E9F19AF" Ref="#PWR?"  Part="1" 
AR Path="/5E9E8D09/5E9F19AF" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 4350 3100 50  0001 C CNN
F 1 "GND" H 4355 3177 50  0000 C CNN
F 2 "" H 4350 3350 50  0001 C CNN
F 3 "" H 4350 3350 50  0001 C CNN
	1    4350 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E9F19B5
P 4350 2250
AR Path="/5E9E6BB8/5E9F19B5" Ref="#PWR?"  Part="1" 
AR Path="/5E9E8D09/5E9F19B5" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 4350 2100 50  0001 C CNN
F 1 "+3V3" H 4365 2423 50  0000 C CNN
F 2 "" H 4350 2250 50  0001 C CNN
F 3 "" H 4350 2250 50  0001 C CNN
	1    4350 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2850 4050 3150
Wire Wire Line
	4050 3150 4350 3150
Wire Wire Line
	4600 3150 4600 2850
Wire Wire Line
	4050 2550 4050 2400
Wire Wire Line
	4050 2400 4350 2400
Wire Wire Line
	4600 2400 4600 2550
Wire Wire Line
	4350 2250 4350 2400
Connection ~ 4350 2400
Wire Wire Line
	4350 2400 4600 2400
Wire Wire Line
	4350 3350 4350 3150
Connection ~ 4350 3150
Wire Wire Line
	4350 3150 4600 3150
Wire Wire Line
	2250 2500 3000 2500
Wire Wire Line
	3000 2500 3000 2700
Wire Wire Line
	2250 2600 3350 2600
Wire Wire Line
	3350 1800 3350 2600
Connection ~ 3000 2700
Wire Wire Line
	3000 2700 3000 3000
Wire Wire Line
	2250 2700 3000 2700
Wire Wire Line
	2250 4400 3000 4400
Connection ~ 3000 4400
Wire Wire Line
	3000 4400 3000 4900
Wire Wire Line
	2250 4100 3000 4100
Connection ~ 3000 4100
Wire Wire Line
	3000 4100 3000 4400
Wire Wire Line
	2250 3700 3000 3700
Connection ~ 3000 3700
Wire Wire Line
	3000 3700 3000 4100
NoConn ~ 2250 4000
Wire Wire Line
	2250 3000 3000 3000
Connection ~ 3000 3000
Wire Wire Line
	3000 3000 3000 3700
$EndSCHEMATC
