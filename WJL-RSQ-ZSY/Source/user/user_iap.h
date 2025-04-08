#ifndef __USER_IAP_H__
#define __USER_IAP_H__
#define FIRST_WRITE_FLAG 0x55AA1234
typedef struct
{
    uint32_t First_write_Flag;
    uint16_t  fanRunTimeSecTotal;//电机累计运行时间 秒
    uint8_t   gestureFuncEnable;//为0，默认开启手势，为1关闭手势
} FlashDataCache;
extern FlashDataCache *FlashDataGet(void);
extern void user_iap_init(void);
extern void user_iap_write(void);



#endif



