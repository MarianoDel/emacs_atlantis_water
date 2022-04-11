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


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------
void Led_Link_Show_Status (int status);
unsigned char Led_Link_Is_On (void);


// Module Functions for testing ------------------------------------------------
void Test_Led_Link_Set_Reset_Toggle (void);
void Test_Led_Link_Pulse (void);



// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{
    Test_Led_Link_Set_Reset_Toggle ();
    Test_Led_Link_Pulse ();
    
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

//--- end of file ---//


