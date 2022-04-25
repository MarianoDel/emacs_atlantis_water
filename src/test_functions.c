//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "tim.h"
// #include "pwm.h"
#include "adc.h"
#include "dma.h"
#include "dsp.h"
#include "stm32f0xx.h"

#include "usart.h"
#include "comm.h"


#include <stdio.h>


// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------
extern volatile unsigned short timer_standby;
extern volatile unsigned short adc_ch [];
extern unsigned char valid_packet;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led(void);
void TF_Pulse_Inputs_Led_Outputs (void);
void TF_Adc_Inputs_Led_Outputs (void);
void TF_Pulse_Inputs_Led_Outputs_Filtered (void);
void TF_Echo_Usart1 (void);
void TF_LinkUp_Usart1 (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led ();
    // TF_Pulse_Inputs_Led_Outputs ();
    // TF_Adc_Inputs_Led_Outputs ();
    // TF_Pulse_Inputs_Led_Outputs_Filtered ();
    // TF_Echo_Usart1 ();
    TF_LinkUp_Usart1 ();
}


void TF_Led (void)
{
    while (1)
    {
        if (LED_LINK)
            LED_LINK_OFF;
        else
            LED_LINK_ON;

        Wait_ms(1000);
    }
}


void TF_Pulse_Inputs_Led_Outputs (void)
{
    while (1)
    {
        if (!timer_standby)
        {
            timer_standby = 1000;
            if (LED_LINK)
                LED_LINK_OFF;
            else
                LED_LINK_ON;
        }

        if (PULSE1_INPUT)
            LED_PULSE1_ON;
        else
            LED_PULSE1_OFF;

        if (PULSE2_INPUT)
            LED_PULSE2_ON;
        else
            LED_PULSE2_OFF;

        if (PULSE3_INPUT)
            LED_PULSE3_ON;
        else
            LED_PULSE3_OFF;

        if (PULSE4_INPUT)
            LED_PULSE4_ON;
        else
            LED_PULSE4_OFF;
        

    }
}


void TF_Adc_Inputs_Led_Outputs (void)
{
    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;
    
    while (1)
    {
        if (!timer_standby)
        {
            timer_standby = 300;
            if (LED_LINK)
                LED_LINK_OFF;
            else
                LED_LINK_ON;
        }

        if (Adc_Pulse1 < 350)
            LED_PULSE1_ON;
        else if (Adc_Pulse1 > 3500)
            LED_PULSE1_OFF;

        if (Adc_Pulse2 < 350)
            LED_PULSE2_ON;
        else if (Adc_Pulse2 > 3500)
            LED_PULSE2_OFF;

        if (Adc_Pulse3 < 350)
            LED_PULSE3_ON;
        else if (Adc_Pulse3 > 3500)
            LED_PULSE3_OFF;

        if (Adc_Pulse4 < 350)
            LED_PULSE4_ON;
        else if (Adc_Pulse4 > 3500)
            LED_PULSE4_OFF;

    }
}


void TF_Pulse_Inputs_Led_Outputs_Filtered (void)
{
    unsigned short last_ch1 = 0;
    unsigned short last_ch2 = 0;
    unsigned short last_ch3 = 0;
    unsigned short last_ch4 = 0;

    unsigned char timer_ch1 = 0;
    unsigned char timer_ch2 = 0;
    unsigned char timer_ch3 = 0;
    unsigned char timer_ch4 = 0;
    
    unsigned short pulses = 0;
    
    while (1)
    {
        HARD_Timeouts();
        HARD_UpdatePulsesFilters();

        pulses = HARD_GetPulses(0);
        
        if (pulses != last_ch1)
        {
            Led_Pulse1_On();
            last_ch1 = pulses;
            timer_ch1 = 200;
        }

        pulses = HARD_GetPulses(1);
        
        if (pulses != last_ch2)
        {
            Led_Pulse2_On();
            last_ch2 = pulses;
            timer_ch2 = 200;
        }

        pulses = HARD_GetPulses(2);
        
        if (pulses != last_ch3)
        {
            Led_Pulse3_On();
            last_ch3 = pulses;
            timer_ch3 = 200;
        }

        pulses = HARD_GetPulses(3);
        
        if (pulses != last_ch4)
        {
            Led_Pulse4_On();
            last_ch4 = pulses;
            timer_ch4 = 200;
        }

        if (!timer_standby)
        {
            timer_standby = 1;

            if (timer_ch1)
                timer_ch1--;
            else
                Led_Pulse1_Off();

            if (timer_ch2)
                timer_ch2--;
            else
                Led_Pulse2_Off();

            if (timer_ch3)
                timer_ch3--;
            else
                Led_Pulse3_Off();

            if (timer_ch4)
                timer_ch4--;
            else
                Led_Pulse4_Off();

        }
    }
}


void TF_LinkUp_Usart1 (void)
{
    char local_buff [128] = { 0 };
    
    // Init Usart1
    Usart1Config ();

    while (1)
    {
        if (Usart1HaveData())
        {
            Usart1HaveDataReset();
            Usart1ReadBuffer((unsigned char *) local_buff, 128);
            COMM_ProcessPayload(local_buff);

            if (valid_packet)
            {
                valid_packet = 0;
                timer_standby = 1500;
                Led_Link_On();
            }
        }

        if (!timer_standby)
            Led_Link_Off();
        
    }
}


void TF_Echo_Usart1 (void)
{
    char local_buff [128] = { 0 };
    
    // Init Usart1
    Usart1Config ();

    while (1)
    {
        if (Usart1HaveData())
        {
            Usart1HaveDataReset();
            Usart1ReadBuffer((unsigned char *) local_buff, 128);
            timer_standby = 1000;
            Led_Link_On();

            // do the echo
            Wait_ms(5);
            Usart1SendDelayed(local_buff);
        }

        if (!timer_standby)
            Led_Link_Off();
        
    }
}


//--- end of file ---//
