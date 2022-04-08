//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "pwm.h"


//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short soft_saved_pwm_ch1;
extern volatile unsigned short soft_saved_pwm_ch2;

extern volatile unsigned char soft_pwm_output_ch1;
extern volatile unsigned char soft_pwm_output_ch2;

extern volatile unsigned short soft_pwm_cnt;
extern volatile unsigned short soft_pwm_next;

extern volatile unsigned short soft_pwm_ch1;
extern volatile unsigned short soft_pwm_ch2;

extern volatile unsigned char edges;
// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Pwm_Set (void);
void Test_Pwm_Int_Handler (void);
void Test_Pwm_Int_Handler_2 (void);
void Test_Pwm_Int_Handler_2_255 (void);
void Test_Pwm_Int_Handler_simul (void);
void Test_Pwm_Int_Handler_2_simul (void);
void Test_Pwm_Soft_Handler_Low_Freq (void);
void Test_Pwm_Soft_Handler_Low_Freq_simul (void);

void Test_Pwm_Int_Handler_3 (void);

// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    // Test_Pwm_Set ();
    // Test_Pwm_Handler ();
    // Test_Pwm_Int_Handler_simul ();    
    // Test_Pwm_Soft_Handler_Low_Freq ();
    // Test_Pwm_Soft_Handler_Low_Freq_simul ();
    Test_Pwm_Int_Handler_2 ();
    // Test_Pwm_Int_Handler_3 ();    
    // Test_Pwm_Int_Handler_2_255 ();    
}


void Test_Pwm_Soft_Handler_Low_Freq (void)
{
    int error = 0;
    int posi = 0;
    int setting = 0;
    int value = 0;    
    
    printf("Test soft pwm handler no output: ");
    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);

    for (int i = 0; i < 512; i++)    //two turns
    {
        PWM_Soft_Handler_Low_Freq();
        if ((soft_pwm_output_ch1) ||
            (soft_pwm_output_ch2))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d\n", posi);
    }
    else
        PrintOK();


    printf("Test soft pwm handler output ch1: ");
    PWM_Soft_Set_Channels (2, 0);

    for (int j = 0; j <= 255; j++)
    {
        PWM_Soft_Set_Channels (1, j);

        for (int i = 0; i < 512; i++)    // two turns
        {
            PWM_Soft_Handler_Low_Freq();

            if (soft_pwm_cnt <= j)    // check one output
            {
                if ((!soft_pwm_output_ch1) &&
                    (soft_pwm_cnt))
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    value = 0;
                    j = 255;
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    value = 1;                    
                    j = 255;                    
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d value: %d\n", posi, setting, value);
    }
    else
        PrintOK();


    printf("Test soft pwm handler output ch2: ");
    PWM_Soft_Set_Channels (1, 0);

    for (int j = 0; j <= 255; j++)
    {
        PWM_Soft_Set_Channels (2, j);

        for (int i = 0; i < 512; i++)    // two turns
        {
            PWM_Soft_Handler_Low_Freq();

            if (soft_pwm_cnt <= j)    // check one output
            {
                if ((!soft_pwm_output_ch2) &&
                    (soft_pwm_cnt))
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    value = 0;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    value = 1;
                    j = 255;                    
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d value: %d\n", posi, setting, value);
    }
    else
        PrintOK();
    
}


#define SOFT_PWM_STEPS    256
void Test_Pwm_Soft_Handler_Low_Freq_simul (void)
{
    int setting = 255;
    int out_ch1_1 = 0;
    int out_ch1_0 = 0;
    int out_ch2_1 = 0;
    int out_ch2_0 = 0;
    
    PWM_Soft_Set_Channels (1, setting);
    for (int i = 0; i < (SOFT_PWM_STEPS * 3); i++)
    {
        PWM_Soft_Handler_Low_Freq();
        printf("loop: %03d set: %d cnt: %d output: %d\n",
               i,
               soft_saved_pwm_ch1,
               soft_pwm_cnt,
               soft_pwm_output_ch1);

        if (soft_pwm_output_ch1)
            out_ch1_1++;
        else
            out_ch1_0++;
        
    }

    printf("setting: %d out_ch1_1: %d out_ch1_0: %d\n",
           setting,
           out_ch1_1,
           out_ch1_0);    
    
    
}


void Test_Pwm_Int_Handler_simul (void)
{
    int error = 0;
    int posi = 0;    
    int setting = 0;
    int called = 0;
    int out_ch1_1 = 0;
    int out_ch1_0 = 0;
    int out_ch2_1 = 0;
    int out_ch2_0 = 0;
    
    printf("Test int pwm handler output ch1 in 0\n");
    PWM_Soft_Set_Channels (2, 0);
    PWM_Soft_Set_Channels (1, 0);

    PWM_Int_Handler();    //first setting call
    called = 0;
    for (int i = 0; i < 4096; i++)
    {
        soft_pwm_cnt = i;
            
        if (soft_pwm_cnt == soft_pwm_next)
        {
            PWM_Int_Handler();
            called++;
        }
        
        if (soft_pwm_output_ch1)
            out_ch1_1++;
        else
            out_ch1_0++;

    }
    
    printf("called: %d out_ch1_1: %d out_ch1_0: %d\n", called, out_ch1_1, out_ch1_0);

    

    int pwm_ch1_value = 254;
    int pwm_ch2_value = 254;    
    printf("Test int pwm handler output ch1: %d ch2: %d\n", pwm_ch1_value, pwm_ch2_value);    
    PWM_Soft_Set_Channels (2, pwm_ch2_value);
    PWM_Soft_Set_Channels (1, pwm_ch1_value);

    PWM_Int_Handler();    //first setting call
    setting = (pwm_ch1_value << 4);
    if (soft_pwm_ch1 != setting) 
        printf(" error on setting ch1 needs: %d get: %d\n", setting, soft_pwm_ch1);
    else
        printf(" pwm ch1: %d setting on: %d\n", pwm_ch1_value, setting);

    setting = (pwm_ch2_value << 4);    
    if (soft_pwm_ch2 != setting) 
        printf(" error on setting ch2 needs: %d get: %d\n", setting, soft_pwm_ch2);
    else
        printf(" pwm ch2: %d setting on: %d\n", pwm_ch2_value, setting);
    
    called = 0;
    out_ch1_1 = 0;
    out_ch1_0 = 0;
    out_ch2_1 = 0;
    out_ch2_0 = 0;
    int delta = 0;
    for (int i = 0; i < 4096; i++)
    {
        soft_pwm_cnt = i;
            
        if (soft_pwm_cnt == soft_pwm_next + delta)
        {
            PWM_Int_Handler();
            called++;
            printf("  call: %d counter: %d next: %d total: %d\n",
                   called,
                   i,
                   soft_pwm_next,
                   soft_pwm_next + i);
            delta = i;
        }
        
        if (soft_pwm_output_ch1)
            out_ch1_1++;
        else
            out_ch1_0++;

        if (soft_pwm_output_ch2)
            out_ch2_1++;
        else
            out_ch2_0++;
        
    }
    printf(" called: %d out_ch1_1: %d out_ch1_0: %d total: %d\n",
           called,
           out_ch1_1,
           out_ch1_0,
           out_ch1_1 + out_ch1_0);

    printf(" called: %d out_ch2_1: %d out_ch2_0: %d total: %d\n",
           called,
           out_ch2_1,
           out_ch2_0,
           out_ch2_1 + out_ch2_0);
    
}


void Test_Pwm_Int_Handler_2_255 (void)
{
    int error = 0;
    int posi = 0;
    int called = 0;
    int out_ch1_1 = 0;
    int out_ch1_0 = 0;
    int out_ch2_1 = 0;
    int out_ch2_0 = 0;
    
    printf("Test int pwm handler ch1 output\n");
    PWM_Soft_Set_Channels (1, 1);
    PWM_Soft_Set_Channels (2, 0);

    // if (soft_saved_pwm_ch1 != 1)
    //     printf(" error on setted\n");
    soft_pwm_cnt = 0;
    for (int i = 0; i < 255; i++)
    // for (int i = 0; i < (4095 * 2); i++)        
    {
        if (!soft_pwm_cnt)
        {
            PWM_Int_Handler_2_255();
            printf(" handler called on: %d counter: %d\n", i, soft_pwm_cnt);
            called++;
        }
        else
        {
            soft_pwm_cnt--;
        }
        
        if (soft_pwm_output_ch1)
            out_ch1_1++;
        else
            out_ch1_0++;

        if (soft_pwm_output_ch2)
            out_ch2_1++;
        else
            out_ch2_0++;
        
    }
    
    printf("  called: %d out_ch1_1: %d out_ch1_0: %d total: %d\n",
           called,
           out_ch1_1,
           out_ch1_0,
           out_ch1_1 + out_ch1_0);

    printf("  called: %d out_ch2_1: %d out_ch2_0: %d total: %d\n",
           called,
           out_ch2_1,
           out_ch2_0,
           out_ch2_1 + out_ch2_0);
    
    
}

void Test_Pwm_Int_Handler_3 (void)
{
    int error = 0;
    int posi = 0;
    int called = 0;
    int out_ch1_1 = 0;
    int out_ch1_0 = 0;
    int out_ch2_1 = 0;
    int out_ch2_0 = 0;
    
    
    printf("Test int pwm handler no output: ");
    PWM_Int_Handler_3_Start();
    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);
    PWM_Int_Handler_3_Update();

    for (int i = 0; i < 4095; i++)
    {
        PWM_Int_Handler_3();
        if ((soft_pwm_output_ch1) ||
            (soft_pwm_output_ch2))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();


    printf("Test int pwm handler ch1 output\n");
    PWM_Soft_Set_Channels (1, 1);
    PWM_Soft_Set_Channels (2, 0);
    PWM_Int_Handler_3_Update();

    // if (soft_saved_pwm_ch1 != 1)
    //     printf(" error on setted\n");
    soft_pwm_cnt = 0;
    // for (int i = 0; i < 4096; i++)
    for (int i = 0; i < 4095; i++)        
    // for (int i = 0; i < (4095 * 2); i++)        
    {
        if (!soft_pwm_cnt)
        {
            PWM_Int_Handler_3();
            printf(" handler called on: %d counter: %d\n", i, soft_pwm_cnt);
            called++;
        }
        else
        {
            soft_pwm_cnt--;
        }
        
        if (soft_pwm_output_ch1)
            out_ch1_1++;
        else
            out_ch1_0++;

        if (soft_pwm_output_ch2)
            out_ch2_1++;
        else
            out_ch2_0++;
        
    }
    
    printf("  called: %d out_ch1_1: %d out_ch1_0: %d total: %d\n",
           called,
           out_ch1_1,
           out_ch1_0,
           out_ch1_1 + out_ch1_0);

    printf("  called: %d out_ch2_1: %d out_ch2_0: %d total: %d\n",
           called,
           out_ch2_1,
           out_ch2_0,
           out_ch2_1 + out_ch2_0);
    
    
}


void Test_Pwm_Int_Handler_2 (void)
{
    int error = 0;
    int posi = 0;
    int called = 0;
    int out_ch1_1 = 0;
    int out_ch1_0 = 0;
    int out_ch2_1 = 0;
    int out_ch2_0 = 0;
    
    
    printf("Test int pwm handler no output: ");
    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);

    for (int i = 0; i < 4095; i++)
    {
        PWM_Int_Handler_2();
        if ((soft_pwm_output_ch1) ||
            (soft_pwm_output_ch2))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();


    printf("Test int pwm handler ch1 output\n");
    PWM_Soft_Set_Channels (1, 255);
    PWM_Soft_Set_Channels (2, 255);

    // if (soft_saved_pwm_ch1 != 1)
    //     printf(" error on setted\n");
    soft_pwm_cnt = 0;
    // for (int i = 0; i < 4096; i++)
    for (int i = 0; i < 4095; i++)        
    // for (int i = 0; i < (4095 * 2); i++)        
    {
        if (!soft_pwm_cnt)
        {
            PWM_Int_Handler_2();
            printf(" handler called on: %d counter: %d\n", i, soft_pwm_cnt);
            called++;
        }
        else
        {
            soft_pwm_cnt--;
        }
        
        if (soft_pwm_output_ch1)
            out_ch1_1++;
        else
            out_ch1_0++;

        if (soft_pwm_output_ch2)
            out_ch2_1++;
        else
            out_ch2_0++;
        
    }
    
    printf("  called: %d out_ch1_1: %d out_ch1_0: %d total: %d\n",
           called,
           out_ch1_1,
           out_ch1_0,
           out_ch1_1 + out_ch1_0);

    printf("  called: %d out_ch2_1: %d out_ch2_0: %d total: %d\n",
           called,
           out_ch2_1,
           out_ch2_0,
           out_ch2_1 + out_ch2_0);
    
    
}


void Test_Pwm_Int_Handler (void)
{
    int error = 0;
    int posi = 0;
    
    printf("Test int pwm handler no output: ");
    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);

    for (int i = 0; i < 0xffff; i++)
    {
        PWM_Int_Handler();
        if ((soft_pwm_output_ch1) ||
            (soft_pwm_output_ch2))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();


    int setting = 0;
    int called = 0;
    printf("Test int pwm handler output ch1: ");
    PWM_Soft_Set_Channels (2, 0);

    for (int j = 0; j < 255; j++)
    {
        PWM_Soft_Set_Channels (1, j);

        PWM_Int_Handler();    //first setting call
        called = 0;
        for (int i = 0; i < 4096; i++)
        {
            soft_pwm_cnt = i;

            if (soft_pwm_cnt == soft_pwm_next)
            {
                PWM_Int_Handler();
                called++;
            }

            if (soft_pwm_cnt < j)    // check one output
            {
                if (!soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
        }

        if ((j) && (called != 2))
        {
            if (j != 255)
                printf(" error in called needs 2 get: %d value: %d\n", called, j);
        }
        else if (called != 1)
            printf(" error in called needs 1 get: %d value: %d\n", called, j);
        
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d\n", posi, setting);
    }
    else
        PrintOK();


    printf("Test int pwm handler output ch2: ");
    PWM_Soft_Set_Channels (1, 0);

    for (int j = 0; j < 255; j++)
    {
        PWM_Soft_Set_Channels (2, j);

        soft_pwm_next = 0;
        edges = 0;
        for (int i = 0; i < 4095; i++)
        {
            soft_pwm_cnt = i;

            if (soft_pwm_cnt == soft_pwm_next)
                PWM_Int_Handler();

            if (soft_pwm_cnt < j)    // check one output
            {
                if (!soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d\n", posi, setting);
    }
    else
        PrintOK();
    
    
    
    
}


void Test_Pwm_Set (void)
{
    int error = 0;
    int posi = 0;

    printf("Test soft pwm settings on ch1: ");
    PWM_Soft_Set_Channels (2, 0);
    for (int i = 0; i < 65536; i++)
    {
        PWM_Soft_Set_Channels (1, i);
        if ((soft_saved_pwm_ch2 != 0) ||
            (soft_saved_pwm_ch1 != i))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();

    printf("Test soft pwm settings on ch2: ");
    PWM_Soft_Set_Channels (1, 0);
    for (int i = 0; i < 65536; i++)
    {
        PWM_Soft_Set_Channels (2, i);
        if ((soft_saved_pwm_ch1 != 0) ||
            (soft_saved_pwm_ch2 != i))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();
    
}



// Module Mocked Functions
void Update_TIM3_CH1 (unsigned short a)
{
    printf("tim3 ch1 set to: %d\n", a);
}


void Update_TIM3_CH2 (unsigned short a)
{
    printf("tim3 ch2 set to: %d\n", a);
}



//--- end of file ---//


