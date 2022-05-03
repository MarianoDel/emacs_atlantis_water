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
#include "usart.h"

#include "test_functions.h"
#include "comm.h"
#include "led_functions.h"
#include "dsp.h"

#include <stdio.h>
#include <string.h>


// Module Types Constants and Macros -------------------------------------------
typedef enum {
    MAIN_WAIT_TO_START,
    MAIN_CHECK_P1,
    MAIN_SEND_P1,
    MAIN_WAIT_ACK_P1,
    MAIN_CHECK_P2,
    MAIN_SEND_P2,
    MAIN_WAIT_ACK_P2,
    MAIN_CHECK_P3,
    MAIN_SEND_P3,
    MAIN_WAIT_ACK_P3,
    MAIN_CHECK_P4,
    MAIN_SEND_P4,
    MAIN_WAIT_ACK_P4
    
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
    // TF_Hardware_Tests ();
    

    // Hardware Inits. ---------------------------
    // Init ADC and DMA
    // AdcConfig();
    // DMAConfig();
    // DMA1_Channel1->CCR |= DMA_CCR_EN;
    // ADC1->CR |= ADC_CR_ADSTART;

    // Init Usart1
    Usart1Config ();
    
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
    
    // unsigned short ch1_input_filtered = 0;
    // unsigned short ch2_input_filtered = 0;

    char send_buff [30] = { 0 };
    unsigned short pulses = 0;

    main_state_e main_state = MAIN_CHECK_P1;
    comm_resp_e resp_comm = resp_working;

    unsigned char pck_tx_error = 0;
    
    COMM_Manager_Reset_SM ();

    while (1)
    {
        switch (main_state)
        {
        case MAIN_WAIT_TO_START:
            if (!timer_standby)
            {
                main_state++;
            }
            break;

        case MAIN_CHECK_P1:
            pulses = HARD_GetPulses(0);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P2;
            
            break;
            
        case MAIN_SEND_P1:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()))
            {
                if (pulses > 999)
                    pulses = 999;

                sprintf(send_buff, "pulses %03d ch1", pulses);
                if (COMM_SendPacket (send_buff) == resp_working)
                    main_state++;
                else
                {
                    pck_tx_error++;
                    main_state = MAIN_CHECK_P2;
                }
            }
            break;

        case MAIN_WAIT_ACK_P1:
            resp_comm = COMM_SendPacket (send_buff);

            if (resp_comm != resp_working)
            {
                if (resp_comm == resp_sended_ok)
                {
                    unsigned short remain_pulses = HARD_GetPulses(0);
                    HARD_SetPulses(0, remain_pulses - pulses);
                    pulses = 0;
                }
                else if ((resp_comm == resp_sended_nok) ||
                         (resp_comm == resp_timeout))
                {
                    pck_tx_error++;
                }

                main_state = MAIN_CHECK_P2;
            }
            break;
            
        case MAIN_CHECK_P2:
            pulses = HARD_GetPulses(1);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P3;

            break;

        case MAIN_SEND_P2:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()))
            {
                if (pulses > 999)
                    pulses = 999;

                sprintf(send_buff, "pulses %03d ch2", pulses);
                if (COMM_SendPacket (send_buff) == resp_working)
                    main_state++;
                else
                {
                    pck_tx_error++;                
                    main_state = MAIN_CHECK_P3;
                }
            }
            break;

        case MAIN_WAIT_ACK_P2:
            resp_comm = COMM_SendPacket (send_buff);

            if (resp_comm != resp_working)
            {
                if (resp_comm == resp_sended_ok)
                {
                    unsigned short remain_pulses = HARD_GetPulses(1);
                    HARD_SetPulses(1, remain_pulses - pulses);
                    pulses = 0;                    
                }
                else if ((resp_comm == resp_sended_nok) ||
                         (resp_comm == resp_timeout))
                {
                    pck_tx_error++;
                }

                main_state = MAIN_CHECK_P3;
            }
            break;

        case MAIN_CHECK_P3:
            pulses = HARD_GetPulses(2);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P4;
            
            break;

        case MAIN_SEND_P3:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()))
            {
                if (pulses > 999)
                    pulses = 999;

                sprintf(send_buff, "pulses %03d ch3", pulses);
                if (COMM_SendPacket (send_buff) == resp_working)
                    main_state++;
                else
                {
                    pck_tx_error++;                
                    main_state = MAIN_CHECK_P4;
                }
            }
            break;

        case MAIN_WAIT_ACK_P3:
            resp_comm = COMM_SendPacket (send_buff);

            if (resp_comm != resp_working)
            {
                if (resp_comm == resp_sended_ok)
                {
                    unsigned short remain_pulses = HARD_GetPulses(2);
                    HARD_SetPulses(2, remain_pulses - pulses);
                    pulses = 0;                    
                }
                else if ((resp_comm == resp_sended_nok) ||
                         (resp_comm == resp_timeout))
                {
                    pck_tx_error++;
                }

                main_state = MAIN_CHECK_P4;
            }
            break;

        case MAIN_CHECK_P4:
            pulses = HARD_GetPulses(3);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P1;
            
            break;
            
        case MAIN_SEND_P4:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()))
            {
                if (pulses > 999)
                    pulses = 999;

                sprintf(send_buff, "pulses %03d ch4", pulses);
                if (COMM_SendPacket (send_buff) == resp_working)
                    main_state++;
                else
                {
                    pck_tx_error++;                
                    main_state = MAIN_CHECK_P1;
                }
            }
            break;

        case MAIN_WAIT_ACK_P4:
            resp_comm = COMM_SendPacket (send_buff);

            if (resp_comm != resp_working)
            {
                if (resp_comm == resp_sended_ok)
                {
                    unsigned short remain_pulses = HARD_GetPulses(3);
                    HARD_SetPulses(3, remain_pulses - pulses);
                    pulses = 0;                    
                }
                else if ((resp_comm == resp_sended_nok) ||
                         (resp_comm == resp_timeout))
                {
                    pck_tx_error++;
                }

                main_state = MAIN_CHECK_P1;
            }
            break;

        default:
            main_state = MAIN_CHECK_P1;
            break;
        }

        // concurrent things
        if (!timer_standby)
        {
            timer_standby = 1;
            HARD_Timeouts();
            COMM_Timeouts();
            LF_Timeouts();
        }

        HARD_UpdatePulsesFilters();
        COMM_Manager_SM();

        // check for link down
        if (pck_tx_error > 5)
        {
            pck_tx_error = 0;
            COMM_Manager_Reset_SM ();
            timer_standby = 1000;
        }

        // pulse indication led its on absolute value
        if (HARD_GetPulsesValue(0))
            Led_Pulse1_On();
        else
            Led_Pulse1_Off();

        if (HARD_GetPulsesValue(1))
            Led_Pulse2_On();
        else
            Led_Pulse2_Off();

        if (HARD_GetPulsesValue(2))
            Led_Pulse3_On();
        else
            Led_Pulse3_Off();

        if (HARD_GetPulsesValue(3))
            Led_Pulse4_On();
        else
            Led_Pulse4_Off();
        
        

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

