EESchema Schematic File Version 4
LIBS:prototype-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "Kaiote LoRa gateway with surge suppression"
Date "2018-04-23"
Rev "1"
Comp "Kaiote Limited"
Comment1 "Design by: Alois Mbutura"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L quectel_L80:QUECTEL_L80 U1
U 1 1 5AB8D6BC
P 1650 1350
F 0 "U1" H 1625 1801 39  0000 C CNN
F 1 "QUECTEL_L80" H 1625 1726 39  0000 C CNN
F 2 "lib_fp:QUECTEL_L80-M39" H 1550 1400 39  0001 C CNN
F 3 "" H 1550 1400 39  0001 C CNN
	1    1650 1350
	1    0    0    -1  
$EndComp
Wire Notes Line
	3420 475  3420 2360
Text Notes 2640 2365 0    50   ~ 0
Quectel GPS module
$Comp
L power:GND #PWR08
U 1 1 5AB8DBE6
P 2200 1400
F 0 "#PWR08" H 2200 1150 50  0001 C CNN
F 1 "GND" H 2205 1227 50  0000 C CNN
F 2 "" H 2200 1400 50  0001 C CNN
F 3 "" H 2200 1400 50  0001 C CNN
	1    2200 1400
	0    -1   -1   0   
$EndComp
NoConn ~ 1100 1300
$Comp
L power:GND #PWR03
U 1 1 5AB8E471
P 950 1650
F 0 "#PWR03" H 950 1400 50  0001 C CNN
F 1 "GND" H 955 1477 50  0000 C CNN
F 2 "" H 950 1650 50  0001 C CNN
F 3 "" H 950 1650 50  0001 C CNN
	1    950  1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  1650 950  1600
NoConn ~ 1100 1100
NoConn ~ 1100 1200
Wire Wire Line
	2150 1300 3150 1300
NoConn ~ 1100 1500
Text Label 2550 1500 0    50   ~ 0
BRD_GPS_TX
Text Label 2550 1600 0    50   ~ 0
BRD_GPS_RX
Text Label 2550 1200 0    50   ~ 0
BRD_GPS_VBCKUP
Text Label 2550 1100 0    50   ~ 0
BRD_GPS_1PPS
Text Notes 8250 3500 2    50   ~ 0
ESP32_controller
Wire Notes Line
	470  3975 11220 3975
Wire Notes Line
	8270 2145 11220 2145
Text Notes 11215 2145 2    50   ~ 0
Cellular_module
$Comp
L mikroe_click3g_eu:MIKROE_3G_CLICK U5
U 1 1 5ABA1831
P 9700 1100
F 0 "U5" H 9700 1625 50  0000 C CNN
F 1 "MIKROE_3G_CLICK" H 9700 1534 50  0000 C CNN
F 2 "lib_fp:MIKROE_UG95" H 9500 1150 50  0001 C CNN
F 3 "https://download.mikroe.com/documents/add-on-boards/click/3g/3g-click-schematic-v103.pdf" H 9500 1150 50  0001 C CNN
F 4 "https://www.mikroe.com/3g-ea-click" H 9700 1100 50  0001 C CNN "purchase page"
	1    9700 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 1400 10600 1400
Wire Wire Line
	10350 1000 10450 1000
Wire Wire Line
	10350 1100 10450 1100
Text Label 10450 1000 0    50   ~ 0
GSM_TX
Text Label 10450 1100 0    50   ~ 0
GSM_RX
Wire Wire Line
	9050 900  9000 900 
Text Label 9000 900  2    50   ~ 0
PWR_KEY
Text Label 11100 1400 1    50   ~ 0
CELLULAR_5V
Text Label 9000 1400 2    50   ~ 0
3V3_REF
Wire Wire Line
	9050 1400 9000 1400
NoConn ~ 9050 1100
NoConn ~ 9050 1200
NoConn ~ 9050 1300
NoConn ~ 10350 1200
NoConn ~ 10350 1300
Wire Wire Line
	2150 1500 2550 1500
Wire Wire Line
	2150 1600 2550 1600
Wire Wire Line
	2150 1100 2550 1100
Wire Wire Line
	2150 1200 2550 1200
Wire Notes Line
	475  5145 3420 5145
Text Notes 3450 7800 0    50   ~ 0
Note: No external RTC is implemented as this is an always on power device. ESP32 \nhas an on power RTC
$Comp
L esp32_devkitC:DevkitC U4
U 1 1 5AC380A0
P 6700 2500
F 0 "U4" H 6675 3575 50  0000 C CNN
F 1 "DevkitC" H 6675 3484 50  0000 C CNN
F 2 "lib_fp:ESP32_DEVKITC" H 6700 2750 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf" H 6700 2750 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/espressif-systems/ESP32-DEVKITC/1904-1011-ND/8544299" H 6700 2500 50  0001 C CNN "purchase page"
	1    6700 2500
	1    0    0    -1  
$EndComp
Text Notes 8270 4970 2    50   ~ 0
5v power_management
Text Notes 11220 6535 2    50   ~ 0
LoRa concentrator connector(Top view), SMA RF connector on right
Text Label 3900 4100 0    50   ~ 0
(+)RAIL
NoConn ~ 10000 5800
$Comp
L narra:LM2596_MODULE U3
U 1 1 5AC09295
P 5650 4450
F 0 "U3" H 5650 4600 60  0000 C CNN
F 1 "LM2596_MODULE" H 5650 4500 60  0000 C CNN
F 2 "lib_fp:LM2596_MODULE" H 5650 4450 60  0001 C CNN
F 3 "" H 5650 4450 60  0001 C CNN
F 4 "https://store.nerokas.co.ke/index.php?route=product/product&product_id=1083&search=lm2596&description=true" H 5650 4450 50  0001 C CNN "purchase page"
	1    5650 4450
	1    0    0    -1  
$EndComp
Text Label 7600 4850 2    50   ~ 0
GLOBAL_5V
$Comp
L power:GND #PWR019
U 1 1 5AC119E2
P 4450 4700
F 0 "#PWR019" H 4450 4450 50  0001 C CNN
F 1 "GND" H 4600 4600 50  0000 C CNN
F 2 "" H 4450 4700 50  0001 C CNN
F 3 "" H 4450 4700 50  0001 C CNN
	1    4450 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4700 4600 4700
$Comp
L power:GND #PWR025
U 1 1 5AC13B96
P 7700 4700
F 0 "#PWR025" H 7700 4450 50  0001 C CNN
F 1 "GND" H 7850 4650 50  0000 C CNN
F 2 "" H 7700 4700 50  0001 C CNN
F 3 "" H 7700 4700 50  0001 C CNN
	1    7700 4700
	1    0    0    -1  
$EndComp
Text Notes 6965 7100 2    50   ~ 0
5v Input voltage tie-point
Wire Wire Line
	5450 6950 5700 6950
Text Label 5900 6700 0    50   ~ 0
CELLULAR_5V
Text Label 5900 6950 0    50   ~ 0
LoRa_5V
Wire Wire Line
	5700 6700 5900 6700
Text Label 5450 6950 2    50   ~ 0
GLOBAL_5V
Wire Notes Line
	3420 5640 8270 5640
$Comp
L Device:Jumper_NC_Dual JP2
U 1 1 5ABE743B
P 1100 5500
F 0 "JP2" H 1100 5739 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 1100 5648 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 1100 5500 50  0001 C CNN
F 3 "~" H 1100 5500 50  0001 C CNN
	1    1100 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP8
U 1 1 5ABE75DF
P 2050 5500
F 0 "JP8" H 2050 5739 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 2050 5648 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 2050 5500 50  0001 C CNN
F 3 "~" H 2050 5500 50  0001 C CNN
	1    2050 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP12
U 1 1 5ABE76D3
P 2950 5500
F 0 "JP12" H 2950 5739 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 2950 5648 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 2950 5500 50  0001 C CNN
F 3 "~" H 2950 5500 50  0001 C CNN
	1    2950 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP3
U 1 1 5ABE773F
P 1100 6050
F 0 "JP3" H 1100 6289 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 1100 6198 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 1100 6050 50  0001 C CNN
F 3 "~" H 1100 6050 50  0001 C CNN
	1    1100 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP7
U 1 1 5ABE779D
P 1900 6050
F 0 "JP7" H 1900 6289 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 1900 6198 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 1900 6050 50  0001 C CNN
F 3 "~" H 1900 6050 50  0001 C CNN
	1    1900 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 5650 1100 5650
Wire Wire Line
	1100 5650 1100 5600
Wire Wire Line
	2100 5650 2050 5650
Wire Wire Line
	2050 5650 2050 5600
Wire Wire Line
	1950 6250 1900 6250
Wire Wire Line
	1900 6250 1900 6150
Wire Wire Line
	1150 6250 1100 6250
Wire Wire Line
	1100 6250 1100 6150
Wire Wire Line
	3000 5700 2950 5700
Wire Wire Line
	2950 5700 2950 5600
Text Label 1200 5650 0    50   ~ 0
3
Text Label 2100 5650 0    50   ~ 0
5
Text Label 3000 5700 0    50   ~ 0
6
Text Label 1150 6250 0    50   ~ 0
11
Text Label 1950 6250 0    50   ~ 0
14
$Comp
L Device:Jumper_NC_Dual JP4
U 1 1 5AC244DA
P 1100 6850
F 0 "JP4" H 1100 7089 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 1100 6998 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 1100 6850 50  0001 C CNN
F 3 "~" H 1100 6850 50  0001 C CNN
	1    1100 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP15
U 1 1 5AC244E8
P 3100 6850
F 0 "JP15" H 3100 7089 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 3100 6998 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 3100 6850 50  0001 C CNN
F 3 "~" H 3100 6850 50  0001 C CNN
	1    3100 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP5
U 1 1 5AC244F6
P 1100 7400
F 0 "JP5" H 1100 7639 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 1100 7548 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 1100 7400 50  0001 C CNN
F 3 "~" H 1100 7400 50  0001 C CNN
	1    1100 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 7000 1100 7000
Wire Wire Line
	1100 7000 1100 6950
Wire Wire Line
	2100 7000 2100 6950
Wire Wire Line
	3100 7000 3000 7000
Wire Wire Line
	3100 7000 3100 6950
Wire Wire Line
	1200 7600 1100 7600
Wire Wire Line
	1100 7600 1100 7500
Text Label 1200 7000 0    50   ~ 0
16
Text Label 2200 7000 0    50   ~ 0
17
Text Label 3000 7000 2    50   ~ 0
19
Text Label 1200 7600 0    50   ~ 0
20
Text Notes 8270 6535 2    50   ~ 0
Alternative programming header
Wire Wire Line
	1400 5500 1350 5500
$Comp
L power:GND #PWR02
U 1 1 5AC89963
P 750 6050
F 0 "#PWR02" H 750 5800 50  0001 C CNN
F 1 "GND" H 755 5877 50  0000 C CNN
F 2 "" H 750 6050 50  0001 C CNN
F 3 "" H 750 6050 50  0001 C CNN
	1    750  6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  6050 850  6050
NoConn ~ 1350 6050
$Comp
L Device:Jumper_NC_Dual JP13
U 1 1 5ACA767E
P 2950 6050
F 0 "JP13" H 2950 6289 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 2950 6198 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 2950 6050 50  0001 C CNN
F 3 "~" H 2950 6050 50  0001 C CNN
	1    2950 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 6250 2950 6250
Wire Wire Line
	2950 6250 2950 6150
Text Label 3000 6250 0    50   ~ 0
15
Wire Wire Line
	3250 6050 3200 6050
Text Label 3250 6050 0    50   ~ 0
MISO
Wire Wire Line
	1350 6850 1400 6850
Text Label 1800 6850 2    50   ~ 0
MISO
NoConn ~ 3350 6850
Wire Wire Line
	750  7400 850  7400
Text Label 2200 7600 0    50   ~ 0
21
Wire Wire Line
	2150 7600 2150 7500
Wire Wire Line
	2200 7600 2150 7600
$Comp
L Device:Jumper_NC_Dual JP10
U 1 1 5AC244FD
P 2150 7400
F 0 "JP10" H 2150 7639 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 2150 7548 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 2150 7400 50  0001 C CNN
F 3 "~" H 2150 7400 50  0001 C CNN
	1    2150 7400
	1    0    0    -1  
$EndComp
Text Label 3000 7600 0    50   ~ 0
22
Wire Wire Line
	2950 7600 2950 7500
Wire Wire Line
	3000 7600 2950 7600
$Comp
L Device:Jumper_NC_Dual JP14
U 1 1 5AD7DBBB
P 2950 7400
F 0 "JP14" H 2950 7639 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 2950 7548 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 2950 7400 50  0001 C CNN
F 3 "~" H 2950 7400 50  0001 C CNN
	1    2950 7400
	1    0    0    -1  
$EndComp
Text Label 4050 7050 0    50   ~ 0
23
Wire Wire Line
	4000 7050 4000 6950
Wire Wire Line
	4050 7050 4000 7050
$Comp
L Device:Jumper_NC_Dual JP16
U 1 1 5AD9956F
P 4000 6850
F 0 "JP16" H 4000 7089 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 4000 6998 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 4000 6850 50  0001 C CNN
F 3 "~" H 4000 6850 50  0001 C CNN
	1    4000 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Dual JP11
U 1 1 5AE37C76
P 3250 2800
F 0 "JP11" H 3085 2705 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 3250 2948 50  0001 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 3250 2800 50  0001 C CNN
F 3 "~" H 3250 2800 50  0001 C CNN
	1    3250 2800
	1    0    0    -1  
$EndComp
Wire Notes Line
	475  2360 4695 2360
Wire Wire Line
	10000 5400 9900 5400
Wire Wire Line
	10000 5900 9900 5900
Wire Wire Line
	10000 6000 9900 6000
Wire Wire Line
	10000 6100 9900 6100
Wire Wire Line
	10000 6200 9900 6200
Wire Wire Line
	10000 6300 9900 6300
Wire Wire Line
	10500 5400 10600 5400
Wire Wire Line
	10500 5800 10600 5800
Wire Wire Line
	10500 5900 10600 5900
Wire Wire Line
	10500 6000 10600 6000
Text Label 9900 5400 2    50   ~ 0
5
Text Label 9900 5900 2    50   ~ 0
15
Text Label 9900 6000 2    50   ~ 0
17
Text Label 9900 6100 2    50   ~ 0
19
Text Label 9900 6200 2    50   ~ 0
21
Text Label 9900 6300 2    50   ~ 0
23
Text Label 10600 5900 0    50   ~ 0
16
Text Label 10600 5800 0    50   ~ 0
14
Text Label 10600 5400 0    50   ~ 0
6
Wire Notes Line
	4695 470  4695 3975
Text Notes 4695 2360 2    50   ~ 0
DHT22 Temp, Humidity sensor\n(vertical)
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5AFCF9CD
P 3550 1400
F 0 "J2" H 3470 975 50  0000 C CNN
F 1 "Conn_01x04" H 3450 1050 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x04_P2.54mm_staggered_Vertical" H 3550 1400 50  0001 C CNN
F 3 "~" H 3550 1400 50  0001 C CNN
	1    3550 1400
	-1   0    0    1   
$EndComp
Text Label 4250 1500 0    50   ~ 0
GLOBAL_3V3
Wire Wire Line
	3750 1400 4150 1400
Text Label 4250 1400 0    50   ~ 0
TEMP_DATA
$Comp
L power:GND #PWR014
U 1 1 5AFF154E
P 3850 1200
F 0 "#PWR014" H 3850 950 50  0001 C CNN
F 1 "GND" V 3950 1150 50  0000 R CNN
F 2 "" H 3850 1200 50  0001 C CNN
F 3 "" H 3850 1200 50  0001 C CNN
	1    3850 1200
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C5
U 1 1 5AFFA324
P 4150 1700
F 0 "C5" H 4265 1746 50  0000 L CNN
F 1 "0.1uF" H 4265 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 4188 1550 50  0001 C CNN
F 3 "~" H 4150 1700 50  0001 C CNN
	1    4150 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 1550 4150 1500
$Comp
L power:GND #PWR016
U 1 1 5B002B86
P 4150 1900
F 0 "#PWR016" H 4150 1650 50  0001 C CNN
F 1 "GND" H 4155 1727 50  0000 C CNN
F 2 "" H 4150 1900 50  0001 C CNN
F 3 "" H 4150 1900 50  0001 C CNN
	1    4150 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 1900 4150 1850
Text Notes 1900 6530 2    50   ~ 0
RAK_concentrator selection
Text Notes 1900 6530 0    50   ~ 0
RHF0M301_concentrator selection
Wire Notes Line
	8270 4855 11220 4855
$Comp
L battery-holder:Battery-holder J6
U 1 1 5ABEBE9E
P 9750 4250
F 0 "J6" H 9900 4450 60  0000 C CNN
F 1 "Battery-holder" H 10050 4000 60  0000 C CNN
F 2 "lib_fp:BAT-HLD-001-xx-OTL" H 9750 4250 60  0001 C CNN
F 3 "https://www.mouser.co.za/datasheet/2/238/bat-hld-001-220194.pdf" H 9750 4250 60  0001 C CNN
	1    9750 4250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9950 4350 9850 4350
Text Label 10700 4200 0    50   ~ 0
V_BCKUP
Text Notes 11220 4855 2    50   ~ 0
BAT-HLD-001-xx-OTL CR2032 battery holder
Wire Wire Line
	3550 2950 3250 2950
Wire Wire Line
	3250 2950 3250 2900
Wire Wire Line
	5750 6350 5850 6350
Text Label 5850 6350 0    50   ~ 0
CONTROLLER_TX
Wire Wire Line
	5750 6450 5850 6450
Text Label 5850 6450 0    50   ~ 0
CONTROLLER_RX
$Comp
L Device:R R1
U 1 1 5ACC3516
P 4150 1100
F 0 "R1" H 4200 1000 50  0000 L CNN
F 1 "10k" V 4150 1050 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4080 1100 50  0001 C CNN
F 3 "~" H 4150 1100 50  0001 C CNN
F 4 "NC" H 4250 1150 50  0000 C CNN "Population status"
	1    4150 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 1200 3750 1200
Wire Wire Line
	4150 1250 4150 1400
Connection ~ 4150 1400
Wire Wire Line
	4150 1400 4250 1400
Wire Wire Line
	4150 850  4150 950 
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 5AD1EDDD
P 850 1400
F 0 "JP1" H 850 1513 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 850 1514 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 850 1400 50  0001 C CNN
F 3 "~" H 850 1400 50  0001 C CNN
	1    850  1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	650  1400 700  1400
Wire Wire Line
	1000 1400 1100 1400
Wire Wire Line
	7100 1950 7400 1950
Text Label 7400 1950 0    50   ~ 0
CONTROLLER_TX
Wire Wire Line
	7100 2050 7400 2050
Text Label 7400 2050 0    50   ~ 0
CONTROLLER_RX
$Comp
L power:GND #PWR028
U 1 1 5AD69F58
P 5850 6250
F 0 "#PWR028" H 5850 6000 50  0001 C CNN
F 1 "GND" V 5855 6122 50  0000 R CNN
F 2 "" H 5850 6250 50  0001 C CNN
F 3 "" H 5850 6250 50  0001 C CNN
	1    5850 6250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5750 6250 5850 6250
$Comp
L power:GND #PWR026
U 1 1 5AD767CC
P 7350 1650
F 0 "#PWR026" H 7350 1400 50  0001 C CNN
F 1 "GND" V 7355 1522 50  0000 R CNN
F 2 "" H 7350 1650 50  0001 C CNN
F 3 "" H 7350 1650 50  0001 C CNN
	1    7350 1650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 1650 7100 1650
$Comp
L power:GND #PWR027
U 1 1 5AD812DE
P 7350 2250
F 0 "#PWR027" H 7350 2000 50  0001 C CNN
F 1 "GND" V 7355 2122 50  0000 R CNN
F 2 "" H 7350 2250 50  0001 C CNN
F 3 "" H 7350 2250 50  0001 C CNN
	1    7350 2250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 2250 7100 2250
Wire Wire Line
	5700 6700 5700 6950
Wire Wire Line
	5900 6950 5700 6950
Connection ~ 5700 6950
$Comp
L Regulator_Linear:TLV71333PDBV U2
U 1 1 5AC37939
P 4500 5200
F 0 "U2" H 5150 5000 50  0000 C CNN
F 1 "TLV71333PDBV" H 5350 4900 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 4500 5525 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tlv733p.pdf" H 4500 5200 50  0001 C CNN
F 4 "https://www.digikey.ca/product-detail/en/texas-instruments/TLV73333PDBVR/296-40673-1-ND/5178550" H 4500 5200 50  0001 C CNN "purchase page"
	1    4500 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5AC42802
P 4500 5535
F 0 "#PWR018" H 4500 5285 50  0001 C CNN
F 1 "GND" H 4650 5485 50  0000 C CNN
F 2 "" H 4500 5535 50  0001 C CNN
F 3 "" H 4500 5535 50  0001 C CNN
	1    4500 5535
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 5535 4500 5500
Text Label 3450 5100 0    50   ~ 0
GLOBAL_5V
Text Label 5200 5100 0    50   ~ 0
GLOBAL_3V3
Wire Notes Line
	5655 4970 5655 5640
Text Notes 5655 5640 2    50   ~ 0
3v3 power_management
Text Notes 8300 4750 0    50   ~ 0
Backup current consumption 7uA @ 3v3(Quectel), 15uA Ublox
Text Label 3550 2950 0    50   ~ 0
BRD_GPS_VBCKUP
Text Label 2850 2800 2    50   ~ 0
V_BCKUP
Wire Wire Line
	3500 2800 3600 2800
Wire Wire Line
	2850 2800 3000 2800
Wire Notes Line
	475  3390 4695 3390
Text Notes 4700 3400 2    50   ~ 0
Backup supply selection for SRAM in Cold or Warm/hotstart
Connection ~ 4150 1500
Wire Wire Line
	4150 1500 3750 1500
Wire Wire Line
	4150 1500 4250 1500
Connection ~ 3250 2950
Wire Wire Line
	3250 3050 3250 2950
Wire Wire Line
	3650 3050 3250 3050
Wire Wire Line
	4200 3050 3950 3050
$Comp
L power:GND #PWR012
U 1 1 5ADC417D
P 4200 3050
F 0 "#PWR012" H 4200 2800 50  0001 C CNN
F 1 "GND" H 4350 2950 50  0000 C CNN
F 2 "" H 4200 3050 50  0001 C CNN
F 3 "" H 4200 3050 50  0001 C CNN
	1    4200 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5AD19E5A
P 3800 3050
F 0 "C1" V 3850 2900 50  0000 L CNN
F 1 "0.1uF" V 3850 3100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 3838 2900 50  0001 C CNN
F 3 "~" H 3800 3050 50  0001 C CNN
	1    3800 3050
	0    1    1    0   
$EndComp
Text Label 3600 2800 0    50   ~ 0
GLOBAL_3V3
Wire Wire Line
	950  1600 1100 1600
Wire Notes Line
	4695 1220 8270 1220
Wire Wire Line
	5200 5100 5000 5100
$Comp
L Device:C C7
U 1 1 5B096100
P 5000 5300
F 0 "C7" H 4750 5300 50  0000 L CNN
F 1 "0.1uF" H 4750 5200 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 5038 5150 50  0001 C CNN
F 3 "~" H 5000 5300 50  0001 C CNN
	1    5000 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5150 5000 5100
Connection ~ 5000 5100
Wire Wire Line
	5000 5100 4800 5100
Wire Wire Line
	5000 5500 5000 5450
$Comp
L Device:C C4
U 1 1 5B0B1E08
P 3500 5300
F 0 "C4" H 3650 5350 50  0000 L CNN
F 1 "1uF" H 3650 5250 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 3538 5150 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_X7R_6.3V-to-50V_18.pdf" H 3500 5300 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/yageo/CC0805KKX7R7BB105/311-1365-1-ND/2103149" H 3500 5300 50  0001 C CNN "purchase page"
	1    3500 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 5500 3500 5450
$Comp
L Connector_Specialized:Test_Point_Probe TP1
U 1 1 5B0CED12
P 5000 5100
F 0 "TP1" H 5153 5202 50  0000 L CNN
F 1 "Test_Point_Probe" H 5000 5050 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 5200 5100 50  0001 C CNN
F 3 "~" H 5200 5100 50  0001 C CNN
	1    5000 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5B0CF87A
P 5550 2950
F 0 "#PWR023" H 5550 2700 50  0001 C CNN
F 1 "GND" V 5555 2822 50  0000 R CNN
F 2 "" H 5550 2950 50  0001 C CNN
F 3 "" H 5550 2950 50  0001 C CNN
	1    5550 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	5550 2950 6250 2950
$Comp
L Si2323CDS:Si2323 Q2
U 1 1 5B0E07C0
P 10300 4300
F 0 "Q2" V 10450 4450 50  0000 C CNN
F 1 "Si2323" V 10200 4000 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10500 4225 50  0001 L CIN
F 3 "http://www.vishay.com/docs/65700/si2323cds.pdf" H 10300 4300 50  0001 L CNN
F 4 "https://www.digikey.com/product-detail/en/vishay-siliconix/SI2323CDS-T1-GE3/SI2323CDS-T1-GE3CT-ND/2441900" V 10300 4300 50  0001 C CNN "purchase page"
	1    10300 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9850 4200 10050 4200
$Comp
L power:GND #PWR034
U 1 1 5B0EF63E
P 10100 4550
F 0 "#PWR034" H 10100 4300 50  0001 C CNN
F 1 "GND" H 10250 4500 50  0000 C CNN
F 2 "" H 10100 4550 50  0001 C CNN
F 3 "" H 10100 4550 50  0001 C CNN
	1    10100 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 4550 10300 4500
Wire Wire Line
	10700 4200 10550 4200
Wire Wire Line
	10400 1500 10350 1500
Wire Wire Line
	10600 1450 10600 1400
Connection ~ 10600 1400
Wire Wire Line
	10600 1400 11000 1400
$Comp
L Device:C C12
U 1 1 5B1A558B
P 11000 1600
F 0 "C12" H 11000 1700 50  0000 L CNN
F 1 "0.1uF" H 11000 1500 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 11038 1450 50  0001 C CNN
F 3 "~" H 11000 1600 50  0001 C CNN
	1    11000 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	11000 1450 11000 1400
Connection ~ 11000 1400
Wire Wire Line
	11000 1400 11100 1400
$Comp
L power:GND #PWR039
U 1 1 5B1B3E4A
P 10800 1850
F 0 "#PWR039" H 10800 1600 50  0001 C CNN
F 1 "GND" H 10805 1677 50  0000 C CNN
F 2 "" H 10800 1850 50  0001 C CNN
F 3 "" H 10800 1850 50  0001 C CNN
	1    10800 1850
	1    0    0    -1  
$EndComp
Text Label 4800 3450 1    50   ~ 0
GLOBAL_5V
$Comp
L Device:CP C6
U 1 1 5B255F74
P 4900 3650
F 0 "C6" H 4700 3700 50  0000 L CNN
F 1 "10uF" V 4750 3450 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.24x1.80mm_HandSolder" H 4938 3500 50  0001 C CNN
F 3 "~" H 4900 3650 50  0001 C CNN
	1    4900 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 5B255F7B
P 5250 3650
F 0 "C8" H 5365 3696 50  0000 L CNN
F 1 "0.1uF" H 5250 3550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 5288 3500 50  0001 C CNN
F 3 "~" H 5250 3650 50  0001 C CNN
	1    5250 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3500 4900 3450
Connection ~ 4900 3450
Wire Wire Line
	4900 3450 4800 3450
Wire Wire Line
	5250 3500 5250 3450
Wire Wire Line
	5250 3450 4900 3450
Wire Wire Line
	4900 3850 4900 3800
$Comp
L power:GND #PWR022
U 1 1 5B255F8F
P 5050 3850
F 0 "#PWR022" H 5050 3600 50  0001 C CNN
F 1 "GND" H 4950 3750 50  0000 C CNN
F 2 "" H 5050 3850 50  0001 C CNN
F 3 "" H 5050 3850 50  0001 C CNN
	1    5050 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3850 5250 3800
Wire Wire Line
	7100 1750 7400 1750
Wire Wire Line
	7100 1850 7400 1850
Text Label 5250 2050 2    50   ~ 0
V_BCKUP
Text Label 7400 2550 0    50   ~ 0
RESET
Text Notes 8200 1350 2    50   ~ 0
GPIO35-39 are input pins w/o internal pullup capability
Wire Wire Line
	6250 2350 5800 2350
Text Label 5800 2350 2    50   ~ 0
MISO
Text Label 5800 2250 2    50   ~ 0
MOSI
Wire Wire Line
	5800 2250 6250 2250
Text Label 7400 3050 0    50   ~ 0
SCK
Wire Wire Line
	7400 2450 7100 2450
Text Label 7400 2850 0    50   ~ 0
CSN
Wire Wire Line
	7400 2550 7100 2550
Wire Wire Line
	7400 2850 7100 2850
Wire Wire Line
	2200 1400 2150 1400
$Comp
L Device:C C2
U 1 1 5AE064A3
P 3150 1700
F 0 "C2" H 2900 1750 50  0000 L CNN
F 1 "0.1uF" H 2850 1650 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 3188 1550 50  0001 C CNN
F 3 "~" H 3150 1700 50  0001 C CNN
	1    3150 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1550 3150 1300
Connection ~ 3150 1300
Wire Wire Line
	3150 1300 3350 1300
$Comp
L Device:C C3
U 1 1 5AE15768
P 3350 1700
F 0 "C3" H 3465 1746 50  0000 L CNN
F 1 "10uF" H 3465 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 3388 1550 50  0001 C CNN
F 3 "http://datasheets.avx.com/TAJ.pdf" H 3350 1700 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/avx-corporation/TAJA106K006RNJ/478-1653-1-ND/564685" H 3350 1700 50  0001 C CNN "purchase page"
	1    3350 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1550 3350 1300
Wire Wire Line
	3350 1300 3400 1300
Connection ~ 3350 1300
$Comp
L power:GND #PWR011
U 1 1 5AE347D8
P 3250 2000
F 0 "#PWR011" H 3250 1750 50  0001 C CNN
F 1 "GND" V 3255 1872 50  0000 R CNN
F 2 "" H 3250 2000 50  0001 C CNN
F 3 "" H 3250 2000 50  0001 C CNN
	1    3250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1850 3150 1950
Wire Wire Line
	3150 1950 3250 1950
Wire Wire Line
	3350 1950 3350 1850
Wire Wire Line
	3250 2000 3250 1950
Connection ~ 3250 1950
Wire Wire Line
	3250 1950 3350 1950
Text Label 1650 3100 0    50   ~ 0
BRD_GPS_1PPS
$Comp
L Jumper:Jumper_3_Open JP6
U 1 1 5AE84E08
P 1600 2800
F 0 "JP6" H 1600 3024 50  0000 C CNN
F 1 "Jumper_3_Open" H 1600 2933 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 1600 2800 50  0001 C CNN
F 3 "~" H 1600 2800 50  0001 C CNN
	1    1600 2800
	1    0    0    -1  
$EndComp
Text Label 1900 2800 0    50   ~ 0
PPS_VISUAL
Wire Wire Line
	1900 2800 1850 2800
Wire Notes Line
	2350 2360 2350 3390
$Comp
L Jumper:SolderJumper_2_Open JP19
U 1 1 5AEEED80
P 10300 4050
F 0 "JP19" H 10650 4050 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 10300 4164 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 10300 4050 50  0001 C CNN
F 3 "~" H 10300 4050 50  0001 C CNN
	1    10300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 4050 10550 4050
Wire Wire Line
	10550 4050 10550 4200
Connection ~ 10550 4200
Wire Wire Line
	10550 4200 10500 4200
Wire Wire Line
	10150 4050 10050 4050
Wire Wire Line
	10050 4050 10050 4200
Connection ~ 10050 4200
Wire Wire Line
	10050 4200 10100 4200
$Comp
L Connector_Generic:Conn_02x07_Odd_Even J5
U 1 1 5AF1E970
P 9650 3000
F 0 "J5" H 9700 3517 50  0000 C CNN
F 1 "Conn_02x07_Odd_Even" H 9700 3426 50  0000 C CNN
F 2 "lib_fp:lan_waveshare_8720" H 9650 3000 50  0001 C CNN
F 3 "http://sullinscorp.com/drawings/101_P(N)PxCxx2LJBN-RC,_10494-H.pdf" H 9650 3000 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/sullins-connector-solutions/PPPC072LJBN-RC/S5560-ND/776018" H 9650 3000 50  0001 C CNN "purchase_page"
	1    9650 3000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7100 2150 7400 2150
Text Label 7400 2150 0    50   ~ 0
RMII_TX_EN
Wire Wire Line
	7100 2350 7400 2350
Text Label 7400 2350 0    50   ~ 0
RMII_TX0
Text Label 7400 1850 0    50   ~ 0
RMII_TX1
Wire Wire Line
	6250 2450 5800 2450
Text Label 5800 2450 2    50   ~ 0
RMII-RX0
Wire Wire Line
	6250 2550 5800 2550
Text Label 5800 2550 2    50   ~ 0
RMII_RX1
Wire Wire Line
	6250 2650 5800 2650
Text Label 5800 2650 2    50   ~ 0
RMII_CRS_DV
Text Label 7400 1750 0    50   ~ 0
RMII_MDC
Wire Wire Line
	7100 2750 7400 2750
Text Label 7400 2750 0    50   ~ 0
RMII_MDIO
Text Label 5250 2150 2    50   ~ 0
TEMP_DATA
Wire Wire Line
	7400 3050 7100 3050
Text Label 5800 2750 2    50   ~ 0
GSM_TX
Wire Wire Line
	5800 2750 6250 2750
Wire Wire Line
	5800 2850 6250 2850
Wire Wire Line
	7100 3150 7400 3150
Text Label 7400 3150 0    50   ~ 0
BRD_GPS_TX
Text Label 3400 1300 1    50   ~ 0
GLOBAL_3V3
NoConn ~ 3750 1300
Wire Wire Line
	9350 2700 9300 2700
Wire Wire Line
	9350 2800 9300 2800
Wire Wire Line
	9350 2900 9300 2900
Wire Wire Line
	9350 3000 9300 3000
Wire Wire Line
	9850 2800 9900 2800
Wire Wire Line
	9850 2900 9900 2900
$Comp
L Device:Jumper JP17
U 1 1 5B1BC618
P 5600 3450
F 0 "JP17" H 5600 3400 50  0000 C CNN
F 1 "Jumper" H 5650 3600 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x02_P2.54mm_staggered_Vertical" H 5600 3450 50  0001 C CNN
F 3 "~" H 5600 3450 50  0001 C CNN
	1    5600 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3450 5250 3450
Connection ~ 5250 3450
Wire Wire Line
	5900 3450 6250 3450
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 5B2068E3
P 5550 6350
F 0 "J4" H 5700 6150 50  0000 C CNN
F 1 "Conn_01x04" H 5150 6000 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x04_P2.54mm_staggered_Vertical" H 5550 6350 50  0001 C CNN
F 3 "~" H 5550 6350 50  0001 C CNN
	1    5550 6350
	-1   0    0    1   
$EndComp
NoConn ~ 9050 1000
NoConn ~ 10350 900 
NoConn ~ 10350 800 
Text Notes 5500 3950 0    50   ~ 0
Handle strapping pins GPIO 0, 2, 5, 12, 15 on boot\n(Set as outputs or handle their state on reset if inputs)
Text Label 8600 5200 2    50   ~ 0
LoRa_5V
Wire Wire Line
	8600 5200 8650 5200
Text Label 10850 5200 0    50   ~ 0
LoRa_5V
Text Label 10600 6000 0    50   ~ 0
MOSI
Wire Wire Line
	10000 5300 9900 5300
Text Label 9900 5300 2    50   ~ 0
3
$Comp
L power:GND #PWR036
U 1 1 5ADE0F7D
P 10850 5300
F 0 "#PWR036" H 10850 5050 50  0001 C CNN
F 1 "GND" H 10855 5127 50  0000 C CNN
F 2 "" H 10850 5300 50  0001 C CNN
F 3 "" H 10850 5300 50  0001 C CNN
	1    10850 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 5300 10850 5300
NoConn ~ 10500 5500
NoConn ~ 10500 5600
Wire Wire Line
	10500 6200 10600 6200
Text Label 10600 6200 0    50   ~ 0
22
NoConn ~ 10500 6300
NoConn ~ 10000 5500
NoConn ~ 10000 5600
Wire Wire Line
	10000 5700 9900 5700
Text Label 9900 5700 2    50   ~ 0
11
$Comp
L power:GND #PWR05
U 1 1 5AEB44AA
P 1400 5550
F 0 "#PWR05" H 1400 5300 50  0001 C CNN
F 1 "GND" H 1405 5377 50  0000 C CNN
F 2 "" H 1400 5550 50  0001 C CNN
F 3 "" H 1400 5550 50  0001 C CNN
	1    1400 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 5550 1400 5500
$Comp
L power:GND #PWR07
U 1 1 5AEC5F37
P 1700 5550
F 0 "#PWR07" H 1700 5300 50  0001 C CNN
F 1 "GND" H 1705 5377 50  0000 C CNN
F 2 "" H 1700 5550 50  0001 C CNN
F 3 "" H 1700 5550 50  0001 C CNN
	1    1700 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5550 1700 5500
Wire Wire Line
	1700 5500 1800 5500
NoConn ~ 2300 5500
NoConn ~ 850  5500
$Comp
L power:GND #PWR010
U 1 1 5AF0A224
P 2700 5550
F 0 "#PWR010" H 2700 5300 50  0001 C CNN
F 1 "GND" H 2705 5377 50  0000 C CNN
F 2 "" H 2700 5550 50  0001 C CNN
F 3 "" H 2700 5550 50  0001 C CNN
	1    2700 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5550 2700 5500
NoConn ~ 3200 5500
$Comp
L power:GND #PWR06
U 1 1 5AF2C0FA
P 1550 6100
F 0 "#PWR06" H 1550 5850 50  0001 C CNN
F 1 "GND" H 1555 5927 50  0000 C CNN
F 2 "" H 1550 6100 50  0001 C CNN
F 3 "" H 1550 6100 50  0001 C CNN
	1    1550 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 6100 1550 6050
Wire Wire Line
	1550 6050 1650 6050
Text Label 2200 6050 0    50   ~ 0
RESET
Text Label 2650 6050 2    50   ~ 0
CSN
Wire Wire Line
	2200 6050 2150 6050
Wire Wire Line
	2700 6050 2650 6050
$Comp
L power:GND #PWR01
U 1 1 5AFC6A4F
P 700 6900
F 0 "#PWR01" H 700 6650 50  0001 C CNN
F 1 "GND" H 705 6727 50  0000 C CNN
F 2 "" H 700 6900 50  0001 C CNN
F 3 "" H 700 6900 50  0001 C CNN
	1    700  6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	700  6900 700  6850
Wire Wire Line
	700  6850 850  6850
Text Label 1400 6850 0    50   ~ 0
SCK
Wire Wire Line
	1800 6850 1850 6850
Text Label 2400 6850 0    50   ~ 0
CSN
$Comp
L Device:Jumper_NC_Dual JP9
U 1 1 5AC244E1
P 2100 6850
F 0 "JP9" H 2100 7089 50  0000 C CNN
F 1 "Jumper_NC_Dual" H 2100 6998 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x03_P2.54mm_staggered_Vertical" H 2100 6850 50  0001 C CNN
F 3 "~" H 2100 6850 50  0001 C CNN
	1    2100 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 6850 2350 6850
Wire Wire Line
	2200 7000 2100 7000
Text Label 2800 6850 2    50   ~ 0
RESET
Wire Wire Line
	2800 6850 2850 6850
Text Label 750  7400 2    50   ~ 0
SCK
NoConn ~ 1350 7400
NoConn ~ 1900 7400
$Comp
L power:GND #PWR09
U 1 1 5B08E3BD
P 2450 7450
F 0 "#PWR09" H 2450 7200 50  0001 C CNN
F 1 "GND" H 2455 7277 50  0000 C CNN
F 2 "" H 2450 7450 50  0001 C CNN
F 3 "" H 2450 7450 50  0001 C CNN
	1    2450 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 7450 2450 7400
Wire Wire Line
	2450 7400 2400 7400
NoConn ~ 2700 7400
$Comp
L power:GND #PWR013
U 1 1 5B0B2221
P 3300 7400
F 0 "#PWR013" H 3300 7150 50  0001 C CNN
F 1 "GND" H 3305 7227 50  0000 C CNN
F 2 "" H 3300 7400 50  0001 C CNN
F 3 "" H 3300 7400 50  0001 C CNN
	1    3300 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 7400 3200 7400
NoConn ~ 3750 6850
$Comp
L power:GND #PWR017
U 1 1 5B0D62C7
P 4300 6850
F 0 "#PWR017" H 4300 6600 50  0001 C CNN
F 1 "GND" H 4305 6677 50  0000 C CNN
F 2 "" H 4300 6850 50  0001 C CNN
F 3 "" H 4300 6850 50  0001 C CNN
	1    4300 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 6850 4250 6850
Wire Notes Line
	4540 6535 4540 7100
$Comp
L dk_Coaxial-Connectors-RF:0734120114 J1
U 1 1 5B167719
P 900 3000
F 0 "J1" H 894 2513 60  0000 C CNN
F 1 "0734120114" H 894 2619 60  0000 C CNN
F 2 "digikey-footprints:Molex_734120114_UMC_RF_CONN_Vertical" H 1100 3200 60  0001 L CNN
F 3 "http://www.molex.com/pdm_docs/sd/734120110_sd.pdf" H 1100 3300 60  0001 L CNN
F 4 "WM3894CT-ND" H 1100 3400 60  0001 L CNN "Digi-Key_PN"
F 5 "0734120114" H 1100 3500 60  0001 L CNN "MPN"
F 6 "Connectors, Interconnects" H 1100 3600 60  0001 L CNN "Category"
F 7 "Coaxial Connectors (RF)" H 1100 3700 60  0001 L CNN "Family"
F 8 "http://www.molex.com/pdm_docs/sd/734120110_sd.pdf" H 1100 3800 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/molex-llc/0734120114/WM3894CT-ND/2421931" H 1100 3900 60  0001 L CNN "DK_Detail_Page"
F 10 "CONN UMC RCPT STR 50 OHM SMD" H 1100 4000 60  0001 L CNN "Description"
F 11 "Molex, LLC" H 1100 4100 60  0001 L CNN "Manufacturer"
F 12 "Active" H 1100 4200 60  0001 L CNN "Status"
	1    900  3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	1100 2800 1350 2800
Wire Wire Line
	1650 3100 1600 3100
Wire Wire Line
	1600 3100 1600 2950
$Comp
L power:GND #PWR04
U 1 1 5B194EB9
P 1200 3000
F 0 "#PWR04" H 1200 2750 50  0001 C CNN
F 1 "GND" H 1350 2900 50  0000 C CNN
F 2 "" H 1200 3000 50  0001 C CNN
F 3 "" H 1200 3000 50  0001 C CNN
	1    1200 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 3000 1200 2900
Wire Wire Line
	1200 2900 1100 2900
Text Notes 5500 3750 0    50   ~ 0
Remove jumper on 5v rail when USB programming \nto avoid circulating currents between supplies.
$Comp
L Device:CP C10
U 1 1 5AE00D90
P 9550 5400
F 0 "C10" H 9350 5450 50  0000 L CNN
F 1 "220uF" H 9300 5300 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.8" H 9588 5250 50  0001 C CNN
F 3 "http://nichicon-us.com/english/products/pdfs/e-uud.pdf" H 9550 5400 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/nichicon/UUD0J221MCL1GS/493-2236-1-ND/590211" H 9550 5400 50  0001 C CNN "purchase page"
	1    9550 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5250 9550 5200
Connection ~ 9550 5200
Wire Wire Line
	9550 5200 10000 5200
Wire Wire Line
	9250 5250 9250 5200
Connection ~ 9250 5200
Wire Wire Line
	9250 5200 9550 5200
Wire Wire Line
	10500 5200 10850 5200
Text Label 10750 3100 0    50   ~ 0
RMII_MDIO
Text Label 9900 2800 0    50   ~ 0
RMII_TX_EN
Text Label 9850 2700 0    50   ~ 0
RMII_TX1
Text Label 9300 3100 2    50   ~ 0
RMII_MDC
Text Label 9900 2900 0    50   ~ 0
RMII-RX0
Text Label 9300 3000 2    50   ~ 0
RMII_CRS_DV
Text Label 9300 2900 2    50   ~ 0
RMII_RX1
$Comp
L power:GND #PWR037
U 1 1 5AF132A3
P 11100 3300
F 0 "#PWR037" H 11100 3050 50  0001 C CNN
F 1 "GND" H 11105 3127 50  0000 C CNN
F 2 "" H 11100 3300 50  0001 C CNN
F 3 "" H 11100 3300 50  0001 C CNN
	1    11100 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5AF22DE2
P 8700 3300
F 0 "#PWR029" H 8700 3050 50  0001 C CNN
F 1 "GND" H 8705 3127 50  0000 C CNN
F 2 "" H 8700 3300 50  0001 C CNN
F 3 "" H 8700 3300 50  0001 C CNN
	1    8700 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 3200 8700 3300
Wire Wire Line
	9250 3300 9350 3300
Text Label 9300 2800 2    50   ~ 0
RMII_TX0
Text Notes 11200 3950 2    50   ~ 0
Ethernet PHY connector(Waveshare LAN8720)
$Comp
L Device:CP C13
U 1 1 5AD9A97F
P 10150 3450
F 0 "C13" H 9850 3500 50  0000 L CNN
F 1 "10uF" H 9850 3400 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.24x1.80mm_HandSolder" H 10188 3300 50  0001 C CNN
F 3 "http://datasheets.avx.com/TAJ.pdf" H 10150 3450 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/avx-corporation/TAJA106K006RNJ/478-1653-1-ND/564685" H 10150 3450 50  0001 C CNN "purchase page"
	1    10150 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C14
U 1 1 5AD9AB57
P 10450 3450
F 0 "C14" H 10565 3496 50  0000 L CNN
F 1 "0.1uF" H 10550 3400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 10488 3300 50  0001 C CNN
F 3 "~" H 10450 3450 50  0001 C CNN
	1    10450 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR042
U 1 1 5ADC9FA3
P 10300 3650
F 0 "#PWR042" H 10300 3400 50  0001 C CNN
F 1 "GND" H 10305 3477 50  0000 C CNN
F 2 "" H 10300 3650 50  0001 C CNN
F 3 "" H 10300 3650 50  0001 C CNN
	1    10300 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 3650 10150 3650
Wire Wire Line
	10150 3650 10150 3600
Connection ~ 10300 3650
Wire Wire Line
	9850 3200 11100 3200
Wire Wire Line
	10500 3300 10450 3300
Wire Wire Line
	9850 3300 10150 3300
Connection ~ 10150 3300
Wire Wire Line
	7100 2950 7900 2950
$Comp
L Device:R R7
U 1 1 5AE93230
P 7900 2750
F 0 "R7" H 7950 2700 50  0000 L CNN
F 1 "4.7k" V 7900 2650 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7830 2750 50  0001 C CNN
F 3 "~" H 7900 2750 50  0001 C CNN
	1    7900 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 2900 7900 2950
Text Label 7900 2550 1    50   ~ 0
GLOBAL_3V3
Wire Wire Line
	7900 2550 7900 2600
Connection ~ 7900 2950
Text Label 8150 2950 1    50   ~ 0
RMII_REFOSC_nINT
Wire Wire Line
	7900 2950 8150 2950
Text Label 10450 3000 0    50   ~ 0
RMII_REFOSC_nINT
Text Label 5250 1400 2    50   ~ 0
GLOBAL_3V3
Text Label 7400 2450 0    50   ~ 0
PWR_KEY
Wire Wire Line
	6250 1650 6000 1650
$Comp
L Device:R R3
U 1 1 5ADD9BA8
P 5450 1600
F 0 "R3" H 5300 1650 50  0000 L CNN
F 1 "10k" V 5450 1550 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5380 1600 50  0001 C CNN
F 3 "~" H 5450 1600 50  0001 C CNN
	1    5450 1600
	1    0    0    -1  
$EndComp
Text Label 7400 2650 0    50   ~ 0
GSM_RX
$Comp
L Device:R R8
U 1 1 5AE4C3B2
P 10150 3000
F 0 "R8" V 10100 2850 50  0000 C CNN
F 1 "33R" V 10150 3000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10080 3000 50  0001 C CNN
F 3 "~" H 10150 3000 50  0001 C CNN
	1    10150 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 3000 10000 3000
Wire Wire Line
	10300 3000 10450 3000
Wire Wire Line
	5450 1450 5450 1400
$Comp
L Device:Jumper JP18
U 1 1 5AED67E5
P 7100 4100
F 0 "JP18" H 7100 4050 50  0000 C CNN
F 1 "Jumper" H 7100 4000 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x02_P2.54mm_staggered_Vertical" H 7100 4100 50  0001 C CNN
F 3 "~" H 7100 4100 50  0001 C CNN
	1    7100 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 4100 6750 4100
Wire Wire Line
	7400 4100 7500 4100
Text Label 5350 3050 2    50   ~ 0
PHY_POWER_EN
$Comp
L Device:R R4
U 1 1 5AF11D23
P 5450 3200
F 0 "R4" V 5350 3150 50  0000 L CNN
F 1 "4.7k" V 5450 3100 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5380 3200 50  0001 C CNN
F 3 "~" H 5450 3200 50  0001 C CNN
	1    5450 3200
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR040
U 1 1 5AF12114
P 5200 3250
F 0 "#PWR040" H 5200 3000 50  0001 C CNN
F 1 "GND" H 5050 3150 50  0000 C CNN
F 2 "" H 5200 3250 50  0001 C CNN
F 3 "" H 5200 3250 50  0001 C CNN
	1    5200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3050 5650 3050
Wire Wire Line
	5650 3050 5650 3200
Wire Wire Line
	5650 3200 5600 3200
Connection ~ 5650 3050
Wire Wire Line
	5650 3050 6250 3050
Wire Wire Line
	5200 3250 5200 3200
Wire Wire Line
	5200 3200 5300 3200
Text Notes 11200 2350 2    50   ~ 0
RMII_REFOSC_nINT should be a 33 ohm transmission line
Text Label 6000 1650 2    50   ~ 0
3V3_REF
Wire Wire Line
	5250 2150 6250 2150
Wire Wire Line
	5250 1400 5450 1400
Text Label 5800 2850 2    50   ~ 0
NETWORK_STATUS
Text Label 10650 2500 2    50   ~ 0
GLOBAL_3V3
Text Label 10500 3300 0    50   ~ 0
GLOBAL_3V3
Text Notes 2355 3390 2    50   ~ 0
1PPS output to LED/ uFL connector
$Comp
L Connector_Specialized:Jack-DC J8
U 1 1 5B03C5B5
P 3600 4200
F 0 "J8" H 3500 4050 50  0000 C CNN
F 1 "Jack-DC" H 3600 3950 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 3650 4160 50  0001 C CNN
F 3 "https://www.cui.com/product/resource/pj-002a.pdf" H 3650 4160 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/cui-inc/PJ-002A/CP-002A-ND/96962" H 3600 4200 50  0001 C CNN "purchase page"
	1    3600 4200
	1    0    0    -1  
$EndComp
Text Label 5900 3450 0    50   ~ 0
ESP32_5v
Text Label 5850 6150 0    50   ~ 0
ESP32_5v
Wire Wire Line
	5750 6150 5850 6150
Text Label 6850 4400 2    50   ~ 0
(+)RAIL
Wire Wire Line
	7500 4400 7500 4100
$Comp
L Device:Jumper JP20
U 1 1 5B1637D0
P 7200 4400
F 0 "JP20" H 7200 4350 50  0000 C CNN
F 1 "Jumper" H 7200 4300 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x02_P2.54mm_staggered_Vertical" H 7200 4400 50  0001 C CNN
F 3 "~" H 7200 4400 50  0001 C CNN
	1    7200 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 4400 6900 4400
Wire Wire Line
	4200 5200 4150 5200
Text Label 4150 5200 2    50   ~ 0
PHY_POWER_EN
Wire Wire Line
	3500 5100 4200 5100
Wire Wire Line
	3500 5100 3500 5150
Wire Wire Line
	3500 5100 3450 5100
Connection ~ 3500 5100
Text Label 650  1400 1    50   ~ 0
GLOBAL_3V3
Text Label 5250 1850 2    50   ~ 0
BRD_GPS_RX
Wire Wire Line
	7400 2650 7100 2650
Wire Wire Line
	5250 2050 6250 2050
NoConn ~ 6250 1750
NoConn ~ 6250 3150
NoConn ~ 6250 3250
NoConn ~ 6250 3350
NoConn ~ 7100 3250
NoConn ~ 7100 3350
NoConn ~ 7100 3450
Wire Wire Line
	10500 6100 10600 6100
Text Label 10600 6100 0    50   ~ 0
20
NoConn ~ 10500 5700
Text Label 4200 850  0    50   ~ 0
GLOBAL_3V3
Wire Wire Line
	4200 850  4150 850 
$Comp
L Device:C C15
U 1 1 5AE0CC18
P 8950 5400
F 0 "C15" H 8750 5450 50  0000 L CNN
F 1 "0.1uF" H 8700 5300 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 8988 5250 50  0001 C CNN
F 3 "~" H 8950 5400 50  0001 C CNN
	1    8950 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C16
U 1 1 5AE0CCE7
P 9250 5400
F 0 "C16" H 9050 5450 50  0000 L CNN
F 1 "220uF" H 9000 5300 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.8" H 9288 5250 50  0001 C CNN
F 3 "http://nichicon-us.com/english/products/pdfs/e-uud.pdf" H 9250 5400 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/nichicon/UUD0J221MCL1GS/493-2236-1-ND/590211https://www.digikey.com/product-detail/en/nichicon/UUD0J221MCL1GS/493-2236-1-ND/590211" H 9250 5400 50  0001 C CNN "purchase page"
	1    9250 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 5250 8950 5200
Connection ~ 8950 5200
Wire Wire Line
	8950 5200 9250 5200
$Comp
L Device:C C9
U 1 1 5AE21608
P 8650 5400
F 0 "C9" H 8450 5450 50  0000 L CNN
F 1 "100pF" H 8400 5300 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 8688 5250 50  0001 C CNN
F 3 "http://datasheets.avx.com/C0GNP0-Dielectric.pdf" H 8650 5400 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/avx-corporation/06035A101FAT2A/478-6202-1-ND/2391401" H 8650 5400 50  0001 C CNN "purchase page"
	1    8650 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 5250 8650 5200
Connection ~ 8650 5200
Wire Wire Line
	8650 5200 8950 5200
$Comp
L power:GND #PWR031
U 1 1 5AE363B9
P 9100 5700
F 0 "#PWR031" H 9100 5450 50  0001 C CNN
F 1 "GND" H 9105 5527 50  0000 C CNN
F 2 "" H 9100 5700 50  0001 C CNN
F 3 "" H 9100 5700 50  0001 C CNN
	1    9100 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 5550 8650 5700
Wire Wire Line
	8650 5700 8950 5700
Wire Wire Line
	9100 5700 9250 5700
Wire Wire Line
	9550 5550 9550 5700
Connection ~ 9100 5700
Wire Wire Line
	8950 5550 8950 5700
Connection ~ 8950 5700
Wire Wire Line
	8950 5700 9100 5700
Wire Wire Line
	9250 5550 9250 5700
Connection ~ 9250 5700
Wire Wire Line
	9250 5700 9550 5700
Text Notes 8300 6350 0    50   ~ 0
In RHF0M301 Datasheet p20, filtering \ncircuit on RESET line is ignored as \nthere are no mechanical components \non that line.
$Comp
L Device:CP C11
U 1 1 5AF2E50E
P 10600 1600
F 0 "C11" H 10650 1700 50  0000 L CNN
F 1 "220uF" H 10650 1500 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.8" H 10638 1450 50  0001 C CNN
F 3 "http://nichicon-us.com/english/products/pdfs/e-uud.pdf" H 10600 1600 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/nichicon/UUD0J221MCL1GS/493-2236-1-ND/590211" H 10600 1600 50  0001 C CNN "purchase page"
	1    10600 1600
	1    0    0    -1  
$EndComp
Connection ~ 10450 3300
Wire Wire Line
	10150 3300 10450 3300
Wire Wire Line
	10450 3650 10450 3600
Wire Wire Line
	10300 3650 10450 3650
Wire Wire Line
	10800 1850 10600 1850
Wire Wire Line
	10600 1750 10600 1850
Wire Wire Line
	10800 1850 11000 1850
Wire Wire Line
	11000 1750 11000 1850
Connection ~ 10800 1850
Wire Wire Line
	10300 4550 10100 4550
Wire Wire Line
	10100 4550 9950 4550
Wire Wire Line
	9950 4350 9950 4550
Connection ~ 10100 4550
Wire Wire Line
	5000 5500 4500 5500
Connection ~ 4500 5500
Wire Wire Line
	4500 5500 3500 5500
Wire Wire Line
	4900 3850 5050 3850
Connection ~ 5050 3850
Wire Wire Line
	5050 3850 5250 3850
NoConn ~ 9300 2700
Wire Wire Line
	10400 1600 10400 1500
$Comp
L power:GND #PWR035
U 1 1 5ABA1E29
P 10400 1600
F 0 "#PWR035" H 10400 1350 50  0001 C CNN
F 1 "GND" H 10405 1427 50  0000 C CNN
F 2 "" H 10400 1600 50  0001 C CNN
F 3 "" H 10400 1600 50  0001 C CNN
	1    10400 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR045
U 1 1 5AE9742D
P 9000 1600
F 0 "#PWR045" H 9000 1350 50  0001 C CNN
F 1 "GND" H 9005 1427 50  0000 C CNN
F 2 "" H 9000 1600 50  0001 C CNN
F 3 "" H 9000 1600 50  0001 C CNN
	1    9000 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1600 9000 1500
Wire Wire Line
	9000 1500 9050 1500
Text Label 9000 800  2    50   ~ 0
STATUS
Text Label 5250 1950 2    50   ~ 0
STATUS
Wire Wire Line
	5250 1850 5450 1850
Connection ~ 5450 1850
Wire Wire Line
	5450 1850 6250 1850
Wire Wire Line
	5450 1750 5450 1850
Text Notes 3450 7450 0    50   ~ 0
Conformance standards\n====================\n1. IEC 60950-1 Information technology equipment standard(general)\n2. IEC 60950-22 Information technology equipment-standard(outdoor installation)
$Comp
L Device:R R5
U 1 1 5ADF0459
P 4750 2100
F 0 "R5" H 4800 2050 50  0000 L CNN
F 1 "10k" V 4750 2050 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4680 2100 50  0001 C CNN
F 3 "~" H 4750 2100 50  0001 C CNN
	1    4750 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1950 6250 1950
$Comp
L power:GND #PWR044
U 1 1 5AE07D9E
P 4750 2300
F 0 "#PWR044" H 4750 2050 50  0001 C CNN
F 1 "GND" H 4900 2200 50  0000 C CNN
F 2 "" H 4750 2300 50  0001 C CNN
F 3 "" H 4750 2300 50  0001 C CNN
	1    4750 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2300 4750 2250
Wire Wire Line
	9000 800  9050 800 
$Comp
L Device:Fuse_Small F2
U 1 1 5AE3A097
P 7900 4100
F 0 "F2" H 7850 4050 50  0000 L CNN
F 1 "Fuse_Small" H 7700 4150 50  0000 L CNN
F 2 "lib_fp:461_series_Telelink" H 7900 4100 50  0001 C CNN
F 3 "http://www.littelfuse.com/~/media/electronics/datasheets/fuses/littelfuse_fuse_461_datasheet.pdf.pdf" H 7900 4100 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/littelfuse-inc/0461002.ER/F1551CT-ND/447217" H 7900 4100 50  0001 C CNN "purchase page"
	1    7900 4100
	1    0    0    -1  
$EndComp
Text Notes 5680 5230 0    50   ~ 0
The fuse is a slow blow fuse, that lets downstream components\nwork but opens after sustained surge duration. Example:\nMelting (i^2)*r=17 therefore at 2A buck limit, 4.25s to melt.
Wire Notes Line
	3420 7100 6970 7100
Wire Notes Line
	5655 5230 8270 5230
$Comp
L power:GND #PWR043
U 1 1 5B037B9F
P 3000 5050
F 0 "#PWR043" H 3000 4800 50  0001 C CNN
F 1 "GND" H 3150 5000 50  0000 C CNN
F 2 "" H 3000 5050 50  0001 C CNN
F 3 "" H 3000 5050 50  0001 C CNN
	1    3000 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 5050 3000 5000
Wire Wire Line
	2950 4700 3000 4700
Text Notes 3400 4050 2    50   ~ 0
RMII TVS surge protection
$Comp
L Device:D_TVS D12
U 1 1 5B192C2B
P 8100 4350
F 0 "D12" V 8100 4150 50  0000 L CNN
F 1 "D_TVS" V 8150 4100 50  0001 L CNN
F 2 "digikey-footprints:DO-214AC" H 8100 4350 50  0001 C CNN
F 3 "http://www.littelfuse.com/~/media/electronics/datasheets/tvs_diodes/littelfuse_tvs_diode_smaj_datasheet.pdf.pdf" H 8100 4350 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/littelfuse-inc/SMAJ5.0A/SMAJ5.0ALFCT-ND/762462" V 8100 4350 50  0001 C CNN "purchase page"
	1    8100 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 4100 7700 4100
Connection ~ 7500 4100
Wire Wire Line
	8000 4100 8100 4100
Wire Wire Line
	8100 4100 8100 4200
Wire Wire Line
	8250 4100 8100 4100
Connection ~ 8100 4100
Text Notes 5650 5400 0    50   ~ 0
The TVS diode protects the downstream components at Vwm 5v,  \nVbr 6.4-7.0v and Vc 9.2v and peak pulse current(8/20) 43.5A 
$Comp
L Device:Fuse_Small F1
U 1 1 5B26C3F4
P 4300 4100
F 0 "F1" H 4150 4050 50  0000 L CNN
F 1 "Fuse_Small" H 4100 4000 50  0001 L CNN
F 2 "lib_fp:461_series_Telelink" H 4300 4100 50  0001 C CNN
F 3 "http://www.littelfuse.com/~/media/electronics/datasheets/fuses/littelfuse_fuse_461_datasheet.pdf.pdf" H 4300 4100 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/littelfuse-inc/0461002.ER/F1551CT-ND/447217" H 4300 4100 50  0001 C CNN "purchase page"
	1    4300 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:D_TVS D11
U 1 1 5B2860A1
P 4450 4350
F 0 "D11" V 4450 4400 50  0000 L CNN
F 1 "D_TVS" V 4550 4400 50  0001 L CNN
F 2 "digikey-footprints:DO-214AC" H 4450 4350 50  0001 C CNN
F 3 "http://www.littelfuse.com/~/media/electronics/datasheets/tvs_diodes/littelfuse_tvs_diode_sma6j_datasheet.pdf.pdf" H 4450 4350 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/littelfuse-inc/SMA6J12A/SMA6J12ATR-ND/3997484" V 4450 4350 50  0001 C CNN "purchase page"
	1    4450 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 4200 4450 4100
Wire Wire Line
	4450 4500 4450 4700
Connection ~ 4450 4700
Wire Wire Line
	3900 4200 3900 4300
Wire Wire Line
	3900 4300 3900 4700
Wire Wire Line
	3900 4700 4050 4700
Connection ~ 3900 4300
Wire Wire Line
	3900 4100 4050 4100
Wire Wire Line
	6750 4700 7700 4700
Wire Wire Line
	8100 4700 7700 4700
Wire Wire Line
	8100 4500 8100 4700
Connection ~ 7700 4700
$Comp
L Graphic:SYM_ESD_Large #SYM1
U 1 1 5B37928B
P 7250 6700
F 0 "#SYM1" H 7250 6900 50  0001 C CNN
F 1 "SYM_ESD_Large" H 7250 6450 50  0001 C CNN
F 2 "" H 7245 6670 50  0001 C CNN
F 3 "~" H 7245 6670 50  0001 C CNN
	1    7250 6700
	1    0    0    -1  
$EndComp
$Comp
L Graphic:SYM_Radio_Waves_Large #SYM2
U 1 1 5B37BDDF
P 7900 6700
F 0 "#SYM2" H 7900 6900 50  0001 C CNN
F 1 "SYM_Radio_Waves_Large" H 7900 6450 50  0001 C CNN
F 2 "" H 7900 6500 50  0001 C CNN
F 3 "~" H 7930 6500 50  0001 C CNN
	1    7900 6700
	1    0    0    -1  
$EndComp
$Comp
L GDT-:GDT- GDT1
U 1 1 5B37CA10
P 4050 4400
F 0 "GDT1" V 4200 4400 50  0000 L CNN
F 1 "GDT-" V 4095 4506 50  0001 L CNN
F 2 "lib_fp:SL1411A075-SM" H 4050 4400 50  0001 C CNN
F 3 "http://www.littelfuse.com/~/media/electronics/datasheets/gas_discharge_tubes/littelfuse_gdt_sl1011a_sl1411a_datasheet.pdf.pdf" H 4050 4400 50  0001 L BNN
F 4 "https://www.digikey.com/product-detail/en/littelfuse-inc/SL1411A075A/F5590CT-ND/3307063" V 4050 4400 50  0001 C CNN "purchase page"
	1    4050 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	4050 4200 4050 4100
Connection ~ 4050 4100
Wire Wire Line
	4050 4600 4050 4700
Connection ~ 4050 4700
Wire Wire Line
	4050 4700 4450 4700
Wire Wire Line
	7700 4200 7700 4100
Connection ~ 7700 4100
Wire Wire Line
	7700 4100 7500 4100
Wire Wire Line
	7700 4600 7700 4700
Wire Notes Line
	5655 5405 8270 5405
Text Notes 5650 5600 0    50   ~ 0
The initial GDT provides 10kA lightning protection at the power \ninput. It also reduces the voltage down to 60-90v, 75v typ
Wire Wire Line
	4050 4100 4200 4100
Connection ~ 4450 4100
Wire Wire Line
	4450 4100 4600 4100
$Comp
L GDT-:GDT- GDT2
U 1 1 5AE0C554
P 7700 4400
F 0 "GDT2" V 7800 4150 50  0000 L CNN
F 1 "GDT-" V 7900 4150 50  0001 L CNN
F 2 "lib_fp:SL1411A075-SM" H 7700 4400 50  0001 C CNN
F 3 "http://www.littelfuse.com/~/media/electronics/datasheets/gas_discharge_tubes/littelfuse_gdt_sl1011a_sl1411a_datasheet.pdf.pdf" H 7700 4400 50  0001 L BNN
F 4 "https://www.digikey.com/product-detail/en/littelfuse-inc/SL1411A075A/F5590CT-ND/3307063" V 7700 4400 50  0001 C CNN "purchase page"
	1    7700 4400
	0    1    1    0   
$EndComp
Wire Notes Line
	3420 4970 8270 4970
Wire Wire Line
	4400 4100 4450 4100
Wire Wire Line
	3000 4500 3000 4450
$Comp
L power:GND #PWR038
U 1 1 5B15992F
P 3000 4500
F 0 "#PWR038" H 3000 4250 50  0001 C CNN
F 1 "GND" H 3150 4450 50  0000 C CNN
F 2 "" H 3000 4500 50  0001 C CNN
F 3 "" H 3000 4500 50  0001 C CNN
	1    3000 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 4150 2950 4150
Wire Wire Line
	2350 4700 2300 4700
Text Label 2950 4150 2    50   ~ 0
RMII_REFOSC_nINT
Text Label 2300 4700 2    50   ~ 0
RMII-RX0
Wire Wire Line
	2150 4150 2200 4150
Text Label 2150 4150 2    50   ~ 0
RMII_TX_EN
Wire Wire Line
	2200 4500 2200 4450
$Comp
L power:GND #PWR032
U 1 1 5B06BCB2
P 2200 4500
F 0 "#PWR032" H 2200 4250 50  0001 C CNN
F 1 "GND" H 2350 4450 50  0000 C CNN
F 2 "" H 2200 4500 50  0001 C CNN
F 3 "" H 2200 4500 50  0001 C CNN
	1    2200 4500
	1    0    0    -1  
$EndComp
Text Label 2950 4700 2    50   ~ 0
RMII_MDC
Wire Wire Line
	1050 4150 950  4150
Wire Wire Line
	1050 4500 1050 4450
$Comp
L power:GND #PWR020
U 1 1 5B0053E7
P 1050 4500
F 0 "#PWR020" H 1050 4250 50  0001 C CNN
F 1 "GND" H 1200 4450 50  0000 C CNN
F 2 "" H 1050 4500 50  0001 C CNN
F 3 "" H 1050 4500 50  0001 C CNN
	1    1050 4500
	1    0    0    -1  
$EndComp
Text Label 950  4150 2    50   ~ 0
RMII_CRS_DV
Wire Wire Line
	1750 4700 1800 4700
Text Label 1750 4700 2    50   ~ 0
RMII_RX1
Wire Wire Line
	1250 4700 1300 4700
Text Label 1250 4700 2    50   ~ 0
RMII_TX0
Wire Wire Line
	850  5050 850  5000
$Comp
L power:GND #PWR015
U 1 1 5AFB9D63
P 850 5050
F 0 "#PWR015" H 850 4800 50  0001 C CNN
F 1 "GND" H 1000 5000 50  0000 C CNN
F 2 "" H 850 5050 50  0001 C CNN
F 3 "" H 850 5050 50  0001 C CNN
	1    850  5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  4700 850  4700
Text Label 800  4700 2    50   ~ 0
RMII_TX1
Wire Wire Line
	2350 5050 2350 5000
$Comp
L power:GND #PWR033
U 1 1 5AF411D0
P 2350 5050
F 0 "#PWR033" H 2350 4800 50  0001 C CNN
F 1 "GND" H 2500 5000 50  0000 C CNN
F 2 "" H 2350 5050 50  0001 C CNN
F 3 "" H 2350 5050 50  0001 C CNN
	1    2350 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 5050 1800 5000
$Comp
L power:GND #PWR030
U 1 1 5AF2998C
P 1800 5050
F 0 "#PWR030" H 1800 4800 50  0001 C CNN
F 1 "GND" H 1950 5000 50  0000 C CNN
F 2 "" H 1800 5050 50  0001 C CNN
F 3 "" H 1800 5050 50  0001 C CNN
	1    1800 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 5050 1300 5000
$Comp
L power:GND #PWR021
U 1 1 5AF1230E
P 1300 5050
F 0 "#PWR021" H 1300 4800 50  0001 C CNN
F 1 "GND" H 1450 5000 50  0000 C CNN
F 2 "" H 1300 5050 50  0001 C CNN
F 3 "" H 1300 5050 50  0001 C CNN
	1    1300 5050
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Mounting_Hole_PAD MK1
U 1 1 5B00EC0F
P 3600 5800
F 0 "MK1" H 3700 5851 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 3700 5760 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 3600 5800 50  0001 C CNN
F 3 "" H 3600 5800 50  0001 C CNN
	1    3600 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR046
U 1 1 5B00EEAC
P 3600 5950
F 0 "#PWR046" H 3600 5700 50  0001 C CNN
F 1 "GND" H 3500 5950 50  0000 C CNN
F 2 "" H 3600 5950 50  0001 C CNN
F 3 "" H 3600 5950 50  0001 C CNN
	1    3600 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 5950 3600 5900
Wire Notes Line
	3420 6055 8270 6055
Wire Notes Line
	8270 470  8270 6535
$Comp
L Mechanical:Mounting_Hole_PAD MK2
U 1 1 5B0B997C
P 4850 5800
F 0 "MK2" H 4950 5851 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 4950 5760 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 4850 5800 50  0001 C CNN
F 3 "" H 4850 5800 50  0001 C CNN
	1    4850 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5B0B9983
P 4850 5950
F 0 "#PWR047" H 4850 5700 50  0001 C CNN
F 1 "GND" H 4750 5950 50  0000 C CNN
F 2 "" H 4850 5950 50  0001 C CNN
F 3 "" H 4850 5950 50  0001 C CNN
	1    4850 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 5950 4850 5900
$Comp
L Mechanical:Mounting_Hole_PAD MK3
U 1 1 5B0D5B8F
P 6000 5800
F 0 "MK3" H 6100 5851 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 6100 5760 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 6000 5800 50  0001 C CNN
F 3 "" H 6000 5800 50  0001 C CNN
	1    6000 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR048
U 1 1 5B0D5B96
P 6000 5950
F 0 "#PWR048" H 6000 5700 50  0001 C CNN
F 1 "GND" H 5900 5950 50  0000 C CNN
F 2 "" H 6000 5950 50  0001 C CNN
F 3 "" H 6000 5950 50  0001 C CNN
	1    6000 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 5950 6000 5900
Text Notes 8270 6055 2    50   ~ 0
Grounded mounting holes \nfor safe enclosure discharge
Wire Wire Line
	1550 4150 1600 4150
Wire Wire Line
	1600 4500 1600 4450
$Comp
L power:GND #PWR050
U 1 1 5B16862E
P 1600 4500
F 0 "#PWR050" H 1600 4250 50  0001 C CNN
F 1 "GND" H 1750 4450 50  0000 C CNN
F 2 "" H 1600 4500 50  0001 C CNN
F 3 "" H 1600 4500 50  0001 C CNN
	1    1600 4500
	1    0    0    -1  
$EndComp
Text Label 1550 4150 2    50   ~ 0
RMII_MDIO
Wire Wire Line
	11100 3300 11100 3200
Wire Notes Line
	3420 7100 3420 7795
Wire Notes Line
	3420 6535 3420 3975
Wire Notes Line
	3420 6535 6970 6535
$Comp
L Graphic:SYM_Arrow_XLarge #SYM3
U 1 1 5B3B5A0B
P 1700 6400
F 0 "#SYM3" H 1700 6490 50  0001 C CNN
F 1 "SYM_Arrow_XLarge" H 1700 6320 50  0001 C CNN
F 2 "" H 1700 6400 50  0001 C CNN
F 3 "~" H 1700 6400 50  0001 C CNN
	1    1700 6400
	-1   0    0    1   
$EndComp
$Comp
L Graphic:SYM_Arrow_XLarge #SYM4
U 1 1 5B3B5B56
P 2100 6400
F 0 "#SYM4" H 2100 6490 50  0001 C CNN
F 1 "SYM_Arrow_XLarge" H 2100 6320 50  0001 C CNN
F 2 "" H 2100 6400 50  0001 C CNN
F 3 "~" H 2100 6400 50  0001 C CNN
	1    2100 6400
	1    0    0    -1  
$EndComp
Wire Notes Line
	850  6350 3200 6350
Wire Notes Line
	3200 6350 3200 6550
Wire Notes Line
	3200 6550 850  6550
Wire Notes Line
	850  6550 850  6350
Wire Notes Line
	1900 6550 1900 6350
$Comp
L Device:D_TVS D10
U 1 1 5AF0C44B
P 3000 4850
F 0 "D10" V 2950 4650 50  0000 L CNN
F 1 "D_TVS" V 3050 4600 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 3000 4850 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 3000 4850 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 3000 4850 50  0001 C CNN "purchase page"
	1    3000 4850
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 5B4136D8
P 10650 2700
F 0 "R9" H 10700 2700 50  0000 L CNN
F 1 "1.5k" V 10650 2600 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10580 2700 50  0001 C CNN
F 3 "~" H 10650 2700 50  0001 C CNN
	1    10650 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 3100 9350 3100
Wire Wire Line
	10650 2850 10650 3100
Wire Wire Line
	10650 3100 10750 3100
Wire Wire Line
	9850 3100 10650 3100
Connection ~ 10650 3100
Wire Wire Line
	10650 2550 10650 2500
Text Notes 8300 3750 0    50   ~ 0
IEEE 802.3 specifies a 1.5k 5% pullup \non MDIO line.
$Comp
L Device:D_TVS D9
U 1 1 5B51D991
P 2350 4850
F 0 "D9" V 2300 4650 50  0000 L CNN
F 1 "D_TVS" V 2400 4600 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 2350 4850 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 2350 4850 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 2350 4850 50  0001 C CNN "purchase page"
	1    2350 4850
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D7
U 1 1 5B51DA51
P 1800 4850
F 0 "D7" V 1750 4650 50  0000 L CNN
F 1 "D_TVS" V 1850 4600 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 1800 4850 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 1800 4850 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 1800 4850 50  0001 C CNN "purchase page"
	1    1800 4850
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D5
U 1 1 5B51DB11
P 1300 4850
F 0 "D5" V 1250 4650 50  0000 L CNN
F 1 "D_TVS" V 1350 4600 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 1300 4850 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 1300 4850 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 1300 4850 50  0001 C CNN "purchase page"
	1    1300 4850
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D3
U 1 1 5B51DBCD
P 850 4850
F 0 "D3" V 800 4650 50  0000 L CNN
F 1 "D_TVS" V 900 4600 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 850 4850 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 850 4850 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 850 4850 50  0001 C CNN "purchase page"
	1    850  4850
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D4
U 1 1 5B51DC8B
P 1050 4300
F 0 "D4" V 1000 4100 50  0000 L CNN
F 1 "D_TVS" V 1100 4050 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 1050 4300 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 1050 4300 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 1050 4300 50  0001 C CNN "purchase page"
	1    1050 4300
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D6
U 1 1 5B51DD59
P 1600 4300
F 0 "D6" V 1550 4100 50  0000 L CNN
F 1 "D_TVS" V 1650 4050 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 1600 4300 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 1600 4300 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 1600 4300 50  0001 C CNN "purchase page"
	1    1600 4300
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D8
U 1 1 5B51DE21
P 2200 4300
F 0 "D8" V 2150 4100 50  0000 L CNN
F 1 "D_TVS" V 2250 4050 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 2200 4300 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 2200 4300 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 2200 4300 50  0001 C CNN "purchase page"
	1    2200 4300
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D13
U 1 1 5B51DEEB
P 3000 4300
F 0 "D13" V 2950 4100 50  0000 L CNN
F 1 "D_TVS" V 3050 4050 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 3000 4300 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 3000 4300 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 3000 4300 50  0001 C CNN "purchase page"
	1    3000 4300
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D14
U 1 1 5B521B32
P 900 3650
F 0 "D14" V 850 3450 50  0000 L CNN
F 1 "D_TVS" V 950 3400 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 900 3650 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 900 3650 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 900 3650 50  0001 C CNN "purchase page"
	1    900  3650
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D15
U 1 1 5B521CFA
P 1600 3650
F 0 "D15" V 1550 3450 50  0000 L CNN
F 1 "D_TVS" V 1650 3400 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 1600 3650 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 1600 3650 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 1600 3650 50  0001 C CNN "purchase page"
	1    1600 3650
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D16
U 1 1 5B521DC8
P 2250 3650
F 0 "D16" V 2200 3450 50  0000 L CNN
F 1 "D_TVS" V 2300 3400 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 2250 3650 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 2250 3650 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 2250 3650 50  0001 C CNN "purchase page"
	1    2250 3650
	0    1    1    0   
$EndComp
$Comp
L Device:D_TVS D17
U 1 1 5B521EA2
P 2950 3650
F 0 "D17" V 2900 3450 50  0000 L CNN
F 1 "D_TVS" V 3000 3400 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 2950 3650 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 2950 3650 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 2950 3650 50  0001 C CNN "purchase page"
	1    2950 3650
	0    1    1    0   
$EndComp
Text Label 800  3500 2    50   ~ 0
CSN
Wire Wire Line
	800  3500 900  3500
Text Label 1500 3500 2    50   ~ 0
MISO
Wire Wire Line
	1500 3500 1600 3500
Text Label 2150 3500 2    50   ~ 0
SCK
Wire Wire Line
	2150 3500 2250 3500
Text Label 2800 3500 2    50   ~ 0
RESET
Wire Wire Line
	2800 3500 2950 3500
$Comp
L Device:D_TVS D18
U 1 1 5B5D80AE
P 3700 3650
F 0 "D18" V 3650 3450 50  0000 L CNN
F 1 "D_TVS" V 3750 3400 50  0000 L CNN
F 2 "Diode_SMD:D_0402_1005Metric" H 3700 3650 50  0001 C CNN
F 3 "https://www.semtech.com/uploads/documents/uclamp3311pq.pdf" H 3700 3650 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/semtech-corporation/UCLAMP3311PQTCT/UCLAMP3311PQTCTCT-ND/4626706" V 3700 3650 50  0001 C CNN "purchase page"
	1    3700 3650
	0    1    1    0   
$EndComp
Text Label 3550 3500 2    50   ~ 0
MOSI
Wire Wire Line
	3550 3500 3700 3500
$Comp
L power:GND #PWR055
U 1 1 5B5F83F4
P 3700 3850
F 0 "#PWR055" H 3700 3600 50  0001 C CNN
F 1 "GND" H 3850 3750 50  0000 C CNN
F 2 "" H 3700 3850 50  0001 C CNN
F 3 "" H 3700 3850 50  0001 C CNN
	1    3700 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3850 3700 3800
$Comp
L power:GND #PWR054
U 1 1 5B616FE7
P 2950 3850
F 0 "#PWR054" H 2950 3600 50  0001 C CNN
F 1 "GND" H 3100 3750 50  0000 C CNN
F 2 "" H 2950 3850 50  0001 C CNN
F 3 "" H 2950 3850 50  0001 C CNN
	1    2950 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3850 2950 3800
$Comp
L power:GND #PWR053
U 1 1 5B635DFC
P 2250 3850
F 0 "#PWR053" H 2250 3600 50  0001 C CNN
F 1 "GND" H 2400 3750 50  0000 C CNN
F 2 "" H 2250 3850 50  0001 C CNN
F 3 "" H 2250 3850 50  0001 C CNN
	1    2250 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 3850 2250 3800
$Comp
L power:GND #PWR052
U 1 1 5B654E48
P 1600 3850
F 0 "#PWR052" H 1600 3600 50  0001 C CNN
F 1 "GND" H 1750 3750 50  0000 C CNN
F 2 "" H 1600 3850 50  0001 C CNN
F 3 "" H 1600 3850 50  0001 C CNN
	1    1600 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 3850 1600 3800
$Comp
L power:GND #PWR051
U 1 1 5B6740E3
P 900 3850
F 0 "#PWR051" H 900 3600 50  0001 C CNN
F 1 "GND" H 1050 3750 50  0000 C CNN
F 2 "" H 900 3850 50  0001 C CNN
F 3 "" H 900 3850 50  0001 C CNN
	1    900  3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  3850 900  3800
Text Notes 4700 3500 2    50   ~ 0
SPI TVS surge protection
$Comp
L Connector_Generic:Conn_02x12_Odd_Even J7
U 1 1 5ABD0D0D
P 10200 5700
F 0 "J7" H 10250 6417 50  0000 C CNN
F 1 "Conn_02x12_Odd_Even" H 10250 6326 50  0000 C CNN
F 2 "lib_fp:Hybrid_concentrator" H 10200 5700 50  0001 C CNN
F 3 "http://docs.rakwireless.com/en/RAK831%20LoRa%20Gateway/Hardware%20Design/RAK831%20Datasheet%20V1.3.pdf" H 10200 5700 50  0001 C CNN
	1    10200 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP21
U 1 1 5B7F475F
P 8250 4550
F 0 "JP21" H 8250 4850 50  0000 C CNN
F 1 "Jumper" H 8250 4800 50  0000 C CNN
F 2 "lib_fp:PinHeader_1x02_P2.54mm_staggered_Vertical" H 8250 4550 50  0001 C CNN
F 3 "~" H 8250 4550 50  0001 C CNN
	1    8250 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8250 4100 8250 4250
Wire Wire Line
	8250 4850 7600 4850
Wire Wire Line
	8700 3200 9350 3200
Text Label 9250 3300 2    50   ~ 0
GLOBAL_3V3
$Sheet
S 5700 600  1350 550 
U 5AE62D7C
F0 "Sheet5AE62D7B" 50
F1 "SYSTEM_LEDS.sch" 50
F2 "PPS_VISUAL" I L 5700 700 50 
F3 "GLOBAL_3V3" I R 7050 700 50 
F4 "GLOBAL_5V" I R 7050 950 50 
F5 "NETWORK_STATUS" I L 5700 950 50 
$EndSheet
Text Label 5450 700  2    50   ~ 0
PPS_VISUAL
Text Label 5650 950  2    50   ~ 0
NETWORK_STATUS
Wire Wire Line
	5650 950  5700 950 
Wire Wire Line
	5450 700  5700 700 
Wire Wire Line
	7050 700  7400 700 
Wire Wire Line
	7050 950  7400 950 
Text Label 7400 700  0    50   ~ 0
GLOBAL_3V3
Text Label 7400 950  0    50   ~ 0
GLOBAL_5V
Text Notes 8270 1220 2    50   ~ 0
System LED(s) block
Wire Wire Line
	7100 5950 7100 5900
$Comp
L power:GND #PWR049
U 1 1 5B0F20B8
P 7100 5950
F 0 "#PWR049" H 7100 5700 50  0001 C CNN
F 1 "GND" H 7000 5950 50  0000 C CNN
F 2 "" H 7100 5950 50  0001 C CNN
F 3 "" H 7100 5950 50  0001 C CNN
	1    7100 5950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Mounting_Hole_PAD MK4
U 1 1 5B0F20B1
P 7100 5800
F 0 "MK4" H 7200 5851 50  0000 L CNN
F 1 "Mounting_Hole_PAD" H 7200 5760 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 7100 5800 50  0001 C CNN
F 3 "" H 7100 5800 50  0001 C CNN
	1    7100 5800
	1    0    0    -1  
$EndComp
$EndSCHEMATC