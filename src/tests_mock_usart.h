//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MOCK_USART.H ####################
//---------------------------------------------

// Module Exported Functions ---------------------------------------------------
void Usart1Send (char * msg);
unsigned char Usart1ReadBuffer (unsigned char * bout, unsigned short max_len);
void Usart1FillRxBuffer (char * msg);
unsigned char Usart1HaveData (void);
void Usart1HaveDataReset (void);
void Usart1GetMessageSended (char * msg_sended);

void Usart2Send (char * msg);
unsigned char Usart2ReadBuffer (unsigned char * bout, unsigned short max_len);
void Usart2FillRxBuffer (char * msg);
unsigned char Usart2HaveData (void);
void Usart2HaveDataReset (void);


//--- end of file ---//


