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
    resp_sequence_ok,
    resp_sequence_nok,    
    resp_timeout

} comm_resp_e;


typedef enum {
    READY_TO_SEND,
    WAIT_ACKNOWLEDGE

} send_packet_state_e;


#define CH1_OFFSET    0
#define CH2_OFFSET    1
#define CH3_OFFSET    2
#define CH4_OFFSET    3


// Module Exported Functions ---------------------------------------------------
comm_resp_e COMM_SendPacket (char * p_to_send, unsigned short timeout);
send_packet_state_e COMM_Get_SendPacket_State (void);

void COMM_Manager (void);
void COMM_Timeouts (void);

#endif    /* _COMM_H_ */

//--- end of file ---//
