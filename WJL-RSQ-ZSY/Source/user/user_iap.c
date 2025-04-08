/******************************************************FlashDataCache*******************
 * Copyright: .
 * File:
 * Layer:
 * Author:   xjli
 * Version:  V1.0
 * Decription:
 *************************************************************************/

#include "all_head.h"
FlashDataCache idata FlashDataCache_t;
FlashDataCache *FlashDataGet(void)
{
  return &FlashDataCache_t;
}
uint8_t idata FlashDataCache_temp[sizeof(FlashDataCache_t)];
void user_iap_write(void)
{
  uint8_t i;
  int n;
  for (i = 0; i < 5; i++) // 连续写5次
  {
    FEED_WATCH_DOG();
    IAP_SectorErase(IAP_EEPROM, EEPROM_START_ADDR); // 扇区擦除
    IAP_E2ProgramNByte(EEPROM_START_ADDR, (unsigned char *)&FlashDataCache_t, sizeof(FlashDataCache_t));
    IAP_E2ReadNByte(EEPROM_START_ADDR, FlashDataCache_temp, sizeof(FlashDataCache_t));
    n = memcmp(FlashDataCache_temp, (unsigned char *)&FlashDataCache_t, sizeof(FlashDataCache_t));
    if (n == 0)
    {
      break;
    }
  }
}
void user_iap_init(void)
{
  memset(&FlashDataCache_t, 0, sizeof(FlashDataCache_t));
  IAP_E2ReadNByte(EEPROM_START_ADDR, (unsigned char *)&FlashDataCache_t, sizeof(FlashDataCache_t));
  if (FlashDataCache_t.First_write_Flag != FIRST_WRITE_FLAG) // 首次上电
  {
    memset(&FlashDataCache_t, 0, sizeof(FlashDataCache_t));//清空缓存数据
    FlashDataCache_t.First_write_Flag = FIRST_WRITE_FLAG;
    user_iap_write();
  }
}
