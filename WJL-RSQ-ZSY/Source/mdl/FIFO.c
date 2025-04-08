
#include "all_head.h"
/************************************************************************/

void fifoInit(volatile Fifo* pFifo,volatile void* pData,unsigned char size)
{
    pFifo->pData = pData ;
    pFifo->size = size ;
    pFifo->usedSize = 0 ;
    pFifo->itemSize = 1 ;
    pFifo->readPersition = 0 ;
    pFifo->writePersition = 0 ;
}

bool fifoWrite(volatile Fifo* pFifo,unsigned char * pItem)
{
    if(pFifo->usedSize >= pFifo->size)
    {
        return False ;
    }

    pFifo->usedSize++ ;
    *(pFifo->pData + pFifo->writePersition) = *(pItem) ;
    ++pFifo->writePersition;
    if(pFifo->writePersition >= pFifo->size)
    {
        pFifo->writePersition = 0 ;
    }

    return True ;
}


bool fifoRead(volatile Fifo* pFifo,unsigned char* pItem)
{
    if(pFifo->usedSize == 0)
    {
        return False ;
    }

    pFifo->usedSize -- ;
    *(pItem) =  *(pFifo->pData + pFifo->readPersition) ;
    pFifo->readPersition++;
    if(pFifo->readPersition >= pFifo->size)
    {
        pFifo->readPersition = 0 ;
    }
    return True ;    
}

bool fifoReadNoDelete(volatile Fifo* pFifo,unsigned char* pItem)
{
    if(pFifo->usedSize == 0)
    {
        return False ;
    }

    *(pItem) =  *(pFifo->pData + pFifo->readPersition) ;
    return True ;    
}

bool fifoPeek(volatile Fifo * pFifo, void * pItem)
{
    unsigned char i = 0 ;
    
    if(0 == pFifo->usedSize)
    {
        return False ;
    }
    
    for(i=0;i<pFifo->itemSize;i++)
    {
        *((char*)pItem+i) =  *(pFifo->pData + pFifo->readPersition + i) ;
    }

    return True ;   
}

bool fifoPeekTail(volatile Fifo * pFifo,void * pItem)
{
    unsigned char i = 0 ;
    unsigned char readPersition = pFifo->writePersition ;

    if(0 == pFifo->usedSize)
    {
        return False ;
    }

    if(readPersition > 0)
    {
        readPersition-- ;
    }else{
        readPersition = pFifo->size - 1 ;
    }

    for(i=0;i<pFifo->itemSize;i++)
    {
        *((char*)pItem+i) =  *(pFifo->pData + readPersition + i) ;
    }
    return True ;
}

bool fifoWriteFront(volatile Fifo* pFifo,unsigned char* pItem)
{
    if(pFifo->usedSize >= pFifo->size)
    {
        return False ;
    }

    pFifo->usedSize++ ;
    if(pFifo->readPersition > 0)
    {
        pFifo->readPersition-- ;
    }else{
        pFifo->readPersition = pFifo->size - 1 ;
    }
    
    *(pFifo->pData + pFifo->readPersition) = *(pItem) ;
    return True ;
}

bool fifoWriteNoRepeat(volatile Fifo* pFifo,unsigned char* pItem)
{
    unsigned char itemTmp[FIFO_CONFIG_ITEM_SIZE_MAX] = {0} ;
    if(False == fifoPeekTail(pFifo, itemTmp))
    {
        fifoWrite(pFifo, pItem) ;
        return True ;
    }

    for(int i = 0 ;i<pFifo->itemSize;i++)
    {
        if(itemTmp[i] != *((unsigned char*)pItem + i))
        {
            fifoWrite(pFifo, pItem) ;
            return True ;
        }
    }

    return False ;    
}


bool fifoWriteFrontNoRepeat(volatile Fifo* pFifo,unsigned char* pItem)
{
    unsigned char itemTmp[FIFO_CONFIG_ITEM_SIZE_MAX] = {0} ;
    if(False == fifoPeek(pFifo, itemTmp))
    {
        fifoWrite(pFifo, pItem) ;
        return True ;
    }

    for(int i = 0 ;i<pFifo->itemSize;i++)
    {
        if(itemTmp[i] != *((unsigned char*)pItem + i))
        {
            fifoWriteFront(pFifo, pItem) ;
            return True ;
        }
    }    
    return False ;    
}


