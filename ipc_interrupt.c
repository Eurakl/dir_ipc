/*
 * ipc_interrupt.c
 *
 *  Created on: 2022年5月18日
 *      Author: 乔
 */
#include "./ipc_interrupt.h"

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
                printf("Core %d Vector Number Exceed\n",coreID);
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

void IssueInterruptToNextCore(uint32_t destCoreID, uint32_t interruptInfo)
{
   uint32_t CoreNum;
//   uint32_t iNextCore;
//   static uint32_t interruptInfo=0;

   CoreNum = CSL_chipReadReg (CSL_CHIP_DNUM);

//   iNextCore = (CoreNum + 1)%8; //
   printf("Set interrupt from Core %x to Core %d, cycle = %d\n", CoreNum, (CoreNum+1), TSCL);

    // Unlock Config
   CSL_BootCfgUnlockKicker();
    //向下一个核发送信息 ipc中断由两个寄存器ipcGR（生成核的中断）和IPCAR（确认）两个寄存器组成
   *(volatile uint32_t *) iIPCGRInfo[destCoreID] = interruptInfo;
    //使能
   *(volatile uint32_t *) iIPCGRInfo[destCoreID] |= 1;
    // lock Config
   CSL_BootCfgLockKicker();
}


void IPC_ISR()
{
    volatile uint32_t read_ipcgr,read_ipcar;
    uint32_t CoreNum;
    uint32_t value;
    CoreNum = CSL_chipReadReg (CSL_CHIP_DNUM);
    //当前核接收到的信息
    read_ipcgr = *(volatile Uint32 *) iIPCGRInfo[CoreNum];
    read_ipcar = *(volatile Uint32 *) iIPCARInfo[CoreNum];
    //清除SRCS位
    *(volatile uint32_t *) iIPCARInfo[CoreNum] = read_ipcgr; //clear the related source info
    printf("Receive interrupt from Core %d 0x%x with info 0x%x, cycle = %d\n", (CoreNum - 1) % 8, read_ipcar,read_ipcgr, TSCL);
    // 更新Flag的值
    switch(read_ipcar)
    {
        case(IPC_INFO0_SUF):
        {
            value = 0x00000000;
            Test_DDR(CoreNum,value,0x11111111);
            Test_MSMC(CoreNum,IPC_INFO1_SUF);
            break;
        }
        case(IPC_INFO1_SUF):
        {
            value = 0x11111111;
            Test_DDR(CoreNum,value,0x22222222);
            Test_MSMC(CoreNum,IPC_INFO2_SUF);
            break;
        }
        case(IPC_INFO2_SUF):
        {
            value = 0x22222222;
            Test_DDR(CoreNum,value,0x33333333);
            Test_MSMC(CoreNum,IPC_INFO3_SUF);
            break;
        }
        case(IPC_INFO3_SUF):
        {
            value = 0x33333333;
            Test_DDR(CoreNum,value,0x44444444);
            Test_MSMC(CoreNum,IPC_INFO4_SUF);
            break;
        }
        case(IPC_INFO4_SUF):
        {
            value = 0x44444444;
            Test_DDR(CoreNum,value,0x55555555);
            Test_MSMC(CoreNum,IPC_INFO5_SUF);
            break;
        }
        case(IPC_INFO5_SUF):
        {
            value = 0x55555555;
            Test_DDR(CoreNum,value,0x66666666);
            Test_MSMC(CoreNum,IPC_INFO6_SUF);
            break;
        }
        case(IPC_INFO6_SUF):
        {
            value = 0x66666666;
            Test_DDR(CoreNum,value,0x77777777);
            Test_MSMC(CoreNum,IPC_INFO7_SUF);
            printf("test end\n");
            break;
        }

        default:
        {
            printf("error ipc\n");
            break;
        }

    }

}
void Test_DDR(unsigned int coreid,unsigned int value,unsigned int modify_DDR)
{
    if(value != DDR_Test)
    {
        printf("coreid %d Test_DDR fail DDR_Test:%x value: %x\n",coreid,DDR_Test,value);
    }
    else
    {
        printf("coreid %d Test_DDR success\n",coreid);
    }
    DDR_Test = modify_DDR;
}

void Test_MSMC(unsigned int CoreNum,uint32_t interruptInfo)
{
    Flag += 5;
    Pkt_Test.src+=1;
    Pkt_Test.dst+=1;
    Pkt_Test.type=2;
    if(TEST)
    {
        printf("test ipc pkt src:%d dst:%d pkt_type:%d\n",Pkt_Test.src,Pkt_Test.dst,Pkt_Test.type);
        printf("Flag=%d, core%d receive signal from core%d with address %p\n",Flag, CoreNum, (CoreNum - 1),&Flag);
    }
    // Add this line to write back and invalidate the L2 cache.
    CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT);
    CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
    if(CoreNum!=7)
    {
        IssueInterruptToNextCore((CoreNum+1),interruptInfo);
    }
    else
    {
        printf("CoreNum %d Test End\n",CoreNum);
    }
}
void Init_Core0()
{
    DDR_Test = 0x00000000;
    Flag=5;
    Pkt_Test.src=0;
    Pkt_Test.dst=1;
    Pkt_Test.type=2;
    printf("Flag=%d with address %p\n",Flag,&Flag);
    CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
    CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
    IssueInterruptToNextCore(1,IPC_INFO0_SUF);
}

void Test_SRIO_FIN()
{
    int i;
    int N=256;
    //clear buffer
    for(i = 0; i < N; i++)
    {
       TransData[i] = 0;
       ReadBackData[i] = 0;
    }
    nop_delay(10000000);
    //init fin
    frame_send_block.frame_type=3;
    frame_send_block.fin_part.SubnetID=1;
    frame_send_block.fin_part.src=5;
    frame_send_block.fin_part.finish=1;
    for(i=0;i<8;i++)
    {
        frame_send_block.fin_part.ant_index[4*i]=0;
        frame_send_block.fin_part.ant_index[4*i+1]=1;
        frame_send_block.fin_part.ant_index[4*i+2]=2;
        frame_send_block.fin_part.ant_index[4*i+3]=3;
    }
    for(i=0;i<31;i++)
    {
        frame_send_block.fin_part.dir_online_node[i]=1;
    }
    CACHE_wbInvL2((void *)&frame_send_block, sizeof(frame_send_block), CACHE_WAIT);
    framing_FIN(TransData);
    nop_delay(1000000);
    send_CSL(0, (unsigned int)TransData, (unsigned int)FPGAData, 4*N, FPGA_ID,0);
    nop_delay(1000000);
    read_CSL(0, (unsigned int)ReadBackData, (unsigned int)FPGAData, 4*N, FPGA_ID,0);
    resolve_FIN(ReadBackData);
    CACHE_wbInvL2((void *)&frame_recv_block, sizeof(frame_recv_block), CACHE_WAIT);
    nop_delay(1000000);
}





