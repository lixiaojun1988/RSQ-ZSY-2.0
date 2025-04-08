
#ifndef _FIFO_H_
#define _FIFO_H_
/************************************************************************/
#define FIFO_CONFIG_ITEM_SIZE_MAX       20

typedef struct
{
    volatile unsigned char *pData ;
    volatile unsigned char usedSize ;
    
    volatile unsigned char writePersition ;
    volatile unsigned char readPersition ;
    
    volatile unsigned char size ;
    volatile unsigned char itemSize ;
}Fifo ;

#ifdef __cplusplus
extern C{
#endif

extern void fifoInit(volatile Fifo* pFifo,volatile void* pData,unsigned char size);
// 默认是写到 fifo 尾巴
extern bool fifoWrite(volatile Fifo* pFifo,unsigned char* pItem);
extern bool fifoRead(volatile Fifo* pFifo,unsigned char* pItem);
extern bool fifoReadNoDelete(volatile Fifo* pFifo,unsigned char* pItem);

extern bool fifoPeek(volatile Fifo* pFifo,void* pItem);
extern bool fifoPeekTail(volatile Fifo * pFifo,void * pItem) ;

extern bool fifoWriteFront(volatile Fifo* pFifo,unsigned char* pItem);

extern bool fifoWriteNoRepeat(volatile Fifo* pFifo,unsigned char* pItem);
extern bool fifoWriteFrontNoRepeat(volatile Fifo* pFifo,unsigned char* pItem);

#ifdef __cplusplus
}
#endif




#endif
