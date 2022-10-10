//------------------------------------------------------
// #### ATLANTIS WATER METER PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMM.C #########################################
//------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comm.h"
#include "usart.h"
#include "led_functions.h"
#include "tim.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Private Types Constants and Macros ------------------------------------------
// #define TT_PACKET    20    //20ms for timeouts
#define TT_PACKET    100    //20ms for timeouts for problems with app
#define TT_MGR_PACKET_NO_LINK    5000
#define TT_MGR_PACKET_KEEP_LINK    1500
#define PACKETS_FOR_LINK_DOWN    5

#define Usart1Send(X)    Usart1SendDelayed(X)
// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned short send_packet_timeout = 0;
volatile unsigned short manager_timer = 0;

unsigned short seq = 0;
unsigned short seq_ack = 0;
unsigned short seq_last_sended = 0;

comm_resp_e send_packet_ack = resp_working;
char comm_buff [100] = { 0 };


// Module Private Functions ----------------------------------------------------
unsigned char COMM_SendOKEnable (void);
comm_resp_e COMM_ProcessPayload (char * payload);
unsigned short COMM_WritePacket (char * p_buff, char * p_to_send);


// Module Functions ------------------------------------------------------------
comm_resp_e COMM_ProcessPayload (char * payload)
{
    comm_resp_e resp = resp_sended_nok;
    
    // check packet must start with s
    if ((*(payload + 0) == 's') &&
        (*(payload + 4) == ' '))
    {
        // check if its ok or nok
        if (strncmp((payload + 5), "ok", sizeof("ok") - 1) == 0)
        {
            // sanity check
            unsigned char len = StringCheckNumbers ((payload + 1), 3);
            if (len == 3)
            {
                seq_ack = atoi((payload + 1));
                resp = resp_sended_ok;
            }
        }
    }

    return resp;
}


send_packet_state_e send_packet_state = READY_TO_SEND;
comm_resp_e COMM_SendPacket (char * p_to_send, unsigned short timeout)
{
    comm_resp_e resp = resp_working;
    unsigned short new_seq = 0;

    switch (send_packet_state)
    {
    case READY_TO_SEND:
        new_seq = COMM_WritePacket(comm_buff, p_to_send);
        if (new_seq)
        {
            Usart1Send(comm_buff);
            send_packet_timeout = timeout;
            send_packet_ack = resp_working;
            send_packet_state++;
            seq_last_sended = new_seq;
            LF_Link_Pulse();
        }
        else
            resp = resp_packet_nok;
        
        break;

    case WAIT_ACKNOWLEDGE:
        if (send_packet_ack == resp_sended_ok)
        {
            send_packet_state = READY_TO_SEND;
            if (seq_last_sended == seq_ack)
                resp = resp_sended_ok;
            else
                resp = resp_sended_nok;
            
        }
        else if (send_packet_ack == resp_sended_nok)
        {
            send_packet_state = READY_TO_SEND;
            resp = resp_sended_nok;
        }
        else if (!send_packet_timeout)            
        {
            send_packet_state = READY_TO_SEND;
            resp = resp_timeout;
        }
        break;

    default:
        send_packet_state = READY_TO_SEND;
        break;
        
    }

    return resp;
}


send_packet_state_e COMM_Get_SendPacket_State (void)
{
    return send_packet_state;
}


unsigned short COMM_WritePacket (char * p_buff, char * p_to_send)
{
    unsigned char len = strlen(p_to_send);

    if ((len == 0) || (len > 90))
        return 0;
    
    if (seq < 999)    //numbers from 1 to 999
        seq++;
    else
        seq = 1;

    if (*(p_to_send + len - 1) == '\n')
        sprintf(p_buff, "s%03d %s", seq, p_to_send);
    else
        sprintf(p_buff, "s%03d %s\n", seq, p_to_send);

    return seq;
}


unsigned char led_link_latch = 0;
void COMM_Manager (void)
{
    comm_resp_e resp = resp_working;

    if (Usart1HaveData())
    {
        char local [128] = { 0 };
            
        Usart1HaveDataReset();
        Usart1ReadBuffer((unsigned char *) local, 128);
        resp = COMM_ProcessPayload(local);

        if (resp == resp_sended_ok)
        {
            LF_Link_Set ();
            led_link_latch = 1;
            manager_timer = 1500;
            // acknowledge to sendpacket
            send_packet_ack = resp_sended_ok;
        }
    }

    if (!manager_timer)    // link its down
    {
        if (led_link_latch)
        {
            LF_Link_Reset();
            led_link_latch = 0;
        }
    }

}


void COMM_Timeouts (void)
{
    if (send_packet_timeout)
        send_packet_timeout--;

    if (manager_timer)
        manager_timer--;

}

//--- end of file ---//
