/**
 * *****************************************************************************
 * @file     bsp_gpio.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
//(uint32_t)GPIO_Pin << 16;
#include "sh30f9010.h"
#include "all_head.h"
// #define PA2_SETB()						GPIOA->BSRR.V32=(0x01ul<<2)
// #define PA2_CLRB()						GPIOA->BSRR.V32=((0x01ul<<2)<<16)

//#define  GPIO_MODER(GPIOX,GPIO_Pin,GPIO_STATE) do{\
//																							if(1==GPIO_STATE)  	GPIOX->MODER.V32 |= GPIO_Pin;\
//																							else 								GPIOX->MODER.V32 &= (~GPIO_Pin);\
//																							}while(0)

// #define PA2_INMODE()					GPIO_MODER(GPIOA,GPIO_Pin_2, 0)
// #define PA2_OUTMODE()					GPIO_MODER(GPIOA,GPIO_Pin_2, 1)

#define GPIO_MODERIN(GPIOX, GPIO_Pin) \
  do                                  \
  {                                   \
    GPIOX->MODER.V32 &= (~GPIO_Pin);  \
  } while (0)
#define GPIO_MODEROUT(GPIOX, GPIO_Pin) \
  do                                   \
  {                                    \
    GPIOX->MODER.V32 |= GPIO_Pin;      \
  } while (0)

#define PA2_INMODE() GPIO_MODERIN(GPIOA, GPIO_Pin_2)
#define PA2_OUTMODE() GPIO_MODEROUT(GPIOA, GPIO_Pin_2)

#define PA2_SETB() GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define PA2_CLRB() GPIO_ResetBits(GPIOA, GPIO_Pin_2)

// GPIO_ToggleBits(GPIOA,GPIO_Pin_0)

#define READ_PA2 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

/**
 * @brief  GPIO����Ϊ�����
 * @param  GPIOx: where x can be A..E to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
 * @retval void
 */
void GPIO_OutL(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON); // GPIOʱ���߿���
  /* Configure */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;          // ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    // ���
  GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL; // ������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // �������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOx, GPIO_InitStructure.GPIO_Pin); // �����
  GPIO_PinAFConfig(GPIOx, GPIO_Pin, PXX_AF1);
}

/**
 * @brief  GPIO����Ϊ�����
 * @param  GPIOx: where x can be A..E to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
 * @retval void
 */
void GPIO_OutH(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON); // GPIOʱ���߿���
  //

  /* Configure */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;          // ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    // ���
  GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL; // ������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // �������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  GPIO_SetBits(GPIOx, GPIO_InitStructure.GPIO_Pin); // �����
  GPIO_PinAFConfig(GPIOx, GPIO_Pin, PXX_AF1);
}

/**
 * @brief  GPIO����Ϊ��©���
 * @param  GPIOx: where x can be A..E to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
 * @retval void
 */
void GPIO_OutOD(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON); // GPIOʱ���߿���

  /* Configure */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;          // ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    // ���
  GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL; // ������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   // ��©
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  GPIO_SetBits(GPIOx, GPIO_InitStructure.GPIO_Pin); // �����©
  GPIO_PinAFConfig(GPIOx, GPIO_Pin, PXX_AF1);
}

/**
 * @brief  GPIO����Ϊ�������
 * @param  GPIOx: where x can be A..E to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
 * @retval void
 */
void GPIO_InHZ(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON); // GPIOʱ���߿���

  /* Configure */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;          // ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     // ����
  GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL; // ������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // �������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������
  GPIO_Init(GPIOx, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOx, GPIO_Pin, PXX_AF1);
}

/**
 * @brief  GPIO����Ϊ��������
 * @param  GPIOx: where x can be A..E to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
 * @retval void
 */
void GPIO_InPU(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON); // GPIOʱ���߿���
  /* Configure */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;          // ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     // ����
  GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL; // ������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // �������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;     // ����
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  GPIO_SetBits(GPIOx, GPIO_InitStructure.GPIO_Pin);
  GPIO_PinAFConfig(GPIOx, GPIO_Pin, PXX_AF1);
}
/**
 * @brief
 *
 */
void bsp_gpio_init(void)
{
  //   GPIO_InitTypeDef    GPIO_InitStructure;

  /* Enable the GPIO Clock */
  RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON); // GPIOʱ���߿���

  //    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  //    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
  //    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //    GPIO_SetBits(GPIOA, GPIO_InitStructure.GPIO_Pin);
  //    GPIO_Init(GPIOA, &GPIO_InitStructure);
  //		GPIO_PinAFConfig(GPIOA, GPIO_InitStructure.GPIO_Pin, PXX_AF1);// ��Ϊ��ͨ IO��
  //
  //		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
  //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  //    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
  //    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //    GPIO_SetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);
  //    GPIO_Init(GPIOB, &GPIO_InitStructure);
  //		GPIO_PinAFConfig(GPIOB, GPIO_InitStructure.GPIO_Pin, PXX_AF1);// ��Ϊ��ͨ IO��

  //		PA2_INMODE();
  //		PA2_OUTMODE();
  //		PA2_SETB();
  //		PA2_CLRB();

  // �ο�������ʵ��Ӧ�õ�����ʼ��IO ״̬
  /*---------------------------------------------------------------
   *                  GPIOA
  -*-------------------------------------------------------------*/
  GPIO_OutL(GPIOA, 0xFFFF); /*!< ����� */
  GPIO_OutOD(GPIOA, GPIO_Pin_14);

  // GPIO_OutL(GPIOA, GPIO_Pin_13); /*!< ����� */
  // GPIO_OutH(GPIOA, GPIO_Pin_13);                    /*!< ����� */
  /*---------------------------------------------------------------
   *                  GPIOB
  -*-------------------------------------------------------------*/
  GPIO_OutL(GPIOB, 0xFFFF);        /*!< ����� */
  GPIO_InPU(GPIOB, GPIO_Pin_13);   //  /PC1*!< �������� */
  GPIO_SetBits(GPIOB, GPIO_Pin_6); // �رյ��
  /*---------------------------------------------------------------
 *                  GPIOC
-*-------------------------------------------------------------*/
  GPIO_OutL(GPIOC, 0xFFFF);                                 /*!< ����� */
  GPIO_InPU(GPIOC, GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_12); //  /PC1*!< �������� */
  GPIO_InHZ(GPIOC, GPIO_Pin_14);                            // BLFDL
  /*---------------------------------------------------------------
 *                  GPIOD
-*-------------------------------------------------------------*/
  GPIO_OutL(GPIOD, 0xFFFF); /*!< ����� */
}
/*----------------------------------------- E.N.D -----------------------------------------*/
