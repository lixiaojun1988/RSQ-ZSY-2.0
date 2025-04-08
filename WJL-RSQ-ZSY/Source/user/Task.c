
/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾绘晸閿燂拷
 *************************************************************************/
#include "all_head.h"
#define TasksetTick (10)
typedef struct
{
    unsigned char isReady : 1;
    unsigned int currentTick;
    unsigned int setTick;
    void (*pFun)(void);
} TaskType;

// 闁圭厧鐡ㄥú鏍儑閻楀牄浠氶柛妤冨仜琚熸繝銏ｆ硾鐎氼噣骞冩繝鍥х闁兼亽鍎插▓锟�
void drv_handle(void)
{
    Comm_Recv();
    Drv_InOutput_Process();
    GetWaveTask();
}
// 婵炴垶鎼╅崣鍐ㄎ涢幐搴ｎ浄閻犺桨璀﹀Σ閬嶆煟閻愬弶顥滈柛銈変憾瀵偊鏁撻敓锟�
void mdl_handle(void) // 10MS
{
    mdl_adc_handle();
    ValveWorkFunction();
    // mdlFilterhandle();
    Blf_Iteration_10ms();
    MDL_SetBlfOut(GetSystemRunData()->u16SetBlfI); // 闂婎偄娲ら幊姗€濡撮敓锟�10MS
}
// 闁圭厧鐡ㄥ濠氬极閵堝洣娌柛灞惧嚬濡查亶鏌ｉ悙鍙夘棞闁搞倝浜跺顐︽晸閿燂拷
void user_handle(void)
{
    user_temper();
    Comm_Send();
    DCFanProcess();
    systemRun();
    FanTypeJudge();
    WashModeFunction();
    FunFunction();
    PumpFunction();
    user_adaptive();
    //GPIO_ToggleBits(GPIOB, GPIO_Pin_8);//闁荤姳璁查崜婵嗩渻閸岀偛绠抽柕蹇嬪€栫€氭盯鏌ㄥ☉妯肩伇闁轰降鍊栫粋宥嗘償閵忊晜鈷曞┑鐐存綑椤戝嫮鈧潧鐭傚畷娆撴嚍閵夛附顔嶉柡澶屽剱閸犳盯顢氬Δ鍛強妞ゆ牗纰嶉崕濠勨偓鐐瑰€栭崕鑲╂崲閿燂拷
}

unsigned char Uart5SendFlag;
int fputc(int ch, FILE *f)
{
    // 闂佸憡鍑归崹鐗堟叏閳哄懎鐭楅柟杈捐吂閸嬫捇鏁撻敓锟�
    UART5->FR.BIT.TCC = 1; // 濠电偞鎸搁幊蹇涙偉閿濆牏鐤€婵＄偛澧界粔锟�
    UART5->TDR.BIT.TDR = ch;
    while (!Uart5SendFlag)
        ;
    Uart5SendFlag = 0;
    return ch;
}

static TaskType TasksTab[] =
    {
        {0, 0, 1, drv_handle},
        {0, 0, 10, FSM},
        {0, 0, 10, user_handle},
        {0, 0, 10, mdl_handle},


};

void taskTick(void)
{
    unsigned char i = 0;
    for (; i < sizeof(TasksTab) / sizeof(TaskType); i++)
    {
        if (++TasksTab[i].currentTick < TasksTab[i].setTick)
        {
            continue;
        }
        TasksTab[i].currentTick = 0;
        TasksTab[i].isReady = 1;
    }
}

void task(void)
{
    unsigned char i = 0;
    for (; i < sizeof(TasksTab) / sizeof(TaskType); i++)
    {
        if (TasksTab[i].isReady == 0)
        {
            continue;
        }
        TasksTab[i].isReady = 0;
        TasksTab[i].pFun();
    }
}

void softInit(void)
{
	Drv_InOutput_Init();
    user_temper_init();
    usartInit();
    Comm_Init();
    SystemFlashInit(); // 婵炶揪绲界粔鍫曟偪閸℃鈻旂€广儱鐗嗛崢鎾⒒閸涱厾绠查柛鎾村閹奉偊宕橀埡浣哥哎
    sysTemRunInit();
    SegCtrlInit();
    TmpCtrlInit();
    GetWaveInit();
    WashModeInit();
    FunctionInit();
    PumpFunctionInit();
    user_adaptive_init();
    SetFsmState(FSM_STATE_IDLE);
}
