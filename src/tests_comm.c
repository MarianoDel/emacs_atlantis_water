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

//helper modules
#include "tests_ok.h"
#include "tests_mock_usart.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern unsigned char valid_packet;
extern comm_resp_e send_packet_ack;


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Comms_Basic_Functions (void);
void Test_Comms_Received_Data (void);

    

// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    Test_Comms_Basic_Functions ();

    Test_Comms_Received_Data ();
    
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




//--- end of file ---//


