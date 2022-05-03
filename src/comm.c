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

#include <string.h>
#include <stdio.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    READY_TO_SEND,
    WAIT_ACKNOWLEDGE

} send_packet_state_e;


typedef enum {
    MGR_INIT,
    MGR_NO_LINK,
    MGR_IN_LINK

} manager_state_e;


#define TT_PACKET    20    //20ms for timeouts
#define TT_MGR_PACKET_NO_LINK    5000
#define TT_MGR_PACKET_KEEP_LINK    1500
#define PACKETS_FOR_LINK_DOWN    5

#define Usart1Send(X)    Usart1SendDelayed(X)
// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char send_packet_timeout = 0;
volatile unsigned short manager_timer = 0;
volatile unsigned short manager_timer_tx = 0;

unsigned short seq = 0;
// unsigned short send_packet_expected_seq = 0;
comm_resp_e send_packet_ack = resp_working;

char send_buff [30] = { 0 };

unsigned char valid_packet = 0;



// Module Private Functions ----------------------------------------------------
void CommsCheckSendOK (char * orig_num);


// Module Functions ------------------------------------------------------------
void COMM_ProcessPayload (char * payload)
{
    if (!strncmp(payload, "keepalive", sizeof ("keepalive") - 1))
    {
        valid_packet |= VALID_PKT_KEEP;
    }

    else if (!strncmp(payload, "ok", sizeof ("ok") - 1))
    {
        valid_packet |= VALID_PKT_OK;
        send_packet_ack = resp_sended_ok;
    }

    else if (!strncmp(payload, "nok", sizeof ("nok") - 1))
    {
        valid_packet |= VALID_PKT_NOK;
        send_packet_ack = resp_sended_nok;
    }
}


void COMM_SendKeepAlive (void)
{
    Usart1Send("keepalive\n");
}


void COMM_SendOK (void)
{
    Usart1Send("ok\n");
}


void COMM_SendNOK (void)
{
    Usart1Send("nok\n");
}


send_packet_state_e send_packet_state = READY_TO_SEND;
comm_resp_e COMM_SendPacket (char * p_to_send)
{
    comm_resp_e resp = resp_working;
    unsigned short new_seq = 0;

    switch (send_packet_state)
    {
    case READY_TO_SEND:
        new_seq = COMM_WritePacket(send_buff, p_to_send);
        if (new_seq)
        {
            Usart1Send(send_buff);
            send_packet_timeout = TT_PACKET;
            send_packet_ack = resp_working;
            // send_packet_expected_seq = new_seq;
            send_packet_state++;
        }
        else
            resp = resp_packet_nok;
        
        break;

    case WAIT_ACKNOWLEDGE:
        if (send_packet_ack == resp_sended_ok)
        {
            send_packet_state = READY_TO_SEND;
            resp = resp_sended_ok;
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


unsigned char COMM_ReadyToSend (void)
{
    if (send_packet_state == READY_TO_SEND)
        return 1;

    return 0;
}


unsigned short COMM_WritePacket (char * p_buff, char * p_to_send)
{
    unsigned char len = strlen(p_to_send);

    if ((len == 0) || (len > 20))    // no more than 20 bytes of payload
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


manager_state_e manager_state = MGR_INIT;
void COMM_Manager_SM (void)
{
    unsigned char valid_pkt_rx = 0;

    if (Usart1HaveData())
    {
        char local [128] = { 0 };
            
        Usart1HaveDataReset();
        Usart1ReadBuffer((unsigned char *) local, 128);
        COMM_ProcessPayload(local);

        if (valid_packet & VALID_PKT_KEEP)
        {
            valid_packet = 0;
            valid_pkt_rx = 1;
            Wait_ms(5);
            LF_Link_Pulse();
            COMM_SendOK();
        }

        // if ((valid_packet & VALID_PKT_OK) ||
        //     (valid_packet & VALID_PKT_NOK))
        // {
        //     valid_packet = 0;
        //     valid_pkt_rx = 1;
        // }
    }
    
    switch (manager_state)
    {
    case MGR_INIT:
        LF_Link_Reset();
        manager_timer = 0;
        manager_state++;
        break;

    case MGR_NO_LINK:
        // send my keepalive
        if (!manager_timer_tx)
        {
            COMM_SendKeepAlive ();            
            LF_Link_Pulse();
            manager_timer_tx = TT_MGR_PACKET_KEEP_LINK;            
        }

        // wait to see the ok
        if (valid_packet & VALID_PKT_OK)
        {
            valid_packet = 0;
            manager_timer = TT_MGR_PACKET_NO_LINK;
            LF_Link_Set();
            manager_state++;
        }
        break;

    case MGR_IN_LINK:
        if (valid_pkt_rx)
        {
            valid_pkt_rx = 0;
            manager_timer = TT_MGR_PACKET_NO_LINK;
        }

        if (valid_packet & VALID_PKT_OK)
        {
            valid_packet = 0;
            manager_timer = TT_MGR_PACKET_NO_LINK;
        }
        
        if (!manager_timer_tx)
        {
            COMM_SendKeepAlive ();
            LF_Link_Pulse();
            manager_timer_tx = TT_MGR_PACKET_KEEP_LINK;
        }

        if (!manager_timer)    // link its down
            manager_state = MGR_INIT;

        break;

    default:
        manager_state = MGR_INIT;
        break;
    }
}


void COMM_Manager_Reset_SM (void)
{
    manager_state = MGR_INIT;
}


unsigned char COMM_Manager_In_Link (void)
{
    if (manager_state == MGR_IN_LINK)
        return 1;

    return 0;
}


void COMM_Timeouts (void)
{
    if (send_packet_timeout)
        send_packet_timeout--;

    if (manager_timer)
        manager_timer--;

    if (manager_timer_tx)
        manager_timer_tx--;
    
}
//--- end of file ---//
