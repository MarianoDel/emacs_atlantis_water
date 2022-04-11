//------------------------------------------------------
// #### ATLANTIS WATER METER PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### LED_FUNCTIONS.H ################################
//------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __LED_FUNCTIONS_H_
#define __LED_FUNCTIONS_H_


// Module Exported Types Constants and Macros ----------------------------------
#define L1_TOGGLE    0x01
#define L2_TOGGLE    0x02
#define L3_TOGGLE    0x04
#define L4_TOGGLE    0x08



// Module Exported Functions ---------------------------------------------------
void LF_Link_Pulse (void);
void LF_Link_Set (void);
void LF_Link_Reset (void);

void LF_Leds_Pulse_Toggle_Activate (void);
void LF_Leds_Pulse_Toggle_Set (unsigned char ltoggle);
void LF_Leds_Pulse_Toggle_Reset (unsigned char ltoggle);

    

#endif    /* __LED_FUNCTIONS_H_ */

//--- end of file ---//

