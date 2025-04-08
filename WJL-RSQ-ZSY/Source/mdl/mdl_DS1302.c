#include "ALL_HEAD.H"


extern void GPIO_OutL(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
extern void GPIO_InPU(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#define IO_RST_1 GPIO_SetBits(GPIOA, GPIO_Pin_8)   // pa8
#define IO_RST_0 GPIO_ResetBits(GPIOA, GPIO_Pin_8) 
#define IO_DIO_1 GPIO_SetBits(GPIOA, GPIO_Pin_7)   // pa7
#define IO_DIO_0 GPIO_ResetBits(GPIOA, GPIO_Pin_7) 
#define IO_CLK_1 GPIO_SetBits(GPIOA, GPIO_Pin_6)   // pa6
#define IO_CLK_0 GPIO_ResetBits(GPIOA, GPIO_Pin_6) 
#define IO_DIO_OUT GPIO_OutL(GPIOA, GPIO_Pin_7)
#define IO_DIO_IN GPIO_InPU(GPIOA, GPIO_Pin_7)
#define IO_DIO_READ GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)

#define DS1302_SEC_ADDR 0x80
#define DS1302_MIN_ADDR 0x82
#define DS1302_HOUR_ADDR 0x84
#define DS1302_CONTROL_ADDR 0x8E




void	delay_nus(uint32_t nus)
{
	unsigned char i = 2;

	for (; nus > 0; nus--)
	{
		for (; i > 0; i--)
			;
	}
}
uint8_t au8DS_Time[3];
const uint8_t au8DS1302Adr[EN_DS_MAX] =
    {
        0x80,
        0x82,
        0x84,
        0x8E};

uint8_t DS1302_Read(EN_DS_T _enAddr)
{
    uint8_t i;
    uint8_t _u8Data = 0; // 锟斤拷锟斤拷
    uint8_t _u8SendData;
    _u8SendData = au8DS1302Adr[_enAddr];
    _u8SendData |= 0x01;
    IO_RST_0;
    delay_nus(1);
    IO_CLK_0;
    delay_nus(1);
    IO_RST_1;
    delay_nus(1);
    for (i = 0; i < 8; i++)
    {
        if (_u8SendData & 0x01)
            IO_DIO_1;
        else
            IO_DIO_0;
        _u8SendData >>= 1;
        delay_nus(1);
        IO_CLK_1;
        delay_nus(1);
        IO_CLK_0;
        delay_nus(1);
    }
    IO_DIO_IN;
    delay_nus(1);
    for (i = 0; i < 8; i++)
    {
        _u8Data >>= 1;
        if (IO_DIO_READ)
        {
            _u8Data |= 0x80;
        }
        IO_CLK_1;
        delay_nus(1);
        IO_CLK_0;
        delay_nus(1);
    }
    IO_DIO_OUT;
    IO_RST_0;
    delay_nus(1);
    IO_DIO_0;
    delay_nus(1);
    IO_DIO_1;
    return _u8Data;
}

void DS1302_Write(EN_DS_T _enAddr, uint8_t Data)
{
    unsigned char i;
    uint8_t _u8Addr;
    _u8Addr = au8DS1302Adr[_enAddr];
    IO_RST_0;
    delay_nus(1);
    IO_CLK_0;
    delay_nus(1);
    IO_RST_1;
    for (i = 0; i < 8; i++)
    {
        if (_u8Addr & 0x01)
            IO_DIO_1;
        else
            IO_DIO_0;
        _u8Addr >>= 1;
        delay_nus(1);
        IO_CLK_1;
        delay_nus(1);
        IO_CLK_0;
        delay_nus(1);
    }
    for (i = 0; i < 8; i++)
    {
        if (Data & 0x01)
            IO_DIO_1;
        else
            IO_DIO_0;
        Data >>= 1;
        delay_nus(1);
        IO_CLK_1;
        delay_nus(1);
        IO_CLK_0;
        delay_nus(1);
    }
    IO_RST_0;
}

void DS1302_TimeWrite(uint8_t _Min, uint8_t _Hour)
{
    uint8_t _u8MinData;
    uint8_t _u8HourData;
    _u8MinData = (_Min % 10) + ((_Min / 10) << 4);
    _u8HourData = (_Hour % 10) + ((_Hour / 10) << 4);
    DS1302_Write(EN_DS_CTL, 0x00);
    DS1302_Write(EN_DS_SEC, 0);
    DS1302_Write(EN_DS_MIN, _u8MinData);
    DS1302_Write(EN_DS_HOUR, _u8HourData);
    DS1302_Write(EN_DS_CTL, 0x80);
}

void DS1302_TimeInit(void)
{
    uint8_t _u8Sec;
    uint8_t _u8Hour;
    _u8Sec = DS1302_Read(EN_DS_SEC);
    _u8Hour = DS1302_Read(EN_DS_HOUR);
    if ((0 < (_u8Sec & 0x80)) || (0 < (_u8Hour & 0x80)))
    {
        _u8Sec &= 0x7F;
        _u8Hour &= 0x7F;
        DS1302_Write(EN_DS_CTL, 0x00);
        DS1302_Write(EN_DS_SEC, _u8Sec);
        DS1302_Write(EN_DS_HOUR, _u8Hour);
        DS1302_Write(EN_DS_CTL, 0x80);
    }
}

uint8_t DS1302_TimeRead(EN_DS_T _enAddr)
{
    uint8_t _u8temp;
    _u8temp = DS1302_Read(_enAddr);
    _u8temp = (_u8temp >> 4) * 10 + (_u8temp & 0x0F);
    if ((EN_DS_SEC == _enAddr) || (EN_DS_MIN == _enAddr))
    {
        if (60 <= _u8temp)
        {
            _u8temp = au8DS_Time[_enAddr];
        }
        else
        {
            au8DS_Time[_enAddr] = _u8temp;
        }
    }
    else if (EN_DS_HOUR == _enAddr)
    {
        if (24 <= _u8temp)
        {
            _u8temp = au8DS_Time[_enAddr];
        }
        else
        {
            au8DS_Time[_enAddr] = _u8temp;
        }
    }
    return (_u8temp);
}
