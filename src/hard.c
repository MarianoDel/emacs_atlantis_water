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


// Module Private Types Constants and Macros -----------------------------------
#define CHECK_PULSE_TIMER    1
#define PULSES_COUNTER_ROOF    100
#define PULSES_COUNTER_THRESHOLD    30



// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char pulse_timeouts = 0;


// Module Private Functions ----------------------------------------------------
void PulseFilter (unsigned char * p_cntr, unsigned char input);
void PulseEdgeDetect (unsigned short * p_edges, unsigned char * p_cntr, unsigned char * p_last_edge);



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


unsigned char pulse_cntr[4] = { 0 };
unsigned short pulse_edge_cntr[4] = { 0 };
unsigned char pulse_last_edge[4] = { 0 };
void HARD_UpdatePulsesFilters (void)
{
    if (pulse_timeouts)
        return;
    
    pulse_timeouts = CHECK_PULSE_TIMER;

    PulseFilter(&pulse_cntr[0], PULSE1_INPUT);
    PulseFilter(&pulse_cntr[1], PULSE2_INPUT);
    PulseFilter(&pulse_cntr[2], PULSE3_INPUT);
    PulseFilter(&pulse_cntr[3], PULSE4_INPUT);

    PulseEdgeDetect(&pulse_edge_cntr[0], &pulse_cntr[0], &pulse_last_edge[0]);
    PulseEdgeDetect(&pulse_edge_cntr[1], &pulse_cntr[1], &pulse_last_edge[1]);
    PulseEdgeDetect(&pulse_edge_cntr[2], &pulse_cntr[2], &pulse_last_edge[2]);
    PulseEdgeDetect(&pulse_edge_cntr[3], &pulse_cntr[3], &pulse_last_edge[3]);    

}


unsigned char HARD_GetPulsesValue (unsigned char channel)
{
    unsigned char current_value = 0;
    
    //check the pulse value
    if (pulse_cntr[channel] > PULSES_COUNTER_THRESHOLD)
        current_value = 1;
    else
        current_value = 0;

    return current_value;
}


unsigned short HARD_GetPulses (unsigned char channel)
{
    return pulse_edge_cntr[channel];
}


void HARD_SetPulses (unsigned char channel, unsigned short value)
{
    pulse_edge_cntr[channel] = value;
}


void PulseFilter (unsigned char * p_cntr, unsigned char input)
{
    if (input)
    {
        if (*p_cntr < PULSES_COUNTER_ROOF)
            *p_cntr += 1;
    }
    else
    {
        if (*p_cntr)
            *p_cntr -= 1;        
    }    
}


void PulseEdgeDetect (unsigned short * p_edges, unsigned char * p_cntr, unsigned char * p_last_edge)
{
    unsigned char current_value = 0;
    
    //check if we have rising edge on edge
    if (*p_cntr > PULSES_COUNTER_THRESHOLD)
        current_value = 1;
    else
        current_value = 0;
    
    if ((*p_last_edge == 0) && (current_value == 1))    // have a new rising edge
        *p_edges += 1;

    if (*p_last_edge != current_value)
        *p_last_edge = current_value;
    
}


void HARD_Timeouts (void)
{
    if (pulse_timeouts)
        pulse_timeouts--;
}


//---- end of file ----//
