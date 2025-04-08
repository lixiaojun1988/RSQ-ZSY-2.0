#ifndef	MDL_DS1302_H
#define	MDL_DS1302_H
typedef enum
{
    EN_DS_SEC,
    EN_DS_MIN,
    EN_DS_HOUR,
    EN_DS_CTL,
    EN_DS_MAX
}EN_DS_T;
extern uint8_t DS1302_TimeRead(EN_DS_T _enAddr);
extern uint8_t DS1302_Read(EN_DS_T _enAddr);
extern void DS1302_TimeWrite(uint8_t _Min,uint8_t _Hour);
extern void DS1302_TimeInit(void);
#endif
