//------------------------------------------------------
// #### ATLANTIS WATER METER PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### LED_FUNCTIONS.C ################################
//------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "led_functions.h"
#include "hard.h"
#include "stm32f0xx.h"


// Module Private Types & Macros -----------------------------------------------
#define LED_LINK_TIMEOUT    150


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char led_link_active = 0;
volatile unsigned short led_link_timer = 0;


// Module Private Functions ----------------------------------------------------
void LF_Link_Toggle (void);


// Module Functions ------------------------------------------------------------
void LF_Link_Pulse (void)
{
    LF_Link_Toggle();
    led_link_active = 1;
    led_link_timer = LED_LINK_TIMEOUT;
}


void LF_Link_Set (void)
{
    Led_Link_On ();
}


void LF_Link_Reset (void)
{
    Led_Link_Off ();
}


void LF_Timeouts (void)
{
    // timeout
    if (led_link_timer)
        led_link_timer--;

    // other functions
    if (led_link_active)
    {
        if (!led_link_timer)
        {
            LF_Link_Toggle();
            led_link_active = 0;
        }
    }
}


void LF_Link_Toggle (void)
{
    if (Led_Link_Is_On())
        Led_Link_Off ();
    else
        Led_Link_On ();
}


//--- end of file ---//
