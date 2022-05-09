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
volatile unsigned short timer_concurrent = 0;
volatile unsigned short timer_led_show_error = 0;

// //-- for the filters and outputs
// ma32_u16_data_obj_t pote_1_filter;
// ma32_u16_data_obj_t pote_2_filter;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);

void LedChannelShowStates (void);
void LedChannelShowInnerStates (unsigned char ch, unsigned char global_state);
void LedChannelShowPulses (void);

unsigned char ADC_LineErrors (unsigned char);


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
    COMM_Manager_WaitToStart_SM (0);    //starts inmediately

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
            pulses = HARD_GetPulses(CH1_OFFSET);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P2;
            
            break;
            
        case MAIN_SEND_P1:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()) &&
                (COMM_TimeWindow(CH1_OFFSET)))
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
                    unsigned short remain_pulses = HARD_GetPulses(CH1_OFFSET);
                    HARD_SetPulses(CH1_OFFSET, remain_pulses - pulses);
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
            pulses = HARD_GetPulses(CH2_OFFSET);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P3;

            break;

        case MAIN_SEND_P2:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()) &&
                (COMM_TimeWindow(CH2_OFFSET)))
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
                    unsigned short remain_pulses = HARD_GetPulses(CH2_OFFSET);
                    HARD_SetPulses(CH2_OFFSET, remain_pulses - pulses);
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
            pulses = HARD_GetPulses(CH3_OFFSET);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P4;
            
            break;

        case MAIN_SEND_P3:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()) &&
                (COMM_TimeWindow(CH3_OFFSET)))
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
                    unsigned short remain_pulses = HARD_GetPulses(CH3_OFFSET);
                    HARD_SetPulses(CH3_OFFSET, remain_pulses - pulses);
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
            pulses = HARD_GetPulses(CH4_OFFSET);
            if (pulses)
                main_state++;
            else
                main_state = MAIN_CHECK_P1;
            
            break;
            
        case MAIN_SEND_P4:
            if ((COMM_Manager_In_Link()) &&
                (COMM_ReadyToSend()) &&
                (COMM_TimeWindow(CH4_OFFSET)))
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
                    unsigned short remain_pulses = HARD_GetPulses(CH4_OFFSET);
                    HARD_SetPulses(CH4_OFFSET, remain_pulses - pulses);
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
        if (!timer_concurrent)
        {
            timer_concurrent = 1;
            HARD_Timeouts();
            COMM_Timeouts();
            LF_Timeouts();
        }

        HARD_UpdatePulsesFilters();
        COMM_Manager_SM();

        // check for link down or not packet delivered
        if (pck_tx_error > 5)
        {
            pck_tx_error = 0;
            COMM_Manager_Reset_SM ();
            COMM_Manager_WaitToStart_SM (6000);
            COMM_SendOKReset();    // not answer to keepalives during this time            
            main_state = MAIN_WAIT_TO_START;
            timer_standby = 6000;
        }

        // pulse indication led its on absolute value
        // LedChannelShowPulses ();
        // for 5s in between 20s show line ch state
        LedChannelShowStates();

    }    //end of while 1

    return 0;
}

//--- End of Main ---//

#define LED_GLOBAL_RUNNING    0
#define LED_GLOBAL_CHANGE_ON_ERROR    1
#define LED_GLOBAL_CHANGE_TO_PULSES    2

#define LED_SHOW_PULSES    0
#define LED_SHOW_LINE_ERRORS    1
unsigned char led_show_error = LED_SHOW_PULSES;
void LedChannelShowStates (void)
{
    unsigned char global_state = LED_GLOBAL_RUNNING;
    
    switch (led_show_error)
    {
    case LED_SHOW_PULSES:
        if (!timer_led_show_error)
        {
            global_state = LED_GLOBAL_CHANGE_ON_ERROR;
            timer_led_show_error = 7000;    // one second more than show error routine
            led_show_error = LED_SHOW_LINE_ERRORS;
        }
        
        LedChannelShowInnerStates(CH1_OFFSET, global_state);
        LedChannelShowInnerStates(CH2_OFFSET, global_state);
        LedChannelShowInnerStates(CH3_OFFSET, global_state);
        LedChannelShowInnerStates(CH4_OFFSET, global_state);

        if (global_state == LED_GLOBAL_CHANGE_ON_ERROR)
        {
            LF_Leds_Pulse_Toggle_Activate();
            global_state = LED_GLOBAL_RUNNING;
        }
        break;

    case LED_SHOW_LINE_ERRORS:

        if (!timer_led_show_error)
        {
            global_state = LED_GLOBAL_CHANGE_TO_PULSES;
            timer_led_show_error = 13000;    // total 20 secs
            led_show_error = LED_SHOW_PULSES;
        }
        
        LedChannelShowInnerStates(CH1_OFFSET, global_state);
        LedChannelShowInnerStates(CH2_OFFSET, global_state);
        LedChannelShowInnerStates(CH3_OFFSET, global_state);
        LedChannelShowInnerStates(CH4_OFFSET, global_state);

        if (global_state == LED_GLOBAL_CHANGE_TO_PULSES)
            global_state = LED_GLOBAL_RUNNING;
        
        break;

    default:
        led_show_error = LED_SHOW_PULSES;
        break;
        
    }
}


unsigned char led_ch_inner_state [4] = { 0 };
void LedChannelShowInnerStates (unsigned char ch, unsigned char global_state)
{
    unsigned char * inner_state;

    inner_state = &led_ch_inner_state[ch];
    
    switch (*inner_state)
    {
    case LED_SHOW_PULSES:
        if (HARD_GetPulsesValue(ch))
            Led_Pulse_On(ch);
        else
            Led_Pulse_Off(ch);

        if (global_state == LED_GLOBAL_CHANGE_ON_ERROR)            
        {
            if (ADC_LineErrors(ch))
            {
                Led_Pulse_Off(ch);
                LF_Leds_Pulse_Toggle_Set((1 << ch));
                *inner_state = LED_SHOW_LINE_ERRORS;
            }
            else
                LF_Leds_Pulse_Toggle_Reset((1 << ch));
        }
        break;

    case LED_SHOW_LINE_ERRORS:
        // do nothing here, just wait go back to pulses
        if (global_state == LED_GLOBAL_CHANGE_TO_PULSES)            
            *inner_state = LED_SHOW_PULSES;
            
        break;

    default:
        *inner_state = LED_SHOW_PULSES;
        break;
        
    }
}


unsigned char ADC_LineErrors (unsigned char ch)
{
    // return errors on ch1 and ch2
    if ((ch == CH1_OFFSET) || (ch == CH2_OFFSET))
        return 1;

    return 0;
}


void LedChannelShowPulses (void)
{
    if (HARD_GetPulsesValue(CH1_OFFSET))
        Led_Pulse_On(CH1_OFFSET);
    else
        Led_Pulse_Off(CH1_OFFSET);

    if (HARD_GetPulsesValue(CH2_OFFSET))
        Led_Pulse_On(CH2_OFFSET);
    else
        Led_Pulse_Off(CH2_OFFSET);

    if (HARD_GetPulsesValue(CH3_OFFSET))
        Led_Pulse_On(CH3_OFFSET);
    else
        Led_Pulse_Off(CH3_OFFSET);

    if (HARD_GetPulsesValue(CH4_OFFSET))
        Led_Pulse_On(CH4_OFFSET);
    else
        Led_Pulse_Off(CH4_OFFSET);
}


void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    if (timer_concurrent)
        timer_concurrent--;

    if (timer_led_show_error)
        timer_led_show_error--;
    
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

