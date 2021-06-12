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
L power:GND #PWR04
U 1 1 60A10F93
P 6400 2750
F 0 "#PWR04" H 6400 2500 50  0001 C CNN
F 1 "GND" H 6405 2577 50  0000 C CNN
F 2 "" H 6400 2750 50  0001 C CNN
F 3 "" H 6400 2750 50  0001 C CNN
	1    6400 2750
	1    0    0    -1  
$EndComp
Text GLabel 5750 2050 2    50   Input ~ 0
Anode
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 60CBC0B8
P 5550 2050
F 0 "J3" H 5658 2231 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5658 2140 50  0000 C CNN
F 2 "Connector_Wire:SolderWire-1.5sqmm_1x02_P6mm_D1.7mm_OD3mm" H 5550 2050 50  0001 C CNN
F 3 "~" H 5550 2050 50  0001 C CNN
	1    5550 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 2150 6400 2150
Wire Wire Line
	6400 2150 6400 2750
$EndSCHEMATC
