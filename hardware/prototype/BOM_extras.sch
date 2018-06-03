EESchema Schematic File Version 4
LIBS:prototype-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title "ESP32 LoRa gateway with surge suppression"
Date "2018-04-30"
Rev "1"
Comp "Kaiote Limited"
Comment1 "Design by: Alois Mbutura"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x08 J15
U 1 1 5AEDADAD
P 1555 3355
F 0 "J15" H 1635 3347 50  0000 L CNN
F 1 "Conn_01x08" H 1635 3256 50  0000 L CNN
F 2 "" H 1555 3355 50  0001 C CNN
F 3 "~" H 1555 3355 50  0001 C CNN
F 4 "PPTC081LFBN-RC " H 1555 3355 50  0001 C CNN "manf#"
	1    1555 3355
	1    0    0    -1  
$EndComp
Wire Notes Line
	475  2150 11220 2150
Wire Notes Line
	475  4330 11220 4330
Text Notes 2940 4330 2    50   ~ 0
Cellular_module_female_sockets, quantity 2
$Comp
L Connector_Generic:Conn_01x19 J8
U 1 1 5AF3EE38
P 3310 3160
F 0 "J8" H 3390 3202 50  0000 L CNN
F 1 "Conn_01x19" H 3390 3111 50  0000 L CNN
F 2 "" H 3310 3160 50  0001 C CNN
F 3 "~" H 3310 3160 50  0001 C CNN
F 4 "PPTC191LFBN-RC " H 3310 3160 50  0001 C CNN "manf#"
	1    3310 3160
	1    0    0    -1  
$EndComp
Text Notes 4755 4330 2    50   ~ 0
ESP32_female_sockets, quantity 2
$Comp
L Connector_Generic:Conn_02x12_Odd_Even J10
U 1 1 5AF3F12C
P 5430 3160
F 0 "J10" H 5480 3877 50  0000 C CNN
F 1 "Conn_02x12_Odd_Even" H 5480 3786 50  0000 C CNN
F 2 "" H 5430 3160 50  0001 C CNN
F 3 "~" H 5430 3160 50  0001 C CNN
F 4 "PPPC072LJBN-RC" H 5430 3160 50  0001 C CNN "manf#"
	1    5430 3160
	1    0    0    -1  
$EndComp
Text Notes 6345 4330 2    50   ~ 0
LoRa_concentrator_male_socket
Wire Notes Line
	4755 2150 4755 4330
Wire Notes Line
	2935 2150 2935 4330
$Comp
L Connector_Generic:Conn_02x12_Odd_Even J11
U 1 1 5AF3FB4E
P 7005 3160
F 0 "J11" H 7055 3877 50  0000 C CNN
F 1 "Conn_02x12_Odd_Even" H 7055 3786 50  0000 C CNN
F 2 "" H 7005 3160 50  0001 C CNN
F 3 "~" H 7005 3160 50  0001 C CNN
F 4 "PPPC122LFBN-RC " H 7005 3160 50  0001 C CNN "manf#"
	1    7005 3160
	1    0    0    -1  
$EndComp
Text Notes 7860 4330 2    50   ~ 0
LoRa_concentrator_female_socket
Wire Notes Line
	7875 4330 7875 2150
Wire Notes Line
	8970 2145 8970 4325
$Comp
L Connector_Generic:Conn_02x07_Odd_Even J13
U 1 1 5AF41862
P 9440 2740
F 0 "J13" H 9490 3257 50  0000 C CNN
F 1 "Conn_02x07_Odd_Even" H 9490 3166 50  0000 C CNN
F 2 "" H 9440 2740 50  0001 C CNN
F 3 "~" H 9440 2740 50  0001 C CNN
F 4 "PPTC072LFBN-RC" H 9440 2740 50  0001 C CNN "manf#"
	1    9440 2740
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x07_Odd_Even J14
U 1 1 5AF419DF
P 9470 3675
F 0 "J14" H 9115 3265 50  0000 C CNN
F 1 "Conn_02x07_Odd_Even" H 9490 3185 50  0000 C CNN
F 2 "" H 9470 3675 50  0001 C CNN
F 3 "" H 9470 3675 50  0001 C CNN
F 4 "PPPC072LJBN-RC" H 9470 3675 50  0001 C CNN "manf#"
	1    9470 3675
	1    0    0    -1  
$EndComp
Text Notes 10140 4325 2    50   ~ 0
R.A_LAN8720_female_connector
Text Notes 10110 2230 2    50   ~ 0
LAN8720_female_connector
Text Notes 480  5860 0    50   ~ 0
Others\n======\n1. Waveshare_Lan_8720(10)\n2. CR2320 battery(3)\n3. DHT22 sensor(3)- Digikey\n4. MIKROE cellular module(3)-Digikey\n5. POE injector and splitter(3)-Digikey\n6. LM2596 module(3)\n7. Triton 2G/3G/4G antena-Digikey\n\nExtras(Distributed)\n=================\n1. 3mm thread tap(1)\n2. solder wire(2)\n3. Standoffs(3d printed)\n4. M3X?? Screws(self tapping?)(6)\n5. Cable tie set(1)- 10-15cm\n6. Polyurethane conformal coating(3)\n7. Coating plate
$Comp
L Connector_Generic:Conn_01x02 J12
U 1 1 5AF4244C
P 8270 3780
F 0 "J12" H 8350 3772 50  0000 L CNN
F 1 "Conn_01x02" H 8350 3681 50  0000 L CNN
F 2 "" H 8270 3780 50  0001 C CNN
F 3 "~" H 8270 3780 50  0001 C CNN
F 4 "SSC02SYAN" H 8270 3780 50  0001 C CNN "manf#"
	1    8270 3780
	1    0    0    -1  
$EndComp
Text Notes 8905 4310 2    50   ~ 0
Jumper, quantity: 19
$Comp
L Connector_Generic:Conn_01x08 J16
U 1 1 5AF6E6A8
P 2085 3350
F 0 "J16" H 2165 3342 50  0000 L CNN
F 1 "Conn_01x08" H 2165 3251 50  0000 L CNN
F 2 "" H 2085 3350 50  0001 C CNN
F 3 "~" H 2085 3350 50  0001 C CNN
F 4 "PPTC081LFBN-RC " H 2085 3350 50  0001 C CNN "manf#"
	1    2085 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x19 J9
U 1 1 5AF6E7D0
P 4235 3170
F 0 "J9" H 4315 3212 50  0000 L CNN
F 1 "Conn_01x19" H 4315 3121 50  0000 L CNN
F 2 "" H 4235 3170 50  0001 C CNN
F 3 "~" H 4235 3170 50  0001 C CNN
F 4 "PPTC191LFBN-RC " H 4235 3170 50  0001 C CNN "manf#"
	1    4235 3170
	1    0    0    -1  
$EndComp
Text Notes 11215 4325 2    50   ~ 0
RJ45 coupler housing
Wire Notes Line
	10075 2150 10075 6535
Text Notes 11210 6520 2    50   ~ 0
Ethernet line protection
Wire Notes Line
	7880 3110 10075 3110
$Comp
L Connector_Generic:Conn_01x04 J19
U 1 1 5AEDB274
P 8560 2590
F 0 "J19" H 8640 2582 50  0000 L CNN
F 1 "Conn_01x04" H 8640 2491 50  0000 L CNN
F 2 "" H 8560 2590 50  0001 C CNN
F 3 "~" H 8560 2590 50  0001 C CNN
F 4 "PPTC041LFBN-RC " H 8560 2590 50  0001 C CNN "manf#"
	1    8560 2590
	1    0    0    -1  
$EndComp
$Comp
L Device:Antenna AE1
U 1 1 5AEF8A88
P 9450 5050
F 0 "AE1" H 9530 5041 50  0000 L CNN
F 1 "Antenna" H 9530 4950 50  0000 L CNN
F 2 "" H 9450 5050 50  0001 C CNN
F 3 "~" H 9450 5050 50  0001 C CNN
F 4 "TG.10.0113 " H 9450 5050 50  0001 C CNN "manf#"
	1    9450 5050
	1    0    0    -1  
$EndComp
Wire Notes Line
	8970 4330 8970 6540
Text Notes 10070 6530 2    50   ~ 0
2G/3G/4G Terminal antenna
Wire Notes Line
	6345 6535 6970 6535
Wire Notes Line
	6345 2150 6345 6535
Text Notes 8930 6520 2    50   ~ 0
Passive POE cable assembly
$Comp
L Connector:RJ45 J17
U 1 1 5AF97458
P 10600 3150
F 0 "J17" H 10655 3817 50  0000 C CNN
F 1 "RJ45" H 10655 3726 50  0000 C CNN
F 2 "" V 10600 3175 50  0001 C CNN
F 3 "~" V 10600 3175 50  0001 C CNN
F 4 "RCP-5SPFFH-TCU7001 " H 10600 3150 50  0001 C CNN "manf#"
	1    10600 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector:RJ45_LED J18
U 1 1 5AF9804C
P 10650 5250
F 0 "J18" H 10650 5917 50  0000 C CNN
F 1 "RJ45_LED" H 10650 5826 50  0000 C CNN
F 2 "" V 10650 5275 50  0001 C CNN
F 3 "" V 10650 5275 50  0001 C CNN
F 4 "ESP-100-POE " H 10650 5250 50  0001 C CNN "manf#"
	1    10650 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector:RJ45_Shielded J20
U 1 1 5AF986FC
P 7500 5350
F 0 "J20" H 7555 6017 50  0000 C CNN
F 1 "RJ45_Shielded" H 7555 5926 50  0000 C CNN
F 2 "" V 7500 5375 50  0001 C CNN
F 3 "~" V 7500 5375 50  0001 C CNN
F 4 "1528-1773-ND " H 7500 5350 50  0001 C CNN "manf#"
	1    7500 5350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
