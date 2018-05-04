EESchema Schematic File Version 4
LIBS:prototype-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title "ESP32 LoRa gateway with surge suppression"
Date "2018-04-30"
Rev "1"
Comp "Kaiote Limited"
Comment1 "Design by: Alois Mbutura"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	1350 4000 1450 4000
$Comp
L Device:R R8
U 1 1 5AE81518
P 1800 3500
F 0 "R8" H 1700 3500 50  0000 C CNN
F 1 "110R" V 1800 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1730 3500 50  0001 C CNN
F 3 "http://www.vishay.com/docs/20035/dcrcwe3.pdf" H 1800 3500 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/vishay-dale/CRCW0805110RFKEA/541-110CCT-ND/1180673" H 1800 3500 50  0001 C CNN "purchase_page"
F 5 "CRCW0805110RFKEA" H 1800 3500 50  0001 C CNN "manf#"
	1    1800 3500
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR041
U 1 1 5AE8151F
P 1800 4210
F 0 "#PWR041" H 1800 3960 50  0001 C CNN
F 1 "GND" H 1925 4135 50  0000 C CNN
F 2 "" H 1800 4210 50  0001 C CNN
F 3 "" H 1800 4210 50  0001 C CNN
	1    1800 4210
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 4210 1800 4200
Text Notes 500  2450 0    50   ~ 0
GPS FIX indicator
$Comp
L Transistor_BJT:DTC143Z Q2
U 1 1 5AE81531
P 1700 4000
F 0 "Q2" H 1888 4046 50  0000 L CNN
F 1 "DTC143Z" H 1888 3955 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SC-59_Handsoldering" H 1700 4000 50  0001 L CNN
F 3 "http://www.rohm.com/web/global/datasheet/DTC143ZEB/dtc143zebtl-e" H 1700 4000 50  0001 L CNN
F 4 "https://www.digikey.com/product-detail/en/rohm-semiconductor/DTC143ZKAT146/DTC143ZKAT146CT-ND/650725" H 1700 4000 50  0001 C CNN "purchase page"
F 5 "DTC143ZKAT146" H 1700 4000 50  0001 C CNN "manf#"
	1    1700 4000
	1    0    0    -1  
$EndComp
Wire Notes Line
	500  2350 11200 2350
Wire Notes Line
	500  4350 11200 4350
Text HLabel 1350 4000 0    50   Input ~ 0
PPS_VISUAL
Text HLabel 1550 2900 0    50   Input ~ 0
GLOBAL_3V3
Text Notes 3400 2850 0    50   ~ 0
Network status LED\n=================\nToggles once every 500ms(2Hz) for a network error. Check network interfaces.\nOnce a ETH interface is sensed, moves from 2Hz(2sec) to 5Hz(1.5sec) finally 10Hz(1sec), reversed order for cell.\nToggles once rapidly every uplink delay sec(usually 30) for a successful uplink report via cellular.\nToggles twice rapidly every uplink delay sec(usually 30) for a successful uplink report via ethernet.
Wire Notes Line
	3400 2350 3400 4350
Wire Notes Line
	7800 2350 7800 4350
Wire Wire Line
	8850 2950 8600 2950
Wire Wire Line
	8850 3600 8850 3250
$Comp
L power:GND #PWR043
U 1 1 5AEBED95
P 8850 4000
F 0 "#PWR043" H 8850 3750 50  0001 C CNN
F 1 "GND" H 9000 3900 50  0000 C CNN
F 2 "" H 8850 4000 50  0001 C CNN
F 3 "" H 8850 4000 50  0001 C CNN
	1    8850 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 4000 8850 3900
Wire Notes Line
	9650 2350 9650 4350
Text HLabel 8600 2950 0    50   Input ~ 0
GLOBAL_3V3
$Comp
L Device:LED D6
U 1 1 5AEBF85D
P 10650 3100
F 0 "D6" V 10550 3000 50  0000 R CNN
F 1 "LED" V 10650 3000 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad0.97x1.50mm_HandSolder" H 10650 3100 50  0001 C CNN
F 3 "http://optoelectronics.liteon.com/upload/download/DS22-2000-109/LTST-C171KRKT.pdf" H 10650 3100 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/lite-on-inc/LTST-C171KRKT/160-1427-1-ND/386800" H 10650 3100 50  0001 C CNN "purchase page"
F 5 "LTST-C171KRKT" V 10650 3100 50  0001 C CNN "manf#"
	1    10650 3100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10650 2950 10400 2950
$Comp
L Device:R R11
U 1 1 5AEBF865
P 10650 3750
F 0 "R11" H 10750 3750 50  0000 C CNN
F 1 "300R" V 10650 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10580 3750 50  0001 C CNN
F 3 "https://industrial.panasonic.com/cdbs/www-data/pdf/RDM0000/AOA0000C328.pdf" H 10650 3750 50  0001 C CNN
F 4 "ERJ-PB6D3000V" H 10650 3750 50  0001 C CNN "manf#"
F 5 "https://www.digikey.com/product-detail/en/panasonic-electronic-components/ERJ-PB6D3000V/P20957CT-ND/6215212" H 10650 3750 50  0001 C CNN "purchase_page"
	1    10650 3750
	-1   0    0    1   
$EndComp
Wire Wire Line
	10650 3600 10650 3250
$Comp
L power:GND #PWR044
U 1 1 5AEBF86D
P 10650 4000
F 0 "#PWR044" H 10650 3750 50  0001 C CNN
F 1 "GND" H 10800 3900 50  0000 C CNN
F 2 "" H 10650 4000 50  0001 C CNN
F 3 "" H 10650 4000 50  0001 C CNN
	1    10650 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 4000 10650 3900
Text HLabel 10400 2950 0    50   Input ~ 0
GLOBAL_5V
Text Notes 7800 2450 0    50   ~ 0
3V3 power indicator
Text Notes 9650 2450 0    50   ~ 0
5v power indicator
$Comp
L Device:LED D4
U 1 1 5AEC051D
P 5600 3200
F 0 "D4" V 5550 3100 50  0000 R CNN
F 1 "LED" V 5650 3100 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad0.97x1.50mm_HandSolder" H 5600 3200 50  0001 C CNN
F 3 "http://optoelectronics.liteon.com/upload/download/DS22-2000-118/LTST-C171KGKT.pdf" H 5600 3200 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/lite-on-inc/LTST-C171KGKT/160-1426-1-ND/386798" H 5600 3200 50  0001 C CNN "purchase page"
F 5 "LTST-C171KGKT " V 5600 3200 50  0001 C CNN "manf#"
	1    5600 3200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5600 3050 5350 3050
$Comp
L Device:R R9
U 1 1 5AEC0525
P 5600 3850
F 0 "R9" H 5700 3850 50  0000 C CNN
F 1 "130R" V 5600 3850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5530 3850 50  0001 C CNN
F 3 "https://industrial.panasonic.com/cdbs/www-data/pdf/RDM0000/AOA0000C307.pdf" H 5600 3850 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/panasonic-electronic-components/ERA-6AEB131V/P130DACT-ND/1465926" H 5600 3850 50  0001 C CNN "purchase_page"
F 5 "ERA-6AEB131V" H 5600 3850 50  0001 C CNN "manf#"
	1    5600 3850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5600 3700 5600 3350
$Comp
L power:GND #PWR042
U 1 1 5AEC052D
P 5600 4100
F 0 "#PWR042" H 5600 3850 50  0001 C CNN
F 1 "GND" H 5750 4000 50  0000 C CNN
F 2 "" H 5600 4100 50  0001 C CNN
F 3 "" H 5600 4100 50  0001 C CNN
	1    5600 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4100 5600 4000
Text HLabel 5350 3050 0    50   Input ~ 0
NETWORK_STATUS
Wire Wire Line
	1800 3650 1800 3800
Wire Wire Line
	1800 3350 1800 3250
Wire Wire Line
	1550 2900 1800 2900
Wire Wire Line
	1800 2900 1800 2950
Text Notes 7800 4350 2    50   ~ 0
Green LED
Text Notes 3400 4350 2    50   ~ 0
Red_LED
Text Notes 9650 4350 2    50   ~ 0
Red_LED
Text Notes 11200 4350 2    50   ~ 0
Red_LED
$Comp
L Device:LED D5
U 1 1 5AEC1D57
P 8850 3100
F 0 "D5" V 8750 3000 50  0000 R CNN
F 1 "LED" V 8850 3000 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad0.97x1.50mm_HandSolder" H 8850 3100 50  0001 C CNN
F 3 "http://optoelectronics.liteon.com/upload/download/DS22-2000-109/LTST-C171KRKT.pdf" H 8850 3100 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/lite-on-inc/LTST-C171KRKT/160-1427-1-ND/386800" H 8850 3100 50  0001 C CNN "purchase page"
F 5 "LTST-C171KRKT" V 8850 3100 50  0001 C CNN "manf#"
	1    8850 3100
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D3
U 1 1 5B00BAB6
P 1800 3100
F 0 "D3" V 1700 3000 50  0000 R CNN
F 1 "LED" V 1800 3000 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad0.97x1.50mm_HandSolder" H 1800 3100 50  0001 C CNN
F 3 "http://optoelectronics.liteon.com/upload/download/DS22-2000-109/LTST-C171KRKT.pdf" H 1800 3100 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/lite-on-inc/LTST-C171KRKT/160-1427-1-ND/386800" H 1800 3100 50  0001 C CNN "purchase page"
F 5 "LTST-C171KRKT" V 1800 3100 50  0001 C CNN "manf#"
	1    1800 3100
	0    -1   -1   0   
$EndComp
Text Notes 500  2350 0    50   ~ 0
All LEDs at similar intensity, 10mA current
Text Notes 2200 3250 0    50   ~ 0
R = (3.3-2.2)/0.01
Text Notes 6400 3200 0    50   ~ 0
R = (3.3-2.0)/0.01
$Comp
L Device:R R10
U 1 1 5B00D1BF
P 8850 3750
F 0 "R10" H 8950 3750 50  0000 C CNN
F 1 "130R" V 8850 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8780 3750 50  0001 C CNN
F 3 "https://industrial.panasonic.com/cdbs/www-data/pdf/RDM0000/AOA0000C307.pdf" H 8850 3750 50  0001 C CNN
F 4 "ERA-6AEB131V" H 8850 3750 50  0001 C CNN "manf#"
F 5 "https://www.digikey.com/product-detail/en/panasonic-electronic-components/ERA-6AEB131V/P130DACT-ND/1465926" H 8850 3750 50  0001 C CNN "purchase_page"
	1    8850 3750
	-1   0    0    1   
$EndComp
Text Notes 8800 2800 0    50   ~ 0
R = (3.3-2.0)/0.01
Text Notes 10400 2800 0    50   ~ 0
R = (5.0-2.0)/0.01
Text Notes 3165 2455 0    50   ~ 0
DONE
Text Notes 7785 2445 2    50   ~ 0
DONE
Text Notes 9640 2450 2    50   ~ 0
DONE
Text Notes 11215 2440 2    50   ~ 0
DONE
$EndSCHEMATC
