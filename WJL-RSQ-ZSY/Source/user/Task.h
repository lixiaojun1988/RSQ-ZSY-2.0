#ifndef __TASK_H__
#define __TASK_H__

typedef void (*pFun)(void) ;

extern void taskTick(void);
extern void task(void);
extern	void softInit(void);
#endif
