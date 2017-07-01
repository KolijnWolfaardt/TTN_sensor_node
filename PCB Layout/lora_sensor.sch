EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:pic18f2xj50
LIBS:mcp73831
LIBS:switchtactile
LIBS:tps736
LIBS:BH1603FVC
LIBS:mcp9700t-e
LIBS:rn2483
LIBS:fa_238
LIBS:lora_sensor-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4350 5200 2200 850 
U 58974944
F0 "rf_section" 60
F1 "rf_section.sch" 60
F2 "RF_Power" I L 4350 5300 60 
F3 "GND" I L 4350 5400 60 
F4 "RF_UART_RX" I R 6550 5300 60 
F5 "RF_UART_TX" I R 6550 5400 60 
F6 "RF_Reset" I R 6550 5500 60 
$EndSheet
$Sheet
S 1000 1450 2200 850 
U 5899E1BE
F0 "power_section" 60
F1 "power_section.sch" 60
F2 "V_Battery" I R 3200 1550 60 
F3 "RF_Power" I R 3200 1900 60 
F4 "MCU_Power" I R 3200 2000 60 
F5 "3.3V" I R 3200 1750 60 
F6 "V_USB" I R 3200 1650 60 
F7 "GND" I R 3200 2150 60 
$EndSheet
$Sheet
S 4350 1450 2200 1250
U 5899E253
F0 "microcontroller" 60
F1 "microcontroller.sch" 60
F2 "MCU_Power" I L 4350 2000 60 
F3 "GND" I L 4350 2150 60 
F4 "UART_TX" I R 6550 2450 60 
F5 "UART_RX" I R 6550 2350 60 
F6 "AN0_Battery" I R 6550 1550 60 
F7 "AN1_LDR" I R 6550 1650 60 
F8 "AN2_Ambient" I R 6550 1750 60 
F9 "AN3_Temperature" I R 6550 1850 60 
F10 "V_USB" I L 4350 1650 60 
F11 "VB_Control" I R 6550 2150 60 
F12 "Sensors_Enable" I R 6550 2050 60 
F13 "RF_Reset" I R 6550 2550 60 
$EndSheet
$Sheet
S 4350 3150 2200 1700
U 5899F695
F0 "sensor_section" 60
F1 "sensor_section.sch" 60
F2 "V_Battery" I L 4350 3250 60 
F3 "MCU_Power" I L 4350 3350 60 
F4 "GND" I L 4350 3450 60 
F5 "VB_Control" I R 6550 3850 60 
F6 "Sensors_Enable" I R 6550 3750 60 
F7 "AN0_Battery" I R 6550 3250 60 
F8 "AN1_LDR" I R 6550 3350 60 
F9 "AN2_Ambient" I R 6550 3450 60 
F10 "AN3_Temperature" I R 6550 3550 60 
$EndSheet
Wire Wire Line
	3200 1550 3700 1550
Wire Wire Line
	3200 1650 4350 1650
Wire Wire Line
	3200 1750 3400 1750
Wire Wire Line
	3200 1900 3500 1900
Wire Wire Line
	3200 2000 4350 2000
Wire Wire Line
	3200 2150 4350 2150
Wire Wire Line
	3400 5400 4350 5400
Wire Wire Line
	3400 2150 3400 5400
Connection ~ 3400 2150
Wire Wire Line
	3500 1900 3500 5300
Wire Wire Line
	3500 5300 4350 5300
Wire Wire Line
	3700 1550 3700 3250
Wire Wire Line
	3700 3250 4350 3250
Wire Wire Line
	4350 3450 3400 3450
Connection ~ 3400 3450
Wire Wire Line
	4350 3350 3600 3350
Wire Wire Line
	3600 3350 3600 2000
Connection ~ 3600 2000
Wire Wire Line
	6550 3250 6900 3250
Wire Wire Line
	6900 3250 6900 1550
Wire Wire Line
	6900 1550 6550 1550
Wire Wire Line
	6550 1650 6950 1650
Wire Wire Line
	6950 1650 6950 3350
Wire Wire Line
	6950 3350 6550 3350
Wire Wire Line
	6550 3450 7000 3450
Wire Wire Line
	7000 3450 7000 1750
Wire Wire Line
	7000 1750 6550 1750
Wire Wire Line
	6550 1850 7050 1850
Wire Wire Line
	7050 1850 7050 3550
Wire Wire Line
	7050 3550 6550 3550
Wire Wire Line
	6550 2050 7100 2050
Wire Wire Line
	7100 2050 7100 3750
Wire Wire Line
	7100 3750 6550 3750
Wire Wire Line
	6550 3850 7150 3850
Wire Wire Line
	7150 3850 7150 2150
Wire Wire Line
	7150 2150 6550 2150
Wire Wire Line
	6550 2350 7350 2350
Wire Wire Line
	7350 2350 7350 5400
Wire Wire Line
	7350 5400 6550 5400
Wire Wire Line
	6550 5300 7400 5300
Wire Wire Line
	7400 5300 7400 2450
Wire Wire Line
	7400 2450 6550 2450
Wire Wire Line
	6550 2550 7450 2550
Wire Wire Line
	7450 2550 7450 5500
Wire Wire Line
	7450 5500 6550 5500
$Comp
L CONN_01X01 J1
U 1 1 589F7D41
P 1650 6250
F 0 "J1" H 1728 6291 50  0000 L CNN
F 1 "CONN_01X01" H 1728 6200 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 100 1200 50  0001 C CNN
F 3 "" H 100 1200 50  0001 C CNN
	1    1650 6250
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 J2
U 1 1 589F8006
P 1650 6450
F 0 "J2" H 1728 6491 50  0000 L CNN
F 1 "CONN_01X01" H 1728 6400 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 100 1400 50  0001 C CNN
F 3 "" H 100 1400 50  0001 C CNN
	1    1650 6450
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 J3
U 1 1 589F80EC
P 1650 6650
F 0 "J3" H 1728 6691 50  0000 L CNN
F 1 "CONN_01X01" H 1728 6600 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 100 1600 50  0001 C CNN
F 3 "" H 100 1600 50  0001 C CNN
	1    1650 6650
	1    0    0    -1  
$EndComp
NoConn ~ 1450 6250
NoConn ~ 1450 6450
NoConn ~ 1450 6650
$EndSCHEMATC
