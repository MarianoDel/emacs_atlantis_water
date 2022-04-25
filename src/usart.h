//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### USART.H ###############################
//---------------------------------------------
#ifndef _USART_H_
#define _USART_H_


// Module Exported Types Constants and Macros ----------------------------------


// Module Exported Functions ---------------------------------------------------
void Usart1Config(void);
void Usart1SendSingle(unsigned char );

unsigned char Usart1ReadBuffer (unsigned char *, unsigned short);

void USART1_IRQHandler(void);
void Usart1Send (char *);
void Usart1SendUnsigned(unsigned char *, unsigned char);
void Usart1SendDelayed (char * send);

unsigned char Usart1HaveData (void);
void Usart1HaveDataReset (void);


#endif    /* _USART_H_ */

//--- end of file ---//

