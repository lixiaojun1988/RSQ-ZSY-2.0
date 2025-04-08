/**********************************************
 * All rights reserved.
 *
 * File:
 * Author:	lxj
 * Date:		2022-12-14
 */
// usart
//*********************************************
#include "all_head.h"

#define SYS_CLK 32000000 //
/*****************************************************
 *函数名称: SC_USCI0_Init
 *函数功能: USCI0初始化函数
 *入口参数：void
 *出口参数：void
 *****************************************************/
/*
void SC_USCI0_Init(void)
{
   GPIO_Init(GPIO0, GPIO_PIN_5, GPIO_MODE_IN_PU);
   GPIO_Init(GPIO0, GPIO_PIN_6, GPIO_MODE_IN_PU);
   USCI0_ITConfig(ENABLE, LOW);
   USCI0_UART_Init(SYS_CLK, 9600, USCI0_UART_Mode_10B, USCI0_UART_RX_ENABLE);
}
*/
/*****************************************************
 *函数名称: SC_USCI1_Init
 *函数功能: USCI1初始化函数
 *入口参数：void
 *出口参数：void
 *****************************************************/
/*
void SC_USCI1_Init(void)
{
   P1CON &= 0xF5;    //TX/RXÉèÖÃÎªÊäÈë´øÉÏÀ­
   P1PH  |= 0x0A;	  //P13(TXD) P11(RXD)

   OTCON |= 0xC0;    //´®ÐÐ½Ó¿ÚSSI1Ñ¡ÔñUart1Í¨ÐÅ
   US1CON0 = 0x50;   //ÉèÖÃÍ¨ÐÅ·½Ê½ÎªÄ£Ê½Ò»£¬ÔÊÐí½ÓÊÕ
   US1CON1 = 32*1000000/9600;   //²¨ÌØÂÊµÍÎ»¿ØÖÆ
   US1CON2 = (32*1000000/9600)>>8;   //²¨ÌØÂÊ¸ßÎ»¿ØÖÆ
   IE2 |= 0x01;      //¿ªÆôSSI1ÖÐ¶Ï
}
*/
/*****************************************************
 *函数名称: SC_USCI2_Init
 *函数功能: USCI2初始化函数
 *入口参数：void
 *出口参数：void
 *****************************************************/

/*
void SC_USCI2_Init(void)
{
   USXINX = 0X02;
   P4CON &= 0xCF; // TX/RX设置为输入带上拉
   P4PH |= 0x30;  // P44(TXD) P45(RXD)
   TMCON |= 0xC0;						  // 串行接口SSI2选择Uart2通信
   USXCON0 = 0x50;						  // 设置通信方式为模式一，允许接收
   USXCON1 = 32 * 1000000 / 9600;		  // 波特率低位控制
   USXCON2 = (32 * 1000000 / 9600) >> 8; // 波特率高位控制
   IE2 |= 0x02;						  // 开启SSI2中断
}
*/
/*
void SC_USCI3_Init(void)
{
   USXINX = 0X03;                        //指向串口3
   P2CON &= 0x3F;                        // TX/RX 均要设置为输入带上拉(注意)
   P2PH |= 0xC0;                         // P26(TXD) P27(RXD)(注意)
   TMCON |= 0xC0;                        // 串行接口SSIX选择Uart通信
   USXCON0 = 0x50;                       // 设置通信方式为模式一，允许接收
   USXCON1 = 32 * 1000000 / 9600;        // 波特率低位控制
   USXCON2 = (32 * 1000000 / 9600) >> 8; // 波特率高位控制
   IE2 |= 0x04;                          // 开启中断
}*/


void SC_USCI5_Init(void)
{
   USXINX = 0X05;                        //指向串口3
   P0CON &= 0xF3;    //TX/RX 均要设置为输入带上拉(注意)
	 P0PH  |= 0x0C;		//P02(TXD) P03(RXD)(注意)
   TMCON |= 0xC0;                        // 串行接口SSIX选择Uart通信
   USXCON0 = 0x50;                       // 设置通信方式为模式一，允许接收
   USXCON1 = 32 * 1000000 / 9600;        // 波特率低位控制
   USXCON2 = (32 * 1000000 / 9600) >> 8; // 波特率高位控制
   	IE2 |= 0x10;     //开启中断
}
void usartDrvInit(void)
{
   //SC_USCI3_Init();
	SC_USCI5_Init();
}


