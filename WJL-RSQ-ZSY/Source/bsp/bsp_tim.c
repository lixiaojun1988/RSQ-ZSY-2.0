/**
 * *****************************************************************************
 * @file     bsp_tim.c
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
 * @brief
 *
 */
void    bsp_tim2_init(void)
{
    TIM_InitTypeDef TIM_InitStructure;

    /* Enable the TIMx Clock */
    RCC_APB0PeriphClockOnOff(RCC_APB0_TIM2, SH_ON);

    /* ---------------------------------------------------------------
	Timer5 Source Clock is PCLK0=24MHz
    Prescaler = 24
    Counter clock = 1000
	Period = 1000 * 24 / 24mHz = 1000us
	--------------------------------------------------------------- */
		TIM_Reset(TIM2);
    TIM_InitStructure.TIM_Prescaler = (24-1);
    TIM_InitStructure.TIM_Period    = (100-1);//
    TIM_InitStructure.TIM_CLKSource = TIM_CLKSource_PCLK;
    TIM_InitStructure.TIM_OPMode    = TIM_OPMode_Continue;
    TIM_InitStructure.TIM_TCMode    = TIM_TCMode_Disable;
    TIM_InitStructure.TIM_ECFFilter = TIM_ECF_FLITER_TIME_NONE;//TIM_FLITER_TIMER_NONE;
    TIM_Init(TIM2, &TIM_InitStructure);

    TIM_INTConfig(TIM2, SH_ENABLE);//��timer2�ж�
    NVIC_EnableIRQ(TIM2_FIFO1_IRQn);

    TIM_OnOff(TIM2, SH_ON);
}

/**
 * @brief
 *
 */
void    bsp_tim_init(void)
{
    bsp_tim2_init();
}

/*----------------------------------------- E.N.D -----------------------------------------*/
