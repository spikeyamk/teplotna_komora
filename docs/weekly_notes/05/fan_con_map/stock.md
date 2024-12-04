# Location
| Row | Left | Center | Right |
| --- | ---- | ---- | ---- |
| 0   | FL   | BL   | FR   |
| 1   | RL   | RR   | BR   |

# Function and TIM
| Pin  | Function | TIM      |
| ---- | -------- | -------- |
| PA0  | BR_CTL   | TIM5_CH1 |
| PA1  | BR_FB    | TIM5_CH2 |
| PA2  | FR_CTL   | TIM5_CH3 |
| PA3  | FR_FB    | TIM5_CH4 |
| PC6  | RL_CTL   | TIM3_CH1 |
| PC7  | RL_FB    | TIM3_CH2 |
| PC8  | FL_CTL   | TIM3_CH3 |
| PC9  | FL_FB    | TIM3_CH4 |
| PD12 | RR_CTL   | TIM4_CH1 |
| PD13 | RR_FB    | TIM4_CH2 |
| PD14 | BL_CTL   | TIM4_CH3 |
| PD15 | BL_FB    | TIM4_CH4 |

# Location per function
|     | 0   | 1   | 2    | 3    | 4   | 5   |
| --- | --- | --- | ---- | ---- | --- | --- |
| 0   | FL_CTL | FL_FB | BL_CTL | BL_FB | FR_CTL | FR_FB |
| 1   | RL_CTL | RL_FB | RR_CTL | RR_FB | BR_CTL | BR_FB |

# Location per TIM
|     | 0        | 1        | 2         | 3         | 4        | 5        |
| --- | -------- | -------- | --------- | --------- | -------- | -------- |
| 0   | TIM3_CH3 | TIM3_CH4 | TIM4_CH3  | TIM4_CH4  | TIM5_CH3 | TIM5_CH4 |
| 1   | TIM3_CH1 | TIM3_CH2 | TIM4_CH1  | TIM4_CH2  | TIM5_CH1 | TIM5_CH2 |