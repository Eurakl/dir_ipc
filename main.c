#include <c6x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ti/csl/csl_chip.h>
#include <ti/csl/src/intc/csl_intc.h>
#include <ti/csl/csl_cpintcAux.h>
#include "ipc_interrupt.h"

unsigned int * TransData =    (unsigned int *) 0x10830000;    //Ҫ���͸�FPGA��������DSP�еĴ�ŵ�ַ
unsigned int * FPGAData =     (unsigned int *) 0x00120000;    //������FPGA�еĴ�ŵ�ַ
unsigned int * ReadBackData = (unsigned int *) 0x10850000;    //��FPGA�ж��ص�������DSP�еĴ�ŵ�ַ

void main()
{
    uint32_t i;
    // ��ȡ�ں˺�
    uint32_t coreID = CSL_chipReadReg (CSL_CHIP_DNUM);

    TSCL = 0;
    //��ʼ��
    intcInit();  //init the intc CSL global data structures, enable global ISR
    //ע��
    registerInterrupt(); //register the Host interrupt with the event
    for (i=0; i<1000; i++)
        asm (" NOP 5");
    //core 0��ʼ������Ϣ����һ����
    if (coreID == 0)
    {
        DDR3_Init();
        SRIO_Init(C6678_ID, SRIO_RATE_3125);
//      Init_Core0();
        Test_SRIO_FIN();
        printf("%d %d %d %d\n",frame_send_block.frame_type,frame_send_block.fin_part.SubnetID,frame_send_block.fin_part.src,frame_send_block.fin_part.finish);
        printf("%d %d %d %d\n",frame_recv_block.frame_type,frame_recv_block.fin_part.SubnetID,frame_recv_block.fin_part.src,frame_send_block.fin_part.finish);
        printf("%d %d\n",frame_send_block.fin_part.ant_index[12],frame_send_block.fin_part.dir_online_node[12]);
        printf("%d %d\n",frame_recv_block.fin_part.ant_index[12],frame_recv_block.fin_part.dir_online_node[12]);

    }

    while(1)
    {
        asm(" NOP 9");
    };
}

