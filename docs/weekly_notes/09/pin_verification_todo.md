# Status legend
- ❌ not configured
- ✔️ working
- ⬜ configured, untested
- 🟡 intentional wrong configuration
- ⚠️ hardware fault
- 🔎 missing
- ![PIN_NAME] wrong name in schematics

# clk
## hse
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PH0 | XI | Vstup | pre 16 MHz signál z kryštálu | ✔️ |
| PH1 | XO | Budiaci | výstup pre 16 MHz kryštál | ✔️ |

## lse
| Pin   | Signál  | Typ  | Popis                                         | Status |
|-------|---------|------|-----------------------------------------------| --- |
| PC14 | XI | Vstup | pre 32.768 kHz signál z kryštálu | ✔️ |
| PC15 | XO | Budiaci | výstup pre 32.768 kHz kryštál | ✔️ |

# comm
## usb_uart
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PA9 | TX1 | DO | Výstup sériových dát do prevodníka USB/UART | ✔️ | USB_UART_TX |
| PA10 | RX1 | DI | Vstup sériových dát z prevodníka USB/UART | ✔️ | USB_UART_RX |
| PA11 | CTS1 | DO | Výstup pre kontrolu toku dát z prevodníka USB/UART | 🟡️ | N/A |
| PA12 | RTS1 | DI | Vstup pre kontrolu toku dát do prevodníka USB/UART | 🟡 | N/A |
## rs232_uart
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PD8 | TX3 | DO | Výstup sériových dát do prevodníka RS232/UART | ✔️ | RS232_UART_TX |
| PD9 | RX3 | DI | Vstup sériových dát z prevodníka RS232/UART | ✔️ | RS232_UART_RX |
## swd
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PA13 | SWDIO | DIO | Vstup/výstup pre programovanie cez SWD | ⬜️ | N/A |
| PA14 | SWCLK | DI | Hodinový vstup pre programovanie cez SWD | ⬜️ | N/A |

# actu
## fan
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PA0 | FC4 | DO | Riadenie ventilátora 4 | ✔️ | TIM5_CH1_FAN4_BR_CTL |
| PA1 | FF4 | DI | Spätná väzba ventilátora 4 | ✔️ | TIM5_CH2_FAN4_BR_FB |
| PA2 | FC5 | DO | Riadenie ventilátora 5 | ✔️ | TIM5_CH3_FAN5_FR_CTL |
| PA3 | FF5 | DI | Spätná väzba ventilátora 5 | ✔️ | TIM5_CH4_FAN5_FR_FB |
| PB8 | FC7 | DO | Riadenie ventilátora 7 | ✔️ | BROILEN |
| PB9 | FF7 | DI | Spätná väzba ventilátora 7 | 🔎 | $$ |
| PC6 | FC0 | DO | Riadenie ventilátora 0 | ✔️ | TIM3_CH1_FAN0_RL_CTL |
| PC7 | FF0 | DI | Spätná väzba ventilátora 0 | ✔️ | TIM3_CH2_FAN0_RL_FB |
| PC8 | FC1 | DO | Riadenie ventilátora 1 | ✔️ | TIM3_CH3_FAN1_FL_CTL | 
| PC9 | FF1 | DI | Spätná väzba ventilátora 1 | ✔️ | TIM3_CH3_FAN1_FL_FB |
| PD12 | FC2 | DO | Riadenie ventilátora 2 | ✔️ | TIM4_CH1_FAN2_RR_CTL |
| PD13 | FF2 | DI | Spätná väzba ventilátora 2 | ✔️ | TIM4_CH2_FAN2_RR_FB |
| PD14 | FC3 | DO | Riadenie ventilátora 3 | ✔️ | TIM4_CH3_FAN3_BL_CTL |
| PD15 | FF3 | DI | Spätná väzba ventilátora 3 | ✔️ | TIM4_CH4_FAN3_BL_FB |
| PE5 | FC6 | DO | Riadenie ventilátora 6 | 🔎 | $$ | 
| PE6 | FF6 | DI | Spätná väzba ventilátora 6 | 🔎 | $$ |
## pump
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| !PD11 | APD | DO | Výstup pre zapnute/vypnute vzduchovej pumpy | ✔️ | NPUMPEN |
## lin_source
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PA4 | AOUTB | AO | Výstup B z DAC | ✔️ | DAC_OUT1_LSF |
| PA5 | AOUTA | AO | Výstup A z DAC | ✔️ | DAC_OUT2_LSR |
## bridge
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PC4 | HC0 | DO | Invertovaný výstup pre ovládanie výkonového mostíka A (J6-2) | ✔️ | BRDGF_LHIGH |
| PC5 | HC1 | DO | Invertovaný výstup pre ovládanie výkonového mostíka A (J6-4) | ✔️ | BRDGF_RHIGH |
| PB0 | HC2 | DO | Invertovaný výstup pre ovládanie výkonového mostíka A (J6-3) | ✔️ | BRDGF_LLOW |
| PB1 | HC3 | DO | Invertovaný výstup pre ovládanie výkonového mostíka A (J6-1) | ✔️ | BRDGF_RLOW |
|-----|-----|----|--------------------------------------------------------------|-----|------------|
| PC11 | HC5 | DO | Invertovaný výstup pre ovládanie výkonového mostíka B (J7-4) | ✔️ | BRDGR_RHIGH |
| PC12 | HC4 | DO | Invertovaný výstup pre ovládanie výkonového mostíka B (J7-2) | ✔️ | BRDGR_LHIGH |
| PA8 | HC7 | DO | Invertovaný výstup pre ovládanie výkonového mostíka B (J7-1) | ✔️ | BRDGR_RLOW |
| PC10 | HC6 | DO | Invertovaný výstup pre ovládanie výkonového mostíka B (J7-3) | ✔️ | BRDGR_LLOW |

## buzzer
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| !PD10 | BUZ | DO | Výstup pre zapnute/vypnute piezoelektrického akustckého indikátora | ✔️ | BUZZEN |

# panel
## out
### led
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PC0 | LED1 | DO | Panelová LED, môže byť použitá pre indikáciu znižovania teploty | ✔️ | LED1_COOL |
| PC1 | LED0 | DO | Panelová LED, môže byť použitá pre indikáciu zvyšovania teploty | ✔️ | LED0_HEAT |
| PC2 | LED2 | DO | Panelová LED, môže byť použitá pre indikáciu toku dát cez RS232/UART | ✔️ | LED2_RS232 |
| PC3 | LED3 | DO | Panelová LED, môže byť použitá pre indikáciu toku dát cez USB/UART | ✔️ | LED3_USB |
### sevseg
#### white
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PA15 | WHBK | DO | Výstup pre nastavovanie jasu bieleho displeja | ✔️ | TIM2_CH1_SEVW_BKLT |
| PE0 | WH3 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 0 | ✔️ | SEVW_CC0 |
| PE1 | WH4 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 1 | ✔️ | SEVW_CC1 |
| PE2 | WH0 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 2 | ✔️ | SEVW_CC2 |
| PE3 | WH2 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 3 | ✔️ | SEVW_CC3 |
| PE4 | WH1 | DO | Výstup pre riadenie spoločnej katódy bieleho 7-segmentového displeja 4 | ✔️ | SEVW_CC4 |
| PE7 | WHA | DO | Výstup pre riadenie segmentov A | ✔️ | SEVW_SA |
| PE8 | WHB | DO | Výstup pre riadenie segmentov B | ✔️ | SEVW_SB |
| PE9 | WHC | DO | Výstup pre riadenie segmentov C | ✔️ | SEVW_SC |
| PE10 | WHD | DO | Výstup pre riadenie segmentov D | ✔️ | SEVW_SD |
| PE11 | WHE | DO | Výstup pre riadenie segmentov E | ✔️ | SEVW_SE |
| PE12 | WHF | DO | Výstup pre riadenie segmentov F | ✔️ | SEVW_SF |
| PE13 | WHG | DO | Výstup pre riadenie segmentov G | ✔️ | SEVW_SG |
| PE14 | WHDP | DO | Výstup pre riadenie segmentov DP | ✔️ | SEVW_SDP |
#### green_yellow
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PB12 | DDCS | DO | Selektovací výstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ✔️ | SPI2_SEVYG_NSS |
| PB13 | SCLK2 | DO | Hodinový výstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ✔️ | SPI2_SEVYG_SCK |
| PB14 | MISO2 | DI | Dátový vstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ✔️ | SPI2_SEVYG_MISO |
| PB15 | MOSI2 | DO | Dátový výstup pre SPI pripojené ku ovládaču sekundárnych 7-segmentových displejov | ✔️ | SPI2_SEVYG_MOSI |
## in
### button
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PD0 | SW0 | DI | Vstup z pravého krajného panelového spínača | ✔️ | BUT0_FR_EXTI0 |
| PD1 | SW1 | DI | Vstup z pravého stredného panelového spínača | ✔️ | BUT1_MD_EXTI1 |
| PD2 | SW2 | DI | Vstup z ľavého stredného panelového spínača | ✔️ | BUT2_ML_EXTI2 |
| PD3 | SW3 | DI | Vstup z ľavého krajného panelového spínača | ✔️ | BUT3_FL_EXTI3 |
### encoder
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PB10 | ENCA | DI | Vstup A z kvadratúrneho enkodéra | ✔️ | ENCA_EXTI10  |
| PB11 | ENCB | DI | Vstup B z kvadratúrneho enkodéra | ✔️ | ENCB |
| PE15 | SWENC | DI | Vstup zo spínača integrovaného v kvadratúrnom enkodéri | 🔎 | N/A |

# sens
## spi
### temp
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PB3 | SCLK1 | DO | Hodinový výstup pre SPI pripojené k obom R/T prevodníkom | ✔️ | SPI3_TEMP_SCK |
| PB4 | MISO1 | DI | Dátový vstup pre SPI pripojené k obom R/T prevodníkom | ✔️ | SPI3_TEMP_MISO |
| PB5 | MOSI1 | DO | Dátový výstup pre SPI pripojené k obom R/T prevodníkom | ✔️ | SPI3_TEMP_MOSI |
| PD4 | DRDY0 | DI | Vstup z prvého R/T prevodníka indikujúci pripravenosť dát na čítanie | ✔️️ | SPI3_TEMP_NDRDY0 |
| PD5 | MCS0 | DO | Selektovací výstup pre SPI pripojené k prvému R/T prevodníku | ✔️ | SPI3_TEMP_NSS0 |
| PD6 | DRDY1 | DI | Vstup z druhého R/T prevodníka indikujúci pripravenosť dát na čítanie | ✔️️ | SPI3_TEMP_NDRDY1 |
| PD7 | MCS1 | DO | Selektovací výstup pre SPI pripojené k druhému R/T prevodníku | ✔️ | SPI3_TEMP_NSS1 |
## i2c
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PB6 | SDA1 | DIO | Obojsmerné sériové dáta pre senzor osvetlenia, senzory teploty a vlhkost a pre EEPROM | ✔️ | IC21_SCL |
| PB7 | SCL1 | DO | Hodinový výstup pre senzor osvetlenia, senzory teploty a vlhkost a pre EEPROM | ✔️ | IC21_SDA |
### light
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| none |
### temp_hum
| Pin   | Signál  | Typ  | Popis                                         | Status | User Label |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| PA6 | ALRT0 | DI | Prerušenie zo senzora teploty a vlhkost na doske (v miestnost) | ❌️ | N/A |
| PA7 | ALRT1 | DI | Prerušenie z externého senzora teploty a vlhkost (v komore) | 🔎 | N/A |
### ~~eeprom~~
| ~~Pin~~   | ~~Signál~~  | ~~Typ~~ | ~~Popis~~                                         | ~~Status~~ | ~~User Label~~ |
|-------|---------|------|-----------------------------------------------| --- | ------------- |
| ~~PC13~~ | ~~WCN~~ | ~~DO~~ | ~~Výstup pre riadenie zápisu do EEPROM~~ | ~~❌~~ | N/A |