#ifndef __DRV_BSP_IAP_H
#define	__DRV_BSP_IAP_H
#define EEPROM_START_ADDR 0X0000
#define IAP_APROM 0x00
#define IAP_UniqueID 0x01
#define IAP_EEPROM 0x02
#define IAP_LDROM 0x03 // 只有APROM和EEPROM允许IAP写操作
extern void IAP_SectorErase(unsigned char IAP_MemType, unsigned long IAP_SectorEraseAddress);
extern void IAP_E2ProgramNByte(unsigned long Address, unsigned char *Data, unsigned char len);
extern void IAP_E2ReadNByte(unsigned long IAP_IapAddr, unsigned char *Data, unsigned char len);

#endif
