
/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:      V1
 * Brief:        万家乐热水器第三代恒温算法
 * date：        2024-12-05
 *************************************************************************/

#ifndef __WJL_RSQ_AI_V1_H__
#define __WJL_RSQ_AI_V1_H__


extern void Pid_Init(void);									// PID初始化函数,清0变量
extern int16_t PID_realize(SystemRunDataTypeDef *pSysData); // PID处理函数
#endif
