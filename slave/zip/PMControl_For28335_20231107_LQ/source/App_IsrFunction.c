/*
###########################################################################

 文件名:	  F28335_IsrFunction.c

 功能描述:	
		 	  F28335 默认中断服务程序初始化.

 编写者:    伍泽东
 编写日期:  2011-9-1
###########################################################################
*/
#include "F28335_Device.h"     /* F28335 外设头文件*/
#include "App_UserDef.h"      /* F28335 应用项目头文件*/

#pragma CODE_SECTION(Cpu_Timer0_ISR, "ramfuncs");
#pragma CODE_SECTION(EPWM1_INT_ISR, "ramfuncs");  
#pragma CODE_SECTION(XINT1_Fault_Isr, "ramfuncs");


/*==============================================================================
 Cpu定时器中断服务程序,用于系统控制周期
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
	EPwm1Regs.ETCLR.bit.INT = 1;             //清中断标志
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


//------------------------------------------------
//XINT1_Fault_Isr():SYN_CLK_IN中断
//------------------------------------------------
interrupt void XINT1_Fault_Isr(void)
{
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//-----------------------------------------------

/*=========================================================================
  其他不用的中断源产生的中断
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
