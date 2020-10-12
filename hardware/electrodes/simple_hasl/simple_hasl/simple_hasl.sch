EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector:Conn_01x04_Female J2
U 1 1 5F8507F7
P 3750 3450
F 0 "J2" H 3778 3426 50  0000 L CNN
F 1 "Conn_01x04_Female" H 3778 3335 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3750 3450 50  0001 C CNN
F 3 "~" H 3750 3450 50  0001 C CNN
	1    3750 3450
	1    0    0    -1  
$EndComp
$Comp
L electrolyser_electrodes:PCB_Electrode J1
U 1 1 5F851900
P 2750 3450
F 0 "J1" H 2668 3225 50  0000 C CNN
F 1 "PCB_Electrode" H 2668 3316 50  0000 C CNN
F 2 "electrodes:Electrode_PCB_V0.1" H 2750 3450 50  0001 C CNN
F 3 "~" H 2750 3450 50  0001 C CNN
	1    2750 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	2950 3450 3550 3450
Wire Wire Line
	3550 3350 3550 3450
Connection ~ 3550 3450
Wire Wire Line
	3550 3450 3550 3550
Connection ~ 3550 3550
Wire Wire Line
	3550 3550 3550 3650
$EndSCHEMATC
