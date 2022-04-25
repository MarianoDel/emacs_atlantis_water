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
#define LED_LINK_TIMEOUT    100
#define LED_PULSE_TOGGLE_TIMEOUT    150
#define LED_PULSE_CYCLES    40    // total 6 secs


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char led_link_active = 0;
volatile unsigned short led_link_timer = 0;
volatile unsigned short led_pulse_timer = 0;

volatile unsigned char led_pulse_active = 0;
volatile unsigned char led_pulse_cntr = 0;

volatile unsigned char leds_to_toggle = 0;
#define L1_TOGGLE_STATUS    (leds_to_toggle & L1_TOGGLE)
#define L2_TOGGLE_STATUS    (leds_to_toggle & L2_TOGGLE)
#define L3_TOGGLE_STATUS    (leds_to_toggle & L3_TOGGLE)
#define L4_TOGGLE_STATUS    (leds_to_toggle & L4_TOGGLE)


// Module Private Functions ----------------------------------------------------
void LF_Link_Toggle (void);
void LF_Leds_Pulse_Toggle (void);
void LF_Leds_Pulse_Toggle_Down (void);


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


void LF_Link_Toggle (void)
{
    if (Led_Link_Is_On())
        Led_Link_Off ();
    else
        Led_Link_On ();
}


void LF_Leds_Pulse_Toggle_Activate (void)
{
    led_pulse_active = 1;
    led_pulse_cntr = LED_PULSE_CYCLES;
}


void LF_Leds_Pulse_Toggle_Set (unsigned char ltoggle)
{
    leds_to_toggle |= ltoggle;
}


void LF_Leds_Pulse_Toggle_Reset (unsigned char ltoggle)
{
    leds_to_toggle &= ~ltoggle;
}


void LF_Leds_Pulse_Toggle (void)
{
    if (L1_TOGGLE_STATUS)
    {
        if (Led_Pulse1_Is_On())
            Led_Pulse1_Off();
        else
            Led_Pulse1_On();
    }

    if (L2_TOGGLE_STATUS)
    {
        if (Led_Pulse2_Is_On())
            Led_Pulse2_Off();
        else
            Led_Pulse2_On();
    }

    if (L3_TOGGLE_STATUS)
    {
        if (Led_Pulse3_Is_On())
            Led_Pulse3_Off();
        else
            Led_Pulse3_On();
    }

    if (L4_TOGGLE_STATUS)
    {
        if (Led_Pulse4_Is_On())
            Led_Pulse4_Off();
        else
            Led_Pulse4_On();
    }
}


void LF_Leds_Pulse_Toggle_Down (void)
{
    if (L1_TOGGLE_STATUS)
        Led_Pulse1_Off();

    if (L2_TOGGLE_STATUS)
        Led_Pulse2_Off();

    if (L3_TOGGLE_STATUS)
        Led_Pulse3_Off();

    if (L4_TOGGLE_STATUS)
        Led_Pulse4_Off();
    
}


void LF_Timeouts (void)
{
    // timeouts
    if (led_link_timer)
        led_link_timer--;

    if (led_pulse_timer)
        led_pulse_timer--;
    
    // other functions
    if (led_link_active)
    {
        if (!led_link_timer)
        {
            LF_Link_Toggle();
            led_link_active = 0;
        }
    }

    if (led_pulse_active)
    {
        if (!led_pulse_timer)
        {
            if (led_pulse_cntr)
            {
                led_pulse_cntr--;
                led_pulse_timer = LED_PULSE_TOGGLE_TIMEOUT;
                LF_Leds_Pulse_Toggle();
            }
            else
            {
                led_pulse_active = 0;
                LF_Leds_Pulse_Toggle_Down();
            }
        }
    }
}

//--- end of file ---//
