#include <c6x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ti/csl/csl_chip.h>
#include <ti/csl/src/intc/csl_intc.h>
#include <ti/csl/csl_cpintcAux.h>

#include "ipc_interrupt.h"


void main()
{
    uint32_t i;
    // 获取内核号
    uint32_t coreID = CSL_chipReadReg (CSL_CHIP_DNUM);

    TSCL = 0;
    //初始化
    intcInit();  //init the intc CSL global data structures, enable global ISR
    //注册
    registerInterrupt(); //register the Host interrupt with the event

    for (i=0; i<1000; i++)
        asm (" NOP 5");

    //core 0开始发送信息给下一个核
    if (coreID == 0)
    {
        Flag=5;
        Pkt_Test.src=0;
        Pkt_Test.dst=1;
        Pkt_Test.type=2;
        printf("Flag=%d with address %p\n",Flag,&Flag);

        CACHE_wbInvL2((void *)&Flag, sizeof(Flag), CACHE_WAIT); // Add this line to write back and invalidate the L2 cache.
        CACHE_wbInvL2((void *)&Pkt_Test, sizeof(Pkt_Test), CACHE_WAIT);
        IssueInterruptToNextCore((coreID+1),IPC_INFO0_SUF);
    }

    while(1)
    {
        asm(" NOP 9");
    };
}
