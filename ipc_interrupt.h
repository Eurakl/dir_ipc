/*
 * ipc_interrupt.h
 *
 *  Created on: 2022Äê5ÔÂ18ÈÕ
 *      Author: ÇÇ
 */

#ifndef IPC_INTERRUPT_H_
#define IPC_INTERRUPT_H_

#include <stdio.h>
#include <string.h>
#include <c6x.h>
#include <ti/csl/csl_cpintc.h>
#include <ti/csl/src/intc/csl_intcAux.h>
#include <ti/csl/csl_tmr.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/src/intc/csl_intc.h>
#include <ti/csl/csl_cacheAux.h>



#define MAX_CORE_NUM 8
#define CORENUM 8
#define MAX_SYSTEM_VECTOR 8
#define MAX_CORE_VECTOR 8

#define IPCGR0 0x02620240
#define IPCGR1 0x02620244
#define IPCGR2 0x02620248
#define IPCGR3 0x0262024c
#define IPCGR4 0x02620250
#define IPCGR5 0x02620254
#define IPCGR6 0x02620258
#define IPCGR7 0x0262025c

#define IPCAR0 0x02620280
#define IPCAR1 0x02620284
#define IPCAR2 0x02620288
#define IPCAR3 0x0262028c
#define IPCAR4 0x02620290
#define IPCAR5 0x02620294
#define IPCAR6 0x02620298
#define IPCAR7 0x0262029c

#define IPC_INFO0_SUF       0x0010
#define IPC_INFO1_SUF       0x0020
#define IPC_INFO2_SUF       0x0040
#define IPC_INFO3_SUF       0x0080
#define IPC_INFO4_SUF       0x0100
#define IPC_INFO5_SUF       0x0200
#define IPC_INFO6_SUF       0x0400
#define IPC_INFO7_SUF       0x0800

typedef struct interruptCfg
{
    uint32_t core;
    uint32_t event;
    uint32_t vect;
    CSL_IntcEventHandler isr;
}interruptCfg;

typedef struct IPC_PKT_TEST
{
    uint32_t src;
    uint32_t dst;
    uint32_t type;
}IPC_PKT_TEST;

//#pragma DATA_SECTION(Flag, ".IPC")
extern volatile unsigned int far Flag;

//#pragma DATA_SECTION(Flag_1, ".IPC")
extern volatile IPC_PKT_TEST far Pkt_Test;


int32_t intcInit();
int32_t registerInterrupt();
void IssueInterruptToNextCore();
void IPC_ISR();

#endif /* IPC_INTERRUPT_H_ */
