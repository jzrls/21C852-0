#include "F28335_Device.h"
#include "App_UserDef.h"

#pragma CODE_SECTION(Cpu_Timer0_ISR, "ramfuncs");
#pragma CODE_SECTION(EPWM1_INT_ISR, "ramfuncs");
#pragma CODE_SECTION(XINT1_Fault_Isr, "ramfuncs");

/*==============================================================================
 Cpu定时器中断服务程序,用于系统控制周期
==============================================================================*/
interrupt void Cpu_Timer0_ISR(void) {
    Uint16 isSciTxFinish = SCIB.FLAG.bit.TXBO_FLAG == 1;
    Uint16 isSciRxValid = SCIB.FLAG.bit.RX_FLAG == 0;
    Uint16 isCanTxFinish2 = CANB2.FLAG.bit.TXBO_FLAG == 1;
    Uint16 isCanRxValid2 = CANB2.FLAG.bit.RX_FLAG == 0;
    Uint16 isCanTxFinish = CANB.FLAG.bit.TXBO_FLAG == 1;
    Uint16 isCanRxValid = CANB.FLAG.bit.RX_FLAG == 0;

    if (isSciTxFinish && isSciRxValid && isCanTxFinish2 && isCanRxValid2 && isCanTxFinish) {
        CANB.FLAG.bit.TIMETX_FLAG = 1;
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//---------------------------------------------------------------------------
//==============================================================================
interrupt void EPWM1_INT_ISR(void) {
    Uint32 Cal1 = 0, Cal2 = 0;
    Cal1 = CpuTimer0Regs.TIM.all;
    Control();
    Cal2 = CpuTimer0Regs.TIM.all;
    if (Cal1 >= Cal2) {
        Time_Cal = Cal1 - Cal2;
    } else {
        Time_Cal = CPUTIMER0_PRD + Cal1 - Cal2;
    }
    EPwm1Regs.ETCLR.bit.INT = 1;  // 清中断标志
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//------------------------------------------------
// XINT1_Fault_Isr():SYN_CLK_IN中断
//------------------------------------------------
interrupt void XINT1_Fault_Isr(void) { PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; }
//-----------------------------------------------

/*=========================================================================
  其他不用的中断源产生的中断
=========================================================================*/
interrupt void rsvd_ISR(void) {
    ESTOP0;
    Forbid_PWMA();
    for (;;) {
    }
}
