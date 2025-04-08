/**
 * *****************************************************************************
 * @file     bsp_adc.c
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
 * @brief: AD通道设置
 * @note:  这个表格需要根据应用进行修改：需要的通道就加入，不用的就删掉。
 * @scope: ADC_Channel_0 ~ ADC_Channel_26 和 ADC_Channel_VCC
 */
ADC_Channel_TypeDef const tab_adc_ch[] =
{
//    ADC_Channel_0,
//    ADC_Channel_1,
//    ADC_Channel_2,
//    ADC_Channel_3,
    ADC_Channel_4,
//    ADC_Channel_5,
//    ADC_Channel_6,
    ADC_Channel_7,
  //  ADC_Channel_8,
    ADC_Channel_9,
    ADC_Channel_10,
    ADC_Channel_11,
    ADC_Channel_12,
    ADC_Channel_13,
 //   ADC_Channel_14,
    // ADC_Channel_15,
    // ADC_Channel_16,
    // ADC_Channel_17,
    // ADC_Channel_18,
    // ADC_Channel_19,
    // ADC_Channel_20,
    // ADC_Channel_21,
    // ADC_Channel_22,
    // ADC_Channel_23,
    // ADC_Channel_24,
    // ADC_Channel_25,
    // ADC_Channel_26,
    // ADC_Channel_VCC,
};

/**
 * @brief: AD通道对应的引脚定义
 *
 */
typedef struct
{
    GPIO_TypeDef * port;
    uint16_t       pin;
}adc_gpio_map_t;

/**
 * @brief   adc 引脚映射 
 *
 */
const   adc_gpio_map_t   tab_adc_gpio_map[]=
{
    GPIOA,      GPIO_Pin_0,     // AN0
    GPIOA,      GPIO_Pin_1,     // AN1
    GPIOA,      GPIO_Pin_2,     // AN2
    GPIOA,      GPIO_Pin_4,     // AN3
    GPIOB,      GPIO_Pin_7,     // AN4
    GPIOB,      GPIO_Pin_8,     // AN5
    GPIOC,      GPIO_Pin_12,    // AN6
    GPIOC,      GPIO_Pin_13,    // AN7
    GPIOC,      GPIO_Pin_14,    // AN8
    GPIOC,      GPIO_Pin_15,    // AN9
    GPIOD,      GPIO_Pin_0,     // AN10
    GPIOD,      GPIO_Pin_1,     // AN11
    GPIOD,      GPIO_Pin_2,     // AN12
    GPIOD,      GPIO_Pin_3,     // AN13
    GPIOD,      GPIO_Pin_4,     // AN14
    GPIOD,      GPIO_Pin_7,     // AN15
    GPIOD,      GPIO_Pin_8,     // AN16
    GPIOA,      GPIO_Pin_12,    // AN17 ..
    GPIOA,      GPIO_Pin_9,     // AN18 ..
    GPIOA,      GPIO_Pin_13,    // AN19
    GPIOA,      GPIO_Pin_14,    // AN20
    GPIOC,      GPIO_Pin_10,    // AN21
    GPIOC,      GPIO_Pin_11,    // AN22
    GPIOD,      GPIO_Pin_9,     // AN23
    GPIOD,      GPIO_Pin_10,    // AN24
    GPIOD,      GPIO_Pin_11,    // AN25
    GPIOD,      GPIO_Pin_12,    // AN26
};

/**
 * @brief
 *
 */
void    bsp_adc_init(void)
{
    uint8_t i, channel;
    GPIO_InitTypeDef    GPIO_InitStructure;
    ADC_InitTypeDef     ADC_InitStructure;

    /* Enable the GPIO Clock */
   RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);   

    /* Enable the ADC Clock */
    RCC_APB0PeriphClockOnOff(RCC_APB0_ADC, SH_ON);

    /* 引脚：设置为输入高阻态，并复用为ADC */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    for (i=0; i<(sizeof(tab_adc_ch)/sizeof(tab_adc_ch[0])); i++)
    {
        channel = tab_adc_ch[i];
        if (channel < (sizeof(tab_adc_gpio_map)/sizeof(tab_adc_gpio_map[0])))
        {
            /* GPIO初始化 */
            GPIO_InitStructure.GPIO_Pin = tab_adc_gpio_map[channel].pin;
            GPIO_ResetBits(tab_adc_gpio_map[channel].port, GPIO_InitStructure.GPIO_Pin);
            GPIO_Init(tab_adc_gpio_map[channel].port, &GPIO_InitStructure);

            /* 复用功能 ad*/
            GPIO_PinAFConfig(tab_adc_gpio_map[channel].port, GPIO_InitStructure.GPIO_Pin, PXX_AF10);
        }
    }

    // 12bit转换时间 ： ADC_SampleTime + 14*Tadc
    // TIPS：TADC设置需保证ADC时钟周期 Tadc ≥ 50ns
    // 这里预分频设置为16，即使APB1时钟是最高的36MHz，Tadc = 1000*16/36=444ns。
    // 这里设置单通道ADC转换频率 = 36000KHz / 16 / (14+6) = 112.5KHz
    // 两个通道采样间隔>=0.5us。这里设置为8个ADC时钟周期，即为8*16/36MHz=3.5us
    ADC_InitStructure.ADC_Resolution      = ADC_Resolution_12Bit;       // 结果为12位
    ADC_InitStructure.ADC_Trigger         = ADC_Trigger_Software;       // 软件启动，GO
    ADC_InitStructure.ADC_RefVoltage      = ADC_RefVoltage_AVDD;        // 参考电压为VDD
    ADC_InitStructure.ADC_ConversionMode  = ADC_ConversionMode_Single;  // 单次转换
    ADC_InitStructure.ADC_SequencePointer = ADC_Sequence_0;             // 采样序列从序列0开始（仅对间断模式有效）
    ADC_InitStructure.ADC_NumOfConversion = ADC_NumOfConversion_1;      // 采样序列数量 1个
    ADC_InitStructure.ADC_Prescaler       = ADC_Prescaler_16;           // 时钟分频
    ADC_InitStructure.ADC_TwoSampleGap    = ADC_TwoSampleGap_8Cycles;   // 通道采样间隔
    ADC_InitStructure.ADC_SampleTime      = ADC_SampleTime_6Cycles;     // 采样时间
    ADC_Init(ADC, &ADC_InitStructure);                                  /* 初始化 */

	// 通道配置
    ADC_ChannelConfig(ADC, tab_adc_ch[0], ADC_Sequence_0, ADC_TwoSampleGap_Disable);   // 通道配置：采样序列0配置为通道AN15，无采样间隔

    ADC_OnOff(ADC, SH_ON);                                              // ADC模块开启

    for (i=0; i<120; i++)           // 要求ADCON之后至少10us才可以GO进行转换，因为ADC模块打开后有个稳定时间
    {
        __nop();
    }
}

/**
  * @brief   ADC转换并返回12位AD值
  * @param   chn：待转换通道
  * @retval  V16：12位的AD转换结果
  */
uint16_t    bps_adc_get_value(uint8_t chn) //查询方式读取
{
    uint16_t V16;
    uint8_t i;

		ADC_Channel_TypeDef chx;
	
	  chx =(ADC_Channel_TypeDef)chn;
    ADC_ChannelConfig(ADC, chx, ADC_Sequence_0, ADC_TwoSampleGap_Disable);   // 通道配置：采样序列x配置为通道ANx，无采样间隔
    for (i=0; i<10; i++);                                   // 建议延时>=5us
    ADC_SoftwareStartConversion(ADC);                       // GO开始转换
    for (i=0; i<20; i++);                                  // 建议延时>=10us
    while( ADC_GetSoftwareStartConversionStatus(ADC) );     // 等待转换完成
    V16 = ADC_GetConversionValue(ADC, ADC_Sequence_0);      // 获取序列0(AN15)的AD值

    return  V16;
}


/*----------------------------------------- E.N.D -----------------------------------------*/
