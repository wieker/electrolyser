EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 9
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
L Connector:ATX-24 J2
U 1 1 60970A62
P 3900 3300
F 0 "J2" H 3900 3967 50  0000 C CNN
F 1 "ATX-24" H 3900 3876 50  0000 C CNN
F 2 "Connector_Molex:Molex_Mini-Fit_Jr_5566-24A_2x12_P4.20mm_Vertical" H 3900 3200 50  0001 C CNN
F 3 "https://www.intel.com/content/dam/www/public/us/en/documents/guides/power-supply-design-guide-june.pdf#page=33" H 6300 2750 50  0001 C CNN
	1    3900 3300
	1    0    0    -1  
$EndComp
NoConn ~ 4400 3700
NoConn ~ 4400 3100
NoConn ~ 4400 3300
NoConn ~ 4400 3400
NoConn ~ 4400 2900
Text GLabel 3400 2900 0    50   Input ~ 0
ps_ctl
Text GLabel 4400 3200 2    50   Input ~ 0
5VPSU
$Comp
L power:GND #PWR03
U 1 1 60A0377E
P 3900 4200
F 0 "#PWR03" H 3900 3950 50  0001 C CNN
F 1 "GND" H 3905 4027 50  0000 C CNN
F 2 "" H 3900 4200 50  0001 C CNN
F 3 "" H 3900 4200 50  0001 C CNN
	1    3900 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3900 3900 4200
$EndSCHEMATC
