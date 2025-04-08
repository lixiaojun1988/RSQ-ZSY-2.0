/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:         singlecomm.c
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:		 万家乐烟机单线通信协议
 *************************************************************************/
#include "all_head.h"

#define SINGLE_BIT_HEAD_L (30) //低电平头
#define SINGLE_BIT_HEAD_H (5)  //高电平头
#define SINGLE_BIT_LONG (10)   //长电平
#define SINGLE_BIT_SHORT (5)   //短电平
#define SINGLE_BIT_END_L (20)  //低电平尾
#define SINGLE_BIT_END_H (5)   //高电平头

typedef enum
{
    SINGLE_COMM_BIT_HEAD,  //数据头
    SINGLE_COMM_BIT_DATA,  //数据
    SINGLE_COMM_BIT_END,   //数据尾
    SINGLE_COMM_BIT_ERROR, //数据错误
    SINGLE_COMM_BIT_SUM
} SINGLE_COMM_BIT_TYPE;

typedef struct
{
    unsigned char frameOverFlag : 1; //一帧发送完成标志
    unsigned char bitOverFlag : 1;   //位发送完成标志
    unsigned char state;             //状态
    unsigned char stateLast;         //历史状态
    unsigned char cnt;               //位计数
    unsigned char byteCnt;           //字节移位计数
    unsigned char bitCnt;            //位移位计数
} singleCommSendBits;                //单线发送结构体
static data singleCommSendBits singleCommSendBits_st;
typedef struct
{
    unsigned char frameOverFlag : 1; //一帧接收完成标志
    unsigned char bitOverFlag : 1;   //位接收完成标志
    unsigned char state;             //状态
    unsigned char stateLast;
    unsigned char cnt;      //位计数
    unsigned char byteCnt;  //字节移位计数
    unsigned char bitCnt;   //位移位计数
    unsigned char bitCnt_H; //位高位计数
    unsigned char bitCnt_L; //位低位计数
} singleCommRevBits;        //单线接收结构体
static data singleCommRevBits singleCommRevBits_st;
//singleCommError singleCommError_st;

data sinleCommData sinleCommData_st;
//单线发送
void singleCommSendHandle(void)
{
    if (!sinleCommData_st.allowSend)
        return;
    if (singleCommSendBits_st.stateLast != singleCommSendBits_st.state)
    {
        singleCommSendBits_st.stateLast = singleCommSendBits_st.state;
        singleCommSendBits_st.cnt = 0;
        singleCommSendBits_st.byteCnt = 0;
        singleCommSendBits_st.bitCnt = 0;
    }

    switch (singleCommSendBits_st.state)
    {
    case SINGLE_COMM_BIT_HEAD:
        sinleCommData_st.sendIsBusy = 1; //正在发送数据
        if (++singleCommSendBits_st.cnt <= SINGLE_BIT_HEAD_L)
            singleCommSendPort_L;
        else if (singleCommSendBits_st.cnt >= SINGLE_BIT_HEAD_L + SINGLE_BIT_HEAD_H)
            singleCommSendBits_st.state = SINGLE_COMM_BIT_DATA;
        else
            singleCommSendPort_H;
        break;

    case SINGLE_COMM_BIT_DATA:

        if ((sinleCommData_st.sendBuff[singleCommSendBits_st.byteCnt] << singleCommSendBits_st.bitCnt) & 0x80) // 逻辑H
        {
            if (++singleCommSendBits_st.cnt <= SINGLE_BIT_LONG)
            {
                singleCommSendPort_L;
            }
            else if (singleCommSendBits_st.cnt > SINGLE_BIT_LONG && singleCommSendBits_st.cnt <= (SINGLE_BIT_LONG + SINGLE_BIT_SHORT - 1))
            {
                singleCommSendPort_H;
            }
            else
            {
                singleCommSendBits_st.bitOverFlag = 1;
                singleCommSendBits_st.cnt = 0;
            }
        }
        else // 逻辑L
        {
            if (++singleCommSendBits_st.cnt <= SINGLE_BIT_SHORT)
            {
                singleCommSendPort_L;
            }
            else if (singleCommSendBits_st.cnt > SINGLE_BIT_SHORT && singleCommSendBits_st.cnt <= (SINGLE_BIT_LONG + SINGLE_BIT_SHORT - 1))
            {
                singleCommSendPort_H;
            }
            else
            {
                singleCommSendBits_st.bitOverFlag = 1;
                singleCommSendBits_st.cnt = 0;
            }
        }
        if (singleCommSendBits_st.bitOverFlag)
        {
            singleCommSendBits_st.bitOverFlag = 0;
            if (++singleCommSendBits_st.bitCnt > 7)
            {
                singleCommSendBits_st.bitCnt = 0;
                if (++singleCommSendBits_st.byteCnt >= sinleCommData_st.SendLen)
                {
                    singleCommSendBits_st.byteCnt = 0;
                    singleCommSendBits_st.state = SINGLE_COMM_BIT_END;
                }
            }
        }
        break;
    case SINGLE_COMM_BIT_END:
        singleCommSendBits_st.cnt++;
        if (singleCommSendBits_st.cnt <= SINGLE_BIT_END_L)
            singleCommSendPort_L;
        else if (singleCommSendBits_st.cnt > (SINGLE_BIT_END_L + SINGLE_BIT_END_H))
        {
            singleCommSendPort_H; //结束电平拉高
            singleCommSendBits_st.state = SINGLE_COMM_BIT_HEAD;
            sinleCommData_st.sendIsBusy = 0;
            sinleCommData_st.allowSend = 0;
        }
        else
            singleCommSendPort_H;
        break;
    default:
        break;
    }
}

//单线接收
void singleCommRevHandle(void)
{
    if (sinleCommData_st.validRev)
        return;
    if (singleCommRevBits_st.stateLast != singleCommRevBits_st.state)
    {
        singleCommRevBits_st.stateLast = singleCommRevBits_st.state;
        singleCommRevBits_st.cnt = 0;
        singleCommRevBits_st.byteCnt = 0;
        singleCommRevBits_st.bitCnt = 0;
        singleCommRevBits_st.frameOverFlag = false;
        singleCommRevBits_st.bitOverFlag = 0;
        singleCommRevBits_st.bitCnt_H = 0;
        singleCommRevBits_st.bitCnt_L = 0;
    }
    switch (singleCommRevBits_st.state)
    {
    case SINGLE_COMM_BIT_HEAD: //帧头
        if (!singleCommRevBits_st.bitOverFlag)
        {
            if (!singleCommRevPort)
            {
                if (++singleCommRevBits_st.cnt > SINGLE_BIT_HEAD_L + 2)
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
            }
            else
            {
                if (singleCommRevBits_st.cnt < SINGLE_BIT_HEAD_L - 2)
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
                singleCommRevBits_st.bitOverFlag = 1;
                singleCommRevBits_st.cnt = 0;
            }
        }
        else
        {
            if (singleCommRevPort)
            {
                if (++singleCommRevBits_st.cnt > SINGLE_BIT_HEAD_H + 2)
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
            }
            else
            {
                if (singleCommRevBits_st.cnt < SINGLE_BIT_HEAD_H - 2)
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
                singleCommRevBits_st.state = SINGLE_COMM_BIT_DATA;
            }
        }

        break;
    case SINGLE_COMM_BIT_DATA: //数据
        if (!singleCommRevBits_st.bitOverFlag)
        {
            if (!singleCommRevPort) //记录低电平长度
            {
                if (++singleCommRevBits_st.bitCnt_L > SINGLE_BIT_END_L + 5) //比尾电平还要长
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
            }
            else
            {
                singleCommRevBits_st.bitOverFlag = 1;
                singleCommRevBits_st.bitCnt_H++;
            }
        }
        else
        {
            //电平逻辑判断
            if (!singleCommRevPort)
            {
                if (singleCommRevBits_st.bitCnt_L < (SINGLE_BIT_SHORT - 2) || singleCommRevBits_st.bitCnt_H < (SINGLE_BIT_SHORT - 2)) //电平长度不够
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
                if (singleCommRevBits_st.bitCnt_L <= (SINGLE_BIT_SHORT + 2) && singleCommRevBits_st.bitCnt_H <= SINGLE_BIT_LONG + 2 && singleCommRevBits_st.bitCnt_H >= SINGLE_BIT_LONG - 2)
                { //逻辑0
                    singleCommRevBits_st.bitCnt++;
                    if (singleCommRevBits_st.bitCnt <= 8)
                    {
                        sinleCommData_st.revBuff[0] <<= 1;
                        sinleCommData_st.revBuff[0] &= 0xfe;
                    }
                    else if (singleCommRevBits_st.bitCnt > 8 & singleCommRevBits_st.bitCnt <= 16)
                    {
                        sinleCommData_st.revBuff[1] <<= 1;
                        sinleCommData_st.revBuff[1] &= 0xfe;
                    }
                    else if (singleCommRevBits_st.bitCnt > 16 & singleCommRevBits_st.bitCnt <= 24)
                    {
                        sinleCommData_st.revBuff[2] <<= 1;
                        sinleCommData_st.revBuff[2] &= 0xfe;
                    }
                    else if (singleCommRevBits_st.bitCnt > 24 & singleCommRevBits_st.bitCnt <= 32)
                    {
                        sinleCommData_st.revBuff[3] <<= 1;
                        sinleCommData_st.revBuff[3] &= 0xfe;
                    }
                    singleCommRevBits_st.bitCnt_H = 0;
                    singleCommRevBits_st.bitCnt_L = 1;
                }
                else if (singleCommRevBits_st.bitCnt_H <= SINGLE_BIT_SHORT + 2 && singleCommRevBits_st.bitCnt_L <= SINGLE_BIT_LONG + 2 && singleCommRevBits_st.bitCnt_L >= SINGLE_BIT_LONG - 2)
                { //逻辑1
                    singleCommRevBits_st.bitCnt++;
                    if (singleCommRevBits_st.bitCnt <= 8)
                    {
                        sinleCommData_st.revBuff[0] <<= 1;
                        sinleCommData_st.revBuff[0] |= 0x01;
                    }
                    else if (singleCommRevBits_st.bitCnt > 8 & singleCommRevBits_st.bitCnt <= 16)
                    {
                        sinleCommData_st.revBuff[1] <<= 1;
                        sinleCommData_st.revBuff[1] |= 0x01;
                    }
                    else if (singleCommRevBits_st.bitCnt > 16 & singleCommRevBits_st.bitCnt <= 24)
                    {
                        sinleCommData_st.revBuff[2] <<= 1;
                        sinleCommData_st.revBuff[2] |= 0x01;
                    }
                    else if (singleCommRevBits_st.bitCnt > 24 & singleCommRevBits_st.bitCnt <= 32)
                    {
                        sinleCommData_st.revBuff[3] <<= 1;
                        sinleCommData_st.revBuff[3] |= 0x01;
                    }
                    singleCommRevBits_st.bitCnt_H = 0;
                    singleCommRevBits_st.bitCnt_L = 1;
                    //逻辑1
                }
                singleCommRevBits_st.bitOverFlag = 0;
            }
            else
            {
                if (++singleCommRevBits_st.bitCnt_H >= SINGLE_BIT_LONG + 5) //高电平比长电平还要长
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
            }
            //结束电平接收
            if (singleCommRevBits_st.bitCnt_L <= SINGLE_BIT_END_L + 3 && singleCommRevBits_st.bitCnt_L >= SINGLE_BIT_END_L - 3)
            {
                if (singleCommRevBits_st.bitCnt == 16) //两个字节
                {
                    sinleCommData_st.revLen = 2;
                    sinleCommData_st.validRev = 1;
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_HEAD;
                  //  singleCommError_st.cnt = 0;
                }
                else if (singleCommRevBits_st.bitCnt == 32) // 4个字节
                {
                    sinleCommData_st.revLen = 4;
                    sinleCommData_st.validRev = 1;
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_HEAD;
                    //singleCommError_st.cnt = 0;
                }
                else
                {
                    singleCommRevBits_st.state = SINGLE_COMM_BIT_ERROR;
                    break;
                }
            }
        }
        break;
    case SINGLE_COMM_BIT_ERROR:
        singleCommRevBits_st.state = SINGLE_COMM_BIT_HEAD;
        break;
    default:
        break;
    }
}

sinleCommData *sinleCommDataGet(void)
{
    return &sinleCommData_st;
}
/*
singleCommError *sinleCommErrorGet(void)
{
    return &singleCommError_st;
}*/
//定时器调用100US 一次
void singleCommHandle(void)
{
    singleCommSendHandle();
    singleCommRevHandle();
}
void singleCommInit(void)
{
    memset(&sinleCommData_st, 0, sizeof(sinleCommData_st));
    memset(&singleCommSendBits_st, 0, sizeof(singleCommSendBits_st));
    memset(&singleCommRevBits_st, 0, sizeof(singleCommRevBits_st));
}
