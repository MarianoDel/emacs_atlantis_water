//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### USART.C ###############################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "usart.h"
#include "stm32f0xx.h"

#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
#define USART1_CLK    (RCC->APB2ENR & 0x00004000)
#define USART1_CLK_ON    (RCC->APB2ENR |= 0x00004000)
#define USART1_CLK_OFF    (RCC->APB2ENR &= ~0x00004000)

#define USART_9600		5000
#define USART_115200		416
#define USART_250000		192

#define SIZEOF_RXDATA 128
#define SIZEOF_TXDATA 128


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char usart1_have_data = 0;
volatile unsigned char * ptx1;
volatile unsigned char * ptx1_pckt_index;
volatile unsigned char * prx1;
volatile unsigned char tx1buff[SIZEOF_TXDATA];
volatile unsigned char rx1buff[SIZEOF_RXDATA];


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Usart1Config (void)
{
    if (!USART1_CLK)
        USART1_CLK_ON;

    ptx1 = tx1buff;
    ptx1_pckt_index = tx1buff;
    prx1 = rx1buff;

    USART1->BRR = USART_9600;
    // USART1->CR2 |= USART_CR2_STOP_1;	//2 bits stop
    // USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

    unsigned int temp;
    temp = GPIOA->AFR[1];
    temp &= 0xFFFFF00F;
    temp |= 0x00000110;    //PA10 -> AF1, PA9 -> AF1
    GPIOA->AFR[1] = temp;

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 7);
}


void Usart1SendSingle(unsigned char tosend)
{
    Usart1SendUnsigned(&tosend, 1);
}


void Usart1Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Usart1SendUnsigned((unsigned char *) send, i);
}


void Usart1SendUnsigned(unsigned char * send, unsigned char size)
{
    if ((ptx1_pckt_index + size) < &tx1buff[SIZEOF_TXDATA])
    {
        memcpy((unsigned char *)ptx1_pckt_index, send, size);
        ptx1_pckt_index += size;
        USART1->CR1 |= USART_CR1_TXEIE;
    }
}


unsigned char Usart1ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx1 - rx1buff;

    if (len < max_len)
        len += 1;    //space for '\0' from int
    else
        len = max_len;

    memcpy(bout, (unsigned char *) rx1buff, len);

    //pointer adjust after copy
    prx1 = rx1buff;
    return (unsigned char) len;
}


void USART1_IRQHandler(void)
{
    unsigned char dummy;

    // USART in Rx mode --------------------------------------------------
    if (USART1->ISR & USART_ISR_RXNE)
    {
        dummy = USART1->RDR & 0x0FF;

        if (prx1 < &rx1buff[SIZEOF_RXDATA - 1])
        {
            if (dummy == '\r')
            {
            }
            else if ((dummy == '\n') || (dummy == 26))    //CTRL+J ("\r\n"); CTRL-Z (26)
            {
                if (dummy == 26)    //special case handle
                {
                    *prx1 = dummy;
                    prx1++;
                }

                *prx1 = '\0';
                usart1_have_data = 1;
            }
            else
            {
                *prx1 = dummy;
                prx1++;
            }
        }
        else
            prx1 = rx1buff;    // fix the block by gardbage bytes

    }

    // USART in Tx mode -------------------------------------------------
    if (USART1->CR1 & USART_CR1_TXEIE)
    {
        if (USART1->ISR & USART_ISR_TXE)
        {
            if ((ptx1 < &tx1buff[SIZEOF_TXDATA]) && (ptx1 < ptx1_pckt_index))
            {
                USART1->TDR = *ptx1;
                ptx1++;
            }
            else
            {
                ptx1 = tx1buff;
                ptx1_pckt_index = tx1buff;
                USART1->CR1 &= ~USART_CR1_TXEIE;
            }
        }
    }

    if ((USART1->ISR & USART_ISR_ORE) || (USART1->ISR & USART_ISR_NE) || (USART1->ISR & USART_ISR_FE))
    {
        USART1->ICR |= 0x0e;
        dummy = USART1->RDR;
    }
}


unsigned char Usart1HaveData (void)
{
    return usart1_have_data;
}


void Usart1HaveDataReset (void)
{
    usart1_have_data = 0;
}


//--- end of file ---//
