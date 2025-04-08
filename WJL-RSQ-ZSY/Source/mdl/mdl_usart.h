
#ifndef _MDL_USART_H_
#define _MDL_USART_H_

typedef enum
{
    UsartSucceed = 0,
    UsartFalse,
} UsartBool;

typedef enum
{
    Usart_0,
    UsartSum,
} UsartIndex;

typedef struct
{
    unsigned char *pBuffer;
    unsigned int bufferLen;
    unsigned int usedLen;
    unsigned int counter;
} UsartBufferStruct;

extern void usartInit(void);
extern UsartBool usartSend(UsartIndex Index, unsigned char *pBuffer, unsigned char len);
extern UsartBufferStruct  *usartGetReceive(UsartIndex Index);
extern void usartClearReceive(UsartIndex Index);
extern void usartCheckReceive(void);
//extern unsigned char usartIsSending(UsartIndex index);


#endif
