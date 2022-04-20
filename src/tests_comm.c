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
extern unsigned char valid_packet;
extern comm_resp_e send_packet_ack;
extern unsigned char manager_state;
extern unsigned char send_packet_timeout;
extern unsigned char send_packet_state;


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

    Test_Comms_Basic_Functions ();

    Test_Comms_Received_Data ();

    Test_Comms_Manager ();

    Test_Send_Packet ();
    
}


void Test_Comms_Basic_Functions (void)
{
    char message[200];

    COMM_SendKeepAlive();
    Usart1GetMessageSended(message);
    printf("Test keepalive: ");    
    if (strcmp("keepalive\n", message) == 0)
        PrintOK();
    else
        PrintERR();

    COMM_SendOK();
    Usart1GetMessageSended(message);
    printf("Test send ok: ");    
    if (strcmp("ok\n", message) == 0)
        PrintOK();
    else
        PrintERR();
    
    COMM_SendNOK();
    Usart1GetMessageSended(message);
    printf("Test send nok: ");    
    if (strcmp("nok\n", message) == 0)
        PrintOK();
    else
        PrintERR();
    
}


void Test_Comms_Received_Data (void)
{
    char msg [200];
    char msg_tx [200];    

    valid_packet = 0;
    strcpy (msg, "keepalive\n");
    COMM_ProcessPayload(msg);
    Usart1GetMessageSended(msg_tx);

    printf("Test rx keepalive: ");    
    if ((valid_packet) &&
        (strcmp("ok\n", msg_tx) == 0))
    {
        PrintOK();
    }
    else
        PrintERR();


    valid_packet = 0;
    strcpy (msg, "ok\n");
    COMM_ProcessPayload(msg);

    printf("Test rx ok: ");    
    if ((valid_packet) &&
        (send_packet_ack == resp_sended_ok))
    {
        PrintOK();
    }
    else
        PrintERR();


    valid_packet = 0;
    strcpy (msg, "nok\n");
    COMM_ProcessPayload(msg);

    printf("Test rx nok: ");    
    if ((valid_packet) &&
        (send_packet_ack == resp_sended_nok))
    {
        PrintOK();
    }
    else
        PrintERR();
    
}


void Test_Comms_Manager (void)
{
    Led_Link_Show_Status(1);
    valid_packet = 0;
    printf("Test Manager SM reset: ");
    manager_state = 5;
    COMM_Manager_Reset_SM ();
    if (manager_state == 0)
        PrintOK();
    else
        PrintERR();

    printf("Test Manager SM default: ");
    manager_state = 5;
    COMM_Manager_SM ();
    if (manager_state == 0)
        PrintOK();
    else
        PrintERR();

    
    printf("Test Manager SM no link: ");
    for (int i = 0; i < (150 + 1); i++)    //150ms for led toggle
    {
        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
    }

    if (manager_state == 1)
        PrintOK();
    else
        PrintERR();


    // Usart1FillRxBuffer("ok\n");
    printf("Test Manager SM one link: ");
    COMM_ProcessPayload ("ok\n");
    for (int i = 0; i < (150 + 1); i++)    //150ms for led toggle
    {
        COMM_Manager_SM ();
        COMM_Timeouts ();

        LF_Timeouts ();
    }

    if (manager_state == 2)
        PrintOK();
    else
        PrintERR();


    printf("Test Manager SM drop link: ");
    for (int i = 0; i < 6500; i++)
    {
        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
    }

    if (manager_state == 1)
        PrintOK();
    else
        PrintERR();

    
    printf("Test Manager SM one link: ");
    COMM_ProcessPayload ("ok\n");
    for (int i = 0; i < (150 + 1); i++)    //150ms for led toggle
    {
        COMM_Manager_SM ();
        COMM_Timeouts ();

        LF_Timeouts ();
    }

    if (manager_state == 2)
        PrintOK();
    else
        PrintERR();

    
}


void Test_Send_Packet (void)
{
    int ans = 1000;
    char s_answer [200];

    printf("Test Send Packet default: ");
    send_packet_state = 5;
    ans = COMM_SendPacket ("pulse 001 ch1\n");
    if ((ans == resp_working) &&
        (send_packet_state == 0))
        PrintOK();
    else
        PrintERR();


    // printf("Test Send Packet sended ok: ");
    for (int i = 0; i < 6500; i++)
    {
        ans = COMM_SendPacket ("pulse 001 ch1\n");
  //   resp_working,
  //   resp_sended_ok,
  //   resp_sended_nok,
  //   resp_packet_nok,    
  //   resp_timeout

        if (ans != resp_working)
        {
            sprintf(s_answer, "on index i: %d resp: %d", i, ans);
            break;
        }

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();

        COMM_ProcessPayload ("ok\n");        
    }
    printf("Test Send Packet sended ok: ");
    if (ans == 1)
        PrintOK();
    else
        PrintERR();

    printf("%s\n\n", s_answer);
    

    // printf("Test Send Packet sended nok: ");
    for (int i = 0; i < 6500; i++)
    {
        ans = COMM_SendPacket ("pulse 001 ch1\n");
  //   resp_working,
  //   resp_sended_ok,
  //   resp_sended_nok,
  //   resp_packet_nok,    
  //   resp_timeout

        if (ans != resp_working)
        {
            sprintf(s_answer, "on index i: %d resp: %d", i, ans);
            break;
        }

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();

        COMM_ProcessPayload ("nok\n");        
    }
    printf("Test Send Packet sended nok: ");
    if (ans == 2)
        PrintOK();
    else
        PrintERR();
    
    printf("%s\n\n", s_answer);

    
    // printf("Test Send Packet nok too long: ");
    for (int i = 0; i < 6500; i++)
    {
        ans = COMM_SendPacket ("pulse pulse pulse pulse 001 ch1\n");
  //   resp_working,
  //   resp_sended_ok,
  //   resp_sended_nok,
  //   resp_packet_nok,    
  //   resp_timeout

        if (ans != resp_working)
        {
            sprintf(s_answer, "on index i: %d resp: %d", i, ans);
            break;
        }

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
    }
    printf("Test Send Packet nok too long: ");
    if (ans == 3)
        PrintOK();
    else
        PrintERR();

    printf("%s\n\n", s_answer);


    // printf("Test Send Packet nok too small: ");
    for (int i = 0; i < 6500; i++)
    {
        ans = COMM_SendPacket ("");
  //   resp_working,
  //   resp_sended_ok,
  //   resp_sended_nok,
  //   resp_packet_nok,    
  //   resp_timeout

        if (ans != resp_working)
        {
            sprintf(s_answer, "on index i: %d resp: %d", i, ans);
            break;
        }

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
    }
    printf("Test Send Packet nok too small: ");
    if (ans == 3)
        PrintOK();
    else
        PrintERR();

    printf("%s\n\n", s_answer);
    

    // printf("Test Send Packet timeout: ");
    for (int i = 0; i < 6500; i++)
    {
        ans = COMM_SendPacket ("pulse 001 ch1\n");
  //   resp_working,
  //   resp_sended_ok,
  //   resp_sended_nok,
  //   resp_packet_nok,    
  //   resp_timeout

        if (ans != resp_working)
        {
            sprintf(s_answer, "on index i: %d resp: %d", i, ans);
            break;
        }

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
    }
    printf("Test Send Packet timeout: ");
    if (ans == 4)
        PrintOK();
    else
        PrintERR();

    printf("%s\n\n", s_answer);


    // printf("Test Send Packet no newline: ");
    for (int i = 0; i < 6500; i++)
    {
        ans = COMM_SendPacket ("pulse 001 ch1");
  //   resp_working,
  //   resp_sended_ok,
  //   resp_sended_nok,
  //   resp_packet_nok,    
  //   resp_timeout

        if (ans != resp_working)
        {
            sprintf(s_answer, "on index i: %d resp: %d", i, ans);
            break;
        }

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
    }
    printf("Test Send Packet no newline: ");
    if (ans == 4)
        PrintOK();
    else
        PrintERR();

    printf("%s\n\n", s_answer);

    printf("Test Send Packet READY_TO_SEND: ");
    if (COMM_ReadyToSend())
        PrintOK();
    else
        PrintERR();
    
    // printf("Test Send Packet not READY_TO_SEND: ");
    ans = COMM_SendPacket ("pulse 001 ch1\n");
    printf("Test Send Packet not READY_TO_SEND: ");    
    if ((ans == resp_working) &&
        (!COMM_ReadyToSend()))
        PrintOK();
    else
        PrintERR();


    // printf("Test Send Packet seq loop: ");
    int seq = 0;
    do {
        ans = COMM_SendPacket ("pulse 001 ch1");
        if (ans != resp_working)
            seq++;

        COMM_Manager_SM ();
        COMM_Timeouts ();
        LF_Timeouts ();
        
    } while (seq < 1000);
    printf("Test Send Packet seq loop: ");
    if (ans == 4)
        PrintOK();
    else
        PrintERR();

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


//--- end of file ---//


