
#ifndef _FSM_H_
#define _FSM_H_

typedef enum
{
    FSM_STATE_IDLE, //
    FSM_STATE_INIT,
    FSM_STATE_PREFAN,
    FSM_STATE_IGINTION,
    FSM_STATE_STABLE,
    FSM_STATE_ENDCLEAN,
    FSM_STATE_ERROR,
    FSM_STATE_DEBUG,
    FSM_STATE_F6,
    FSM_STATE_E0,
    FSM_STATE_SUM
} FSM_STATE;

typedef enum
{
    FSM_STAGE_ENTER,
    FSM_STAGE_DOING,
} FSM_STAGE;

typedef void (*PFFsmStateEnterHandler)(void);
typedef void (*PFFsmStateDoingHandler)(void);

typedef struct
{
    FSM_STATE eState;
    PFFsmStateEnterHandler pfFsmStateEnterHandler;
    PFFsmStateDoingHandler pfFsmStateDoingHandler;
} FsmStateEntryTable, *PFsmStateEntryTable;

typedef struct
{
    FSM_STATE eState;
    FSM_STAGE eStage;
} FSM_Para, *pFSM_Para;

extern void FSM(void);
extern void SetFsmState(FSM_STATE eTargetState);
extern void SetFsmStage(FSM_STAGE eTargetStage);
extern FSM_STATE GetFsmState(void);

#endif
