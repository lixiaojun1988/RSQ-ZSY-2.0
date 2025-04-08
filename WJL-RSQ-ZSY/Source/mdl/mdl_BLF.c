#include "ALL_HEAD.H"

static uint16_t u16SetBlfOut;
static uint8_t u8DelaySet = 0;
static uint16_t u16CurBlfOut;
#define BLF_PWM_CYCLE 6000

void Drv_SetBlfPWM(uint16_t _u16SetBlfI)
{
	_u16SetBlfI = (uint16_t)(((uint32_t)_u16SetBlfI - BLF_MIN) * (BLF_PWM_CYCLE) / (BLF_MAX - BLF_MIN));
    u16CurBlfOut = _u16SetBlfI;
	PCA_SetCCRValue(PCA2, PCA_CHANNEL_B, _u16SetBlfI);
}

void Blf_Iteration_10ms(void)
{
	uint16_t	_u16BlfQuickReach;
	uint16_t _u16Temp, _u16TempE;
	uint16_t _u16SetBlfI = GetSystemRunData()->u16SetBlfI;

	_u16SetBlfI = RANGLMT(_u16SetBlfI, GetSystemRunData()->u16BlfIRun_Min, GetSystemRunData()->u16BlfIRun_Max);

	if (0 < _u16SetBlfI)
	{
		if (0 == u8DelaySet)
		{
			//��������ٱ仯�ı���������
            _u16BlfQuickReach = (uint32_t)(GetSystemRunData()->u16BlfIRun_E) * GetWorkCon()->u8GasQucikReachPer / 100;

			// ��Ҫ����
			if (_u16SetBlfI >= u16SetBlfOut)
			{
				//��Ҫ���ӵı���������
				_u16TempE = _u16SetBlfI - u16SetBlfOut;
				//�����仯����ֵ
				_u16Temp = (uint32_t)(GetSystemRunData()->u16BlfIRun_E) * GetWorkCon()->u16AddGasSpd / 1000;
				//����Ӧ����ӷ�
				_u16Temp = (uint32_t)_u16Temp * getAdapData()->AddGasSpedPer / 100;
				if ((_u16TempE > _u16Temp) && (_u16TempE >= _u16BlfQuickReach))
				{
					if (1 > _u16Temp)
					{
						_u16Temp = 1;
					}
					u16SetBlfOut += _u16Temp;
				}
				else
				{
					u16SetBlfOut = _u16SetBlfI;
				}
			}
			//��Ҫ����
			else
			{
				//��Ҫ���ӵı���������
				_u16TempE = u16SetBlfOut - _u16SetBlfI;
				//�����仯����ֵ
				_u16Temp = (uint32_t)(GetSystemRunData()->u16BlfIRun_E) * GetWorkCon()->u16DecGasSpd / 1000;

				if ((_u16TempE > _u16Temp) && (_u16TempE >= _u16BlfQuickReach))
				{
					if (1 > _u16Temp)
					{
						_u16Temp = 1;
					}
					u16SetBlfOut -= _u16Temp;
				}
				else
				{
					u16SetBlfOut = _u16SetBlfI;
				}
			}
		}
		else
		{
			DEC(u8DelaySet);
		}
	}
	else
	{
		u16SetBlfOut = _u16SetBlfI;
	}
	Drv_SetBlfPWM(u16SetBlfOut);
}
void MDL_SetBlfOut(uint16_t _u16SetBlfI)
{
	u16SetBlfOut = _u16SetBlfI;
}
uint16_t GetBLFRealOut(void)
{
	return u16SetBlfOut;
}

uint16_t GetBlfCurOut(void)
{
    return u16CurBlfOut;
}