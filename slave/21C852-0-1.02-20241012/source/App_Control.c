/*
//###########################################################################
//
// ÎÄ¼þÃû:	  App_Control.c
//
// ¹¦ÄÜÃèÊö:	
//		 	  ºËÐÄ¿ØÖÆ³ÌÐò
//
// ±àÐ´Õß:    ÎéÔó¶«
// ±àÐ´ÈÕÆÚ:  2011-9-1
//###########################################################################
*/
//--------------------------------------------------------------------------
#include "F28335_Device.h"     // F28335 ÍâÉèÍ·ÎÄ¼þ
#include "App_UserDef.h"      // F28335 Ó¦ÓÃÏîÄ¿Í·ÎÄ¼þ
//--------------------------------------------------------------------------
//------------------------------------------------------------------------- 
#pragma CODE_SECTION(Control, "ramfuncs");    
#pragma CODE_SECTION(ControlA, "ramfuncs");   
#pragma CODE_SECTION(Ready_Data, "ramfuncs");  
#pragma CODE_SECTION(Ready_DataA, "ramfuncs");
#pragma CODE_SECTION(Main_ControlA, "ramfuncs"); 
#pragma CODE_SECTION(MeasureInitDigital, "ramfuncs");
#pragma CODE_SECTION(OpenLoop, "ramfuncs");
#pragma CODE_SECTION(RunLed, "ramfuncs");  
#pragma CODE_SECTION(RCFilter, "ramfuncs"); 
#pragma CODE_SECTION(Ctrl_Change, "ramfuncs");
#pragma CODE_SECTION(Loop_Ctrl, "ramfuncs");


void Loop_Ctrl(Uint16 Board)
{
	volatile _iq tmp=0;

	if(Board == 0)
	{	
		if( (CAN_CHANGE_CTRL[2] == 0x11 && CAN_CHANGE_CTRL[3] == 0x5A) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_CHANGE_CTRL[2] == 0x10 && CAN_CHANGE_CTRL[3] == 0xA5)
		{
			Ctrl_Flag.bit.CAN_Ctrl = 1;
			/*Isd_Set = 0;
	 		Pos_Set = Angle_Fa_Q16;
			if(Pos_Set > POSMAX)
				Pos_Set = POSMAX;
			if(Pos_Set < POSMIN)
				Pos_Set = POSMIN;
			TorqueAngleA = 0;
	 	  	Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
			Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
	 		Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
	 		Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;*/
		}
		else if(CAN_CHANGE_CTRL[2] == 0x12)
		{
			Ctrl_Flag.bit.CAN_Ctrl = 1;
			Isd_Set = 0;
	 		tmp = Hex_Float(CAN_CHANGE_CTRL[3],CAN_CHANGE_CTRL[4],CAN_CHANGE_CTRL[5],CAN_CHANGE_CTRL[6]);
			tmp = _IQmpy(tmp,_IQ(0.0055555555555555555555555555555556));//65536/360>>15
			Pos_Set = tmp;
			if(Pos_Set > POSMAX)
				Pos_Set = POSMAX;
			if(Pos_Set < POSMIN)
				Pos_Set = POSMIN;
			TorqueAngleA = 0;
	 	  	if(RunAllow_Flag == 1)
			{
			 	Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
		 		Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
		 		Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
			}
		}
    }
    if(Board == 2)
	{			   
	   	if( (CAN_CHANGE_CTRL[2] == 0x11 && CAN_CHANGE_CTRL[3] == 0xA5) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_CHANGE_CTRL[2] == 0x10 && CAN_CHANGE_CTRL[3] == 0x55)
		{
			Ctrl_Flag.bit.CAN_Ctrl = 1;
			/*Veloover_Set = 0;
			Isd_Set = 0;
	 		TorqueAngleA = 0;
	 	  	Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
			Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;*/
		}
		else if(CAN_CHANGE_CTRL[2] == 0x14)
		{
			Ctrl_Flag.bit.CAN_Ctrl = 1;
			tmp = Hex_Float(CAN_CHANGE_CTRL[3],CAN_CHANGE_CTRL[4],CAN_CHANGE_CTRL[5],CAN_CHANGE_CTRL[6]);
			Veloover_Set = _IQdiv(tmp,_IQmpy((V_Base>>5),_IQdiv(_IQ(9.5492965855137201461330258023509),Mp)));
			if(Veloover_Set > Vel_Speed)
				Veloover_Set = Vel_Speed;
			if(Ctrl_Flag.bit.VelWay_Flag == 1)
				Veloover_Set = -Veloover_Set;
			Isd_Set = 0;
	 		TorqueAngleA = 0;
	 	  	if(RunAllow_Flag == 1)
			{
				Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
				Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
				Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
			}
		}
    }
    if(Board == 4)
	{			   
	   	if( (CAN_CHANGE_CTRL[2] == 0x11 && CAN_CHANGE_CTRL[3] == 0x55) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_CHANGE_CTRL[2] == 0x10 && CAN_CHANGE_CTRL[3] == 0xAA)
		{
			Ctrl_Flag.bit.CAN_Ctrl = 1;
			/*Isd_Set = 0;
	 		Pos_Set = Angle_Fa_Q16;
			if(Pos_Set > POSMAX)
				Pos_Set = POSMAX;
			if(Pos_Set < POSMIN)
				Pos_Set = POSMIN;
			TorqueAngleA = 0;
	 	  	Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
			Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
	 		Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
	 		Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;*/
		}
		else if(CAN_CHANGE_CTRL[2] == 0x13)
		{
			Ctrl_Flag.bit.CAN_Ctrl = 1;
			Isd_Set = 0;
	 		tmp = Hex_Float(CAN_CHANGE_CTRL[3],CAN_CHANGE_CTRL[4],CAN_CHANGE_CTRL[5],CAN_CHANGE_CTRL[6]);
			tmp = _IQmpy(tmp,_IQ(0.0055555555555555555555555555555556));//65536/360>>15
			Pos_Set = tmp;
			if(Pos_Set > POSMAX)
				Pos_Set = POSMAX;
			if(Pos_Set < POSMIN)
				Pos_Set = POSMIN;
			TorqueAngleA = 0;
	 	  	if(RunAllow_Flag == 1)
			{
				Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
		 		Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
		 		Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
			}
		}
    }
}
  
//-------------------------------------------------------------------------
void Ctrl_Change(void)
{
	volatile Uint16	CAN_CONTROL_PRE = 0x07;
	if(CAN_CHANGE_CTRL[2] == 0x11 || CAN_CHANGE_CTRL[2] == 0x00)
		return;
	//ÏÈÑ¡Ôñ´Ó°å,Èç¹ûÖ÷°åÎÞ¹ÊÕÏ£¬ÇÒ´Ó°å²»ÔÚÔËÐÐÖÐ£¬ÔòÑ¡ÔñÖ÷°åÔËÐÐ
	if((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01)
		CAN_CONTROL_PRE &=	0xFE;
	if((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01)
		CAN_CONTROL_PRE &=	0xFD;
	if((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01)
		CAN_CONTROL_PRE &=	0xFB;
				
	if((CAN_CONTROL_PRE & 0x01) == 0x01 && (BORAD_NUM == 0 || BORAD_NUM == 1))
	{
		if(BORAD_NUM == 0)
		{
			ET_IO_OUT1_L;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		if(BORAD_NUM == 1)
		{
			ET_IO_OUT2_L;
			/*if((CAN_CONTROL_PRE & 0x01) != (CAN_CONTROL & 0x01))
			{
				Delay_Flag = 0;
				while(Delay_Flag == 0);
			}*/
			if(BC_SLT_IN == 1)
				Loop_Ctrl(0);
		}
	}				
	if((CAN_CONTROL_PRE & 0x01) == 0x00 && (BORAD_NUM == 0 || BORAD_NUM == 1))
	{
		if(BORAD_NUM == 0)
		{
			ET_IO_OUT1_H;
			/*if((CAN_CONTROL_PRE & 0x01) != (CAN_CONTROL & 0x01))
			{
				Delay_Flag = 0;
				while(Delay_Flag == 0);
			}*/
			if(BC_SLT_IN == 0)
				Loop_Ctrl(0);
		}
		if(BORAD_NUM == 1)
		{
			ET_IO_OUT2_H;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
	}
	if((CAN_CONTROL_PRE & 0x02) == 0x02 && (BORAD_NUM == 2 || BORAD_NUM == 3))
	{
		if(BORAD_NUM == 2)
		{
			ET_IO_OUT1_L;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		if(BORAD_NUM == 3)
		{
			ET_IO_OUT2_L;
			Loop_Ctrl(2);
		}
	}				
	if((CAN_CONTROL_PRE & 0x02) == 0x00 && (BORAD_NUM == 2 || BORAD_NUM == 3))
	{
		if(BORAD_NUM == 2)
		{
			ET_IO_OUT1_L;
			Loop_Ctrl(2);
		}
		if(BORAD_NUM == 3)
		{
			ET_IO_OUT2_L;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
	}
	if((CAN_CONTROL_PRE & 0x04) == 0x04 && (BORAD_NUM == 4 || BORAD_NUM == 5))
	{
		if(BORAD_NUM == 4)
		{
			ET_IO_OUT1_L;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		if(BORAD_NUM == 5)
		{
			ET_IO_OUT2_L;
			/*if((CAN_CONTROL_PRE & 0x04) != (CAN_CONTROL & 0x04))
			{
				Delay_Flag = 0;
				while(Delay_Flag == 0);
			}*/
			if(BC_SLT_IN == 1)
				Loop_Ctrl(4);
		}
	}				
	if((CAN_CONTROL_PRE & 0x04) == 0x00 && (BORAD_NUM == 4 || BORAD_NUM == 5))
	{
		if(BORAD_NUM == 4)
		{
			ET_IO_OUT1_H;
			/*if((CAN_CONTROL_PRE & 0x04) != (CAN_CONTROL & 0x04))
			{
				Delay_Flag = 0;
				while(Delay_Flag == 0);
			}*/
			if(BC_SLT_IN == 0)
				Loop_Ctrl(4);
		}
		if(BORAD_NUM == 5)
		{
			ET_IO_OUT2_H;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
	}	
	CAN_CONTROL = CAN_CONTROL_PRE;
}

	
//-----------------------------------------------------------------------------
// ÔËÐÐµÆÉÁË¸
//-----------------------------------------------------------------------------
void RunLed(void)
{
	volatile Uint16	i,Fi_Number;
	volatile Uint16	Store_Temp = 0;
	if(Run_Led_Index < RUN_LED_PERIOD) 
	{
		Run_Led_Index++;
	}
	else
	{
		Run_Led_Index = 0;
		LED1 = 1;
	}
	/*if(Delay_Flag == 0 && Delay_Index < 20) 
		Delay_Index++;
	else
	{
		Delay_Index = 0;
		Delay_Flag = 1;
	}*/
	if(ScicRegs.SCIFFRX.bit.RXFFST > 0 && SCIB.FLAG.bit.RXTIME_FLAG == 0)
	{
		Fi_Number = ScicRegs.SCIFFRX.bit.RXFFST;
		for(i=0;i<Fi_Number;i++)
		{
			if(SCIB.p_Rx_AllBuffer >= (Uint16 *)(SCI_RX_ALLBUFFER + RX_ALLBUFFER_LENGTH))
			{
				SCIB.p_Rx_AllBuffer = SCI_RX_ALLBUFFER;
			}
			Store_Temp = ScicRegs.SCIRXBUF.bit.RXDT;
			*SCIB.p_Rx_AllBuffer = (Store_Temp & 0x00FF);
			SCIB.p_Rx_AllBuffer++;
		}
		ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
		ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
		PieCtrlRegs.PIEACK.bit.ACK8=1;
	}
}
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//Ò»½×RCÂË²¨³ÌÐò
//Ëã·¨ Y(k) = a*X(k)+(1-a)*Y(k-1); 
//½ØÖ¹ÆµÂÊ f=filta/(2*pi*T)
//---------------------------------------------------------------------------------
_iq RCFilter(volatile _iq filtout,volatile _iq filtin,volatile _iq Freq_cut)
{
	volatile _iq filta=0,tmp=0;
	filta = _IQmpy(Freq_cut, _IQmpy(_IQ(6.283185307179586476925286766559), Per_Ctrl));		// ¿ØÖÆÖÜÆÚÎª	100us
	if(filta >=_IQ(1))
		filta = _IQ(1);
	filta = _IQmpy(filta, filtin - filtout);
	tmp = (filtin - filtout);
	if(filta == 0 && _IQabs(tmp) > _IQ(0.000001))
	{
		if(tmp > 0)
			return (0x1 + filtout);
		else
			return (-0x1 + filtout);
	}
	else
		return (filta + filtout);
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// ²âÁ¿Ðý±äµÄ³õÊ¼Î»ÖÃ½Ç
//----------------------------------------------------------------------
void MeasureInitDigital(void)
{
	Udc_Mche = Udc_Lock;
	phase_trans.as = Usa_Lock;
	phase_trans.bs = Usb_Lock;
	phase_trans.cs = Usc_Lock;
	THREE2TWO_CALC ();
	Us_Alfa_Mche = phase_trans.ds;
	Us_Beta_Mche = phase_trans.qs;

	SVPWMA_Gen(Us_Alfa_Mche,Us_Beta_Mche);
}

//----------------------------------------------------------------------
// Open_Loop
//----------------------------------------------------------------------
void OpenLoop(void)
{
	volatile _iq sin_anga,sin_angb,sin_angc;

    Angle_ElecOpen += _IQmpy(Freq_OpenLoop,Per_Ctrl);   //Per_CtrlÊÇ¿ØÖÆÖÜÆÚ£¬f*T
    Angle_ElecOpen = AngleAdjust(Angle_ElecOpen);
  	sin_anga = _IQcosPU(Angle_ElecOpen);
    sin_angb = _IQcosPU(Angle_ElecOpen - _IQ(0.3333333333));  //ÕâÀïµÄ1/3¶ÔÓ¦120¶È
    sin_angc = _IQcosPU(Angle_ElecOpen + _IQ(0.3333333333));

    pwma.mDuty1 = _IQ(0.5) + _IQmpy(_IQmpy(Um_OpenLoop,sin_anga),Udc_OpenLoop);    //    Udc_OpenLoopÓ¦Îªµ¹Êý
	if(pwma.mDuty1 > _IQ(1))
		pwma.mDuty1 = _IQ(1);
	else if(pwma.mDuty1  < 0)
		pwma.mDuty1 = 0;

	pwma.mDuty2 = _IQ(0.5) + _IQmpy(_IQmpy(Um_OpenLoop,sin_angb),Udc_OpenLoop);
	if(pwma.mDuty2 > _IQ(1))
		pwma.mDuty2 = _IQ(1);
	else if(pwma.mDuty2  < 0)
		pwma.mDuty2 = 0;

	pwma.mDuty3 = _IQ(0.5) + _IQmpy(_IQmpy(Um_OpenLoop,sin_angc),Udc_OpenLoop);
	if(pwma.mDuty3 > _IQ(1))
		pwma.mDuty3 = _IQ(1);
	else if(pwma.mDuty3 < 0)
		pwma.mDuty3 = 0;
}

//---------------------------------------------------------------------------
// ¿ØÖÆÖÜÆÚ³ÌÐòÐèÒªµÄµçÑ¹¡¢µçÁ÷¡¢ËÙ¶È¡¢½Ç¶ÈµÄ²É¼¯¼ÆËã¡£
// ÇóµÃËùÐèÊý¾ÝµÄ±êçÛÖµ
//---------------------------------------------------------------------------
void Ready_DataA(void)
{
	volatile _iq17 isa,isb,isc,udc,idc;
	volatile _iq17 Temp_Ia=0,Temp_Ib=0,Temp_Ic=0,Temp_Udc=0,Temp_Idc=0;
	volatile _iq tmp=0;
	volatile Uint32 tmp2;
	volatile Uint16 i=0;

	/**
     * ÐÞ¸ÄÊ±¼ä£º20241006
     * ÐÞ¸ÄÈË£ºÀîÊ¢
     * ÐÞ¸ÄÀàÐÍ£ºÐÂÔö
     *
     * ÐÞ¸ÄÄÚÈÝ£ºÔö¼ÓËÙ¶È²¨¶¯¼ÆËã£¬ÅÐ¶ÏÊÇ·ñÐèÒª½øÐÐÇÐ°å²Ù×÷
     */
    static _iq speedThreshold = 104858; /* Êµ¼Ê×ªËÙÓë¼ÆËãÊ¹ÓÃµÄ×ªËÙÏà²î£º524.288£¬ 200 * 524.88 = 104857.6 */
    static Uint16 speedProtectionCount = 0;
    static Uint16 speedProtectionExitCount = 0;
    static Uint16 speedMaxProtectionCount = 5;
    static Uint16 startTime = 20000; /* Æô¶¯½×¶Î²»½øÐÐ×ªËÙ²¨¶¯ÅÐ¶Ï£¬ÏÖÊµ¼ÊÆô¶¯Ê±¼ä1s£¬150 * 5000 / 1000 / 1000 = 0.75s  */
    static Uint16 startTimeCount = 0;
    static Uint16 exitTimeCount = 0;
    volatile _iq speedError = 0;

	//----------------------------------------------------------------------
	// ½Ç¶È¡¢¼°ËÙ¶ÈµÄ¼ÆËã
	//-------------------------------------------
	tmp = (long)(Angle_Q16);
	if(Ctrl_Flag.bit.RotorWay_Flag==1)
		Angle_Elec = _IQmpy(_IQ16toIQ(tmp),p);		// µç½Ç¶È:µ¥¶Ô¼«,p=µç»ú¼«¶ÔÊý;¶à¶Ô¼«p=1
	else
		Angle_Elec = -_IQmpy(_IQ16toIQ(tmp),p);		// µç½Ç¶È:µ¥Ô¼«,p=µç»ú¼«¶ÔÊý;¶à¶Ô¼«p=1

	Angle_Elec = AngleAdjust(Angle_Elec);
	//--------------------------------------------------------
	// ¼ÆËãËÙ¶È
	
	Velo_Calc_flag = Angle_Q16 - Angle0_Q16;
	Velo_Calc_flag = Velo_Calc_flag & 0x8000;//ÕâÀïÁ½¸öÎÞ·ûºÅµÄ16Î»ÊýÏà¼õ£¬·´×ªÎª¸º£¿ÈôÔÚÕý×ªÊ±±ä»¯0x8000£¨¼´32768£©
                                               //7000r/min¶ÔÓ¦µÄÊÇ0.07330383rad/100us£¬  ÓëpiÏà²î42±¶£¡£¡£¡
	if(Velo_Calc_flag == 0)
		Velo_Calc_flag = 0;			// Õý×ª
	else
		Velo_Calc_flag = 1;			// ·´×ª

	if(Velo_Calc_flag > 0)
		tmp2 = ((Uint32)(Angle0_Q16 - Angle_Q16))&0xffff;
	else
		tmp2 = ((Uint32)(Angle_Q16 - Angle0_Q16))&0xffff;
	Angle0_Q16 = Angle_Q16;

	tmp2 = (tmp2 << 4);          //ÕâÀïÊµ¼ÊÉÏÊÇÎ»ÖÃµÄ±ä»¯


	if(Ctrl_Flag.bit.RotorWay_Flag == 1 && Velo_Calc_flag > 0)   //Î»ÖÃµÄÀÛ¼Ó
		Velotmp -=  (_iq)tmp2;
	else if(Ctrl_Flag.bit.RotorWay_Flag == 0 && Velo_Calc_flag == 0)
		Velotmp -=  (_iq)tmp2;
	else
		Velotmp +=  (_iq)tmp2;

	Velo_Avg_Index++;
	if( Velo_Avg_Index >= Velo_Calc_Num )	// ÅÐ¶ÏÊÇ·ñÐèÒª¼ÆËãËÙ¶È£¬Velo_Calc_Num²»ÊÇQ20µÄÊý£¡£¡£¡
	{
		Velo_Avg_Index = 0;
		Velo_Elec_Cal = _IQmpy(Velotmp,K_Velo_Cal);

		Velotmp = _IQ(0);
		if(Velo_Elec_abs >= Velo_Max)
		{
			VeloProtect++;
			if(VeloProtect >= VeloProtectNum)
			{
				Sys_Flag.bit.STOP_PWM_Flag_Velo = 1;
				Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
				Ctrl_Flag.bit.STOP_PWM_Flag = 1;
			}
		}
		else
		{
			VeloProtect = 0;
		}

		if(Sys_Flag.bit.STOP_PWM_Flag_Velo == 1 && Velo_Elec_abs < Velo_Max && Velo_Elec_abs < Velo_Start)
		{
			VeloProtect1++;
			if(VeloProtect1 >= (ProtectNum>>4))
			{
				Sys_Flag.bit.STOP_PWM_Flag_Velo = 0;
				VeloProtect1 = 0;
			}
		}
		else
		{
			VeloProtect1 = 0;
		}
	}
//	ËÙ¶ÈÂË²¨
	Velo_Elec = RCFilter(Velo_Elec0, Velo_Elec_Cal, Velo_Filter);
	Velo_Elec0 = Velo_Elec;
	Velo_Elec_abs = _IQabs(Velo_Elec);

	if (BORAD_NUM == 2 || BORAD_NUM == 3) {
		/* Éè¶¨×ªËÙÎª0Ê±£¬ ¼ÆÊýÆ÷Çå0 */
	    if (_IQabs(setSpeed - prevSpeed) > 0 || (setSpeed <= 0 && prevSpeed <= 0)) {
	        startTimeCount = 0;
	        prevSpeed = setSpeed;
	    } else {
	        startTimeCount++;
	    }

	    /* ËÙ¶È²¨¶¯¹ý´óÇÐ»»Âß¼­ */
	    if (startTimeCount >= startTime) {
	        speedError = _IQabs(Velo_Elec_abs - setSpeed);
	        if (speedError >= speedThreshold) {
	            speedProtectionCount++;
	            if (speedProtectionCount >= speedMaxProtectionCount) {
					prevSpeed = 0;
	                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
	                Ctrl_Flag.bit.speedFluctuation = 1;
	                Ctrl_Flag.bit.STOP_PWM_Flag = 1;
	            }
	        } else {
	            speedProtectionCount = 0;
	        }
	    }

	    if (Ctrl_Flag.bit.speedFluctuation == 1) {
	        exitTimeCount++;
	        if (exitTimeCount >= 200) { // 30: 4.5ms, 100: 150ms, 200: 300ms
	            speedProtectionExitCount++;
	            if (speedProtectionExitCount >= speedMaxProtectionCount) {
	                Ctrl_Flag.bit.speedFluctuation = 0;
	                speedProtectionExitCount = 0;
	            }
	        }
	    } else {
	        exitTimeCount = 0;
	        speedProtectionExitCount = 0;
	    }
	}

//----------------------------------------------------------
	ADc_Isa = AdcRegs.ADCRESULT0; 	//aÏàµçÁ÷
	ADc_Isb =  AdcRegs.ADCRESULT1;   //bÏàµçÁ÷
	ADc_Isc = AdcRegs.ADCRESULT2; 	//cÏàµçÁ÷
	ADc_Udc = AdcRegs.ADCRESULT3;  // Ö±Á÷²àµçÑ¹
    ADc_Temp4 = AdcRegs.ADCRESULT4;//TEMP_IGBT
	ADc_Idc = AdcRegs.ADCRESULT5;   //Ö±Á÷²àµçÁ÷
    ADc_Temp0 = AdcRegs.ADCRESULT6;//PT1000_IN1
    ADc_Temp2 = AdcRegs.ADCRESULT7;//PT1000_IN2

	Temp_Ia = Adc_Arith(ADc_Isa);
	isa = _IQ17mpy((Temp_Ia - Off_Isa), K_Isa);
	Isa = _IQmpy(isa << 3, I_Base);			// Isa->±êçÛÖµ

	Temp_Ib = Adc_Arith(ADc_Isb);
	isb = _IQ17mpy((Temp_Ib - Off_Isb), K_Isb);
	Isb = _IQmpy(isb << 3, I_Base);			// Isb->±êçÛÖµ                       I_BaseÎªµ¹Êý£¡£¡£¡
	

	Temp_Ic = Adc_Arith(ADc_Isc);
	isc = _IQ17mpy((Temp_Ic - Off_Isc), K_Isc);
	Isc = _IQmpy(isc << 3, I_Base);			// Isc->±êçÛÖµ                       I_BaseÎªµ¹Êý£¡£¡£¡


	Temp_Udc = Adc_Arith(ADc_Udc);
	udc = _IQ17mpy((Temp_Udc - Off_Udc), K_Udc);
	Udc_Mche_1 = _IQmpy(udc << 3, U_Base);			// Udc1->±êçÛÖµ               U_BaseÎªµ¹Êý£¡£¡£¡
	//-----------------------------------------------------------------
	Udc_Mche_1 = RCFilter(Udc_Mche0,Udc_Mche_1,Udc_Filter);
	Udc_Mche0 = Udc_Mche_1;

	Temp_Idc = Adc_Arith(ADc_Idc);
	idc = _IQ17mpy((Temp_Idc - Off_Idc), K_Idc);
	Idc = _IQmpy(idc << 3, I_Base);			// Udc1->±êçÛÖµ               U_BaseÎªµ¹Êý£¡£¡£¡
	//-----------------------------------------------------------------
	Idc = RCFilter(Idc0,Idc,Udc_Filter);
	Idc0 = Idc;

	Temperature1 = Adc_Temp(ADc_Temp0);
	Temperature1 = _IQmpy((Temperature1 - Temperature1_Off), Temperature1_K);
	Temperature1 = RCFilter(Temperature10,Temperature1,Temp_Filter);
	Temperature10 = Temperature1;

	Temperature3 = Adc_Temp(ADc_Temp2);
	Temperature3 = _IQmpy((Temperature3 - Temperature3_Off), Temperature3_K);
	Temperature3 = RCFilter(Temperature30,Temperature3,Temp_Filter);
	Temperature30 = Temperature3;

	Temperature5 = Adc_Temp(ADc_Temp4);
	Temperature5 = _IQmpy((Temperature5 - Temperature5_Off), Temperature5_K);
	Temperature5 = RCFilter(Temperature50,Temperature5,Temp_Filter);
	Temperature50 = Temperature5;

	if(Temperature5 <= m_PT_V[0])
		Temperature5 = m_PT_T[0];
	else if(Temperature5 >= m_PT_V[30])
		Temperature5 = m_PT_T[30];
	else
	{
		for(i=1;i<31;i++)
		{
			if(Temperature5 <= m_PT_V[i])
			{
				Temperature5 = _IQdiv(_IQmpy(m_PT_T[i]-m_PT_T[i-1],Temperature5-m_PT_V[i-1]),m_PT_V[i]-m_PT_V[i-1])+m_PT_T[i-1];
				break;
			}
		}
	}

//==================================================================
// Id,Iq
//==================================================================
	abctodq.as = Isa;
	abctodq.bs = Isb;
	abctodq.cs = Isc;
	abctodq.ang = Angle_Elec + _IQmpy(Com_angle_I,Velo_Elec);

	ABC2DQ_CALC();									// ×ø±ê±ä»»ÇóId¡¢Iq
	Id = abctodq.dr;
	Iq = abctodq.qr; 
	//ÇóId£¬Iq½áÊø	

	Id = RCFilter(Id0,Id,id_Filter);
	Id0 = Id;
	Iq = RCFilter(Iq0,Iq,id_Filter);
	Iq0 = Iq;
	
	//---------------------------------------------------------
	//Id£¬Iq±£»¤³ÌÐò	lxy
	//---------------------------------------------------------
	Is = _IQmag(Id, Iq);				
	if(Is >= id_Max)
	{
		IdProtect++;
		if(IdProtect >= IdProtectNum)
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Sys_Flag.bit.STOP_PWM_Flag_Id = 1;
			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
		}
	}
	else
	{
		IdProtect = 0;
	}

	if(Sys_Flag.bit.STOP_PWM_Flag_Id == 1 && Is < id_Max && Velo_Elec_abs < Velo_Start)
	{
		IdProtect1++;
		if(IdProtect1 >= ProtectNum)
		{
			Sys_Flag.bit.STOP_PWM_Flag_Id = 0;
			IdProtect1 = 0;
		}
	}
	else
	{
		IdProtect1 = 0;
	}
	//Id£¬Iq±£»¤³ÌÐò½áÊø

	//---------------------------------------------------------
	if(_IQabs(Udc_Mche_1) >= UdcLimit_Set)    //ÉÏÎ»»úÉè¶¨µÄÖ±Á÷²àµçÑ¹ÉÏÏÞ  ,Udc_Mche_1:Ö±Á÷µçÑ¹²âÁ¿Öµ
	{
		UdcOverProtect++;
		if(UdcOverProtect >= PROTECT_NUM)
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Sys_Flag.bit.UDC_FLAG = 1;
			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
		}
	}
	else
	{
		UdcOverProtect = 0;
	}

	if(Sys_Flag.bit.UDC_FLAG == 1 && _IQabs(Udc_Mche_1)< UdcLimit_Set
			&& Velo_Elec_abs < Velo_Start)
	{
		UdcOverProtect1++;
		if(UdcOverProtect1 >= PROTECT_NUM)
		{
			Sys_Flag.bit.UDC_FLAG = 0;
			UdcOverProtect1 = 0;
		}
	}
	else
	{
		UdcOverProtect1 = 0;
	}

	if(_IQabs(Udc_Mche_1) < UdcLimit1_Set)       //Ö±Á÷²àµçÑ¹ÏÂÏÞ
	{
		UdcLowProtect++;
		if(UdcLowProtect >= PROTECT_NUM)
		{
			if(Ctrl_Flag.bit.STOP_VELO_FLAG == 0)
				Ctrl_Flag.bit.STOP_PWM_Flag = 1;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.UdcLow_Flag = 1;
		}
	}
	else
	{
		UdcLowProtect = 0;
	}

	if(Ctrl_Flag.bit.UdcLow_Flag == 1 && _IQabs(Udc_Mche_1) > UdcLimit1_Set && Velo_Elec_abs < Velo_Start)       //Ö±Á÷²àµçÑ¹ÏÂÏÞ
	{
		UdcLowEnable++;
		if(UdcLowEnable >= PROTECT_NUM)
		{
			Ctrl_Flag.bit.UdcLow_Flag = 0;
			UdcLowEnable = 0;
		}
	}
	else
	{
		UdcLowEnable = 0;
	}

//	if(Temperature1 >= MotorTM || Temperature3 >= MotorTM)       //µç»ú¹ýÎÂ
//	{
//		Temp1OverProtect++;
//		if(Temp1OverProtect >= PROTECT_NUM)
//		{
//			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
//			Ctrl_Flag.bit.Temp1Over_Flag = 1;
//			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
//		}
//	}
//	else
//	{
//		Temp1OverProtect = 0;
//	}
//
//	if(Ctrl_Flag.bit.Temp1Over_Flag == 1 && Temperature1 < MotorTM && Temperature3 < MotorTM)
//	{
//		Temp1OverProtect1++;
//		if(Temp1OverProtect1 >= ProtectNum)
//		{
//			Ctrl_Flag.bit.Temp1Over_Flag = 0;
//			Temp1OverProtect1 = 0;
//		}
//	}
//	else
//	{
//		Temp1OverProtect1 = 0;
//	}

	if(Temperature5 >= IgbtTM)       //IGBT¹ýÎÂPT1000
	{
		Temp2OverProtect++;
		if(Temp2OverProtect >= PROTECT_NUM)
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.Temp2Over_Flag = 1;
			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
		}
	}
	else
	{
		Temp2OverProtect = 0;
	}

	if(Ctrl_Flag.bit.Temp2Over_Flag == 1 && Temperature5 < IgbtTM)
	{
		Temp2OverProtect1++;
		if(Temp2OverProtect1 >= ProtectNum)
		{
			Ctrl_Flag.bit.Temp2Over_Flag = 0;
			Temp2OverProtect1 = 0;
		}
	}
	else
	{
		Temp2OverProtect1 = 0;
	}

	if(Sys_Flag.bit.STOP_PWM_Flag_Driv == 1 && INT_IGBT == 1)
	{
		IGBTProtect1++;
		if(IGBTProtect1 >= ProtectNum)
		{
			Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;
			IGBTProtect1 = 0;
		}
	}
	else
	{
		IGBTProtect1 = 0;
	}

	if( (rdc1.FAULT & 0x40) == 0x40 )    //Ðý±ä¹ÊÕÏ
	{
		RdcProtect++;
		if(RdcProtect >= IdProtectNum)
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Fault_Flag.bit.RDC1_FAULT = 1;
			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
		}
	}
	else
	{
		RdcProtect = 0;
	}

	if(Fault_Flag.bit.RDC1_FAULT == 1 && (rdc1.FAULT & 0x40) == 0x00
			&& Velo_Elec_abs < Velo_Start)
	{
		RdcProtect1++;
		if(RdcProtect1 >= ProtectNum)
		{
			Fault_Flag.bit.RDC1_FAULT = 0;
			RdcProtect1 = 0;
		}
	}
	else
	{
		RdcProtect1 = 0;
	}
	
	if( (rdc2.FAULT & 0x40) == 0x40 )    //Ðý±ä¹ÊÕÏ
	{
		Rdc2Protect++;
		if(Rdc2Protect >= IdProtectNum)
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Fault_Flag.bit.RDC2_FAULT = 1;
			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
		}
	}
	else
	{
		Rdc2Protect = 0;
	}

	if(Fault_Flag.bit.RDC2_FAULT == 1 && (rdc2.FAULT & 0x40) == 0x00
			&& Velo_Elec_abs < Velo_Start)
	{
		Rdc2Protect1++;
		if(Rdc2Protect1 >= ProtectNum)
		{
			Fault_Flag.bit.RDC2_FAULT = 0;
			Rdc2Protect1 = 0;
		}
	}
	else
	{
		Rdc2Protect1 = 0;
	}

}
//-------------------------------------------------------------------------- 
//----------------------------------------------------------------------
void Ready_Data(void)
{
	AdcRegs.ADCTRL2.all = 0x2000;					// Start SEQ1
	while (AdcRegs.ADCST.bit.INT_SEQ1== 0){}		// Wait for INT1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
		
	
	RDC_READ();									// ¶ÁÐý±äÎ»ÖÃ

	Angle_Q16 = rdc1.DATA_BUFFER - Angle_Init_Digital;
	Angle_Fa_Q16 = rdc2.DATA_BUFFER - Angle_Init_Fa;

	Ready_DataA();
}

//--------------------------------------------------------------------------
// µç¶¯×´Ì¬ÏÂµÄ¿ØÖÆ³ÌÐò
//--------------------------------------------------------------------------
void Main_ControlA(void)
{
	volatile _iq Temp_Q = 0;

	//-------------------------------------------------------------------
//	 ËÙ¶ÈµÄPIµ÷½ÚÆ÷
//-------------------------------------------------------------------
	if(Ctrl_Flag.bit.POS_CONTROL_FLAG == 1)
	{
		pi_Pos.ctrl_index++;
		if(pi_Pos.ctrl_index >= pi_Pos.ctrl_period)
		{
			pi_Pos.ctrl_index = 0;
			pi_Pos.pi_ref_reg = Pos_Set;
			pi_Pos.pi_fdb_reg = Angle_Fa_Q16;
			pi_Pos.calc(&pi_Pos);
		}
		Veloover_Set = pi_Pos.pi_out_reg;
  	}

	if(Ctrl_Flag.bit.VELO_CONTROL_FLAG == 1)
	{
		if(Veloover_Set - Velo_Set > Velo_PerAdd)     //Veloover_Set±íÊ¾µÄÊÇ£¿ÕâÒ»´ÎµÄËÙ¶ÈÉè¶¨Öµ£¿£¿
			Velo_Set = Velo_Set + Velo_PerAdd;
		else if(Velo_Set - Veloover_Set > Velo_PerAdd)
			Velo_Set = Velo_Set - Velo_PerAdd;
		else
			Velo_Set = Veloover_Set;
		pi_velo.ctrl_index++;
		if(pi_velo.ctrl_index >= pi_velo.ctrl_period)
		{
			pi_velo.ctrl_index = 0;
			pi_velo.pi_ref_reg = Velo_Set;
			pi_velo.pi_fdb_reg = Velo_Elec;
			pi_velo.calc(&pi_velo);
		}



		if(IdSet >= 0)
		{
			if(Isd_Set - IdSet > Is_PerAdd)     //¿ØÖÆµçÁ÷ÉÏÉý¡¢ÏÂ½µÐ±ÂÊ
				IdSet = IdSet + Is_PerAdd;
			else if(IdSet - Isd_Set > Is_DownPerAdd)
				IdSet = IdSet - Is_DownPerAdd;
			else
				IdSet = Isd_Set;
		}
		else
		{
			if(Isd_Set - IdSet > Is_DownPerAdd)
				IdSet = IdSet + Is_DownPerAdd;
			else if(IdSet - Isd_Set > Is_PerAdd)
				IdSet = IdSet - Is_PerAdd;
			else
				IdSet = Isd_Set;
		}
		Id_Set = -_IQmpy(_IQabs(pi_velo.pi_out_reg), _IQsinPU(TorqueAngleA)) + IdSet;
		Iq_Set = -_IQmpy(pi_velo.pi_out_reg, _IQcosPU(TorqueAngleA));      //ÕâÀïµÄÕýÓàÏÒÃ»ÓÐÓÃ·´£¿£¿£¿£¡£¡£¡£¬¸ººÅÊÇ£¿£¿£¿
  		Isdq_Set2 = Iq_Set;
  	}
//--------------------------------------------------------------
//×ª¾Ø¿ØÖÆ													//	????????????lxy
//--------------------------------------------------------------
	else
	{
		Velo_Set = Velo_Elec;
		Isdq_Set = Isdq_Set1;

		if(Isdq_Set2 >= 0)
		{
			if(Isdq_Set - Isdq_Set2 > Is_PerAdd)     //¿ØÖÆµçÁ÷ÉÏÉý¡¢ÏÂ½µÐ±ÂÊ
				Isdq_Set2 = Isdq_Set2 + Is_PerAdd;
			else if(Isdq_Set2 - Isdq_Set > Is_DownPerAdd)
				Isdq_Set2 = Isdq_Set2 - Is_DownPerAdd;
			else
				Isdq_Set2 = Isdq_Set;
		}
		else
		{
			if(Isdq_Set - Isdq_Set2 > Is_DownPerAdd)
				Isdq_Set2 = Isdq_Set2 + Is_DownPerAdd;
			else if(Isdq_Set2 - Isdq_Set > Is_PerAdd)
				Isdq_Set2 = Isdq_Set2 - Is_PerAdd;
			else
				Isdq_Set2 = Isdq_Set;
		}


		if(IdSet >= 0)
		{
			if(Isd_Set - IdSet > Is_PerAdd)     //¿ØÖÆµçÁ÷ÉÏÉý¡¢ÏÂ½µÐ±ÂÊ
				IdSet = IdSet + Is_PerAdd;
			else if(IdSet - Isd_Set > Is_DownPerAdd)
				IdSet = IdSet - Is_DownPerAdd;
			else
				IdSet = Isd_Set;
		}
		else
		{
			if(Isd_Set - IdSet > Is_DownPerAdd)
				IdSet = IdSet + Is_DownPerAdd;
			else if(IdSet - Isd_Set > Is_PerAdd)
				IdSet = IdSet - Is_PerAdd;
			else
				IdSet = Isd_Set;
		}

		Id_Set = -_IQmpy(_IQabs(Isdq_Set2), _IQsinPU(TorqueAngleA)) + IdSet;   ///ÕâÀïµÄTorqueAngleÒª²»Òªµ÷Õû
		Iq_Set = _IQmpy(Isdq_Set2, _IQcosPU(TorqueAngleA));             //âÀïµÄTorqueAngleÒª²»Òªµ÷Õû

	}
	if(Ctrl_Flag.bit.UsCtrl_Flag == 1)
	{
		pi_usLimit.ctrl_index++;
		if(pi_usLimit.ctrl_index >= pi_usLimit.ctrl_period)
		{
			pi_usLimit.ctrl_index = 0;
			pi_usLimit.pi_ref_reg = _IQmpy(Us_Max, UsSet);   //ÕâÀïÏà³ËÊÇÊ²Ã´ÒâË¼£¿£¿
			pi_usLimit.pi_fdb_reg = Us;
			pi_usLimit.calc(&pi_usLimit);
		}
	}


	if(Ctrl_Flag.bit.TORQ_CONTROL_FLAG	== 1)
	{
		IqZ_Set = Iq_Set;

		IdZ_Set = Id_Set + pi_usLimit.pi_out_reg;

		if(_IQmag(IdZ_Set, IqZ_Set) > idSet_Max)
		{
			Temp_Q = _IQasin4PU(IdZ_Set, IqZ_Set);          //ËÄÏóÏÞµÄ·´ÕýÏÒº¯Êý
			IdZ_Set = _IQmpy(idSet_Max, _IQcosPU(Temp_Q));//°´Éè¶¨ÏÞµÄ×î´óÖµÊä³öUd
			IqZ_Set = _IQmpy(idSet_Max, _IQsinPU(Temp_Q));//°´Éè¶¨ÏÞ·ùµÄ×î´óÖµÊä³öUq
		}
		// IdµÄPIµ÷½ÚÆ÷
		pi_Id_Kc.ctrl_index++;
		if(pi_Id_Kc.ctrl_index >= pi_Id_Kc.ctrl_period)
		{
			pi_Id_Kc.ctrl_index = 0;
			pi_Id_Kc.pi_ref_reg = IdZ_Set;
			pi_Id_Kc.pi_fdb_reg = Id;
			pi_Id_Kc.calc(&pi_Id_Kc);
			if((pi_Id_Kc.ui_out_max <= pi_Id_Kc.ui_reg) || (pi_Id_Kc.ui_reg <= pi_Id_Kc.ui_out_min))
			{
				Sys_Flag.bit.Flag_Id = 1;
			}
			else
			{
				Sys_Flag.bit.Flag_Id = 0;
			}
		}

		//--------------------------------------------------------------
		// IqµÄPIµ÷½ÚÆ÷
		pi_Iq_Kc.ctrl_index++;
		if(pi_Iq_Kc.ctrl_index >= pi_Iq_Kc.ctrl_period)
		{
			pi_Iq_Kc.ctrl_index = 0;
			pi_Iq_Kc.pi_ref_reg = IqZ_Set;
			pi_Iq_Kc.pi_fdb_reg = Iq;
			pi_Iq_Kc.calc(&pi_Iq_Kc);
			if((pi_Iq_Kc.ui_out_max <= pi_Iq_Kc.ui_reg) || (pi_Iq_Kc.ui_reg <= pi_Iq_Kc.ui_out_min))
			{
				Sys_Flag.bit.Flag_Iq = 1;
			}
			else
			{
				Sys_Flag.bit.Flag_Iq = 0;
			}
		}
	}

//----------------------------------------------------------------
//------------------------------------------------------------
	// ¿ØÖÆ·½³Ì
	// ÇóµÃ¸ø³öµÄUd,Uq
	// µçÑ¹·½³ÌÊ½,µÃµ½Vd,Vq
	if( Ctrl_Flag.bit.TORQ_CONTROL_FLAG	== 1 )
	{
		Ud = _IQmpy(pi_Id_Kc.ui_reg,Radq)
		   + _IQmpy(Velo_Elec,_IQmpy(Lq,pi_Iq_Kc.ui_reg))
		   + _IQmpy(pi_Id_Kc.ui_reg,Rs) + pi_Id_Kc.up_reg
		   - _IQmpy(Id,Radq);

		Uq = _IQmpy(pi_Iq_Kc.ui_reg,Radq)
		   - _IQmpy(Velo_Elec,PHI + _IQmpy(Ld,pi_Id_Kc.ui_reg))
		   + _IQmpy(pi_Iq_Kc.ui_reg,Rs)+ pi_Iq_Kc.up_reg
		   - _IQmpy(Iq,Radq);
	}

	//	Ud,UqÏÞ·ù
	Udc_Mche = Udc_Setg;           //Udc_SetgÊÇÉÏÎ»»úÉèÖÃµÄÖ±Á÷µçÑ¹µÄµ¹Êý

	Us = _IQmag(Ud, Uq);
	Us_Max = _IQmpy(Udc_Mche_realvalue, _IQ(0.70710678118654752440084436210485));
	Us_Max = _IQmpy(Us_Max, Us_M);

	if(Us > Us_Max)
	{
		Temp_Q = _IQasin4PU(Ud, Uq);          //ËÄÏóÏÞµÄ·´ÕýÏÒº¯Êý
		Ud = _IQmpy(Us_Max, _IQcosPU(Temp_Q));//°´Éè¶¨ÏÞ·ùµÄ×î´óÖµÊä³öUd
		Uq = _IQmpy(Us_Max, _IQsinPU(Temp_Q));//°´Éè¶¨ÏÞ·ùµÄ×î´óÖµÊä³öUq
		Us = _IQmag(Ud, Uq);
		Sys_Flag.bit.Flag_Us = 1;
	}
	else
	{
		Sys_Flag.bit.Flag_Us = 0;
	}
	dqtoabc.dr = Ud;
	dqtoabc.qr = Uq;
	dqtoabc.ang = Angle_Elec + _IQmpy(Com_angle_U,Velo_Elec);

	DQ2ABC_CALC();		 			// ×ø±ê±ä»»ÇóUa Ub Uc
 	Usa = dqtoabc.as;
	Usb = dqtoabc.bs;
  	Usc = dqtoabc.cs;

	phase_trans.as = Usa;
	phase_trans.bs = Usb;
	phase_trans.cs = Usc;
	THREE2TWO_CALC();
	Us_Alfa_Mche = phase_trans.ds;
	Us_Beta_Mche = phase_trans.qs;

	SVPWMA_Gen(Us_Alfa_Mche,Us_Beta_Mche);           //ºóÃæÒª²»Òª¼Ópwm¸üÐÂº¯Êý£¿£¿£¿

}


//********************************************************************************
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Ö÷¿ØÖÆº¯Êý
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
//********************************************************************************       
//---------------------------------------------------------------------------
// ¿ØÖÆÖÜÆÚ¼ÆËã³ÌÐò (100uS)  µç¶¯×´Ì¬

void ControlA(void)
{
	if(INT_IGBT==0)
	{
	    Forbid_PWMA();
		Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
	  	Sys_Flag.bit.STOP_PWM_Flag_Driv = 1;
		Ctrl_Flag.bit.STOP_PWM_Flag = 1;
	}
	if(Ctrl_Flag.bit.CANERROR_FLAG == 0 
		&& Sys_Flag.bit.STOP_PWM_Flag_Velo == 0
		&& Ctrl_Flag.bit.speedFluctuation == 0
		&& Ctrl_Flag.bit.Temp2Over_Flag == 0 
		&& Sys_Flag.bit.STOP_PWM_Flag_Id == 0
		&& Sys_Flag.bit.UDC_FLAG == 0
		&& Sys_Flag.bit.STOP_PWM_Flag_Driv == 0 
		&& Ctrl_Flag.bit.UdcLow_Flag == 0 
		&& Fault_Flag.bit.RDC1_FAULT == 0 
		&& Fault_Flag.bit.RDC2_FAULT == 0)
	{
		Ctrl_Flag.bit.STOP_PWM_Flag = 0;
	}

	if(Ctrl_Flag.bit.STOP_VELO_FLAG == 1)
	{
		if(Ctrl_Flag.bit.LOCK_FLAG == 1 
			&& Sys_Flag.bit.STOP_PWM_Flag_Velo == 0
		 	&& Ctrl_Flag.bit.speedFluctuation == 0
			&& Sys_Flag.bit.STOP_PWM_Flag_Id == 0 
			&& Sys_Flag.bit.STOP_PWM_Flag_Driv == 0)
		{
			PWMA_Update();
			MeasureInitDigital();
		}
		else if(Ctrl_Flag.bit.OPEN_LOOP_FLAG == 1 
			&& Sys_Flag.bit.STOP_PWM_Flag_Velo == 0 
			&& Ctrl_Flag.bit.speedFluctuation == 0
			&& Sys_Flag.bit.STOP_PWM_Flag_Id == 0 
			&& Sys_Flag.bit.STOP_PWM_Flag_Driv == 0)
		{
			PWMA_Update();
			OpenLoop();
		}
		else
		{
			pwma.mDuty1 = _IQ(0.5);
			pwma.mDuty2 = _IQ(0.5);
			pwma.mDuty3 = _IQ(0.5);
	    	Forbid_PWMA();
	    	Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 0;			//ÖÃ×ª¾Ø¿ØÖÆ±êÖ¾¿£¿£¿ÖÃ 0 £¿
			Ctrl_Flag.bit.VELO_CONTROL_FLAG = 0;
			Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.LOCK_FLAG = 0;
	   		Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
			Id_Set = _IQ(0);
			Iq_Set = _IQ(0);
			IdZ_Set = _IQ(0);
			IqZ_Set = _IQ(0);
			Isd_Set = IsLimit_Set;
			IdSet = _IQ(0);
			Isdq_Set = _IQ(0);
			Isdq_Set1 = _IQ(0);
			Isdq_Set2 = _IQ(0);
			Velo_Set = Velo_Elec;
			Veloover_Set = _IQ(0);
			TorqueAngleA = TorqueAngle;
			pi_usLimit.pi_out_reg = _IQ(0);
			pi_Pos.pi_out_reg 	= 	_IQ(0);	
			pi_velo.pi_out_reg 	= 	_IQ(0);			// ËÙ¶È»·µÄPI
			pi_Id_Kc.pi_out_reg = _IQ(0);
			pi_Iq_Kc.pi_out_reg = _IQ(0);

			pi_usLimit.ui_reg 	= 	_IQ(0);			// Vdc»·µÄPI
			pi_Pos.ui_reg 	= 	_IQ(0);
			pi_velo.ui_reg 	= 	_IQ(0);			// ËÙ¶È»·µÄPI
			pi_Id_Kc.ui_reg = _IQ(0);
			pi_Iq_Kc.ui_reg = _IQ(0);
		}
		Ctrl_Flag.bit.FIRST_PWM_Flag = 1;
	}
	else
	{
		Ctrl_Flag.bit.FIRST_PWM_Flag = 0;
		Ctrl_Flag.bit.LOCK_FLAG = 0;
   		Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
		Main_ControlA();
		PWMA_Update();
	}
}

void Control(void)
{
	if(Ctrl_Flag.bit.STOP_VELO_FLAG == 1)
	{
		if(Ctrl_Flag.bit.LOCK_FLAG == 0 && Ctrl_Flag.bit.OPEN_LOOP_FLAG == 0)
		{
			pwma.mDuty1 = _IQ(0.5);
			pwma.mDuty2 = _IQ(0.5);
			pwma.mDuty3 = _IQ(0.5);
	    	Forbid_PWMA();
			pwma.mHalfPeriod = Control_Period;
	 	    EPwm1Regs.TBPRD = (Uint16)Control_Period;  
		    EPwm2Regs.TBPRD = (Uint16)Control_Period;  
		    EPwm3Regs.TBPRD = (Uint16)Control_Period; 
			EPwm1Regs.DBRED = Deadtime;			/*ÉÏÉýÑØÑÓÊ±ËÀÇøÊ±¼ä£ºDBRED*TBCLK*/						
	   		EPwm1Regs.DBFED = Deadtime;			/*ÏÂ½µÑØÑÓÊ±ËÀÇøÊ±¼ä£ºDBFED*TBCLK*/
			EPwm2Regs.DBRED = Deadtime;			/*ÉÏÉýÑØÑÓÊ±ËÀÇøÊ±¼ä£ºDBRED*TBCLK*/						
	   		EPwm2Regs.DBFED = Deadtime;			/*ÏÂ½µÑØÑÓÊ±ËÀÇøÊ±¼ä£ºDBFED*TBCLK*/
			EPwm3Regs.DBRED = Deadtime;			/*ÉÏÉýÑØÑÓÊ±ËÀÇøÊ±¼ä£ºDBRED*TBCLK*/						
	   		EPwm3Regs.DBFED = Deadtime;			/*ÏÂ½µÑØÑÓÊ±ËÀÇøÊ±¼ä£ºDBFED*TBCLK*/
		  	if(Velo_Duty0 != Velo_Duty)
		  	{
				Velo_Duty0 = Velo_Duty;
				K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
				K_Velo_Cal = _IQmpy(K_Velo_Cal,Velo_Duty0);
				K_Velo_Cal = _IQdiv(K_Velo_Cal,Velo_Calc_Num<<20);
				K_Velo_Cal = _IQmpy(K_Velo_Cal,p);

				Per_Ctrl = _IQdiv(Control_Period,37500000);
				Velo_PerAdd = _IQmpy(Velo_Add,Per_Ctrl);
				Is_PerAdd = _IQmpy(Is_Add,Per_Ctrl);
				Is_DownPerAdd = _IQmpy(Is_DownAdd,Per_Ctrl);

	   			pi_Id_Kc.Ki_reg		= _IQmpy(V_Base,Per_Ctrl);
				pi_Id_Kc.Ki_reg		= _IQmpy(pi_Id_Kc.Ki_reg,(pi_Id_Kc.i_period<<20));
				pi_Id_Kc.Ki_reg		= _IQmpy(Idq_Kc,pi_Id_Kc.Ki_reg);
				pi_Iq_Kc.Ki_reg		= pi_Id_Kc.Ki_reg;
		  	}
		}
	}

	Ready_Data();
	ControlA();
	RunLed();
}


//----------------------------------------------------------------------------
// No More
//----------------------------------------------------------------------------
