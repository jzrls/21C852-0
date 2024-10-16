/*
//#################################################################################
//
// 文件名:	  F28335_Svpwm.c
//
// 功能描述:	
//		 
//			该文件包含全比较PWM驱动器更新占空比
//			
//
// 编写者:    周求宽
// 编写日期:  2005-3-6
//###############################################################################
*/

//--------------------------------------------------------------------------
#include "F28335_Device.h"     // F28335 外设头文件
#include "App_UserDef.h"      // F28335 应用项目头文件
//--------------------------------------------------------------------------


//----------------------------------------------------------------------------
#pragma CODE_SECTION(PWMA_Update, "ramfuncs"); 
#pragma CODE_SECTION(Forbid_PWMA, "ramfuncs");  
#pragma CODE_SECTION(SVPWMA_Gen, "ramfuncs"); 
//----------------------------------------------------------------------------

//=========================================================================================================
//--------------------------------------------------------------------------------
// SPWMA_Gen 生成函数
// 利用软件产生SPWM
// 输入参数: Alfa,Beta分量
//
//-------------------------------------------------------------------------------
//=================================================================================================
void SVPWMA_Gen(_iq Alfa,_iq Beta) 
{       
	volatile long  N;
    volatile _iq  theta=0,Uxa=0,Uya=0,Uxb=0,Uyb=0,T1=0,T2=0,T3=0;
    volatile int32	temp;

    theta = _IQasin4PU(Alfa,Beta);
    N = _IQmpyI32int(theta,6);
    if(N==0)
    {
    	Uxa = _IQ(1.2247448713915890490986420373529);//sqrt(3/2)
    	Uya = _IQ(-0.70710678118654752440084436210485);	//-sqrt(3)/2
    	Uxb = _IQ(0);
    	Uyb = _IQ(1.4142135623730950488016887242097);//sqrt(2)
    	
    	T1 = _IQmpy(Uxa, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uya, _IQmpy(Beta, _IQabs(Udc_Mche)));//这里把_IQabs变成了abs();
		T2 = _IQmpy(Uxb, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uyb, _IQmpy(Beta, _IQabs(Udc_Mche)));
		T3 = _IQ(1) - T1 - T2;
		pwma.mDuty1 = _IQmpy(_IQ(0.5), T3);
		pwma.mDuty2 = pwma.mDuty1 + _IQmpy(_IQ(1), T1);
    	pwma.mDuty3 = pwma.mDuty2 + _IQmpy(_IQ(1), T2);
    }
    
    else if(N==1)
    {
     	Uxa = _IQ(-1.2247448713915890490986420373529);
    	Uya = _IQ(0.70710678118654752440084436210485);	
    	Uxb = _IQ(1.2247448713915890490986420373529);
    	Uyb = _IQ(0.70710678118654752440084436210485);
    	
    	T1 = _IQmpy(Uxa, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uya, _IQmpy(Beta, _IQabs(Udc_Mche)));//这里把_IQabs变成了abs();
		T2 = _IQmpy(Uxb, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uyb, _IQmpy(Beta, _IQabs(Udc_Mche)));
		T3 = _IQ(1) - T1 - T2;
		pwma.mDuty2 = _IQmpy(_IQ(0.5), T3);
		pwma.mDuty1 = pwma.mDuty2 + _IQmpy(_IQ(1), T1);
    	pwma.mDuty3 = pwma.mDuty1 + _IQmpy(_IQ(1), T2);
    }
    
    else if(N==2)
    {	
     	Uxa = _IQ(0);
    	Uya = _IQ(1.4142135623730950488016887242097);	
    	Uxb = _IQ(-1.2247448713915890490986420373529);
    	Uyb = _IQ(-0.70710678118654752440084436210485);
    	
    	T1 = _IQmpy(Uxa, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uya, _IQmpy(Beta, _IQabs(Udc_Mche)));//这里把_IQabs变成了abs();
		T2 = _IQmpy(Uxb, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uyb, _IQmpy(Beta, _IQabs(Udc_Mche)));
		T3 = _IQ(1) - T1 - T2;
		pwma.mDuty2 = _IQmpy(_IQ(0.5), T3);
		pwma.mDuty3 = pwma.mDuty2 + _IQmpy(_IQ(1), T1);
    	pwma.mDuty1 = pwma.mDuty3 + _IQmpy(_IQ(1), T2);
    }

    else if(N==3)
    {	
    	Uxa = _IQ(0);
    	Uya = _IQ(-1.4142135623730950488016887242097);	
    	Uxb = _IQ(-1.2247448713915890490986420373529);
    	Uyb = _IQ(0.70710678118654752440084436210485);
    	
    	T1 = _IQmpy(Uxa, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uya, _IQmpy(Beta, _IQabs(Udc_Mche)));//这里把_IQabs变成了abs();
		T2 = _IQmpy(Uxb, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uyb, _IQmpy(Beta, _IQabs(Udc_Mche)));
		T3 = _IQ(1) - T1 - T2;
		pwma.mDuty3 = _IQmpy(_IQ(0.5), T3);
		pwma.mDuty2 = pwma.mDuty3 + _IQmpy(_IQ(1), T1);
    	pwma.mDuty1 = pwma.mDuty2 + _IQmpy(_IQ(1), T2);
    }
    
    else if(N==4)
    {	
    	Uxa = _IQ(-1.2247448713915890490986420373529);
    	Uya = _IQ(-0.70710678118654752440084436210485);	
    	Uxb = _IQ(1.2247448713915890490986420373529);
    	Uyb = _IQ(-0.70710678118654752440084436210485);
    	
    	T1 = _IQmpy(Uxa, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uya, _IQmpy(Beta, _IQabs(Udc_Mche)));//这里把_IQabs变成了abs();
		T2 = _IQmpy(Uxb, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uyb, _IQmpy(Beta, _IQabs(Udc_Mche)));
		T3 = _IQ(1) - T1 - T2;
		pwma.mDuty3 = _IQmpy(_IQ(0.5), T3);
		pwma.mDuty1 = pwma.mDuty3 + _IQmpy(_IQ(1), T1);
    	pwma.mDuty2 = pwma.mDuty1 + _IQmpy(_IQ(1), T2);
    }
    
    else 
    {	
    	Uxa = _IQ(1.2247448713915890490986420373529);
    	Uya = _IQ(0.70710678118654752440084436210485);	
    	Uxb = _IQ(0);
    	Uyb = _IQ(-1.4142135623730950488016887242097);
    	
    	T1 = _IQmpy(Uxa, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uya, _IQmpy(Beta, _IQabs(Udc_Mche)));//这里把_IQabs变成了abs();
		T2 = _IQmpy(Uxb, _IQmpy(Alfa, _IQabs(Udc_Mche))) + _IQmpy(Uyb, _IQmpy(Beta, _IQabs(Udc_Mche)));
		T3 = _IQ(1) - T1 - T2;
		pwma.mDuty1 = _IQmpy(_IQ(0.5), T3);
		pwma.mDuty3 = pwma.mDuty1 + _IQmpy(_IQ(1), T1);
    	pwma.mDuty2 = pwma.mDuty3 + _IQmpy(_IQ(1), T2);
    }

	pwma.mDuty1 = _IQ(1) - pwma.mDuty1;
	pwma.mDuty2 = _IQ(1) - pwma.mDuty2;
	pwma.mDuty3 = _IQ(1) - pwma.mDuty3;

	pwma.mDuty1 = pwma.mDuty1 + PWMA_a;
	pwma.mDuty2 = pwma.mDuty2 + PWMA_b;
	pwma.mDuty3 = pwma.mDuty3 + PWMA_c;

	if(pwma.mDuty1 > _IQ(1))
		pwma.mDuty1 = _IQ(1);
	else if(pwma.mDuty1  < 0)
		pwma.mDuty1 = 0;
			
	if(pwma.mDuty2 > _IQ(1))
		pwma.mDuty2 = _IQ(1);
	else if(pwma.mDuty2  < 0)
		pwma.mDuty2 = 0;
		
	if(pwma.mDuty3 > _IQ(1))
		pwma.mDuty3 = _IQ(1);
	else if(pwma.mDuty3 < 0)
		pwma.mDuty3 = 0;
}
//=========================================================================================================
//--------------------------------------------------------------------------------
// PWMA更新函数
//-------------------------------------------------------------------------------
void PWMA_Update() 
{       
	volatile Uint32  halfPeriod;
    volatile int32	temp;	
			

 	halfPeriod = pwma.mHalfPeriod; 
    EPwm1Regs.TBPRD = (Uint16)halfPeriod; 
    EPwm2Regs.TBPRD = (Uint16)halfPeriod; 
    EPwm3Regs.TBPRD = (Uint16)halfPeriod;  
    
    // Q0 = Q0*QG  => _IQmpy()        
    temp = _IQmpy((_iq)halfPeriod, pwma.mDuty1);		
    EPwm1Regs.CMPA.half.CMPA = (Uint16)temp;	
        
    temp = _IQmpy((_iq)halfPeriod, pwma.mDuty2);
	EPwm2Regs.CMPA.half.CMPA = (Uint16)temp;

    temp = _IQmpy((_iq)halfPeriod, pwma.mDuty3);
    EPwm3Regs.CMPA.half.CMPA = (Uint16)temp;
    
    if(Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
	{
		Forbid_PWMA();
	}
	else
	{
		#if (FORBIDPWMA == FALSE)
		EPwm1Regs.AQSFRC.bit.RLDCSF = 3;
		EPwm1Regs.AQCSFRC.bit.CSFA = 3;          //PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定
		EPwm1Regs.AQCSFRC.bit.CSFB = 3;		   	//00：无效 01：强制低 10：强制高 11：无效
		EPwm1Regs.DBCTL.bit.OUT_MODE = 3;      //00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块 11:A上什沿B下降沿均延时

		EPwm2Regs.AQSFRC.bit.RLDCSF = 3;
		EPwm2Regs.AQCSFRC.bit.CSFA = 3;          //PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定
		EPwm2Regs.AQCSFRC.bit.CSFB = 3;		   	//00：无效 01：强制低 10：强制高 11：无效 
		EPwm2Regs.DBCTL.bit.OUT_MODE = 3;      //00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块 11:A上什沿B下降沿均延时

		EPwm3Regs.AQSFRC.bit.RLDCSF = 3;
		EPwm3Regs.AQCSFRC.bit.CSFA = 3;          //PWMA输出 00：无效 01：强制低 10：恐聘� 11：无效,输出状态由AQ、DB共同决定
		EPwm3Regs.AQCSFRC.bit.CSFB = 3;		   	//00：无效 01：强制低 10：强制高 11：无效
		EPwm3Regs.DBCTL.bit.OUT_MODE = 3;      //00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块 11:A上什沿B下降沿均延时 

		
	  	#endif


	    #if (FORBIDPWMA == TRUE)
		Forbid_PWMA();
	  	#endif 
	}
}

void Forbid_PWMA(void)
{
//输出强制低
	EPwm1Regs.AQSFRC.bit.RLDCSF = 3;
	EPwm1Regs.AQCSFRC.bit.CSFA = 1;          //PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定
	EPwm1Regs.AQCSFRC.bit.CSFB = 1;		   	//00：无效 01：强制低 10：强制高 11：无效
	EPwm1Regs.DBCTL.bit.OUT_MODE = 0;      //00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块 11:A上什沿B下降沿均延时

	EPwm2Regs.AQSFRC.bit.RLDCSF = 3;
	EPwm2Regs.AQCSFRC.bit.CSFA = 1;          //PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定
	EPwm2Regs.AQCSFRC.bit.CSFB = 1;		   	//00：无效 01：强制低 10：强制高 11：无效 
	EPwm2Regs.DBCTL.bit.OUT_MODE = 0;      //00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块 11:A上什沿B下降沿均延时

	EPwm3Regs.AQSFRC.bit.RLDCSF = 3;
	EPwm3Regs.AQCSFRC.bit.CSFA = 1;          //PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定
	EPwm3Regs.AQCSFRC.bit.CSFB = 1;		   	//00：无效 01：强制低 10：强制高 11：无效
	EPwm3Regs.DBCTL.bit.OUT_MODE = 0;      //00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块 11:A上什沿B下降沿均延时 
	
}

//------------------------------------------------
//TZ2_Fault_Isr():IGBT故障中断
//------------------------------------------------
interrupt void TZ2_Fault_Isr(void)
{
//输出强制低
	Forbid_PWMA();
	Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
  	Sys_Flag.bit.STOP_PWM_Flag_Driv = 1;
	Ctrl_Flag.bit.STOP_PWM_Flag = 1;
	/*EALLOW;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm2Regs.TZCLR.bit.INT = 1;
	EDIS;*/

  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}	


//===========================================================================
// No more.
//===========================================================================
