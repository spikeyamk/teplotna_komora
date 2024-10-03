# Status legend
- ❌ not configured
- ✔️ working
- ⬜ configured, untested
- 🟡 intentional wrong configuration
- ⚠️ hardware fault

# clk
## hse
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PH0 | XI | Vstup | pre 16 MHz signál z kryštálu | ⬜️ |
| PH1 | XO | Budiaci | výstup pre 16 MHz kryštál | ⬜️ |

## lse
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PC14 | XI | Vstup | pre 32.768 kHz signál z kryštálu | ⬜️ |
| PC15 | XO | Budiaci | výstup pre 32.768 kHz kryštál | ⬜ |

# comm
## usb_uart
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA9 | TX1 | DO | Výstup sériových dát do prevodníka USB/UART | ✔️ |
| PA10 | RX1 | DI | Vstup sériových dát z prevodníka USB/UART | ✔️ |
| PA11 | CTS1 | DO | Výstup pre kontrolu toku dát z prevodníka USB/UART | 🟡️ |
| PA12 | RTS1 | DI | Vstup pre kontrolu toku dát do prevodníka USB/UART | 🟡 |
## ~~rs232_uart~~
| ~~Pin~~   | ~~Signál~~  | ~~Typ~~  | ~~Popis~~                                         | ~~Status~~ |
|-------|---------|------|-----------------------------------------------| --- |
| ~~PD8~~ | ~~TX3~~ | ~~DO~~ | ~~Výstup sériových dát do prevodníka RS232/UART~~ | ~~❌~~ |
| ~~PD9~~ | ~~RX3~~ | ~~DI~~ | ~~Vstup sériových dát z prevodníka RS232/UART~~ | ~~❌~~ |
## swd
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA13 | SWDIO | DIO | Vstup/výstup pre programovanie cez SWD | ⬜️ |
| PA14 | SWCLK | DI | Hodinový vstup pre programovanie cez SWD | ⬜️ |

# actu
## fan
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA0 | FC4 | DO | Riadenie ventilátora 4 | ✔️ |
| PA1 | FF4 | DI | Spätná väzba ventilátora 4 | ❌ |
| PA2 | FC5 | DO | Riadenie ventilátora 5 | ✔️|
| PA3 | FF5 | DI | Spätná väzba ventilátora 5 | ❌ |
| PB8 | FC7 | DO | Riadenie ventilátora 7 | ✔️ |
| PB9 | FF7 | DI | Spätná väzba ventilátora 7 | ❌ |
| PC6 | FC0 | DO | Riadenie ventilátora 0 | ✔️ |
| PC7 | FF0 | DI | Spätná väzba ventilátora 0 | ❌ |
| PC8 | FC1 | DO | Riadenie ventilátora 0 | ✔️ |
| PC9 | FF1 | DI | Spätná väzba ventilátora 0 | ❌ |
| PD12 | FC2 | DO | Riadenie ventilátora 2 | ✔️ |
| PD13 | FF2 | DI | Spätná väzba ventilátora 2 | ❌ |
| PD14 | FC3 | DO | Riadenie ventilátora 3 | ✔️ |
| PD15 | FF3 | DI | Spätná väzba ventilátora 3 | ❌ |
| PE5 | FC6 | DO | Riadenie ventilátora 6 | ✔️ |
| PE6 | FF6 | DI | Spätná väzba ventilátora 6 | ❌ |
## pump
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PD10 | APD | DO | Výstup pre zapnute/vypnute vzduchovej pumpy | ✔️ |
## lin_source
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA4 | AOUTB | AO | Výstup B z DAC | ⬜ |
| PA5 | AOUTA | AO | Výstup A z DAC | ⬜ |
## bridge
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA8 | HC7 | DO | Invertovaný výstup pre ovládanie výkonového mosơka B (J7-1) | ⬜ |
| PB0 | HC2 | DO | Invertovaný výstup pre ovládanie výkonového mosơka A (J6-3) | ⬜ |
| PB1 | HC3 | DO | Invertovaný výstup pre ovládanie výkonového mosơka A (J6-1) | ⬜ |
| PC4 | HC0 | DO | Invertovaný výstup pre ovládanie výkonového mosơka A (J6-2) | ⬜ |
| PC5 | HC1 | DO | Invertovaný výstup pre ovládanie výkonového mosơka A (J6-4) | ⬜ |
| PC10 | HC6 | DO | Invertovaný výstup pre ovládanie výkonového mosơka B (J7-3) | ⬜ |
| PC11 | HC5 | DO | Invertovaný výstup pre ovládanie výkonového mosơka B (J7-4) | ⬜ |
| PC12 | HC4 | DO | Invertovaný výstup pre ovládanie výkonového mosơka B (J7-2) | ⬜ |
## buzzer
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PD11 | BUZ | DO | Výstup pre zapnute/vypnute piezoelektrického akustckého indikátora | ✔️ |

# panel
## out
### led
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PC0 | LED1 | DO | Panelová LED, môže byť použitá pre indikáciu znižovania teploty | ✔️ |
| PC1 | LED0 | DO | Panelová LED, môže byť použitá pre indikáciu zvyšovania teploty | ✔️ |
| PC2 | LED2 | DO | Panelová LED, môže byť použitá pre indikáciu toku dát cez RS232/UART | ✔️ |
| PC3 | LED3 | DO | Panelová LED, môže byť použitá pre indikáciu toku dát cez USB/UART | ✔️ |
### sevseg
#### white
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA15 | WHBK | DO | Výstup pre nastavovanie jasu bieleho displeja | ✔️ |
| PE0 | WH3 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 0 | ✔️ |
| PE1 | WH4 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 1 | ✔️ |
| PE2 | WH0 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 2 | ✔️ |
| PE3 | WH2 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 3 | ✔️ |
| PE4 | WH1 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 4 | ✔️ |
| PE7 | WHA | DO | Výstup pre riadenie segmentov A | ✔️ |
| PE8 | WHB | DO | Výstup pre riadenie segmentov B | ✔️ |
| PE9 | WHC | DO | Výstup pre riadenie segmentov C | ✔️ |
| PE10 | WHD | DO | Výstup pre riadenie segmentov D | ✔️ |
| PE11 | WHE | DO | Výstup pre riadenie segmentov E | ✔️ |
| PE12 | WHF | DO | Výstup pre riadenie segmentov F | ✔️ |
| PE13 | WHG | DO | Výstup pre riadenie segmentov G | ✔️ |
| PE14 | WHDP | DO | Výstup pre riadenie segmentov DP | ✔️ |
#### green_yellow
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PB12 | DDCS | DO | Selektovací výstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ❌ |
| PB13 | SCLK2 | DO | Hodinový výstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ❌ |
| PB14 | MISO2 | DI | Dátový vstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ❌ |
| PB15 | MOSI2 | DO | Dátový výstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ❌ |
## in
### button
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PD0 | SW0 | DI | Vstup z pravého krajného panelového spínača | ❌ |
| PD1 | SW1 | DI | Vstup z pravého stredného panelového spínača | ❌ |
| PD2 | SW2 | DI | Vstup z ľavého stredného panelového spínača | ❌ |
| PD3 | SW3 | DI | Vstup z ľavého krajného panelového spínača | ❌ |
### encoder
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PB10 | ENCA | DI | Vstup A z kvadratúrneho enkodéra | ❌ |
| PB11 | ENCB | DI | Vstup B z kvadratúrneho enkodéra | ❌ |
| PE15 | SWENC | DI | Vstup zo spínača integrovaného v kvadratúrnom enkodéri | ❌ |

# sens
## i2c
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PB6 | SDA1 | DIO | Obojsmerné sériové dáta pre senzor osvetlenia, senzory teploty a vlhkost a pre EEPROM | ⚠️ |
| PB7 | SCL1 | DO | Hodinový výstup pre senzor osvetlenia, senzory teploty a vlhkost a pre EEPROM | ⚠️ |
### light
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| none |
### temp_hum
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PA6 | ALRT0 | DI | Prerušenie zo senzora teploty a vlhkost na doske (v miestnost) | ❌️ |
| PA7 | ALRT1 | DI | Prerušenie z externého senzora teploty a vlhkost (v komore) | ❌️ |
### ~~eeprom~~
| ~~Pin~~   | ~~Signál~~  | ~~Typ~~  | ~~Popis~~                                         | ~~Status~~ |
|-------|---------|------|-----------------------------------------------| --- |
| ~~PC13~~ | ~~WCN~~ | ~~DO~~ | ~~Výstup pre riadenie zápisu do EEPROM~~ | ~~❌~~ |
## spi
### temp
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PB3 | SCLK1 | DO | Hodinový výstup pre SPI pripojené k obom R/T prevodníkom | ❌ |
| PB4 | MISO1 | DI | Dátový vstup pre SPI pripojené k obom R/T prevodníkom | ❌ |
| PB5 | MOSI1 | DO | Dátový výstup pre SPI pripojené k obom R/T prevodníkom | ❌ |
| PD7 | MCS1 | DO | Selektovací výstup pre SPI pripojené k druhému R/T prevodníku | ❌ |
| PD4 | DRDY0 | DI | Vstup z prvého R/T prevodníka indikujúci pripravenosť dát na čítanie | ❌ |
| PD5 | MCS0 | DO | Selektovací výstup pre SPI pripojené k prvému R/T prevodníku | ❌ |
| PD6 | DRDY1 | DI | Vstup z druhého R/T prevodníka indikujúci pripravenosť dát na čítanie | ❌ |