//----------------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.C ##############################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "pwm.h"
#include "hard.h"
#include "tim.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void PWMChannelsReset (void)
{
    PWM_Update_CH1(DUTY_NONE);
    PWM_Update_CH2(DUTY_NONE);
#ifdef HARDWARE_VERSION_1_1
    ENA_CH1_OFF;
    ENA_CH2_OFF;
#endif
}


unsigned short PWM_Map_From_Dmx (unsigned char dmx_val)
{
    unsigned int pwm = 0;

#if (DUTY_100_PERCENT == 1000)    
    if (dmx_val)
    {
        pwm = dmx_val * 391;
        pwm = pwm / 100;
        pwm += 4;
    }
#endif
#if (DUTY_100_PERCENT == 4000)
    if (dmx_val)
    {
        pwm = dmx_val * 157;
        pwm = pwm / 10;
        pwm += 13;
    }
#endif

    return (unsigned short) pwm;

}


// void PWM_Set_PwrCtrl (unsigned char * ch_dmx_val, unsigned char chnls_qtty, unsigned short max_power)
// {
//     unsigned short total_dmx = 0;

//     //cuantos en total
//     for (unsigned char i = 0; i < chnls_qtty; i++)
//         total_dmx += *(ch_dmx_val + i);

//     if (total_dmx > max_power)
//     {
//         unsigned int new = 0;
//         for (unsigned char i = 0; i < chnls_qtty; i++)
//         {
//             // si el canal tiene algo
//             if (*(ch_dmx_val + i))
//             {
//                 new = *(ch_dmx_val + i) * max_power;
//                 new = new / total_dmx;

//                 // no dejo que se apaguen los canales
//                 if (new)
//                     *(ch_dmx_val + i) = (unsigned char) new;
//                 else
//                     *(ch_dmx_val + i) = 1;
                
//             }
//         }
//     }
// }


// get dmx_data from 0 to 255
// answer pwm_ena 0 to 4096
// answer pwm_ch 0 to 4096
void PWM_Map_Pre_Filter (unsigned char dmx_data, unsigned short * pwm_ena, unsigned short * pwm_ch)
{
    unsigned char dmx_ena = 0;
    unsigned short dmx_ch = 0;
    
    if (dmx_data > 4)
    {
        dmx_ena = 4;
        dmx_ch = dmx_data - 4;
    }
    else
    {
        dmx_ena = dmx_data;
        dmx_ch = 0;
    }

    // *pwm_ena = dmx_ena * 1024;
    *pwm_ena = dmx_ena << 10;
    
    dmx_ch = dmx_ch * 164;
    dmx_ch = dmx_ch / 10;
    *pwm_ch = dmx_ch;
    
}


// get dmx_filtered from 0 to 4095
// answer pwm_ena 0 to 4096
// answer pwm_ch 0 to 4096
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch)
{
    unsigned short dmx_ena = 0;
    unsigned int dmx_ch = 0;
    
    if (dmx_filtered > 64)
    {
        dmx_ena = 64;
        dmx_ch = dmx_filtered - 64;
    }
    else
    {
        dmx_ena = dmx_filtered;
        dmx_ch = 0;
    }

    // *pwm_ena = dmx_ena * 64;
    *pwm_ena = dmx_ena << 6;
    
    dmx_ch = dmx_ch * 1016;
    dmx_ch = dmx_ch / 1000;
    *pwm_ch = (unsigned short) dmx_ch;
    
}


#define DEFAULT_SOFT_PWM    4095
#define SOFT_PWM_STEPS    256
volatile unsigned char edges = 0;
volatile unsigned short soft_pwm_ch1 = 0;
volatile unsigned short soft_pwm_ch2 = 0;
volatile unsigned short soft_saved_pwm_ch1 = 0;
volatile unsigned short soft_saved_pwm_ch2 = 0;
volatile unsigned short soft_pwm_cnt = 0;
volatile unsigned short soft_pwm_next = 0;

volatile unsigned char soft_pwm_output_ch1 = 0;
volatile unsigned char soft_pwm_output_ch2 = 0;

volatile unsigned short first_falling_timer = 0;
volatile unsigned short second_falling_timer = 0;
volatile unsigned char first_falling_flag = 0;
volatile unsigned char second_falling_flag = 0;
#define ch1_flag    0x01
#define ch2_flag    0x02

void PWM_Int_Handler_2_255 (void)
{
    switch (edges)
    {
    case 0:
        // starting edge set defaults
        soft_pwm_cnt = 255;

        first_falling_timer = 0;
        second_falling_timer = 0;

        first_falling_flag = 0;
        second_falling_flag = 0;

        // backup conf data for the cycle
        soft_pwm_ch1 = soft_saved_pwm_ch1;
        soft_pwm_ch2 = soft_saved_pwm_ch2;        
        
        if (soft_pwm_ch1 > 255)    // ch1 always on
        {
            if (!soft_pwm_output_ch1)    // but last time was in 0
                soft_pwm_output_ch1 = 1;
        }
        else if (soft_pwm_ch1 == 0)    // check for never
        {
            if (soft_pwm_output_ch1)
                soft_pwm_output_ch1 = 0;
        }
        else
        {
            soft_pwm_output_ch1 = 1;
            first_falling_timer = soft_pwm_ch1;
            soft_pwm_cnt = first_falling_timer;
            first_falling_flag |= ch1_flag;
            edges++;
        }

        if (soft_pwm_ch2 > 255)    // ch2 always on
        {
            if (!soft_pwm_output_ch2)    // but was 0 last time
                soft_pwm_output_ch2 = 1;
        }
        else if (soft_pwm_ch2 == 0)    // check for never
        {
            if (soft_pwm_output_ch2)
                soft_pwm_output_ch2 = 0;
        }
        else
        {
            if (edges)    // we will have at least two edges
            {
                if (soft_pwm_ch2 < soft_pwm_ch1)    // second edge comes first
                {
                    soft_pwm_output_ch2 = 1;
                    second_falling_timer = soft_pwm_ch2;
                    soft_pwm_cnt = second_falling_timer;
                    second_falling_flag |= ch2_flag;
                    edges++;
                }
                else if (soft_pwm_ch2 == soft_pwm_ch1)    // in the same edge
                {
                    soft_pwm_output_ch2 = 1;
                    first_falling_flag |= ch2_flag;
                }
                else    //ch2 edge comes after ch1, we need to compensate
                {
                    soft_pwm_output_ch2 = 1;
                    second_falling_timer = first_falling_timer;
                    second_falling_flag = first_falling_flag;
                    first_falling_timer = soft_pwm_ch2;
                    soft_pwm_cnt = second_falling_timer;
                    first_falling_flag |= ch2_flag;
                    edges++;
                }
            }
            else    // ch2 will have the first edge
            {
                soft_pwm_output_ch2 = 1;
                first_falling_timer = soft_pwm_ch2;
                soft_pwm_cnt = first_falling_timer;
                first_falling_flag |= ch2_flag;
                edges++;
            }
        }
        break;

    case 1:    // one edge or the last edge
        // wich channel to fall
        if (first_falling_flag & ch1_flag)
        {
            soft_pwm_output_ch1 = 0;
            first_falling_flag &= ~(ch1_flag);
        }

        if (first_falling_flag & ch2_flag)
        {
            soft_pwm_output_ch2 = 0;
            first_falling_flag &= ~(ch2_flag);
        }

        // next timer
        soft_pwm_cnt = 255 - first_falling_timer;
        edges--;
        break;

    case 2:    // two edges
        // wich channel to fall
        if (second_falling_flag & ch1_flag)
        {
            soft_pwm_output_ch1 = 0;
            second_falling_flag &= ~(ch1_flag);
        }

        if (second_falling_flag & ch2_flag)
        {
            soft_pwm_output_ch2 = 0;
            second_falling_flag &= ~(ch2_flag);
        }

        // next timer
        soft_pwm_cnt = first_falling_timer - second_falling_timer - 1;
        edges--;
        break;

    default:
        edges = 0;
        break;
    }    
}

#define CH1_NEVER    0x08
#define CH1_ALWAYS    0x04
#define CH1_DWN    0x02
#define CH1_UP    0x01

#define CH2_NEVER    0x80
#define CH2_ALWAYS    0x40
#define CH2_DWN    0x20
#define CH2_UP    0x10

volatile unsigned char edges_saved = 0;
volatile unsigned char vfunctions_saved[3];
volatile unsigned short vtime_saved[3];

// volatile unsigned char edges = 0;
volatile unsigned char vfunctions[3];
volatile unsigned short vtime[3];

volatile unsigned short * ptime;
volatile unsigned char * pfunc;

void PWM_Int_Handler_3_Start (void)
{
    ptime = vtime;
    pfunc = vfunctions;
}

void PWM_Int_Handler_3_Update (void)
{
    edges_saved = 0;

    vfunctions_saved[0] = 0;
    vfunctions_saved[1] = 0;
    vfunctions_saved[2] = 0;    

    vtime_saved[0] = 0;
    vtime_saved[1] = 0;
    vtime_saved[2] = 0;    
    
    if (soft_saved_pwm_ch1 > 255)
    {
        vfunctions_saved[0] |= CH1_ALWAYS;
    }
    else if (soft_saved_pwm_ch1 == 0)
    {
        vfunctions_saved[0] |= CH1_NEVER;
    }
    else
    {
        vtime_saved[0] = soft_saved_pwm_ch1 << 4;
        vtime_saved[1] = 4095 - vtime_saved[0];
        vfunctions_saved[0] |= CH1_UP;
        vfunctions_saved[1] |= CH1_DWN;
        edges_saved++;
    }

    if (soft_saved_pwm_ch2 > 255)
    {
        vfunctions_saved[0] |= CH2_ALWAYS;
    }
    else if (soft_saved_pwm_ch2 == 0)
    {
        vfunctions_saved[0] |= CH2_NEVER;
    }
    else
    {
        if (edges_saved)    // we will have at least one edge
        {
            if (soft_saved_pwm_ch2 > soft_saved_pwm_ch1)    //ch2 after ch1
            {
                vtime_saved[1] = (soft_saved_pwm_ch2 << 4) - vtime_saved[0];
                vtime_saved[2] = 4095 - vtime_saved[1] - vtime_saved[0];
                
                vfunctions_saved[0] |= CH2_UP;
                vfunctions_saved[2] |= CH2_DWN;
                edges_saved++;
            }
            else if (soft_saved_pwm_ch2 == soft_saved_pwm_ch1)    // in the same edge
            {
                vfunctions_saved[0] |= CH2_UP;
                vfunctions_saved[1] |= CH2_DWN;
            }
            else    //ch2 edge comes first, we need to compensate
            {
                vtime_saved[1] = vtime_saved[0];
                vtime_saved[0] = soft_saved_pwm_ch2 << 4;
                vtime_saved[2] = 4095 - vtime_saved[1];
                
                vfunctions_saved[0] |= CH2_UP;
                vfunctions_saved[2] = vfunctions_saved[1];
                vfunctions_saved[1] = CH2_DWN;
                edges_saved++;
            }
        }
        else    // ch2 will have the first edge
        {
            vfunctions_saved[0] |= CH2_UP;
            vfunctions_saved[1] |= CH2_DWN;
            vtime_saved[0] = soft_saved_pwm_ch2 << 4;
            vtime_saved[1] = 4095 - vtime_saved[0];
            edges_saved++;
        }
    }

    if (!edges_saved)
        vtime_saved[0] = 4095;
    
}

void PWM_Int_Handler_3 (void)
{
    if (ptime == vtime)
    {
        // save new values on roundup
        vtime [0] = vtime_saved [0];
        vtime [1] = vtime_saved [1];
        vtime [2] = vtime_saved [2];

        vfunctions [0] = vfunctions_saved [0];
        vfunctions [1] = vfunctions_saved [1];
        vfunctions [2] = vfunctions_saved [2];

        edges = edges_saved;
        
        ptime = vtime;
        pfunc = vfunctions;

        // functions on ch1
        if (vfunctions[0] & CH1_ALWAYS)
        {
            if (!soft_pwm_output_ch1)
                soft_pwm_output_ch1 = 1;
        }
        else if (vfunctions[0] & CH1_NEVER)
        {
            if (soft_pwm_output_ch1)
                soft_pwm_output_ch1 = 0;
        }
        else if (vfunctions[0] & CH1_UP)
        {
            soft_pwm_output_ch1 = 1;            
        }

        // functions on ch2
        if (vfunctions[0] & CH2_ALWAYS)
        {
            if (!soft_pwm_output_ch2)
                soft_pwm_output_ch2 = 1;
        }
        else if (vfunctions[0] & CH2_NEVER)
        {
            if (soft_pwm_output_ch2)
                soft_pwm_output_ch2 = 0;
        }
        else if (vfunctions[0] & CH2_UP)
        {
            soft_pwm_output_ch2 = 1;
        }

        soft_pwm_cnt = *ptime;
        ptime++;
        pfunc++;
    }
    else if (ptime < (vtime + edges))
    {
        if (*pfunc & CH1_DWN)
        {
            soft_pwm_output_ch1 = 0;
        }
        
        if (*pfunc & CH2_DWN)
        {
            soft_pwm_output_ch2 = 0;            
        }

        soft_pwm_cnt = *ptime;
        ptime++;
        pfunc++;
    }
}


void PWM_Int_Handler_2 (void)
{
    switch (edges)
    {
    case 0:
        // starting edge set defaults
        soft_pwm_cnt = DEFAULT_SOFT_PWM;

        first_falling_timer = 0;
        second_falling_timer = 0;

        first_falling_flag = 0;
        second_falling_flag = 0;

        // backup conf data for the cycle
        soft_pwm_ch1 = soft_saved_pwm_ch1;
        soft_pwm_ch2 = soft_saved_pwm_ch2;        
        
        if (soft_pwm_ch1 > 255)    // ch1 always on
        {
            if (!soft_pwm_output_ch1)    // but last time was in 0
                soft_pwm_output_ch1 = 1;
        }
        else if (soft_pwm_ch1 == 0)    // check for never
        {
            if (soft_pwm_output_ch1)
                soft_pwm_output_ch1 = 0;
        }
        else
        {
            soft_pwm_output_ch1 = 1;
            first_falling_timer = soft_pwm_ch1 << 4;
            soft_pwm_cnt = first_falling_timer - 1;
            first_falling_flag |= ch1_flag;
            edges++;
        }

        if (soft_pwm_ch2 > 255)    // ch2 always on
        {
            if (!soft_pwm_output_ch2)    // but was 0 last time
                soft_pwm_output_ch2 = 1;
        }
        else if (soft_pwm_ch2 == 0)    // check for never
        {
            if (soft_pwm_output_ch2)
                soft_pwm_output_ch2 = 0;
        }
        else
        {
            if (edges)    // we will have at least two edges
            {
                if (soft_pwm_ch2 < soft_pwm_ch1)    // second edge comes first
                {
                    soft_pwm_output_ch2 = 1;
                    second_falling_timer = soft_pwm_ch2 << 4;
                    soft_pwm_cnt = second_falling_timer - 1;
                    second_falling_flag |= ch2_flag;
                    edges++;
                }
                else if (soft_pwm_ch2 == soft_pwm_ch1)    // in the same edge
                {
                    soft_pwm_output_ch2 = 1;
                    first_falling_flag |= ch2_flag;
                }
                else    //ch2 edge comes after ch1, we need to compensate
                {
                    soft_pwm_output_ch2 = 1;
                    second_falling_timer = first_falling_timer;
                    second_falling_flag = first_falling_flag;
                    first_falling_timer = soft_pwm_ch2 << 4;
                    soft_pwm_cnt = second_falling_timer - 1;
                    first_falling_flag |= ch2_flag;
                    edges++;
                }
            }
            else    // ch2 will have the first edge
            {
                soft_pwm_output_ch2 = 1;
                first_falling_timer = soft_pwm_ch2 << 4;
                soft_pwm_cnt = first_falling_timer - 1;
                first_falling_flag |= ch2_flag;
                edges++;
            }
        }
        break;

    case 1:    // one edge or the last edge
        // wich channel to fall
        if (first_falling_flag & ch1_flag)
        {
            soft_pwm_output_ch1 = 0;
            first_falling_flag &= ~(ch1_flag);
        }

        if (first_falling_flag & ch2_flag)
        {
            soft_pwm_output_ch2 = 0;
            first_falling_flag &= ~(ch2_flag);
        }

        // next timer
        soft_pwm_cnt = DEFAULT_SOFT_PWM - first_falling_timer - 1;
        edges--;
        break;

    case 2:    // two edges
        // wich channel to fall
        if (second_falling_flag & ch1_flag)
        {
            soft_pwm_output_ch1 = 0;
            second_falling_flag &= ~(ch1_flag);
        }

        if (second_falling_flag & ch2_flag)
        {
            soft_pwm_output_ch2 = 0;
            second_falling_flag &= ~(ch2_flag);
        }

        // next timer
        soft_pwm_cnt = first_falling_timer - second_falling_timer - 1;
        edges--;
        break;

    default:
        edges = 0;
        break;
    }    
}


void PWM_Int_Handler (void)
{
    switch (edges)
    {
    case 0:
        // starting edge
        soft_pwm_next = 4095;    //default delta
        
        if (soft_saved_pwm_ch1)
        {
            soft_pwm_output_ch1 = 1;
            soft_pwm_ch1 = soft_saved_pwm_ch1 << 4;
            soft_pwm_next = soft_pwm_ch1;

            edges++;
        }

        if (soft_saved_pwm_ch2)
        {
            soft_pwm_output_ch2 = 1;
            soft_pwm_ch2 = soft_saved_pwm_ch2 << 4;
            
            if (soft_pwm_next > soft_pwm_ch2)
                soft_pwm_next = soft_pwm_ch2;

            edges++;
        }
        break;

    case 1:    // one edge
        // first falling edge
        if ((soft_pwm_output_ch1) &&
            (soft_pwm_ch1 <= soft_pwm_cnt))
        {
            soft_pwm_output_ch1 = 0;
            soft_pwm_next = 4095 - soft_pwm_ch1;
            edges--;            
        }

        if ((soft_pwm_output_ch2) &&
            (soft_pwm_ch2 <= soft_pwm_cnt))
        {
            soft_pwm_output_ch2 = 0;
            soft_pwm_next = 4095 - soft_pwm_ch2;            
            edges--;
        }
        break;

    case 2:    // two edges
        // second falling edge
        if ((soft_pwm_output_ch1) &&
            (soft_pwm_ch1 <= soft_pwm_cnt))
        {
            soft_pwm_output_ch1 = 0;
            soft_pwm_next = soft_pwm_ch2 - soft_pwm_ch1;
            edges--;            
        }

        if ((soft_pwm_output_ch2) &&
            (soft_pwm_ch2 <= soft_pwm_cnt))
        {
            soft_pwm_output_ch2 = 0;
            soft_pwm_next = soft_pwm_ch1 - soft_pwm_ch2;            
            edges--;
        }

        if (!edges)    //same time edges
        {
            soft_pwm_next = 4095 - soft_pwm_ch1;
        }
        break;

    default:
        edges = 0;
        break;
    }    
}


void PWM_Soft_Handler_Low_Freq (void)
{
    if (soft_pwm_cnt)
    {
        if (soft_saved_pwm_ch1 <= soft_pwm_cnt)        
            PWM_Soft_Reset_Output_Ch1 ();

        if (soft_saved_pwm_ch2 <= soft_pwm_cnt)
            PWM_Soft_Reset_Output_Ch2 ();
        
        if (soft_pwm_cnt < (SOFT_PWM_STEPS - 1))
            soft_pwm_cnt++;
        else
            soft_pwm_cnt = 0;
        
    }
    else
    {
        if (soft_saved_pwm_ch1)
            PWM_Soft_Set_Output_Ch1 ();

        if (soft_saved_pwm_ch2)
            PWM_Soft_Set_Output_Ch2 ();

        soft_pwm_cnt++;
    }
}


void PWM_Soft_Set_Output_Ch1 (void)
{
    // soft_pwm_output_ch1 = 1;
    TIM1->ARR = VALUE_FOR_LEAST_FREQ;
    TIM1->EGR |= TIM_EGR_UG;    
    LED_ON;    
}


void PWM_Soft_Reset_Output_Ch1 (void)
{
    // soft_pwm_output_ch1 = 0;
    TIM1->ARR = 0;
    LED_OFF;
}


void PWM_Soft_Set_Output_Ch2 (void)
{
    // soft_pwm_output_ch2 = 1;
    TIM3->ARR = VALUE_FOR_LEAST_FREQ;
    TIM3->EGR |= TIM_EGR_UG;
}


void PWM_Soft_Reset_Output_Ch2 (void)
{
    // soft_pwm_output_ch2 = 0;
    TIM3->ARR = 0;
}


void PWM_Soft_Set_Channels (unsigned char ch, unsigned short value)
{
    if (ch == 1)
        soft_saved_pwm_ch1 = value;

    if (ch == 2)
        soft_saved_pwm_ch2 = value;
}

//--- end of file ---//
