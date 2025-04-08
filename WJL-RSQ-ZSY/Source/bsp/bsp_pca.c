/**
 * *****************************************************************************
 * @file     bsp_pca.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "sh30f9010.h"

#define PCA_Test_Capture 0
#define PCA_Test_Compare 1
#define PCA_Test_Freq 2
#define PCA_Test_Pwm 3
#define PCA_Test_Mode PCA_Test_Pwm

/**
 * @brief   输入捕捉模式
 *
 */
void bsp_pca_capture_init(void) // PCA_Test_Capture
{
    GPIO_InitTypeDef GPIO_InitStructure;
    PCA_InitTypeDef PCA_InitStructure;

    /* Enable the GPIO Clock */
    //    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);     // sh30f9071的GPIO时钟线一直是开启的，不用额外开启

    /* GPIO初始化 */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_SetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_Pin_1, PB1_PCA1A);

    // Open PCAx clock gate
    RCC_APB0PeriphClockOnOff(RCC_APB0_PCA1, SH_ON);

    PCA_OnOff(PCA1, SH_OFF); // off PCA
    NVIC_DisableIRQ(PCA1_IRQn);

    PCA_StructInit(&PCA_InitStructure);

    PCA_InitStructure.PCA_CFGR.ClkSel = PCLK0;                        // PCA clock is system clock
    PCA_InitStructure.PCA_CFGR.ECFSel = FILTERING_32PCLK0;            // 输入信号滤波时间
    PCA_InitStructure.PCA_CFGR.PCACounterMode = SINGLEEDGE;           // PCA counting method is upward counting
    PCA_InitStructure.PCA_CCMR0.PCA_CEN = SH_ENABLE;                  // Enable capture \ compare module
    PCA_InitStructure.PCA_CCMR0.ModeSelect = CAPTUREMODE;             // PCA capture mode
    PCA_InitStructure.PCA_CCMR0.FunctionSelect = RISING_EDGE_TRIGGER; // Rising edge trigger
    PCA_InitStructure.PCA_CCMR0.PCA_CC = SH_DISABLE;                  // 发生捕捉事件时清除计数器值
    // PCA_InitStructure.PCA_CCMR0.PCA_TCP= SH_DISABLE;
    PCA_InitStructure.PCA_PSC = (1 - 1); // 预分频器 分频系数为PSC+1
    PCA_InitStructure.PCA_PR = 0xFFFF;   // Capture mode PR needs to be set
    PCA_InitStructure.PCA_CCR0 = 0;      // Capture mode CCR0 reads the count value at the time of capture

    PCA_Init(PCA1, &PCA_InitStructure);

    PCA_CapINTConfig(PCA1, PCA_CHANNEL_A, SH_ENABLE); // ENable comparison interrupt

    PCA_OnOff(PCA1, SH_ON); // enable PCA

    NVIC_EnableIRQ(PCA1_IRQn);
}

/**
 * @brief   bsp pca中断处理
 *
 */
#if 1
// PCA_CC = SH_DISABLE时， 发生捕捉事件时不清除计数器值+
// 数据比较理想，抓取10KHz的方波时，pcaPeriod稳定为2400。
uint16_t pcaPeriod;     // 周期
uint16_t pcaCounter;    // 当前计数值
uint16_t pcaCounterPre; // 前一个计数值
#else
// PCA_CC = SH_ENABLE时， 发生捕捉事件时清除计数器值
// 数据有损耗，抓取10KHz的方波时，pcaPeriod稳定为2398。
uint16_t pcaPeriod; // 周期
#endif
void bsp_pca_handler(void)
{
#if (PCA_Test_Mode == PCA_Test_Capture)
#if 1
    // PCA_CC = SH_DISABLE时， 发生捕捉事件时不清除计数器值
    pcaCounter = PCA1->CCR0.V32;
    pcaPeriod = pcaCounter - pcaCounterPre;
    pcaCounterPre = pcaCounter;
#else
    // PCA_CC = SH_ENABLE时， 发生捕捉事件时清除计数器值
    pcaPeriod = PCA1->CCR0.V32;
#endif
#endif
}

/**
 * @brief   比较匹配输出  （此模式下的PCAxA/B 引脚输出是50%占空比的方波）
 *
 */
void bsp_pca_compare_init(void) // PCA_Test_Compare
{
    GPIO_InitTypeDef GPIO_InitStructure;
    PCA_InitTypeDef PCA_InitStructure;

    /* Enable the GPIO Clock */
    //    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);     // sh30f9071的GPIO时钟线一直是开启的，不用额外开启

    /* GPIO初始化 */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_ResetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_Pin_1, PB1_PCA1A);

    // Open PCAx clock gate
    RCC_APB0PeriphClockOnOff(RCC_APB0_PCA1, SH_ON);

    PCA_OnOff(PCA1, SH_OFF); // off PCA
    NVIC_DisableIRQ(PCA1_IRQn);
    PCA_StructInit(&PCA_InitStructure);

    /* T = 1/(24000000/24000)= 1 ms */
    /* 此模式的占空比固定为50%，占空比的时间是PCA_PR */

    PCA_InitStructure.PCA_CFGR.ClkSel = PCLK0;               // PCA clock source is the system clock
    PCA_InitStructure.PCA_CCMR0.PCA_CEN = SH_ENABLE;         // Enable capture \ compare module
    PCA_InitStructure.PCA_CCMR0.ModeSelect = COMPAREMODE;    // PCA works in comparing matching mode
    PCA_InitStructure.PCA_CCMR0.FunctionSelect = CONTINUOUS; // Continuous comparison mode
    PCA_InitStructure.PCA_CFGR.PCACounterMode = SINGLEEDGE;  // PCA works in single edge mode
    PCA_InitStructure.PCA_CCMR0.PCA_TCP = INITAL_LOW;        //
    PCA_InitStructure.PCA_PSC = (1 - 1);                     // 预分频器 分频系数为PSC+1
    PCA_InitStructure.PCA_PR = 24000;                        // It is related to the output waveform period
    PCA_InitStructure.PCA_CCR0 = 4000;                       // It is related to the phase of the output waveform
    PCA_Init(PCA1, &PCA_InitStructure);

#if 1
    // 这个溢出的时间为 PCA_PR
    PCA_INTConfig(PCA1, PCA_COUNTER_OVERFLOW_INT, SH_ENABLE); // Enable overflow interrupt
#endif
    PCA_CapINTConfig(PCA1, PCA_CHANNEL_A, SH_ENABLE); // Enable capture \ compare interrup

    PCA_OnOff(PCA1, SH_ON); // enable PCA

    NVIC_EnableIRQ(PCA1_IRQn);
}

/**
 * @brief   频率输出方式  （此模式下的PCAxA/B 引脚输出是50%占空比的方波）
 *
 */
void bsp_pca_freq_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    PCA_InitTypeDef PCA_InitStructure;

    /* Enable the GPIO Clock */
    //    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);     // sh30f9071的GPIO时钟线一直是开启的，不用额外开启

    /* GPIO初始化 */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_ResetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_Pin_1, PB1_PCA1A);

    // Open PCAx clock gate
    RCC_APB0PeriphClockOnOff(RCC_APB0_PCA1, SH_ON);

    PCA_OnOff(PCA1, SH_OFF); // off PCA
    NVIC_DisableIRQ(PCA1_IRQn);
    PCA_StructInit(&PCA_InitStructure);

    /*f = PCAclock/(2*CCRH),In this mode, the PRL value is fixed to 0xff, and the user can configure the PRH value
    to change the maximum count valueso as to change the overflow interrupt, CPRL low-level automatic counting,
    CPRH bit change, adjustable cycle, when the value in ccrh is 0x00, it is equivalent to 256*/

    PCA_InitStructure.PCA_CFGR.ClkSel = PCLK0;               // PCA clock source is the system clock
    PCA_InitStructure.PCA_CCMR0.PCA_CEN = SH_ENABLE;         // Enable capture \ compare module
    PCA_InitStructure.PCA_CCMR0.ModeSelect = FREQOUTPUTMODE; // PCA works in timer mode
    PCA_InitStructure.PCA_CFGR.PCACounterMode = SINGLEEDGE;  // PCA counting method is up counting
    PCA_InitStructure.PCA_CCMR0.PCA_TCP = INITAL_HIGH;       // Normal output waveform
    PCA_InitStructure.PCA_PSC = (1 - 1);                     // 预分频器 分频系数为PSC+1
    PCA_InitStructure.PCA_PR = 0xFFFF;                       // In frequency output mode, PR is larger than CCR
    PCA_InitStructure.PCA_CCR0 = 0xF0FF;                     // ֻIt is only related to the high eight bits, that is,or call PCA_FreqOutValue(xx)
                                                             // when the high eight bits overflow, the level is reversed and the timing starts from 0 again
    /*
      f = PCAclock/(2*CCRH) ： PWM输出的周期为PCA_CCR的高8位的2倍。
      此模式的占空比固定为50%
    */
    PCA_Init(PCA1, &PCA_InitStructure);

#if 0
    // 这个溢出的时间为 PCA_PR
    PCA_INTConfig(PCA1, PCA_COUNTER_OVERFLOW_INT, SH_ENABLE);           // Enable overflow interrupt
#endif

    PCA_OnOff(PCA1, SH_ON); // enable PCA

#if 0
    NVIC_EnableIRQ(PCA1_IRQn);
#endif
}

/**
 * @brief   PWM 输出
 *
 */
void bsp_pca_pwm_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    PCA_InitTypeDef PCA_InitStructure;

    //    /* Enable the GPIO Clock */
    //    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);

    //    /* GPIO初始化 */
    //    GPIO_StructInit(&GPIO_InitStructure);
    //    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    //    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
    //    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    //    GPIO_ResetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
    //    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //    GPIO_PinAFConfig(GPIOB, GPIO_Pin_1, PB1_PCA1A);

    //    // Open PCAx clock gate
    //    RCC_APB0PeriphClockOnOff(RCC_APB0_PCA1, SH_ON);

    //		PCA_OnOff(PCA1, SH_OFF); //off PCA
    //    NVIC_DisableIRQ(PCA1_IRQn);
    //
    //    PCA_StructInit(&PCA_InitStructure);

    //    /* Duty=(PCAx->PR - PCAx->CCRn)/(PCAx->PR + 1),TOPR control frequency, CCRnAdjust the duty cycle */
    //    PCA_InitStructure.PCA_CFGR.ClkSel           = PCLK0;            // PCA clock source is the system clock
    //    PCA_InitStructure.PCA_CFGR.PCACounterMode   = SINGLEEDGE;       // PCA counting method is upward countine
    //    PCA_InitStructure.PCA_CCMR0.PCA_CEN         = SH_ENABLE;        // Enable capture \ compare module PCA_CHANNEL_A
    //    PCA_InitStructure.PCA_CCMR0.ModeSelect      = PWMMODE;          // PCA works in PWM mode PCA_CHANNEL_A
    //    PCA_InitStructure.PCA_CCMR0.FunctionSelect  = PWM_16BIT;        // 16 bit PWM mode PCA_CHANNEL_A
    //    PCA_InitStructure.PCA_CCMR0.PCA_TCP         = ACTIVE_LOW;       // Normal output waveform PCA_CHANNEL_A
    //    PCA_InitStructure.PCA_PSC                   = (1 - 1);          // 预分频器 分频系数为PSC+1
    //    PCA_InitStructure.PCA_PR                    = 24000;            // 1kHz  50%
    //    PCA_InitStructure.PCA_CCR0                  = 4000;
    //    PCA_Init(PCA1, &PCA_InitStructure);

    //    /**
    //        ACTIVE_LOW时输出波形如下：
    //             _____________                        ____
    //            |             |                      |
    //        ____|             |______________________|
    //            |<- PCA_CCR ->|<- (PCA_PR-PCA_CCR) ->|

    //    */
    //    PCA_OnOff(PCA1, SH_ON);                                  //enable PCA

    // SBPWM/pca1A
    /* Enable the GPIO Clock */
    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);

    /* GPIO初始化 */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_ResetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_InitStructure.GPIO_Pin, PB1_PCA1A);

    // Open PCAx clock gate
    RCC_APB0PeriphClockOnOff(RCC_APB0_PCA1, SH_ON);

    PCA_OnOff(PCA1, SH_OFF); // off PCA
    NVIC_DisableIRQ(PCA1_IRQn);
    PCA_StructInit(&PCA_InitStructure);

    /* Duty=(PCAx->PR - PCAx->CCRn)/(PCAx->PR + 1),TOPR control frequency, CCRnAdjust the duty cycle */
    PCA_InitStructure.PCA_CFGR.ClkSel = PCLK0;              // PCA clock source is the system clock
    PCA_InitStructure.PCA_CFGR.PCACounterMode = SINGLEEDGE; // PCA counting method is upward countine
    PCA_InitStructure.PCA_CCMR0.PCA_CEN = SH_ENABLE;        // Enable capture \ compare module PCA_CHANNEL_B
    PCA_InitStructure.PCA_CCMR0.ModeSelect = PWMMODE;       // PCA works in PWM mode   PCA_CHANNEL_B
    PCA_InitStructure.PCA_CCMR0.FunctionSelect = PWM_16BIT; // 16 bit PWM mode             PCA_CHANNEL_B
    PCA_InitStructure.PCA_CCMR0.PCA_TCP = ACTIVE_LOW;       // Normal output waveform
    PCA_InitStructure.PCA_PSC = (1 - 1);                    // 预分频器 分频系数为PSC+1
    PCA_InitStructure.PCA_PR = 24000 / 4;                   // 4kHz  50%    6000等分
    PCA_InitStructure.PCA_CCR0 = (24000 >> 4);              // 4000;  PCA_CHANNEL_A
    //	PCA_InitStructure.PCA_CCR1                  = ((24000/4)>>1);//4000;  PCA_CHANNEL_B
    PCA_Init(PCA1, &PCA_InitStructure);
    /**
        ACTIVE_LOW时输出波形如下：
             _____________                        ____
            |             |                      |
        ____|             |______________________|
            |<- PCA_CCR ->|<- (PCA_PR-PCA_CCR) ->|

    */
    PCA_SetCCRValue(PCA1, PCA_CHANNEL_A, 0); // 设置p
    PCA_OnOff(PCA1, SH_ON);                  // enable PCA
    //

    // PC4  PCA2B  	BLF

    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);

    /* GPIO初始化 */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // fj
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_ResetBits(GPIOC, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_InitStructure.GPIO_Pin, PC4_PCA2B);

    // Open PCAx clock gate
    RCC_APB0PeriphClockOnOff(RCC_APB0_PCA2, SH_ON);

    PCA_OnOff(PCA2, SH_OFF); // off PCA
    NVIC_DisableIRQ(PCA2_IRQn);
    PCA_StructInit(&PCA_InitStructure);

    /* Duty=(PCAx->PR - PCAx->CCRn)/(PCAx->PR + 1),TOPR control frequency, CCRnAdjust the duty cycle */
    PCA_InitStructure.PCA_CFGR.ClkSel = PCLK0;              // PCA clock source is the system clock
    PCA_InitStructure.PCA_CFGR.PCACounterMode = SINGLEEDGE; // PCA counting method is upward countine
    PCA_InitStructure.PCA_CCMR1.PCA_CEN = SH_ENABLE;        // Enable capture \ compare module
    PCA_InitStructure.PCA_CCMR1.ModeSelect = PWMMODE;       // PCA works in PWM mode
    PCA_InitStructure.PCA_CCMR1.FunctionSelect = PWM_16BIT; // 16 bit PWM mode
    PCA_InitStructure.PCA_CCMR1.PCA_TCP = ACTIVE_LOW;       // Normal output waveform
    PCA_InitStructure.PCA_PSC = (1 - 1);                    // 预分频器 分频系数为PSC+1
    PCA_InitStructure.PCA_PR = 24000 / 4;                   // 4kHz  50%
                                                            //    PCA_InitStructure.PCA_CCR0                  = (24000>>1);//4000;  PCA_CHANNEL_A
    PCA_InitStructure.PCA_CCR1 = ((24000 / 4) >> 1);        // 4000;  PCA_CHANNEL_B
    PCA_Init(PCA2, &PCA_InitStructure);
    /**
        ACTIVE_LOW时输出波形如下：
             _____________                        ____
            |             |                      |
        ____|             |______________________|
            |<- PCA_CCR ->|<- (PCA_PR-PCA_CCR) ->|

    */
    PCA_SetCCRValue(PCA2, PCA_CHANNEL_B, 0); // 设置period
    PCA_OnOff(PCA2, SH_ON);                  // enable PCA

    //		PCA_SetPRValue(PCA2, 24000);//设置period

    // PA2 PCA3B  	FJ BRAKE

    // RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);

    /* GPIO初始化 */
    // GPIO_StructInit(&GPIO_InitStructure);
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    // GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
    // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    // GPIO_ResetBits(GPIOA, GPIO_InitStructure.GPIO_Pin);
    // GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOA, GPIO_InitStructure.GPIO_Pin, PA2_PCA3B);

    // Open PCAx clock gate
    // RCC_APB0PeriphClockOnOff(RCC_APB0_PCA3, SH_ON);

    // PCA_OnOff(PCA3, SH_OFF); // off PCA
    // NVIC_DisableIRQ(PCA3_IRQn);
    // PCA_StructInit(&PCA_InitStructure);

    // /* Duty=(PCAx->PR - PCAx->CCRn)/(PCAx->PR + 1),TOPR control frequency, CCRnAdjust the duty cycle */
    // PCA_InitStructure.PCA_CFGR.ClkSel = PCLK0;              // PCA clock source is the system clock
    // PCA_InitStructure.PCA_CFGR.PCACounterMode = SINGLEEDGE; // PCA counting method is upward countine
    // PCA_InitStructure.PCA_CCMR1.PCA_CEN = SH_ENABLE;        // Enable capture \ compare module
    // PCA_InitStructure.PCA_CCMR1.ModeSelect = PWMMODE;       // PCA works in PWM mode
    // PCA_InitStructure.PCA_CCMR1.FunctionSelect = PWM_16BIT; // 16 bit PWM mode
    // PCA_InitStructure.PCA_CCMR1.PCA_TCP = ACTIVE_LOW;       // Normal output waveform
    // PCA_InitStructure.PCA_PSC = (1 - 1);                    // 预分频器 分频系数为PSC+1
    // PCA_InitStructure.PCA_PR = 24000 / 4;                   // 4kHz  50%
    //                                                         //    PCA_InitStructure.PCA_CCR0                  = (24000>>1);//4000;  PCA_CHANNEL_A
    // PCA_InitStructure.PCA_CCR1 = ((24000 / 4) >> 1);        // 4000;  PCA_CHANNEL_B
    // PCA_Init(PCA3, &PCA_InitStructure);
    // /**
    //     ACTIVE_LOW时输出波形如下：
    //          _____________                        ____
    //         |             |                      |
    //     ____|             |______________________|
    //         |<- PCA_CCR ->|<- (PCA_PR-PCA_CCR) ->|

    // */
    // PCA_SetCCRValue(PCA3, PCA_CHANNEL_B, 0); // 设置period
    // PCA_OnOff(PCA3, SH_ON);                  // enable PCA

    //		PCA_SetPRValue(PCA2, 24000);//设置period
}

/**
 * @brief
 *
 */
void bsp_pca_init(void)
{
    // #if (PCA_Test_Mode == PCA_Test_Capture)
    //     bsp_pca_capture_init();
    // #elif (PCA_Test_Mode == PCA_Test_Compare)
    //     bsp_pca_compare_init();
    // #elif (PCA_Test_Mode == PCA_Test_Freq)
    //     bsp_pca_freq_init();
    // #else
    bsp_pca_pwm_init();
    // #endif
}

/*----------------------------------------- E.N.D -----------------------------------------*/
