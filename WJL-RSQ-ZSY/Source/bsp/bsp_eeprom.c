/**
  ******************************************************************************
  * @file    eeprom.c
  * @author
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "sh30f9010.h"
#include "bsp.h"

#define EA_DIS()		 EA_Disable()	//关闭总中断
#define EA_EN() 		 EA_Enable()	   //开启总中断


// 如果一次写入长度太长，建议分段写入，避免关中断时间过长
#define     E_WRITE_ONE_SHOT_LEN        (20)    //一段最多写入字节数定义，建议大于16   E2


/**
  * @brief   擦除EEPROM指定扇区
  * @param   sector : 要擦除的扇区：0~3. 4K E2每个扇区1K
  * @retval  void
  */
void    E2EraseSector(E2_SECTOR_Num_TypeDef sector) 
{

		/* 开始擦除 */
		EA_DIS();                    // 关中断
		FLASH_E2_Unlock(SINGLE_OPERATION);
		FLASH_E2_EraseSector(sector);
		FLASH_E2_Lock();
		EA_EN();                     // 使能中断
}



/**
  * @brief   根据起始地址读指定长度字节的E2内容
  * @param  e2psector 扇区
  * @param   StartAddr : 起始地址
  * @param   Length  ：要获取的数据长度
  * @param   pBuf   ：读取到的E2内容
  * @retval  void
  */
////库函数
//void FLASH_E2_ReadByte(E2_SECTOR_NUM_Type e2psector, uint32_t StartAddr,
//                       uint32_t Length, uint8_t *pBuf)

/**
  * @brief   根据起始地址写入指定长度字节的E2内容
  * @param   addr : 起始地址，2的倍数
  * @param   len  ：待写入的数据长度, len要是2的倍数   注意!!!!!!!!!!!!!!!
  * @param   buf  ：待写入的E2内容
  * @retval  U8 : result操作结果 : 0-写入失败；   1：写入成功
  */
//FLASH_E2_ProgramPage_HalfWord(E2Sector, addr, oneShotLen, buf); //库函数

// 数据多的情况分次写，，避免关中断时间过长
uint8_t E2WriteBuf(E2_SECTOR_Num_TypeDef E2Sector,  uint32_t addr, uint32_t len, uint8_t *buf)
{
    uint8_t     oneShotLen;     /* 单次写入的字节长度 */
		FLASH_Status status = FLASH_COMPLETE;
    
    //len要是2的倍数   addr : 起始地址，2的倍数    注意!!!!!!!!!!!!!!!
    while( len )
    {

        /* 不要一次性写入较长的字节，否则会因为关中断过长 */
        if( len >= E_WRITE_ONE_SHOT_LEN )
        {
            oneShotLen  = E_WRITE_ONE_SHOT_LEN;
            len        -= E_WRITE_ONE_SHOT_LEN;
        }
        else
        {
            oneShotLen = len;
            len = 0;
        }
		////E2 /* 写入 */
        EA_DIS();                // 关中断
				FLASH_E2_Unlock(MULTI_OPERATION);
        status =FLASH_E2_ProgramPage_HalfWord(E2Sector, addr, oneShotLen, buf);
				FLASH_E2_Lock(); 
        EA_EN();                 // 使能中断

				addr += oneShotLen;
				buf += oneShotLen;			
				if(status != FLASH_COMPLETE)
				{
						return 0;//error
				}
    }

   if(status == FLASH_COMPLETE)
	 {
			return  1;                     /* 成功 */
	 }
	 else
	 {
			return 0;//error
	 }
}


// 测试 demo----------------------

#define E2_len_W 32
void E2_test(void)
{
		uint8_t i;
		uint8_t buf[E2_len_W];
		uint8_t Readbuf[E2_len_W];
		for(i=0; i<E2_len_W; i++)
		{
				buf[i]=i+1;
				Readbuf[i] =0;
		}
		E2EraseSector(EEPROM_Sector_0);
		
		E2WriteBuf(EEPROM_Sector_0,0,E2_len_W,buf);
		E2WriteBuf(EEPROM_Sector_0,32,E2_len_W,buf);
		
		
		FLASH_E2_ReadByte(EEPROM_Sector_0, 0, E2_len_W,Readbuf);
		__nop();
		__nop();
		
		FLASH_E2_ReadByte(EEPROM_Sector_0, 32, E2_len_W,Readbuf);
		__nop();
		__nop();
		E2EraseSector(EEPROM_Sector_0);
		__nop();
		__nop();
		FLASH_E2_ReadByte(EEPROM_Sector_0, 0, E2_len_W,Readbuf);
		__nop();
		__nop();
		
		FLASH_E2_ReadByte(EEPROM_Sector_0, 32, E2_len_W,Readbuf);
		__nop();
		__nop();
		
//		//扇區1
//		E2EraseSector(1);
//		E2WriteBuf(1,0,E2_len_W,buf);
//		E2WriteBuf(1,32,E2_len_W,buf);
//		
//		FLASH_E2_ReadByte(1, 0, E2_len_W,Readbuf);
//		__nop();
//		__nop();
//		
//		FLASH_E2_ReadByte(1, 32, E2_len_W,Readbuf);
//		__nop();
//		__nop();
//		E2EraseSector(1);
//		FLASH_E2_ReadByte(1, 0, E2_len_W,Readbuf);
//		__nop();
//		__nop();
//		
//		FLASH_E2_ReadByte(1, 32, E2_len_W,Readbuf);
//		__nop();
//		__nop(); 
}

// 测试 demo----------------------
/*----------------------------------------- E.N.D -----------------------------------------*/
