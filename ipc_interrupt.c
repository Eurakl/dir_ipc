/*
 * ipc_interrupt.c
 *
 *  Created on: 2022年5月18日
 *      Author: 乔
 */
#include "ipc_interrupt.h"


//extern volatile uint32_t sync_flag; // 声明 sync_flag
//extern volatile uint32_t Flag;


CSL_IntcGlobalEnableState state;    //使能
CSL_IntcContext context;     //上下文
CSL_IntcEventHandlerRecord Record[CSL_INTC_EVENTID_CNT]; //128个事件句柄
CSL_IntcEventHandlerRecord  EventRecord; //当前事件句柄
uint32_t        coreVector[MAX_CORE_NUM]; //内核表
CSL_IntcObj     intcObj[16]; //中断控制器对象
CSL_IntcHandle  hintc[16]; //中断控制器句柄
volatile Uint32 interruptNumber=0;



/* IPCGR Info */
int32_t iIPCGRInfo[CORENUM] = {
                                IPCGR0,
                                IPCGR1,
                                IPCGR2,
                                IPCGR3,
                                IPCGR4,
                                IPCGR5,
                                IPCGR6,
                                IPCGR7
                             };
/* IPCAR Info */
int32_t iIPCARInfo[CORENUM] = {
                                IPCAR0,
                                IPCAR1,
                                IPCAR2,
                                IPCAR3,
                                IPCAR4,
                                IPCAR5,
                                IPCAR6,
                                IPCAR7
                             };



interruptCfg intInfo[MAX_SYSTEM_VECTOR] =
{
    /* core   event   vector*/
    {  0,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  1,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  2,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  3,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  4,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  5,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  6,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
    {  7,     91,     CSL_INTC_VECTID_4, &IPC_ISR},
};

int32_t intcInit()
{
    //初始化上下文环境
    /* INTC module initialization */
    context.eventhandlerRecord = Record;
    context.numEvtEntries      = CSL_INTC_EVENTID_CNT;
    if (CSL_intcInit (&context) != CSL_SOK)
        return -1;

    /* Enable NMIs */
    if (CSL_intcGlobalNmiEnable () != CSL_SOK)
        return -1;

    /* Enable global interrupts */
    if (CSL_intcGlobalEnable (&state) != CSL_SOK)
        return -1;

    /* INTC has been initialized successfully. */
    return 0;
}

//注册中断
int32_t registerInterrupt()
{
    uint32_t i;
    uint32_t event;
    uint32_t vector;
    uint32_t core;
    uint32_t coreID = CSL_chipReadReg (CSL_CHIP_DNUM);
    CSL_IntcEventHandler isr;

    for (i=0; i<MAX_CORE_NUM; i++)
    {
        coreVector[i] = 0;
    }

    //只有为当前内核，将顶一的core信息与中断源绑定
    for (i=0; i<MAX_SYSTEM_VECTOR; i++)
    {
        core = intInfo[i].core;
        if (coreID == core)
        {
            event  = intInfo[i].event;
            vector = intInfo[i].vect;
            isr    = intInfo[i].isr;

            if (MAX_CORE_VECTOR <= coreVector[core])
            {
                printf("Core %d Vector Number Exceed\n");
            }

            hintc[vector] = CSL_intcOpen (&intcObj[vector], event, (CSL_IntcParam*)&vector , NULL);
            if (hintc[vector] == NULL)
            {
                printf("Error: GEM-INTC Open failed\n");
                return -1;
            }

            /* Register an call-back handler which is invoked when the event occurs. */
            EventRecord.handler = isr;
            EventRecord.arg = 0;
            if (CSL_intcPlugEventHandler(hintc[vector],&EventRecord) != CSL_SOK)
            {
                printf("Error: GEM-INTC Plug event handler failed\n");
                return -1;
            }

            /* clear the events. */
            if (CSL_intcHwControl(hintc[vector],CSL_INTC_CMD_EVTCLEAR, NULL) != CSL_SOK)
            {
                printf("Error: GEM-INTC CSL_INTC_CMD_EVTCLEAR command failed\n");
                return -1;
            }

            /* Enabling the events. */
            if (CSL_intcHwControl(hintc[vector],CSL_INTC_CMD_EVTENABLE, NULL) != CSL_SOK)
            {
                printf("Error: GEM-INTC CSL_INTC_CMD_EVTENABLE command failed\n");
                return -1;
            }
            coreVector[core]++;
        }
    }

    return 0;
}
// BOOT and CONFIG dsp system modules Definitions
#define CHIP_LEVEL_REG  0x02620000
// Boot cfg registers
#define KICK0           *(unsigned int*)(CHIP_LEVEL_REG + 0x0038)
#define KICK1           *(unsigned int*)(CHIP_LEVEL_REG + 0x003C)
#define KICK0_UNLOCK (0x83E70B13)
#define KICK1_UNLOCK (0x95A4F1E0)
#define KICK_LOCK    0

void IssueInterruptToNextCore(uint32_t destCoreID, uint32_t interruptInfo)
{
   uint32_t CoreNum;
//   uint32_t iNextCore;
//   static uint32_t interruptInfo=0;

   CoreNum = CSL_chipReadReg (CSL_CHIP_DNUM);

//   iNextCore = (CoreNum + 1)%8; //
   printf("Set interrupt from Core %x to Core %d, cycle = %d\n", CoreNum, (CoreNum+1), TSCL);

//   interruptInfo +=16;
    // Unlock Config
    KICK0 = KICK0_UNLOCK;
    KICK1 = KICK1_UNLOCK;

    //向下一个核发送信息 ipc中断由两个寄存器ipcGR（生成核的中断）和IPCAR（确认）两个寄存器组成
   *(volatile uint32_t *) iIPCGRInfo[destCoreID] = interruptInfo;
    //使能
   *(volatile uint32_t *) iIPCGRInfo[destCoreID] |= 1;
    // lock Config
    KICK0 = KICK_LOCK;
    KICK1 = KICK_LOCK;
//   printf("Interrupt Info %d\n", interruptInfo);
}


void IPC_ISR()
{
    volatile uint32_t read_ipcgr,read_ipcar;
    uint32_t CoreNum;
    uint32_t iPrevCore;
    CoreNum = CSL_chipReadReg (CSL_CHIP_DNUM);

    iPrevCore = (CoreNum - 1) % 8;
    //当前核接收到的信息
    read_ipcgr = *(volatile Uint32 *) iIPCGRInfo[CoreNum];

    read_ipcar = *(volatile Uint32 *) iIPCARInfo[CoreNum];
    //清除SRCS位
    *(volatile uint32_t *) iIPCARInfo[CoreNum] = read_ipcgr; //clear the related source info

    printf("Receive interrupt from Core %d 0x%x with info 0x%x, cycle = %d\n", iPrevCore, read_ipcar,read_ipcgr, TSCL);

//    read_ipcgr = read_ipcgr<<1;
    // 更新Flag的值
    switch(read_ipcar)
    {
        case(IPC_INFO0_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=1;
            Pkt_Test.dst=2;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            IssueInterruptToNextCore((CoreNum+1),IPC_INFO1_SUF);
            break;
        }
        case(IPC_INFO1_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=2;
            Pkt_Test.dst=3;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            IssueInterruptToNextCore((CoreNum+1),IPC_INFO2_SUF);
            break;
        }
        case(IPC_INFO2_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=3;
            Pkt_Test.dst=4;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            IssueInterruptToNextCore((CoreNum+1),IPC_INFO3_SUF);
            break;
        }
        case(IPC_INFO3_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=4;
            Pkt_Test.dst=5;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            IssueInterruptToNextCore((CoreNum+1),IPC_INFO4_SUF);
            break;
        }
        case(IPC_INFO4_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=5;
            Pkt_Test.dst=6;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            IssueInterruptToNextCore((CoreNum+1),IPC_INFO5_SUF);
            break;
        }
        case(IPC_INFO5_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=6;
            Pkt_Test.dst=7;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            IssueInterruptToNextCore((CoreNum+1),IPC_INFO6_SUF);
            break;
        }
        case(IPC_INFO6_SUF):
        {
            Flag += 5;
            printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
            Pkt_Test.src=6;
            Pkt_Test.dst=7;
            Pkt_Test.type=2;
            printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
            CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
            CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
            printf("test end\n");
//            IssueInterruptToNextCore((CoreNum+1),IPC_INFO6_SUF);
            break;
        }

        default:
        {
            printf("error ipc\n");
            break;
        }

    }

}





