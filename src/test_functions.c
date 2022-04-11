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
#include "temperatures.h"
#include "dsp.h"
#include "stm32f0xx.h"



#include <stdio.h>


// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------
extern volatile unsigned short timer_standby;
extern volatile unsigned short adc_ch [];

extern ma32_u16_data_obj_t pote_1_filter;
extern ma32_u16_data_obj_t pote_2_filter;

// extern volatile unsigned char f_channel_2_int;
// extern volatile unsigned char f_channel_4_int;
// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led(void);
void TF_Pulse_Inputs_Led_Outputs (void);
void TF_Adc_Inputs_Led_Outputs (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led ();
    // TF_Pulse_Inputs_Led_Outputs ();
    TF_Adc_Inputs_Led_Outputs ();
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




// void TF_Adc_With_DMA_TIM16_Pwm_CH1N (void)
// {
//     TIM_16_Init ();
    
//     // Init ADC and DMA
//     AdcConfig();
//     DMAConfig();
//     DMA1_Channel1->CCR |= DMA_CCR_EN;
//     ADC1->CR |= ADC_CR_ADSTART;

//     while (1)
//     {
//         Wait_ms(5);
//         // Update_TIM16_CH1N (Temp_Channel);
//         // Update_TIM16_CH1N (Pote_Channel_1);
//         Update_TIM16_CH1N (Pote_Channel_2);        
//     }
    
// }


// void TF_Two_Complete_Channels_Hardware (void)
// {
//     // Init ADC and DMA
//     AdcConfig();
//     DMAConfig();
//     DMA1_Channel1->CCR |= DMA_CCR_EN;
//     ADC1->CR |= ADC_CR_ADSTART;

//     // Start of Complete Pote Channel 1
//     TIM_14_Init ();
//     TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
//     // Start of Complete Pote Channel 2
//     TIM_16_Init ();
//     TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
//     while (1)
//     {
//         Wait_ms(5);

//         Update_TIM14_CH1 (Pote_Channel_1);
//         Update_TIM16_CH1N (Pote_Channel_2);        
//     }
    
// }


// void TF_Two_Complete_Channels_Hardware_With_Offset (void)
// {
//     // Init ADC and DMA
//     AdcConfig();
//     DMAConfig();
//     DMA1_Channel1->CCR |= DMA_CCR_EN;
//     ADC1->CR |= ADC_CR_ADSTART;

//     // Start of Complete Pote Channel 1
//     TIM_14_Init ();
//     TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
//     // Start of Complete Pote Channel 2
//     TIM_16_Init ();
//     TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
//     while (1)
//     {
//         Wait_ms(5);

//         Update_TIM14_CH1 (Pote_Channel_1 + 125);
//         Update_TIM16_CH1N (Pote_Channel_2 + 125);        
//     }
    
// }


// void TF_TIM17_Interrupt (void)
// {
//     TIM_17_Init ();

//     TIM17Enable();
//     while (1);
    
// }


// void TF_TIM17_Interrupt_Soft_Pwm (void)
// {
//     TIM_17_Init ();

//     // Start of Complete Pote Channel 1    
//     TIM_14_Init ();
//     Update_TIM14_CH1 (125);    
//     TIM_1_Init_pwm_neg_CH1_trig_CH2 ();

//     // Start of Complete Pote Channel 2
//     TIM_16_Init ();
//     Update_TIM16_CH1N (125);
//     TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
//     PWM_Soft_Set_Channels (1, 16);
//     PWM_Soft_Set_Channels (2, 0);
    
//     TIM17Enable();
//     while (1);
    
// }


// void TF_Two_Complete_Channels_Hardware_With_Offset_Soft_PWM (void)
// {
//     // Init ADC and DMA
//     AdcConfig();
//     DMAConfig();
//     DMA1_Channel1->CCR |= DMA_CCR_EN;
//     ADC1->CR |= ADC_CR_ADSTART;

//     // Start of Complete Pote Channel 1
//     TIM_14_Init ();
//     TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
//     // Start of Complete Pote Channel 2
//     TIM_16_Init ();
//     TIM_3_Init_pwm_neg_CH1_trig_CH2 ();

//     TIM_17_Init ();

//     PWM_Soft_Set_Channels (1, 0);
//     PWM_Soft_Set_Channels (2, 0);
    
//     TIM17Enable();
    
//     unsigned short ch1_input_filtered = 0;
//     unsigned short ch2_input_filtered = 0;

//     unsigned short bright = 0;
//     unsigned short temp0 = 0;
//     unsigned short temp1 = 0;

//     unsigned int calc = 0;
    

//     MA32_U16Circular_Reset (&pote_1_filter);
//     MA32_U16Circular_Reset (&pote_2_filter);    
//     while (1)
//     {
//         Wait_ms(5);

//         ch1_input_filtered = MA32_U16Circular (&pote_1_filter, Pote_Channel_1);
//         ch2_input_filtered = MA32_U16Circular (&pote_2_filter, Pote_Channel_2);

//         //Mezcla de colores
//         bright = ch1_input_filtered;
//         temp0 = 4095 - ch2_input_filtered;
//         temp1 = 4095 - temp0;
        
//         calc = temp0 * bright;
//         calc >>= 12;    // to 4095
//         ch1_input_filtered = (unsigned short) calc;
        
//         calc = temp1 * bright;
//         calc >>= 12;    // to 4095
//         ch2_input_filtered = (unsigned short) calc;
//         // end of Mezcla colores
        

//         if (ch1_input_filtered > 271)
//         {
//             PWM_Soft_Set_Channels (1, 256);
//             Update_TIM14_CH1 (ch1_input_filtered - 16);
//         }
//         else if (ch1_input_filtered > 15)
//         {
//             ch1_input_filtered -= 16;
//             PWM_Soft_Set_Channels (1, ch1_input_filtered);
//             Update_TIM14_CH1 (255);
//         }
//         else
//         {
//             PWM_Soft_Set_Channels (1, 0);
//             Update_TIM14_CH1 (255);
//         }

        
//         if (ch2_input_filtered > 271)
//         {
//             PWM_Soft_Set_Channels (2, 256);
//             Update_TIM16_CH1N (ch2_input_filtered - 16);
//         }
//         else if (ch2_input_filtered > 15)
//         {
//             ch2_input_filtered -= 16;
//             PWM_Soft_Set_Channels (2, ch2_input_filtered);
//             Update_TIM16_CH1N (255);
//         }
//         else
//         {
//             PWM_Soft_Set_Channels (2, 0);
//             Update_TIM16_CH1N (255);
//         }
//     }
// }

//--- end of file ---//
