/*
 * srio.h
 *
 * Created on 2023骞�5鏈�22鏃�
 * Author wangyuxin
 *
 * Copyright (c) zhejiang univerisity
 */

#ifndef __C6678_SRIO_H_
#define __C6678_SRIO_H_

#include <ti/csl/soc.h>
#include <ti/csl/csl.h>
#include <ti/csl/cslr_srio.h>
#include <stdio.h>
#include <ti/csl/csl_chip.h>
#include <stdio.h>
#include <ti\csl\csl_bootcfgAux.h>
#include <ti\csl\csl_cacheAux.h>
#include <ti\csl\csl_chip.h>
#include <ti\csl\csl_xmcAux.h>
#include <ti\csl\csl_chipAux.h>
#include <ti\csl\csl_types.h>
#include <ti\csl\csl_srio.h>
#include <ti\csl\csl_srioAux.h>

#define SRIO_RATE_1250              0x00000020  //0x00000020 (312.5MHz)
#define SRIO_RATE_2500              0x00000040  //0x00000040 (312.5MHz)
#define SRIO_RATE_3125              0x00000050  //0x00000050 (312.5MHz)
#define SRIO_RATE_5000              0x00000080  //0x00000080 (312.5MHz)

#define    C6678_ID     0x000A
#define    FPGA_ID      0x000C
#define SRIO_BASE_ADDR              0x02900000
// ID Registers
#define DEVICEID_REG0 				(*(unsigned int*)(SRIO_BASE_ADDR + 0x0080))
#define DEVICEID_REG1 				(*(unsigned int*)(SRIO_BASE_ADDR + 0x0084))
#define DEVICEID_REG14 				(*(unsigned int*)(SRIO_BASE_ADDR + 0x00B8))
#define DEVICEID_REG15 				(*(unsigned int*)(SRIO_BASE_ADDR + 0x00BC))


extern void nop_delay(int num);
extern void SRIO_Init(int device_id, int rate);
extern void send_CSL(unsigned int lsu, unsigned int SrcAddr, unsigned int DstAddr,\
		             unsigned int ByteCnt, unsigned int DstID,unsigned int portnum);
extern void read_CSL(unsigned int lsu, unsigned int SrcAddr, unsigned int DstAddr,\
		             unsigned int ByteCnt, unsigned int DstID,unsigned int portnum);


#endif

