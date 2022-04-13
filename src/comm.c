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
#include "uart.h"

#include <string.h>
#include <stdio.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    READY_TO_SEND,
    WAIT_ACKNOWLEDGE

} send_packet_state_e;

#define TT_PACKET    20    //20ms for timeouts


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char send_packet_timeout = 0;

unsigned short seq = 0;
unsigned short send_packet_expected_seq = 0;
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
        valid_packet = 1;
        COMM_SendOK();
    }

    else if (!strncmp(payload, "ok", sizeof ("ok") - 1))
    {
        valid_packet = 1;
        send_packet_ack = resp_sended_ok;
    }

    else if (!strncmp(payload, "nok", sizeof ("nok") - 1))
    {
        valid_packet = 1;
        send_packet_ack = resp_sended_nok;
    }

//     else if (!strncmp(payload, "REPORTAR_NUM:", sizeof ("REPORTAR_NUM:") - 1))
//     {
//         char * p_new_number = (payload + sizeof ("REPORTAR_NUM:") - 1);
//         unsigned char report_ok = 0;

//         unsigned char len = strlen(p_new_number);
//         if (len < (20 + 2))
//         {
//             char new_number [20] = { 0 };
//             strncpy(new_number, p_new_number, (len - 2));

// #ifdef COMM_DEBUG_ON
//             char debug [60] = {'\0'};
//             sprintf(debug, "nuevo numero a reportar %s\n", new_number);
//             Usart2Send(debug);
// #endif
//             if (VerifyNumberString(new_number) == 1)
//             {
//                 num_tel_rep_change_set;
//                 strcpy(num_tel_rep, new_number);
//                 report_ok = 1;
//             }
//         }
        
//         if (envios_ok)
//         {
//             enviar_sms = 1;
//             strcpy(enviar_sms_num, orig_num);
//             if (report_ok)
//                 strcpy(enviar_sms_msg, "OK");
//             else
//                 strcpy(enviar_sms_msg, "NOK");
            
//         }
//     }

//     else if (!strncmp(payload, "REPORTAR_SITIO:", sizeof ("REPORTAR_SITIO:") -1))
//     {
//         char * p_new_place = (payload + sizeof ("REPORTAR_SITIO:") - 1);
//         unsigned char report_ok = 0;

//         unsigned char len = strlen(p_new_place);
//         if (len < (SITE_MAX_LEN + 2))
//         {
//             char new_site [SITE_MAX_LEN + 1] = { 0 };
//             strncpy(new_site, p_new_place, len - 2);    //quito el trailing OK

// #ifdef COMM_DEBUG_ON
//             char debug [SITE_MAX_LEN + 20] = {'\0'};
//             sprintf(debug, "nuevo lugar %s\n", new_site);
//             Usart2Send(debug);
// #endif
//             if (VerifySiteString(new_site) == 1)
//             {
//                 sitio_prop_change_set;
//                 strcpy(sitio_prop, new_site);
//                 report_ok = 1;
//             }
//         }
        
//         if (envios_ok)
//         {
//             enviar_sms = 1;
//             strcpy(enviar_sms_num, orig_num);

//             if (report_ok)
//                 strcpy(enviar_sms_msg, "OK");
//             else
//                 strcpy(enviar_sms_msg, "NOK");
//         }
//     }

//     else if (!strncmp(payload, "REPORTAR_BAT:", sizeof ("REPORTAR_BAT:") - 1))
//     {
//         char * p_new_conf = (payload + sizeof ("REPORTAR_BAT:") - 1);
//         if (*p_new_conf == '0')
//         {
//             battery_check = 0;
//             battery_check_change_set;
//             CommsCheckSendOK (orig_num);
//         }
//         else if (*p_new_conf == '1')
//         {
//             battery_check = 1;
//             battery_check_change_set;
//             CommsCheckSendOK (orig_num);
//         }
//     }

//     // Diagnostics and Activations    
//     else if (!strncmp(payload, (const char *)"PRENDER:", sizeof ("PRENDER:") - 1))
//     {
//         diag_prender_set;
//         CommsCheckSendOK (orig_num);        
//     }

//     else if (!strncmp(payload, "BATERIA:", sizeof ("BATERIA:") - 1))
//     {
//         // diag_battery_set;
//         unsigned char volts_int = 0;
//         unsigned char volts_dec = 0;
//         Battery_Voltage(&volts_int, &volts_dec);

//         enviar_sms = 1;
//         strcpy(enviar_sms_num, orig_num);
//         sprintf(enviar_sms_msg, "BAT: %02d.%02dV", volts_int, volts_dec);
//     }
}


// //answer 1 -> ok; 0 -> some error
// unsigned char VerifyNumberString (char * number)
// {
//     unsigned char len = 0;
//     len = strlen(number);

//     if ((len > 19) || (len < 3))
//         return 0;

//     for (unsigned char i = 0; i < len; i++)
//     {
//         if (*(number + i) != '+')
//         {
//             if ((*(number + i) > '9') ||
//                 (*(number + i) < '0'))
//                 return 0;
//         }
//     }

//     return 1;
// }


// //answer 1 -> ok; 0 -> some error
// unsigned char VerifySiteString (char * site)
// {
//     unsigned char len = 0;
//     len = strlen(site);

//     if ((len > SITE_MAX_LEN) || (len < 3))
//         return 0;

//     for (unsigned char i = 0; i < len; i++)
//     {
//         if ((*(site + i) == 'á') ||
//             (*(site + i) == 'é') ||
//             (*(site + i) == 'í') ||
//             (*(site + i) == 'ó') ||
//             (*(site + i) == 'ú'))
//         {
//             // do nothing here
//         }
//         else if ((unsigned char) *(site + i) == 193)
//             *(site + i) = 'A';
//         else if ((unsigned char) *(site + i) == 201)
//             *(site + i) = 'E';
//         else if ((unsigned char) *(site + i) == 205)
//             *(site + i) = 'I';
//         else if ((unsigned char) *(site + i) == 211)
//             *(site + i) = 'O';
//         else if ((unsigned char) *(site + i) == 218)
//             *(site + i) = 'U';
//         else if ((*(site + i) > '~') ||
//                  (*(site + i) < ' '))
//             return 0;
//     }

//     return 1;
    
// }


// void CommsCheckSendOK (char * orig_num)
// {
//     if (envios_ok)
//     {
//         enviar_sms = 1;
//         strcpy(enviar_sms_num, orig_num);
//         strcpy(enviar_sms_msg, "OK");
//     }
// }


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
            send_packet_expected_seq = new_seq;
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

    if (len > 20)    // no more than 20 bytes of payload
        return 0;

    if (seq < 1000)
        seq++;
    else
        seq = 1;

    if (*(p_to_send + len) == '\n')
        sprintf(p_buff, "s%03d %s", seq, p_to_send);
    else
        sprintf(p_buff, "s%03d %s\n", seq, p_to_send);

    return seq;
}


void COMM_Timeouts (void)
{
    if (send_packet_timeout)
        send_packet_timeout--;
    
}
//--- end of file ---//
