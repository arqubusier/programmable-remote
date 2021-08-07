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
L Connector:Conn_01x01_Male J?
U 1 1 60BCCB41
P 2600 5300
F 0 "J?" H 2708 5481 50  0000 C CNN
F 1 "Conn_01x01_Male" H 2750 5200 50  0000 C CNN
F 2 "" H 2600 5300 50  0001 C CNN
F 3 "~" H 2600 5300 50  0001 C CNN
	1    2600 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60BCDAEA
P 2250 2900
F 0 "D?" H 2243 3117 50  0000 C CNN
F 1 "LED" H 2243 3026 50  0000 C CNN
F 2 "" H 2250 2900 50  0001 C CNN
F 3 "~" H 2250 2900 50  0001 C CNN
	1    2250 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60BCEE39
P 2250 2200
F 0 "D?" H 2243 2417 50  0000 C CNN
F 1 "LED" H 2243 2326 50  0000 C CNN
F 2 "" H 2250 2200 50  0001 C CNN
F 3 "~" H 2250 2200 50  0001 C CNN
	1    2250 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60BCF699
P 2250 1800
F 0 "D?" H 2243 2017 50  0000 C CNN
F 1 "LED" H 2243 1926 50  0000 C CNN
F 2 "" H 2250 1800 50  0001 C CNN
F 3 "~" H 2250 1800 50  0001 C CNN
	1    2250 1800
	1    0    0    -1  
$EndComp
$Comp
L Interface_Optical:TSOP43xx U?
U 1 1 60BD0275
P 4150 6750
F 0 "U?" H 3684 6708 50  0000 R CNN
F 1 "TSOP43xx" H 3684 6799 50  0000 R CNN
F 2 "OptoDevice:Vishay_MOLD-3Pin" H 4100 6375 50  0001 C CNN
F 3 "http://www.vishay.com/docs/82460/tsop45.pdf" H 4800 7050 50  0001 C CNN
	1    4150 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 5900 8900 5900
Wire Wire Line
	8900 3900 9200 3900
Wire Wire Line
	9200 3900 9200 4300
Wire Wire Line
	8900 4300 9200 4300
Connection ~ 9200 4300
Wire Wire Line
	9200 4300 9200 4700
Wire Wire Line
	8900 4700 9200 4700
Connection ~ 9200 4700
Wire Wire Line
	9200 4700 9200 5100
Wire Wire Line
	8900 5100 9200 5100
Connection ~ 9200 5100
Wire Wire Line
	9200 5100 9200 5500
Wire Wire Line
	8900 5500 9200 5500
Connection ~ 9200 5500
Wire Wire Line
	9200 5500 9200 5900
$Comp
L Device:R R?
U 1 1 60C3FCAC
P 2750 2900
F 0 "R?" V 2543 2900 50  0000 C CNN
F 1 "20" V 2634 2900 50  0000 C CNN
F 2 "" V 2680 2900 50  0001 C CNN
F 3 "~" H 2750 2900 50  0001 C CNN
	1    2750 2900
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 60C4B223
P 2750 2200
F 0 "R?" V 2543 2200 50  0000 C CNN
F 1 "68" V 2634 2200 50  0000 C CNN
F 2 "" V 2680 2200 50  0001 C CNN
F 3 "~" H 2750 2200 50  0001 C CNN
	1    2750 2200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 60C4BF80
P 2750 1800
F 0 "R?" V 2543 1800 50  0000 C CNN
F 1 "68" V 2634 1800 50  0000 C CNN
F 2 "" V 2680 1800 50  0001 C CNN
F 3 "~" H 2750 1800 50  0001 C CNN
	1    2750 1800
	0    1    1    0   
$EndComp
Wire Wire Line
	2900 1800 3150 1800
Wire Wire Line
	3150 2200 2900 2200
Wire Wire Line
	2600 2200 2400 2200
Wire Wire Line
	2600 1800 2400 1800
Wire Wire Line
	2900 2900 3150 2900
Wire Wire Line
	2600 2900 2400 2900
Text Notes 1200 1400 0    50   ~ 0
R=(Vdd - Vled)/Iled\nVred = 2 V, Ired = 20 mA, Rred = 65 Ohm\nVgreen = 2.1 ,V Igreen = 20 mA, Rgreen = 60 Ohm\n
Wire Wire Line
	2100 1800 1900 1800
Wire Wire Line
	1900 1800 1900 2200
Wire Wire Line
	1900 2900 2100 2900
Wire Wire Line
	2100 2200 1900 2200
Connection ~ 1900 2200
Wire Wire Line
	1900 2200 1900 2900
Text Label 3150 1800 0    50   ~ 0
STATUS
Text Label 3150 2200 0    50   ~ 0
FAIL
Text Label 3150 2900 0    50   ~ 0
IR_OUT
Text Label 3150 5300 0    50   ~ 0
USART_TX
$Comp
L Device:C C?
U 1 1 60BE8135
P 5000 6600
F 0 "C?" H 5150 6600 50  0000 L CNN
F 1 ".1u" H 5150 6500 50  0000 L CNN
F 2 "" H 5038 6450 50  0001 C CNN
F 3 "~" H 5000 6600 50  0001 C CNN
	1    5000 6600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60C0358E
P 5400 6450
F 0 "R?" V 5300 6450 50  0000 C CNN
F 1 "100" V 5500 6450 50  0000 C CNN
F 2 "" V 5330 6450 50  0001 C CNN
F 3 "~" H 5400 6450 50  0001 C CNN
	1    5400 6450
	0    1    1    0   
$EndComp
$Comp
L programmable-remote:M95640_RMN6TP U?
U 1 1 610CB268
P 2200 4450
F 0 "U?" H 2350 4900 50  0000 C CNN
F 1 "M95640_RMN6TP" H 2750 4350 50  0000 C CNN
F 2 "" H 2200 4450 50  0001 C CNN
F 3 "" H 2200 4450 50  0001 C CNN
	1    2200 4450
	-1   0    0    1   
$EndComp
$Comp
L Device:Battery BT?
U 1 1 610D4A1F
P 8050 2850
F 0 "BT?" H 8158 2896 50  0000 L CNN
F 1 "Battery" H 8158 2805 50  0000 L CNN
F 2 "" V 8050 2910 50  0001 C CNN
F 3 "~" V 8050 2910 50  0001 C CNN
	1    8050 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 610D8C9C
P 2950 4350
F 0 "R?" H 3020 4396 50  0000 L CNN
F 1 "4.7k" H 3020 4305 50  0000 L CNN
F 2 "" V 2880 4350 50  0001 C CNN
F 3 "~" H 2950 4350 50  0001 C CNN
	1    2950 4350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 610E1AA4
P 8750 2750
F 0 "J?" H 8700 2700 50  0000 C CNN
F 1 "Conn_01x04_Male" H 8600 2950 50  0000 C CNN
F 2 "" H 8750 2750 50  0001 C CNN
F 3 "~" H 8750 2750 50  0001 C CNN
	1    8750 2750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 610E3531
P 9900 4250
F 0 "SW?" H 9900 4535 50  0000 C CNN
F 1 "SW_Push" H 9900 4400 50  0000 C CNN
F 2 "" H 9900 4450 50  0001 C CNN
F 3 "~" H 9900 4450 50  0001 C CNN
	1    9900 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:Crystal Y?
U 1 1 610EE2E7
P 2200 6750
F 0 "Y?" H 2200 7018 50  0000 C CNN
F 1 "8 MHz" H 2200 6927 50  0000 C CNN
F 2 "" H 2200 6750 50  0001 C CNN
F 3 "~" H 2200 6750 50  0001 C CNN
	1    2200 6750
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 610EEAF2
P 1650 6450
F 0 "C?" H 1765 6496 50  0000 L CNN
F 1 "25p" H 1765 6405 50  0000 L CNN
F 2 "" H 1688 6300 50  0001 C CNN
F 3 "~" H 1650 6450 50  0001 C CNN
	1    1650 6450
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 610EF415
P 1650 7050
F 0 "C?" H 1765 7096 50  0000 L CNN
F 1 "25p" H 1765 7005 50  0000 L CNN
F 2 "" H 1688 6900 50  0001 C CNN
F 3 "~" H 1650 7050 50  0001 C CNN
	1    1650 7050
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D?
U 1 1 610F2DED
P 8050 2350
F 0 "D?" H 8050 2200 50  0000 C CNN
F 1 "D_Schottky" H 8100 2100 50  0000 C CNN
F 2 "" H 8050 2350 50  0001 C CNN
F 3 "~" H 8050 2350 50  0001 C CNN
	1    8050 2350
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D?
U 1 1 610F35EA
P 8950 2350
F 0 "D?" H 8950 2500 50  0000 C CNN
F 1 "D_Schottky" H 9050 2200 50  0000 C CNN
F 2 "" H 8950 2350 50  0001 C CNN
F 3 "~" H 8950 2350 50  0001 C CNN
	1    8950 2350
	0    -1   1    0   
$EndComp
Wire Wire Line
	2950 4500 2500 4500
Text Label 3150 4500 0    50   ~ 0
CS
Text Label 3150 4600 0    50   ~ 0
SCK
Text Label 3150 4700 0    50   ~ 0
MOSI
Text Label 1900 4700 2    50   ~ 0
MISO
Wire Wire Line
	8950 2650 8950 2500
Wire Wire Line
	8050 2650 8050 2500
Wire Wire Line
	8050 2200 8500 2200
Wire Wire Line
	8950 2950 8950 3050
Wire Wire Line
	8950 3050 8500 3050
Text Label 9600 2750 0    50   ~ 0
SWCLK
Text Label 9600 2850 0    50   ~ 0
SWDIO
$Comp
L power:PWR_FLAG #FLG?
U 1 1 612115B1
P 8500 2200
F 0 "#FLG?" H 8500 2275 50  0001 C CNN
F 1 "PWR_FLAG" H 8500 2373 50  0000 C CNN
F 2 "" H 8500 2200 50  0001 C CNN
F 3 "~" H 8500 2200 50  0001 C CNN
	1    8500 2200
	1    0    0    -1  
$EndComp
Connection ~ 8500 2200
Wire Wire Line
	8500 2200 8950 2200
$Comp
L power:GND #PWR?
U 1 1 612121B0
P 8500 3050
F 0 "#PWR?" H 8500 2800 50  0001 C CNN
F 1 "GND" H 8505 2877 50  0000 C CNN
F 2 "" H 8500 3050 50  0001 C CNN
F 3 "" H 8500 3050 50  0001 C CNN
	1    8500 3050
	1    0    0    -1  
$EndComp
Connection ~ 8500 3050
Wire Wire Line
	8500 3050 8050 3050
Text Label 9600 2200 0    50   ~ 0
V3_3
Text Label 9600 3050 0    50   ~ 0
V0
$Comp
L Switch:SW_Push SW?
U 1 1 60BCB460
P 8700 5900
F 0 "SW?" H 8700 6185 50  0000 C CNN
F 1 "SW_Push" H 8700 6094 50  0000 C CNN
F 2 "" H 8700 6100 50  0001 C CNN
F 3 "~" H 8700 6100 50  0001 C CNN
	1    8700 5900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 60BCB45A
P 8700 5500
F 0 "SW?" H 8700 5785 50  0000 C CNN
F 1 "SW_Push" H 8700 5694 50  0000 C CNN
F 2 "" H 8700 5700 50  0001 C CNN
F 3 "~" H 8700 5700 50  0001 C CNN
	1    8700 5500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 60BCB454
P 8700 5100
F 0 "SW?" H 8700 5385 50  0000 C CNN
F 1 "SW_Push" H 8700 5294 50  0000 C CNN
F 2 "" H 8700 5300 50  0001 C CNN
F 3 "~" H 8700 5300 50  0001 C CNN
	1    8700 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 60BC5F33
P 8700 4700
F 0 "SW?" H 8700 4985 50  0000 C CNN
F 1 "SW_Push" H 8700 4894 50  0000 C CNN
F 2 "" H 8700 4900 50  0001 C CNN
F 3 "~" H 8700 4900 50  0001 C CNN
	1    8700 4700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 60BC5BC2
P 8700 4300
F 0 "SW?" H 8700 4585 50  0000 C CNN
F 1 "SW_Push" H 8700 4494 50  0000 C CNN
F 2 "" H 8700 4500 50  0001 C CNN
F 3 "~" H 8700 4500 50  0001 C CNN
	1    8700 4300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 60BC1C19
P 8700 3900
F 0 "SW?" H 8700 4185 50  0000 C CNN
F 1 "SW_Push" H 8700 4094 50  0000 C CNN
F 2 "" H 8700 4100 50  0001 C CNN
F 3 "~" H 8700 4100 50  0001 C CNN
	1    8700 3900
	1    0    0    -1  
$EndComp
Text Label 8300 4300 2    50   ~ 0
KOK
Text Label 8300 3900 2    50   ~ 0
KESC
Text Label 8300 4700 2    50   ~ 0
K0
Text Label 8300 5100 2    50   ~ 0
K1
Text Label 8300 5500 2    50   ~ 0
K2
Text Label 8300 5900 2    50   ~ 0
K3
Wire Wire Line
	8300 3900 8500 3900
Wire Wire Line
	8300 4300 8500 4300
Wire Wire Line
	8300 4700 8500 4700
Wire Wire Line
	8300 5100 8500 5100
Wire Wire Line
	8300 5500 8500 5500
Wire Wire Line
	8300 5900 8500 5900
Text Label 9200 3900 0    50   ~ 0
V3_3
$Comp
L Device:C C?
U 1 1 61242CB5
P 10200 4200
F 0 "C?" H 10315 4246 50  0000 L CNN
F 1 ".1u" H 10315 4155 50  0000 L CNN
F 2 "" H 10238 4050 50  0001 C CNN
F 3 "~" H 10200 4200 50  0001 C CNN
	1    10200 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 4050 9900 3900
Wire Wire Line
	9900 3900 10200 3900
Wire Wire Line
	10200 3900 10200 4050
Wire Wire Line
	10200 4350 10200 4550
Wire Wire Line
	10200 4550 9900 4550
Wire Wire Line
	9900 4550 9900 4450
Text Label 10350 3900 0    50   ~ 0
NRST
Text Label 10350 4550 0    50   ~ 0
V0
Wire Wire Line
	10350 3900 10200 3900
Connection ~ 10200 3900
Wire Wire Line
	10350 4550 10200 4550
Connection ~ 10200 4550
Text Label 5650 6800 0    50   ~ 0
IR_IN
Text Label 5650 6450 0    50   ~ 0
V3_3
Text Label 5650 6950 0    50   ~ 0
V0
Text Label 3150 4150 0    50   ~ 0
V3_3
Text Label 3150 4900 0    50   ~ 0
V0
Wire Wire Line
	2200 4900 3150 4900
Wire Wire Line
	2100 4150 2950 4150
Wire Wire Line
	2950 4200 2950 4150
Connection ~ 2950 4150
Wire Wire Line
	2950 4150 3150 4150
Wire Wire Line
	3150 4500 2950 4500
Connection ~ 2950 4500
Wire Wire Line
	3150 4600 2500 4600
Wire Wire Line
	2500 4700 3150 4700
Wire Wire Line
	3150 5300 2800 5300
Wire Wire Line
	4550 6950 5000 6950
Wire Wire Line
	5000 6750 5000 6950
Connection ~ 5000 6950
Wire Wire Line
	5000 6950 5650 6950
Wire Wire Line
	4550 6550 4700 6550
Wire Wire Line
	4700 6550 4700 6450
Wire Wire Line
	4700 6450 5000 6450
Connection ~ 5000 6450
Wire Wire Line
	5000 6450 5250 6450
Wire Wire Line
	5550 6450 5650 6450
Wire Wire Line
	5650 6800 4700 6800
Wire Wire Line
	4700 6800 4700 6750
Wire Wire Line
	4700 6750 4550 6750
Text Label 6700 2000 0    50   ~ 0
KOK
Text Label 6700 1800 0    50   ~ 0
KESC
Text Label 6700 2100 0    50   ~ 0
K0
Text Label 6700 2200 0    50   ~ 0
K1
Text Label 6700 2300 0    50   ~ 0
K2
Text Label 6700 2400 0    50   ~ 0
K3
Text Label 6700 4900 0    50   ~ 0
MOSI
Text Label 6700 4800 0    50   ~ 0
MISO
Text Label 6700 4700 0    50   ~ 0
SCK
Text Label 6700 4600 0    50   ~ 0
CS
Text Label 4700 4700 2    50   ~ 0
STATUS
Text Label 4700 4900 2    50   ~ 0
FAIL
Text Label 6700 4000 0    50   ~ 0
IR_OUT
Text Label 6700 1900 0    50   ~ 0
USART_TX
Wire Wire Line
	4700 4900 5000 4900
Wire Wire Line
	4700 4700 5000 4700
Text Label 6700 1700 0    50   ~ 0
IR_IN
Text Label 4700 1700 2    50   ~ 0
NRST
Wire Wire Line
	4700 1700 5000 1700
Text Label 4700 3000 2    50   ~ 0
OSC_IN
Text Label 4700 3100 2    50   ~ 0
OSC_OUT
Wire Wire Line
	4700 3000 5000 3000
Wire Wire Line
	4700 3100 5000 3100
Text Label 2400 6450 0    50   ~ 0
OSC_IN
Text Label 2400 7050 0    50   ~ 0
OSC_OUT
Wire Wire Line
	2400 6450 2200 6450
Text Label 1250 7050 2    50   ~ 0
V0
Wire Wire Line
	1800 7050 2200 7050
Wire Wire Line
	2200 6900 2200 7050
Connection ~ 2200 7050
Wire Wire Line
	2200 7050 2400 7050
Wire Wire Line
	2200 6600 2200 6450
Connection ~ 2200 6450
Wire Wire Line
	2200 6450 1800 6450
Wire Wire Line
	1250 7050 1500 7050
Wire Wire Line
	1500 7050 1500 6450
Connection ~ 1500 7050
Wire Wire Line
	8950 2200 9200 2200
Connection ~ 8950 2200
Connection ~ 8950 3050
$Comp
L Device:C C?
U 1 1 613C61C6
P 9950 1350
F 0 "C?" H 10065 1396 50  0000 L CNN
F 1 "4.7u" H 10065 1305 50  0000 L CNN
F 2 "" H 9988 1200 50  0001 C CNN
F 3 "~" H 9950 1350 50  0001 C CNN
	1    9950 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 613C698D
P 8300 1350
F 0 "C?" H 8415 1396 50  0000 L CNN
F 1 ".1u" H 8415 1305 50  0000 L CNN
F 2 "" H 8338 1200 50  0001 C CNN
F 3 "~" H 8300 1350 50  0001 C CNN
	1    8300 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 613C7A7D
P 9400 1350
F 0 "C?" H 9515 1396 50  0000 L CNN
F 1 ".1u" H 9515 1305 50  0000 L CNN
F 2 "" H 9438 1200 50  0001 C CNN
F 3 "~" H 9400 1350 50  0001 C CNN
	1    9400 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 613C7F00
P 8850 1350
F 0 "C?" H 8965 1396 50  0000 L CNN
F 1 ".1u" H 8965 1305 50  0000 L CNN
F 2 "" H 8888 1200 50  0001 C CNN
F 3 "~" H 8850 1350 50  0001 C CNN
	1    8850 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4900 6700 4900
Wire Wire Line
	6700 4800 6400 4800
Wire Wire Line
	6400 4700 6700 4700
Wire Wire Line
	6700 4600 6400 4600
Wire Wire Line
	6700 4000 6400 4000
Wire Wire Line
	6400 2400 6700 2400
Wire Wire Line
	6700 2300 6400 2300
Wire Wire Line
	6400 2200 6700 2200
Wire Wire Line
	6700 2100 6400 2100
Wire Wire Line
	6400 2000 6700 2000
Wire Wire Line
	6700 1900 6400 1900
Wire Wire Line
	6700 1800 6400 1800
Wire Wire Line
	6700 1700 6400 1700
$Comp
L Device:C C?
U 1 1 613C4E64
P 10400 2200
F 0 "C?" H 10515 2246 50  0000 L CNN
F 1 "1u" H 10515 2155 50  0000 L CNN
F 2 "" H 10438 2050 50  0001 C CNN
F 3 "~" H 10400 2200 50  0001 C CNN
	1    10400 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 613C3361
P 10050 2200
F 0 "C?" H 10165 2246 50  0000 L CNN
F 1 "10n" H 10165 2155 50  0000 L CNN
F 2 "" H 10088 2050 50  0001 C CNN
F 3 "~" H 10050 2200 50  0001 C CNN
	1    10050 2200
	1    0    0    -1  
$EndComp
$Comp
L MCU_ST_STM32F1:STM32F103RBTx U?
U 1 1 610E5B85
P 5700 3300
F 0 "U?" H 5700 3250 50  0000 C CNN
F 1 "STM32F103RBTx" H 6400 1450 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5100 1600 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00161566.pdf" H 5700 3300 50  0001 C CNN
	1    5700 3300
	1    0    0    -1  
$EndComp
Text Label 8400 1200 0    50   ~ 0
VDD1
Text Label 8950 1200 0    50   ~ 0
VDD2
Text Label 10050 1200 0    50   ~ 0
VDD3
Text Label 10500 1200 0    50   ~ 0
VDD4
Wire Wire Line
	9400 1200 9950 1200
Text Label 8300 900  0    50   ~ 0
V3_3
Text Label 8300 1800 2    50   ~ 0
V0
Text Label 10400 2500 0    50   ~ 0
V0
Text Label 10400 1950 0    50   ~ 0
V3_3
Wire Wire Line
	8300 1200 8300 1050
Wire Wire Line
	8850 1650 8850 1550
Wire Wire Line
	8850 1050 8850 1200
Wire Wire Line
	9400 1050 9400 1200
Connection ~ 9400 1200
Wire Wire Line
	9400 1650 9400 1550
Wire Wire Line
	9950 1650 9950 1550
Wire Wire Line
	10400 1650 10400 1550
Wire Wire Line
	10400 1200 10400 1050
Wire Wire Line
	10400 1050 9400 1050
Wire Wire Line
	8300 1050 8300 900 
Connection ~ 8300 1050
Connection ~ 8850 1050
Wire Wire Line
	8850 1050 8300 1050
Connection ~ 9400 1050
Wire Wire Line
	9400 1050 8850 1050
Wire Wire Line
	8300 1500 8300 1550
Wire Wire Line
	8300 1650 8850 1650
Connection ~ 8850 1650
Wire Wire Line
	8850 1650 9400 1650
Connection ~ 9400 1650
Wire Wire Line
	9400 1650 9950 1650
Connection ~ 9950 1650
Wire Wire Line
	9950 1650 10400 1650
Wire Wire Line
	8300 1200 8400 1200
Connection ~ 8300 1200
Wire Wire Line
	8850 1200 8950 1200
Connection ~ 8850 1200
Wire Wire Line
	9950 1200 10050 1200
Connection ~ 9950 1200
Wire Wire Line
	10400 1200 10500 1200
Connection ~ 10400 1200
Wire Wire Line
	10400 2500 10400 2350
Wire Wire Line
	10050 2350 10400 2350
Connection ~ 10400 2350
Wire Wire Line
	10050 2050 10400 2050
Wire Wire Line
	10400 2050 10400 1950
Connection ~ 10400 2050
Text Label 10650 2050 0    50   ~ 0
VDDA
Wire Wire Line
	10400 2050 10650 2050
Text Label 10650 2350 0    50   ~ 0
VSSA
Wire Wire Line
	10400 2350 10650 2350
Text Label 8400 1550 0    50   ~ 0
VSS1
Text Label 8950 1550 0    50   ~ 0
VSS2
Text Label 10050 1550 0    50   ~ 0
VSS3
Text Label 10500 1550 0    50   ~ 0
VSS4
Wire Wire Line
	8300 1550 8400 1550
Connection ~ 8300 1550
Wire Wire Line
	8300 1550 8300 1650
Wire Wire Line
	8850 1550 8950 1550
Connection ~ 8850 1550
Wire Wire Line
	8850 1550 8850 1500
Wire Wire Line
	9950 1550 10050 1550
Connection ~ 9950 1550
Wire Wire Line
	9950 1550 9950 1500
Wire Wire Line
	10400 1550 10500 1550
Connection ~ 10400 1550
Wire Wire Line
	10400 1550 10400 1500
Wire Wire Line
	8300 1800 8300 1650
Connection ~ 8300 1650
Text Label 5700 1250 1    50   ~ 0
VDD1
Text Label 5800 1250 1    50   ~ 0
VDD2
Text Label 5900 1250 1    50   ~ 0
VDD3
Text Label 5600 1250 1    50   ~ 0
VDD4
Wire Wire Line
	5600 1500 5600 1250
Wire Wire Line
	5700 1250 5700 1500
Wire Wire Line
	5800 1500 5800 1250
Wire Wire Line
	5900 1250 5900 1500
Text Label 6000 1250 1    50   ~ 0
VDDA
Text Label 5500 1250 1    50   ~ 0
V3_3
Wire Wire Line
	5500 1250 5500 1500
Wire Wire Line
	6000 1250 6000 1500
Text Label 5600 5350 3    50   ~ 0
VSS1
Text Label 5700 5350 3    50   ~ 0
VSS2
Text Label 5800 5350 3    50   ~ 0
VSS3
Text Label 5500 5350 3    50   ~ 0
VSS4
Text Label 5900 5350 3    50   ~ 0
VSSA
Wire Wire Line
	5900 5350 5900 5100
Wire Wire Line
	5800 5100 5800 5350
Wire Wire Line
	5700 5350 5700 5100
Wire Wire Line
	5600 5100 5600 5350
Wire Wire Line
	5500 5350 5500 5100
Text Notes 8000 750  0    50   ~ 10
Power and decoupling
Text Notes 1200 950  0    50   ~ 10
Diodes
Text Notes 8100 3500 0    50   ~ 10
Buttons
Text Notes 1300 3900 0    50   ~ 10
Serial devices
Text Notes 1150 6200 0    50   ~ 10
Crystal
Text Notes 3300 6200 0    50   ~ 10
Ir sensing
Text Notes 750  7700 0    50   ~ 0
C_stray, the capacitance of the pcb trace and mcu is assumed to be 5 pF.\nC_l is the expected load capacitance of the\ncrystal. C is the capacitance from the capacitor at each leg.\nUse the following formula to determine C:\nC = 2*C_l - 2*C_stray = 2*18 - 10 [pF] =26 pF.
Wire Wire Line
	9950 1550 9400 1550
Connection ~ 9400 1550
Wire Wire Line
	9400 1550 9400 1500
$Comp
L Device:Fuse 500m
U 1 1 615B7066
P 9350 2200
F 0 "500m" V 9153 2200 50  0000 C CNN
F 1 "Fuse" V 9244 2200 50  0000 C CNN
F 2 "" V 9280 2200 50  0001 C CNN
F 3 "~" H 9350 2200 50  0001 C CNN
	1    9350 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	9500 2200 9600 2200
Wire Wire Line
	8950 2750 9600 2750
Wire Wire Line
	8950 2850 9600 2850
Wire Wire Line
	8950 3050 9600 3050
Text Label 1900 3150 0    50   ~ 0
V0
Wire Wire Line
	1900 3150 1900 2900
Connection ~ 1900 2900
$Comp
L Device:C C?
U 1 1 613C85A7
P 10400 1350
F 0 "C?" H 10515 1396 50  0000 L CNN
F 1 ".1u" H 10515 1305 50  0000 L CNN
F 2 "" H 10438 1200 50  0001 C CNN
F 3 "~" H 10400 1350 50  0001 C CNN
	1    10400 1350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
