
#ifndef _MDL_FILTER_H_
#define _MDL_FILTER_H_

typedef struct
{
	uint16_t *pBuff;
	uint8_t resultSuccessFlag;
	uint8_t midCount;
	uint8_t avgCount;
	uint16_t result;
	uint32_t ValueSum;
} MidFilterTypedef;

extern void mdlFilterhandle(void);
extern MidFilterTypedef* getloadMidFilterResult(void);
#endif
