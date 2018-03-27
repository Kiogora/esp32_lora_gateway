EESchema Schematic File Version 4
LIBS:prototype-cache
EELAYER 26 0
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
L quectel_L80:QUECTEL_L80 U?
U 1 1 5AB8D6BC
P 1650 1350
F 0 "U?" H 1625 1801 39  0000 C CNN
F 1 "QUECTEL_L80" H 1625 1726 39  0000 C CNN
F 2 "" H 1550 1400 39  0001 C CNN
F 3 "" H 1550 1400 39  0001 C CNN
	1    1650 1350
	1    0    0    -1  
$EndComp
Wire Notes Line
	3420 475  3420 2360
Wire Notes Line
	3420 2360 475  2360
Text Notes 2640 2365 0    50   ~ 0
Quectel GPS module
$Comp
L power:GND #PWR?
U 1 1 5AB8DBE6
P 3250 1400
F 0 "#PWR?" H 3250 1150 50  0001 C CNN
F 1 "GND" H 3255 1227 50  0000 C CNN
F 2 "" H 3250 1400 50  0001 C CNN
F 3 "" H 3250 1400 50  0001 C CNN
	1    3250 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1400 2150 1400
Wire Wire Line
	3250 1400 2550 1400
NoConn ~ 1100 1300
$Comp
L power:GND #PWR?
U 1 1 5AB8E471
P 600 1650
F 0 "#PWR?" H 600 1400 50  0001 C CNN
F 1 "GND" H 605 1477 50  0000 C CNN
F 2 "" H 600 1650 50  0001 C CNN
F 3 "" H 600 1650 50  0001 C CNN
	1    600  1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	600  1650 600  1600
NoConn ~ 1100 1100
NoConn ~ 1100 1200
Wire Wire Line
	2150 1300 2550 1300
Text Label 2550 1300 0    50   ~ 0
BRD_GPS_VCC
NoConn ~ 1100 1500
Text Label 2550 1500 0    50   ~ 0
BRD_GPS_TX
Text Label 2550 1600 0    50   ~ 0
BRD_GPS_RX
Text Label 2550 1200 0    50   ~ 0
BRD_GPS_VBCKUP
Text Label 2550 1100 0    50   ~ 0
BRD_GPS_1PPS
Text Notes 3420 3970 2    50   ~ 0
GPS select jumper
Text Label 2350 2550 2    50   ~ 0
BRD_GPS_RST
Text Notes 8270 3975 2    50   ~ 0
ESP32_controller
$Comp
L Jumper:SolderJumper_3_Open JP?
U 1 1 5AB9FE96
P 1300 2650
F 0 "JP?" H 1300 2764 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 1300 2764 50  0001 C CNN
F 2 "" H 1300 2650 50  0001 C CNN
F 3 "~" H 1300 2650 50  0001 C CNN
	1    1300 2650
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP?
U 1 1 5AB9FFCE
P 1450 3100
F 0 "JP?" H 1450 3214 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 1450 3214 50  0001 C CNN
F 2 "" H 1450 3100 50  0001 C CNN
F 3 "~" H 1450 3100 50  0001 C CNN
	1    1450 3100
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP?
U 1 1 5ABA0014
P 2600 3400
F 0 "JP?" H 2600 3514 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 2600 3514 50  0001 C CNN
F 2 "" H 2600 3400 50  0001 C CNN
F 3 "~" H 2600 3400 50  0001 C CNN
	1    2600 3400
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP?
U 1 1 5ABA006A
P 1450 3550
F 0 "JP?" H 1450 3664 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 1450 3664 50  0001 C CNN
F 2 "" H 1450 3550 50  0001 C CNN
F 3 "~" H 1450 3550 50  0001 C CNN
	1    1450 3550
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP?
U 1 1 5ABA00A0
P 2600 2550
F 0 "JP?" H 2600 2664 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 2600 2664 50  0001 C CNN
F 2 "" H 2600 2550 50  0001 C CNN
F 3 "~" H 2600 2550 50  0001 C CNN
	1    2600 2550
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP?
U 1 1 5ABA00F0
P 2600 2950
F 0 "JP?" H 2600 3064 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 2600 3064 50  0001 C CNN
F 2 "" H 2600 2950 50  0001 C CNN
F 3 "~" H 2600 2950 50  0001 C CNN
	1    2600 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 2650 1050 2650
Wire Wire Line
	1250 3100 1150 3100
Text Label 1050 2650 2    50   ~ 0
BRD_GPS_1PPS
Text Label 1150 3100 2    50   ~ 0
BRD_GPS_VBCKUP
Text Label 1000 3550 2    50   ~ 0
BRD_GPS_VCC
Wire Wire Line
	1000 3550 1250 3550
Wire Wire Line
	2350 2550 2400 2550
Text Label 2300 2950 2    50   ~ 0
BRD_GPS_RX
Wire Wire Line
	2300 2950 2400 2950
Text Label 2300 3400 2    50   ~ 0
BRD_GPS_TX
Wire Notes Line
	470  3975 11220 3975
Wire Notes Line
	6970 6535 475  6535
Wire Notes Line
	8270 2145 11220 2145
Text Label 1000 1400 2    50   ~ 0
BRD_GPS_RST
Text Notes 11215 2145 2    50   ~ 0
Cellular_module
$Comp
L mikroe_click3g_eu:MIKROE_3G_CLICK U?
U 1 1 5ABA1831
P 9700 1100
F 0 "U?" H 9700 1625 50  0000 C CNN
F 1 "MIKROE_3G_CLICK" H 9700 1534 50  0000 C CNN
F 2 "" H 9500 1150 50  0001 C CNN
F 3 "" H 9500 1150 50  0001 C CNN
	1    9700 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 1400 10550 1400
$Comp
L power:GND #PWR?
U 1 1 5ABA1E29
P 10650 1600
F 0 "#PWR?" H 10650 1350 50  0001 C CNN
F 1 "GND" H 10655 1427 50  0000 C CNN
F 2 "" H 10650 1600 50  0001 C CNN
F 3 "" H 10650 1600 50  0001 C CNN
	1    10650 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 1500 10350 1500
Wire Wire Line
	10650 1500 10650 1600
Wire Wire Line
	10350 1000 10650 1000
Wire Wire Line
	10350 1100 10650 1100
Text Label 10650 1000 0    50   ~ 0
GSM_TX
Text Label 10650 1100 0    50   ~ 0
GSM_RX
$Comp
L power:GND #PWR?
U 1 1 5ABA32B6
P 8800 1600
F 0 "#PWR?" H 8800 1350 50  0001 C CNN
F 1 "GND" H 8805 1427 50  0000 C CNN
F 2 "" H 8800 1600 50  0001 C CNN
F 3 "" H 8800 1600 50  0001 C CNN
	1    8800 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 1600 8800 1500
Wire Wire Line
	8800 1500 9050 1500
Wire Wire Line
	9050 900  8800 900 
Text Label 8800 900  2    50   ~ 0
PWR_KEY
Text Label 10550 1400 0    50   ~ 0
5V
Text Label 8800 1400 2    50   ~ 0
3V3
Wire Wire Line
	9050 1400 8800 1400
Wire Wire Line
	10350 900  10650 900 
Text Label 10650 900  0    50   ~ 0
GSM_CTS
Wire Wire Line
	9050 1000 8800 1000
Text Label 8800 1000 2    50   ~ 0
GSM_RTS
NoConn ~ 9050 800 
NoConn ~ 9050 1100
NoConn ~ 9050 1200
NoConn ~ 9050 1300
NoConn ~ 10350 1200
NoConn ~ 10350 1300
Text Notes 8300 2750 0    50   ~ 0
RTS_ENABLE(DISABLED DEFAULT)
Text Notes 9950 2750 0    50   ~ 0
CTS_ENABLE(DISABLED DEFAULT)
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 5ABA9218
P 8900 2600
F 0 "JP?" H 8900 2713 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 8900 2714 50  0001 C CNN
F 2 "" H 8900 2600 50  0001 C CNN
F 3 "~" H 8900 2600 50  0001 C CNN
	1    8900 2600
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 5ABA92A7
P 10650 2600
F 0 "JP?" H 10650 2713 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 10650 2714 50  0001 C CNN
F 2 "" H 10650 2600 50  0001 C CNN
F 3 "~" H 10650 2600 50  0001 C CNN
	1    10650 2600
	1    0    0    -1  
$EndComp
Wire Notes Line
	8270 2765 11220 2765
Text Label 8700 2600 2    50   ~ 0
GSM_RTS
Wire Wire Line
	8700 2600 8750 2600
Text Label 10400 2600 2    50   ~ 0
GSM_CTS
Wire Wire Line
	10400 2600 10500 2600
Wire Wire Line
	10350 800  10650 800 
Text Label 10650 800  0    50   ~ 0
GSM_RI
Text Notes 9150 2450 0    50   ~ 0
RI_ENABLE(DISABLED DEFAULT)
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 5ABAF538
P 9750 2300
F 0 "JP?" H 9750 2400 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 9750 2414 50  0001 C CNN
F 2 "" H 9750 2300 50  0001 C CNN
F 3 "~" H 9750 2300 50  0001 C CNN
	1    9750 2300
	1    0    0    -1  
$EndComp
Text Label 9550 2300 2    50   ~ 0
GSM_RI
Wire Wire Line
	9550 2300 9600 2300
$Comp
L Jumper:SolderJumper_2_Bridged JP?
U 1 1 5ABB548F
P 2400 1400
F 0 "JP?" H 2550 1450 50  0000 C CNN
F 1 "SolderJumper_2_Bridged" H 2400 1514 50  0001 C CNN
F 2 "" H 2400 1400 50  0001 C CNN
F 3 "~" H 2400 1400 50  0001 C CNN
	1    2400 1400
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Bridged JP?
U 1 1 5ABB55B9
P 850 1600
F 0 "JP?" H 850 1700 50  0000 C CNN
F 1 "SolderJumper_2_Bridged" H 850 1714 50  0001 C CNN
F 2 "" H 850 1600 50  0001 C CNN
F 3 "~" H 850 1600 50  0001 C CNN
	1    850  1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	700  1600 600  1600
Wire Wire Line
	1000 1600 1100 1600
Wire Wire Line
	2150 1500 2550 1500
Wire Wire Line
	2150 1600 2550 1600
Wire Wire Line
	2150 1100 2550 1100
Wire Wire Line
	2150 1200 2550 1200
Wire Wire Line
	1000 1400 1100 1400
Text Label 1550 2650 0    50   ~ 0
HDR_GPS_1PPS
Wire Wire Line
	1500 2650 1550 2650
Wire Wire Line
	2800 2550 2850 2550
Text Label 2850 2550 0    50   ~ 0
HDR_GPS_RST
Wire Wire Line
	2800 2950 2850 2950
Text Label 2850 2950 0    50   ~ 0
HDR_GPS_RX
Wire Wire Line
	1650 3100 1800 3100
Text Label 1800 3100 0    50   ~ 0
HDR_GPS_VBCKUP
Wire Wire Line
	2400 3400 2300 3400
Wire Wire Line
	2800 3400 2850 3400
Text Label 2850 3400 0    50   ~ 0
HDR_GPS_TX
Wire Wire Line
	1650 3550 1800 3550
Text Label 1800 3550 0    50   ~ 0
HDR_GPS_VCC
$Comp
L quectel_L80:UBX_M8Q_GPS_HEADER U?
U 1 1 5ABCB800
P 1850 4500
F 0 "U?" H 1825 4925 50  0000 C CNN
F 1 "UBX_M8Q_GPS_HEADER" H 1825 4834 50  0000 C CNN
F 2 "" H 1700 4500 50  0001 C CNN
F 3 "" H 1700 4500 50  0001 C CNN
	1    1850 4500
	1    0    0    -1  
$EndComp
Text Label 2450 4800 0    50   ~ 0
HDR_GPS_RX
Text Label 2450 4400 0    50   ~ 0
HDR_GPS_VBCKUP
Text Label 2450 4500 0    50   ~ 0
HDR_GPS_VCC
Wire Notes Line
	475  5145 3420 5145
Text Notes 3415 5145 2    50   ~ 0
Alternative_GPS_header
Wire Wire Line
	2450 4800 2350 4800
Text Label 2450 4700 0    50   ~ 0
HDR_GPS_TX
Wire Wire Line
	2450 4700 2350 4700
Text Label 2450 4300 0    50   ~ 0
HDR_GPS_1PPS
Wire Wire Line
	2450 4300 2350 4300
Wire Wire Line
	2450 4400 2350 4400
Wire Wire Line
	2450 4500 2350 4500
NoConn ~ 1300 4300
NoConn ~ 1300 4500
NoConn ~ 1300 4700
$Comp
L power:GND #PWR?
U 1 1 5ABE8587
P 3150 4800
F 0 "#PWR?" H 3150 4550 50  0001 C CNN
F 1 "GND" H 3155 4627 50  0000 C CNN
F 2 "" H 3150 4800 50  0001 C CNN
F 3 "" H 3150 4800 50  0001 C CNN
	1    3150 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 4800 3150 4600
Wire Wire Line
	3150 4600 2350 4600
$Comp
L Connector_Specialized:Test_Point_Probe TP?
U 1 1 5ABE97AB
P 950 4400
F 0 "TP?" V 1240 4456 50  0000 C CNN
F 1 "Test_Point_Probe" V 1149 4456 50  0000 C CNN
F 2 "" H 1150 4400 50  0001 C CNN
F 3 "~" H 1150 4400 50  0001 C CNN
	1    950  4400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	950  4400 1300 4400
$Comp
L power:GND #PWR?
U 1 1 5ABEAAB0
P 1000 4800
F 0 "#PWR?" H 1000 4550 50  0001 C CNN
F 1 "GND" H 1005 4627 50  0000 C CNN
F 2 "" H 1000 4800 50  0001 C CNN
F 3 "" H 1000 4800 50  0001 C CNN
	1    1000 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 4800 1300 4800
Text Label 1200 4600 2    50   ~ 0
HDR_GPS_RST
Wire Wire Line
	1200 4600 1300 4600
Wire Notes Line
	8270 470  8270 6535
Text Notes 6950 7800 2    50   ~ 0
No RTC as the GSM module and ESP32 has an internal, non battery backed RTC available
Wire Notes Line
	3420 2365 3420 7795
$Comp
L esp32_devkitC:DevkitC U?
U 1 1 5AC380A0
P 5650 2100
F 0 "U?" H 5625 3175 50  0000 C CNN
F 1 "DevkitC" H 5625 3084 50  0000 C CNN
F 2 "" H 5650 2350 50  0001 C CNN
F 3 "" H 5650 2350 50  0001 C CNN
	1    5650 2100
	1    0    0    -1  
$EndComp
Text Notes 8270 6535 2    50   ~ 0
Power_management
Text Notes 11220 6535 2    50   ~ 0
Kaiote standard LoRa concentrator connector
$Comp
L Connector_Specialized:Jack-DC J?
U 1 1 5AC3C1D6
P 3850 4550
F 0 "J?" H 3928 4875 50  0000 C CNN
F 1 "Jack-DC" H 3928 4784 50  0000 C CNN
F 2 "" H 3900 4510 50  0001 C CNN
F 3 "~" H 3900 4510 50  0001 C CNN
	1    3850 4550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
