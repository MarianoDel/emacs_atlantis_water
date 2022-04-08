//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------


#include <stdio.h>
// Module Types Constants and Macros -------------------------------------------
// Switches actions
typedef enum {
    selection_none = 0,
    selection_up,
    selection_dwn,
    selection_enter,
    selection_back

} sw_actions_t;

typedef enum {
    SW_NO = 0,
    SW_MIN,
    SW_HALF,
    SW_FULL
    
} resp_sw_t;


#define SWITCHES_TIMER_RELOAD    5
#define SWITCHES_THRESHOLD_FULL	1000    //5 segundos
#define SWITCHES_THRESHOLD_HALF	50    //250 ms
#define SWITCHES_THRESHOLD_MIN	10    //50 ms

// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
unsigned short sw_sel_cntr = 0;


// Testing Functions -----------------------------------------------------------
void Test_Switch_Select (void);
void Test_Dmx_to_Percent (void);
void Test_Dmx_to_Temperature (void);


// Module Functions to Test ----------------------------------------------------
resp_sw_t Check_SW_SEL (void);
void DMX_to_Percent (unsigned char dmx, unsigned char * int_perc, unsigned char * dec_perc);
void ColorTemp (unsigned char dmx_value, unsigned short * temp);


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    Test_Switch_Select ();
    // Test_Dmx_to_Percent ();
    // Test_Dmx_to_Temperature ();
    // printf("quiero un %% en la salida\n");

    return 0;    
    
}


// Embedded Functions to test ----
#define SWITCHES_PULSES_FULL    60    //3 segs
#define SWITCHES_PULSES_HALF    20    //1 seg
unsigned char sw_sel_pulses_cntr = 0;
resp_sw_t Check_SW_SEL (void)
{
    resp_sw_t sw = SW_NO;
    unsigned char current_pulses = 0;

    // check how many pulses on counter or reset pulses counter
    if (sw_sel_cntr)
    {
        for (unsigned char i = 0; i < SWITCHES_PULSES_FULL; i++)
        {
            if (sw_sel_cntr > (SWITCHES_THRESHOLD_MIN * i))
                current_pulses++;
            else
                i = SWITCHES_PULSES_FULL;
        }

        if (current_pulses > (sw_sel_pulses_cntr + 1))
        {
            sw_sel_pulses_cntr++;

            if (sw_sel_pulses_cntr > SWITCHES_PULSES_FULL)
                sw = SW_FULL;
            else if (sw_sel_pulses_cntr > SWITCHES_PULSES_HALF)
                sw = SW_HALF;
            else
                sw = SW_MIN;
        }
    }
    else
    {
        sw_sel_pulses_cntr = 0;
    }
    
    return sw;    
}


void ColorTemp (unsigned char dmx_value, unsigned short * temp)
{
    unsigned int calc = 0;

    calc = dmx_value * 3800;
    calc = calc / 255;
    calc = calc + 2700;
    *temp = (unsigned short) calc;
}


void DMX_to_Percent (unsigned char dmx, unsigned char * int_perc, unsigned char * dec_perc)
{
    unsigned short calc = 0;
    unsigned short remain = 0;            

    calc = dmx * 100;
    calc = calc / 255;
    *int_perc = (unsigned char) calc;
    calc = calc * 255;
    calc = calc / 100;
    remain = dmx - calc;
    if (remain > 2)
        remain = 7;
    else if (remain > 1)
        remain = 5;
    else if (remain)
        remain = 2;
    else
        remain = 0;
    
    *dec_perc = (unsigned char) remain;
}


void Test_Switch_Select (void)
{
    resp_sw_t sw = SW_NO;
    int t = 0;

    printf("going up\n");
    for (int i = 0; i < 220; i++)
    {
        sw_sel_cntr = i;
        sw = Check_SW_SEL ();
        t += SWITCHES_TIMER_RELOAD;
        printf("t: %d cntr: %d sw: %d\n", t, sw_sel_cntr, sw);
    }
    
    printf("\ngoing down\n");
    
    for (int i = 221; i >= 0; i--)
    {
        sw_sel_cntr = i;
        sw = Check_SW_SEL ();
        t += SWITCHES_TIMER_RELOAD;
        printf("t: %d cntr: %d sw: %d\n", t, sw_sel_cntr, sw);
    }
}


void Test_Dmx_to_Percent (void)
{
    unsigned char dmx_data = 0;
    unsigned char dmx_integer = 0;
    unsigned char dmx_decimal = 0;
    float dmx_float = 0.0;

    // dmx_data = 145;
    // printf("Testing for dmx %d ", dmx_data);
    // DMX_to_Percent (dmx_data, &dmx_integer, &dmx_decimal);
    // dmx_float = dmx_data * 100.0 / 255.0;
    // printf("-> %03d.%01d real: %03.1f\n", dmx_integer, dmx_decimal, dmx_float);

    for (int i = 0; i < 256; i++)
    {
        dmx_data = (unsigned char) i;
        printf("Testing for dmx %d ", dmx_data);
        DMX_to_Percent (dmx_data, &dmx_integer, &dmx_decimal);
        dmx_float = dmx_data * 100.0 / 255.0;
        printf("-> %03d.%01d real: %03.1f\n", dmx_integer, dmx_decimal, dmx_float);
    }
    
}


void Test_Dmx_to_Temperature (void)
{
    unsigned char dmx_data = 0;
    unsigned short dmx_temp = 0;

    // dmx_data = 145;
    // printf("Testing for dmx %d ", dmx_data);
    // DMX_to_Percent (dmx_data, &dmx_integer, &dmx_decimal);
    // dmx_float = dmx_data * 100.0 / 255.0;
    // printf("-> %03d.%01d real: %03.1f\n", dmx_integer, dmx_decimal, dmx_float);

    for (int i = 0; i < 256; i++)
    {
        dmx_data = (unsigned char) i;
        printf("Testing for dmx %d ", dmx_data);
        ColorTemp (dmx_data, &dmx_temp);
        printf("-> %d\n", dmx_temp);
    }
    
}

//--- end of file ---//


