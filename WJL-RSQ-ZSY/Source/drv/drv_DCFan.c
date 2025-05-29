#include	"ALL_HEAD.H"

#define FJ_PWN_CYCLE  12000

//每10ms的最大变化频率
#define FAN_QUICK_PER	50//风机快速变化

#define FAN_SLOW_TIME	300


uint16_t DCFanSet;
uint8_t DcBrake_10ms;

#if (FanDutySlow)
void	Drv_SetDCFanPWM(uint16_t _u16SetI)
{
	static uint16_t u16CurFanI = 0;//缓慢更改风机变化
	static uint16_t u16Slow_10ms = 0;//大风速变化的时候，做一定时间限制，限制风机转速变化
	static uint16_t u16LimitFanI = 0;
	uint16_t u16TempE,u16Temp;
	uint16_t u16TempMin;//每10ms的缓慢变化
	DEC(u16Slow_10ms);
	_u16SetI = RANGLMT(_u16SetI, FANU_MIN, FANU_MAX);
	//一次性可调最大范围判断可调范围
	if (GetDCFanData()->u16FanRunMax > GetDCFanData()->u16FanRunMin)
	{
		u16TempE = GetDCFanData()->u16FanRunMax - GetDCFanData()->u16FanRunMin;
		//计算出当前风机占空比变化值
		if (_u16SetI > u16CurFanI)
			u16Temp = _u16SetI - u16CurFanI;
		else
			u16Temp = u16CurFanI - _u16SetI;

		//变化超过一半，则做限制
		if (u16Temp > u16TempE / 2)
		{
			u16Slow_10ms = d;
			u16LimitFanI = u16CurFanI;
		}
			
		
		if (0 == u16Slow_10ms)
			_u16SetI = (uint16_t)(((uint32_t)_u16SetI - FANU_MIN) * (FJ_PWN_CYCLE) / (FANU_MAX - FANU_MIN));
		else
		{
			if (u16Slow_10ms > (FAN_SLOW_TIME - 100))
			{

			}
			if ()
		}
	}
		
	u16CurFanI = _u16SetI;
	PWM_SetDuty(PWM3,  _u16SetI);///duty
}
#else
uint8_t TestFan = 1;
uint8_t TestBrakeTime;
uint8_t TestA;
uint16_t u16TestA;
uint8_t TestBrakeCnt;
void	Drv_SetDCFanPWM(uint16_t _u16SetI)
{
	uint32_t _u32Tmp;
	_u16SetI = RANGLMT(_u16SetI, FANU_MIN, FANU_MAX);
	_u16SetI = (uint16_t)(((uint32_t)_u16SetI - FANU_MIN) * (FJ_PWN_CYCLE) / (FANU_MAX - FANU_MIN));

	if ((DcBrake_10ms) && (TestFan))
		Drv_OutPut_Set(EN_OUTPORT_FjBrake,1);
	else
		Drv_OutPut_Set(EN_OUTPORT_FjBrake,0);
	DEC(DcBrake_10ms);

	if ((_u16SetI < DCFanSet) && (0 == DcBrake_10ms))
	{
		_u32Tmp = DCFanSet - _u16SetI;
		//占空比变化百分比
		_u32Tmp = _u32Tmp * 100 / FJ_PWN_CYCLE;
        u16TestA = _u32Tmp;
		if (20 < _u32Tmp)
		{
			_u32Tmp = (_u32Tmp - 20) / 16;// （_u16Tmp - 20）/ 80 * 50 / 10
			TestA = _u32Tmp;
			DcBrake_10ms = 5 + _u32Tmp;
			// DcBrake_10ms = TestBrakeCnt;
		}
		else if (5 <= _u32Tmp)
		{
			DcBrake_10ms = 5;
			// DcBrake_10ms = TestBrakeCnt;
		}
		TestBrakeTime = DcBrake_10ms;
	}
	PWM_SetDuty(PWM3,  _u16SetI);///duty
	
	DCFanSet = _u16SetI;
}

uint16_t	GetDCFanSet(void)
{
    return DCFanSet;
}

#endif
