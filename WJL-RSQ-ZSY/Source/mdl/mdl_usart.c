/**********************************************
 * All rights reserved.
 *
 * File:
 * Author:	lxj
 * Date:		2022-8-12
 */
// usart
//*********************************************
#include "all_head.h"

#define USART_SUM 1
#define USART_TICK_MAX 10
#define USART_NO_RECEIVE_TIME_MAX 50 // 5 *10 ms
#define UART_SEND_OVER_TIME 500
typedef struct
{
    UsartIndex index;
    unsigned char isSending : 1;
    unsigned char isReceving : 1;
    unsigned char isSendFinished : 1;
    unsigned char isReceiveFinished : 1;
    unsigned char reserve : 4;
    uint16_t noReceiveCounter;
    uint16_t usartSendOverTimerCnt;

    UsartBufferStruct *pSendBuffer;
    UsartBufferStruct *pReceiveBuffer;
} UsartStruct;

#if (USART_SUM)
#define USART0_SEND_BUFFER_LEN 100
#define USART0_RECE_BUFFER_LEN 100
static unsigned char usart0SendBufferPool[USART0_SEND_BUFFER_LEN];
static unsigned char usart0ReceBufferPool[USART0_RECE_BUFFER_LEN];
static UsartBufferStruct usart0SendBuffer = {usart0SendBufferPool, USART0_SEND_BUFFER_LEN, 0, 0};
static UsartBufferStruct usart0ReceBuffer = {usart0ReceBufferPool, USART0_RECE_BUFFER_LEN, 0, 0};
#endif
#if (USART_SUM > 1)
#define USART1_SEND_BUFFER_LEN 50
#define USART1_RECE_BUFFER_LEN 50
static unsigned char usart1SendBufferPool[USART1_SEND_BUFFER_LEN];
static unsigned char usart1ReceBufferPool[USART1_RECE_BUFFER_LEN];
static UsartBufferStruct usart1SendBuffer = {usart1SendBufferPool, USART1_SEND_BUFFER_LEN, 0, 0};
static UsartBufferStruct usart1ReceBuffer = {usart1ReceBufferPool, USART1_RECE_BUFFER_LEN, 0, 0};
#endif
#if (USART_SUM > 2)
#define USART2_SEND_BUFFER_LEN 20
#define USART2_RECE_BUFFER_LEN 20
static unsigned char usart2SendBufferPool[USART2_SEND_BUFFER_LEN];
static unsigned char usart2ReceBufferPool[USART2_RECE_BUFFER_LEN];
static UsartBufferStruct usart2SendBuffer = {usart2SendBufferPool, USART2_SEND_BUFFER_LEN, 0, 0};
static UsartBufferStruct usart2ReceBuffer = {usart2ReceBufferPool, USART2_RECE_BUFFER_LEN, 0, 0};
#endif

static UsartStruct usarts[USART_SUM] =
    {
#if (USART_SUM)
        {
            Usart_0, // index
            0,       // isSending
            1,       // isReceving
            0,       // isSendFinished
            0,       // isReceiveFinished
            0,       // reserve
            0,       // noReceiveCounter
            0,
            &usart0SendBuffer, // pSendBuffer
            &usart0ReceBuffer  // pReceiveBuffer
        },
#endif
#if (USART_SUM > 1)
        {
            Usart_1, // index
            0,       // isSending
            1,       // isReceving
            0,       // isSendFinished
            0,       // isReceiveFinished
            0,       // reserve
            0,       // noReceiveCounter
            0,
            &usart1SendBuffer, // pSendBuffer
            &usart1ReceBuffer  // pReceiveBuffer
        },
#endif
#if (USART_SUM > 2)
        {
            Usart_0, // index
            0,       // isSending
            1,       // isReceving
            0,       // isSendFinished
            0,       // isReceiveFinished
            0,       // reserve
            0,       // noReceiveCounter
            0,
            &usart2SendBuffer, // pSendBuffer
            &usart2ReceBuffer  // pReceiveBuffer
        },
#endif
};
#if (USART_SUM)
void UART2_Handler(void)
{
    unsigned char receivedData = 0;
    // Transmit
    if (UART2->FR.BIT.TC)
    {
        UART2->FR.BIT.TCC = 1; // 清标志位
        if (usarts[Usart_0].pSendBuffer->counter >= usarts[Usart_0].pSendBuffer->usedLen)
        {
            usarts[Usart_0].isSending = 0;
            usarts[Usart_0].pSendBuffer->counter = 0;
            usarts[Usart_0].pSendBuffer->usedLen = 0;
        }
        else
        {

            UART2->TDR.BIT.TDR = *(usarts[Usart_0].pSendBuffer->pBuffer + usarts[Usart_0].pSendBuffer->counter);
            usarts[Usart_0].pSendBuffer->counter++;
        }
    }

    if (UART2->FR.BIT.RI)
    {
        receivedData = UART2->RDR.BIT.RDR; // RI只能通过对RDR读操作来清零  每次一定要读取缓存，取走数据
        usarts[Usart_0].noReceiveCounter = 0;

        if (usarts[Usart_0].isReceving == 0)
        {
            return;
        }

        if (usarts[Usart_0].pReceiveBuffer->counter >= usarts[Usart_0].pReceiveBuffer->bufferLen)
        {
            return;
        }

        *(usarts[Usart_0].pReceiveBuffer->pBuffer + usarts[Usart_0].pReceiveBuffer->counter) = receivedData;
        usarts[Usart_0].pReceiveBuffer->counter++;
    }
    //		// 判断是否发生接收溢出标志, // 或主判断
    if (UART2->FR.BIT.RXOV)
    {
        // 1： 要读取buf一下RDR	，清RDR缓存
        // 2:  要清接收溢出标志位
        UART_ReceiveData(UART2); // 读取一次接收缓存
        UART2->FR.BIT.RXOVC = 1; // 清接收溢出标志位
    }
}
#endif
#if (USART_SUM > 1)
void UART1_IRQHandler(void) interrupt 15
{
    unsigned char receivedData = 0;
    if (US1CON0 & 0x02)
    {
        US1CON0 &= 0xFD;

        if (usarts[Usart_1].pSendBuffer->counter >= usarts[Usart_1].pSendBuffer->usedLen)
        {
            usarts[Usart_1].isSending = 0;
            usarts[Usart_1].pSendBuffer->counter = 0;
            usarts[Usart_1].pSendBuffer->usedLen = 0;
        }
        else
        {
            US1CON3 = *(usarts[Usart_1].pSendBuffer->pBuffer + usarts[Usart_1].pSendBuffer->counter);
            usarts[Usart_1].pSendBuffer->counter++;
        }
    }

    if ((US1CON0 & 0x01))
    {
        US1CON0 &= 0xFE;
        receivedData = US1CON3;
        usarts[Usart_1].noReceiveCounter = 0;

        if (usarts[Usart_1].isReceving == 0)
        {
            return;
        }

        if (usarts[Usart_1].pReceiveBuffer->counter >= usarts[Usart_1].pReceiveBuffer->bufferLen)
        {
            return;
        }

        *(usarts[Usart_1].pReceiveBuffer->pBuffer + usarts[Usart_1].pReceiveBuffer->counter) = receivedData;
        usarts[Usart_1].pReceiveBuffer->counter++;
    }
}
#endif
#if (USART_SUM > 2)
void UART2_IRQHandler(void) interrupt 17
{
    unsigned char receivedData = 0;
    if (USXCON0 & 0x02) // 发送标志位判断
    {
        USXCON0 &= 0xFE;

        if (usarts[Usart_0].pSendBuffer->counter >= usarts[Usart_0].pSendBuffer->usedLen)
        {
            usarts[Usart_0].isSending = 0;
            usarts[Usart_0].pSendBuffer->counter = 0;
            usarts[Usart_0].pSendBuffer->usedLen = 0;
        }
        else
        {
            USXCON3 = *(usarts[Usart_0].pSendBuffer->pBuffer + usarts[Usart_0].pSendBuffer->counter);
            usarts[Usart_0].pSendBuffer->counter++;
        }
    }

    if ((USXCON0 & 0x01)) // 接收标志位判断
    {
        USXCON0 &= 0xFD;
        receivedData = USXCON3;
        usarts[Usart_0].noReceiveCounter = 0;

        if (usarts[Usart_0].isReceving == 0)
        {
            return;
        }

        if (usarts[Usart_0].pReceiveBuffer->counter >= usarts[Usart_0].pReceiveBuffer->bufferLen)
        {
            return;
        }

        *(usarts[Usart_0].pReceiveBuffer->pBuffer + usarts[Usart_0].pReceiveBuffer->counter) = receivedData;
        usarts[Usart_0].pReceiveBuffer->counter++;
    }
}
#endif

// 串口发送超时保护
void usartSendOverTimerProt(UsartIndex Index)
{
    if (usarts[Index].isSending)
        usarts[Index].usartSendOverTimerCnt++;
    else
        usarts[Index].usartSendOverTimerCnt = 0;
    if (usarts[Index].usartSendOverTimerCnt >= UART_SEND_OVER_TIME)
    {
        usarts[Index].isSending = 0;
        usarts[Index].usartSendOverTimerCnt = 0;
    }
}

// 1ms轮询一次
void usartCheckReceive(void)
{
    unsigned char i;

    for (i = 0; i < USART_SUM; i++)
    {
        usartSendOverTimerProt((UsartIndex)i);
        if (usarts[i].pReceiveBuffer->counter == 0)
        {
            usarts[i].isReceiveFinished = 0;
            usarts[i].noReceiveCounter = 0;
            continue;
        }

        if (++usarts[i].noReceiveCounter < USART_NO_RECEIVE_TIME_MAX)
        {
            continue;
        }

        usarts[i].noReceiveCounter = USART_NO_RECEIVE_TIME_MAX;
        usarts[i].isReceving = 0;
        usarts[i].isReceiveFinished = 1;
    }
}

void usartInit(void)
{
    unsigned char i = 0;
    unsigned char j = 0;

    unsigned char *pBufferTmp = 0;

    for (; i < sizeof(usarts) / sizeof(UsartStruct); i++)
    {
        pBufferTmp = usarts[i].pSendBuffer->pBuffer;
        for (j = 0; j < usarts[i].pSendBuffer->bufferLen; j++)
        {
            *(pBufferTmp + j) = 0;
        }

        pBufferTmp = usarts[i].pReceiveBuffer->pBuffer;
        for (j = 0; j < usarts[i].pReceiveBuffer->bufferLen; j++)
        {
            *(pBufferTmp + j) = 0;
        }
    }
}

UsartBufferStruct *usartGetReceive(UsartIndex index)
{
    if (index >= USART_SUM)
    {
        return 0;
    }

    if (usarts[index].isReceiveFinished)
    {
        return usarts[index].pReceiveBuffer;
    }
    return 0;
}

void usartClearReceive(UsartIndex Index)
{
    usarts[Index].pReceiveBuffer->counter = 0;
    usarts[Index].isReceving = 1;
    usarts[Index].isReceiveFinished = 0;
}

UsartBool usartSend(UsartIndex Index, unsigned char *pBuffer, unsigned char len)
{
    unsigned char i = 0;

    // if (usarts[Index].isSending || usarts[Index].pSendBuffer->bufferLen < len)
    if (usarts[Index].isSending)
    {
        return UsartFalse;
    }

    for (i = 0; i < len; i++)
    {
        *(usarts[Index].pSendBuffer->pBuffer + i) = *(pBuffer + i);
    }
    usarts[Index].pSendBuffer->usedLen = len;
    usarts[Index].pSendBuffer->counter = 1;
    
#if (USART_SUM)
    if (Index == Usart_0)
    {
        // 启动发送
        UART2->FR.BIT.TCC = 1; // 清标志位
        UART2->TDR.BIT.TDR = *(usarts[Index].pSendBuffer->pBuffer);
    }
#endif
#if (USART_SUM > 1)
    else if (Index == Usart_1)
        USCI1_UART_SendData8(*pOptionUsart->pSendBuffer->pBuffer);
#endif
#if (USART_SUM > 1)
    else if (Index == Usart_0)
        USXCON3 = *pOptionUsart->pSendBuffer->pBuffer;
#endif
    usarts[Index].isSending = 1;
    return UsartSucceed;
}
