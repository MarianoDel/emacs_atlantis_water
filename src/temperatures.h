//-------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEMPERATURES.H ############################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _TEMPERATURES_H_
#define _TEMPERATURES_H_


// Exported Constants ----------------------------------------------------------
//---- LM335 measurement Temperatures (5.5mV/C in this circuit)
//---- 0.55 resistor divider
//25    1.65V    lm335 datasheet 3V
//30    1.677V
//35    1.705V
//50    1.787V
//65    1.87V
//75    1.925V    lm335 datasheet 3.5V
#define TEMP_IN_25    2047
#define TEMP_IN_30    2081
#define TEMP_IN_35    2115
#define TEMP_IN_50    2217
#define TEMP_IN_65    2320    
#define TEMP_IN_75    2389


//---- LM335 measurement Temperatures (8.2mV/C in this circuit)
//---- 0.82 resistor divider, 10.2 pts per degree
//37	2,572V
//40	2,600V
//45	2,650V
//50	2,681V
//55	2,725V
//60	2,765V
// #define TEMP_IN_30    3226
// #define TEMP_IN_35    3279
// #define TEMP_IN_48    3412
// #define TEMP_IN_50    3434
// #define TEMP_IN_65    3591
// #define TEMP_IN_70    3642
// #define TEMP_IN_85    3795


#define TEMP_DISCONECT    TEMP_IN_65
#define TEMP_RECONNECT    TEMP_IN_50



// Module Exported Functions ---------------------------------------------------


#endif    /* _TEMPERATURES_H_ */

//--- end of file ---//
