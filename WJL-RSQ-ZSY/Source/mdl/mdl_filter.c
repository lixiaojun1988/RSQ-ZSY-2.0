/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:         adc.c
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:      滤波处理
 *************************************************************************/
#include "all_head.h"

// 中位值滤波 5次中值+3次平均
#define FILTER_LOAD_VALUE_MID 3
#define FILTER_LOAD_VALUE_AVG 3

static uint16_t LoadMidFilterBuffer[FILTER_LOAD_VALUE_MID];
static MidFilterTypedef loadMidFilter = {LoadMidFilterBuffer, 0, 0, 0, 0, 0};
//
// 中值滤波处理
void midFilterhandle(MidFilterTypedef *pFilter, uint16_t OrgValue, uint8_t midCt, uint8_t avgCt)
{
	unsigned char x, y;
	uint16_t result = 0xffff;
	uint16_t temp;
	// 中位值次数
	if (pFilter->midCount < (midCt - 1))
	{
		pFilter->pBuff[pFilter->midCount] = OrgValue;
		pFilter->midCount++;
	}
	else
	{
		pFilter->pBuff[pFilter->midCount] = OrgValue;
		// 冒泡排序
		for (x = 0; x < midCt - 1; x++)
		{
			for (y = 0; y < midCt - 1 - x; y++)
			{
				// 交换元素位置
				if (pFilter->pBuff[y] > pFilter->pBuff[y + 1])
				{
					temp = pFilter->pBuff[y];
					pFilter->pBuff[y] = pFilter->pBuff[y + 1];
					pFilter->pBuff[y + 1] = temp;
				}
			}
		}
		pFilter->midCount = 0;
		// 取中值
		if (midCt % 2)
			pFilter->ValueSum += pFilter->pBuff[midCt / 2 + 1];
		else
			pFilter->ValueSum += pFilter->pBuff[midCt / 2];
		if (++pFilter->avgCount >= avgCt)
		{
			// 求平均
			result = pFilter->ValueSum / avgCt;
			// 还原参数
			pFilter->ValueSum = 0;
			pFilter->avgCount = 0;
		}
	}
	if (result != 0xffff)
	{
		pFilter->resultSuccessFlag = 1;
		pFilter->result = result;
	}
}

MidFilterTypedef *getloadMidFilterResult(void)
{
	return &loadMidFilter;
}

void mdlFilterhandle(void) // 10MS
{
	static uint8_t mdlFilterTick;
	if(++mdlFilterTick<3)return;
	mdlFilterTick=0;
	midFilterhandle(&loadMidFilter, GetTmpCtrlData()->u16CalcLoad, FILTER_LOAD_VALUE_MID, FILTER_LOAD_VALUE_AVG);
}
