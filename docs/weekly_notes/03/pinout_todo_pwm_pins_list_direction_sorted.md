# pwm
## out
| Pin   | Signál  | Typ  | Popis                                         | Status | Available TIM |
|-------|---------|------|-----------------------------------------------| --- | --- |
| PA0 | FC4 | DO | Riadenie ventilátora 4 | ⬜️ | TIM5_CH1 |
| PA2 | FC5 | DO | Riadenie ventilátora 5 | ⬜️ | TIM5_CH3 |
| PB8 | FC7 | DO | Riadenie ventilátora 7 | ⬜️ | TIM10_CH1 |
| PC6 | FC0 | DO | Riadenie ventilátora 0 | ⬜️ | TIM3_CH1 |
| PC8 | FC1 | DO | Riadenie ventilátora 0 | ⬜️ | TIM3_CH3 |
| PD12 | FC2 | DO | Riadenie ventilátora 2 | ⬜️ | TIM4_CH1 |
| PD14 | FC3 | DO | Riadenie ventilátora 3 | ⬜️ | TIM4_CH3 |
| PE5 | FC6 | DO | Riadenie ventilátora 6 | ⬜️ | TIM9_CH1 |
| PA15 | WHBK | DO | Výstup pre nastavovanie jasu bieleho displeja | ⬜️ | TIM2_CH1 |

## in
| Pin   | Signál  | Typ  | Popis                                         | Status | Available TIM |
|-------|---------|------|-----------------------------------------------| --- | --- |
| PA1 | FF4 | DI | Spätná väzba ventilátora 4 | ❌ | TIM5_CH2 |
| PA3 | FF5 | DI | Spätná väzba ventilátora 5 | ❌ | TIM5_CH4 |
| PB9 | FF7 | DI | Spätná väzba ventilátora 7 | ❌ | TIM11_CH1 |
| PC7 | FF0 | DI | Spätná väzba ventilátora 0 | ❌ | TIM3_CH2 |
| PC9 | FF1 | DI | Spätná väzba ventilátora 0 | ❌ | TIM3_CH4 |
| PD13 | FF2 | DI | Spätná väzba ventilátora 2 | ❌ | TIM4_CH2 |
| PD15 | FF3 | DI | Spätná väzba ventilátora 3 | ❌ | TIM4_CH4 |
| PE6 | FF6 | DI | Spätná väzba ventilátora 6 | ❌ | TIM9_CH2 |

# pwm
## out
| Pin   | Signál  | Typ  | Popis                                         | Status | Available TIM |
|-------|---------|------|-----------------------------------------------| --- | --- |
| PA0 | FC4 | DO | Riadenie ventilátora 4 | ✔️ | TIM2_CH1, TIM5_CH1, TIM8 |
| PA2 | FC5 | DO | Riadenie ventilátora 5 | 🟡 | TIM2_CH3, TIM5_CH3, TIM9_CH1 |
| PB8 | FC7 | DO | Riadenie ventilátora 7 | 🟡 | TIM10_CH1, TIM4_CH3 |
| PC6 | FC0 | DO | Riadenie ventilátora 0 | 🟡 | TIM3_CH1, TIM8_CH1 |
| PC8 | FC1 | DO | Riadenie ventilátora 0 | 🟡 | TIM3_CH3, TIM8_CH3 |
| PD12 | FC2 | DO | Riadenie ventilátora 2 | 🟡 | TIM4_CH1 |
| PD14 | FC3 | DO | Riadenie ventilátora 3 | 🟡 | TIM4_CH3 |
| PE5 | FC6 | DO | Riadenie ventilátora 6 | 🟡 | TIM9_CH1 |
| PA15 | WHBK | DO | Výstup pre nastavovanie jasu bieleho displeja | 🟡 | TIM2_CH1 |
## in
| Pin   | Signál  | Typ  | Popis                                         | Status | Available TIM |
|-------|---------|------|-----------------------------------------------| --- | --- |
| PA1 | FF4 | DI | Spätná väzba ventilátora 4 | ❌ | TIM2_CH2, TIM5_CH2 |
| PA3 | FF5 | DI | Spätná väzba ventilátora 5 | ❌ | TIM2_CH4, TIM5_CH4, TIM9_CH2 |
| PB9 | FF7 | DI | Spätná väzba ventilátora 7 | ❌ | TIM11_CH1, TIM4_CH4 |
| PC7 | FF0 | DI | Spätná väzba ventilátora 0 | ❌ | TIM3_CH2, TIM8_CH2 |
| PC9 | FF1 | DI | Spätná väzba ventilátora 0 | ❌ | TIM3_CH4, TIM8_CH4 |
| PD13 | FF2 | DI | Spätná väzba ventilátora 2 | ❌ | TIM4_CH2 |
| PD15 | FF3 | DI | Spätná väzba ventilátora 3 | ❌ | TIM4_CH4 |
| PE6 | FF6 | DI | Spätná väzba ventilátora 6 | ❌ | TIM9_CH2 |
## encoder
| Pin   | Signál  | Typ  | Popis                                         | Status | Available TIM |
|-------|---------|------|-----------------------------------------------| --- | --- |
| PB10 | ENCA | DI | Vstup A z kvadratúrneho enkodéra | ❌ |  TIM2_CH3 |
| PB11 | ENCB | DI | Vstup B z kvadratúrneho enkodéra | ❌ |  TIM2_CH4 |