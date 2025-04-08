
/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		FSM有限状态机
 *************************************************************************/

#include "all_head.h"
/************************************************************************/

FsmStateEntryTable const c_xsFsmStateEntryTable[FSM_STATE_SUM] =
	{
		{
			FSM_STATE_IDLE,
			FsmIdleEnterHandler,
			FsmIdleEnterDoingHandler,
		},

		{
			FSM_STATE_INIT,
			FsmInitEnterHandler,
			FsmInitDoingHandler,
		},

		{
			FSM_STATE_PREFAN,
			FsmPreFanEnterHandler,
			FsmPreFanDoingHandler,
		},
		{
			FSM_STATE_IGINTION,
			FsmIgintionEnterHandler,
			FsmIgintionDoingHandler,
		},
		{
			FSM_STATE_STABLE,
			FsmStableEnterHandler,
			FsmStableDoingHandler,
		},
		{
			FSM_STATE_ENDCLEAN,
			FsmEndCleanEnterHandler,
			FsmEndCleanDoingHandler,
		},
		{
			FSM_STATE_ERROR,
			FsmErrorEnterHandler,
			FsmErrorDoingHandler,
		},
		{
			FSM_STATE_DEBUG,
			FsmDebugEnterHandler,
			FsmDebugDoingHandler,
		},
		{
			FSM_STATE_F6,
			FsmF6EnterHandler,
			FsmF6DoingHandler,
		},
        {
			FSM_STATE_E0,
			FsmE0EnterHandler,
			FsmE0DoingHandler,
		},
};

FSM_Para xsFSM_Para;

void FSM(void)
{
	switch (xsFSM_Para.eStage)
	{
	case FSM_STAGE_ENTER:
	{
		c_xsFsmStateEntryTable[xsFSM_Para.eState].pfFsmStateEnterHandler();
	}
	break;
	case FSM_STAGE_DOING:
	{
		c_xsFsmStateEntryTable[xsFSM_Para.eState].pfFsmStateDoingHandler();
	}
	break;
	default:
		break;
	}
}

void SetFsmState(FSM_STATE eTargetState)
{
	xsFSM_Para.eState = eTargetState;
	SetFsmStage(FSM_STAGE_ENTER);
}

void SetFsmStage(FSM_STAGE eTargetStage)
{
	xsFSM_Para.eStage = eTargetStage;
}

FSM_STATE GetFsmState(void)
{
	return xsFSM_Para.eState;
}
