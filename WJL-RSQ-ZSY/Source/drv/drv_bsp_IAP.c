
#include "all_head.h"

/**************************************************
 *函数名称：unsigned char IAP_Read(unsigned long Add,unsigned char ADER)
 *函数功能：单Byte读取
 *入口参数：Add ：地址
 *         Data：要写入的Byte，ADER:操作对象 APROM为00，EEPROM为02
 *出口参数：void
 **************************************************/
unsigned char IAP_Read(unsigned long Add, unsigned char Iapade)
{
    unsigned char xdata IAP_IapData;
    unsigned char code *point = 0;
    // 保存IAPADE、EA
    unsigned char tempADER = IAPADE;
    unsigned char tempEA = EA;

    EA = 0; // 关闭中断

    IAPADE = Iapade;
    IAP_IapData = *(point + Add);

    // 恢复IAPADE、EA，避免MOVC位置出错
    IAPADE = tempADER;
    EA = tempEA;
    return IAP_IapData;
}
void IAP_SectorErase(unsigned char IAP_MemType, unsigned long IAP_SectorEraseAddress)
{
    unsigned char tempADER = IAPADE;
    bit temp = EA;
    EA = 0;
    IAPADE = IAP_MemType;
    IAPADH = (unsigned char)(IAP_SectorEraseAddress >> 8); // 擦除IAP目标地址高位值
    IAPADL = (unsigned char)IAP_SectorEraseAddress;        // 擦除IAP目标地址低位值
    IAPKEY = 0x40;
    IAPCTL = 0x20;
    IAPCTL |= 0x02;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    IAPADE = tempADER;
    EA = temp;
}

void IAP_ProgramByte(unsigned long Address, unsigned char Data, unsigned char IAP_MemType)
{
  unsigned char tempADER = IAPADE;
  bit temp = EA;
  EA = 0;
  IAPADE = IAP_MemType;
  IAPDAT = Data;
  IAPADH = (unsigned char)(Address >> 8);
  IAPADL = (unsigned char)Address;
  IAPKEY = 0x40;
  IAPCTL = 0x10;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  IAPADE = tempADER;
  EA = temp;
}
void IAP_E2ProgramNByte(unsigned long Address, unsigned char *Data, unsigned char len)
{
    unsigned char i;
    for (i = 0; i < len; i++)
    {
        IAP_ProgramByte(Address + i, *(Data + i),IAP_EEPROM);//对E2Prom区域进行写操作
    }
}

void IAP_E2ReadNByte(unsigned long IAP_IapAddr, unsigned char *Data, unsigned char len)
{
    unsigned char i;
    for (i = 0; i < len; i++)
    {
        *(Data + i) = IAP_Read(IAP_IapAddr + i, IAP_EEPROM);
    }
}
