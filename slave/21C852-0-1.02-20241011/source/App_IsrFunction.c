/*
###########################################################################

 �ļ���:	  F28335_IsrFunction.c

 ��������:	
		 	  F28335 Ĭ���жϷ�������ʼ��.

 ��д��:    ����
 ��д����:  2011-9-1
###########################################################################
*/
#include "F28335_Device.h"     /* F28335 ����ͷ�ļ�*/
#include "App_UserDef.h"      /* F28335 Ӧ����Ŀͷ�ļ�*/

#pragma CODE_SECTION(Cpu_Timer0_ISR, "ramfuncs");
#pragma CODE_SECTION(EPWM1_INT_ISR, "ramfuncs");  
#pragma CODE_SECTION(XINT1_Fault_Isr, "ramfuncs");


/*==============================================================================
 Cpu��ʱ���жϷ������,����ϵͳ��������
==============================================================================*/
interrupt void Cpu_Timer0_ISR(void)
{
	//Control();
	/*CANB.m_Time_Index++;
	if(CANB.m_Time_Index>=CANB.m_Time_Length)
	{
		CANB.m_Time_Index = 0;
		CANB.FLAG.bit.TIMETX_FLAG = 1;
	}*/
	if(SCIB.FLAG.bit.TXBO_FLAG ==1 && SCIB.FLAG.bit.RX_FLAG ==0 
	&& CANB2.FLAG.bit.TXBO_FLAG ==1 && CANB2.FLAG.bit.RX_FLAG ==0 
	&& CANB.FLAG.bit.TXBO_FLAG ==1 && CANB.FLAG.bit.RX_FLAG ==0)
		CANB.FLAG.bit.TIMETX_FLAG = 1;
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}
//---------------------------------------------------------------------------
//==============================================================================
interrupt void EPWM1_INT_ISR(void)
{
	Uint32 Cal1=0,Cal2=0;
	Cal1 = CpuTimer0Regs.TIM.all;
	Control();
	Cal2 = CpuTimer0Regs.TIM.all;
	if(Cal1 >= Cal2)
	{
		Time_Cal = Cal1 - Cal2;
	}
	else
	{
		Time_Cal = CPUTIMER0_PRD + Cal1 - Cal2;
	}
	EPwm1Regs.ETCLR.bit.INT = 1;             //���жϱ�־
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


//------------------------------------------------
//XINT1_Fault_Isr():SYN_CLK_IN�ж�
//------------------------------------------------
interrupt void XINT1_Fault_Isr(void)
{
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//-----------------------------------------------

/*=========================================================================
  �������õ��ж�Դ�������ж�
=========================================================================*/
interrupt void rsvd_ISR(void)  
{
  	ESTOP0;
	Forbid_PWMA();
  	for(;;)
  	{

	}
}



/*===========================================================================
 No more.
===========================================================================*/
