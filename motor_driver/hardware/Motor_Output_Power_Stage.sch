EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 7
Title "Motor Driver - Power Stage"
Date "2021-11-21"
Rev "0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2800 2700 2800 2800
$Comp
L power:GND #PWR036
U 1 1 61F1F6FF
P 2800 2800
AR Path="/61F11923/61F1F6FF" Ref="#PWR036"  Part="1" 
AR Path="/61784CBE/61F1F6FF" Ref="#PWR?"  Part="1" 
AR Path="/6179D64D/61F1F6FF" Ref="#PWR?"  Part="1" 
AR Path="/617B73F7/61F1F6FF" Ref="#PWR?"  Part="1" 
AR Path="/617B844F/61F1F6FF" Ref="#PWR039"  Part="1" 
AR Path="/617D8A55/61F1F6FF" Ref="#PWR042"  Part="1" 
AR Path="/61835CF5/61F1F6FF" Ref="#PWR?"  Part="1" 
AR Path="/61835CFA/61F1F6FF" Ref="#PWR?"  Part="1" 
AR Path="/61835CFF/61F1F6FF" Ref="#PWR?"  Part="1" 
AR Path="/6187BE23/61F1F6FF" Ref="#PWR0107"  Part="1" 
AR Path="/6187BE28/61F1F6FF" Ref="#PWR0104"  Part="1" 
AR Path="/6187BE2D/61F1F6FF" Ref="#PWR0101"  Part="1" 
AR Path="/619C9AA1/61F1F6FF" Ref="#PWR0105"  Part="1" 
AR Path="/619C9AA7/61F1F6FF" Ref="#PWR0108"  Part="1" 
AR Path="/619C9AAD/61F1F6FF" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0108" H 2800 2550 50  0001 C CNN
F 1 "GND" H 2805 2627 50  0000 C CNN
F 2 "" H 2800 2800 50  0001 C CNN
F 3 "" H 2800 2800 50  0001 C CNN
	1    2800 2800
	1    0    0    -1  
$EndComp
$Comp
L lm5106:LM5106MM_NOPB U6
U 1 1 61F1F705
P 3150 1600
AR Path="/61F11923/61F1F705" Ref="U6"  Part="1" 
AR Path="/61784CBE/61F1F705" Ref="U?"  Part="1" 
AR Path="/6179D64D/61F1F705" Ref="U?"  Part="1" 
AR Path="/617B73F7/61F1F705" Ref="U?"  Part="1" 
AR Path="/617B844F/61F1F705" Ref="U7"  Part="1" 
AR Path="/617D8A55/61F1F705" Ref="U8"  Part="1" 
AR Path="/61835CF5/61F1F705" Ref="U?"  Part="1" 
AR Path="/61835CFA/61F1F705" Ref="U?"  Part="1" 
AR Path="/61835CFF/61F1F705" Ref="U?"  Part="1" 
AR Path="/6187BE23/61F1F705" Ref="U11"  Part="1" 
AR Path="/6187BE28/61F1F705" Ref="U10"  Part="1" 
AR Path="/6187BE2D/61F1F705" Ref="U9"  Part="1" 
AR Path="/619C9AA1/61F1F705" Ref="U10"  Part="1" 
AR Path="/619C9AA7/61F1F705" Ref="U11"  Part="1" 
AR Path="/619C9AAD/61F1F705" Ref="U13"  Part="1" 
F 0 "U11" H 3950 1987 60  0000 C CNN
F 1 "LM5106MM_NOPB" H 3950 1881 60  0000 C CNN
F 2 "lm5106:LM5106MM_slash_NOPB" H 3950 1840 60  0001 C CNN
F 3 "" H 3150 1600 60  0000 C CNN
	1    3150 1600
	1    0    0    -1  
$EndComp
NoConn ~ 3150 2300
Wire Wire Line
	4750 1700 4800 1700
Wire Wire Line
	2900 1500 2900 1700
Wire Wire Line
	2900 1700 3150 1700
Text GLabel 2600 1200 0    50   Input ~ 0
V_DRIVERS
$Comp
L Device:C_Small C16
U 1 1 61F1F737
P 2750 1650
AR Path="/61F11923/61F1F737" Ref="C16"  Part="1" 
AR Path="/61784CBE/61F1F737" Ref="C?"  Part="1" 
AR Path="/6179D64D/61F1F737" Ref="C?"  Part="1" 
AR Path="/617B73F7/61F1F737" Ref="C?"  Part="1" 
AR Path="/617B844F/61F1F737" Ref="C19"  Part="1" 
AR Path="/617D8A55/61F1F737" Ref="C22"  Part="1" 
AR Path="/61835CF5/61F1F737" Ref="C?"  Part="1" 
AR Path="/61835CFA/61F1F737" Ref="C?"  Part="1" 
AR Path="/61835CFF/61F1F737" Ref="C?"  Part="1" 
AR Path="/6187BE23/61F1F737" Ref="C31"  Part="1" 
AR Path="/6187BE28/61F1F737" Ref="C28"  Part="1" 
AR Path="/6187BE2D/61F1F737" Ref="C25"  Part="1" 
AR Path="/619C9AA1/61F1F737" Ref="C24"  Part="1" 
AR Path="/619C9AA7/61F1F737" Ref="C26"  Part="1" 
AR Path="/619C9AAD/61F1F737" Ref="C28"  Part="1" 
F 0 "C26" H 2550 1700 50  0000 L CNN
F 1 "470nF" H 2450 1600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2750 1650 50  0001 C CNN
F 3 "~" H 2750 1650 50  0001 C CNN
	1    2750 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1750 2750 1800
Wire Wire Line
	2750 1800 3150 1800
Wire Wire Line
	2600 1200 2750 1200
Wire Wire Line
	2000 900  2000 1800
Wire Wire Line
	2000 900  4950 900 
Connection ~ 2750 1800
Text GLabel 2700 2500 0    50   Input ~ 0
V_DRIVERS
$Comp
L power:GND #PWR035
U 1 1 61F1F759
P 1950 2400
AR Path="/61F11923/61F1F759" Ref="#PWR035"  Part="1" 
AR Path="/61784CBE/61F1F759" Ref="#PWR?"  Part="1" 
AR Path="/6179D64D/61F1F759" Ref="#PWR?"  Part="1" 
AR Path="/617B73F7/61F1F759" Ref="#PWR?"  Part="1" 
AR Path="/617B844F/61F1F759" Ref="#PWR038"  Part="1" 
AR Path="/617D8A55/61F1F759" Ref="#PWR041"  Part="1" 
AR Path="/61835CF5/61F1F759" Ref="#PWR?"  Part="1" 
AR Path="/61835CFA/61F1F759" Ref="#PWR?"  Part="1" 
AR Path="/61835CFF/61F1F759" Ref="#PWR?"  Part="1" 
AR Path="/6187BE23/61F1F759" Ref="#PWR0108"  Part="1" 
AR Path="/6187BE28/61F1F759" Ref="#PWR0105"  Part="1" 
AR Path="/6187BE2D/61F1F759" Ref="#PWR0102"  Part="1" 
AR Path="/619C9AA1/61F1F759" Ref="#PWR0106"  Part="1" 
AR Path="/619C9AA7/61F1F759" Ref="#PWR0109"  Part="1" 
AR Path="/619C9AAD/61F1F759" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0109" H 1950 2150 50  0001 C CNN
F 1 "GND" H 1955 2227 50  0000 C CNN
F 2 "" H 1950 2400 50  0001 C CNN
F 3 "" H 1950 2400 50  0001 C CNN
	1    1950 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R14
U 1 1 61F1F75F
P 1950 2200
AR Path="/61F11923/61F1F75F" Ref="R14"  Part="1" 
AR Path="/61784CBE/61F1F75F" Ref="R?"  Part="1" 
AR Path="/6179D64D/61F1F75F" Ref="R?"  Part="1" 
AR Path="/617B73F7/61F1F75F" Ref="R?"  Part="1" 
AR Path="/617B844F/61F1F75F" Ref="R20"  Part="1" 
AR Path="/617D8A55/61F1F75F" Ref="R26"  Part="1" 
AR Path="/61835CF5/61F1F75F" Ref="R?"  Part="1" 
AR Path="/61835CFA/61F1F75F" Ref="R?"  Part="1" 
AR Path="/61835CFF/61F1F75F" Ref="R?"  Part="1" 
AR Path="/6187BE23/61F1F75F" Ref="R44"  Part="1" 
AR Path="/6187BE28/61F1F75F" Ref="R38"  Part="1" 
AR Path="/6187BE2D/61F1F75F" Ref="R32"  Part="1" 
AR Path="/619C9AA1/61F1F75F" Ref="R35"  Part="1" 
AR Path="/619C9AA7/61F1F75F" Ref="R40"  Part="1" 
AR Path="/619C9AAD/61F1F75F" Ref="R45"  Part="1" 
F 0 "R40" H 2018 2246 50  0000 L CNN
F 1 "47k" H 2018 2155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 1950 2200 50  0001 C CNN
F 3 "~" H 1950 2200 50  0001 C CNN
	1    1950 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2000 1950 2100
Wire Wire Line
	3000 1050 3050 1050
Wire Wire Line
	3050 1050 3050 1600
Wire Wire Line
	3050 1600 3150 1600
Wire Wire Line
	2700 2500 3000 2500
$Comp
L Device:D D8
U 1 1 61F1F787
P 6900 1600
AR Path="/61F11923/61F1F787" Ref="D8"  Part="1" 
AR Path="/61784CBE/61F1F787" Ref="D?"  Part="1" 
AR Path="/6179D64D/61F1F787" Ref="D?"  Part="1" 
AR Path="/617B73F7/61F1F787" Ref="D?"  Part="1" 
AR Path="/617B844F/61F1F787" Ref="D13"  Part="1" 
AR Path="/617D8A55/61F1F787" Ref="D18"  Part="1" 
AR Path="/61835CF5/61F1F787" Ref="D?"  Part="1" 
AR Path="/61835CFA/61F1F787" Ref="D?"  Part="1" 
AR Path="/61835CFF/61F1F787" Ref="D?"  Part="1" 
AR Path="/6187BE23/61F1F787" Ref="D33"  Part="1" 
AR Path="/6187BE28/61F1F787" Ref="D28"  Part="1" 
AR Path="/6187BE2D/61F1F787" Ref="D23"  Part="1" 
AR Path="/619C9AA1/61F1F787" Ref="D23"  Part="1" 
AR Path="/619C9AA7/61F1F787" Ref="D28"  Part="1" 
AR Path="/619C9AAD/61F1F787" Ref="D33"  Part="1" 
F 0 "D28" V 6854 1680 50  0000 L CNN
F 1 "MUR460" V 6945 1680 50  0000 L CNN
F 2 "added:D_DO-15_P10.16mm__H1.4mm_Horizontal" H 6900 1600 50  0001 C CNN
F 3 "~" H 6900 1600 50  0001 C CNN
	1    6900 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	6900 1450 6900 1150
Wire Wire Line
	6900 1150 6400 1150
Wire Wire Line
	1850 1900 3150 1900
Text HLabel 3000 1050 0    50   Input ~ 0
MOTOR_ENABLE
Text Notes 8550 1550 0    50   ~ 0
Diode Candidates (fast switching):\n- UF4007 
Wire Wire Line
	4800 1700 4800 2650
Wire Wire Line
	4950 900  4950 2150
Text HLabel 7100 2150 2    50   Input ~ 0
MOTOR_OUT_PHASE_X
Wire Wire Line
	6900 2150 7100 2150
Connection ~ 6900 2150
Wire Wire Line
	6900 1750 6900 2150
Wire Wire Line
	6400 2150 6900 2150
Wire Wire Line
	4950 2150 6400 2150
Connection ~ 6400 2150
Wire Wire Line
	6400 3100 6400 3150
Connection ~ 6400 3100
Wire Wire Line
	6900 3100 6400 3100
Wire Wire Line
	6900 2800 6900 3100
Wire Wire Line
	6900 2150 6900 2500
$Comp
L Device:D D9
U 1 1 61F1F78D
P 6900 2650
AR Path="/61F11923/61F1F78D" Ref="D9"  Part="1" 
AR Path="/61784CBE/61F1F78D" Ref="D?"  Part="1" 
AR Path="/6179D64D/61F1F78D" Ref="D?"  Part="1" 
AR Path="/617B73F7/61F1F78D" Ref="D?"  Part="1" 
AR Path="/617B844F/61F1F78D" Ref="D14"  Part="1" 
AR Path="/617D8A55/61F1F78D" Ref="D19"  Part="1" 
AR Path="/61835CF5/61F1F78D" Ref="D?"  Part="1" 
AR Path="/61835CFA/61F1F78D" Ref="D?"  Part="1" 
AR Path="/61835CFF/61F1F78D" Ref="D?"  Part="1" 
AR Path="/6187BE23/61F1F78D" Ref="D34"  Part="1" 
AR Path="/6187BE28/61F1F78D" Ref="D29"  Part="1" 
AR Path="/6187BE2D/61F1F78D" Ref="D24"  Part="1" 
AR Path="/619C9AA1/61F1F78D" Ref="D24"  Part="1" 
AR Path="/619C9AA7/61F1F78D" Ref="D29"  Part="1" 
AR Path="/619C9AAD/61F1F78D" Ref="D34"  Part="1" 
F 0 "D29" V 6854 2730 50  0000 L CNN
F 1 "MUR460" V 6945 2730 50  0000 L CNN
F 2 "added:D_DO-15_P10.16mm__H1.4mm_Horizontal" H 6900 2650 50  0001 C CNN
F 3 "~" H 6900 2650 50  0001 C CNN
	1    6900 2650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR037
U 1 1 61F1F751
P 6400 3150
AR Path="/61F11923/61F1F751" Ref="#PWR037"  Part="1" 
AR Path="/61784CBE/61F1F751" Ref="#PWR?"  Part="1" 
AR Path="/6179D64D/61F1F751" Ref="#PWR?"  Part="1" 
AR Path="/617B73F7/61F1F751" Ref="#PWR?"  Part="1" 
AR Path="/617B844F/61F1F751" Ref="#PWR040"  Part="1" 
AR Path="/617D8A55/61F1F751" Ref="#PWR043"  Part="1" 
AR Path="/61835CF5/61F1F751" Ref="#PWR?"  Part="1" 
AR Path="/61835CFA/61F1F751" Ref="#PWR?"  Part="1" 
AR Path="/61835CFF/61F1F751" Ref="#PWR?"  Part="1" 
AR Path="/6187BE23/61F1F751" Ref="#PWR0109"  Part="1" 
AR Path="/6187BE28/61F1F751" Ref="#PWR0106"  Part="1" 
AR Path="/6187BE2D/61F1F751" Ref="#PWR0103"  Part="1" 
AR Path="/619C9AA1/61F1F751" Ref="#PWR0107"  Part="1" 
AR Path="/619C9AA7/61F1F751" Ref="#PWR0110"  Part="1" 
AR Path="/619C9AAD/61F1F751" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0110" H 6400 2900 50  0001 C CNN
F 1 "GND" H 6405 2977 50  0000 C CNN
F 2 "" H 6400 3150 50  0001 C CNN
F 3 "" H 6400 3150 50  0001 C CNN
	1    6400 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Small D6
U 1 1 61F1F71B
P 5550 1450
AR Path="/61F11923/61F1F71B" Ref="D6"  Part="1" 
AR Path="/61784CBE/61F1F71B" Ref="D?"  Part="1" 
AR Path="/6179D64D/61F1F71B" Ref="D?"  Part="1" 
AR Path="/617B73F7/61F1F71B" Ref="D?"  Part="1" 
AR Path="/617B844F/61F1F71B" Ref="D11"  Part="1" 
AR Path="/617D8A55/61F1F71B" Ref="D16"  Part="1" 
AR Path="/61835CF5/61F1F71B" Ref="D?"  Part="1" 
AR Path="/61835CFA/61F1F71B" Ref="D?"  Part="1" 
AR Path="/61835CFF/61F1F71B" Ref="D?"  Part="1" 
AR Path="/6187BE23/61F1F71B" Ref="D31"  Part="1" 
AR Path="/6187BE28/61F1F71B" Ref="D26"  Part="1" 
AR Path="/6187BE2D/61F1F71B" Ref="D21"  Part="1" 
AR Path="/619C9AA1/61F1F71B" Ref="D21"  Part="1" 
AR Path="/619C9AA7/61F1F71B" Ref="D26"  Part="1" 
AR Path="/619C9AAD/61F1F71B" Ref="D31"  Part="1" 
F 0 "D26" H 5550 1657 50  0000 C CNN
F 1 "UF4007" H 5550 1566 50  0000 C CNN
F 2 "Diode_THT:D_T-1_P5.08mm_Horizontal" V 5550 1450 50  0001 C CNN
F 3 "~" V 5550 1450 50  0001 C CNN
	1    5550 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R16
U 1 1 61F1F714
P 5300 1600
AR Path="/61F11923/61F1F714" Ref="R16"  Part="1" 
AR Path="/61784CBE/61F1F714" Ref="R?"  Part="1" 
AR Path="/6179D64D/61F1F714" Ref="R?"  Part="1" 
AR Path="/617B73F7/61F1F714" Ref="R?"  Part="1" 
AR Path="/617B844F/61F1F714" Ref="R22"  Part="1" 
AR Path="/617D8A55/61F1F714" Ref="R28"  Part="1" 
AR Path="/61835CF5/61F1F714" Ref="R?"  Part="1" 
AR Path="/61835CFA/61F1F714" Ref="R?"  Part="1" 
AR Path="/61835CFF/61F1F714" Ref="R?"  Part="1" 
AR Path="/6187BE23/61F1F714" Ref="R46"  Part="1" 
AR Path="/6187BE28/61F1F714" Ref="R40"  Part="1" 
AR Path="/6187BE2D/61F1F714" Ref="R34"  Part="1" 
AR Path="/619C9AA1/61F1F714" Ref="R37"  Part="1" 
AR Path="/619C9AA7/61F1F714" Ref="R42"  Part="1" 
AR Path="/619C9AAD/61F1F714" Ref="R47"  Part="1" 
F 0 "R42" V 5500 1600 50  0000 C CNN
F 1 "10R" V 5400 1600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5300 1600 50  0001 C CNN
F 3 "~" H 5300 1600 50  0001 C CNN
	1    5300 1600
	0    1    1    0   
$EndComp
Connection ~ 2750 1500
Wire Wire Line
	2750 1500 2750 1550
Wire Wire Line
	2900 1500 2750 1500
$Comp
L Device:D_Small D5
U 1 1 61F1F741
P 2750 1350
AR Path="/61F11923/61F1F741" Ref="D5"  Part="1" 
AR Path="/61784CBE/61F1F741" Ref="D?"  Part="1" 
AR Path="/6179D64D/61F1F741" Ref="D?"  Part="1" 
AR Path="/617B73F7/61F1F741" Ref="D?"  Part="1" 
AR Path="/617B844F/61F1F741" Ref="D10"  Part="1" 
AR Path="/617D8A55/61F1F741" Ref="D15"  Part="1" 
AR Path="/61835CF5/61F1F741" Ref="D?"  Part="1" 
AR Path="/61835CFA/61F1F741" Ref="D?"  Part="1" 
AR Path="/61835CFF/61F1F741" Ref="D?"  Part="1" 
AR Path="/6187BE23/61F1F741" Ref="D30"  Part="1" 
AR Path="/6187BE28/61F1F741" Ref="D25"  Part="1" 
AR Path="/6187BE2D/61F1F741" Ref="D20"  Part="1" 
AR Path="/619C9AA1/61F1F741" Ref="D20"  Part="1" 
AR Path="/619C9AA7/61F1F741" Ref="D25"  Part="1" 
AR Path="/619C9AAD/61F1F741" Ref="D30"  Part="1" 
F 0 "D25" V 2800 1500 50  0000 R CNN
F 1 "UF4007" V 2700 1700 50  0000 R CNN
F 2 "Diode_THT:D_T-1_P5.08mm_Horizontal" V 2750 1350 50  0001 C CNN
F 3 "~" V 2750 1350 50  0001 C CNN
	1    2750 1350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2750 1200 2750 1250
Wire Wire Line
	2750 1450 2750 1500
Text Notes 8550 700  0    55   ~ 0
Gate Resistor calculations:
Text Notes 8550 1100 0    50   ~ 0
With VDD=9V of Gate Driver\nRmax_rise = 7ohm (1.3A)  -> use 10ohm\nRmax_fall = 5ohm (1.8A)    \n   ->  10 ohm for diode in parallel 10ohm  -> 5ohm
Wire Wire Line
	5400 1600 5750 1600
$Comp
L Device:R_Small_US R15
U 1 1 616CB3D9
P 5300 1450
AR Path="/61F11923/616CB3D9" Ref="R15"  Part="1" 
AR Path="/61784CBE/616CB3D9" Ref="R?"  Part="1" 
AR Path="/6179D64D/616CB3D9" Ref="R?"  Part="1" 
AR Path="/617B73F7/616CB3D9" Ref="R?"  Part="1" 
AR Path="/617B844F/616CB3D9" Ref="R21"  Part="1" 
AR Path="/617D8A55/616CB3D9" Ref="R27"  Part="1" 
AR Path="/61835CF5/616CB3D9" Ref="R?"  Part="1" 
AR Path="/61835CFA/616CB3D9" Ref="R?"  Part="1" 
AR Path="/61835CFF/616CB3D9" Ref="R?"  Part="1" 
AR Path="/6187BE23/616CB3D9" Ref="R45"  Part="1" 
AR Path="/6187BE28/616CB3D9" Ref="R39"  Part="1" 
AR Path="/6187BE2D/616CB3D9" Ref="R33"  Part="1" 
AR Path="/619C9AA1/616CB3D9" Ref="R36"  Part="1" 
AR Path="/619C9AA7/616CB3D9" Ref="R41"  Part="1" 
AR Path="/619C9AAD/616CB3D9" Ref="R46"  Part="1" 
F 0 "R41" V 5100 1450 50  0000 C CNN
F 1 "10R" V 5200 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5300 1450 50  0001 C CNN
F 3 "~" H 5300 1450 50  0001 C CNN
	1    5300 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 1450 5450 1450
Wire Wire Line
	5650 1450 5750 1450
Wire Wire Line
	5750 1450 5750 1600
Connection ~ 5750 1600
Wire Wire Line
	4750 1600 5100 1600
Wire Wire Line
	5100 1450 5100 1600
Wire Wire Line
	5100 1450 5200 1450
Connection ~ 5100 1600
Wire Wire Line
	5100 1600 5200 1600
$Comp
L Device:D_Small D7
U 1 1 616DC86D
P 5550 2500
AR Path="/61F11923/616DC86D" Ref="D7"  Part="1" 
AR Path="/61784CBE/616DC86D" Ref="D?"  Part="1" 
AR Path="/6179D64D/616DC86D" Ref="D?"  Part="1" 
AR Path="/617B73F7/616DC86D" Ref="D?"  Part="1" 
AR Path="/617B844F/616DC86D" Ref="D12"  Part="1" 
AR Path="/617D8A55/616DC86D" Ref="D17"  Part="1" 
AR Path="/61835CF5/616DC86D" Ref="D?"  Part="1" 
AR Path="/61835CFA/616DC86D" Ref="D?"  Part="1" 
AR Path="/61835CFF/616DC86D" Ref="D?"  Part="1" 
AR Path="/6187BE23/616DC86D" Ref="D32"  Part="1" 
AR Path="/6187BE28/616DC86D" Ref="D27"  Part="1" 
AR Path="/6187BE2D/616DC86D" Ref="D22"  Part="1" 
AR Path="/619C9AA1/616DC86D" Ref="D22"  Part="1" 
AR Path="/619C9AA7/616DC86D" Ref="D27"  Part="1" 
AR Path="/619C9AAD/616DC86D" Ref="D32"  Part="1" 
F 0 "D27" H 5550 2707 50  0000 C CNN
F 1 "UF4007" H 5550 2616 50  0000 C CNN
F 2 "Diode_THT:D_T-1_P5.08mm_Horizontal" V 5550 2500 50  0001 C CNN
F 3 "~" V 5550 2500 50  0001 C CNN
	1    5550 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R18
U 1 1 616DC873
P 5300 2650
AR Path="/61F11923/616DC873" Ref="R18"  Part="1" 
AR Path="/61784CBE/616DC873" Ref="R?"  Part="1" 
AR Path="/6179D64D/616DC873" Ref="R?"  Part="1" 
AR Path="/617B73F7/616DC873" Ref="R?"  Part="1" 
AR Path="/617B844F/616DC873" Ref="R24"  Part="1" 
AR Path="/617D8A55/616DC873" Ref="R30"  Part="1" 
AR Path="/61835CF5/616DC873" Ref="R?"  Part="1" 
AR Path="/61835CFA/616DC873" Ref="R?"  Part="1" 
AR Path="/61835CFF/616DC873" Ref="R?"  Part="1" 
AR Path="/6187BE23/616DC873" Ref="R48"  Part="1" 
AR Path="/6187BE28/616DC873" Ref="R42"  Part="1" 
AR Path="/6187BE2D/616DC873" Ref="R36"  Part="1" 
AR Path="/619C9AA1/616DC873" Ref="R39"  Part="1" 
AR Path="/619C9AA7/616DC873" Ref="R44"  Part="1" 
AR Path="/619C9AAD/616DC873" Ref="R49"  Part="1" 
F 0 "R44" V 5500 2650 50  0000 C CNN
F 1 "10R" V 5400 2650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5300 2650 50  0001 C CNN
F 3 "~" H 5300 2650 50  0001 C CNN
	1    5300 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 2650 5750 2650
$Comp
L Device:R_Small_US R17
U 1 1 616DC87A
P 5300 2500
AR Path="/61F11923/616DC87A" Ref="R17"  Part="1" 
AR Path="/61784CBE/616DC87A" Ref="R?"  Part="1" 
AR Path="/6179D64D/616DC87A" Ref="R?"  Part="1" 
AR Path="/617B73F7/616DC87A" Ref="R?"  Part="1" 
AR Path="/617B844F/616DC87A" Ref="R23"  Part="1" 
AR Path="/617D8A55/616DC87A" Ref="R29"  Part="1" 
AR Path="/61835CF5/616DC87A" Ref="R?"  Part="1" 
AR Path="/61835CFA/616DC87A" Ref="R?"  Part="1" 
AR Path="/61835CFF/616DC87A" Ref="R?"  Part="1" 
AR Path="/6187BE23/616DC87A" Ref="R47"  Part="1" 
AR Path="/6187BE28/616DC87A" Ref="R41"  Part="1" 
AR Path="/6187BE2D/616DC87A" Ref="R35"  Part="1" 
AR Path="/619C9AA1/616DC87A" Ref="R38"  Part="1" 
AR Path="/619C9AA7/616DC87A" Ref="R43"  Part="1" 
AR Path="/619C9AAD/616DC87A" Ref="R48"  Part="1" 
F 0 "R43" V 5100 2500 50  0000 C CNN
F 1 "10R" V 5200 2500 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5300 2500 50  0001 C CNN
F 3 "~" H 5300 2500 50  0001 C CNN
	1    5300 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 2500 5450 2500
Wire Wire Line
	5650 2500 5750 2500
Wire Wire Line
	5750 2500 5750 2650
Wire Wire Line
	5100 2500 5100 2650
Wire Wire Line
	5100 2500 5200 2500
Wire Wire Line
	5100 2650 5200 2650
Wire Wire Line
	4800 2650 5100 2650
Connection ~ 5100 2650
Connection ~ 5750 2650
Wire Wire Line
	1950 2000 3150 2000
Wire Wire Line
	2800 2700 3000 2700
$Comp
L Device:C_Small C17
U 1 1 616F5ED2
P 3000 2600
AR Path="/61F11923/616F5ED2" Ref="C17"  Part="1" 
AR Path="/61784CBE/616F5ED2" Ref="C?"  Part="1" 
AR Path="/6179D64D/616F5ED2" Ref="C?"  Part="1" 
AR Path="/617B73F7/616F5ED2" Ref="C?"  Part="1" 
AR Path="/617B844F/616F5ED2" Ref="C20"  Part="1" 
AR Path="/617D8A55/616F5ED2" Ref="C23"  Part="1" 
AR Path="/61835CF5/616F5ED2" Ref="C?"  Part="1" 
AR Path="/61835CFA/616F5ED2" Ref="C?"  Part="1" 
AR Path="/61835CFF/616F5ED2" Ref="C?"  Part="1" 
AR Path="/6187BE23/616F5ED2" Ref="C32"  Part="1" 
AR Path="/6187BE28/616F5ED2" Ref="C29"  Part="1" 
AR Path="/6187BE2D/616F5ED2" Ref="C26"  Part="1" 
AR Path="/619C9AA1/616F5ED2" Ref="C25"  Part="1" 
AR Path="/619C9AA7/616F5ED2" Ref="C27"  Part="1" 
AR Path="/619C9AAD/616F5ED2" Ref="C29"  Part="1" 
F 0 "C27" H 2800 2650 50  0000 L CNN
F 1 "10uF" H 2700 2550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3000 2600 50  0001 C CNN
F 3 "~" H 3000 2600 50  0001 C CNN
	1    3000 2600
	1    0    0    -1  
$EndComp
Connection ~ 3000 2500
Wire Wire Line
	3000 2500 3150 2500
Connection ~ 3000 2700
Wire Wire Line
	3000 2700 3150 2700
$Comp
L Device:Q_NMOS_GDS Q1
U 1 1 6190108C
P 6300 1600
AR Path="/61F11923/6190108C" Ref="Q1"  Part="1" 
AR Path="/61784CBE/6190108C" Ref="Q?"  Part="1" 
AR Path="/6179D64D/6190108C" Ref="Q?"  Part="1" 
AR Path="/617B73F7/6190108C" Ref="Q?"  Part="1" 
AR Path="/617B844F/6190108C" Ref="Q3"  Part="1" 
AR Path="/617D8A55/6190108C" Ref="Q5"  Part="1" 
AR Path="/61835CF5/6190108C" Ref="Q?"  Part="1" 
AR Path="/61835CFA/6190108C" Ref="Q?"  Part="1" 
AR Path="/61835CFF/6190108C" Ref="Q?"  Part="1" 
AR Path="/6187BE23/6190108C" Ref="Q11"  Part="1" 
AR Path="/6187BE28/6190108C" Ref="Q9"  Part="1" 
AR Path="/6187BE2D/6190108C" Ref="Q7"  Part="1" 
AR Path="/619C9AA1/6190108C" Ref="Q7"  Part="1" 
AR Path="/619C9AA7/6190108C" Ref="Q9"  Part="1" 
AR Path="/619C9AAD/6190108C" Ref="Q11"  Part="1" 
F 0 "Q9" H 6100 1850 50  0000 L CNN
F 1 "LNG03R031" H 5900 1750 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 6500 1700 50  0001 C CNN
F 3 "~" H 6300 1600 50  0001 C CNN
	1    6300 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1150 6400 1400
Wire Wire Line
	6400 1800 6400 2150
Wire Wire Line
	5750 1600 6100 1600
$Comp
L Device:Q_NMOS_GDS Q2
U 1 1 61914378
P 6300 2650
AR Path="/61F11923/61914378" Ref="Q2"  Part="1" 
AR Path="/61784CBE/61914378" Ref="Q?"  Part="1" 
AR Path="/6179D64D/61914378" Ref="Q?"  Part="1" 
AR Path="/617B73F7/61914378" Ref="Q?"  Part="1" 
AR Path="/617B844F/61914378" Ref="Q4"  Part="1" 
AR Path="/617D8A55/61914378" Ref="Q6"  Part="1" 
AR Path="/61835CF5/61914378" Ref="Q?"  Part="1" 
AR Path="/61835CFA/61914378" Ref="Q?"  Part="1" 
AR Path="/61835CFF/61914378" Ref="Q?"  Part="1" 
AR Path="/6187BE23/61914378" Ref="Q12"  Part="1" 
AR Path="/6187BE28/61914378" Ref="Q10"  Part="1" 
AR Path="/6187BE2D/61914378" Ref="Q8"  Part="1" 
AR Path="/619C9AA1/61914378" Ref="Q8"  Part="1" 
AR Path="/619C9AA7/61914378" Ref="Q10"  Part="1" 
AR Path="/619C9AAD/61914378" Ref="Q12"  Part="1" 
F 0 "Q10" H 6100 2900 50  0000 L CNN
F 1 "LNG03R031" H 5900 2800 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 6500 2750 50  0001 C CNN
F 3 "~" H 6300 2650 50  0001 C CNN
	1    6300 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2150 6400 2450
Wire Wire Line
	5750 2650 6100 2650
Wire Wire Line
	6400 2850 6400 3100
Text HLabel 1850 1900 0    50   Input ~ 0
PWM_IN_PHASE_X
Wire Wire Line
	2000 1800 2750 1800
Wire Wire Line
	1950 2300 1950 2400
Text HLabel 6300 900  0    50   Input ~ 0
PHASE_V_MOTOR_IN
Wire Wire Line
	6300 900  6400 900 
Wire Wire Line
	6400 900  6400 1150
Connection ~ 6400 1150
$Comp
L Device:C_Small C?
U 1 1 61F3BFDF
P 8200 2150
F 0 "C?" H 8292 2196 50  0000 L CNN
F 1 "10uF" H 8292 2105 50  0000 L CNN
F 2 "" H 8200 2150 50  0001 C CNN
F 3 "~" H 8200 2150 50  0001 C CNN
	1    8200 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 900  8200 900 
Wire Wire Line
	8200 900  8200 2050
Connection ~ 6400 900 
Wire Wire Line
	8200 2250 8200 3100
Wire Wire Line
	8200 3100 6900 3100
Connection ~ 6900 3100
Text Notes 8600 2200 0    50   ~ 0
<- new cap\n   tests showed this reduces noise cased by mosfet switching
$EndSCHEMATC
