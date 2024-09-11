#include "F28335_Device.h"
#include "App_UserDef.h"

/*--------------------------------------------------------------------------
 #############################################################################
 �жϳ�ʼ������
 ----------------------------------------------------------------------------
 �ú�����ʼ��PIECRTL
 */
void InitPieCtrl(void) {
    /* �ر�CPU���ж�:*/
    DINT;

    /* �ر�PIE*/
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

    /* �������PIEIER�Ĵ���:*/
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

    /* ������� PIEIFR �Ĵ���:*/
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
 ��ʼ���ж�������:
 ---------------------------------------------------------------------------
 �ú�����ʼ���ж�������ʹ��ָ��Ĭ�ϵ��жϷ������
 �ú�������������ʱ����ִ��
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

    /* ʹ��PIE�ж�*/
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}
/*-----------------------------------------------------------------------------
 �жϲ��ֳ�ʼ�����
 #############################################################################*/
