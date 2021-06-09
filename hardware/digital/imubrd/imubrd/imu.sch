EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L Sensor_Motion:ICM-20948 U?
U 1 1 60C0FE4D
P 5100 3300
F 0 "U?" H 5100 2511 50  0000 C CNN
F 1 "ICM-20948" H 5100 2420 50  0000 C CNN
F 2 "Sensor_Motion:InvenSense_QFN-24_3x3mm_P0.4mm" H 5100 2300 50  0001 C CNN
F 3 "http://www.invensense.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf" H 5100 3150 50  0001 C CNN
	1    5100 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60C10660
P 5050 4650
F 0 "#PWR?" H 5050 4400 50  0001 C CNN
F 1 "GND" H 5055 4477 50  0000 C CNN
F 2 "" H 5050 4650 50  0001 C CNN
F 3 "" H 5050 4650 50  0001 C CNN
	1    5050 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 4000 5100 4650
Wire Wire Line
	5100 4650 5050 4650
NoConn ~ 5600 3200
NoConn ~ 5600 3300
$Comp
L Device:C C?
U 1 1 60C10B6D
P 6200 3900
F 0 "C?" H 6315 3946 50  0000 L CNN
F 1 "C" H 6315 3855 50  0000 L CNN
F 2 "" H 6238 3750 50  0001 C CNN
F 3 "~" H 6200 3900 50  0001 C CNN
	1    6200 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3500 6200 3500
Wire Wire Line
	6200 3500 6200 3750
$Comp
L power:GND #PWR?
U 1 1 60C110BE
P 6200 4700
F 0 "#PWR?" H 6200 4450 50  0001 C CNN
F 1 "GND" H 6205 4527 50  0000 C CNN
F 2 "" H 6200 4700 50  0001 C CNN
F 3 "" H 6200 4700 50  0001 C CNN
	1    6200 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4050 6200 4700
$EndSCHEMATC
