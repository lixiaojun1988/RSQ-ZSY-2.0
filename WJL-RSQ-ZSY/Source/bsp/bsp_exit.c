/**
 * *****************************************************************************
 * @file     bsp_exit.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "sh30f9010.h"

/**
 * 注意事项：
 * 1：必须要把对应的引脚AF设置为GPIO，否则无法触发EXTI。
 */
// 2237  2207
// PA0设置为EXTI功能

/**
 * @brief
 *
 */
void bsp_exit_init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Enable the GPIO Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);

  /* Enable the EXTI Clock */
  RCC_APB0PeriphClockOnOff(RCC_APB0_EXTI, SH_ON);

  /* IO设置为输入上拉 */ //	PIN6 --PA8,
                         /*
                         GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//水流量
                         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
                         GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
                         GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                         GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
                         GPIO_SetBits(GPIOA, GPIO_InitStructure.GPIO_Pin);
                         GPIO_Init(GPIOA, &GPIO_InitStructure);
                         GPIO_PinAFConfig(GPIOA, GPIO_InitStructure.GPIO_Pin, PXX_AF1);              // 必须要把对应的引脚设置为GPIO，否则无法触发EXTI
                       
                         NVIC_DisableIRQ(EXTI9_4_IRQn);
                       
                         EXTI_InitStructure.EXTI_Line                = EXTI_Line8;                   //
                         EXTI_InitStructure.EXTI_Mode                = EXTI_Mode_Interrupt;          // 中断
                         EXTI_InitStructure.EXTI_Trigger             = EXTI_Trigger_Falling;         // 下降沿
                         EXTI_InitStructure.EXTI_SampleCycle         = EXTI_SampleCycle_4;           // 连续采样次数 4次
                         EXTI_InitStructure.EXTI_SampleClockDivision = EXTI_SampleClockDivision_16;  // 采样时钟预分频比/16     即去抖时间=4*16/36MHz=1.78us
                         EXTI_InitStructure.EXTI_LineCmd             = SH_ENABLE;                    // 线使能
                         EXTI_Init(&EXTI_InitStructure);
                       
                         EXTI_PinConfig(EXTI_Port_GPIOA, EXTI_PinSource8);    // EXTIx的触发源
                         NVIC_EnableIRQ(EXTI9_4_IRQn);                     // 中断使能
                         */

  /* IO设置为输入上拉 */ // PIN21--- PB12    水流量   PB14 风机反馈    PB0水泵反馈
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_SetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_InitStructure.GPIO_Pin, PXX_AF1); /* 必须要把对应的引脚设置为GPIO，否则无法触发EXTI */

  NVIC_DisableIRQ(EXTI15_10_IRQn);
  /* EXTI Initail */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                                 //
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                        // 中断
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                    // 下降沿
  EXTI_InitStructure.EXTI_SampleCycle = EXTI_SampleCycle_4;                  // 连续采样次数 4次
  EXTI_InitStructure.EXTI_SampleClockDivision = EXTI_SampleClockDivision_16; // 采样时钟预分频比/16     即去抖时间=4*16/36MHz=1.78us
  EXTI_InitStructure.EXTI_LineCmd = SH_ENABLE;                               // 线使能
  EXTI_Init(&EXTI_InitStructure);

  EXTI_PinConfig(EXTI_Port_GPIOB, EXTI_PinSource0); // EXTIx的触发源

  EXTI_InitStructure.EXTI_Line = EXTI_Line12;                                //
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                        // 中断
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                    // 下降沿
  EXTI_InitStructure.EXTI_SampleCycle = EXTI_SampleCycle_4;                  // 连续采样次数 4次
  EXTI_InitStructure.EXTI_SampleClockDivision = EXTI_SampleClockDivision_16; // 采样时钟预分频比/16     即去抖时间=4*16/36MHz=1.78us
  EXTI_InitStructure.EXTI_LineCmd = SH_ENABLE;                               // 线使能
  EXTI_Init(&EXTI_InitStructure);

  EXTI_PinConfig(EXTI_Port_GPIOB, EXTI_PinSource12);                         // EXTIx的触发源
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;                                //
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                        // 中断
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                    // 下降沿
  EXTI_InitStructure.EXTI_SampleCycle = EXTI_SampleCycle_4;                  // 连续采样次数 4次
  EXTI_InitStructure.EXTI_SampleClockDivision = EXTI_SampleClockDivision_16; // 采样时钟预分频比/16     即去抖时间=4*16/36MHz=1.78us
  EXTI_InitStructure.EXTI_LineCmd = SH_ENABLE;                               // 线使能
  EXTI_Init(&EXTI_InitStructure);

  EXTI_PinConfig(EXTI_Port_GPIOB, EXTI_PinSource14); // EXTIx的触发源
  NVIC_EnableIRQ(EXTI15_10_IRQn);                    // 中断使能
}

/*----------------------------------------- E.N.D -----------------------------------------*/
