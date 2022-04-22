//------------------------------------------------------
// #### ATLANTIS WATER METER PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #########################################
//------------------------------------------------------
#ifndef _HARD_H_
#define _HARD_H_


//-- Defines For Configuration -------------------
//---- Configuration for Hardware Versions ---
// #define HARDWARE_VERSION_1_1    //
#define HARDWARE_VERSION_1_0    // micro F030K6T6

// #define SOFTWARE_VERSION_1_1
#define SOFTWARE_VERSION_1_0

//---- Features Configuration ----------------

//-- End of Features Configuration ---------------


// Exported Pinout Names -------------------------------------------------------
// #ifdef HARDWARE_VERSION_1_0
//GPIOA pin0
#define PULSE1_INPUT    ((GPIOA->IDR & 0x0001) == 0)
//GPIOA pin1    Analog Pulse 1 Input

//GPIOA pin2
#define PULSE2_INPUT    ((GPIOA->IDR & 0x0004) == 0)
//GPIOA pin3    Analog Pulse 2 Input

//GPIOA pin4
#define PULSE3_INPUT    ((GPIOA->IDR & 0x0010) == 0)
//GPIOA pin5    Analog Pulse 3 Input

//GPIOA pin6
#define PULSE4_INPUT    ((GPIOA->IDR & 0x0040) == 0)
//GPIOA pin7    Analog Pulse 4 Input

//GPIOB pin0
#define LED_LINK    ((GPIOB->ODR & 0x0001) != 0)
#define LED_LINK_ON    (GPIOB->BSRR = 0x00000001)
#define LED_LINK_OFF    (GPIOB->BSRR = 0x00010000)

//GPIOB pin1    NC

//GPIOA pin8    NC

//GPIOA pin9    Usart1 Tx
//GPIOA pin10    Usart1 Rx

//GPIOA pin11
#define SW_RX_TX    ((GPIOA->ODR & 0x0800) != 0)
#define SW_RX_TX_ON    (GPIOA->BSRR = 0x00000800)
#define SW_RX_TX_OFF    (GPIOA->BSRR = 0x08000000)
#define SW_RX_TX_DE    SW_RX_TX_ON
#define SW_RX_TX_RE_NEG    SW_RX_TX_OFF


//GPIOA pin12
//GPIOA pin13
//GPIOA pin14    NC

//GPIOA pin15    
#define LED_PULSE4    ((GPIOA->ODR & 0x8000) != 0)
#define LED_PULSE4_ON    (GPIOA->BSRR = 0x00008000)
#define LED_PULSE4_OFF    (GPIOA->BSRR = 0x80000000)

//GPIOB pin3
#define LED_PULSE3    ((GPIOB->ODR & 0x0008) != 0)
#define LED_PULSE3_ON    (GPIOB->BSRR = 0x00000008)
#define LED_PULSE3_OFF    (GPIOB->BSRR = 0x00080000)

//GPIOB pin4
#define LED_PULSE2    ((GPIOB->ODR & 0x0010) != 0)
#define LED_PULSE2_ON    (GPIOB->BSRR = 0x00000010)
#define LED_PULSE2_OFF    (GPIOB->BSRR = 0x00100000)

//GPIOB pin5
#define LED_PULSE1    ((GPIOB->ODR & 0x0020) != 0)
#define LED_PULSE1_ON    (GPIOB->BSRR = 0x00000020)
#define LED_PULSE1_OFF    (GPIOB->BSRR = 0x00200000)

//GPIOB pin6    
//GPIOB pin7    NC

// #endif    //HARDWARE_VER_1_0

// Exported Types & Macros -----------------------------------------------------
#define LED_SYS_ERR    LED_LINK
#define LED_SYS_ERR_ON    LED_LINK_ON
#define LED_SYS_ERR_OFF    LED_LINK_OFF


//--- Stringtify Utils -----------------------
#define str_macro(s) #s


// Module Exported Functions ---------------------------------------------------
void Led_Link_On (void);
void Led_Link_Off (void);
unsigned char Led_Link_Is_On (void);

void Led_Pulse1_On (void);
void Led_Pulse1_Off (void);
unsigned char Led_Pulse1_Is_On (void);

void Led_Pulse2_On (void);
void Led_Pulse2_Off (void);
unsigned char Led_Pulse2_Is_On (void);

void Led_Pulse3_On (void);
void Led_Pulse3_Off (void);
unsigned char Led_Pulse3_Is_On (void);

void Led_Pulse4_On (void);
void Led_Pulse4_Off (void);
unsigned char Led_Pulse4_Is_On (void);

void HARD_UpdatePulsesFilters (void);
unsigned char HARD_GetPulsesValue (unsigned char channel);
unsigned short HARD_GetPulses (unsigned char channel);
void HARD_SetPulses (unsigned char channel, unsigned short value);
void HARD_Timeouts (void);


#endif    /* _HARD_H_ */

//--- end of file ---//

