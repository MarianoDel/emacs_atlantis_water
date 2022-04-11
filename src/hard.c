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


//---- end of file ----//
