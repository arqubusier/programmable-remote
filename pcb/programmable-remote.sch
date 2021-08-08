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
L Connector:Conn_01x01_Male J2
U 1 1 60BCCB41
P 2300 5700
F 0 "J2" H 2408 5881 50  0000 C CNN
F 1 "Conn_01x01_Male" H 2450 5600 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2300 5700 50  0001 C CNN
F 3 "~" H 2300 5700 50  0001 C CNN
	1    2300 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 60BCEE39
P 1350 1900
F 0 "D2" H 1343 2117 50  0000 C CNN
F 1 "LED" H 1343 2026 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 1350 1900 50  0001 C CNN
F 3 "~" H 1350 1900 50  0001 C CNN
	1    1350 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 60BCF699
P 1350 1500
F 0 "D1" H 1343 1717 50  0000 C CNN
F 1 "LED" H 1343 1626 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 1350 1500 50  0001 C CNN
F 3 "~" H 1350 1500 50  0001 C CNN
	1    1350 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 6000 8550 6000
Wire Wire Line
	8550 4000 8850 4000
Wire Wire Line
	8850 4000 8850 4400
Wire Wire Line
	8550 4400 8850 4400
Connection ~ 8850 4400
Wire Wire Line
	8850 4400 8850 4800
Wire Wire Line
	8550 4800 8850 4800
Connection ~ 8850 4800
Wire Wire Line
	8850 4800 8850 5200
Wire Wire Line
	8550 5200 8850 5200
Connection ~ 8850 5200
Wire Wire Line
	8850 5200 8850 5600
Wire Wire Line
	8550 5600 8850 5600
Connection ~ 8850 5600
Wire Wire Line
	8850 5600 8850 6000
$Comp
L Device:R R2
U 1 1 60C4B223
P 1850 1900
F 0 "R2" V 1643 1900 50  0000 C CNN
F 1 "68" V 1734 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 1780 1900 50  0001 C CNN
F 3 "~" H 1850 1900 50  0001 C CNN
	1    1850 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 60C4BF80
P 1850 1500
F 0 "R1" V 1643 1500 50  0000 C CNN
F 1 "68" V 1734 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 1780 1500 50  0001 C CNN
F 3 "~" H 1850 1500 50  0001 C CNN
	1    1850 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 1500 2250 1500
Wire Wire Line
	2250 1900 2000 1900
Wire Wire Line
	1700 1900 1500 1900
Wire Wire Line
	1700 1500 1500 1500
Text Notes 850  1200 0    50   ~ 0
R=(Vdd - Vled)/Iled\nVred = 2 V, Ired = 20 mA, Rred = 65 Ohm\nVgreen = 2.1 ,V Igreen = 20 mA, Rgreen = 60 Ohm\n
Wire Wire Line
	1200 1500 1000 1500
Wire Wire Line
	1000 1500 1000 1900
Wire Wire Line
	1200 1900 1000 1900
Text Label 2250 1500 0    50   ~ 0
STATUS
Text Label 2250 1900 0    50   ~ 0
FAIL
Text Label 2850 5700 0    50   ~ 0
USART_TX
$Comp
L Device:C C10
U 1 1 60BE8135
P 5750 6600
F 0 "C10" H 5900 6600 50  0000 L CNN
F 1 ".1u" H 5900 6500 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5788 6450 50  0001 C CNN
F 3 "~" H 5750 6600 50  0001 C CNN
	1    5750 6600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 60C0358E
P 6150 6450
F 0 "R6" V 6050 6450 50  0000 C CNN
F 1 "100" V 6250 6450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 6080 6450 50  0001 C CNN
F 3 "~" H 6150 6450 50  0001 C CNN
	1    6150 6450
	0    1    1    0   
$EndComp
$Comp
L programmable-remote:M95640_RMN6TP U2
U 1 1 610CB268
P 1950 5100
F 0 "U2" H 2100 5550 50  0000 C CNN
F 1 "M95640_RMN6TP" H 2500 5000 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 1950 5100 50  0001 C CNN
F 3 "" H 1950 5100 50  0001 C CNN
	1    1950 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery BT1
U 1 1 610D4A1F
P 8050 2850
F 0 "BT1" H 8158 2896 50  0000 L CNN
F 1 "Battery" H 8158 2805 50  0000 L CNN
F 2 "programmable-remote:Battery" V 8050 2910 50  0001 C CNN
F 3 "~" V 8050 2910 50  0001 C CNN
	1    8050 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 610D8C9C
P 1250 4600
F 0 "R5" H 1320 4646 50  0000 L CNN
F 1 "4.7k" H 1320 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 1180 4600 50  0001 C CNN
F 3 "~" H 1250 4600 50  0001 C CNN
	1    1250 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 610E1AA4
P 8750 2750
F 0 "J1" H 8700 2700 50  0000 C CNN
F 1 "Conn_01x04_Male" H 8600 2950 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8750 2750 50  0001 C CNN
F 3 "~" H 8750 2750 50  0001 C CNN
	1    8750 2750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 610E3531
P 9700 4050
F 0 "SW2" H 9700 4335 50  0000 C CNN
F 1 "SW_Push" H 9700 4200 50  0000 C CNN
F 2 "programmable-remote:SW_PTS526" H 9700 4250 50  0001 C CNN
F 3 "~" H 9700 4250 50  0001 C CNN
	1    9700 4050
	0    -1   -1   0   
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 610EE2E7
P 2200 6750
F 0 "Y1" H 2200 7018 50  0000 C CNN
F 1 "8 MHz" H 2200 6927 50  0000 C CNN
F 2 "programmable-remote:Crystal_ABLS-8.000MHZ-B4-T" H 2200 6750 50  0001 C CNN
F 3 "~" H 2200 6750 50  0001 C CNN
	1    2200 6750
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C9
U 1 1 610EEAF2
P 1650 6450
F 0 "C9" H 1765 6496 50  0000 L CNN
F 1 "25p" H 1765 6405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 1688 6300 50  0001 C CNN
F 3 "~" H 1650 6450 50  0001 C CNN
	1    1650 6450
	0    1    1    0   
$EndComp
$Comp
L Device:C C11
U 1 1 610EF415
P 1650 7050
F 0 "C11" H 1765 7096 50  0000 L CNN
F 1 "25p" H 1765 7005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 1688 6900 50  0001 C CNN
F 3 "~" H 1650 7050 50  0001 C CNN
	1    1650 7050
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D3
U 1 1 610F2DED
P 8050 2350
F 0 "D3" H 8050 2200 50  0000 C CNN
F 1 "D_Schottky" H 8100 2100 50  0000 C CNN
F 2 "Diode_SMD:D_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8050 2350 50  0001 C CNN
F 3 "~" H 8050 2350 50  0001 C CNN
	1    8050 2350
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D4
U 1 1 610F35EA
P 8950 2350
F 0 "D4" H 8950 2500 50  0000 C CNN
F 1 "D_Schottky" H 9050 2200 50  0000 C CNN
F 2 "Diode_SMD:D_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8950 2350 50  0001 C CNN
F 3 "~" H 8950 2350 50  0001 C CNN
	1    8950 2350
	0    -1   1    0   
$EndComp
Text Label 1100 5050 2    50   ~ 0
CS
Text Label 1100 4950 2    50   ~ 0
SCK
Text Label 1100 4850 2    50   ~ 0
MOSI
Text Label 2450 4850 0    50   ~ 0
MISO
Wire Wire Line
	8950 2650 8950 2500
Wire Wire Line
	8050 2650 8050 2500
Wire Wire Line
	8950 2950 8950 3050
Text Label 9600 2750 0    50   ~ 0
SWCLK
Text Label 9600 2850 0    50   ~ 0
SWDIO
$Comp
L Switch:SW_Push SW7
U 1 1 60BCB460
P 8350 6000
F 0 "SW7" H 8350 6285 50  0000 C CNN
F 1 "SW_Push" H 8350 6194 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8350 6200 50  0001 C CNN
F 3 "~" H 8350 6200 50  0001 C CNN
	1    8350 6000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW6
U 1 1 60BCB45A
P 8350 5600
F 0 "SW6" H 8350 5885 50  0000 C CNN
F 1 "SW_Push" H 8350 5794 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8350 5800 50  0001 C CNN
F 3 "~" H 8350 5800 50  0001 C CNN
	1    8350 5600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW5
U 1 1 60BCB454
P 8350 5200
F 0 "SW5" H 8350 5485 50  0000 C CNN
F 1 "SW_Push" H 8350 5394 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8350 5400 50  0001 C CNN
F 3 "~" H 8350 5400 50  0001 C CNN
	1    8350 5200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 60BC5F33
P 8350 4800
F 0 "SW4" H 8350 5085 50  0000 C CNN
F 1 "SW_Push" H 8350 4994 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8350 5000 50  0001 C CNN
F 3 "~" H 8350 5000 50  0001 C CNN
	1    8350 4800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 60BC5BC2
P 8350 4400
F 0 "SW3" H 8350 4685 50  0000 C CNN
F 1 "SW_Push" H 8350 4594 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8350 4600 50  0001 C CNN
F 3 "~" H 8350 4600 50  0001 C CNN
	1    8350 4400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 60BC1C19
P 8350 4000
F 0 "SW1" H 8350 4285 50  0000 C CNN
F 1 "SW_Push" H 8350 4194 50  0000 C CNN
F 2 "Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB" H 8350 4200 50  0001 C CNN
F 3 "~" H 8350 4200 50  0001 C CNN
	1    8350 4000
	1    0    0    -1  
$EndComp
Text Label 7950 4400 2    50   ~ 0
KOK
Text Label 7950 4000 2    50   ~ 0
KESC
Text Label 7950 4800 2    50   ~ 0
K0
Text Label 7950 5200 2    50   ~ 0
K1
Text Label 7950 5600 2    50   ~ 0
K2
Text Label 7950 6000 2    50   ~ 0
K3
Wire Wire Line
	7950 4000 8150 4000
Wire Wire Line
	7950 4400 8150 4400
Wire Wire Line
	7950 4800 8150 4800
Wire Wire Line
	7950 5200 8150 5200
Wire Wire Line
	7950 5600 8150 5600
Wire Wire Line
	7950 6000 8150 6000
Text Label 8850 4000 0    50   ~ 0
V3_3
$Comp
L Device:C C8
U 1 1 61242CB5
P 10000 4000
F 0 "C8" H 10115 4046 50  0000 L CNN
F 1 ".1u" H 10115 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10038 3850 50  0001 C CNN
F 3 "~" H 10000 4000 50  0001 C CNN
	1    10000 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 3850 9700 3700
Wire Wire Line
	9700 3700 10000 3700
Wire Wire Line
	10000 3700 10000 3850
Wire Wire Line
	10000 4150 10000 4350
Wire Wire Line
	10000 4350 9700 4350
Wire Wire Line
	9700 4350 9700 4250
Text Label 10150 3700 0    50   ~ 0
NRST
Wire Wire Line
	10150 3700 10000 3700
Connection ~ 10000 3700
Wire Wire Line
	10150 4350 10000 4350
Connection ~ 10000 4350
Text Label 6400 6800 0    50   ~ 0
IR_IN
Wire Wire Line
	2850 5700 2500 5700
Wire Wire Line
	5750 6750 5750 6900
Connection ~ 5750 6900
Wire Wire Line
	5750 6900 6400 6900
Wire Wire Line
	5300 6600 5450 6600
Wire Wire Line
	5450 6600 5450 6450
Wire Wire Line
	5450 6450 5750 6450
Connection ~ 5750 6450
Wire Wire Line
	5750 6450 6000 6450
Wire Wire Line
	6300 6450 6400 6450
Wire Wire Line
	6400 6800 5450 6800
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
L Device:C C4
U 1 1 613C61C6
P 9950 1350
F 0 "C4" H 10065 1396 50  0000 L CNN
F 1 "4.7u" H 10065 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 9988 1200 50  0001 C CNN
F 3 "~" H 9950 1350 50  0001 C CNN
	1    9950 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 613C698D
P 8300 1350
F 0 "C1" H 8415 1396 50  0000 L CNN
F 1 ".1u" H 8415 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 8338 1200 50  0001 C CNN
F 3 "~" H 8300 1350 50  0001 C CNN
	1    8300 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 613C7A7D
P 9400 1350
F 0 "C3" H 9515 1396 50  0000 L CNN
F 1 ".1u" H 9515 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 9438 1200 50  0001 C CNN
F 3 "~" H 9400 1350 50  0001 C CNN
	1    9400 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 613C7F00
P 8850 1350
F 0 "C2" H 8965 1396 50  0000 L CNN
F 1 ".1u" H 8965 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 8888 1200 50  0001 C CNN
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
L Device:C C7
U 1 1 613C4E64
P 10400 2200
F 0 "C7" H 10515 2246 50  0000 L CNN
F 1 "1u" H 10515 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10438 2050 50  0001 C CNN
F 3 "~" H 10400 2200 50  0001 C CNN
	1    10400 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 613C3361
P 10050 2200
F 0 "C6" H 10165 2246 50  0000 L CNN
F 1 "10n" H 10165 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10088 2050 50  0001 C CNN
F 3 "~" H 10050 2200 50  0001 C CNN
	1    10050 2200
	1    0    0    -1  
$EndComp
$Comp
L MCU_ST_STM32F1:STM32F103RBTx U1
U 1 1 610E5B85
P 5700 3300
F 0 "U1" H 5700 3250 50  0000 C CNN
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
Text Notes 7900 650  0    50   ~ 10
Power and decoupling
Text Notes 850  850  0    50   ~ 10
Diodes
Text Notes 8100 3500 0    50   ~ 10
Switches
Text Notes 1150 6200 0    50   ~ 10
Crystal
Text Notes 4250 6200 0    50   ~ 10
Ir sensing
Text Notes 750  7700 0    50   ~ 0
C_stray, the capacitance of the pcb trace and mcu is assumed to be 5 pF.\nC_l is the expected load capacitance of the\ncrystal. C is the capacitance from the capacitor at each leg.\nUse the following formula to determine C:\nC = 2*C_l - 2*C_stray = 2*18 - 10 [pF] =26 pF.
Wire Wire Line
	9950 1550 9400 1550
Connection ~ 9400 1550
Wire Wire Line
	9400 1550 9400 1500
$Comp
L Device:Fuse 500m1
U 1 1 615B7066
P 9350 2200
F 0 "500m1" V 9153 2200 50  0000 C CNN
F 1 "Fuse" V 9244 2200 50  0000 C CNN
F 2 "Fuse:Fuse_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 9280 2200 50  0001 C CNN
F 3 "~" H 9350 2200 50  0001 C CNN
	1    9350 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	8950 2750 9600 2750
Wire Wire Line
	8950 2850 9600 2850
$Comp
L Device:C C5
U 1 1 613C85A7
P 10400 1350
F 0 "C5" H 10515 1396 50  0000 L CNN
F 1 ".1u" H 10515 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10438 1200 50  0001 C CNN
F 3 "~" H 10400 1350 50  0001 C CNN
	1    10400 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 2100 1000 1900
Connection ~ 1000 1900
Wire Wire Line
	3300 2250 3300 2350
$Comp
L Device:R R4
U 1 1 6114BC55
P 2500 3350
F 0 "R4" V 2293 3350 50  0000 C CNN
F 1 "68k" V 2384 3350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 2430 3350 50  0001 C CNN
F 3 "~" H 2500 3350 50  0001 C CNN
	1    2500 3350
	0    1    1    0   
$EndComp
$Comp
L Device:LED D5
U 1 1 60BCDAEA
P 3300 2500
F 0 "D5" H 3293 2717 50  0000 C CNN
F 1 "LED" H 3293 2626 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm_IRGrey" H 3300 2500 50  0001 C CNN
F 3 "~" H 3300 2500 50  0001 C CNN
	1    3300 2500
	0    -1   -1   0   
$EndComp
Text Label 2050 3350 2    50   ~ 0
IR_OUT
Text Notes 1000 4300 0    50   ~ 10
Serial devices
$Comp
L Device:R R3
U 1 1 6119D27D
P 3300 2900
F 0 "R3" V 3093 2900 50  0000 C CNN
F 1 "10" V 3184 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3230 2900 50  0001 C CNN
F 3 "~" H 3300 2900 50  0001 C CNN
	1    3300 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	3300 2650 3300 2750
Wire Wire Line
	3300 3050 3300 3150
Wire Wire Line
	3300 3550 3300 3700
Wire Wire Line
	2050 3350 2350 3350
Wire Wire Line
	2650 3350 3000 3350
Text Notes 800  2950 0    50   ~ 0
Assume saturation for the following:\nI_ledmax = 100 mA\nR_collector = (V3_3 - V_led - V_transistor)/l_ledmax\nR_collector = (3.3 - 1.42 - 1)/0.1 ohm = 8.8 ohm\nI_base = I_ledmax/Hfe*ODF = .1/50000*10 A = 20 uA\nR_base = (V3_3 - V_be) / I_base\n= (3.3 - 2) / 0.00002 ohm = 65 kohm\n
$Comp
L Device:Q_NPN_Darlington_BCE Q1
U 1 1 6121DE4B
P 3200 3350
F 0 "Q1" H 3390 3396 50  0000 L CNN
F 1 "Q_NPN_Darlington_BCE" H 3390 3305 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TSOT-23_HandSoldering" H 3400 3450 50  0001 C CNN
F 3 "~" H 3200 3350 50  0001 C CNN
	1    3200 3350
	1    0    0    -1  
$EndComp
NoConn ~ 5000 4800
NoConn ~ 5000 3400
NoConn ~ 5000 3500
NoConn ~ 5000 3600
NoConn ~ 5000 3700
$Comp
L Switch:SW_DIP_x02 SW8
U 1 1 612CF507
P 9850 5400
F 0 "SW8" H 9850 5767 50  0000 C CNN
F 1 "SW_DIP_x02" H 9850 5676 50  0000 C CNN
F 2 "programmable-remote:SW_2LP" H 9850 5400 50  0001 C CNN
F 3 "~" H 9850 5400 50  0001 C CNN
	1    9850 5400
	1    0    0    -1  
$EndComp
Text Label 10750 5300 0    50   ~ 0
BOOT0
Text Label 10750 5400 0    50   ~ 0
BOOT1
$Comp
L Device:R R8
U 1 1 612F1F40
P 10550 5550
F 0 "R8" H 10620 5596 50  0000 L CNN
F 1 "4.7k" H 10620 5505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 10480 5550 50  0001 C CNN
F 3 "~" H 10550 5550 50  0001 C CNN
	1    10550 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 612F236A
P 10250 5550
F 0 "R7" H 10320 5596 50  0000 L CNN
F 1 "4.7k" H 10320 5505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 10180 5550 50  0001 C CNN
F 3 "~" H 10250 5550 50  0001 C CNN
	1    10250 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 5400 10250 5400
Wire Wire Line
	10150 5300 10550 5300
Wire Wire Line
	10550 5300 10550 5400
Connection ~ 10550 5300
Wire Wire Line
	10550 5300 10750 5300
Connection ~ 10250 5400
Wire Wire Line
	10250 5700 10250 5800
Wire Wire Line
	10250 5800 10550 5800
Wire Wire Line
	10550 5700 10550 5800
Connection ~ 10550 5800
Wire Wire Line
	10550 5800 10750 5800
Wire Wire Line
	9550 5300 9550 4800
Wire Wire Line
	9550 4800 10750 4800
Wire Wire Line
	9550 5400 9550 5300
Connection ~ 9550 5300
Wire Wire Line
	10250 5400 10750 5400
Text Label 4700 1900 2    50   ~ 0
BOOT0
Text Label 6700 3600 0    50   ~ 0
BOOT1
Wire Wire Line
	6700 3600 6400 3600
Wire Wire Line
	5000 1900 4700 1900
NoConn ~ 6400 2500
NoConn ~ 6400 2600
NoConn ~ 6400 2700
NoConn ~ 6400 2800
NoConn ~ 6400 2900
NoConn ~ 6400 3200
NoConn ~ 6400 3400
NoConn ~ 6400 3500
NoConn ~ 6400 3700
NoConn ~ 6400 3800
NoConn ~ 6400 3900
NoConn ~ 6400 4100
NoConn ~ 6400 4200
NoConn ~ 6400 4300
NoConn ~ 6400 4400
NoConn ~ 6400 4500
NoConn ~ 5000 3800
NoConn ~ 5000 3900
NoConn ~ 5000 4000
NoConn ~ 5000 4100
NoConn ~ 5000 4200
NoConn ~ 5000 4300
NoConn ~ 5000 4400
NoConn ~ 5000 4500
NoConn ~ 5000 4600
NoConn ~ 5000 3200
$Comp
L power:VDD #PWR0101
U 1 1 61487E14
P 9600 2200
F 0 "#PWR0101" H 9600 2050 50  0001 C CNN
F 1 "VDD" H 9615 2373 50  0000 C CNN
F 2 "" H 9600 2200 50  0001 C CNN
F 3 "" H 9600 2200 50  0001 C CNN
	1    9600 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2200 8950 2200
Wire Wire Line
	8050 3050 8950 3050
Wire Wire Line
	9500 2200 9600 2200
Wire Wire Line
	1100 4450 1250 4450
Wire Wire Line
	1250 4450 1500 4450
Wire Wire Line
	1950 4450 1950 4650
Connection ~ 1250 4450
Wire Wire Line
	1100 4850 1650 4850
Wire Wire Line
	1100 4950 1650 4950
Wire Wire Line
	1100 5050 1250 5050
Wire Wire Line
	1250 4750 1250 5050
Connection ~ 1250 5050
Wire Wire Line
	1250 5050 1650 5050
Wire Wire Line
	1100 5450 2050 5450
Wire Wire Line
	2050 5450 2050 5400
Wire Wire Line
	2250 4850 2450 4850
Wire Wire Line
	1650 5150 1600 5150
Wire Wire Line
	1600 5150 1600 4450
Connection ~ 1600 4450
Wire Wire Line
	1600 4450 1950 4450
Wire Wire Line
	1650 5250 1500 5250
Wire Wire Line
	1500 5250 1500 4450
Connection ~ 1500 4450
Wire Wire Line
	1500 4450 1600 4450
Text Notes 2250 4600 0    50   ~ 0
Pull-up resistor for CS.\nR > (Vdd - Vol)/I = 2.9/0.008\n= 362.5 ohm\nI is the max current the mcu can sink
Text Label 6700 3000 0    50   ~ 0
SWDIO
Text Label 6700 3100 0    50   ~ 0
SWCLK
Wire Wire Line
	6700 3100 6400 3100
Wire Wire Line
	6400 3000 6700 3000
Text Notes 9750 3050 0    50   ~ 0
Using internal pull-up/down\nresistors for SWCLK and SWDIO\n
Wire Wire Line
	8950 3050 9600 3050
$Comp
L power:GND #PWR0102
U 1 1 615C8EF3
P 9600 3050
F 0 "#PWR0102" H 9600 2800 50  0001 C CNN
F 1 "GND" H 9605 2877 50  0000 C CNN
F 2 "" H 9600 3050 50  0001 C CNN
F 3 "" H 9600 3050 50  0001 C CNN
	1    9600 3050
	1    0    0    -1  
$EndComp
Text GLabel 8300 900  1    50   Input ~ 0
VDD
Text GLabel 8300 1800 3    50   Input ~ 0
GND
Text GLabel 10400 1950 1    50   Input ~ 0
VDD
Text GLabel 10400 2500 3    50   Input ~ 0
GND
Text GLabel 10150 4350 2    50   Input ~ 0
GND
Text GLabel 10750 4800 2    50   Input ~ 0
VDD
Text GLabel 6400 6900 2    50   Input ~ 0
GND
Text GLabel 10750 5800 2    50   Input ~ 0
GND
Text GLabel 6400 6450 2    50   Input ~ 0
VDD
Text GLabel 1100 4450 0    50   Input ~ 0
VDD
Text GLabel 1100 5450 0    50   Input ~ 0
GND
Text GLabel 3300 2250 1    50   Input ~ 0
VDD
Text GLabel 3300 3700 3    50   Input ~ 0
GND
Text GLabel 1000 2100 3    50   Input ~ 0
GND
Text GLabel 5500 1250 0    50   Input ~ 0
VDD
Wire Wire Line
	5450 6800 5450 6750
Wire Wire Line
	5450 6750 5300 6750
$Comp
L programmable-remote:TSOP953xx U3
U 1 1 616AA23F
P 5050 6850
F 0 "U3" H 5103 7325 50  0000 C CNN
F 1 "TSOP953xx" H 5103 7234 50  0000 C CNN
F 2 "programmable-remote:TSOP953xx" H 5110 6850 50  0001 C CNN
F 3 "" H 5110 6850 50  0001 C CNN
	1    5050 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 6900 5750 6900
Text GLabel 1250 7050 0    50   Input ~ 0
GND
Wire Wire Line
	8300 900  8300 1200
Text GLabel 8850 1050 1    50   Input ~ 0
VDD
Text GLabel 9400 1050 1    50   Input ~ 0
VDD
Text GLabel 10400 1050 1    50   Input ~ 0
VDD
$EndSCHEMATC
