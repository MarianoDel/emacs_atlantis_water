//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### HARD.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "stm32f0xx.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------
void Led_Link_On (void)
{
    LED_LINK_ON;
}


void Led_Link_Off (void)
{
    LED_LINK_OFF;
}


unsigned char Led_Link_Is_On (void)
{
    return LED_LINK;
}


void Led_Pulse1_On (void)
{
    LED_PULSE1_ON;
}


void Led_Pulse1_Off (void)
{
    LED_PULSE1_OFF;
}


unsigned char Led_Pulse1_Is_On (void)
{
    return LED_PULSE1;
}


void Led_Pulse2_On (void)
{
    LED_PULSE2_ON;
}


void Led_Pulse2_Off (void)
{
    LED_PULSE2_OFF;
}


unsigned char Led_Pulse2_Is_On (void)
{
    return LED_PULSE2;
}


void Led_Pulse3_On (void)
{
    LED_PULSE3_ON;
}


void Led_Pulse3_Off (void)
{
    LED_PULSE3_OFF;
}


unsigned char Led_Pulse3_Is_On (void)
{
    return LED_PULSE3;
}


void Led_Pulse4_On (void)
{
    LED_PULSE4_ON;
}


void Led_Pulse4_Off (void)
{
    LED_PULSE4_OFF;
}


unsigned char Led_Pulse4_Is_On (void)
{
    return LED_PULSE4;
}


//---- end of file ----//
