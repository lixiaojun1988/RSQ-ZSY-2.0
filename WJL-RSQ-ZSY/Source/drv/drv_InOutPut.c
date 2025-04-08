#include "ALL_HEAD.H"

ST_IN_PORT_REALTIME_T stPortsRealTime;
ST_BOARD_INPUTS_T stInputsSta;

/**********************************************
 *
 *
 **********************************************/
const ST_BOARD_INPUTS_T *GetInportsSta(void)
{
	return &stInputsSta;
}

/**********************************************
 *
 *
 **********************************************/
// uint8_t GetWaveNewSta(void)
//{
// return ((P13_bit.no7) | (P5_bit.no1 << 1));
//}
/**********************************************
 *
 *
 **********************************************/

/**********************************************
 *
 *
 **********************************************/
static void InputPortsUpdate(void)
{
	stPortsRealTime.unNewSta.bits.bOverTmpChk = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6); // PC1  温控器超温检测
	stPortsRealTime.unNewSta.bits.bValveCheck = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12); // 阀短路检测
	stPortsRealTime.unNewSta.bits.bFanPressSw = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13); // 风压开关
	// stPortsRealTime.unNewSta.bits.bWaterFluxIO = P13_bit.no7;
	// stPortsRealTime.unNewSta.bits.bFanSpeedIO = P5_bit.no1;
//	stPortsRealTime.unNewSta.bits.bFireFeedBack = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
	//stPortsRealTime.unNewSta.bits.bWBLFReset = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
	//stPortsRealTime.unNewSta.bits.bPumpSpeedIO = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}
/**********************************************
 *
 *
 **********************************************/
static void InputPortsFilter(void)
{
	uint8_t _u8ports = 0;
	uint8_t _u8newbit = 0;
	uint8_t _u8oldbit = 0;
	for (_u8ports = 0; _u8ports < EN_INPORT_MAX; _u8ports++)
	{
		_u8newbit = (stPortsRealTime.unNewSta.u8Byte & (1 << _u8ports));
		_u8oldbit = (stPortsRealTime.unLastSta.u8Byte & (1 << _u8ports));
		if (_u8oldbit != _u8newbit)
		{
			if (PORT_FILER_CNT <= stPortsRealTime.u8KeepTime[_u8ports])
			{
				stPortsRealTime.unLastSta.u8Byte &= ~(1 << _u8ports);
				stPortsRealTime.unLastSta.u8Byte |= _u8newbit;
			}
			else
			{
				INC_B(stPortsRealTime.u8KeepTime[_u8ports]);
			}
		}
		else
		{
			stPortsRealTime.u8KeepTime[_u8ports] = 0;
		}
	}
	stInputsSta = stPortsRealTime.unLastSta.bits;
}
/**********************************************
 *
 *
 **********************************************/
void Drv_OutPut_Set(EN_OUTPORTS_T enPort, uint8_t _u8set)
{
	if (EN_OUTPORT_ValveAll == enPort)
	{
		
	}
	else if (EN_OUTPORT_Valve1 == enPort)
	{
		if (0 < _u8set)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_9);
		}
		else
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}
	}
	else if (EN_OUTPORT_Valve2 == enPort)
	{
		if (0 < _u8set)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_8);
		}
		else
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		}
	}
	else if (EN_OUTPORT_Valve3 == enPort)
	{
		if (0 < _u8set)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_7);
		}
		else
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		}
	}
	else if (EN_OUTPORT_Valve4 == enPort)
	{
		if (0 < _u8set)
		{
			//GPIO_SetBits(GPIOC, GPIO_Pin_5);
		}
		else
		{
			//GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		}
	}
	else if (EN_OUTPORT_ValveMain == enPort)
	{
		if (0 < _u8set)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_10);
		}
		else
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_10);
		}
	}
	else if (EN_OUTPORT_WBLF == enPort)
	{
	}
	else if (EN_OUTPORT_WPump == enPort)	//调速水泵
	{
		if (0 < _u8set)
		{
			// Drv_SetPumpPWM(GetFlashDataSector0()->debugData.u8Pu);
            Drv_SetPumpPWM(0Xf0);
		}
		else
		{
			Drv_SetPumpPWM(0);
		}
	}
	else if (EN_OUTPORT_IGINTION == enPort)
	{
		#if (1 == DIS_IGNITION)
            GPIO_SetBits(GPIOB, GPIO_Pin_7);
		#else
		if (0 < _u8set)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		}
		else
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		}
		#endif
	}
    else if (EN_OUTPORT_FjBrake == enPort)
    {
        if (0 < _u8set)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
		}
    }
}

void Drv_InOutput_Init(void)
{
	// 输出关闭
	for (uint8_t i = 0; i < EN_OUTPORT_MAX; i++)
		Drv_OutPut_Set((EN_OUTPORTS_T)i, Disable);
}

void Drv_InOutput_Process(void) // 1MS
{
	InputPortsUpdate();
	InputPortsFilter();
}
