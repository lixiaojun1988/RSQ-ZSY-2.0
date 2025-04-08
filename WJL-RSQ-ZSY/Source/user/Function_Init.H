#ifndef _Function_Init_H_
#define _Function_Init_H_

#include "H/SC95F857x_C.H"
#include "intrins.H"
#define uchar unsigned char
#define uint unsigned int

extern void IO_Init(void);
extern void BTM_Test(void);
extern void EXTI_Test(void);
extern void Timer_Test(void);
extern void PWM_Test(void);
extern void Uart_Test(void);
extern void USCI0_Test(void);
extern void USCI1_Test(void);
extern void USCI2_Test(void);
extern void ADC_Test(void);
extern void IAP_Test(void);
extern void CRC_Test(void);
extern void Multiplier_Divider_Test(void);
extern void Delay(unsigned long time);

/*****************************************
ËµÃ÷£ºÑ¡Ôñ²âÊÔIC
*****************************************/
#define SC95F8575  0
#define SC95F8573  1
#define SC95F8572  2
#define SC95F8571  3

#define IC_MODEL  SC95F8575

#endif