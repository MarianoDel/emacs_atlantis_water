//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "comm.h"
#include "led_functions.h"

//helper modules
#include "tests_ok.h"
#include "tests_mock_usart.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern comm_resp_e send_packet_ack;
extern unsigned short send_packet_timeout;
extern unsigned char send_packet_state;
extern unsigned short manager_timer;

extern unsigned char seq_ack;

extern unsigned short COMM_WritePacket (char * p_buff, char * p_to_send);
extern comm_resp_e COMM_ProcessPayload (char * payload);
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
void Test_Comms_Basic_Functions (void);
void Test_Comms_Received_Data (void);
void Test_Send_Packet (void);
void Test_Comms_Manager (void);

    

// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    // Test_Comms_Basic_Functions ();

    // Test_Comms_Manager ();

    Test_Send_Packet ();
    
}


void Test_Comms_Basic_Functions (void)
{
    char message[200];

    Usart1GetMessageSended(message);
    printf("Test COMM_WritePacket sequence: ");

    int sequence = 0;
    int error = 0;
    for (int i = 0; i < 2000; i++)
    {
        sequence = COMM_WritePacket (message, "000 000 000 000 0000");
        if ((sequence < 1) || (sequence > 999))
        {
            error = 1;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf("error on sequence: %d\n", sequence);
    }
    else
        PrintOK();

    printf("Test COMM_WritePacket string: ");
    PrintOK();
    sequence = COMM_WritePacket (message, "000 000 000 000 0000");
    printf("\n");
    printf(message);

    sequence = COMM_WritePacket (message, "000 000 000 000 0000");
    printf("\n");
    printf(message);

    sequence = COMM_WritePacket (message, "000 000 000 000 0000");
    printf("\n");
    printf(message);

    char error_str [100];
    strcpy(error_str, "s  s  sok");
    printf("Test COMM_ProcessPayload error %s: ", error_str);
    if (COMM_ProcessPayload(error_str) == resp_sended_nok)
        PrintOK();
    else
        PrintERR();

    strcpy(error_str, "s00000 ok");
    printf("Test COMM_ProcessPayload error %s: ", error_str);
    if (COMM_ProcessPayload(error_str) == resp_sended_nok)
        PrintOK();
    else
        PrintERR();

    strcpy(error_str, "s00  ok");
    printf("Test COMM_ProcessPayload error %s: ", error_str);
    if (COMM_ProcessPayload(error_str) == resp_sended_nok)
        PrintOK();
    else
        PrintERR();

    printf("Test COMM_ProcessPayload all seq ok: ");
    for (int i = 0; i < 1000; i++)
    {        
        sprintf(message, "s%03d ok", i);
        if (COMM_ProcessPayload(message) == resp_sended_nok)
        {
            error = 1;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf("error on sequence: %s\n", message);
    }
    else
        PrintOK();
        
}


void Test_Comms_Manager (void)
{
    // up led link
    Led_Link_Show_Status (1);
    COMM_Manager ();
    send_packet_ack = resp_working;
    Usart1FillRxBuffer ("s000 ok");
    COMM_Manager ();
    Led_Link_Show_Status(0);
    for (int i = 0; i < 1505; i++)
    {
        COMM_Manager ();
        if (manager_timer)
            manager_timer--;
        else
        {
            Led_Link_Show_Status(1);
            printf("on i: %d\n", i);
        }
    }

    printf("Comm manager test: ");
    if (send_packet_ack == resp_sended_ok)
        PrintOK();
    else
        PrintERR();
}


void Test_Send_Packet (void)
{
    int error = 0;
    int ans = 1000;
    char s_answer [200];

    ans = COMM_SendPacket ("001 001 001 001 0000", 1000);
    if ((ans == resp_working) &&
        (send_packet_state == 1))
    {
        error = 1;
    }

    Usart1FillRxBuffer ("s001 ok");
    for (int i = 0; i < 10; i++)
    {
        ans = COMM_SendPacket ("001 001 001 001 0000", 1000);
        if (ans != resp_working)
            break;

        COMM_Manager();
    }

    if ((ans == resp_sended_ok) &&
        (send_packet_state == 0) &&
        (seq_ack == 1))
    {
        error = 2;
    }

    printf("Test Send Packet default: ");

    if (error != 2)
    {
        PrintERR();
        printf("error: %d\n", error);
    }
    else
        PrintOK();

}


// -- Module Mocked Functions --------------------------------------------------
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


void Usart1SendDelayed (char * to_send)
{
    int len = strlen(to_send);

    if (len < 128)
    {
        Usart1Send(to_send);
    }
    else
        printf("Usart1SendDelayed ERROR! buffer too long\n");
}
//--- end of file ---//


