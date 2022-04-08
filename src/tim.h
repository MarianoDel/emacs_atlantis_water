//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### TIM.H #################################
//---------------------------------------------
#ifndef _TIM_H_
#define _TIM_H_

#include "hard.h"    //for freq conf

// Exported Types Constants and Macros -----------------------------------------
#define DUTY_NONE    (DUTY_00_PERCENT)
#define DUTY_MAX    (DUTY_100_PERCENT)
#define DUTY_ALWAYS    (DUTY_100_PERCENT + 1)

#define VALUE_FOR_LEAST_FREQ    1200    //40KHz

#ifdef USE_PWM_16000_FREQ_1KHZ
#define DUTY_00_PERCENT		0
#define DUTY_10_PERCENT		1600
#define DUTY_20_PERCENT		3200
#define DUTY_50_PERCENT		8000
#define DUTY_100_PERCENT	16000

#define USE_TIM_FREQ_16KHZ
#endif

#ifdef USE_PWM_8000_FREQ_2KHZ
#define DUTY_00_PERCENT		0
#define DUTY_10_PERCENT		800
#define DUTY_20_PERCENT		1600
#define DUTY_50_PERCENT		4000
#define DUTY_100_PERCENT	8000

#define USE_TIM_FREQ_16KHZ
#endif

#ifdef USE_PWM_4000_FREQ_4KHZ
#define DUTY_00_PERCENT		0
#define DUTY_10_PERCENT		400
#define DUTY_20_PERCENT		800
#define DUTY_50_PERCENT		2000
#define DUTY_100_PERCENT	4000

#define USE_TIM_FREQ_4_8KHZ
#endif

#ifdef USE_PWM_1000_FREQ_4_8KHZ
#define DUTY_00_PERCENT		0
#define DUTY_10_PERCENT		100
#define DUTY_20_PERCENT		200
#define DUTY_50_PERCENT		500
#define DUTY_80_PERCENT		800
#define DUTY_70_PERCENT		700
#define DUTY_95_PERCENT		950
#define DUTY_100_PERCENT	1000

#define USE_TIM_FREQ_4_8KHZ
#endif

#define EnablePreload_TIM3_CH1    (TIM3->CCMR1 |= TIM_CCMR1_OC1PE)
#define EnablePreload_TIM3_CH2    (TIM3->CCMR1 |= TIM_CCMR1_OC2PE)

#define DisablePreload_TIM3_CH1    (TIM3->CCMR1 &= ~TIM_CCMR1_OC1PE)
#define DisablePreload_TIM3_CH2    (TIM3->CCMR1 &= ~TIM_CCMR1_OC2PE)

#define EnablePreload_TIM1_CH2    (TIM1->CCMR1 |= TIM_CCMR1_OC2PE)
#define EnablePreload_TIM1_CH4    (TIM1->CCMR1 |= TIM_CCMR2_OC4PE)

#define DisablePreload_TIM1_CH2    (TIM1->CCMR1 &= ~TIM_CCMR1_OC2PE)
#define DisablePreload_TIM1_CH4    (TIM1->CCMR1 &= ~TIM_CCMR2_OC4PE)



// Module Exported Functions -----------------------------------------
void TIM_1_Init_pwm_neg_CH1_trig_CH2 (void);

void TIM_3_Init_pwm_neg_CH1_trig_CH2 (void);
void TIM3_IRQHandler (void);

void TIM_6_Init (void);

void TIM_14_Init(void);
void TIM14_IRQHandler (void);

void TIM_16_Init(void);
void TIM16_IRQHandler (void);

void TIM_17_Init(void);
void TIM17_IRQHandler (void);
void TIM17Enable (void);
void TIM17Disable (void);
void TIM_17_Init_Pwm_Int (void);

void Update_TIM1_CH1 (unsigned short);
void Update_TIM1_CH2 (unsigned short);
void Update_TIM1_CH4 (unsigned short);
void Update_TIM3_CH1 (unsigned short);
void Update_TIM3_CH2 (unsigned short);
void Update_TIM3_CH3 (unsigned short);
void Update_TIM3_CH4 (unsigned short);
void Update_TIM14_CH1 (unsigned short); 
void Update_TIM16_CH1N (unsigned short);

void Wait_ms (unsigned short wait);

#endif    /* _TIM_H_ */

//--- end of file ---//
