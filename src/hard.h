//----------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #################################
//----------------------------------------------
#ifndef _HARD_H_
#define _HARD_H_

#include "stm32f0xx.h"


//-- Defines For Configuration -------------------
//---- Configuration for Hardware Versions -------
// #define HARDWARE_VERSION_1_1    //fuente 48V->12V en placa control
#define HARDWARE_VERSION_1_0    //micro F030K6T6


// #define SOFTWARE_VERSION_1_1
#define SOFTWARE_VERSION_1_0


//---- Features Configuration -----------------
#define USE_TEMP_PROT
// #define USE_CTROL_FAN_ALWAYS_ON

// -- Freq for the timers
// #define USE_PWM_16000_FREQ_1KHZ
#define USE_PWM_4000_FREQ_4KHZ
// #define USE_PWM_8000_FREQ_2KHZ
// #define USE_


//---- End of Features Configuration ----------


// Exported Pinout Names -------------------------------------------------------
// #ifdef HARDWARE_VERSION_1_0
//GPIOA pin0    
//GPIOA pin1    
//GPIOA pin2    
//GPIOA pin3    NC

//GPIOA pin4    Analog Temp LM335

//GPIOA pin5    
//GPIOA pin6    NC

//GPIOA pin7    TIM14_CH1

//GPIOB pin0    Analog Adc_In8

//GPIOB pin1    Analog Adc_In9

//GPIOA pin8    TIM1_CH1

//GPIOA pin9    TIM1_CH2

//GPIOA pin10
//GPIOA pin11    NC

//GPIOA pin12
#define LED    ((GPIOA->ODR & 0x1000) == 0)
#define LED_OFF    (GPIOA->BSRR = 0x00001000)
#define LED_ON    (GPIOA->BSRR = 0x10000000)


//GPIOA pin13
//GPIOA pin14    
//GPIOA pin15    NC

//GPIOB pin3    CTRL_FAN
#define CTRL_FAN    ((GPIOB->ODR & 0x0008) != 0)
#define CTRL_FAN_ON    (GPIOB->BSRR = 0x00000008)
#define CTRL_FAN_OFF    (GPIOB->BSRR = 0x00080000)


//GPIOB pin4    TIM3_CH1

//GPIOB pin5    TIM3_CH2

//GPIOB pin6    TIM16_CH1N

//GPIOB pin7

// #endif    //HARDWARE_VER_1_0

// Exported Types & Macros -----------------------------------------------------


//--- Stringtify Utils -----------------------
#define str_macro(s) #s


// Module Exported Functions ---------------------------------------------------


#endif    /* _HARD_H_ */

//--- end of file ---//

