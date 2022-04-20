//------------------------------------------------------
// #### ATLANTIS WATER METER PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MAIN.C #########################################
//------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "stm32f0xx.h"

#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "dma.h"

#include "test_functions.h"
#include "dsp.h"

#include <stdio.h>
#include <string.h>


// Module Types Constants and Macros -------------------------------------------
typedef enum {
    MAIN_HARD_INIT,
    MAIN_RUNNING,
    MAIN_IN_OVERTEMP
    
} main_state_e;



// Externals -------------------------------------------------------------------
// externals for ADC
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];
// volatile unsigned char seq_ready;

// externals for timers
volatile unsigned short wait_ms_var = 0;

// Globals ---------------------------------------------------------------------
//-- Timers globals ----------------------------------
volatile unsigned short timer_standby = 0;

// //-- for the filters and outputs
// ma32_u16_data_obj_t pote_1_filter;
// ma32_u16_data_obj_t pote_2_filter;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);


// Module Functions ------------------------------------------------------------
int main(void)
{
    //GPIO Configuration.
    GPIO_Config();

    //Systick Timer
    if (SysTick_Config(48000))
        SysTickError();

    // Begin Hardware Tests - check test_functions module
    TF_Hardware_Tests ();
    

    // Hardware Inits. ---------------------------
    // Init ADC and DMA
    // AdcConfig();
    // DMAConfig();
    // DMA1_Channel1->CCR |= DMA_CCR_EN;
    // ADC1->CR |= ADC_CR_ADSTART;

    // // Start of Complete Pote Channel 1
    // TIM_14_Init ();
    // TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // // Start of Complete Pote Channel 2
    // TIM_16_Init ();
    // TIM_3_Init_pwm_neg_CH1_trig_CH2 ();

    // // Init TIM 17 for Soft or Int init
    // TIM_17_Init ();

    // PWM_Soft_Set_Channels (1, 0);
    // PWM_Soft_Set_Channels (2, 0);
    
    unsigned short ch1_input_filtered = 0;
    unsigned short ch2_input_filtered = 0;

    unsigned short bright = 0;
    unsigned short temp0 = 0;
    unsigned short temp1 = 0;

    unsigned int calc = 0;
    
    main_state_e main_state = MAIN_HARD_INIT;

    while (1)
    {
    //     switch (main_state)
    //     {
    //     case MAIN_HARD_INIT:
            
    //         MA32_U16Circular_Reset (&pote_1_filter);
    //         MA32_U16Circular_Reset (&pote_2_filter);    

    //         TIM17Enable();
            
    //         main_state++;
    //         break;

    //     case MAIN_RUNNING:
            
    //         if (!timer_standby)
    //         {
    //             ch1_input_filtered = MA32_U16Circular (&pote_1_filter, Pote_Channel_1);
    //             ch2_input_filtered = MA32_U16Circular (&pote_2_filter, Pote_Channel_2);

    //             // colors mixer
    //             bright = ch1_input_filtered;
    //             temp0 = 4095 - ch2_input_filtered;
    //             temp1 = 4095 - temp0;
        
    //             calc = temp0 * bright;
    //             calc >>= 12;    // to 4095
    //             ch1_input_filtered = (unsigned short) calc;
        
    //             calc = temp1 * bright;
    //             calc >>= 12;    // to 4095
    //             ch2_input_filtered = (unsigned short) calc;
    //             // end of colors mixer
        

    //             if (ch1_input_filtered > START_OF_CONTINUOS_DIMMER)
    //             {
    //                 PWM_Soft_Set_Channels (1, 256);
    //                 Update_TIM14_CH1 (ch1_input_filtered - START_OF_PWM_DIMMER);
    //             }
    //             else if (ch1_input_filtered > (START_OF_PWM_DIMMER - 1))
    //             {
    //                 ch1_input_filtered -= START_OF_PWM_DIMMER;
    //                 PWM_Soft_Set_Channels (1, ch1_input_filtered);
    //                 Update_TIM14_CH1 (ANALOG_FOR_PWM_DIMMER);
    //             }
    //             else
    //             {
    //                 PWM_Soft_Set_Channels (1, 0);
    //                 Update_TIM14_CH1 (ANALOG_FOR_PWM_DIMMER);
    //             }

        
    //             if (ch2_input_filtered > START_OF_CONTINUOS_DIMMER)
    //             {
    //                 PWM_Soft_Set_Channels (2, 256);
    //                 Update_TIM16_CH1N (ch2_input_filtered - START_OF_PWM_DIMMER);
    //             }
    //             else if (ch2_input_filtered > (START_OF_PWM_DIMMER - 1))
    //             {
    //                 ch2_input_filtered -= START_OF_PWM_DIMMER;
    //                 PWM_Soft_Set_Channels (2, ch2_input_filtered);
    //                 Update_TIM16_CH1N (ANALOG_FOR_PWM_DIMMER);
    //             }
    //             else
    //             {
    //                 PWM_Soft_Set_Channels (2, 0);
    //                 Update_TIM16_CH1N (ANALOG_FOR_PWM_DIMMER);
    //             }

    //             timer_standby = 5;
    //         }            
    //         break;

    //     case MAIN_IN_OVERTEMP:

    //         if (!timer_check_temp)
    //         {
    //             if (Temp_Channel < TEMP_RECONNECT)
    //                 main_state = MAIN_HARD_INIT;
                
    //             timer_check_temp = 2000;    //check again in two seconds            
    //         }
    //         break;

    //     default:
    //         main_state = MAIN_HARD_INIT;
    //         break;
    //     }

        

    }    //end of while 1

    return 0;
}

//--- End of Main ---//


void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

}


void SysTickError (void)
{
    // Capture systick error...
    while (1)
    {
        if (LED_SYS_ERR)
            LED_SYS_ERR_OFF;
        else
            LED_SYS_ERR_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

