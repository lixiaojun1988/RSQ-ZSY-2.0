#ifndef __MDL_FLASH_H_
#define __MDL_FLASH_H_

#define FLASH_ADDR_USER 0 // 扇区第0个字节开始写SN
typedef struct
{
	uint8_t u8Fa;

#if (1 == ADD_DBG_AA)
	uint8_t u8Aa;
#endif

	uint8_t u8Pl;
	uint8_t u8Dh;
	uint8_t u8Ph;

	uint8_t u8Fh;
	uint8_t u8Fl;
	uint8_t u8Hc;
	uint8_t u8Lc;

	uint8_t u8Sp;
	uint8_t u8Sl;
	uint8_t u8Ne;
	uint8_t u8Cl;
	uint8_t u8Co;
	uint8_t u8Ls;
	uint8_t u8Nf;
	uint8_t u8De;

    uint8_t u8Nh;//后清扫时间

	uint8_t u8Ns;
	uint8_t u8Ee;
    uint8_t u8Pu;
	uint8_t u8Null;
} ST_DBG_FLH_T;

typedef union mdl_flash
{
	unsigned char ucByte;
	struct
	{
		uint8_t bAdptDH		: 1;
		uint8_t bAdptHC		: 1;
		uint8_t bAdptPL		: 1;
	} bits;
} ST_ADAPT_FLH_T;

typedef struct
{
	uint32_t u32GasSum;
	uint32_t u32WaterSum;
	uint8_t u8TmpSet;
	uint8_t u8FluxSet;
	uint8_t u8WaterSet;
	uint8_t u8ModeSet;

	uint8_t u8HandEn;
	uint8_t u8HandMode;

    uint8_t u8RepairDh;
    uint8_t u8RepairE1;
    uint8_t u8RepairE8;

	uint8_t u8PowerOn;//开关机掉电记忆
} ST_BASIC_FLH_T; // e2p存储的数据

typedef struct
{
	uint8_t u8PreWarmTime; // 保温时间
	uint8_t u8PreWarmDif;  // 回差温度
	uint8_t u8CycleMode;   // 循环模式

	uint8_t au8Book24Set[6]; // 预约时间
	uint8_t u8MorWashStart;	 // 晨洗开始
	uint8_t u8MorWashEnd;	 // 晨洗结束
	uint8_t u8NigWashStart;	 // 夜浴开始
	uint8_t u8NigWashEnd;	 // 夜浴结束

	uint8_t u8WaterUSedTime;

	
} ST_PUMP_FLH_T;
// 写入数据必须是偶数
typedef struct
{
	unsigned char SafeCodebuff[4]; // 安全码
	unsigned char snCodebuff[9];   // SN码
	ST_DBG_FLH_T debugData;		   // 强制信息
	ST_ADAPT_FLH_T adaptData;	   // 自适应信息
} FlashDataCache0;				   // 扇区0缓存
// 写入数据必须是偶数
typedef struct
{
	ST_BASIC_FLH_T basicFlashData;
	ST_PUMP_FLH_T pumpData;
} FlashDataCache1; // 扇区1缓存

typedef enum
{
	EN_FLH_READ_SAFE_CODE,
	EN_FLH_READ_SN,
	EN_FLH_READ_DBG,
	EN_FLH_READ_USER,
	EN_FLH_READ_MACHINE,
	EN_FLH_READ_BASIC,
	EN_FLH_READ_PUMP,
	EN_FLH_READ_TYPE_MAX,
} EN_FLH_TYPE_T;

extern FlashDataCache0 FlashDataCache0_st;
extern FlashDataCache1 FlashDataCache1_st;
extern unsigned char WriteFlash(uint8_t Sector, uint8_t *pData, uint8_t length);
extern void ReadFlash(uint8_t Sector, uint8_t addr, uint8_t *pData, uint8_t length);
extern void SystemFlashInit(void);
void writeFlashSector0(void);
void writeFlashSector1(void);

extern FlashDataCache1 *GetFlashDataSector1(void);
extern FlashDataCache0 *GetFlashDataSector0(void);
#endif
