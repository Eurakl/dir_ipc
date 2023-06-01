/*
 * srio.c
 *
 * Created on 2023年5月22日
 * Author wangyuxin
 *
 * Copyright (c) zhejiang univerisity
 */

#include "../inc/SRIO/srio.h"
#include <stdio.h>
#include <csl_bootcfgaux.h>
#include <cslr_xmc.h>
#include <csl_edma3.h>
#include "../inc/KeyStone_common.h"


//typedef volatile CSL_SrioRegs*   CSL_SrioHandle;


int Flag_DoorBell = 0;
unsigned int rd_PLM_SP0_PATH_CTL = 0;
unsigned int rd_PLM_SP1_PATH_CTL = 0;
unsigned int rd_PLM_SP2_PATH_CTL = 0;
unsigned int rd_PLM_SP3_PATH_CTL = 0;
unsigned int rd_LANE0_STAT0 = 0;
unsigned int rd_LANE1_STAT0 = 0;
unsigned int rd_LANE2_STAT0 = 0;
unsigned int rd_LANE3_STAT0 = 0;
unsigned int rd_SP0_CTL = 0;
unsigned int rd_SP1_CTL = 0;
unsigned int rd_SP2_CTL = 0;
unsigned int rd_SP3_CTL = 0;




// name:	 void InitSRIO(int device_id, int rate)
// entry:	 int device_id：要初始化的设备id号 (16bits)
//			 int rate     ：每个通道上的传输速率 (1.25Gbps 2.5Gbps 3.125Gbps 5Gbps)
// result:   void
// function：初始化C6678的SRIO v2.1
// note：	  外部输入时钟312.5MHz，用4x mode, port0
//=====================================================================================================
void SRIO_Init(int device_id, int rate)
{ 
	int rdata=0;

	/**************** Set SRIO Power Domain, Only core0 can set PSC *******************/
	if (DNUM == 0)
	{
        hPscRegs->PDCTL[4] = hPscRegs->PDCTL[4] | 0x1; // Set SRIO Power domain to ON .datasheet Page:122
        hPscRegs->MDCTL[11] = (hPscRegs->MDCTL[11] & ~(0x1f)) | 0x3;// Set SRIO Module Next State to Enable State
        hPscRegs->PTCMD =   hPscRegs->PTCMD | 0x10;// Start SRIO Power domain Transition
        while((hPscRegs->PTSTAT & 0x10)!= 0);
        while(((hPscRegs->PDSTAT[4] & 0x1) != 0x1)&& ((hPscRegs->MDSTAT[11] & 0x1f) != 0x3))
        /**************** Global enable SRIO *******************/
        gpSRIO_regs->RIO_GBL_EN = 0x00000001; //GBL_EN
        gpSRIO_regs->BLOCK_ENABLE_STATUS[0].RIO_BLK_EN = 0x00000001; //MMR_EN
        gpSRIO_regs->BLOCK_ENABLE_STATUS[1].RIO_BLK_EN = 0x00000001; //LSU_EN
        gpSRIO_regs->BLOCK_ENABLE_STATUS[2].RIO_BLK_EN = 0x00000001; //MAU_EN
        gpSRIO_regs->BLOCK_ENABLE_STATUS[3].RIO_BLK_EN = 0x00000001; //TXU_EN
        gpSRIO_regs->BLOCK_ENABLE_STATUS[4].RIO_BLK_EN = 0x00000001; //RXU_EN
        gpSRIO_regs->BLOCK_ENABLE_STATUS[5].RIO_BLK_EN = 0x00000001; //PORT0_EN=1
        gpSRIO_regs->BLOCK_ENABLE_STATUS[6].RIO_BLK_EN = 0x00000001; //PORT1_EN=1
        gpSRIO_regs->BLOCK_ENABLE_STATUS[7].RIO_BLK_EN = 0x00000001; //PORT2_EN=1
        gpSRIO_regs->BLOCK_ENABLE_STATUS[8].RIO_BLK_EN = 0x00000001; //PORT3_EN=1
        gpSRIO_regs->BLOCK_ENABLE_STATUS[9].RIO_BLK_EN = 0x00000001; 
        /********** Configure SRIO to operate in normal mode ***********/
        gpSRIO_regs->RIO_PER_SET_CNTL1 = gpSRIO_regs->RIO_PER_SET_CNTL1 & 0xFFFFFF0F;//lane0~lane3 Loop back = 0
        /********** response timeout timer frequency Initialization ***********/
        gpSRIO_regs->RIO_PER_SET_CNTL = 0x08053860;
        /********** unlock the kicker lock mechanism ***********/
        CSL_BootCfgUnlockKicker();
        /******* PLL, Data Rate Initialization *******/
        gpBootCfgRegs->SRIO_SERDES_CFGPLL   = rate | 0x00000201; // MPY=4x,8x,10x,16x, Enabled PLL Page:152
        gpBootCfgRegs->SRIO_SERDES_CFGRX0   = 0x000204A5; // Fully adaptive equalization, Quarter Rate, Enable Receiver0，CDR = 100b Page:156
        gpBootCfgRegs->SRIO_SERDES_CFGRX1   = 0x000204A5; // Fully adaptive equalization, Quarter Rate, Enable Receiver1
        gpBootCfgRegs->SRIO_SERDES_CFGRX2   = 0x000204A5; // Fully adaptive equalization, Quarter Rate, Enable Receiver2
        gpBootCfgRegs->SRIO_SERDES_CFGRX3   = 0x000204A5; // Fully adaptive equalization, Quarter Rate, Enable Receiver3
        gpBootCfgRegs->SRIO_SERDES_CFGTX0 	= 0x001807A5; // Quarter Rate, Enable Transmitter0
    	gpBootCfgRegs->SRIO_SERDES_CFGTX1 	= 0x001807A5; // Quarter Rate, Enable Transmitter1
    	gpBootCfgRegs->SRIO_SERDES_CFGTX2 	= 0x001807A5; // Quarter Rate, Enable Transmitter2
    	gpBootCfgRegs->SRIO_SERDES_CFGTX3 	= 0x001807A5; // Quarter Rate, Enable Transmitter3

        do{
    		rdata = gpBootCfgRegs->STS_SRIO;  // poll on to make sure the SERDES PLL is locked before proceeding
    	} while ((rdata & 0x00000001) != 0x00000001);  //PLL锁定后，SRIO_SERDES_STS的bi-0，为高。
        CSL_BootCfgLockKicker();

        /******* Clear the LSU pending interrupts *******/
        gpSRIO_regs->LSU_ICSR_ICCR[0].RIO_LSU_ICCR = 0xFFFFFFFF;
        gpSRIO_regs->LSU_ICSR_ICCR[1].RIO_LSU_ICCR = gpSRIO_regs->LSU_ICSR_ICCR[1].RIO_LSU_ICCR | 0xFF;
        /******* Set the Device Information *******/
        gpSRIO_regs->RIO_DEV_ID = 0x009D0030; // JTAGID->PART NUMBER=0x009D, ti=0x0030 Page:202 设备供应商ID
        gpSRIO_regs->RIO_DEV_INFO = 0x00000000; // If not use SRIO boot mode，they have to be programmed by 0x00
        /******* Set the Assembly Information *******/
        gpSRIO_regs->RIO_ASBLY_ID = 0x00000030; // ti=0x0030
        gpSRIO_regs->RIO_ASBLY_INFO = 0x00000100;
        /******* Configure the processing element features *******/       //Read only
        gpSRIO_regs->RIO_PE_FEAT = 0x200001D9; // proc, SBR ext, flow ctrl, retransmit suppress, 34-bit addr
    	/******* Configure the source operation CAR *******/              //Read only
    	gpSRIO_regs->RIO_SRC_OP = 0x0000FDF4; // all support
        /******* Configure the destination operation CAR *******/         //Read only
    	gpSRIO_regs->RIO_DEST_OP = 0x0000FC04; // all except atomic
        /******* Set the 16 bit and 8 bit id for the SRIO Device *******/
    	gpSRIO_regs->RIO_BASE_ID = device_id;  // 16bit id
    	/******* Set the Host Device Identifier *******/
    	gpSRIO_regs->RIO_HOST_BASE_ID_LOCK = device_id;  // 16bit id, lock
        /******* Configure the component tag CSR *******/
    	gpSRIO_regs->RIO_COMP_TAG = 0x00000000; // not touched
        //存在问题，这里的地址在寄存器中为res字段，因此此处采用c6678_srio的操作
    	DEVICEID_REG0	  			= device_id; // Set Device ID, 16bit ID
    	DEVICEID_REG1	  			= device_id; // Set Device ID, 16bit ID
    	gpSRIO_regs->RIO_PE_LL_CTL	= 0x00000001; // 34-bit addr
    	gpSRIO_regs->RIO_LCL_CFG_HBAR  = 0x00000000; // all 0
    	gpSRIO_regs->RIO_LCL_CFG_BAR   = 0x00000000; // all 0
        /******* Configure PLM for all the ports *******/
    	gpSRIO_regs->RIO_SP_MB_HEAD		  			= 0x10000002; // end-point with software recovery

       	/******* Configure the path mode for the ports *******/
    	//SRIO.pdf page 242
        gpSRIO_regs->RIO_PLM[0].RIO_PLM_SP_PATH_CTL = 0x404; // configuration 4,mode 4, 4x mode

		/******* Enable the port,Input port enable,Output port enable *******/
    	//SRIO.pdf page 217  4x mode supported ,Four-lane port
    	gpSRIO_regs->RIO_SP[0].RIO_SP_CTL = gpSRIO_regs->RIO_SP[0].RIO_SP_CTL  | 0x00600001; // enable i/o, serial port
//    	SP1_CTL        	  			= SP1_CTL | 0x00600000; // enable i/o, serial port
//    	SP2_CTL        	  			= SP2_CTL | 0x00600000; // enable i/o, serial port
//    	SP3_CTL        	  			= SP3_CTL | 0x00600000; // enable i/o, serial port
        gpSRIO_regs->RIO_PLM[0].RIO_PLM_SP_DISCOVERY_TIMER 	= 0x40000000; // short cycles
        gpSRIO_regs->RIO_PLM[1].RIO_PLM_SP_DISCOVERY_TIMER 	= 0x40000000; // short cycles
        gpSRIO_regs->RIO_PLM[2].RIO_PLM_SP_DISCOVERY_TIMER 	= 0x40000000; // short cycles
        gpSRIO_regs->RIO_PLM[3].RIO_PLM_SP_DISCOVERY_TIMER 	= 0x40000000; // short cycles
        /******* Set the LLM Port IP Prescalar *******/
    	gpSRIO_regs->RIO_PRESCALAR_SRV_CLK      		= 0x00000021; // DMA clk=333.33MHz

    	gpSRIO_regs->RIO_PLM[0].RIO_PLM_SP_SILENCE_TIMER   	= 0x40000000; // short cycles
        gpSRIO_regs->RIO_PLM[1].RIO_PLM_SP_SILENCE_TIMER   	= 0x40000000; // short cycles
        gpSRIO_regs->RIO_PLM[2].RIO_PLM_SP_SILENCE_TIMER   	= 0x40000000; // short cycles
        gpSRIO_regs->RIO_PLM[3].RIO_PLM_SP_SILENCE_TIMER   	= 0x40000000; // short cycles

        /******* Set the Port link timeout CSR *******/
    	gpSRIO_regs->RIO_SP_LT_CTL 					= 0xFFFFFF00; // long
//    	SP_RT_CTL 					= 0xFFFFFF00; // long
    	gpSRIO_regs->RIO_SP_RT_CTL 					= 0x0000FF00; // long
        /******* Set the Port General CSR: Only executing as Master Enable *******/
    	gpSRIO_regs->RIO_SP_GEN_CTL 					= 0x40000000; // agent, master, undiscovered

    	gpSRIO_regs->RIO_ERR_RPT_BH 					= 0x00000000; // next ext=0x0000(last)
    	gpSRIO_regs->RIO_ERR_DET 					= 0x00000000; // clear
    	gpSRIO_regs->RIO_ERR_EN 						= 0x00000000; // disable
    	gpSRIO_regs->RIO_H_ADDR_CAPT 				= 0x00000000; // clear
    	gpSRIO_regs->RIO_ADDR_CAPT 					= 0x00000000; // clear
    	gpSRIO_regs->RIO_ID_CAPT 					= 0x00000000; // clear
    	gpSRIO_regs->RIO_CTRL_CAPT 					= 0x00000000; // clear
    	gpSRIO_regs->RIO_PW_RX_CAPT[0]				= 0x00000000; // clear
    	gpSRIO_regs->RIO_PW_RX_CAPT[1] 				= 0x00000000; // clear
        gpSRIO_regs->RIO_PW_RX_CAPT[2] 				= 0x00000000; // clear
        gpSRIO_regs->RIO_PW_RX_CAPT[3] 				= 0x00000000; // clear
        /******* Clear the sticky register bits *******/
    	gpSRIO_regs->RIO_REG_RST_CTL					= 0x00000001; // clear sticky register bits

        /******* Set the Data Streaming MTU *******/
    	gpSRIO_regs->RIO_DS_LL_CTL					= 0x00000040; // maximum transmission unit 256B block size

    	/******* DoorBell Interrupt Initialization *******/
    	gpSRIO_regs->DOORBELL_ICSR_ICCR[0].RIO_DOORBELL_ICCR				= 0xFFFFFFFF; //clear the Doorbell ICSR
        gpSRIO_regs->DOORBELL_ICRR[0].RIO_DOORBELL_ICRR1                    = 0x00000000; //route to INTDST0
        gpSRIO_regs->RIO_INTDST_RATE_CNT[0]                                 = 0x00000001; //set count-down value
    	Flag_DoorBell 				= 1;
    	TSC_delay_us(50);
    	/******* enable the peripheral, enable logical layer data flow *******/
    	gpSRIO_regs->RIO_PCR 					= 0x00000005; // PEREN=1
    	TSC_delay_us(600);
        /******* Configuration has been completed *******/
    	gpSRIO_regs->RIO_PER_SET_CNTL 				= gpSRIO_regs->RIO_PER_SET_CNTL | 0x01000000; // BOOT_COMPLETE=1, Write to read only registers disabled.

    	/****** check port0(4x mode) Physical Link is OK *****/
    	do{
    		rdata = gpSRIO_regs->RIO_SP[0].RIO_SP_ERR_STAT;  // polling SRIO_MAC's PORT_OK bit
    	}while((rdata & 0x00000002) != 0x00000002);
    	/******* Read registers' values to watch configuration status *******/
    	rd_PLM_SP0_PATH_CTL 		= gpSRIO_regs->RIO_PLM[0].RIO_PLM_SP_PATH_CTL;
    	rd_PLM_SP1_PATH_CTL         = gpSRIO_regs->RIO_PLM[1].RIO_PLM_SP_PATH_CTL;
    	rd_PLM_SP2_PATH_CTL         = gpSRIO_regs->RIO_PLM[2].RIO_PLM_SP_PATH_CTL;
    	rd_PLM_SP3_PATH_CTL         = gpSRIO_regs->RIO_PLM[3].RIO_PLM_SP_PATH_CTL;
        rd_LANE0_STAT0				= gpSRIO_regs->RIO_LANE[0].RIO_LANE_STAT0;
        rd_LANE1_STAT0				= gpSRIO_regs->RIO_LANE[1].RIO_LANE_STAT0;
        rd_LANE2_STAT0				= gpSRIO_regs->RIO_LANE[2].RIO_LANE_STAT0;
        rd_LANE3_STAT0				= gpSRIO_regs->RIO_LANE[3].RIO_LANE_STAT0;
    	rd_SP0_CTL					= gpSRIO_regs->RIO_SP[0].RIO_SP_CTL;
        rd_SP1_CTL					= gpSRIO_regs->RIO_SP[1].RIO_SP_CTL;
        rd_SP2_CTL					= gpSRIO_regs->RIO_SP[2].RIO_SP_CTL;
        rd_SP3_CTL					= gpSRIO_regs->RIO_SP[3].RIO_SP_CTL;
    	if(rd_PLM_SP0_PATH_CTL & 0x00000404)
    	{
    		printf("PLM_SP0_PATH_CTL : 0x%8x\n",rd_PLM_SP0_PATH_CTL);
    		printf("Port0 Set the Path's Configuration 4 and Path's Mode4\n\n");

    	}
    	else
    	{
    		printf("PLM_SP0_PATH_CTL : 0x%8x\n",rd_PLM_SP0_PATH_CTL);
    	}

    	printf("PLM_SP1_PATH_CTL : 0x%8x\n",rd_PLM_SP1_PATH_CTL);
    	printf("PLM_SP2_PATH_CTL : 0x%8x\n",rd_PLM_SP2_PATH_CTL);
    	printf("PLM_SP3_PATH_CTL : 0x%8x\n\n",rd_PLM_SP3_PATH_CTL);

       	printf("SP0_CTL : 0x%8x\n",rd_SP0_CTL);
		printf("SP1_CTL : 0x%8x\n",rd_SP1_CTL);
		printf("SP2_CTL : 0x%8x\n",rd_SP2_CTL);
		printf("SP3_CTL : 0x%8x\n\n",rd_SP3_CTL);

    	printf("LANE0_STAT0 : 0x%8x\n",rd_LANE0_STAT0);  //SRIO.pdf page 225
    	printf("LANE1_STAT0 : 0x%8x\n",rd_LANE1_STAT0);
    	printf("LANE2_STAT0 : 0x%8x\n",rd_LANE2_STAT0);
    	printf("LANE3_STAT0 : 0x%8x\n\n",rd_LANE3_STAT0);

    	printf("BASE_ID : 0x%8x\n",gpSRIO_regs->RIO_BASE_ID);
    	printf("HOST_BASE_ID_LOCK : 0x%8x\n\n",gpSRIO_regs->RIO_HOST_BASE_ID_LOCK);

    	printf("DSP core #%d SRIO Port0 Initialization is OK.\n", DNUM);
	}
	else
	{
	    printf("DSP core #%d cannot set SRIO PSC.\n", DNUM);
	}

}
void send_CSL(unsigned int lsu, unsigned int SrcAddr, \
		unsigned int DstAddr, unsigned int ByteCnt, unsigned int DstID,unsigned int portnum)
{
	SRIO_LSU_TRANSFER  lsuTransfer;
	CSL_SrioHandle      hSrio;
	Uint8              context;
	Uint8              transID;
	Uint8              compCode;
	Uint8              contextBit;
	transID = 0x10;
//	PSC_PDCTL4 = PSC_PDCTL4 | 0x1;  // Set SRIO Power domain to ON
//	PSC_MDCTL11 = (PSC_MDCTL11 & ~(0x1f)) | 0x3; // Enable SRIO clocks, Set SRIO Module Next State to Enable State
//	PSC_PTCMD = PSC_PTCMD | 0x10; // Start SRIO Power domain Transition
//	while((PSC_PTSTAT & 0x10) != 0 ) ; //Wait until the state transition process is completed
//	while(((PSC_PDSTAT4 & 0x1) != 0x1)&& ((PSC_MDSTAT11 & 0x1f) != 0x3)) ; // Verify Power Domain State and Module state changed

	hSrio = CSL_SRIO_Open (0);
	while (1)
	{
		if (CSL_SRIO_IsLSUFull (hSrio, lsu) == FALSE)
			break;
	}


//	while(CSL_SRIO_IsLSUBusy(hSrio, lsu)==FALSE){}
	retry:
	// Populate the Transfer Information.
	lsuTransfer.rapidIOMSB    = 0x0;//0
	lsuTransfer.rapidIOLSB    = DstAddr;//目的地址
	lsuTransfer.dspAddress    = SrcAddr;//源地址
	lsuTransfer.bytecount     = ByteCnt;//数据大小
	lsuTransfer.doorbellValid = 0;//0
	lsuTransfer.intrRequest   = 0;//1
	lsuTransfer.supInt        = 0;//0
	lsuTransfer.xambs         = 0;//0
	lsuTransfer.priority      = 2;//0
	lsuTransfer.outPortID     = portnum;//输出port号
	lsuTransfer.idSize        = 1;//目的ID 0-8bit or 1-16bit
	lsuTransfer.srcIDMap      = 0;//0
	lsuTransfer.dstID         = DstID;//目的ID
	lsuTransfer.ttype         = 4;
	lsuTransfer.ftype         = 5;
	lsuTransfer.hopCount      = 0;
	lsuTransfer.doorbellInfo  = 0;
	CSL_SRIO_GetLSUContextTransaction (hSrio,lsu ,&context, &transID);
	// Send the transfer on LSU 0
	CSL_SRIO_SetLSUTransfer (hSrio, lsu, &lsuTransfer);

	while(CSL_SRIO_IsLSUBusy(hSrio, lsu)==TRUE){} // polling, test BSY bit

	CSL_SRIO_GetLSUCompletionCode (hSrio, lsu, transID, &compCode, &contextBit);
	if(compCode!=0x0){

			goto retry;

	}

}
void read_CSL(unsigned int lsu, unsigned int SrcAddr, \
		unsigned int DstAddr, unsigned int ByteCnt, unsigned int DstID,unsigned int portnum)
{
	SRIO_LSU_TRANSFER  lsuTransfer;
	CSL_SrioHandle      hSrio;
	Uint8              context;
	Uint8              transID;
	Uint8              compCode;
	Uint8              contextBit;

//	PSC_PDCTL4 = PSC_PDCTL4 | 0x1;  // Set SRIO Power domain to ON
//	PSC_MDCTL11 = (PSC_MDCTL11 & ~(0x1f)) | 0x3; // Enable SRIO clocks, Set SRIO Module Next State to Enable State
//	PSC_PTCMD = PSC_PTCMD | 0x10; // Start SRIO Power domain Transition
//	while((PSC_PTSTAT & 0x10) != 0 ) ; //Wait until the state transition process is completed
//	while(((PSC_PDSTAT4 & 0x1) != 0x1)&& ((PSC_MDSTAT11 & 0x1f) != 0x3)) ; // Verify Power Domain State and Module state changed

	hSrio = CSL_SRIO_Open (0);
	while (1)
	{
		if (CSL_SRIO_IsLSUFull (hSrio, lsu) == FALSE)
			break;
	}
//	while(CSL_SRIO_IsLSUBusy(hSrio, lsu)==FALSE){}
	retry:
	// Populate the Transfer Information.
	lsuTransfer.rapidIOMSB    = 0x0;//0
	lsuTransfer.rapidIOLSB    = DstAddr;//目的地址
	lsuTransfer.dspAddress    = SrcAddr;//源地址
	lsuTransfer.bytecount     = ByteCnt;//数据大小
	lsuTransfer.doorbellValid = 0;//0
	lsuTransfer.intrRequest   = 0;//1
	lsuTransfer.supInt        = 0;//0
	lsuTransfer.xambs         = 0;//0
	lsuTransfer.priority      = 2;//0
	lsuTransfer.outPortID     = portnum;//输出port号
	lsuTransfer.idSize        = 1;//目的ID 0-8bit or 1-16bit
	lsuTransfer.srcIDMap      = 0;//0
	lsuTransfer.dstID         = DstID;//目的ID
	lsuTransfer.ttype         = 4;
	lsuTransfer.ftype         = 2;
	lsuTransfer.hopCount      = 0;
	lsuTransfer.doorbellInfo  = 0;
	CSL_SRIO_GetLSUContextTransaction (hSrio,lsu ,&context, &transID);
	// Send the transfer on LSU 0
	CSL_SRIO_SetLSUTransfer (hSrio, lsu, &lsuTransfer);

	while(CSL_SRIO_IsLSUBusy(hSrio, lsu)==TRUE){} // polling, test BSY bit

	CSL_SRIO_GetLSUCompletionCode (hSrio, lsu, transID, &compCode, &contextBit);
	if(compCode!=0x0){

			goto retry;

	}

}
/********************************************************************/
//time delay 
//这个函数执行时间如下：
//num:			1		2		3		4		5		6		7		8		9		10		11
//delay(clks):	56		99		142		185		228		271		314		357		400		443		486
//ratio:		56		49.5	47.3	46.25	45.6	45.2	44.9	44.6	44.4	44.3	44.2
//1us:num=4.4, 10us:num=44, 143us:num=636, 143us*16:10176
void nop_delay(int num)  //num => A4
{
	asm("		MV		A4, A1");
	asm("loop:");
	asm("		NOP		9");
	asm("		NOP		9");
	asm("		NOP		9");
	asm("		NOP		9");
	asm("		SUB		A1, 1, A1");
	asm("   [A1]B		loop");
	asm("		NOP		5");
	asm("		NOP		5");
}
