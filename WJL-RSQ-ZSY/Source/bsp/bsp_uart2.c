/**
 * *****************************************************************************
 * @file     bsp_uart.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "sh30f9010.h"


// TXD
#define     UARTx_TXD_PORT      GPIOD
#define     UARTx_TXD_PIN       GPIO_Pin_10
#define     UARTx_TXD_AF        PD10_TXD2

// RXD
#define     UARTx_RXD_PORT      GPIOD
#define     UARTx_RXD_PIN       GPIO_Pin_9
#define     UARTx_RXD_AF        PD9_RXD2

// Uart定义
#define     UARTx_BAUDRATE      (4800)              // 波特率
#define     UARTx               UART2               // Uart Base
#define     UARTx_IRQn          UART2_IRQn          // IRQ
#define     UARTx_Handler       UART2_Handler       // 中断处理程序
#define     UARTx_TDR           UART2->TDR          // 发送数据寄存器
#define     UARTx_RDR           UART2->RDR          // 接收数据寄存器
#define     UARTx_CLOCK         RCC_APB1_UART2      // 时钟
#define     UARTx_GPIO_AF       GPIO_AF_UART        // 复用



/**
 * @brief
 *
 */
void    bsp_uart2_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    UART_InitTypeDef    UART_InitStructure;

    /* Enable the GPIO Clock */
    /* Enable the GPIO Clock */
   RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);    

    /* Enable the Uartx Clock */
    RCC_APB1PeriphClockOnOff(UARTx_CLOCK, SH_ON);
		NVIC_DisableIRQ(UARTx_IRQn);//
		RCC_APB1PeriphReset(UARTx_CLOCK);//
    /* 串口TXD引脚设置为输出高 */
    GPIO_InitStructure.GPIO_Pin   = UARTx_TXD_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_SetBits(UARTx_TXD_PORT, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(UARTx_TXD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(UARTx_TXD_PORT, GPIO_InitStructure.GPIO_Pin, UARTx_TXD_AF);

    /* RXD：设置为输入上拉 */
    GPIO_InitStructure.GPIO_Pin  = UARTx_RXD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_SetBits(UARTx_RXD_PORT, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(UARTx_RXD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(UARTx_RXD_PORT, GPIO_InitStructure.GPIO_Pin, UARTx_RXD_AF);

    /* 串口设置 */
    UART_InitStructure.UART_Mode     = UART_Mode_1;                     // 模式1
    UART_InitStructure.UART_BaudRate = UARTx_BAUDRATE;                  // 波特率：
    UART_InitStructure.UART_StopBits = UART_StopBits_1;                 // 停止位：1位
    UART_InitStructure.UART_Parity   = UART_SoftWare_Custom;            // 软件自定义(无校验)
    UART_InitStructure.UART_Enable   = UART_Enable_Tx | UART_Enable_Rx; // 允许收发
	//	UART_InitStructure.UART_Enable   =  UART_Enable_Tx; // 允许发
    UART_Init(UARTx, &UART_InitStructure);

    UART_ClearFlag(UARTx, UART_FLAG_ALL);                                        // 清除中断标志位
    UART_INTConfig(UARTx, UART_INT_RX, SH_ENABLE);        // 允许中断：发送/接收
		UART_INTConfig(UARTx, UART_INT_TC , SH_ENABLE);        // 允许中断：发送/接收
    NVIC_EnableIRQ(UARTx_IRQn);



/////  直接操作寄存器位控制 参考
//		UART2->CR.BIT.TEN=0; //发送使能关闭
//		UART2->CR.BIT.TEN=1; //发送使能打开	
//		UART2->CR.BIT.REN=0; //接收使能关闭
//		UART2->CR.BIT.REN=1; //接收使能打开
}

