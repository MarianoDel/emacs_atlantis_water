//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### FLASH_PROGRAM.H #######################
//---------------------------------------------

// Define to prevent recursive inclusion ---------------------------------------
#ifndef _FLASH_PROGRAM_H_
#define _FLASH_PROGRAM_H_

// Includes for Configurations -------------------------------------------------
#include "stm32f0xx.h"

// Configurations Defines ------------------------------------------------------
//- Where to Backup ------------------------------------------------------------
#define FLASH_PAGE_FOR_BKP PAGE31    //

//- Data Struct to Backup ------------------------------------------------------
 //OJO esta estructuras deben estar alineadas en 4 bytes
// typedef struct {

//     unsigned char treatment_time_min;    //1
//     unsigned char alarms_onoff;    //2
//     unsigned char ticker_onoff;    //3
//     unsigned short ticker_time;    //5

//     unsigned int saved_psw;    //9
//     unsigned int new_psw;    //13    
//     unsigned char operation_mode;    //14
    
//     unsigned char dummy1;    //15
//     unsigned char dummy2;    //16
//     // unsigned char dummy3;    //17


// } parameters_typedef;

// typedef enum {
//     MODE_UNKNOW = 0,
//     NORMAL_MODE,
//     CARD_MODE
    
// } operation_modes_t;



//- End of Data Struct to Backup -----------------------------------------------

// Define the STM32F10x FLASH Page Size depending on the used STM32 device
// si es mayor a 128K la pagina es de 2KB, sino 1KB

//#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#define FLASH_PAGE_SIZE			1024
#define FLASH_PAGE_SIZE_DIV2	512
#define FLASH_PAGE_SIZE_DIV4	256

#define PAGE28_START	0
#define PAGE29_START	256
#define PAGE30_START	512
#define PAGE31_START	768
#define PAGE31_END		1023

#define PAGE27			((uint32_t)0x08006C00)
#define PAGE28			((uint32_t)0x08007000)
#define PAGE29			((uint32_t)0x08007400)
#define PAGE30			((uint32_t)0x08007800)
#define PAGE31			((uint32_t)0x08007C00)

#define PAGE60			((uint32_t)0x0800F000)
#define PAGE61			((uint32_t)0x0800F400)
#define PAGE63			((uint32_t)0x0800FC00)


//de libreria st las tiene #include "stm32f0xx_flash.h"
/**
  * @brief  FLASH Status
  */
typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

/** @defgroup FLASH_Timeout_definition
  * @{
  */
#define FLASH_ER_PRG_TIMEOUT         ((uint32_t)0x000B0000)

/** @defgroup FLASH_Flags
  * @{
  */

#define FLASH_FLAG_BSY                 FLASH_SR_BSY     /*!< FLASH Busy flag */
#define FLASH_FLAG_PGERR               FLASH_SR_PGERR   /*!< FLASH Programming error flag */
#define FLASH_FLAG_WRPERR              FLASH_SR_WRPERR  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_EOP                 FLASH_SR_EOP     /*!< FLASH End of Programming flag */

#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFFCB) == 0x00000000) && ((FLAG) != 0x00000000))

#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_PGERR) || \
                                  ((FLAG) == FLASH_FLAG_WRPERR) || ((FLAG) == FLASH_FLAG_EOP))



typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


// Module exported Functions ---------------------------------------------------
unsigned char WriteConfigurations (void);
unsigned char WriteFlash(unsigned int * p, uint32_t p_addr, unsigned char with_lock, unsigned char len_in_4);

#endif    /* _FLASH_PROGRAM_H_ */

//--- end of file ---//

