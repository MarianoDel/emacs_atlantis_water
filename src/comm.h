//------------------------------------------------------
// #### ATLANTIS WATER METER PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMM.H #########################################
//------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _COMM_H_
#define _COMM_H_


// Module Exported Types Constants and Macros ----------------------------------
typedef enum {
    resp_working,
    resp_sended_ok,
    resp_sended_nok,
    resp_packet_nok,    
    resp_timeout

} comm_resp_e;


// Module Exported Functions ---------------------------------------------------
void COMM_SendKeepAlive (void);
void COMM_SendOK (void);
void COMM_SendNOK (void);

comm_resp_e COMM_SendPacket (char * p_to_send);
unsigned char COMM_ReadyToSend (void);
unsigned short COMM_WritePacket (char * p_buff, char * p_to_send);

void COMM_ProcessPayload (char * msg);

void COMM_Manager_SM (void);
void COMM_Manager_Reset_SM (void);
void COMM_Timeouts (void);

// void CommsProcessSMSPayload (char * orig_num, char * payload);
// unsigned char VerifyNumberString (char * number);
// unsigned char VerifySiteString (char * site);

#endif    /* _COMM_H_ */

//--- end of file ---//
