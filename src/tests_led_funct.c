//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "led_functions.h"


//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned char led_pulse_active;
extern volatile unsigned char led_pulse_cntr;
extern volatile unsigned char leds_to_toggle;


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------
void Led_Link_Show_Status (int status);
unsigned char Led_Link_Is_On (void);

void Led_Pulse1_On (void);
void Led_Pulse1_Off (void);
unsigned char Led_Pulse1_Is_On (void);

void Led_Pulse2_On (void);
void Led_Pulse2_Off (void);
unsigned char Led_Pulse2_Is_On (void);

void Led_Pulse3_On (void);
void Led_Pulse3_Off (void);
unsigned char Led_Pulse3_Is_On (void);

void Led_Pulse4_On (void);
void Led_Pulse4_Off (void);
unsigned char Led_Pulse4_Is_On (void);

    

// Module Functions for testing ------------------------------------------------
void Test_Led_Link_Set_Reset_Toggle (void);
void Test_Led_Link_Pulse (void);

void Test_Led_Pulse_Activation (void);
void Test_Led_Pulse_Toggle_Set_Reset (void);

void Test_Led_Pulse1 (void);
void Test_Led_Pulse2 (void);
void Test_Led_Pulse3 (void);
void Test_Led_Pulse4 (void);



// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{
    Test_Led_Link_Set_Reset_Toggle ();

    Test_Led_Link_Pulse ();

    Test_Led_Pulse_Activation ();

    Test_Led_Pulse_Toggle_Set_Reset ();    
    
    Test_Led_Pulse1 ();
    Test_Led_Pulse2 (); 
    Test_Led_Pulse3 ();
    Test_Led_Pulse4 ();
    
}


void Test_Led_Link_Set_Reset_Toggle (void)
{
    printf("Testing led link set: ");
    LF_Link_Set();
    if (Led_Link_Is_On())
        PrintOK();
    else
        PrintERR();

    printf("Testing led link reset: ");
    LF_Link_Reset();
    if (Led_Link_Is_On())
        PrintERR();
    else
        PrintOK();

    int error = 0;
    printf("Testing led link toggle: ");
    LF_Link_Reset();
    LF_Link_Toggle();
    if (!Led_Link_Is_On())
    {
        PrintERR();
        printf("error on rising edge\n");
        error = 1;
    }

    LF_Link_Set();
    LF_Link_Toggle();
    if (Led_Link_Is_On())
    {
        PrintERR();
        printf("error on rising edge\n");
        error = 1;
    }

    if (!error)
        PrintOK();
    
}


void Test_Led_Link_Pulse (void)
{
    int error = 0;

    printf("Testing led link pulse: ");
    LF_Link_Reset();
    LF_Link_Pulse();
    if (!Led_Link_Is_On())
    {
        PrintERR();
        printf("error on rising edge\n");
        error = 1;
    }

    for (int i = 0; i < 151; i++)
        LF_Timeouts();

    if (Led_Link_Is_On())
    {
        PrintERR();
        printf("error on rising timeouts\n");
        error = 1;
    }

    LF_Link_Set();
    LF_Link_Pulse();
    if (Led_Link_Is_On())
    {
        PrintERR();
        printf("error on falling edge\n");
        error = 1;
    }

    for (int i = 0; i < 151; i++)
        LF_Timeouts();

    if (!Led_Link_Is_On())
    {
        PrintERR();
        printf("error on falling timeouts\n");
        error = 1;
    }

    if (!error)
        PrintOK();

}


void Test_Led_Pulse_Activation (void)
{
    printf("Testing led activation: ");

    if (!led_pulse_active)
        LF_Leds_Pulse_Toggle_Activate();

    if ((!led_pulse_active) || (!led_pulse_cntr))
    {
        PrintERR();
        printf("led activation error on set\n");
        return;
    }
    
    for (int i = 0; i < 6001; i++)
        LF_Timeouts();

    if ((led_pulse_active) || (led_pulse_cntr))
    {
        PrintERR();
        printf("led activation error on reset\n");
    }
    else
        PrintOK();
    
}


void Test_Led_Pulse_Toggle_Set_Reset (void)
{
    printf("Testing led pulse1 set: ");
    LF_Leds_Pulse_Toggle_Set(L1_TOGGLE);
    if (!(leds_to_toggle & L1_TOGGLE))
    {
        PrintERR();
        printf("led pulse1 toggle error on set\n");
        return;        
    }

    LF_Leds_Pulse_Toggle_Reset(L1_TOGGLE);
    if (leds_to_toggle & L1_TOGGLE)
    {
        PrintERR();
        printf("led pulse1 toggle error on reset\n");
        return;        
    }
    PrintOK();

    printf("Testing led pulse2 set: ");
    LF_Leds_Pulse_Toggle_Set(L2_TOGGLE);
    if (!(leds_to_toggle & L2_TOGGLE))
    {
        PrintERR();
        printf("led pulse2 toggle error on set\n");
        return;        
    }

    LF_Leds_Pulse_Toggle_Reset(L2_TOGGLE);
    if (leds_to_toggle & L2_TOGGLE)
    {
        PrintERR();
        printf("led pulse2 toggle error on reset\n");
        return;        
    }
    PrintOK();    

    printf("Testing led pulse3 set: ");
    LF_Leds_Pulse_Toggle_Set(L3_TOGGLE);
    if (!(leds_to_toggle & L3_TOGGLE))
    {
        PrintERR();
        printf("led pulse3 toggle error on set\n");
        return;        
    }

    LF_Leds_Pulse_Toggle_Reset(L3_TOGGLE);
    if (leds_to_toggle & L3_TOGGLE)
    {
        PrintERR();
        printf("led pulse3 toggle error on reset\n");
        return;        
    }
    PrintOK();    

    printf("Testing led pulse4 set: ");
    LF_Leds_Pulse_Toggle_Set(L4_TOGGLE);
    if (!(leds_to_toggle & L4_TOGGLE))
    {
        PrintERR();
        printf("led pulse4 toggle error on set\n");
        return;        
    }

    LF_Leds_Pulse_Toggle_Reset(L4_TOGGLE);
    if (leds_to_toggle & L4_TOGGLE)
    {
        PrintERR();
        printf("led pulse4 toggle error on reset\n");
        return;        
    }
    PrintOK();    
    
}


void Test_Led_Pulse1 (void)
{
    int error = 0;

    printf("Testing led pulse1: ");
    LF_Leds_Pulse_Toggle_Set (L1_TOGGLE);
    LF_Leds_Pulse_Toggle_Activate();
    for (int i = 0; i < 75; i++)
        LF_Timeouts();

    if (!Led_Pulse1_Is_On())
    {
        PrintERR();
        printf("error on toggling set timeouts\n");
        error = 1;
    }

    for (int i = 76; i < 6002; i++)
        LF_Timeouts();

    if (Led_Pulse1_Is_On())
    {
        PrintERR();
        printf("error on toggling reset timeouts\n");
        error = 1;
    }

    if (!error)
        PrintOK();

}


void Test_Led_Pulse2 (void)
{
    int error = 0;

    printf("Testing led pulse2: ");
    LF_Leds_Pulse_Toggle_Set (L2_TOGGLE);
    LF_Leds_Pulse_Toggle_Activate();
    for (int i = 0; i < 75; i++)
        LF_Timeouts();

    if (!Led_Pulse2_Is_On())
    {
        PrintERR();
        printf("error on toggling set timeouts\n");
        error = 1;
    }

    for (int i = 76; i < 6002; i++)
        LF_Timeouts();

    if (Led_Pulse2_Is_On())
    {
        PrintERR();
        printf("error on toggling reset timeouts\n");
        error = 1;
    }

    if (!error)
        PrintOK();

}


void Test_Led_Pulse3 (void)
{
    int error = 0;

    printf("Testing led pulse3: ");
    LF_Leds_Pulse_Toggle_Set (L3_TOGGLE);
    LF_Leds_Pulse_Toggle_Activate();
    for (int i = 0; i < 75; i++)
        LF_Timeouts();

    if (!Led_Pulse3_Is_On())
    {
        PrintERR();
        printf("error on toggling set timeouts\n");
        error = 1;
    }

    for (int i = 76; i < 6002; i++)
        LF_Timeouts();

    if (Led_Pulse3_Is_On())
    {
        PrintERR();
        printf("error on toggling reset timeouts\n");
        error = 1;
    }

    if (!error)
        PrintOK();

}


void Test_Led_Pulse4 (void)
{
    int error = 0;

    printf("Testing led pulse4: ");
    LF_Leds_Pulse_Toggle_Set (L4_TOGGLE);
    LF_Leds_Pulse_Toggle_Activate();
    for (int i = 0; i < 75; i++)
        LF_Timeouts();

    if (!Led_Pulse4_Is_On())
    {
        PrintERR();
        printf("error on toggling set timeouts\n");
        error = 1;
    }

    for (int i = 76; i < 6002; i++)
        LF_Timeouts();

    if (Led_Pulse4_Is_On())
    {
        PrintERR();
        printf("error on toggling reset timeouts\n");
        error = 1;
    }

    if (!error)
        PrintOK();

}



// Module Mocked Functions
unsigned char mocked_led_link = 0;
int led_link_show_status = 0;
void Led_Link_Show_Status (int status)
{
    led_link_show_status = status;
}


void Led_Link_On (void)
{
    mocked_led_link = 1;
    if (led_link_show_status)
        printf("led_link is on: %d\n", mocked_led_link);
}


void Led_Link_Off (void)
{
    mocked_led_link = 0;
    if (led_link_show_status)
        printf("led_link is off: %d\n", mocked_led_link);
}


unsigned char Led_Link_Is_On (void)
{
    if (led_link_show_status)
    {
        if (mocked_led_link)
            printf("led_link is on: %d\n", mocked_led_link);
        else
            printf("led_link is off: %d\n", mocked_led_link);
    }

    return mocked_led_link;
}    


int mocked_led_pulse1 = 0;
int mocked_led_pulse2 = 0;
int mocked_led_pulse3 = 0;
int mocked_led_pulse4 = 0;
void Led_Pulse1_On (void)
{
    mocked_led_pulse1 = 1;
}


void Led_Pulse1_Off (void)
{
    mocked_led_pulse1 = 0;
}


unsigned char Led_Pulse1_Is_On (void)
{
    return mocked_led_pulse1;
}


void Led_Pulse2_On (void)
{
    mocked_led_pulse2 = 2;
}


void Led_Pulse2_Off (void)
{
    mocked_led_pulse2 = 0;
}


unsigned char Led_Pulse2_Is_On (void)
{
    return mocked_led_pulse2;
}


void Led_Pulse3_On (void)
{
    mocked_led_pulse3 = 3;
}


void Led_Pulse3_Off (void)
{
    mocked_led_pulse3 = 0;
}


unsigned char Led_Pulse3_Is_On (void)
{
    return mocked_led_pulse3;
}


void Led_Pulse4_On (void)
{
    mocked_led_pulse4 = 4;
}


void Led_Pulse4_Off (void)
{
    mocked_led_pulse4 = 0;
}


unsigned char Led_Pulse4_Is_On (void)
{
    return mocked_led_pulse4;
}

//--- end of file ---//


