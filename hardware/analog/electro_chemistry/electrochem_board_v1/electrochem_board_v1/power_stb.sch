EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 9
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
L Transistor_FET:FQP27P06 Q1
U 1 1 60A12E1C
P 3650 3600
F 0 "Q1" H 3854 3646 50  0000 L CNN
F 1 "FQP27P06" H 3854 3555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3850 3525 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/FQP27P06-D.PDF" H 3650 3600 50  0001 L CNN
	1    3650 3600
	1    0    0    -1  
$EndComp
Text GLabel 3250 2600 0    50   Input ~ 0
ps_ctl
Text GLabel 3000 3600 0    50   Input ~ 0
pwr_ctl
Wire Wire Line
	3000 3600 3450 3600
Wire Wire Line
	3250 2600 3750 2600
Wire Wire Line
	3750 2600 3750 3400
$Comp
L power:GND #PWR011
U 1 1 60A14963
P 3750 4100
F 0 "#PWR011" H 3750 3850 50  0001 C CNN
F 1 "GND" H 3755 3927 50  0000 C CNN
F 2 "" H 3750 4100 50  0001 C CNN
F 3 "" H 3750 4100 50  0001 C CNN
	1    3750 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3800 3750 4100
$EndSCHEMATC
