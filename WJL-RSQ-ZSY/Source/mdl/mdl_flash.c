/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:         adc.c
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:
 *************************************************************************/
#include "all_head.h"
const uint8_t E2SafeCode[4] = {0xaa, 0x55, 0x12, 0x34};
FlashDataCache0 FlashDataCache0_st;
FlashDataCache1 FlashDataCache1_st;

FlashDataCache1 *GetFlashDataSector1(void)
{
	return &FlashDataCache1_st;
}

FlashDataCache0 *GetFlashDataSector0(void)
{
	return &FlashDataCache0_st;
}
extern const ST_DBG_FLH_T stDbgFlhDef;
extern void E2EraseSector(E2_SECTOR_Num_TypeDef sector);
extern uint8_t E2WriteBuf(E2_SECTOR_Num_TypeDef E2Sector, uint32_t addr, uint32_t len, uint8_t *buf);
const uint8_t ReadFlashAddrTab0[EN_FLH_READ_TYPE_MAX] =
	{
		FLASH_ADDR_USER,
		FLASH_ADDR_USER + sizeof(FlashDataCache0_st.SafeCodebuff),
		FLASH_ADDR_USER + sizeof(FlashDataCache0_st.SafeCodebuff) + sizeof(FlashDataCache0_st.snCodebuff),
};

unsigned char WriteFlash(uint8_t Sector, uint8_t *pData, uint8_t length)
{
	uint8_t WriteFlashBufferTemp[100];
	E2EraseSector((E2_SECTOR_Num_TypeDef)Sector);
	if ((length / 2) != 0) // 必须写入偶数个
	{
		if (length >= 100)
			return 0;
		memcpy(&WriteFlashBufferTemp, pData, length);
		length++;
	}
	E2WriteBuf((E2_SECTOR_Num_TypeDef)Sector, FLASH_ADDR_USER, length, pData);
	return 0;
}
void ReadFlash(uint8_t Sector, uint8_t addr, uint8_t *pData, uint8_t length)
{
	if (Sector == 0)
	{
		FLASH_E2_ReadByte((E2_SECTOR_Num_TypeDef)Sector, ReadFlashAddrTab0[addr], length, pData);
	}
	else
	{
        FLASH_E2_ReadByte((E2_SECTOR_Num_TypeDef)Sector, ReadFlashAddrTab0[addr], length, pData);
	}
}
void SystemFlashInit(void)
{
	uint8_t temp = 0;
	ReadFlash(EEPROM_Sector_0, FLASH_ADDR_USER, (uint8_t *)&FlashDataCache0_st, sizeof(FlashDataCache0_st)); //

	if (0 != memcmp(&FlashDataCache0_st.SafeCodebuff, &E2SafeCode, sizeof(FlashDataCache0_st.SafeCodebuff))) // 不等于安全码，系统第一次上电
	{
		// 将安全码写入E2P
		memcpy(&FlashDataCache0_st.SafeCodebuff, &E2SafeCode, sizeof(FlashDataCache0_st.SafeCodebuff));
		// 初始化默认配置值
		memcpy(&FlashDataCache0_st.debugData, &stDbgFlhDef, sizeof(stDbgFlhDef));
		// 再次写入到E2p
		WriteFlash(EEPROM_Sector_0, (uint8_t *)&FlashDataCache0_st, sizeof(FlashDataCache0_st));

		// 初始化用户数据默认配置值
		memset(&FlashDataCache1_st, 0, sizeof(FlashDataCache1_st));
		FlashDataCache1_st.basicFlashData.u8TmpSet = COMFORT_TEMP_DEF; // 默认设定温度41度
		// 再次写入到E2p
		WriteFlash(EEPROM_Sector_1, (uint8_t *)&FlashDataCache1_st, sizeof(FlashDataCache1_st));
	}
	else // 已经写入过数据
	{
		/*
		// 对读出的强制代码数据进行有效性校验，恢复默认值
		if (FlashDataCache0_st.debugData.u8Fa)
		{
			FlashDataCache0_st.debugData.u8Fa =
				temp = 1;
		}
		if (FlashDataCache0_st.debugData.u8Pl<&&FlashDataCache0_st.debugData.u8Pl>)
		{
			FlashDataCache0_st.debugData.u8Pl =
				temp = 1;
		}
		if (FlashDataCache0_st.debugData.u8Ph<&&FlashDataCache0_st.debugData.u8Ph>)
		{
			FlashDataCache0_st.debugData.u8Ph =
				temp = 1;
		}
		if (FlashDataCache0_st.debugData.u8Fh<&&FlashDataCache0_st.debugData.u8Fh>)
		{
			FlashDataCache0_st.debugData.u8Fh =
				temp = 1;
		}
		if (FlashDataCache0_st.debugData.u8Fl<&&FlashDataCache0_st.debugData.u8Fl>)
		{
			FlashDataCache0_st.debugData.u8Fl =
				temp = 1;
		}
		if (FlashDataCache0_st.debugData.u8Dh<&&FlashDataCache0_st.debugData.u8Dh>)
		{
			FlashDataCache0_st.debugData.u8Dh =
				temp = 1;
		}*/
		// 再写一次E2P
		if (temp)
			WriteFlash(EEPROM_Sector_0, (uint8_t *)&FlashDataCache0_st, sizeof(FlashDataCache0_st));

		// 读取用户数据
		ReadFlash(EEPROM_Sector_1, FLASH_ADDR_USER, (uint8_t *)&FlashDataCache1_st, sizeof(FlashDataCache1_st)); //
	   // 对读出的用户数据进行有效性校验，恢复默认值

		if (temp)
			WriteFlash(EEPROM_Sector_1, (uint8_t *)&FlashDataCache1_st, sizeof(FlashDataCache1_st));
		if (FlashDataCache1_st.basicFlashData.u8TmpSet == 0)
			FlashDataCache1_st.basicFlashData.u8TmpSet = COMFORT_TEMP_DEF;
	}
	// 将flash数据写入到系统变量
	GetCommSet()->u8ModeSet = GetFlashDataSector1()->basicFlashData.u8ModeSet;
	GetCommSet()->u8TmpSet = GetFlashDataSector1()->basicFlashData.u8TmpSet;
	GetCommSet()->u8FluxSet = GetFlashDataSector1()->basicFlashData.u8FluxSet;
	GetCommSet()->u8WaterSet = GetFlashDataSector1()->basicFlashData.u8WaterSet;
    GetCommSet()->u8OnOffSet = GetFlashDataSector1()->basicFlashData.u8PowerOn;
}

void writeFlashSector0(void)
{
	WriteFlash(EEPROM_Sector_0, (uint8_t *)&FlashDataCache0_st, sizeof(FlashDataCache0_st));
}

void writeFlashSector1(void)
{
	WriteFlash(EEPROM_Sector_1, (uint8_t *)&FlashDataCache1_st, sizeof(FlashDataCache1_st));
}
