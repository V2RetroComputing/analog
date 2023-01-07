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
Wire Wire Line
	800  3300 1550 3300
Wire Wire Line
	2650 3300 2050 3300
Text GLabel 2250 1700 2    50   Input ~ 0
D7
Text GLabel 2250 1600 2    50   Input ~ 0
D6
Text GLabel 2250 1500 2    50   Input ~ 0
D5
Text GLabel 2250 1400 2    50   Input ~ 0
D4
Text GLabel 2250 1300 2    50   Input ~ 0
D3
Text GLabel 2250 1200 2    50   Input ~ 0
D2
Text GLabel 2250 1100 2    50   Input ~ 0
D1
Text GLabel 2250 1000 2    50   Input ~ 0
D0
Text GLabel 1350 1700 0    50   Input ~ 0
A7
Text GLabel 1350 1600 0    50   Input ~ 0
A6
Text GLabel 1350 1500 0    50   Input ~ 0
A5
Text GLabel 1350 1400 0    50   Input ~ 0
A4
Text GLabel 1350 1300 0    50   Input ~ 0
A3
Text GLabel 1350 1200 0    50   Input ~ 0
A2
Text GLabel 1350 1100 0    50   Input ~ 0
A1
Text GLabel 1350 1000 0    50   Input ~ 0
A0
Text GLabel 1350 2600 0    50   Input ~ 0
nWR
Text GLabel 2250 1800 2    50   Input ~ 0
nDEVSEL
Text GLabel 2250 900  2    50   Input ~ 0
+12V
Text GLabel 2250 2600 2    50   Input ~ 0
-12V
Text GLabel 2250 3000 2    50   Input ~ 0
nNMI
Text GLabel 2250 2900 2    50   Input ~ 0
nIRQ
Text GLabel 1350 3000 0    50   Input ~ 0
nDMARQ
Text GLabel 1350 2800 0    50   Input ~ 0
nIOSTROBE
Text GLabel 1350 900  0    50   Input ~ 0
nIOSEL
Text GLabel 1350 1800 0    50   Input ~ 0
A8
Text GLabel 1350 1900 0    50   Input ~ 0
A9
Text GLabel 1350 2000 0    50   Input ~ 0
A10
Text GLabel 1350 2100 0    50   Input ~ 0
A11
Text GLabel 1350 2200 0    50   Input ~ 0
A12
Text GLabel 1350 2300 0    50   Input ~ 0
A13
Text GLabel 1350 2400 0    50   Input ~ 0
A14
Text GLabel 1350 2500 0    50   Input ~ 0
A15
Wire Wire Line
	2050 1500 2250 1500
Wire Wire Line
	2250 1600 2050 1600
Wire Wire Line
	2250 1700 2050 1700
Wire Wire Line
	1550 2400 1350 2400
Wire Wire Line
	1350 2500 1550 2500
Wire Wire Line
	2250 1400 2050 1400
Wire Wire Line
	1550 2100 1350 2100
Wire Wire Line
	1350 2200 1550 2200
Wire Wire Line
	1350 2300 1550 2300
Wire Wire Line
	1550 1800 1350 1800
Wire Wire Line
	1350 1900 1550 1900
Wire Wire Line
	1350 2000 1550 2000
Wire Wire Line
	1550 2800 1350 2800
Wire Wire Line
	1350 900  1550 900 
Wire Wire Line
	1550 3000 1350 3000
Wire Wire Line
	2250 3000 2050 3000
Wire Wire Line
	2250 2900 2050 2900
Wire Wire Line
	2050 2600 2250 2600
Wire Wire Line
	2250 1100 2050 1100
Wire Wire Line
	2050 1200 2250 1200
Wire Wire Line
	2050 1300 2250 1300
Wire Wire Line
	1350 1600 1550 1600
Wire Wire Line
	1550 1700 1350 1700
Wire Wire Line
	2050 1000 2250 1000
Wire Wire Line
	1350 1300 1550 1300
Wire Wire Line
	1550 1400 1350 1400
Wire Wire Line
	1550 1500 1350 1500
Wire Wire Line
	1350 1000 1550 1000
Wire Wire Line
	1550 1100 1350 1100
Wire Wire Line
	1550 1200 1350 1200
Wire Wire Line
	2050 1800 2250 1800
Wire Wire Line
	1550 2600 1350 2600
Wire Wire Line
	2250 900  2050 900 
$Comp
L power:GND #PWR02
U 1 1 6290839F
P 2650 3300
F 0 "#PWR02" H 2650 3050 50  0001 C CNN
F 1 "GND" H 2655 3127 50  0000 C CNN
F 2 "" H 2650 3300 50  0001 C CNN
F 3 "" H 2650 3300 50  0001 C CNN
	1    2650 3300
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 62908697
P 800 3300
F 0 "#PWR01" H 800 3150 50  0001 C CNN
F 1 "+5V" H 815 3473 50  0000 C CNN
F 2 "" H 800 3300 50  0001 C CNN
F 3 "" H 800 3300 50  0001 C CNN
	1    800  3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x25_Counter_Clockwise J1
U 1 1 629381EB
P 1750 2100
F 0 "J1" H 1800 3517 50  0000 C CNN
F 1 "Apple II" H 1800 3426 50  0000 C CNN
F 2 "STDBUS:AppleII" H 1750 2100 50  0001 C CNN
F 3 "~" H 1750 2100 50  0001 C CNN
	1    1750 2100
	1    0    0    -1  
$EndComp
Text GLabel 1350 2900 0    50   Input ~ 0
nRDY
Wire Wire Line
	1550 2900 1350 2900
Wire Wire Line
	1550 3100 2050 3100
Text GLabel 2250 2700 2    50   Input ~ 0
nROMINHIBIT
Wire Wire Line
	2050 2700 2250 2700
Text GLabel 2250 2500 2    50   Input ~ 0
-5V
Wire Wire Line
	2050 2500 2250 2500
NoConn ~ 2050 2400
Text GLabel 2250 2300 2    50   Input ~ 0
7MHz
Wire Wire Line
	2050 2300 2250 2300
Text GLabel 2250 2200 2    50   Input ~ 0
2MHz
Wire Wire Line
	2050 2200 2250 2200
Text GLabel 2250 2100 2    50   Input ~ 0
PHI1
Wire Wire Line
	2050 2100 2250 2100
Text GLabel 2250 1900 2    50   Input ~ 0
PHI0
Wire Wire Line
	2050 1900 2250 1900
Text GLabel 2250 2000 2    50   Input ~ 0
USER1
Wire Wire Line
	2050 2000 2250 2000
$Comp
L power:GND #PWR03
U 1 1 629F17F8
P 3550 2950
F 0 "#PWR03" H 3550 2700 50  0001 C CNN
F 1 "GND" H 3555 2777 50  0000 C CNN
F 2 "" H 3550 2950 50  0001 C CNN
F 3 "" H 3550 2950 50  0001 C CNN
	1    3550 2950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Counter_Clockwise U2
U 1 1 62A8607A
P 7250 1400
F 0 "U2" H 7300 2117 50  0000 C CNN
F 1 "SN74LVC8T245DGVR" H 7300 2026 50  0000 C CNN
F 2 "Package_SO:TSSOP-24_4.4x5mm_P0.4mm" H 7250 1400 50  0001 C CNN
F 3 "~" H 7250 1400 50  0001 C CNN
	1    7250 1400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Counter_Clockwise U1
U 1 1 62A87454
P 7250 3550
F 0 "U1" H 7300 4267 50  0000 C CNN
F 1 "SN74LVC8T245DGVR" H 7300 4176 50  0000 C CNN
F 2 "Package_SO:TSSOP-24_4.4x5mm_P0.4mm" H 7250 3550 50  0001 C CNN
F 3 "~" H 7250 3550 50  0001 C CNN
	1    7250 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Counter_Clockwise U3
U 1 1 62A887EA
P 9200 1450
F 0 "U3" H 9250 2167 50  0000 C CNN
F 1 "SN74LVC8T245DGVR" H 9250 2076 50  0000 C CNN
F 2 "Package_SO:TSSOP-24_4.4x5mm_P0.4mm" H 9200 1450 50  0001 C CNN
F 3 "~" H 9200 1450 50  0001 C CNN
	1    9200 1450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Counter_Clockwise U4
U 1 1 62A898B8
P 9200 3550
F 0 "U4" H 9250 4267 50  0000 C CNN
F 1 "SN74LVC8T245DGVR" H 9250 4176 50  0000 C CNN
F 2 "Package_SO:TSSOP-24_4.4x5mm_P0.4mm" H 9200 3550 50  0001 C CNN
F 3 "~" H 9200 3550 50  0001 C CNN
	1    9200 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 900  7050 900 
$Comp
L power:+5V #PWR015
U 1 1 62A8AF5B
P 7900 900
F 0 "#PWR015" H 7900 750 50  0001 C CNN
F 1 "+5V" H 7915 1073 50  0000 C CNN
F 2 "" H 7900 900 50  0001 C CNN
F 3 "" H 7900 900 50  0001 C CNN
	1    7900 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3050 7050 3050
$Comp
L power:+5V #PWR016
U 1 1 62A8E96D
P 7950 3050
F 0 "#PWR016" H 7950 2900 50  0001 C CNN
F 1 "+5V" H 7965 3223 50  0000 C CNN
F 2 "" H 7950 3050 50  0001 C CNN
F 3 "" H 7950 3050 50  0001 C CNN
	1    7950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 950  9000 950 
$Comp
L power:+5V #PWR023
U 1 1 62A9282A
P 9850 950
F 0 "#PWR023" H 9850 800 50  0001 C CNN
F 1 "+5V" H 9865 1123 50  0000 C CNN
F 2 "" H 9850 950 50  0001 C CNN
F 3 "" H 9850 950 50  0001 C CNN
	1    9850 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 3050 9000 3050
$Comp
L power:+5V #PWR024
U 1 1 62A964E5
P 9850 3050
F 0 "#PWR024" H 9850 2900 50  0001 C CNN
F 1 "+5V" H 9865 3223 50  0000 C CNN
F 2 "" H 9850 3050 50  0001 C CNN
F 3 "" H 9850 3050 50  0001 C CNN
	1    9850 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 62A9A430
P 7300 4300
F 0 "#PWR014" H 7300 4050 50  0001 C CNN
F 1 "GND" H 7305 4127 50  0000 C CNN
F 2 "" H 7300 4300 50  0001 C CNN
F 3 "" H 7300 4300 50  0001 C CNN
	1    7300 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4150 7000 4150
Wire Wire Line
	7000 4150 7000 4300
Wire Wire Line
	7000 4300 7300 4300
Wire Wire Line
	7550 4150 7600 4150
Wire Wire Line
	7600 4150 7600 4300
Wire Wire Line
	7600 4300 7300 4300
Connection ~ 7300 4300
$Comp
L power:GND #PWR021
U 1 1 62AB32C7
P 9250 4300
F 0 "#PWR021" H 9250 4050 50  0001 C CNN
F 1 "GND" H 9255 4127 50  0000 C CNN
F 2 "" H 9250 4300 50  0001 C CNN
F 3 "" H 9250 4300 50  0001 C CNN
	1    9250 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4150 8950 4150
Wire Wire Line
	8950 4150 8950 4300
Wire Wire Line
	8950 4300 9250 4300
Wire Wire Line
	9500 4150 9550 4150
Wire Wire Line
	9550 4150 9550 4300
Wire Wire Line
	9550 4300 9250 4300
Connection ~ 9250 4300
$Comp
L power:GND #PWR020
U 1 1 62AC5749
P 9250 2200
F 0 "#PWR020" H 9250 1950 50  0001 C CNN
F 1 "GND" H 9255 2027 50  0000 C CNN
F 2 "" H 9250 2200 50  0001 C CNN
F 3 "" H 9250 2200 50  0001 C CNN
	1    9250 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2050 8950 2050
Wire Wire Line
	8950 2050 8950 2200
Wire Wire Line
	8950 2200 9250 2200
Wire Wire Line
	9500 2050 9550 2050
Wire Wire Line
	9550 2050 9550 2200
Wire Wire Line
	9550 2200 9250 2200
Connection ~ 9250 2200
$Comp
L power:GND #PWR013
U 1 1 62ACA613
P 7300 2150
F 0 "#PWR013" H 7300 1900 50  0001 C CNN
F 1 "GND" H 7305 1977 50  0000 C CNN
F 2 "" H 7300 2150 50  0001 C CNN
F 3 "" H 7300 2150 50  0001 C CNN
	1    7300 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 2000 7000 2000
Wire Wire Line
	7000 2000 7000 2150
Wire Wire Line
	7000 2150 7300 2150
Wire Wire Line
	7550 2000 7600 2000
Wire Wire Line
	7600 2000 7600 2150
Wire Wire Line
	7600 2150 7300 2150
Connection ~ 7300 2150
Text GLabel 6850 1500 0    50   Input ~ 0
LD3
Text GLabel 6850 1600 0    50   Input ~ 0
LD2
Text GLabel 6850 1700 0    50   Input ~ 0
LD1
Text GLabel 6850 1800 0    50   Input ~ 0
LD0
Wire Wire Line
	7750 1300 7550 1300
Wire Wire Line
	7550 1400 7750 1400
Wire Wire Line
	7550 1500 7750 1500
Wire Wire Line
	7550 1200 7750 1200
Text GLabel 6850 1300 0    50   Input ~ 0
LD5
Text GLabel 6850 1400 0    50   Input ~ 0
LD4
Wire Wire Line
	7550 1700 7750 1700
Wire Wire Line
	7750 1600 7550 1600
Text GLabel 6850 1100 0    50   Input ~ 0
LD7
Text GLabel 6850 1200 0    50   Input ~ 0
LD6
Wire Wire Line
	7750 1800 7550 1800
Wire Wire Line
	7750 1900 7550 1900
Text GLabel 7750 1200 2    50   Input ~ 0
D7
Text GLabel 7750 1300 2    50   Input ~ 0
D6
Text GLabel 7750 1400 2    50   Input ~ 0
D5
Text GLabel 7750 1500 2    50   Input ~ 0
D4
Text GLabel 7750 1600 2    50   Input ~ 0
D3
Text GLabel 7750 1700 2    50   Input ~ 0
D2
Text GLabel 7750 1800 2    50   Input ~ 0
D1
Text GLabel 7750 1900 2    50   Input ~ 0
D0
Wire Wire Line
	7050 1600 6850 1600
Wire Wire Line
	6850 1700 7050 1700
Wire Wire Line
	6850 1800 7050 1800
Wire Wire Line
	6850 1500 7050 1500
Wire Wire Line
	6850 1200 7050 1200
Wire Wire Line
	7050 1300 6850 1300
Wire Wire Line
	7050 1400 6850 1400
Wire Wire Line
	7050 1100 6850 1100
Wire Wire Line
	7050 1900 7000 1900
Wire Wire Line
	7000 1900 7000 2000
Connection ~ 7000 2000
Text GLabel 7750 1100 2    50   Input ~ 0
nDATAOE
Wire Wire Line
	7550 1100 7750 1100
Wire Wire Line
	9000 1950 8950 1950
Wire Wire Line
	8950 1950 8950 2050
Connection ~ 8950 2050
Wire Wire Line
	9000 4050 8950 4050
Wire Wire Line
	8950 4050 8950 4150
Connection ~ 8950 4150
Wire Wire Line
	7050 4050 7000 4050
Wire Wire Line
	7000 4050 7000 4150
Connection ~ 7000 4150
$Comp
L Device:R R13
U 1 1 62B78737
P 5750 4350
F 0 "R13" V 5543 4350 50  0001 C CNN
F 1 "10K" V 5750 4350 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5680 4350 50  0001 C CNN
F 3 "~" H 5750 4350 50  0001 C CNN
	1    5750 4350
	-1   0    0    1   
$EndComp
Wire Wire Line
	7550 3250 7750 3250
Wire Wire Line
	9500 1150 9700 1150
$Comp
L Device:R R14
U 1 1 62B8B68F
P 5950 4350
F 0 "R14" V 5743 4350 50  0001 C CNN
F 1 "10K" V 5950 4350 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5880 4350 50  0001 C CNN
F 3 "~" H 5950 4350 50  0001 C CNN
	1    5950 4350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R12
U 1 1 62B95325
P 5550 4350
F 0 "R12" V 5343 4350 50  0001 C CNN
F 1 "10K" V 5550 4350 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 5480 4350 50  0001 C CNN
F 3 "~" H 5550 4350 50  0001 C CNN
	1    5550 4350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R9
U 1 1 62BA8CC7
P 4100 5250
F 0 "R9" V 3893 5250 50  0001 C CNN
F 1 "2K" V 4100 5250 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 5250 50  0001 C CNN
F 3 "~" H 4100 5250 50  0001 C CNN
	1    4100 5250
	0    -1   -1   0   
$EndComp
Text GLabel 7750 3350 2    50   Input ~ 0
A7
Text GLabel 7750 3450 2    50   Input ~ 0
A6
Text GLabel 7750 3550 2    50   Input ~ 0
A5
Text GLabel 7750 3650 2    50   Input ~ 0
A4
Text GLabel 7750 3750 2    50   Input ~ 0
A3
Text GLabel 7750 3850 2    50   Input ~ 0
A2
Text GLabel 7750 3950 2    50   Input ~ 0
A1
Text GLabel 7750 4050 2    50   Input ~ 0
A0
Text GLabel 9700 1950 2    50   Input ~ 0
A8
Text GLabel 9700 1850 2    50   Input ~ 0
A9
Text GLabel 9700 1750 2    50   Input ~ 0
A10
Text GLabel 9700 1650 2    50   Input ~ 0
A11
Text GLabel 9700 1550 2    50   Input ~ 0
A12
Text GLabel 9700 1450 2    50   Input ~ 0
A13
Text GLabel 9700 1350 2    50   Input ~ 0
A14
Text GLabel 9700 1250 2    50   Input ~ 0
A15
Wire Wire Line
	9000 1750 8800 1750
Wire Wire Line
	8800 1850 9000 1850
Wire Wire Line
	9000 1450 8800 1450
Wire Wire Line
	8800 1550 9000 1550
Wire Wire Line
	8800 1650 9000 1650
Wire Wire Line
	9000 1150 8800 1150
Wire Wire Line
	8800 1250 9000 1250
Wire Wire Line
	8800 1350 9000 1350
Wire Wire Line
	7750 3450 7550 3450
Wire Wire Line
	7550 3350 7750 3350
Wire Wire Line
	7750 3750 7550 3750
Wire Wire Line
	7550 3650 7750 3650
Wire Wire Line
	7550 3550 7750 3550
Wire Wire Line
	7750 4050 7550 4050
Wire Wire Line
	7550 3950 7750 3950
Wire Wire Line
	7550 3850 7750 3850
Wire Wire Line
	6850 3750 7050 3750
Wire Wire Line
	7050 3650 6850 3650
Wire Wire Line
	7050 3550 6850 3550
Wire Wire Line
	7050 3850 6850 3850
Wire Wire Line
	7050 3450 6850 3450
Wire Wire Line
	9700 1750 9500 1750
Wire Wire Line
	9500 1850 9700 1850
Wire Wire Line
	9500 1950 9700 1950
Wire Wire Line
	9500 1650 9700 1650
Wire Wire Line
	9700 1350 9500 1350
Wire Wire Line
	9500 1450 9700 1450
Wire Wire Line
	9500 1250 9700 1250
Wire Wire Line
	6850 3950 7050 3950
Wire Wire Line
	6850 3250 7050 3250
Wire Wire Line
	9500 1550 9700 1550
Wire Wire Line
	6850 3350 7050 3350
Text GLabel 8800 3750 0    50   Input ~ 0
LR\W
Wire Wire Line
	9000 3850 8800 3850
Text GLabel 8800 3850 0    50   Input ~ 0
LPHI0
Wire Wire Line
	9000 3750 8800 3750
Text GLabel 8800 3650 0    50   Input ~ 0
nLRESET
Wire Wire Line
	9000 3950 8800 3950
Text GLabel 6850 1000 0    50   Input ~ 0
nDATADIR
Wire Wire Line
	7050 1000 6850 1000
Text GLabel 9700 3850 2    50   Input ~ 0
nWR
Wire Wire Line
	9500 3950 9700 3950
Text GLabel 9700 3950 2    50   Input ~ 0
PHI0
Wire Wire Line
	9500 3850 9700 3850
Text GLabel 9700 3750 2    50   Input ~ 0
nSYSRESET
Wire Wire Line
	9500 4050 9700 4050
Wire Wire Line
	3900 1400 4100 1400
Wire Wire Line
	4100 1500 3900 1500
Text GLabel 3900 1400 0    50   Input ~ 0
LD4
Text GLabel 3900 1500 0    50   Input ~ 0
LD5
Wire Wire Line
	4100 900  3900 900 
Wire Wire Line
	4100 1300 3900 1300
Wire Wire Line
	4100 1200 3900 1200
Wire Wire Line
	3900 1000 4100 1000
Text GLabel 3900 900  0    50   Input ~ 0
LD0
Text GLabel 3900 1000 0    50   Input ~ 0
LD1
Text GLabel 3900 1200 0    50   Input ~ 0
LD2
Text GLabel 3900 1300 0    50   Input ~ 0
LD3
Wire Wire Line
	3900 1800 4100 1800
Text GLabel 3900 1700 0    50   Input ~ 0
LD6
Text GLabel 3900 1800 0    50   Input ~ 0
LD7
$Comp
L Device:C C8
U 1 1 637DE45E
P 6350 6200
F 0 "C8" H 6465 6246 50  0000 L CNN
F 1 "C" H 6465 6155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6388 6050 50  0001 C CNN
F 3 "~" H 6350 6200 50  0001 C CNN
	1    6350 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 637DEEB3
P 6050 6200
F 0 "C6" H 6165 6246 50  0000 L CNN
F 1 "C" H 6165 6155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6088 6050 50  0001 C CNN
F 3 "~" H 6050 6200 50  0001 C CNN
	1    6050 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 637DF0C7
P 5750 6200
F 0 "C4" H 5865 6246 50  0000 L CNN
F 1 "C" H 5865 6155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5788 6050 50  0001 C CNN
F 3 "~" H 5750 6200 50  0001 C CNN
	1    5750 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 637DF358
P 5450 6200
F 0 "C2" H 5565 6246 50  0000 L CNN
F 1 "C" H 5565 6155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5488 6050 50  0001 C CNN
F 3 "~" H 5450 6200 50  0001 C CNN
	1    5450 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 6382C81F
P 6300 7250
F 0 "C7" H 6415 7296 50  0000 L CNN
F 1 "C" H 6415 7205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6338 7100 50  0001 C CNN
F 3 "~" H 6300 7250 50  0001 C CNN
	1    6300 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 6382C825
P 6000 7250
F 0 "C5" H 6115 7296 50  0000 L CNN
F 1 "C" H 6115 7205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6038 7100 50  0001 C CNN
F 3 "~" H 6000 7250 50  0001 C CNN
	1    6000 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 6382C82B
P 5700 7250
F 0 "C3" H 5815 7296 50  0000 L CNN
F 1 "C" H 5815 7205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5738 7100 50  0001 C CNN
F 3 "~" H 5700 7250 50  0001 C CNN
	1    5700 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 6382C831
P 5400 7250
F 0 "C1" H 5515 7296 50  0000 L CNN
F 1 "C" H 5515 7205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 5438 7100 50  0001 C CNN
F 3 "~" H 5400 7250 50  0001 C CNN
	1    5400 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 6382C837
P 6600 7250
F 0 "C9" H 6715 7296 50  0000 L CNN
F 1 "C" H 6715 7205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric_Pad0.74x0.62mm_HandSolder" H 6638 7100 50  0001 C CNN
F 3 "~" H 6600 7250 50  0001 C CNN
	1    6600 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 638413B0
P 6000 7500
F 0 "#PWR08" H 6000 7250 50  0001 C CNN
F 1 "GND" H 6005 7327 50  0000 C CNN
F 2 "" H 6000 7500 50  0001 C CNN
F 3 "" H 6000 7500 50  0001 C CNN
	1    6000 7500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 63841887
P 6000 7000
F 0 "#PWR07" H 6000 6850 50  0001 C CNN
F 1 "+5V" H 6015 7173 50  0000 C CNN
F 2 "" H 6000 7000 50  0001 C CNN
F 3 "" H 6000 7000 50  0001 C CNN
	1    6000 7000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 63841F61
P 6050 6450
F 0 "#PWR010" H 6050 6200 50  0001 C CNN
F 1 "GND" H 6055 6277 50  0000 C CNN
F 2 "" H 6050 6450 50  0001 C CNN
F 3 "" H 6050 6450 50  0001 C CNN
	1    6050 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 6350 5450 6400
Wire Wire Line
	5450 6400 5750 6400
Wire Wire Line
	6050 6400 6050 6450
Connection ~ 6050 6400
Wire Wire Line
	6050 6350 6050 6400
Wire Wire Line
	6350 6350 6350 6400
Wire Wire Line
	6350 6400 6050 6400
Wire Wire Line
	5750 6350 5750 6400
Connection ~ 5750 6400
Wire Wire Line
	5750 6400 6050 6400
Wire Wire Line
	5450 6050 5450 6000
Wire Wire Line
	5450 6000 5750 6000
Wire Wire Line
	6350 6050 6350 6000
Wire Wire Line
	6050 6050 6050 6000
Connection ~ 6050 6000
Wire Wire Line
	6050 6000 6350 6000
Wire Wire Line
	5750 6050 5750 6000
Connection ~ 5750 6000
Wire Wire Line
	5750 6000 6050 6000
Wire Wire Line
	6050 6000 6050 5950
Wire Wire Line
	5400 7100 5400 7050
Wire Wire Line
	5400 7050 5700 7050
Wire Wire Line
	6600 7050 6600 7100
Wire Wire Line
	6300 7100 6300 7050
Connection ~ 6300 7050
Wire Wire Line
	6300 7050 6600 7050
Wire Wire Line
	6000 7100 6000 7050
Connection ~ 6000 7050
Wire Wire Line
	6000 7050 6300 7050
Wire Wire Line
	5700 7100 5700 7050
Connection ~ 5700 7050
Wire Wire Line
	5700 7050 6000 7050
Wire Wire Line
	6000 7050 6000 7000
Wire Wire Line
	5400 7400 5400 7450
Wire Wire Line
	5400 7450 5700 7450
Wire Wire Line
	6600 7450 6600 7400
Wire Wire Line
	6300 7400 6300 7450
Connection ~ 6300 7450
Wire Wire Line
	6300 7450 6600 7450
Wire Wire Line
	6000 7400 6000 7450
Connection ~ 6000 7450
Wire Wire Line
	6000 7450 6300 7450
Wire Wire Line
	5700 7400 5700 7450
Connection ~ 5700 7450
Wire Wire Line
	5700 7450 6000 7450
Wire Wire Line
	6000 7450 6000 7500
Wire Wire Line
	3900 1700 4100 1700
$Comp
L power:+3.3V #PWR011
U 1 1 642C1A40
P 6650 900
F 0 "#PWR011" H 6650 750 50  0001 C CNN
F 1 "+3.3V" H 6665 1073 50  0000 C CNN
F 2 "" H 6650 900 50  0001 C CNN
F 3 "" H 6650 900 50  0001 C CNN
	1    6650 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 1000 7900 1000
Wire Wire Line
	7550 900  7900 900 
Wire Wire Line
	7900 1000 7900 900 
Connection ~ 7900 900 
$Comp
L power:+3.3V #PWR017
U 1 1 64372FE7
P 8650 950
F 0 "#PWR017" H 8650 800 50  0001 C CNN
F 1 "+3.3V" H 8665 1123 50  0000 C CNN
F 2 "" H 8650 950 50  0001 C CNN
F 3 "" H 8650 950 50  0001 C CNN
	1    8650 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 1050 9850 1050
Wire Wire Line
	9500 950  9850 950 
Wire Wire Line
	9850 1050 9850 950 
Connection ~ 9850 950 
$Comp
L power:+3.3V #PWR018
U 1 1 643DD1EE
P 8650 3050
F 0 "#PWR018" H 8650 2900 50  0001 C CNN
F 1 "+3.3V" H 8665 3223 50  0000 C CNN
F 2 "" H 8650 3050 50  0001 C CNN
F 3 "" H 8650 3050 50  0001 C CNN
	1    8650 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 3150 9850 3150
Wire Wire Line
	9500 3050 9850 3050
Wire Wire Line
	9850 3150 9850 3050
Connection ~ 9850 3050
$Comp
L power:+3.3V #PWR012
U 1 1 64423762
P 6700 3050
F 0 "#PWR012" H 6700 2900 50  0001 C CNN
F 1 "+3.3V" H 6715 3223 50  0000 C CNN
F 2 "" H 6700 3050 50  0001 C CNN
F 3 "" H 6700 3050 50  0001 C CNN
	1    6700 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 3150 7950 3150
Wire Wire Line
	7550 3050 7950 3050
Wire Wire Line
	7950 3150 7950 3050
Connection ~ 7950 3050
NoConn ~ 1550 2700
$Comp
L power:+3.3V #PWR09
U 1 1 62A2195F
P 6050 5950
F 0 "#PWR09" H 6050 5800 50  0001 C CNN
F 1 "+3.3V" H 6065 6123 50  0000 C CNN
F 2 "" H 6050 5950 50  0001 C CNN
F 3 "" H 6050 5950 50  0001 C CNN
	1    6050 5950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x20_Counter_Clockwise U6
U 1 1 63A68952
P 4300 1800
F 0 "U6" H 4350 2917 50  0000 C CNN
F 1 "Pi PICO" H 4350 2826 50  0000 C CNN
F 2 "Package_DIP:DIP-40_W16.51mm_SMDSocket_LongPads" H 4300 1800 50  0001 C CNN
F 3 "~" H 4300 1800 50  0001 C CNN
	1    4300 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 63AB0B97
P 8850 6050
F 0 "#PWR019" H 8850 5800 50  0001 C CNN
F 1 "GND" H 8855 5877 50  0000 C CNN
F 2 "" H 8850 6050 50  0001 C CNN
F 3 "" H 8850 6050 50  0001 C CNN
	1    8850 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 6050 8850 5900
Wire Wire Line
	8850 5900 9000 5900
Wire Wire Line
	9650 4850 9650 5000
Wire Wire Line
	9650 5000 9500 5000
Text GLabel 4850 2000 2    50   Input ~ 0
R0
Text GLabel 4850 2200 2    50   Input ~ 0
R1
Text GLabel 4850 2300 2    50   Input ~ 0
R2
Text GLabel 4850 2400 2    50   Input ~ 0
G0
Text GLabel 4850 2500 2    50   Input ~ 0
G1
Text GLabel 4850 2700 2    50   Input ~ 0
G2
Text GLabel 4850 2800 2    50   Input ~ 0
B0
Text GLabel 3850 2800 0    50   Input ~ 0
B1
Text GLabel 3850 2700 0    50   Input ~ 0
B2
Wire Wire Line
	3850 2700 4100 2700
Wire Wire Line
	4100 2800 3850 2800
Wire Wire Line
	4850 2800 4600 2800
Wire Wire Line
	4850 2700 4600 2700
Wire Wire Line
	4600 2500 4850 2500
Wire Wire Line
	4850 2400 4600 2400
Wire Wire Line
	4600 2300 4850 2300
Wire Wire Line
	4850 2200 4600 2200
Wire Wire Line
	4600 2000 4850 2000
Text GLabel 3700 4450 0    50   Input ~ 0
R0
Text GLabel 3700 4550 0    50   Input ~ 0
R1
Text GLabel 3700 4650 0    50   Input ~ 0
R2
Text GLabel 3700 4750 0    50   Input ~ 0
G0
Text GLabel 3700 4850 0    50   Input ~ 0
G1
Text GLabel 3700 4950 0    50   Input ~ 0
G2
Text GLabel 3700 5050 0    50   Input ~ 0
B0
Text GLabel 3700 5150 0    50   Input ~ 0
B1
Text GLabel 3700 5250 0    50   Input ~ 0
B2
Wire Wire Line
	3700 5250 3950 5250
Wire Wire Line
	3950 5150 3700 5150
Wire Wire Line
	3700 5050 3950 5050
Wire Wire Line
	3700 4950 3950 4950
Wire Wire Line
	3950 4850 3700 4850
Wire Wire Line
	3700 4750 3950 4750
Wire Wire Line
	3950 4650 3700 4650
Wire Wire Line
	3700 4550 3950 4550
Wire Wire Line
	3950 4450 3700 4450
Text GLabel 9700 1150 2    50   Input ~ 0
nMSBOE
Text GLabel 7750 3250 2    50   Input ~ 0
nLSBOE
Wire Wire Line
	9500 3250 9550 3250
Wire Wire Line
	9000 1050 8950 1050
Wire Wire Line
	4100 1100 3550 1100
Wire Wire Line
	3550 1100 3550 1600
Wire Wire Line
	3550 2600 4100 2600
Wire Wire Line
	4100 2100 3550 2100
Connection ~ 3550 2100
Wire Wire Line
	3550 2100 3550 2600
Wire Wire Line
	4100 1600 3550 1600
Connection ~ 3550 1600
Wire Wire Line
	3550 1600 3550 2100
Wire Wire Line
	4600 2600 5150 2600
Wire Wire Line
	5150 2600 5150 2100
Wire Wire Line
	5150 1100 4600 1100
Wire Wire Line
	4600 1600 5150 1600
Connection ~ 5150 1600
Wire Wire Line
	5150 1600 5150 1100
Wire Wire Line
	4600 2100 5150 2100
Connection ~ 5150 2100
Wire Wire Line
	5150 2100 5150 1600
Wire Wire Line
	4100 2200 4000 2200
Text GLabel 8800 5400 0    50   Input ~ 0
nDATAOE
Wire Wire Line
	4100 2300 4000 2300
Wire Wire Line
	4100 2500 4000 2500
Text GLabel 8800 5200 0    50   Input ~ 0
nMSBOE
Wire Wire Line
	4100 2400 4000 2400
Text GLabel 8800 5300 0    50   Input ~ 0
nLSBOE
Wire Wire Line
	9000 3150 8950 3150
Connection ~ 8950 4050
Wire Wire Line
	9550 3250 9550 3350
Connection ~ 9550 4150
Wire Wire Line
	7050 3150 7000 3150
Wire Wire Line
	7000 3150 7000 4050
Connection ~ 7000 4050
Wire Wire Line
	8950 1050 8950 1950
Connection ~ 8950 1950
Text GLabel 3900 2000 0    50   Input ~ 0
LR\W
Wire Wire Line
	4100 1900 3900 1900
Text GLabel 4800 1800 2    50   Input ~ 0
LPHI0
Wire Wire Line
	4600 1800 4800 1800
Wire Wire Line
	4100 2000 3900 2000
$Comp
L power:GND #PWR05
U 1 1 64A0F496
P 5150 2950
F 0 "#PWR05" H 5150 2700 50  0001 C CNN
F 1 "GND" H 5155 2777 50  0000 C CNN
F 2 "" H 5150 2950 50  0001 C CNN
F 3 "" H 5150 2950 50  0001 C CNN
	1    5150 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D1
U 1 1 64A2A017
P 4950 1000
F 0 "D1" H 4950 1125 50  0000 C CNN
F 1 "D_Schottky" H 4950 1126 50  0001 C CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 4950 1000 50  0001 C CNN
F 3 "~" H 4950 1000 50  0001 C CNN
	1    4950 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1000 4600 1000
Wire Wire Line
	5100 1000 5150 1000
Wire Wire Line
	5150 1000 5150 800 
Wire Wire Line
	5150 2600 5150 2950
Connection ~ 5150 2600
Wire Wire Line
	3550 2600 3550 2950
Connection ~ 3550 2600
$Comp
L Device:R R10
U 1 1 64AAFE41
P 4900 1500
F 0 "R10" V 4693 1500 50  0001 C CNN
F 1 "47" V 4900 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4830 1500 50  0001 C CNN
F 3 "~" H 4900 1500 50  0001 C CNN
	1    4900 1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4600 1700 4750 1700
Wire Wire Line
	4600 1500 4750 1500
Text GLabel 5550 4600 3    50   Input ~ 0
nDATAOE
Text GLabel 5750 4600 3    50   Input ~ 0
nMSBOE
Text GLabel 5950 4600 3    50   Input ~ 0
nLSBOE
$Comp
L Device:R R8
U 1 1 64AE851E
P 4100 5150
F 0 "R8" V 3893 5150 50  0001 C CNN
F 1 "1K" V 4100 5150 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 5150 50  0001 C CNN
F 3 "~" H 4100 5150 50  0001 C CNN
	1    4100 5150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 64AE8658
P 4100 5050
F 0 "R7" V 3893 5050 50  0001 C CNN
F 1 "470" V 4100 5050 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 5050 50  0001 C CNN
F 3 "~" H 4100 5050 50  0001 C CNN
	1    4100 5050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R6
U 1 1 64AE890E
P 4100 4950
F 0 "R6" V 3893 4950 50  0001 C CNN
F 1 "2K" V 4100 4950 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 4950 50  0001 C CNN
F 3 "~" H 4100 4950 50  0001 C CNN
	1    4100 4950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R5
U 1 1 64AE8BE2
P 4100 4850
F 0 "R5" V 3893 4850 50  0001 C CNN
F 1 "1K" V 4100 4850 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 4850 50  0001 C CNN
F 3 "~" H 4100 4850 50  0001 C CNN
	1    4100 4850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 64AE8D47
P 4100 4750
F 0 "R4" V 3893 4750 50  0001 C CNN
F 1 "470" V 4100 4750 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 4750 50  0001 C CNN
F 3 "~" H 4100 4750 50  0001 C CNN
	1    4100 4750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 64AE8EE1
P 4100 4650
F 0 "R3" V 3893 4650 50  0001 C CNN
F 1 "2K" V 4100 4650 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 4650 50  0001 C CNN
F 3 "~" H 4100 4650 50  0001 C CNN
	1    4100 4650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 64AE9127
P 4100 4550
F 0 "R2" V 3893 4550 50  0001 C CNN
F 1 "1K" V 4100 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 4550 50  0001 C CNN
F 3 "~" H 4100 4550 50  0001 C CNN
	1    4100 4550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 64AE92CF
P 4100 4450
F 0 "R1" V 3893 4450 50  0001 C CNN
F 1 "470" V 4100 4450 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4030 4450 50  0001 C CNN
F 3 "~" H 4100 4450 50  0001 C CNN
	1    4100 4450
	0    -1   -1   0   
$EndComp
Text GLabel 4550 4450 2    50   Input ~ 0
RED_OUT
Text GLabel 4550 4750 2    50   Input ~ 0
GREEN_OUT
Text GLabel 4550 5050 2    50   Input ~ 0
BLUE_OUT
Wire Wire Line
	4250 5050 4400 5050
Wire Wire Line
	4250 5250 4400 5250
Wire Wire Line
	4400 5250 4400 5150
Connection ~ 4400 5050
Wire Wire Line
	4400 5050 4550 5050
Wire Wire Line
	4250 5150 4400 5150
Connection ~ 4400 5150
Wire Wire Line
	4400 5150 4400 5050
Wire Wire Line
	4550 4750 4400 4750
Connection ~ 4400 4750
Wire Wire Line
	4400 4750 4250 4750
Wire Wire Line
	4250 4950 4400 4950
Wire Wire Line
	4400 4750 4400 4850
Wire Wire Line
	4250 4850 4400 4850
Connection ~ 4400 4850
Wire Wire Line
	4400 4850 4400 4950
Wire Wire Line
	4550 4450 4400 4450
Wire Wire Line
	4250 4550 4400 4550
Wire Wire Line
	4400 4550 4400 4450
Connection ~ 4400 4450
Wire Wire Line
	4400 4450 4250 4450
Wire Wire Line
	4250 4650 4400 4650
Wire Wire Line
	4400 4650 4400 4550
Connection ~ 4400 4550
Wire Wire Line
	5550 4500 5550 4600
Wire Wire Line
	5750 4500 5750 4600
Wire Wire Line
	5950 4500 5950 4600
$Comp
L power:+3.3V #PWR06
U 1 1 64CB19D0
P 5750 3900
F 0 "#PWR06" H 5750 3750 50  0001 C CNN
F 1 "+3.3V" H 5765 4073 50  0000 C CNN
F 2 "" H 5750 3900 50  0001 C CNN
F 3 "" H 5750 3900 50  0001 C CNN
	1    5750 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 4200 5550 4050
Wire Wire Line
	5550 4050 5750 4050
Wire Wire Line
	5950 4050 5950 4200
Wire Wire Line
	5750 3900 5750 4050
Connection ~ 5750 4050
Wire Wire Line
	5750 4050 5950 4050
Wire Wire Line
	5750 4200 5750 4050
Text GLabel 5250 1500 2    50   Input ~ 0
HSYNC_OUT
Text GLabel 5250 1700 2    50   Input ~ 0
VSYNC_OUT
Wire Wire Line
	5250 1700 5050 1700
Wire Wire Line
	5050 1500 5250 1500
NoConn ~ 4600 900 
NoConn ~ 4600 1200
NoConn ~ 4600 1300
NoConn ~ 4600 1400
Text GLabel 3900 1900 0    50   Input ~ 0
nLSEL
Wire Wire Line
	8800 5800 9000 5800
Wire Wire Line
	9000 5700 8800 5700
Wire Wire Line
	8800 5600 9000 5600
Wire Wire Line
	9500 5800 9700 5800
Text GLabel 9700 5800 2    50   Input ~ 0
nCARDSEL
NoConn ~ 9500 5200
NoConn ~ 9500 5300
NoConn ~ 9500 5100
$Comp
L power:+5V #PWR022
U 1 1 6570BC7D
P 9650 4850
F 0 "#PWR022" H 9650 4700 50  0001 C CNN
F 1 "+5V" H 9665 5023 50  0000 C CNN
F 2 "" H 9650 4850 50  0001 C CNN
F 3 "" H 9650 4850 50  0001 C CNN
	1    9650 4850
	1    0    0    -1  
$EndComp
Text GLabel 8800 5800 0    50   Input ~ 0
nIOSTROBE
Text GLabel 8800 5700 0    50   Input ~ 0
nIOSEL
Text GLabel 8800 5600 0    50   Input ~ 0
nDEVSEL
Text GLabel 9700 4050 2    50   Input ~ 0
nCARDSEL
Wire Wire Line
	9700 3750 9500 3750
Text GLabel 8800 3950 0    50   Input ~ 0
nLSEL
Wire Wire Line
	8800 3650 9000 3650
Wire Wire Line
	9500 3350 9550 3350
Connection ~ 9550 3350
Text GLabel 6850 3650 0    50   Input ~ 0
LD3
Text GLabel 6850 3750 0    50   Input ~ 0
LD2
Text GLabel 6850 3850 0    50   Input ~ 0
LD1
Text GLabel 6850 3950 0    50   Input ~ 0
LD0
Text GLabel 6850 3450 0    50   Input ~ 0
LD5
Text GLabel 6850 3550 0    50   Input ~ 0
LD4
Text GLabel 6850 3250 0    50   Input ~ 0
LD7
Text GLabel 6850 3350 0    50   Input ~ 0
LD6
Text GLabel 8800 1550 0    50   Input ~ 0
LD3
Text GLabel 8800 1650 0    50   Input ~ 0
LD2
Text GLabel 8800 1750 0    50   Input ~ 0
LD1
Text GLabel 8800 1850 0    50   Input ~ 0
LD0
Text GLabel 8800 1350 0    50   Input ~ 0
LD5
Text GLabel 8800 1450 0    50   Input ~ 0
LD4
Text GLabel 8800 1150 0    50   Input ~ 0
LD7
Text GLabel 8800 1250 0    50   Input ~ 0
LD6
$Comp
L Connector:DB15_Female_HighDensity_MountingHoles J2
U 1 1 659834C1
P 2400 6450
F 0 "J2" H 2400 7317 50  0000 C CNN
F 1 "DB15_Female_HighDensity_MountingHoles" H 2400 7226 50  0000 C CNN
F 2 "Connector_Dsub:DSUB-15-HD_Female_Horizontal_P2.29x1.98mm_EdgePinOffset3.03mm_Housed_MountingHolesOffset4.94mm" H 1450 6850 50  0001 C CNN
F 3 " ~" H 1450 6850 50  0001 C CNN
	1    2400 6450
	1    0    0    -1  
$EndComp
Text GLabel 1950 6050 0    50   Input ~ 0
RED_OUT
Text GLabel 1950 6250 0    50   Input ~ 0
GREEN_OUT
Text GLabel 1950 6450 0    50   Input ~ 0
BLUE_OUT
Wire Wire Line
	2100 6450 1950 6450
Wire Wire Line
	1950 6250 2100 6250
Wire Wire Line
	1950 6050 2100 6050
Text GLabel 2900 6450 2    50   Input ~ 0
HSYNC_OUT
Text GLabel 2900 6650 2    50   Input ~ 0
VSYNC_OUT
Wire Wire Line
	2900 6650 2700 6650
Wire Wire Line
	2700 6450 2900 6450
$Comp
L power:GND #PWR0101
U 1 1 65A38707
P 2400 7350
F 0 "#PWR0101" H 2400 7100 50  0001 C CNN
F 1 "GND" H 2405 7177 50  0000 C CNN
F 2 "" H 2400 7350 50  0001 C CNN
F 3 "" H 2400 7350 50  0001 C CNN
	1    2400 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 7350 2400 7250
Wire Wire Line
	2100 6750 2050 6750
Wire Wire Line
	2050 6750 2050 6850
Wire Wire Line
	2050 7250 2400 7250
Connection ~ 2400 7250
Wire Wire Line
	2400 7250 2400 7150
Wire Wire Line
	2100 6850 2050 6850
Connection ~ 2050 6850
Wire Wire Line
	2050 6850 2050 7250
Wire Wire Line
	2100 5950 2050 5950
Wire Wire Line
	2050 5950 2050 6150
Connection ~ 2050 6750
Wire Wire Line
	2100 6150 2050 6150
Connection ~ 2050 6150
Wire Wire Line
	2050 6150 2050 6350
Wire Wire Line
	2100 6350 2050 6350
Connection ~ 2050 6350
Wire Wire Line
	2050 6350 2050 6750
$Comp
L Device:R R11
U 1 1 64AB05DA
P 4900 1700
F 0 "R11" V 4693 1700 50  0001 C CNN
F 1 "47" V 4900 1700 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric_Pad0.72x0.64mm_HandSolder" V 4830 1700 50  0001 C CNN
F 3 "~" H 4900 1700 50  0001 C CNN
	1    4900 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8950 3150 8950 4050
NoConn ~ 9000 3250
Wire Wire Line
	9550 3350 9550 3450
Wire Wire Line
	10300 3200 10500 3200
Text GLabel 10500 3200 2    50   Input ~ 0
nCARDSEL
Text GLabel 8800 5500 0    50   Input ~ 0
nDATADIR
Wire Wire Line
	8800 5200 9000 5200
Wire Wire Line
	8800 5300 9000 5300
Wire Wire Line
	8800 5400 9000 5400
Wire Wire Line
	8800 5500 9000 5500
$Comp
L Connector_Generic:Conn_02x10_Counter_Clockwise U5
U 1 1 66540AAF
P 9200 5400
F 0 "U5" H 9250 6017 50  0000 C CNN
F 1 "PALCE16V8H-5JC/5" H 9250 5926 50  0000 C CNN
F 2 "Package_LCC:PLCC-20" H 9200 5400 50  0001 C CNN
F 3 "~" H 9200 5400 50  0001 C CNN
	1    9200 5400
	1    0    0    -1  
$EndComp
Text GLabel 4000 2300 0    50   Input ~ 0
nDATAOE
Text GLabel 4000 2500 0    50   Input ~ 0
nMSBOE
Text GLabel 4000 2400 0    50   Input ~ 0
nLSBOE
Text GLabel 4000 2200 0    50   Input ~ 0
nDATADIR
Text GLabel 8800 5100 0    50   Input ~ 0
nSYSRESET
Wire Wire Line
	8800 5100 9000 5100
NoConn ~ 9000 5000
NoConn ~ 9500 5400
NoConn ~ 9500 5500
$Comp
L Regulator_Linear:AP1117-33 U7
U 1 1 6698C94F
P 1850 4300
F 0 "U7" H 1850 4542 50  0000 C CNN
F 1 "AZ1117I-H-3.3" H 1850 4451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 1850 4300 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/1117fd.pdf" H 1850 4300 50  0001 C CNN
	1    1850 4300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 6698D42B
P 1400 4150
F 0 "#PWR0102" H 1400 4000 50  0001 C CNN
F 1 "+5V" H 1415 4323 50  0000 C CNN
F 2 "" H 1400 4150 50  0001 C CNN
F 3 "" H 1400 4150 50  0001 C CNN
	1    1400 4150
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 6698D692
P 2300 4150
F 0 "#PWR0103" H 2300 4000 50  0001 C CNN
F 1 "+3.3V" H 2315 4323 50  0000 C CNN
F 2 "" H 2300 4150 50  0001 C CNN
F 3 "" H 2300 4150 50  0001 C CNN
	1    2300 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 4300 2300 4300
Wire Wire Line
	2300 4300 2300 4150
Wire Wire Line
	1550 4300 1400 4300
Wire Wire Line
	1400 4300 1400 4150
$Comp
L power:GND #PWR0104
U 1 1 66A25B9A
P 1850 4850
F 0 "#PWR0104" H 1850 4600 50  0001 C CNN
F 1 "GND" H 1855 4677 50  0000 C CNN
F 2 "" H 1850 4850 50  0001 C CNN
F 3 "" H 1850 4850 50  0001 C CNN
	1    1850 4850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1850 4850 1850 4750
$Comp
L Device:C C11
U 1 1 66A4CA84
P 1400 4550
F 0 "C11" H 1515 4596 50  0000 L CNN
F 1 "10uF" H 1515 4505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 1438 4400 50  0001 C CNN
F 3 "~" H 1400 4550 50  0001 C CNN
	1    1400 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 66A4D029
P 2300 4550
F 0 "C10" H 2415 4596 50  0000 L CNN
F 1 "22uF" H 2415 4505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 2338 4400 50  0001 C CNN
F 3 "~" H 2300 4550 50  0001 C CNN
	1    2300 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 4700 1400 4750
Wire Wire Line
	1400 4750 1850 4750
Connection ~ 1850 4750
Wire Wire Line
	1850 4750 1850 4600
Wire Wire Line
	2300 4700 2300 4750
Wire Wire Line
	2300 4750 1850 4750
Wire Wire Line
	2300 4400 2300 4300
Connection ~ 2300 4300
Wire Wire Line
	1400 4400 1400 4300
Connection ~ 1400 4300
NoConn ~ 9000 3350
NoConn ~ 9000 3450
NoConn ~ 9000 3550
Wire Wire Line
	9500 3450 9550 3450
Connection ~ 9550 3450
Wire Wire Line
	9550 3450 9550 3550
Wire Wire Line
	9500 3550 9550 3550
Connection ~ 9550 3550
Wire Wire Line
	9550 3550 9550 3650
Wire Wire Line
	9500 3650 9550 3650
Connection ~ 9550 3650
Wire Wire Line
	9550 3650 9550 4150
Wire Wire Line
	1550 3200 2050 3200
$Comp
L power:+5V #PWR0105
U 1 1 6704A14E
P 5150 800
F 0 "#PWR0105" H 5150 650 50  0001 C CNN
F 1 "+5V" H 5165 973 50  0000 C CNN
F 2 "" H 5150 800 50  0001 C CNN
F 3 "" H 5150 800 50  0001 C CNN
	1    5150 800 
	1    0    0    -1  
$EndComp
Text GLabel 9700 5900 2    50   Input ~ 0
A10
Text GLabel 9700 5600 2    50   Input ~ 0
A8
Text GLabel 9700 5700 2    50   Input ~ 0
A9
Wire Wire Line
	9500 5900 9700 5900
Wire Wire Line
	9700 5700 9500 5700
Wire Wire Line
	9500 5600 9700 5600
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 63A3190C
P 7550 5400
F 0 "J3" H 7630 5392 50  0000 L CNN
F 1 "NOPAL" H 7630 5301 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_TrianglePad1.0x1.5mm" H 7550 5400 50  0001 C CNN
F 3 "~" H 7550 5400 50  0001 C CNN
	1    7550 5400
	1    0    0    -1  
$EndComp
Text GLabel 7200 5500 0    50   Input ~ 0
nCARDSEL
Wire Wire Line
	7200 5500 7350 5500
Wire Wire Line
	7350 5400 7200 5400
Text GLabel 7200 5400 0    50   Input ~ 0
nDEVSEL
Wire Wire Line
	2250 2800 2050 2800
Text GLabel 2250 2800 2    50   Input ~ 0
nSYSRESET
NoConn ~ 4600 1900
$EndSCHEMATC
