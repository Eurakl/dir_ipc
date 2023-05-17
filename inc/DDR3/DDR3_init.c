/*
 * DDR3_init.c
 *
 *  Created on: 2023年5月17日
 *      Author: wangyuxin
 */

#include "../inc/DDR3/DDR3_init.h"
#include <stdio.h>
#include <csl_bootcfgaux.h>
#include <cslr_xmc.h>
#include <csl_edma3.h>
#include "../inc/KeyStone_common.h"


void DDR3_PLL_Init(PLL_ControlRegs * PLL_Regs)
{
    unsigned int TEMP = 0;
    /* 1. In PLLCTL1, write ENSAT = 1 (for optimal PLL operation) */
    PLL_Regs->PLL_CTL1 |= PLLCTL1_ENSAT_MASK;

    /* 2. In PLLCTL0, write BYPASS = 1 (set the PLL in Bypass) */
    PLL_Regs->PLL_CTL0 |= PLLCTL_BYPASS_MASK;

    /* 3. In PLLCTL1, write RESET = 1  请注意此处PLL_RESET中原寄存器设置错误*/
    PLL_Regs->PLL_CTL1 |= PLLCTL1_PAPLL_MASK;

    /* 4. In PLLCTL0, SET DIVIDER */
    PLL_Regs->PLL_CTL0 &= ~PLLCTL0_PLLD_MASK;
    PLL_Regs->PLL_CTL0 |= ((DDR3PLL_DIVIDER-1) & PLLCTL0_PLLD_MASK);

    /* 5. In PLLCTL0 Set Multipler  */
    PLL_Regs->PLL_CTL0 &= ~PLLCTL0_PLLM_MASK;
    PLL_Regs->PLL_CTL0 |= ((DDR3PLL_MULTIPLIER-1) & PLLCTL0_PLLM_MASK);

    /* Set the BWADJ */
    TEMP = (DDR3PLL_MULTIPLIER >> 1) - 1;
    PLL_Regs->PLL_CTL0 &= ~PLLCTL0_BWADJ_MASK;
    PLL_Regs->PLL_CTL0 |= ((TEMP << PLLCTL0_BWADJ_SHIFT) & PLLCTL0_BWADJ_MASK);


    PLL_Regs->PLL_CTL1 &= ~PLLCTL1_BWADJ_MASK;
    PLL_Regs->PLL_CTL1 |= ((TEMP >> 8) & PLLCTL1_BWADJ_MASK);

    TSC_delay_us(7);
    PLL_Regs->PLL_CTL1 &= ~PLLCTL1_PAPLL_MASK;
    TSC_delay_us(500);
    PLL_Regs->PLL_CTL0 &= ~PLLCTL_BYPASS_MASK;
}

void DDR3_clock_phase_init()
{
    /* when we set INVERT_CLKOUT to 1, the CTRL_SLAVE_RATIO field in
     * DDR3_CONFIG_REG_0 should be programmed to 0x100. If the
     * INVERT_CLKOUT bit remains at 0, the default value of the
     * CTRL_SLAVE_RATIO field must be programmed to 0x80 */
    /* clear ctrl_slave_ratio field */
    gpBootCfgRegs->DDR3_CONFIG_REG[0] &=
            ~CSL_BOOTCFG_DDR3_CONFIG_REG_0_CMD_REG_PHY_CTRL_SLAVE_RATIO_MASK;
    gpBootCfgRegs->DDR3_CONFIG_REG[0] |=
                    0x100 << CSL_BOOTCFG_DDR3_CONFIG_REG_0_CMD_REG_PHY_CTRL_SLAVE_RATIO_SHIFT;
    gpBootCfgRegs->DDR3_CONFIG_REG[12] |=
                    CSL_BOOTCFG_DDR3_CONFIG_REG_12_CMD_REG_PHY_INVERT_CLKOUT_MASK;
    /* set dll_lock_diff to 15 */
    gpBootCfgRegs->DDR3_CONFIG_REG[0] |=
            CSL_BOOTCFG_DDR3_CONFIG_REG_0_CMD_REQ_PHY_DLL_LOCK_DIFF_MASK;
    /*Set bit 9 = 1 to use forced ratio leveling for read DQS*/
    gpBootCfgRegs->DDR3_CONFIG_REG[23] |= 0x00000200;
}

void DDR3_Init()
{
    /********** Only core0 can set PLL***********/
    if(DNUM == 0)
    {
        /********** unlock the kicker lock mechanism ***********/
//        KICK0                     = KICK0_UNLOCK;
//        KICK1                     = KICK1_UNLOCK;
        CSL_BootCfgUnlockKicker();
        DDR3_PLL_Init((PLL_ControlRegs *)&gpBootCfgRegs->PA_PLL_CTL0);
        DDR3_clock_phase_init();

        //delay per inch 170
        gpBootCfgRegs->DDR3_CONFIG_REG[2] = 0x5E;
        gpBootCfgRegs->DDR3_CONFIG_REG[3] = 0x5E;
        gpBootCfgRegs->DDR3_CONFIG_REG[4] = 0x5E;
        gpBootCfgRegs->DDR3_CONFIG_REG[5] = 0x51;
        gpBootCfgRegs->DDR3_CONFIG_REG[6] = 0x38;
        gpBootCfgRegs->DDR3_CONFIG_REG[7] = 0x3A;
        gpBootCfgRegs->DDR3_CONFIG_REG[8] = 0x24;
        gpBootCfgRegs->DDR3_CONFIG_REG[9] = 0x20;
        gpBootCfgRegs->DDR3_CONFIG_REG[10] = 0x44;

        gpBootCfgRegs->DDR3_CONFIG_REG[14] = 0xDD;
        gpBootCfgRegs->DDR3_CONFIG_REG[15] = 0xDD;
        gpBootCfgRegs->DDR3_CONFIG_REG[16] = 0xBE;
        gpBootCfgRegs->DDR3_CONFIG_REG[17] = 0xCA;
        gpBootCfgRegs->DDR3_CONFIG_REG[18] = 0xA9;
        gpBootCfgRegs->DDR3_CONFIG_REG[19] = 0xA7;
        gpBootCfgRegs->DDR3_CONFIG_REG[20] = 0x9E;
        gpBootCfgRegs->DDR3_CONFIG_REG[21] = 0xA1;
        gpBootCfgRegs->DDR3_CONFIG_REG[22] = 0xBA;



        /********** the PHY_RESET is pulsed (0 -> 1 -> 0) to latch these leveling configuration values into the PHY logic ***********/
//        DDR_PHYCTRL             &= ~(0x00008000);
//        DDR_PHYCTRL             |=  (0x00008000);
//        DDR_PHYCTRL             &= ~(0x00008000);
        gpDDR_regs->DDR_PHY_CTRL_1 &= ~0x00008000;
        gpDDR_regs->DDR_PHY_CTRL_1 |= 0x00008000;
        gpDDR_regs->DDR_PHY_CTRL_1 &= ~0x00008000;

        /********** Basic Controller and DRAM Configuration ***********/
        gpDDR_regs->SDRAM_REF_CTRL = 0x00005162;

        gpDDR_regs->SDRAM_TIM_1 =
                (0x8 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_RP_SHIFT) |
                (0x8 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_RCD_SHIFT) |
                (0x9 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_WR_SHIFT) |
                (0x17 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_RAS_SHIFT) |
                (0x20 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_RC_SHIFT) |
                (0x7 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_RRD_SHIFT) |
                (0x4 << CSL_EMIF4F_SDRAM_TIM_1_REG_T_WTR_SHIFT);
        gpDDR_regs->SDRAM_TIM_2 =
                (3 << CSL_EMIF4F_SDRAM_TIM_2_REG_T_XP_SHIFT) |
                (0xB3 << CSL_EMIF4F_SDRAM_TIM_2_REG_T_XSNR_SHIFT) |
                (0x1ff << CSL_EMIF4F_SDRAM_TIM_2_REG_T_XSRD_SHIFT) |
                (0x4 << CSL_EMIF4F_SDRAM_TIM_2_REG_T_RTP_SHIFT) |
                (0x3 << CSL_EMIF4F_SDRAM_TIM_2_REG_T_CKE_SHIFT);
        gpDDR_regs->SDRAM_TIM_3 =
                /* This field must always be programmed to 0x5 */
                (0x5 << CSL_EMIF4F_SDRAM_TIM_3_REG_T_PDLL_UL_SHIFT) |
                /* This field should be set according to PHY requirements as 0x5 */
                (0x5 << CSL_EMIF4F_SDRAM_TIM_3_REG_T_CSTA_SHIFT) |
                (0x4 << CSL_EMIF4F_SDRAM_TIM_3_REG_T_CKESR_SHIFT) |
                (0x3f << CSL_EMIF4F_SDRAM_TIM_3_REG_ZQ_ZQCS_SHIFT) |
                (0xAD << CSL_EMIF4F_SDRAM_TIM_3_REG_T_RFC_SHIFT) |
                /* This field must always be programmed to 0xF */
                (0xf << CSL_EMIF4F_SDRAM_TIM_3_REG_T_RAS_MAX_SHIFT);
        gpDDR_regs->DDR_PHY_CTRL_1 =
                /*between CAS Latency + 1 and CAS Latency + 7*/
                0x0010010E;

        gpDDR_regs->ZQ_CONFIG = 0x70073214;

        gpDDR_regs->PWR_MGMT_CTRL = 0x0;
        gpDDR_regs->SDRAM_REF_CTRL = 0x00005162;
        gpDDR_regs->SDRAM_CONFIG =
                /* Set to 3 for DDR3. All other values reserved */
                (3 << CSL_EMIF4F_SDRAM_CONFIG_REG_SDRAM_TYPE_SHIFT) |
                (0 << CSL_EMIF4F_SDRAM_CONFIG_REG_IBANK_POS_SHIFT) |
                (DDR_TERM_RZQ_OVER_6 << CSL_EMIF4F_SDRAM_CONFIG_REG_DDR_TERM_SHIFT) |
                (DDR_DYN_ODT_DISABLED << CSL_EMIF4F_SDRAM_CONFIG_REG_DYN_ODT_SHIFT) |
                (0 << CSL_EMIF4F_SDRAM_CONFIG_REG_DDR_DISABLE_DLL_SHIFT) |
                (SDRAM_DRIVE_RZQ_OVER_7 << CSL_EMIF4F_SDRAM_CONFIG_REG_SDRAM_DRIVE_SHIFT) |
                (DDR_CWL_7 << CSL_EMIF4F_SDRAM_CONFIG_REG_CWL_SHIFT) |
                (DDR_BUS_WIDTH_64 << CSL_EMIF4F_SDRAM_CONFIG_REG_NARROW_MODE_SHIFT) |
                (DDR_CL_9 << CSL_EMIF4F_SDRAM_CONFIG_REG_CL_SHIFT) |
                (DDR_ROW_SIZE_13_BIT << CSL_EMIF4F_SDRAM_CONFIG_REG_ROWSIZE_SHIFT) |
                (DDR_BANK_NUM_8 << CSL_EMIF4F_SDRAM_CONFIG_REG_IBANK_SHIFT) |
                (0 << CSL_EMIF4F_SDRAM_CONFIG_REG_EBANK_SHIFT) |
                (DDR_PAGE_SIZE_10_BIT_1024_WORD << CSL_EMIF4F_SDRAM_CONFIG_REG_PAGESIZE_SHIFT);

        TSC_delay_us(600);
        gpDDR_regs->SDRAM_REF_CTRL = 0x1450;
        gpDDR_regs->RDWR_LVL_RMP_CTRL =
                (unsigned int)(1 << CSL_EMIF4F_RDWR_LVL_RMP_CTRL_REG_RDWRLVL_EN_SHIFT);

        /* start full leveling */
        gpDDR_regs->RDWR_LVL_CTRL =
                (unsigned int)(1 << CSL_EMIF4F_RDWR_LVL_CTRL_REG_RDWRLVLFULL_START_SHIFT);
        TSC_delay_ms(3);
        CSL_BootCfgLockKicker();


        /******* check Interface logic is powered up, locked and ready for operation *******/
        while((gpDDR_regs->STATUS & 0x00000004) != 0x00000004) ;
        printf("DSP core #%d DDR3 Initialization is OK.\n", DNUM);
        printf("DDR_STATUS=0x%8x,DDR_STATUS bit[654]=%d.\n\n", gpDDR_regs->STATUS,(gpDDR_regs->STATUS & 0x00000070)>>4);
    }
    else
    {
        printf("DSP core #%d cannot set DDR3 PLL.\n", DNUM);
    }
}


