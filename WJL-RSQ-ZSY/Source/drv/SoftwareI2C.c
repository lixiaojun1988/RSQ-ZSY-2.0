/*******************************************************************************
* Copyright (C), 2000-2016,  Electronic Technology Co., Ltd.
* FileName: SoftwareI2C.c
* Author: Li.wen
* Version:
* Date: 2016-1-8     
* Description: MCU IO controlled I2C program
* History:       
*    1. Date: 2016-1-8
*       Author: Li.wen
*       Modification: create
*    2.
* Others:           //
*******************************************************************************/


#include	"ALL_HEAD.H"

extern void GPIO_OutL(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
extern void GPIO_OutOD(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
extern void GPIO_InHZ(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#define HB_CLK_OUT GPIO_OutL(GPIOA, GPIO_Pin_13)
#define HB_CLK_1 GPIO_SetBits(GPIOA, GPIO_Pin_13)   // pa13
#define HB_CLK_0 GPIO_ResetBits(GPIOA, GPIO_Pin_13) 

#define HB_DIO_1 GPIO_SetBits(GPIOA, GPIO_Pin_14)   // pa14
#define HB_DIO_0 GPIO_ResetBits(GPIOA, GPIO_Pin_14) 
#define HB_DIO_OUT GPIO_OutOD(GPIOA, GPIO_Pin_14)
#define HB_DIO_IN GPIO_InHZ(GPIOA, GPIO_Pin_14)
#define HB_DIO_READ GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14)


#define DelayTick 5

void SoftI2C_delay(uint8_t i)
{
    while(i--);
}

void SoftI2C_stop(void)
{
    HB_CLK_1;
    SoftI2C_delay(DelayTick);
    HB_DIO_1;
    SoftI2C_delay(DelayTick);
}
void SoftI2C_start(void)
{
    HB_CLK_OUT;
    HB_DIO_OUT;
    HB_DIO_1;
    SoftI2C_delay(DelayTick);
    HB_CLK_1;
    HB_DIO_0;
    SoftI2C_delay(DelayTick);
    HB_CLK_0;
}
void SoftI2C_write_byte(uint8_t value)
{
    uint8_t i;
   // uint8_t result = 0;
    i = 8;
    while (i)
    {
        if (value & 0x80)
            HB_DIO_1;
        else
            HB_DIO_0;
        SoftI2C_delay(DelayTick);
        HB_CLK_1;
        i--;
        value <<= 1;
        SoftI2C_delay(DelayTick);
        HB_CLK_0;
        SoftI2C_delay(DelayTick);
    }
    HB_DIO_IN;        //input
    SoftI2C_delay(DelayTick);

    HB_CLK_1;
    SoftI2C_delay(DelayTick);
    if (HB_DIO_READ)
    {
        //result = 1;
    }
    HB_CLK_0;
    HB_DIO_0;
    HB_DIO_OUT;   //output
}
uint8_t SoftI2C_read_byte(uint8_t ack)
{
    uint8_t i;
    uint8_t result = 0;
    i = 8;
    HB_DIO_IN;        //input
    while (i)
    {
        result <<= 1;
        HB_CLK_1;
        i--;
        SoftI2C_delay(DelayTick);
        if (HB_DIO_READ)
        {
            result |= 0x01;
        }
        HB_CLK_0;

        SoftI2C_delay(DelayTick);
    }
    HB_DIO_OUT;   //output
    if (ack)
        HB_DIO_1;
    else
        HB_DIO_0;
    SoftI2C_delay(DelayTick);
    HB_CLK_1;
    SoftI2C_delay(DelayTick);
    HB_CLK_0;
    HB_DIO_0;
    SoftI2C_delay(3);
    return result;
}


