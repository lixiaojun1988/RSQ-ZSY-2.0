#include "ALL_HEAD.H"

ST_SEGCTRL_T stSegCtrl;
static void RefreshSwitchPnt(void);
static void GetSegSet(void);
/**********************************************
 *
 *
 **********************************************/
const ST_SEGCTRL_T *GetSegCtrl(void)
{
	return &stSegCtrl;
}
static uint16_t SegSumLimit(uint16_t _u16Value, uint16_t _u16Variance, uint8_t _Sum)
{
	int32_t _int32_temp;
	if (0 < _Sum)
	{
		_int32_temp = (int32_t)_u16Value + _u16Variance;
	}
	else
	{
		_int32_temp = (int32_t)_u16Value - _u16Variance;
	}

	_int32_temp = RANGLMT(_int32_temp, 0, 0xFFFF);
	return (uint16_t)_int32_temp;
}

/**********************************************
 *
 *
 **********************************************/
static void RefreshSwitchPnt(void)
{
	uint8_t _u8segMax;
	uint16_t uint16_temp;
	uint16_t u16_uppermin, u16_lowermax;

	_u8segMax = GetWorkCon()->u8MaxSeg;

	stSegCtrl.astSegLoad[_u8segMax].u16MaxLoad = GetTmpCtrlData()->astSegLoad[_u8segMax].u16MaxLoad;
	stSegCtrl.astSegLoad[0].u16MinLoad = GetTmpCtrlData()->astSegLoad[0].u16MinLoad;

	while (_u8segMax > 0)
	{
		u16_uppermin = GetTmpCtrlData()->astSegLoad[_u8segMax].u16MinLoad;
		u16_lowermax = GetTmpCtrlData()->astSegLoad[_u8segMax - 1].u16MaxLoad;
		if (u16_lowermax > u16_uppermin)
		{
			uint16_temp = u16_lowermax - u16_uppermin; // 7000
			if (uint16_temp > 1600)
			{
				uint16_temp -= 1600;									  // 5400
				uint16_temp /= 2;										  // 2700
				u16_uppermin = SegSumLimit(u16_uppermin, uint16_temp, 1); // 16000
				// u16_uppermin += uint16_temp;
				u16_lowermax = SegSumLimit(u16_lowermax, uint16_temp, 0); // 17300
																		  // u16_lowermax -= uint16_temp;
			}
			else
			{
				uint16_temp = 1600 - uint16_temp;
				uint16_temp /= 2;
				u16_uppermin = SegSumLimit(u16_uppermin, uint16_temp, 0);
				// u16_uppermin -= uint16_temp;
				u16_lowermax = SegSumLimit(u16_lowermax, uint16_temp, 1);
				// u16_lowermax += uint16_temp;
			}
		}
		else
		{
			uint16_temp = u16_uppermin - u16_lowermax;
			uint16_temp = 1600 + uint16_temp;
			uint16_temp /= 2;
			u16_uppermin = SegSumLimit(u16_uppermin, uint16_temp, 0);
			// u16_uppermin -= uint16_temp;
			u16_lowermax = SegSumLimit(u16_lowermax, uint16_temp, 1);
			// u16_lowermax += uint16_temp;
		}

		stSegCtrl.astSegLoad[_u8segMax].u16MinLoad = u16_uppermin;
		stSegCtrl.astSegLoad[_u8segMax - 1].u16MaxLoad = u16_lowermax;
		_u8segMax -= 1;
	}
	_u8segMax = GetWorkCon()->u8MaxSeg;
	while (_u8segMax > 0)
	{
		if (stSegCtrl.astSegLoad[_u8segMax].u16MaxLoad <= stSegCtrl.astSegLoad[_u8segMax - 1].u16MaxLoad)
		{
			stSegCtrl.astSegLoad[_u8segMax].u16MaxLoad = stSegCtrl.astSegLoad[_u8segMax - 1].u16MaxLoad + 1;
		}
		_u8segMax -= 1;
	}
}
static void GetSegSet(void)
{
	uint8_t _u8temp;
	uint16_t _u16SetLoad, _u16temp;
	const ST_SEG_LOAD_T *_pstSetInfo = stSegCtrl.astSegLoad;
	_u16SetLoad = RANGLMT(GetTmpCtrlData()->u16SetLoad,
							_pstSetInfo[0].u16MinLoad,
							_pstSetInfo[GetWorkCon()->u8MaxSeg].u16MaxLoad);

	if (0 == stSegCtrl.u8ChgStep)
	{
        //换挡条件（满足其一）：1、目标负荷超过当前分段上限；2、目标负荷超过当前分段下限；3、首次进入稳定燃烧状态
		if ((_u16SetLoad > _pstSetInfo[stSegCtrl.u8Cur].u16MaxLoad) 
        || (_u16SetLoad < _pstSetInfo[stSegCtrl.u8Cur].u16MinLoad)
        || (GetFirstSwSeg()))
		{
			_u8temp = GetWorkCon()->u8MaxSeg;
			while (0 < _u8temp)
			{
				//从大分段开始判断工作范围
				if ((_u16SetLoad <= (_pstSetInfo + _u8temp)->u16MaxLoad) && (_u16SetLoad >= (_pstSetInfo + _u8temp)->u16MinLoad))
				{
					stSegCtrl.u8Set = _u8temp;
					stSegCtrl.u8Cal = _u8temp;
					break;
				}
				_u8temp -= 1;
				stSegCtrl.u8Set = _u8temp;
				stSegCtrl.u8Cal = _u8temp;
			}
		}

        //需要换挡
		if (stSegCtrl.u8Set != stSegCtrl.u8Cur)
		{
            //在目标负荷未发生大变化时，换挡到达一定次数时，锁定换挡
			if (3 <= stSegCtrl.u8ChgCnt)
			{
				stSegCtrl.u8Set = stSegCtrl.u8Cur;
				stSegCtrl.u16SlowSegCnt = 0;
			}
			else
			{
#if (SLOW_SEG_EN)
				//当stSegCtrl.u8ChgCnt = 0时，若温度稳定，则不触发快速换挡，之前进行过换挡，同样不触发快速换挡
				if ((1 <= stSegCtrl.u8ChgCnt) || ((0 == stSegCtrl.u8ChgCnt) && (GetTmpCtrlData()->bHeatStable)))
				{
					//换挡临时标志位
					_u8temp = 0;
					//处于比例阀最大开度，且需要向上换挡
					if ((GetSystemRunData()->u16BlfIRun_Max <= GetSystemRunData()->u16SetBlfI) && (stSegCtrl.u8Set > stSegCtrl.u8Cur))
					{
						//出水温度高于设置温度
						if (GetSystemRunData()->TmpSet > GetSystemRunData()->TmpOut)
						{
							_u16temp = GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut;
							if (_u16temp > SLOW_SEG_TMP)
								INC_B(stSegCtrl.u16SlowSegCnt);
						}
						else
							stSegCtrl.u16SlowSegCnt = 0;
					}
					//处于比例阀最小开度，且需要向下换挡
					else if ((GetSystemRunData()->u16BlfIRun_Min >= GetSystemRunData()->u16SetBlfI) && (stSegCtrl.u8Set < stSegCtrl.u8Cur))
					{
						//设置温度高于出水温度
						if (GetSystemRunData()->TmpSet < GetSystemRunData()->TmpOut)
						{
							_u16temp = GetSystemRunData()->TmpOut - GetSystemRunData()->TmpSet;
							if (_u16temp > SLOW_SEG_TMP)
								INC_B(stSegCtrl.u16SlowSegCnt);
						}
						else
							stSegCtrl.u16SlowSegCnt = 0;
					}
					else
						stSegCtrl.u16SlowSegCnt = 0;

					if (SLOW_SEG_CNT <= stSegCtrl.u16SlowSegCnt)
						_u8temp = 1;

					if (0 == _u8temp)
						stSegCtrl.u8Set = stSegCtrl.u8Cur;
                    else
                        stSegCtrl.u8ChgCnt++;
				}
#else
				stSegCtrl.u8ChgCnt ++;
#endif

			}
		}
#if (SLOW_SEG_EN)
        else
        {
			stSegCtrl.u16SlowSegCnt = 0;
        }
#endif
	}
}
void QuickSwitchSeg(void)
{
	uint8_t _u8temp;
	//uint16_t _u16temp;
	stSegCtrl.u16SegSwitchBlfI = GetBlfIFromPercent(SEG_SWITCH_BLF_MIN);
	if (stSegCtrl.u8Set != stSegCtrl.u8Cur)
	{
		ResetHeatStable();
		if (0 == stSegCtrl.u8ChgStep)
		{
			//判断使用哪一种换挡方式：1、没有重合的分段；2、与重合的分段
			_u8temp = (GetWorkCon()->aunSubSegSet[stSegCtrl.u8Set].BYTE & 0xFE) & (GetWorkCon()->aunSubSegSet[stSegCtrl.u8Cur].BYTE & 0xFE);

			//没有共同火排
			if (0 == _u8temp)
			{
				stSegCtrl.u8ChgStep = 1;
				stSegCtrl.u8ValveStay_100ms = 15;//需要配置传火时间
			}
			else
			{
				stSegCtrl.u8ChgStep = 2;
				stSegCtrl.u8ValveStay_100ms = 0;
				for (_u8temp = 1; _u8temp < 5; _u8temp++)
				{
					//需要打开新的分段，需要一定的负荷进行传火，因此预留一定时间（部分机器存在二次压小情况下传火困难）
					if ((1 == ((GetWorkCon()->aunSubSegSet[stSegCtrl.u8Set].BYTE >> _u8temp) & 0x01)) && (0 == ((GetWorkCon()->aunSubSegSet[stSegCtrl.u8Cur].BYTE >> _u8temp) & 0x01)))
					{
						stSegCtrl.u8ValveStay_100ms = 5;//需要配置传火时间
						break;
					}
				}
			}
		}
	}
	//TODO:后续考虑要不要同时降低风速
	if (1 == stSegCtrl.u8ChgStep) // 换挡中无重合段（无用到）
	{
		GetSystemRunData()->unSetValve.BYTE = GetWorkCon()->aunSubSegSet[stSegCtrl.u8Set].BYTE;
		if (0 < stSegCtrl.u8ValveStay_100ms)
		{
			//两个档位的分段全部打开，方便传火
			GetSystemRunData()->unSetValve.BYTE |= GetWorkCon()->aunSubSegSet[stSegCtrl.u8Cur].BYTE;
			if (2 == stSegCtrl.u8Set)
				GetSystemRunData()->unSetValve.BYTE |= ~(EN_SUB3_BIT);
		}
		else
		{
			stSegCtrl.u8ChgStep = 0;
			stSegCtrl.u8Cur = stSegCtrl.u8Set;
		}
		Reset_Pid();
	}
	else if (2 == stSegCtrl.u8ChgStep) // 换挡有重合段
	{
		//直接按照目标分段打开
		GetSystemRunData()->unSetValve.BYTE = GetWorkCon()->aunSubSegSet[stSegCtrl.u8Set].BYTE;
		if (0 < stSegCtrl.u8ValveStay_100ms)
		{
		}
		else
		{
			stSegCtrl.u8ChgStep = 0;
			stSegCtrl.u8Cur = stSegCtrl.u8Set;
		}
		Reset_Pid();
	}
	else
		GetSystemRunData()->unSetValve.BYTE = GetWorkCon()->aunSubSegSet[stSegCtrl.u8Cur].BYTE;
}
uint16_t GetBlfIFromSeg(uint8_t _u8seg)
{
	uint16_t _ret;
	_ret = (uint16_t)((uint32_t)GetSystemRunData()->u16BlfIRun_E * _u8seg / 9 + GetSystemRunData()->u16BlfIRun_Min);
	return _ret;
}
/**********************************************
 *重置换挡次数
 *
 **********************************************/
void ClrSubChgCnt(void)
{
	stSegCtrl.u8ChgCnt = 0;
	ResetHeatStable();
}
/**********************************************
 *
 *
 **********************************************/
void SetManualSeg(uint8_t _u8set)
{
	_u8set = GETMIN(_u8set, GetWorkCon()->u8MaxSeg);
	stSegCtrl.u8Set = _u8set;
}
/**********************************************
 *
 *
 **********************************************/
void SegCtrl_Timer(void)
{
	DEC(stSegCtrl.u8ValveStay_100ms);
	DEC(stSegCtrl.u8Switching_100ms);
}

void SegCtrlInit(void)
{
	stSegCtrl.u8ChgStep = 0;
	stSegCtrl.u8Cur = GetWorkCon()->u8FireSegSet;
	stSegCtrl.u8Set = stSegCtrl.u8Cur;
}
void SegCtrlProcess(void)
{
	SegCtrl_Timer();
	RefreshSwitchPnt(); // 刷新分段阀重合段
	GetSegSet();
	QuickSwitchSeg();
}


// static void fd_val_ctrl(int Tout, int Tset)
// {
//     unsigned char i;

//     if (SWITCH_CHECK == ForceSwitchFlag)
//     {
//         /************************************************************************/
//         /* 根据负荷需求,设置目标分段                                                                     */
//         /************************************************************************/

//         /* 检测是否需要向上切换 */
//         for (i = current_sub_index; i < sub_num; i++)
//         {
//             if (cal_Load < (curve_info_ptr + i)->switch_up_load)
//             {
//                 break;
//             }
//         }
//         /* 检测是否需要向下切阀 */
//         while (i)
//         {
//             if (cal_Load > (curve_info_ptr + i)->switch_down_load)
//             {
//                 break;
//             }
//             i--;
//         }

//         target_sub_index = i;

//         /************************************************************************/
//         /* 根据燃烧状态, 检测是否需要慢开发                                                                   */
//         /************************************************************************/

//         /* 慢速向上切阀 */
//         if (
//             /* 当前不是最大分段组合 */
//             (current_sub_index < (sub_num - 1)) &&
//             /* PWM输出到最大 */
//             (Power_temp1 >= ((curve_info_ptr + current_sub_index)->pwm_max - 5)) &&
//             /* 出水温度比设置温度低1度以上 */
//             ((Tout + 10) < Tset) &&
//             /* 处于正常恒温模式,前馈+反馈 */
//             (ControlMode == NORMAL)
//             )
//         {
//             /* 持续10秒 */
//             ForceCounter++;
//             if (ForceCounter >= 100)
//             {
//                 ForceCounter = 0;

//                 ForceLoad = (unsigned short)cal_Load;
//                 target_sub_index = current_sub_index;
//                 target_sub_index++;
//                 ForceSwitchFlag = LOAD_CHECK;
//             }
//         }

//         /* 慢速向下切阀 */
//         else
//             if (
//                 /* 当前不是最小分段组合 */
//                 (current_sub_index > 0) &&
//                 /* PWM输出已降到最小 */
//                 (Power_temp1 <= (curve_info_ptr + current_sub_index)->pwm_min) &&
//                 /* 出水温度比设置温度高1度以上 */
//                 (Tout > (Tset + 10)) &&
//                 /* 处于正常恒温模式,前馈+反馈 */
//                 (ControlMode == NORMAL)
//                 )
//             {
//                 ForceCounter++;

//                 if (
//                     /* 温度上升的趋势较大且该状态超过4秒 */
//                     (out_tempdot > 2 && ForceCounter >= 40) ||
//                     /* 该状态持续超过10秒 */
//                     (ForceCounter >= 100)
//                     )
//                 {
//                     ForceCounter = 0;

//                     ForceLoad = (unsigned short)cal_Load;
//                     target_sub_index = current_sub_index;
//                     target_sub_index--;
//                     ForceSwitchFlag = LOAD_CHECK;
//                 }
//             }
//             else
//             {
//                 ForceCounter = 0;
//             }
//     }
//     else if (LOAD_CHECK == ForceSwitchFlag)
//     {
//         /* 需求的负荷与慢切阀时的负荷相差超过50(0.5L) */
//         if ((cal_Load + 50 < ForceLoad) || (cal_Load > ForceLoad + 50))
//         {
//             ForceCounter = 0;
//             ForceSwitchFlag = SWITCH_CHECK;
//         }
//     }
// }
