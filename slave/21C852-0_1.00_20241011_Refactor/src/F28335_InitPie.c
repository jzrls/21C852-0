#include "F28335_Device.h"
#include "App_UserDef.h"

/*--------------------------------------------------------------------------
 #############################################################################
 中断初始化程序
 ----------------------------------------------------------------------------
 该函数初始化PIECRTL
 */
void InitPieCtrl(void) {
    /* 关闭CPU级中断:*/
    DINT;

    /* 关闭PIE*/
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

    /* 清除所有PIEIER寄存器:*/
    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

    /* 清除所以 PIEIFR 寄存器:*/
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;

    /* Enables PIE to drive a pulse into the CPU */
    PieCtrlRegs.PIEACK.all = 0xFFFF;
}

/*---------------------------------------------------------------------------
 初始化中断向量表:
 ---------------------------------------------------------------------------
 该函数初始化中断向量表，使其指向默认的中断服务程序
 该函数必须在启动时调用执行
 */
void InitPieVectTable(void) {
    Uint16 i;
    PINT *Dest = &(PieVectTable.PIE1_RESERVED);
    /*Uint32 *Dest = (void *) &PieVectTable;*/
    EALLOW;
    for (i = 0; i < 128; i++) {
        *Dest = (PINT)rsvd_ISR;
        Dest++;
    }
    EDIS;

    /* 使能PIE中断*/
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}
/*-----------------------------------------------------------------------------
 中断部分初始化完成
 #############################################################################*/
