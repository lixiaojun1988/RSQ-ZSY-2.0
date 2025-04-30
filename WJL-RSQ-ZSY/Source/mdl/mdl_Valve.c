#include "ALL_HEAD.H"

static MDL_VALVE_OUTPUT_T stValveOutput;

#define VALVE_NUM_FCT_MAX 5
#define VALVE_CHECK_TIME 50
static uint16_t u16ValveForceCnt[VALVE_NUM_MAX] = {MDL_VALVE_FORCE_TIME, MDL_VALVE_FORCE_TIME, MDL_VALVE_FORCE_TIME};

static EN_OUTPORTS_T aENValvePorts[VALVE_NUM_MAX] =
    {
        EN_OUTPORT_ValveMain,
        EN_OUTPORT_Valve1,
        EN_OUTPORT_Valve2,
				EN_OUTPORT_Valve3,
			};

UN_VALVE_ON_T unValveOn;

static UN_VLALVE_FORCE_T unValveForce;

void SetValveOnOff(EN_VALVE_T index, uint8_t state)
{
    if (Enable == state)
        unValveOn.BYTE |= 0x01 << index;
    else
        unValveOn.BYTE &= 0xff ^ (0x01 << index);
}

uint8_t GetValveOnOffState(EN_VALVE_T index)
{
    return unValveOn.BYTE & (0x01 << index);
}

UN_VALVE_ON_T *pGetValveState(void)
{
    return &unValveOn;
}

UN_VALVE_ERROR_T GetValveError(void)
{
    return stValveOutput.unError;
}

uint8_t GetValveOpenSta(void)
{
    return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
}
uint8_t GetValveShortSta(void)
{
    return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
}

uint8_t ValveCheckFunction(void)
{
#if (1 == UNITTEST)
    return 0;
#else
    uint8_t _u8i;
    uint8_t IO_Temp;
    uint8_t _u8temp = 5;
    uint8_t _u8Short = 0;
    uint8_t _u8Open = 0;

    if (0 == (stValveOutput.unError.Bits.fgCheckOpen | stValveOutput.unError.Bits.fgCheckShort))
    {
        if (0 < unValveOn.BYTE)
        {
            unValveOn.BYTE = 0;
            for (_u8i = 0; VALVE_NUM_MAX > _u8i; _u8i++)
            {
                Drv_OutPut_Set(aENValvePorts[_u8i], Disable);
                u16ValveForceCnt[_u8i] = MDL_VALVE_FORCE_TIME;
            }
        }
        for (_u8i = 0; VALVE_NUM_MAX > _u8i; _u8i++)
        {
            Drv_OutPut_Set(aENValvePorts[_u8i], Enable);
        }

        IO_Temp = GetValveOpenSta();
        while (0 < _u8temp)
        {
            _u8temp--;
            IO_Temp = GetValveShortSta();
            if (0 == IO_Temp)
            {
                _u8Short = 1;
                break;
            }
        }
        for (_u8i = 0; VALVE_NUM_MAX > _u8i; _u8i++)
        {
            Drv_OutPut_Set(aENValvePorts[_u8i], Disable);
        }
        if (0 == _u8Short)
        {
            _u8temp = VALVE_CHECK_TIME;
            while (_u8temp)
            {
                IO_Temp = GetValveOpenSta(); // 0开路
                if (0 == IO_Temp)
                {
                    _u8Open = _u8temp;
                    break;
                }
                _u8temp--;
            }
        }

        if (0 < _u8Short)
        {
            if (++stValveOutput.u8ValveErrCnt >= 5)
            {
                stValveOutput.unError.Bits.fgCheckShort = 1;
            }
        }
        else if (0 == _u8Open)
        {
            if (++stValveOutput.u8ValveErrOpenCnt >= 5)
            {
                stValveOutput.unError.Bits.fgCheckOpen = 1;
            }
        }
        else
        {
            stValveOutput.u8ValveErrCnt = 0;
            stValveOutput.u8ValveErrOpenCnt = 0;
        }
    }

    if (stValveOutput.unError.Bits.fgCheckOpen | stValveOutput.unError.Bits.fgCheckShort)
    {
        _u8i = 1;
    }
    else
    {
        _u8i = 0;
    }
#if (1 == DIS_VALVE_ERR)
    _u8i = 0;
	stValveOutput.unError.BYTE = 0;
#endif

    return _u8i;

#endif
}

void ValveWorkFunction(void) // 10MS
{
    uint8_t _u8i;
    static uint8_t ValveWorkFunction_tick = 0;
    if (++ValveWorkFunction_tick < 10) // 100MS
        return;
    ValveWorkFunction_tick = 0;
    unValveOn.BYTE = GetSystemRunData()->unSetValve.BYTE;
#if (0 == DIS_VALVE_ERR)
    if (0 == unValveForce.BYTE)
    {
        if (1 < stValveOutput.u8ValveOpenCnt)
        {
            INC_B(stValveOutput.u8ValveErrorWorkCnt);
        }
        else
        {
            stValveOutput.u8ValveErrorWorkCnt = 0;
        }
        stValveOutput.u8ValveOpenCnt = 0;
        if (4 < stValveOutput.u8ValveErrorWorkCnt)
        {
            stValveOutput.unError.Bits.fgWorkOpen = 1;
        }
    }
#else
	stValveOutput.unError.BYTE = 0;
#endif

    if (0 < unValveOn.BYTE)
    {
        if (0 < stValveOutput.unError.Bits.fgWorkOpen)
        {
            unValveOn.BYTE = 0;
            unValveForce.BYTE = 0;
        }
        else
        {
            for (_u8i = 0; VALVE_NUM_MAX > _u8i; _u8i++)
            {

                if (0 < (unValveOn.BYTE & (1 << _u8i)))
                {
                    unValveOn.BYTE |= (0x01 << _u8i);
                    if (0 < u16ValveForceCnt[_u8i])
                    {
                        u16ValveForceCnt[_u8i]--;
                        unValveForce.BYTE |= (0x01 << _u8i);
                    }
                    else
                    {
                        unValveForce.BYTE &= ~(0x01 << _u8i);
                    }
                }
                else
                {
                    unValveOn.BYTE &= ~(0x01 << _u8i);
                    unValveForce.BYTE &= ~(0x01 << _u8i);
                }
            }
        }
    }
    else
    {
        unValveOn.BYTE = 0;
        unValveForce.BYTE = 0;
        stValveOutput.u8ValveErrorWorkCnt = 0;
    }
    for (_u8i = 0; VALVE_NUM_MAX > _u8i; _u8i++)
    {
        if (0 == (unValveOn.BYTE & (1 << _u8i)))
        {
            u16ValveForceCnt[_u8i] = MDL_VALVE_FORCE_TIME;
        }
    }
}

void ValveWorkCtrl(void) // 1MS
{
    static uint8_t u8ValveCnt;
    static uint8_t VALVE_LV = 0;
    uint8_t IO_Temp;
    uint16_t PinState=0,PinState2=0;
    if (++u8ValveCnt >= 2)
    {
        u8ValveCnt = 0;
        VALVE_LV = ~VALVE_LV;
        if (0 < VALVE_LV)
        {
            IO_Temp = GetValveOpenSta();
            if (0 < IO_Temp)
            {
                INC_B(stValveOutput.u8ValveOpenCnt);
            }
            if (unValveOn.Bits.fgMainON)PinState|=GPIO_Pin_10;
                //  Drv_OutPut_Set(EN_OUTPORT_ValveMain, Enable);
              
            if (unValveOn.Bits.fg1ON)PinState|=GPIO_Pin_9;
                //  Drv_OutPut_Set(EN_OUTPORT_Valve1, Enable);
              
            if (unValveOn.Bits.fg2ON)PinState|=GPIO_Pin_8;
                //  Drv_OutPut_Set(EN_OUTPORT_Valve2, Enable);
              
            if (unValveOn.Bits.fg3ON)PinState|=GPIO_Pin_7;
                //  Drv_OutPut_Set(EN_OUTPORT_Valve3, Enable);
              
           // if (unValveOn.Bits.fg4ON)PinState|=GPIO_Pin_5;
                // Drv_OutPut_Set(EN_OUTPORT_Valve4, Enable);
              
            GPIO_SetBits(GPIOC, PinState);
        }
        else
        {
            if (unValveForce.Bits.fgMainForce)PinState|=GPIO_Pin_10;
                // Drv_OutPut_Set(EN_OUTPORT_ValveMain, Enable);
            else PinState2|=GPIO_Pin_10;
                // Drv_OutPut_Set(EN_OUTPORT_ValveMain, Disable);
            if (unValveForce.Bits.fg1Force)PinState|=GPIO_Pin_9;
                // Drv_OutPut_Set(EN_OUTPORT_Valve1, Enable);
            else PinState2|=GPIO_Pin_9;
                //  Drv_OutPut_Set(EN_OUTPORT_Valve1, Disable);
            if (unValveForce.Bits.fg2Force)PinState|=GPIO_Pin_8;
                //   Drv_OutPut_Set(EN_OUTPORT_Valve2, Enable);
            else PinState2|=GPIO_Pin_8;
                //  Drv_OutPut_Set(EN_OUTPORT_Valve2, Disable);
            if (unValveForce.Bits.fg3Force)PinState|=GPIO_Pin_7;
            //  Drv_OutPut_Set(EN_OUTPORT_Valve3, Enable);
            else PinState2|=GPIO_Pin_7;
                //   Drv_OutPut_Set(EN_OUTPORT_Valve3, Disable);
          //  if (unValveForce.Bits.fg4Force)PinState|=GPIO_Pin_5;
                //  Drv_OutPut_Set(EN_OUTPORT_Valve4, Enable);
           // else PinState2|=GPIO_Pin_5;
                //    Drv_OutPut_Set(EN_OUTPORT_Valve4, Disable);
             GPIO_ResetBits(GPIOC, PinState2);
             GPIO_SetBits(GPIOC, PinState);   
        }
    }
}

void ValveErrorReset(void)
{
    stValveOutput.unError.BYTE = 0;
    stValveOutput.u8ValveErrCnt = 0;
    stValveOutput.u8ValveErrOpenCnt = 0;
    stValveOutput.u8ValveErrorWorkCnt = 0;
}
