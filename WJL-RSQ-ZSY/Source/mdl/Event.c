#include "all_head.h"
/************************************************************************/
#define EVENT_COMM_QUEUE_LENGTH 50
/************************************************************************/
static uint8_t eEventComm[EVENT_COMM_QUEUE_LENGTH] = {0};
static FIFO EventCommQueue;
/************************************************************************/
void EventComm_Init(void)
{
	FIFO_sInit(&EventCommQueue, (uint8_t*)&eEventComm, EVENT_COMM_QUEUE_LENGTH);
}

void TriggerCommEvent(uint8_t Event)//
{
	FIFO_sWrite(&EventCommQueue, Event);
}

uint8_t GetCommEvent(void)
{
	return FIFO_sRead(&EventCommQueue);
}


