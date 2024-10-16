/*=================================================================================
 文件名:     	F2812_eCAN.c
                   
 功能描述 :   CAN发送接收数据函数
				 
 编写: 	    伍泽东
 编写日期:    2011-6-21  
        
=================================================================================
*/

/*--------------------------------------------------------------------------*/
#include "F28335_Device.h"     /* F2812 外设头文件*/
#include "App_UserDef.h"      /* F2812 应用项目头文件*/
/*--------------------------------------------------------------------------*/

#pragma CODE_SECTION(ECana_T, "ramfuncs");
#pragma CODE_SECTION(Can_Tx_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx2_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx4_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx11_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx13_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Rt_Data, "ramfuncs");

void Can_Rt_Data(Uint16 Num)
{
	volatile _iq tmp=0;
	if(Num == 1)
	{
		if(BORAD_NUM == 2 || BORAD_NUM == 3)
		{
			//将转速标幺值转换为IQ15的实际转速值，再转换为16位整数,低字节在前
			tmp = _IQmpy(Velo_Elec_abs,_IQ(9.5492965855137201461330258023509));
			tmp = _IQdiv(tmp,Mp);
			tmp = _IQmpy(tmp,V_Base>>5);
			tmp = (tmp>>15);
			if(tmp >= 65535)
				tmp = 65535;
			CAN_BUFFER_RT[BORAD_NUM][0] = tmp & 0x00FF;
			CAN_BUFFER_RT[BORAD_NUM][1] = (tmp>>8) & 0x00FF;

			Fault_Flag.bit.POS_LIMIT = 0;
		}
		else
		{
			//将16位的位置转换为角度值，再*100转换为16位整数,低字节在前
			tmp = Angle_Fa_Q16;
			tmp = _IQmpy(tmp,_IQ(0.54931640625));//360/65536*100
			if(tmp >= 65535)
				tmp = 65535;
			CAN_BUFFER_RT[BORAD_NUM][0] = tmp & 0x00FF;
			CAN_BUFFER_RT[BORAD_NUM][1] = (tmp>>8) & 0x00FF;

			if( (Angle_Fa_Q16 > POSMAX) || (Angle_Fa_Q16 < POSMIN) )
				Fault_Flag.bit.POS_LIMIT = 1;
			else
				Fault_Flag.bit.POS_LIMIT = 0;
		}

		Fault_Flag.bit.STOP_VELO_FLAG = Ctrl_Flag.bit.STOP_VELO_FLAG;
		Fault_Flag.bit.STOP_PWM_Flag = Ctrl_Flag.bit.STOP_PWM_Flag;
		Fault_Flag.bit.STOP_PWM_Flag_Id = Sys_Flag.bit.STOP_PWM_Flag_Id;
		Fault_Flag.bit.STOP_PWM_Flag_Velo = Sys_Flag.bit.STOP_PWM_Flag_Velo;
		Fault_Flag.bit.STOP_PWM_Flag_Driv = Sys_Flag.bit.STOP_PWM_Flag_Driv;
		Fault_Flag.bit.UDC_FLAG = Sys_Flag.bit.UDC_FLAG;
		Fault_Flag.bit.Temp1Over_Flag = Ctrl_Flag.bit.Temp1Over_Flag;
		Fault_Flag.bit.Temp2Over_Flag = Ctrl_Flag.bit.Temp2Over_Flag;
		Fault_Flag.bit.UdcLow_Flag = Ctrl_Flag.bit.UdcLow_Flag;
		Fault_Flag.bit.CANERROR_FLAG = Ctrl_Flag.bit.CANERROR_FLAG;
		Fault_Flag.bit.RunAllow_Flag = RunAllow_Flag;

		CAN_BUFFER_RT[BORAD_NUM][2] = (Fault_Flag.all & 0x00FF);
		CAN_BUFFER_RT[BORAD_NUM][3] = ((Fault_Flag.all)>>8) & 0x00FF;

		//温度值转换为IQ15的实际值，再将其转换为单精度浮点数对应的十六进制表示
		if(Temperature5 <= _IQ(0))
			tmp = 0;
		else
			tmp = _IQmpy(Temperature5,100);
		if(tmp >= 65535)
			tmp = 65535;
		CAN_BUFFER_RT[BORAD_NUM][4] = tmp & 0x00FF;
		CAN_BUFFER_RT[BORAD_NUM][5] = (tmp>>8) & 0x00FF;

		//将母线电压标幺值转换为IQ15的实际值，再将其转换为单精度浮点数对应的十六进制表示
		if(Udc_Mche_1 <= _IQ(0))
			tmp = 0;
		else
		{
			tmp = _IQdiv(Udc_Mche_1,U_Base);
			tmp = _IQmpy(tmp,100);
		}
		if(tmp >= 65535)
			tmp = 65535;
		CAN_BUFFER_RT[BORAD_NUM][6] = tmp & 0x00FF;
		CAN_BUFFER_RT[BORAD_NUM][7] = (tmp>>8) & 0x00FF;		
		
		CAN_BUFFER_RT[BORAD_NUM][8]= 0;
	}
	if(Num == 2)
	{
		//将母线电流标幺值转换为IQ15的实际值，再将其转换为单精度浮点数对应的十六进制表示
		tmp = _IQdiv(_IQabs(Idc),I_Base);
		tmp = _IQmpy(tmp,100);
		if(tmp >= 65535)
			tmp = 65535;
		CAN_BUFFER_RT[BORAD_NUM][9] = tmp & 0x00FF;
		CAN_BUFFER_RT[BORAD_NUM][10] = (tmp>>8) & 0x00FF;
			
		//将A相电流标幺值转换为IQ15的实际值，再将其转换为单精度浮点数对应的十六进制表示
		tmp = _IQdiv(_IQabs(Isa),I_Base);
		tmp = _IQmpy(tmp,100);
		if(tmp >= 65535)
			tmp = 65535;
		CAN_BUFFER_RT[BORAD_NUM][11] = tmp & 0x00FF;
		CAN_BUFFER_RT[BORAD_NUM][12] = (tmp>>8) & 0x00FF;
			
		//将B相电流标幺值转换为IQ15的实际值，再将其转换为单精度浮点数对应的十六进制表示
		tmp = _IQdiv(_IQabs(Isb),I_Base);
		tmp = _IQmpy(tmp,100);
		if(tmp >= 65535)
			tmp = 65535;
		CAN_BUFFER_RT[BORAD_NUM][13] = tmp & 0x00FF;
		CAN_BUFFER_RT[BORAD_NUM][14] = (tmp>>8) & 0x00FF;
			
		//将C相电流标幺值转换为IQ15的实际值，再将其转换为单精度浮点数对应的十六进制表示
		tmp = _IQdiv(_IQabs(Isc),I_Base);
		tmp = _IQmpy(tmp,100);
		if(tmp >= 65535)
			tmp = 65535;
		CAN_BUFFER_RT[BORAD_NUM][15] = tmp & 0x00FF;
		CAN_BUFFER_RT[BORAD_NUM][16] = (tmp>>8) & 0x00FF;
	}
}



void Can_Ctrl(Uint16 Board)
{
	volatile _iq tmp=0;

	if(Board == 0)
	{	
		if( (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5)
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
		else if(CAN_BUFFER_RX[2] == 0x12)
		{
			Isd_Set = 0;
			tmp = Hex_Float(CAN_BUFFER_RX[3],CAN_BUFFER_RX[4],CAN_BUFFER_RX[5],CAN_BUFFER_RX[6]);
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
	if(Board == 1)
	{			   
	   	if( (CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x5A) 
	   		|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xA5)
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
		else if(CAN_BUFFER_CTRL[2] == 0x12)
		{
			Isd_Set = 0;
			tmp = Hex_Float(CAN_BUFFER_CTRL[3],CAN_BUFFER_CTRL[4],CAN_BUFFER_CTRL[5],CAN_BUFFER_CTRL[6]);
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
	   	if( (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0xA5) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55)
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
		else if(CAN_BUFFER_RX[2] == 0x14)
		{
			Isd_Set = 0;
	 		TorqueAngleA = 0;
	 	  	tmp = Hex_Float(CAN_BUFFER_RX[3],CAN_BUFFER_RX[4],CAN_BUFFER_RX[5],CAN_BUFFER_RX[6]);
			Veloover_Set = _IQdiv(tmp,_IQmpy((V_Base>>5),_IQdiv(_IQ(9.5492965855137201461330258023509),Mp)));
			if(Veloover_Set > Vel_Speed)
				Veloover_Set = Vel_Speed;
			if(Ctrl_Flag.bit.VelWay_Flag == 1)
				Veloover_Set = -Veloover_Set;
	 	  	if(RunAllow_Flag == 1)
			{
				Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
				Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
				Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
				Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
			}
		}
    }
	if(Board == 3)
	{			   
	   	if( (CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0xA5) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1 
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0x55)
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
		else if(CAN_BUFFER_CTRL[2] == 0x14)
		{
			Isd_Set = 0;
	 		TorqueAngleA = 0;
	 	  	tmp = Hex_Float(CAN_BUFFER_CTRL[3],CAN_BUFFER_CTRL[4],CAN_BUFFER_CTRL[5],CAN_BUFFER_CTRL[6]);
			Veloover_Set = _IQdiv(tmp,_IQmpy((V_Base>>5),_IQdiv(_IQ(9.5492965855137201461330258023509),Mp)));
			if(Veloover_Set > Vel_Speed)
				Veloover_Set = Vel_Speed;
			if(Ctrl_Flag.bit.VelWay_Flag == 1)
				Veloover_Set = -Veloover_Set;
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
	   	if( (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA)
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
		else if(CAN_BUFFER_RX[2] == 0x13)
		{
			Isd_Set = 0;
	 		tmp = Hex_Float(CAN_BUFFER_RX[3],CAN_BUFFER_RX[4],CAN_BUFFER_RX[5],CAN_BUFFER_RX[6]);
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
	if(Board == 5)
	{			   
	   	if( (CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x55) 
			|| Ctrl_Flag.bit.STOP_PWM_Flag == 1
			|| (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
		{
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
			Ctrl_Flag.bit.CAN_Ctrl = 0;
		}
		else if(CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xAA)
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
		else if(CAN_BUFFER_CTRL[2] == 0x13)
		{
			Isd_Set = 0;
	 		tmp = Hex_Float(CAN_BUFFER_CTRL[3],CAN_BUFFER_CTRL[4],CAN_BUFFER_CTRL[5],CAN_BUFFER_CTRL[6]);
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
/*--------------------------------------------------------------------------
--------------------------------------------------------------------------------
 can发送程序
-------------------------------------------------------------------------------*/
void Can_Tx_Data()
{
   	DELAY_US(1000);  
	if(ECanaRegs.CANTA.bit.TA1 == 1)
	{
		ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
		ECanaShadow.CANTA.bit.TA1 = 1;
		ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
	}
	ECanaRegs.CANMC.bit.CDR = 1;     


	if(CANB.FLAG.bit.TXBO_FLAG == 0)
 	{
		if(CANB.m_Tx_Length > 0)
		{
			if(CANB.m_Tx_Length > 0)
			{
				ECanaMboxes.MBOX1.MDL.byte.BYTE4 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
			{
	 			ECanaMboxes.MBOX1.MDL.byte.BYTE3 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
			{
	 			ECanaMboxes.MBOX1.MDL.byte.BYTE2 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX1.MDL.byte.BYTE1 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX1.MDH.byte.BYTE8 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX1.MDH.byte.BYTE7 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX1.MDH.byte.BYTE6 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
	 		if(CANB.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX1.MDH.byte.BYTE5 = *CANB.p_Tx_Buffer++;
	 			CANB.m_Tx_Length--;
			}
		 	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
			ECanaRegs.CANMC.bit.CDR = 0;    
			ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
			ECanaShadow.CANTRS.all= 0;
		    ECanaShadow.CANTRS.bit.TRS1 = 1;                 
		    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
		}
 		else
 		{
 			CANB.FLAG.bit.TXBO_FLAG = 1;
 			CANB.p_Tx_Buffer = TX_BUFFER;
 		}
 	}      
}
void Can_Tx2_Data()
{
   	DELAY_US(1000);  
	if(ECanaRegs.CANTA.bit.TA2 == 1)
	{
		ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
		ECanaShadow.CANTA.bit.TA2 = 1;
		ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
	}
	ECanaRegs.CANMC.bit.CDR = 1;     


	if(CANB2.FLAG.bit.TXBO_FLAG == 0)
 	{
		if(CANB2.m_Tx_Length > 0)
		{
			if(CANB2.m_Tx_Length > 0)
			{
				ECanaMboxes.MBOX2.MDL.byte.BYTE4 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
			{
	 			ECanaMboxes.MBOX2.MDL.byte.BYTE3 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
			{
	 			ECanaMboxes.MBOX2.MDL.byte.BYTE2 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX2.MDL.byte.BYTE1 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX2.MDH.byte.BYTE8 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX2.MDH.byte.BYTE7 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX2.MDH.byte.BYTE6 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
	 		if(CANB2.m_Tx_Length > 0)
	 		{
	 			ECanaMboxes.MBOX2.MDH.byte.BYTE5 = *CANB2.p_Tx_Buffer++;
	 			CANB2.m_Tx_Length--;
			}
		 	ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
			ECanaRegs.CANMC.bit.CDR = 0;    
			ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
			ECanaShadow.CANTRS.all= 0;
		    ECanaShadow.CANTRS.bit.TRS2 = 1;                 
		    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
		}
 		else
 		{
 			CANB2.FLAG.bit.TXBO_FLAG = 1;
 			CANB2.p_Tx_Buffer = SCI_RX_BUFFER;
 		}
 	}      
}

void Can_Tx4_Data()
{
	if(ECanaRegs.CANTA.bit.TA4 == 1)
	{
		ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
		ECanaShadow.CANTA.bit.TA4 = 1;
		ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
	}
	ECanaRegs.CANMC.bit.CDR = 1; 

	Can_Rt_Data(1);
	ECanaMboxes.MBOX4.MDL.byte.BYTE4 = CAN_BUFFER_RT[BORAD_NUM][0];
	ECanaMboxes.MBOX4.MDL.byte.BYTE3 = CAN_BUFFER_RT[BORAD_NUM][1];
	ECanaMboxes.MBOX4.MDL.byte.BYTE2 = CAN_BUFFER_RT[BORAD_NUM][2];
	ECanaMboxes.MBOX4.MDL.byte.BYTE1 = CAN_BUFFER_RT[BORAD_NUM][3];
	ECanaMboxes.MBOX4.MDH.byte.BYTE8 = CAN_BUFFER_RT[BORAD_NUM][4];
	ECanaMboxes.MBOX4.MDH.byte.BYTE7 = CAN_BUFFER_RT[BORAD_NUM][5];
	ECanaMboxes.MBOX4.MDH.byte.BYTE6 = CAN_BUFFER_RT[BORAD_NUM][6];
	ECanaMboxes.MBOX4.MDH.byte.BYTE5 = CAN_BUFFER_RT[BORAD_NUM][7];

	ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
	ECanaRegs.CANMC.bit.CDR = 0;    
	ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
	ECanaShadow.CANTRS.all= 0;
    ECanaShadow.CANTRS.bit.TRS4 = 1;                 
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
}

void Can_Tx13_Data()
{
	if(ECanaRegs.CANTA.bit.TA13 == 1)
	{
		ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
		ECanaShadow.CANTA.bit.TA13 = 1;
		ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
	}
	ECanaRegs.CANMC.bit.CDR = 1; 

	Can_Rt_Data(2);
	ECanaMboxes.MBOX13.MDL.byte.BYTE4 = CAN_BUFFER_RT[BORAD_NUM][9];
	ECanaMboxes.MBOX13.MDL.byte.BYTE3 = CAN_BUFFER_RT[BORAD_NUM][10];
	ECanaMboxes.MBOX13.MDL.byte.BYTE2 = CAN_BUFFER_RT[BORAD_NUM][11];
	ECanaMboxes.MBOX13.MDL.byte.BYTE1 = CAN_BUFFER_RT[BORAD_NUM][12];
	ECanaMboxes.MBOX13.MDH.byte.BYTE8 = CAN_BUFFER_RT[BORAD_NUM][13];
	ECanaMboxes.MBOX13.MDH.byte.BYTE7 = CAN_BUFFER_RT[BORAD_NUM][14];
	ECanaMboxes.MBOX13.MDH.byte.BYTE6 = CAN_BUFFER_RT[BORAD_NUM][15];
	ECanaMboxes.MBOX13.MDH.byte.BYTE5 = CAN_BUFFER_RT[BORAD_NUM][16];

	ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
	ECanaRegs.CANMC.bit.CDR = 0;    
	ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
	ECanaShadow.CANTRS.all= 0;
    ECanaShadow.CANTRS.bit.TRS13 = 1;                 
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
}


void Can_Tx11_Data()
{
	if(ECanaRegs.CANTA.bit.TA11 == 1)
	{
		ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
		ECanaShadow.CANTA.bit.TA11 = 1;
		ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
	}
	ECanaRegs.CANMC.bit.CDR = 1; 

	Can_Rt_Data(1);
	Can_Rt_Data(2);
	
	ECanaMboxes.MBOX11.MDL.byte.BYTE4 = CAN_BUFFER_RX[0];
	ECanaMboxes.MBOX11.MDL.byte.BYTE3 = CAN_BUFFER_RX[1];
	ECanaMboxes.MBOX11.MDL.byte.BYTE2 = CAN_BUFFER_RX[2];
	ECanaMboxes.MBOX11.MDL.byte.BYTE1 = CAN_BUFFER_RX[3];
	ECanaMboxes.MBOX11.MDH.byte.BYTE8 = CAN_BUFFER_RX[4];
	ECanaMboxes.MBOX11.MDH.byte.BYTE7 = CAN_BUFFER_RX[5];
	ECanaMboxes.MBOX11.MDH.byte.BYTE6 = CAN_BUFFER_RX[6];
	ECanaMboxes.MBOX11.MDH.byte.BYTE5 = CAN_BUFFER_RX[7];

 	ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
	ECanaRegs.CANMC.bit.CDR = 0;    
	ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
	ECanaShadow.CANTRS.all= 0;
    ECanaShadow.CANTRS.bit.TRS11 = 1;                 
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
}


interrupt void ECana_T(void)
{
	volatile Uint16	i;
	volatile Uint16	Sum_Temp = 0;
	if(ECanaRegs.CANRMP.bit.RMP0==1)   /*等待所有RMP0置位*/
  	{
  		if(CANB.FLAG.bit.RX_FLAG == 0)// && CANB.FLAG.bit.TXBO_FLAG == 1)
	   {
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE4;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE3;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE2;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE1;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE8;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE7;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE6;
			CANB.p_Rx_Buffer++;
			*CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE5;
			CANB.p_Rx_Buffer++;

			if(CANB.p_Rx_Buffer >= (Uint16 *)(RX_BUFFER + RX_BUFFER_LENGTH))
			{
				CANB.p_Rx_Buffer = RX_BUFFER;
				CANB.m_Rx_Length = 0;
			}
			//检查包头是否为AA55,以及获取数据长度----------------------
			else if(RX_BUFFER[0]==0x00EB && RX_BUFFER[1]==0x0090
					&& RX_BUFFER[4] <= (Uint16)(RX_BUFFER_LENGTH - 7))
			{
				CANB.m_Rx_Length = RX_BUFFER[4];
				if(CANB.p_Rx_Buffer  >= (Uint16 *)(RX_BUFFER + CANB.m_Rx_Length + 7))
				{
					Sum_Temp = CalCRC16_Byte(RX_BUFFER, CANB.m_Rx_Length + 0x0005);
						
					if(Sum_Temp == RX_BUFFER[CANB.m_Rx_Length + 5] + (RX_BUFFER[CANB.m_Rx_Length + 6]<<8))
					{
						CANB.FLAG.bit.RX_FLAG = 1;
						CANB.p_Rx_Buffer = RX_BUFFER;
						CANB.m_Rx_Length = 0;
					}
					else
					{
						CANB.p_Rx_Buffer = RX_BUFFER;
						CANB.m_Rx_Length = 0;
					}

				}
			}
			else
			{
				CANB.p_Rx_Buffer = RX_BUFFER;
				CANB.m_Rx_Length = 0;
			}
	   }

  	   	ECanaRegs.CANRMP.bit.RMP0 =1;     /*清除所有RMP0位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP3==1)   /*等待所有RMP0置位*/
  	{
  		if(CANB2.FLAG.bit.RX_FLAG == 0)// && CANB2.FLAG.bit.TXBO_FLAG == 1)
	   {
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE4;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE3;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE2;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE1;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE8;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE7;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE6;
			CANB2.p_Rx_Buffer++;
			*CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE5;
			CANB2.p_Rx_Buffer++;

			if(CANB2.p_Rx_Buffer >= (Uint16 *)(SCI_TX_BUFFER + RX_BUFFER_LENGTH))
			{
				CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
				CANB2.m_Rx_Length = 0;
			}
			//检查包头是否为AA55,以及获取数据长度----------------------
			else if(SCI_TX_BUFFER[0]==0x00EB && SCI_TX_BUFFER[1]==0x0090
					&& SCI_TX_BUFFER[4] <= (Uint16)(RX_BUFFER_LENGTH - 7))
			{
				CANB2.m_Rx_Length = SCI_TX_BUFFER[4];
				if(CANB2.p_Rx_Buffer  >= (Uint16 *)(SCI_TX_BUFFER + CANB2.m_Rx_Length + 7))
				{
					Sum_Temp = CalCRC16_Byte(SCI_TX_BUFFER, CANB2.m_Rx_Length + 0x0005);
						
					if(Sum_Temp == SCI_TX_BUFFER[CANB2.m_Rx_Length + 5] + (SCI_TX_BUFFER[CANB2.m_Rx_Length + 6]<<8))
					{
						CANB2.FLAG.bit.RX_FLAG = 1;
						CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
						CANB2.m_Rx_Length = 0;
					}
					else
					{
						CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
						CANB2.m_Rx_Length = 0;
					}

				}
			}
			else
			{
				CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
				CANB2.m_Rx_Length = 0;
			}
	   }

  	   	ECanaRegs.CANRMP.bit.RMP3 =1;     /*清除所有RMP0位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP5==1)   /*等待所有RMP5置位*/
  	{
  		if(BORAD_NUM != 0)
		{
			CAN_BUFFER_RT[0][0] = ECanaMboxes.MBOX5.MDL.byte.BYTE4;
			CAN_BUFFER_RT[0][1] = ECanaMboxes.MBOX5.MDL.byte.BYTE3;
			CAN_BUFFER_RT[0][2] = ECanaMboxes.MBOX5.MDL.byte.BYTE2;
			CAN_BUFFER_RT[0][3] = ECanaMboxes.MBOX5.MDL.byte.BYTE1;
			CAN_BUFFER_RT[0][4] = ECanaMboxes.MBOX5.MDH.byte.BYTE8;
			CAN_BUFFER_RT[0][5] = ECanaMboxes.MBOX5.MDH.byte.BYTE7;
			CAN_BUFFER_RT[0][6] = ECanaMboxes.MBOX5.MDH.byte.BYTE6;
			CAN_BUFFER_RT[0][7] = ECanaMboxes.MBOX5.MDH.byte.BYTE5;
			CAN_BUFFER_RT[0][8]= 0;
		}
  	   	ECanaRegs.CANRMP.bit.RMP5 =1;     /*清除所有RMP5位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP6==1)   /*等待所有RMP6置位*/
  	{
  		if(BORAD_NUM != 1)
		{
			CAN_BUFFER_RT[1][0] = ECanaMboxes.MBOX6.MDL.byte.BYTE4;
			CAN_BUFFER_RT[1][1] = ECanaMboxes.MBOX6.MDL.byte.BYTE3;
			CAN_BUFFER_RT[1][2] = ECanaMboxes.MBOX6.MDL.byte.BYTE2;
			CAN_BUFFER_RT[1][3] = ECanaMboxes.MBOX6.MDL.byte.BYTE1;
			CAN_BUFFER_RT[1][4] = ECanaMboxes.MBOX6.MDH.byte.BYTE8;
			CAN_BUFFER_RT[1][5] = ECanaMboxes.MBOX6.MDH.byte.BYTE7;
			CAN_BUFFER_RT[1][6] = ECanaMboxes.MBOX6.MDH.byte.BYTE6;
			CAN_BUFFER_RT[1][7] = ECanaMboxes.MBOX6.MDH.byte.BYTE5;
			CAN_BUFFER_RT[1][8]= 0;
		}
  	   	ECanaRegs.CANRMP.bit.RMP6 =1;     /*清除所有RMP6位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP7==1)   /*等待所有RMP7置位*/
  	{
  		if(BORAD_NUM != 2)
		{
			CAN_BUFFER_RT[2][0] = ECanaMboxes.MBOX7.MDL.byte.BYTE4;
			CAN_BUFFER_RT[2][1] = ECanaMboxes.MBOX7.MDL.byte.BYTE3;
			CAN_BUFFER_RT[2][2] = ECanaMboxes.MBOX7.MDL.byte.BYTE2;
			CAN_BUFFER_RT[2][3] = ECanaMboxes.MBOX7.MDL.byte.BYTE1;
			CAN_BUFFER_RT[2][4] = ECanaMboxes.MBOX7.MDH.byte.BYTE8;
			CAN_BUFFER_RT[2][5] = ECanaMboxes.MBOX7.MDH.byte.BYTE7;
			CAN_BUFFER_RT[2][6] = ECanaMboxes.MBOX7.MDH.byte.BYTE6;
			CAN_BUFFER_RT[2][7] = ECanaMboxes.MBOX7.MDH.byte.BYTE5;
			CAN_BUFFER_RT[2][8]= 0;
		}
  	   	ECanaRegs.CANRMP.bit.RMP7 =1;     /*清除所有RMP7位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP8==1)   /*等待所有RMP8置位*/
  	{
  		if(BORAD_NUM != 3)
		{
			CAN_BUFFER_RT[3][0] = ECanaMboxes.MBOX8.MDL.byte.BYTE4;
			CAN_BUFFER_RT[3][1] = ECanaMboxes.MBOX8.MDL.byte.BYTE3;
			CAN_BUFFER_RT[3][2] = ECanaMboxes.MBOX8.MDL.byte.BYTE2;
			CAN_BUFFER_RT[3][3] = ECanaMboxes.MBOX8.MDL.byte.BYTE1;
			CAN_BUFFER_RT[3][4] = ECanaMboxes.MBOX8.MDH.byte.BYTE8;
			CAN_BUFFER_RT[3][5] = ECanaMboxes.MBOX8.MDH.byte.BYTE7;
			CAN_BUFFER_RT[3][6] = ECanaMboxes.MBOX8.MDH.byte.BYTE6;
			CAN_BUFFER_RT[3][7] = ECanaMboxes.MBOX8.MDH.byte.BYTE5;
			CAN_BUFFER_RT[3][8]= 0;
		}
  	   	ECanaRegs.CANRMP.bit.RMP8 =1;     /*清除所有RMP8位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP9==1)   /*等待所有RMP9置位*/
  	{
  		if(BORAD_NUM != 4)
		{
			CAN_BUFFER_RT[4][0] = ECanaMboxes.MBOX9.MDL.byte.BYTE4;
			CAN_BUFFER_RT[4][1] = ECanaMboxes.MBOX9.MDL.byte.BYTE3;
			CAN_BUFFER_RT[4][2] = ECanaMboxes.MBOX9.MDL.byte.BYTE2;
			CAN_BUFFER_RT[4][3] = ECanaMboxes.MBOX9.MDL.byte.BYTE1;
			CAN_BUFFER_RT[4][4] = ECanaMboxes.MBOX9.MDH.byte.BYTE8;
			CAN_BUFFER_RT[4][5] = ECanaMboxes.MBOX9.MDH.byte.BYTE7;
			CAN_BUFFER_RT[4][6] = ECanaMboxes.MBOX9.MDH.byte.BYTE6;
			CAN_BUFFER_RT[4][7] = ECanaMboxes.MBOX9.MDH.byte.BYTE5;
			CAN_BUFFER_RT[4][8]= 0;
		}
  	   	ECanaRegs.CANRMP.bit.RMP9 =1;     /*清除所有RMP9位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP10==1)   /*等待所有RMP10置位*/
  	{
  		if(BORAD_NUM != 5)
		{
			CAN_BUFFER_RT[5][0] = ECanaMboxes.MBOX10.MDL.byte.BYTE4;
			CAN_BUFFER_RT[5][1] = ECanaMboxes.MBOX10.MDL.byte.BYTE3;
			CAN_BUFFER_RT[5][2] = ECanaMboxes.MBOX10.MDL.byte.BYTE2;
			CAN_BUFFER_RT[5][3] = ECanaMboxes.MBOX10.MDL.byte.BYTE1;
			CAN_BUFFER_RT[5][4] = ECanaMboxes.MBOX10.MDH.byte.BYTE8;
			CAN_BUFFER_RT[5][5] = ECanaMboxes.MBOX10.MDH.byte.BYTE7;
			CAN_BUFFER_RT[5][6] = ECanaMboxes.MBOX10.MDH.byte.BYTE6;
			CAN_BUFFER_RT[5][7] = ECanaMboxes.MBOX10.MDH.byte.BYTE5;
			CAN_BUFFER_RT[5][8]= 0;
		}
  	   	ECanaRegs.CANRMP.bit.RMP10 =1;     /*清除所有RMP10位并开始接收消息*/
	} 

	if(ECanaRegs.CANRMP.bit.RMP12==1)   /*等待所有RMP10置位*/
  	{
  		/*if(Sys_Flag.bit.CAN_FLAG == 0)
		{
			CAN_BUFFER_CTRL[0] = ECanaMboxes.MBOX12.MDL.byte.BYTE4;
			CAN_BUFFER_CTRL[1] = ECanaMboxes.MBOX12.MDL.byte.BYTE3;
			CAN_BUFFER_CTRL[2] = ECanaMboxes.MBOX12.MDL.byte.BYTE2;
			CAN_BUFFER_CTRL[3] = ECanaMboxes.MBOX12.MDL.byte.BYTE1;
			CAN_BUFFER_CTRL[4] = ECanaMboxes.MBOX12.MDH.byte.BYTE8;
			CAN_BUFFER_CTRL[5] = ECanaMboxes.MBOX12.MDH.byte.BYTE7;
			CAN_BUFFER_CTRL[6] = ECanaMboxes.MBOX12.MDH.byte.BYTE6;
			CAN_BUFFER_CTRL[7] = ECanaMboxes.MBOX12.MDH.byte.BYTE5;
			Sys_Flag.bit.CAN_FLAG = 1;
		}*/
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][0] = ECanaMboxes.MBOX12.MDL.byte.BYTE4;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][1] = ECanaMboxes.MBOX12.MDL.byte.BYTE3;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][2] = ECanaMboxes.MBOX12.MDL.byte.BYTE2;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][3] = ECanaMboxes.MBOX12.MDL.byte.BYTE1;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][4] = ECanaMboxes.MBOX12.MDH.byte.BYTE8;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][5] = ECanaMboxes.MBOX12.MDH.byte.BYTE7;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][6] = ECanaMboxes.MBOX12.MDH.byte.BYTE6;
		CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][7] = ECanaMboxes.MBOX12.MDH.byte.BYTE5;
		CAN_BUFFER_CTRL_N_index++;
		if(CAN_BUFFER_CTRL_N_index > 5)
			CAN_BUFFER_CTRL_N_index = 0;

		CAN_BUFFER_CTRL_Num++;

  	   	ECanaRegs.CANRMP.bit.RMP12 =1;     /*清除所有RMP10位并开始接收消息*/
	} 

	if(ECanaRegs.CANRMP.bit.RMP14==1)   /*等待所有RMP14置位*/
  	{
  		if(BORAD_NUM != 0)
		{
			CAN_BUFFER_RT[0][9] = ECanaMboxes.MBOX14.MDL.byte.BYTE4;
			CAN_BUFFER_RT[0][10] = ECanaMboxes.MBOX14.MDL.byte.BYTE3;
			CAN_BUFFER_RT[0][11] = ECanaMboxes.MBOX14.MDL.byte.BYTE2;
			CAN_BUFFER_RT[0][12] = ECanaMboxes.MBOX14.MDL.byte.BYTE1;
			CAN_BUFFER_RT[0][13] = ECanaMboxes.MBOX14.MDH.byte.BYTE8;
			CAN_BUFFER_RT[0][14] = ECanaMboxes.MBOX14.MDH.byte.BYTE7;
			CAN_BUFFER_RT[0][15] = ECanaMboxes.MBOX14.MDH.byte.BYTE6;
			CAN_BUFFER_RT[0][16] = ECanaMboxes.MBOX14.MDH.byte.BYTE5;
		}
  	   	ECanaRegs.CANRMP.bit.RMP14 =1;     /*清除所有RMP14位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP15==1)   /*等待所有RMP15置位*/
  	{
  		if(BORAD_NUM != 1)
		{
			CAN_BUFFER_RT[1][9] = ECanaMboxes.MBOX15.MDL.byte.BYTE4;
			CAN_BUFFER_RT[1][10] = ECanaMboxes.MBOX15.MDL.byte.BYTE3;
			CAN_BUFFER_RT[1][11] = ECanaMboxes.MBOX15.MDL.byte.BYTE2;
			CAN_BUFFER_RT[1][12] = ECanaMboxes.MBOX15.MDL.byte.BYTE1;
			CAN_BUFFER_RT[1][13] = ECanaMboxes.MBOX15.MDH.byte.BYTE8;
			CAN_BUFFER_RT[1][14] = ECanaMboxes.MBOX15.MDH.byte.BYTE7;
			CAN_BUFFER_RT[1][15] = ECanaMboxes.MBOX15.MDH.byte.BYTE6;
			CAN_BUFFER_RT[1][16] = ECanaMboxes.MBOX15.MDH.byte.BYTE5;
		}
  	   	ECanaRegs.CANRMP.bit.RMP15 =1;     /*清除所有RMP15位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP16==1)   /*等待所有RMP16置位*/
  	{
  		if(BORAD_NUM != 2)
		{
			CAN_BUFFER_RT[2][9] = ECanaMboxes.MBOX16.MDL.byte.BYTE4;
			CAN_BUFFER_RT[2][10] = ECanaMboxes.MBOX16.MDL.byte.BYTE3;
			CAN_BUFFER_RT[2][11] = ECanaMboxes.MBOX16.MDL.byte.BYTE2;
			CAN_BUFFER_RT[2][12] = ECanaMboxes.MBOX16.MDL.byte.BYTE1;
			CAN_BUFFER_RT[2][13] = ECanaMboxes.MBOX16.MDH.byte.BYTE8;
			CAN_BUFFER_RT[2][14] = ECanaMboxes.MBOX16.MDH.byte.BYTE7;
			CAN_BUFFER_RT[2][15] = ECanaMboxes.MBOX16.MDH.byte.BYTE6;
			CAN_BUFFER_RT[2][16] = ECanaMboxes.MBOX16.MDH.byte.BYTE5;
		}
  	   	ECanaRegs.CANRMP.bit.RMP16 =1;     /*清除所有RMP16位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP17==1)   /*等待所有RMP17置位*/
  	{
  		if(BORAD_NUM != 3)
		{
			CAN_BUFFER_RT[3][9] = ECanaMboxes.MBOX17.MDL.byte.BYTE4;
			CAN_BUFFER_RT[3][10] = ECanaMboxes.MBOX17.MDL.byte.BYTE3;
			CAN_BUFFER_RT[3][11] = ECanaMboxes.MBOX17.MDL.byte.BYTE2;
			CAN_BUFFER_RT[3][12] = ECanaMboxes.MBOX17.MDL.byte.BYTE1;
			CAN_BUFFER_RT[3][13] = ECanaMboxes.MBOX17.MDH.byte.BYTE8;
			CAN_BUFFER_RT[3][14] = ECanaMboxes.MBOX17.MDH.byte.BYTE7;
			CAN_BUFFER_RT[3][15] = ECanaMboxes.MBOX17.MDH.byte.BYTE6;
			CAN_BUFFER_RT[3][16] = ECanaMboxes.MBOX17.MDH.byte.BYTE5;
		}
  	   	ECanaRegs.CANRMP.bit.RMP17 =1;     /*清除所有RMP17位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP18==1)   /*等待所有RMP18置位*/
  	{
  		if(BORAD_NUM != 4)
		{
			CAN_BUFFER_RT[4][9] = ECanaMboxes.MBOX18.MDL.byte.BYTE4;
			CAN_BUFFER_RT[4][10] = ECanaMboxes.MBOX18.MDL.byte.BYTE3;
			CAN_BUFFER_RT[4][11] = ECanaMboxes.MBOX18.MDL.byte.BYTE2;
			CAN_BUFFER_RT[4][12] = ECanaMboxes.MBOX18.MDL.byte.BYTE1;
			CAN_BUFFER_RT[4][13] = ECanaMboxes.MBOX18.MDH.byte.BYTE8;
			CAN_BUFFER_RT[4][14] = ECanaMboxes.MBOX18.MDH.byte.BYTE7;
			CAN_BUFFER_RT[4][15] = ECanaMboxes.MBOX18.MDH.byte.BYTE6;
			CAN_BUFFER_RT[4][16] = ECanaMboxes.MBOX18.MDH.byte.BYTE5;
		}
  	   	ECanaRegs.CANRMP.bit.RMP18 =1;     /*清除所有RMP18位并开始接收消息*/
	} 
	if(ECanaRegs.CANRMP.bit.RMP19==1)   /*等待所有RMP19置位*/
  	{
  		if(BORAD_NUM != 5)
		{
			CAN_BUFFER_RT[5][9] = ECanaMboxes.MBOX19.MDL.byte.BYTE4;
			CAN_BUFFER_RT[5][10] = ECanaMboxes.MBOX19.MDL.byte.BYTE3;
			CAN_BUFFER_RT[5][11] = ECanaMboxes.MBOX19.MDL.byte.BYTE2;
			CAN_BUFFER_RT[5][12] = ECanaMboxes.MBOX19.MDL.byte.BYTE1;
			CAN_BUFFER_RT[5][13] = ECanaMboxes.MBOX19.MDH.byte.BYTE8;
			CAN_BUFFER_RT[5][14] = ECanaMboxes.MBOX19.MDH.byte.BYTE7;
			CAN_BUFFER_RT[5][15] = ECanaMboxes.MBOX19.MDH.byte.BYTE6;
			CAN_BUFFER_RT[5][16] = ECanaMboxes.MBOX19.MDH.byte.BYTE5;
		}
  	   	ECanaRegs.CANRMP.bit.RMP19 =1;     /*清除所有RMP19位并开始接收消息*/
	}


	PieCtrlRegs.PIEACK.bit.ACK9=1;  
}

//----------------------------------------------------------------------------------------
// 把数据写入EEPROM的打包程序
// 将需要写入EEPROM的数据按照通讯规约打包成数据桢
// 入口参数:需要写入的数据地址、发送的命令字、发送的数据长度(字)
// 出口参数:无
//----------------------------------------------------------------------------------------
/*
void CANEEPROM_DataPackage(_iq	*p_tx_data_L, Uint16 m_tx_length_L)
{
	volatile Uint16	i;
	volatile Uint16	Data_Temp;
	p_EEProm_Buffer = (Uint16 *)EEPROM_BUFFER;
	p_EEProm_Buffer++;
	p_EEProm_Buffer++;
	p_EEProm_Buffer++;
	*p_EEProm_Buffer++ = ((m_tx_length_L << 2)) >> 8;
	*p_EEProm_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

	for(i = 0; i < m_tx_length_L; i++)
	{
		Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
		Data_Temp = (*p_tx_data_L++) & 0x00FF;
		*p_EEProm_Buffer++ = Data_Temp;
	}

	p_EEProm_Buffer = (Uint16 *)EEPROM_BUFFER;

}

*/
//----------------------------------------------------------------------------------------
// 发送数据打包程序
// 将需要发送的数据按照通讯规约打包成数蓁�
// 入口参数:需要发送的数据地址、发送的命令字、发送的数据长度(字)
// 出口参数:无
//----------------------------------------------------------------------------------------

void CAN_DataPackage_Tx(_iq	*p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L)
{
	volatile Uint16	i;
	volatile Uint16	Data_Temp;

	*CANB.p_Tx_Buffer++ = 0x00EB;
	*CANB.p_Tx_Buffer++ = 0x0090;
	*CANB.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

	// 发送的数据字节数加上效验字节
	*CANB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) >> 8;
	*CANB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

	for(i = 0; i < m_tx_length_L; i++)
	{
		Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
		*CANB.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
		*CANB.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
		*CANB.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = (*p_tx_data_L++) & 0x00FF;
		*CANB.p_Tx_Buffer++ = Data_Temp;
	}

	Data_Temp = CalCRC16_Byte(TX_BUFFER, (m_tx_length_L << 2) + 5);
	*CANB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* 累加求效验和*/
	*CANB.p_Tx_Buffer = (Data_Temp>>8); 					/* 累加笮а楹�*/
	CANB.p_Tx_Buffer = (Uint16 *)TX_BUFFER;
	CANB.m_Tx_Length = (m_tx_length_L<<2) + 2 + 5;

}

void CAN2_DataPackage_Tx(_iq	*p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L)
{
	volatile Uint16	i;
	volatile Uint16	Data_Temp;

	*CANB2.p_Tx_Buffer++ = 0x00EB;
	*CANB2.p_Tx_Buffer++ = 0x0090;
	*CANB2.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

	// 发送的数据字节数加上效验字节
	*CANB2.p_Tx_Buffer++ = ((m_tx_length_L << 2)) >> 8;
	*CANB2.p_Tx_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

	for(i = 0; i < m_tx_length_L; i++)
	{
		Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
		*CANB2.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
		*CANB2.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
		*CANB2.p_Tx_Buffer++ = Data_Temp;
		Data_Temp = (*p_tx_data_L++) & 0x00FF;
		*CANB2.p_Tx_Buffer++ = Data_Temp;
	}

	Data_Temp = CalCRC16_Byte(SCI_RX_BUFFER, (m_tx_length_L << 2) + 5);
	*CANB2.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* 累加求效验和*/
	*CANB2.p_Tx_Buffer = (Data_Temp>>8); 					/* 累加求效验和*/
	CANB2.p_Tx_Buffer = (Uint16 *)SCI_RX_BUFFER;
	CANB2.m_Tx_Length = (m_tx_length_L<<2) + 2 + 5;

}




void CAN_DataCombine_Rx(Uint16 *p_Rx)
{
	volatile Uint16	i=0,j=0;
	for(i=1; i<p_Rx[4]; i+=4)
	{
		j=(i-1)>>2;
		ReceData[j] = p_Rx[i+4];
		ReceData[j] = ReceData[j] << 8;
		ReceData[j] += p_Rx[i+5];
		ReceData[j] = ReceData[j] << 8;
		ReceData[j] += p_Rx[i+6];
		ReceData[j] = ReceData[j] << 8;
		ReceData[j] += p_Rx[i+7];
	}

	CANB.p_Rx_Buffer = RX_BUFFER;
}

void CAN2_DataCombine_Rx(Uint16 *p_Rx)
{
	volatile Uint16	i=0,j=0;
	for(i=1; i<p_Rx[4]; i+=4)
	{
		j=(i-1)>>2;
		ReceData[j] = p_Rx[i+4];
		ReceData[j] = ReceData[j] << 8;
		ReceData[j] += p_Rx[i+5];
		ReceData[j] = ReceData[j] << 8;
		ReceData[j] += p_Rx[i+6];
		ReceData[j] = ReceData[j] << 8;
		ReceData[j] += p_Rx[i+7];
	}

	CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
}


void Can_Deal(void)
{
	volatile	Uint16	command,i=0;
	command = (RX_BUFFER[2]);

	switch(command)
   	{
   	//---------------------------------------------------------------------------
   	case 0x0000:		//初始化检测控制器指令			lxy
		SendData[0] = ReceData[1];
		SendData[1] = BORAD_NUM;
   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   		{
	   	  	CAN_DataPackage_Tx(SendData,0x0070,2);
	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		}
   		break;
	//---------------------------------------------------------------------------
	case 0x0001:		//清除故障标志指令			lxy
			Sys_Flag.bit.STOP_PWM_Flag_Id = 0;		// "1"表明电机过流保护
			Sys_Flag.bit.STOP_PWM_Flag_Velo = 0;	// "1"表明超速保护
			Sys_Flag.bit.UDC_FLAG = 0;				// "1"表明直流电压故障
			Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;
		
		if(CANB.FLAG.bit.TXBO_FLAG ==1)
		{
			CAN_DataPackage_Tx(SendData,0x0071,0);
			CANB.FLAG.bit.TXBO_FLAG = 0;
		}
		break;
   	//---------------------------------------------------------------------------
   	//---------------------------------------------------------------------------
   	case 0x000D:		//LOCK

   		CAN_DataCombine_Rx(RX_BUFFER);
   		Usa_Lock = ReceData[0];
   		Usb_Lock = ReceData[1];
   		Usc_Lock = ReceData[2];
   		Udc_Lock = ReceData[3];
		Choose_Mche= ReceData[4];
   		Ctrl_Flag.bit.LOCK_FLAG = 1;
	   	Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   		{
	   		CAN_DataPackage_Tx(SendData,0x007D,0);
	   		CANB.FLAG.bit.TXBO_FLAG = 0;
	   	}
   		break;
   	//------------------------------------------------------------

   	 	//------------------------------------------------------------
   	   	case 0x0002:		//Read Position
   	   			SendData[0] = Isa;
	   	   		SendData[1] = Isb;
	   	   		SendData[2] = Isc;
	   	   		SendData[3] = rdc1.DATA_BUFFER;
				SendData[4] = Idc;
	   	   		//SendData[5] = IsbB;
	   	   		//SendData[6] = IscB;
	   	   		SendData[7] = rdc2.DATA_BUFFER;

   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   	   	CAN_DataPackage_Tx(SendData,0x0072,8);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;
   	   	//------------------------------------------------------------
   	   	case 0x0003:		//Openloop

   	   		CAN_DataCombine_Rx(RX_BUFFER);
   	   		Um_OpenLoop = ReceData[0];
   	   		Freq_OpenLoop = ReceData[1];
   			Udc_OpenLoop = ReceData[2];
			Choose_Mche= ReceData[3];
   	   		Ctrl_Flag.bit.OPEN_LOOP_FLAG = 1;
	   	   	Ctrl_Flag.bit.LOCK_FLAG = 0;
   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   		CAN_DataPackage_Tx(SendData,0x0073,0);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;
   	   	//------------------------------------------------------------
   	   	case 0x000E:		//Release LOCK

	   		CAN_DataCombine_Rx(RX_BUFFER);
			Choose_Mche= ReceData[0];
   	   		Ctrl_Flag.bit.LOCK_FLAG = 0;
	   	   	Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   		CAN_DataPackage_Tx(SendData,0x007E,0);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;
   	   	//---------------------------------------------------------------------------

   		case 0x0004:		//写电机参数和基值系统指令			lxy

   	   		CAN_DataCombine_Rx(RX_BUFFER);
   	   		Rs = ReceData[0];
   	   		Ld = ReceData[1];
   	   		Lq = ReceData[2];
   	   		PHI= ReceData[3];
   	   		Mp = ReceData[4];
   	   		Rp = ReceData[5];
   			p = _IQdiv(ReceData[4],ReceData[5]);
   	   		Udc_Setg= ReceData[6];
   	   	    Udc_Mche_realvalue = _IQdiv(_IQ(1),Udc_Setg);

   	   		U_Base = ReceData[7];
   	   		I_Base = ReceData[8];
   	   		V_Base = ReceData[9];		// 电速度基值(rad/s)
   	   		Angle_Init_Digital = ReceData[10];       //?????
   			Velo_Max = ReceData[11];
   			id_Max = ReceData[12];
   			Is_Add = ReceData[13];
   			Control_Period = ReceData[14];
   			id_Filter = ReceData[15];
   			Udc_Filter = ReceData[16];
   			Velo_Filter = ReceData[17];
   			Deadtime = ReceData[18];
   			Velo_Add = ReceData[19];
   			Is_DownAdd = ReceData[20];
   			Temp_Filter = ReceData[21];
   			Angle_Init_Fa = ReceData[22];
   			Com_angle_U = ReceData[23];
   			Com_angle_I = ReceData[24];
   			idSet_Max = ReceData[25];
   			//Freq_cut = ReceData[26];
   			IsLimit_Set = ReceData[27];
   			MotorTM = ReceData[28];
   			IgbtTM = ReceData[29];

   			Velo_PerAdd = _IQmpy(Velo_Add,Per_Ctrl);
			Velo_Duty = _IQdiv(3750000,Control_Period);

   			K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);//?????
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,Velo_Duty0);   //定时器周期为100us
   			K_Velo_Cal = _IQdiv(K_Velo_Cal,Velo_Calc_Num<<20);
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,p);


   			Is_PerAdd = _IQmpy(Is_Add,Per_Ctrl);
   			Is_DownPerAdd = _IQmpy(Is_DownAdd,Per_Ctrl);

   			pi_Id_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Ld);
			pi_Id_Kc.Ki_reg		= _IQmpy(V_Base,Per_Ctrl);
			pi_Id_Kc.Ki_reg		= _IQmpy(pi_Id_Kc.Ki_reg,(pi_Id_Kc.i_period<<20));
			pi_Id_Kc.Ki_reg		= _IQmpy(Idq_Kc,pi_Id_Kc.Ki_reg);
			pi_Iq_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Lq);
			pi_Iq_Kc.Ki_reg		= pi_Id_Kc.Ki_reg;

   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   		CAN_DataPackage_Tx(SendData,0x0074,0);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}

   	   		break;
   	   	//---------------------------------------------------------------------------


   	    	//---------------------------------------------------------------------------
   	    	case 0x0005:		//读电机参数和基值系统指令		lxy
   	    		SendData[0] = Rs;
   	    		SendData[1] = Ld;
   	 	 	    SendData[2] = Lq;
   	    		SendData[3] = PHI;
   	    		SendData[4] = Mp;
   	    		SendData[5] = Rp;
   	    		SendData[6]	= Udc_Setg;
   	    		SendData[7] = U_Base;
   	    		SendData[8] = I_Base;
   	    		SendData[9] = V_Base;			// 电械速度基值(rad/s)
   	    		SendData[10] = Angle_Init_Digital;
   	 		    SendData[11] = Velo_Max;
   	 		    SendData[12] = id_Max;
   	 		    SendData[13] = Is_Add;
   	 		    SendData[14] = Control_Period;
   	 		    SendData[15] = id_Filter;
   	 		    SendData[16] = Udc_Filter;
   	 	    	SendData[17] = Velo_Filter;
   	 	    	SendData[18] = Deadtime;
   	 		    SendData[19] = Velo_Add;
   	 		    SendData[20] = Is_DownAdd;
   	 	  	    SendData[21] = Temp_Filter;
   	 		    SendData[22] = Angle_Init_Fa;
   	 		    SendData[23] = Com_angle_U;
   	 		    SendData[24] = Com_angle_I;
   	 		    SendData[25] = idSet_Max;
   	 		    //SendData[26] = Freq_cut;
   	 		    SendData[27] = IsLimit_Set;
   	 		    SendData[28] = MotorTM;
   	 		    SendData[29] = IgbtTM;


   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	    		{
   	 	   	   	CAN_DataPackage_Tx(SendData,0x0075,30);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	   	}
   	    		break;
   	    	//---------------------------------------------------------------------------
   	 	case 0x0006:		//位置控制指令			lxy

   	  			if( Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
	   	 		{
	   	    		SendData[0] = 1;
	   	 		}
	   	  		else if( (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
	   	   	 	{
	   	   	    	SendData[0] = 2;
	   	   	 	}
	   	  		else
	   	  		{
	   	    	  	CAN_DataCombine_Rx(RX_BUFFER);
	   	    	    Isd_Set = ReceData[0];
	   	 		    Pos_Set = ReceData[1];
					if(Pos_Set > POSMAX)
						Pos_Set = POSMAX;
					if(Pos_Set < POSMIN)
						Pos_Set = POSMIN;
					TorqueAngleA = ReceData[2];
	   	 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
	   	 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
					Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
	   	 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
	   	 			SendData[0] = 0;
	   	  		}

   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	    		{
   	 	   		CAN_DataPackage_Tx(SendData,0x0076,1);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	     	}

   	    		break;
   	    	//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

   	  	case 0x0007:		//速度控制指令			lxy

   	  			if( Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
	   	 		{
	   	    		SendData[0] = 1;
	   	 		}
	   	  		else if( (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
	   	   	 	{
	   	   	    	SendData[0] = 2;
	   	   	 	}
	   	  		else
	   	  		{
	   	    	  	CAN_DataCombine_Rx(RX_BUFFER);
	   	    	    Isd_Set = ReceData[0];
	   	 		    Veloover_Set = ReceData[1];
	   	 		    if(Veloover_Set > Vel_Speed)
						Veloover_Set = Vel_Speed;
	   	 		    if(Veloover_Set < -Vel_Speed)
						Veloover_Set = -Vel_Speed;			
					TorqueAngleA = ReceData[2];
	   	 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
	   	 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
					Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
	   	 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
	   	 			SendData[0] = 0;
	   	  		}

   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	    		{
   	 	   		CAN_DataPackage_Tx(SendData,0x0077,1);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	     	}

   	    		break;
   	    	//---------------------------------------------------------------------------

   	    	//---------------------------------------------------------------------------
   	    	case 0x0008:		//转矩控制指令			lxy
				CAN_DataCombine_Rx(RX_BUFFER);
				Choose_Mche = ReceData[0];
				Choose_Mode = ReceData[1];
				SendData[0] = 0;
				SendData[1] = 0;
	    		if( Sys_Flag.bit.STOP_PWM_Flag_Driv == 1)
				{
					SendData[0] = 1;
				}
				else if( (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start) )
				{
					SendData[0] = 2;
				}
				else
				{
					if(Choose_Mode == 0)
					{
						Isd_Set = ReceData[2];
						Isdq_Set1 = -ReceData[3];
						TorqueAngleA = ReceData[4];
						Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;			//置转矩控制标志
						Ctrl_Flag.bit.VELO_CONTROL_FLAG = 0;
						Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
					}
		   	  		else if(Choose_Mode == 1)
		   	  		{
		   	    	    Isd_Set = ReceData[2];
		   	 		    Veloover_Set = ReceData[3];
		   	 		    if(Veloover_Set > Vel_Speed)
							Veloover_Set = Vel_Speed;
		   	 		    if(Veloover_Set < -Vel_Speed)
							Veloover_Set = -Vel_Speed;
						TorqueAngleA = ReceData[4];
		   	 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
						Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
		   	 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
		   	 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
		   	  		}
		   	  		else
		   	  		{
		   	    	    Isd_Set = ReceData[2];
		   	 		    Pos_Set = ReceData[3];
						if(Pos_Set > POSMAX)
							Pos_Set = POSMAX;
						if(Pos_Set < POSMIN)
							Pos_Set = POSMIN;
						TorqueAngleA = ReceData[4];
		   	 	  	    Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
						Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
		   	 		    Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
		   	 			Ctrl_Flag.bit.TORQ_CONTROL_FLAG	= 1;
		   	  		}
					SendData[0] = 0;
				}
   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
				{
					CAN_DataPackage_Tx(SendData,0x0078,2);
					CANB.FLAG.bit.TXBO_FLAG = 0;
				}
   	    		break;
   	    	//---------------------------------------------------------------------------
   	    	//---------------------------------------------------------------------------

   	 	case 0x0009:		//读入电机传感器指令			lxy
				CAN_DataCombine_Rx(RX_BUFFER);
				Choose_Mche= ReceData[0];
	 			Sys_Flag.bit.RDC_FAULT = rdc1.FAULT;
   	    	   	SendData[0] = Velo_Elec;
   	 		    SendData[1] = Udc_Mche_1;
   	    	   	SendData[2] = Id;
   	    	   	SendData[3] = Iq;
   	 		    SendData[4] = Sys_Flag.all;
   	    	   	SendData[5] = IdZ_Set;
   	    	   	SendData[6] = IqZ_Set;
   	    	   	SendData[7] = Us;
	   	 		SendData[8] = pi_Id_Kc.ui_reg;
				SendData[9] = pi_Iq_Kc.ui_reg;
   	    	   	SendData[10] = Ud;
   	    	   	SendData[11] = Uq;
				SendData[12] = Time_Cal;
   	    	   	//SendData[12] = Id_5;
   	    	   	SendData[13] = Temperature1;
   	    	   	SendData[14] = Veloover_Set;
   	    	   	SendData[15] = Temperature3;
   	    	   	//SendData[16] = Iq_5;
   	 		    SendData[17] = Ctrl_Flag.all;
   	    	   	SendData[18] = Temperature5;
   	    	   	SendData[19] = Idc;
   	    	   	//SendData[20] = Id_7;
   	    	   	//SendData[21] = Iq_7;
   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	    		{
   	 	   		CAN_DataPackage_Tx(SendData,0x0079,22);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	   	}
   	    		break;
   	    	//---------------------------------------------------------------------------

   	    	//---------------------------------------------------------------------------
   		case 0x000A:		//电机停转指令			lxy
			CAN_DataCombine_Rx(RX_BUFFER);
			Choose_Mche= ReceData[0];
   	  		Ctrl_Flag.bit.STOP_VELO_FLAG = 1;

   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   	  	CAN_DataPackage_Tx(SendData,0x007A,0);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;
   	   	//---------------------------------------------------------------------------

   	   	//---------------------------------------------------------------------------
   	   	case 0x000B:		//写控制参数指令		lxy
   		  	CAN_DataCombine_Rx(RX_BUFFER);
   	   		pi_velo.Kp_reg 		= ReceData[0];
   	   		pi_velo.Ki_reg 		= ReceData[3];
   			pi_velo.ui_out_max		= ReceData[6];
   			pi_velo.pi_out_max		= ReceData[6];
   			pi_velo.ui_out_min		= -ReceData[6];
   			pi_velo.pi_out_min		= -ReceData[6];
   	   		pi_velo.i_period		= ReceData[8];
   	   		pi_velo.ctrl_period		= ReceData[9];

   	   		pi_usLimit.Kp_reg 		= ReceData[1];
   	   		pi_usLimit.Ki_reg		= ReceData[2];
   			pi_usLimit.ui_out_max		= 0;
   			pi_usLimit.pi_out_max		= 0;
   			pi_usLimit.ui_out_min		= -ReceData[7];
   			pi_usLimit.pi_out_min		= -ReceData[7];
   	   		pi_usLimit.i_period 		= ReceData[5];
   			pi_usLimit.ctrl_period		= ReceData[4];
   			
   			pi_Pos.Kp_reg 		= ReceData[10];
   			pi_Pos.Ki_reg 		= ReceData[13];
   	   		pi_Pos.ui_out_max		= ReceData[16];
   			pi_Pos.pi_out_max		= ReceData[16];
   			pi_Pos.ui_out_min		= -ReceData[16];
   			pi_Pos.pi_out_min		= -ReceData[16];
   	   		pi_Pos.i_period 			= ReceData[18];
   			pi_Pos.ctrl_period		= ReceData[19];

   			/*

   			pi_Iq.Kp_reg 		= ReceData[10];
   			pi_Iq.Ki_reg 		= ReceData[13];

   	   		pi_Iq.ui_out_max		= ReceData[16];
   			pi_Iq.pi_out_max		= ReceData[16];
   			pi_Iq.ui_out_min		= -ReceData[16];
   			pi_Iq.pi_out_min		= -ReceData[16];
   	   		pi_Iq.i_period 			= ReceData[18];
   			pi_Iq.ctrl_period		= ReceData[19];*/

   			Idq_Kc = ReceData[11];
   			Radq = ReceData[12];
   			pi_Id_Kc.i_period 		= ReceData[15];
   			pi_Id_Kc.ctrl_period	= ReceData[14];
   			pi_Id_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Ld);

			pi_Id_Kc.Ki_reg		= _IQmpy(V_Base,Per_Ctrl);
			pi_Id_Kc.Ki_reg		= _IQmpy(pi_Id_Kc.Ki_reg,(pi_Id_Kc.i_period<<20));
			pi_Id_Kc.Ki_reg		= _IQmpy(Idq_Kc,pi_Id_Kc.Ki_reg);

   			pi_Id_Kc.ui_out_max		= ReceData[17];
   			pi_Id_Kc.pi_out_max		= ReceData[17];
   			pi_Id_Kc.ui_out_min		= -ReceData[17];
   			pi_Id_Kc.pi_out_min		= -ReceData[17];

			pi_Iq_Kc.i_period 		= pi_Id_Kc.i_period;
			pi_Iq_Kc.ctrl_period	= pi_Id_Kc.ctrl_period;
			pi_Iq_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Lq);
			pi_Iq_Kc.Ki_reg		= pi_Id_Kc.Ki_reg;
			pi_Iq_Kc.ui_out_max		= pi_Id_Kc.ui_out_max;
			pi_Iq_Kc.pi_out_max		= pi_Id_Kc.pi_out_max;
			pi_Iq_Kc.ui_out_min		= pi_Id_Kc.ui_out_min;
			pi_Iq_Kc.pi_out_min		= pi_Id_Kc.pi_out_min;

   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   		CAN_DataPackage_Tx(SendData,0x007B,0);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;
   	   	//---------------------------------------------------------------------------
   	  	//---------------------------------------------------------------------------

   		case 0x000C:		//读参数指令		lxy
   	  	 		SendData[0] 	= 	pi_velo.Kp_reg;
   	   		 	SendData[3] 	= 	pi_velo.Ki_reg;
   				SendData[6] 	= 	pi_velo.pi_out_max;
   				SendData[8] 	= 	pi_velo.i_period;
   	   		 	SendData[9] 	= 	pi_velo.ctrl_period;

   	  	 		SendData[1] 	= 	pi_usLimit.Kp_reg;
   	   		 	SendData[2] 	= 	pi_usLimit.Ki_reg;
   				SendData[7] 	= 	-pi_usLimit.pi_out_min;
   				SendData[5] 	= 	pi_usLimit.i_period;
   	   		 	SendData[4] 	= 	pi_usLimit.ctrl_period;

   	   		 	SendData[10]	= 	pi_Pos.Kp_reg;
   	   		 	SendData[13] 	= 	pi_Pos.Ki_reg;
   		   		SendData[16] 	= 	pi_Pos.pi_out_max;
   	   			SendData[18] 	= 	pi_Pos.i_period ;
   			 	SendData[19] 	= 	pi_Pos.ctrl_period;

   	  	 		SendData[11] 	= 	Idq_Kc;
   	   		 	SendData[12] 	= 	Radq;
   				SendData[17] 	= 	pi_Id_Kc.pi_out_max;
   				SendData[15] 	= 	pi_Id_Kc.i_period;
   	   		 	SendData[14] 	= 	pi_Id_Kc.ctrl_period;

   				//SendData[21] 	= 	HarmCtrl_Kp;
   	   			//SendData[22] 	= 	HarmCtrl_Ki;
   		   		//SendData[27] 	= 	pi_Id_5.pi_out_max;
   	   			//SendData[25] 	=	pi_Id_5.i_period;
   	   			//SendData[24] 	=	pi_Id_5.ctrl_period;
   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   		CAN_DataPackage_Tx(SendData,0x007C,30);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;
   	   	   	//---------------------------------------------------------------------------
   	   	//---------------------------------------------------------------------------
   	   	case 0x0010:		//写模馔ǖ朗噶�		lr

   	 	CAN_DataCombine_Rx(RX_BUFFER);

   	    		K_Udc 	= ReceData[0];
   	    		Off_Udc = ReceData[1];
   	    		K_Isa 	= ReceData[2];
   	    		Off_Isa = ReceData[3];
   	    		K_Isc 	= ReceData[4];
   	    		Off_Isc = ReceData[5];
				//K_IsbB 	= ReceData[6];
				//Off_IsbB = ReceData[7];
				K_Idc 	= ReceData[8];
				Off_Idc = ReceData[9];
				//K_IscB 	= ReceData[10];
				//Off_IscB = ReceData[11];
				PWMA_a = ReceData[12];
				PWMA_b = ReceData[13];
				PWMA_c = ReceData[14];
				//PWMB_a = ReceData[15];
				//PWMB_b = ReceData[16];
				//PWMB_c = ReceData[17];
				//Sys_Flag.bit.HARM_Flag = ReceData[18];
				//Freq_Min_G = ReceData[19];
				//Freq_Min_K = ReceData[20];
   	 		    Us_M = ReceData[22];
   	 		    K_Isb 	= ReceData[23];
   	 		    Off_Isb = ReceData[24];
   	 			//K_IsaB 	= ReceData[25];
   	 			//Off_IsaB = ReceData[26];

   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	    		{
   	 	   		CAN_DataPackage_Tx(SendData,0x0080,0);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	   	}
   	    		break;
   	    	//---------------------------------------------------------------------------
   	   	//---------------------------------------------------------------------------
   	    	case 0x0011:		//读参数指令		lr
   	   	 		SendData[0] = K_Udc;
   	    		SendData[1] = Off_Udc;
   	   	 		SendData[2] = K_Isa;
   	    		SendData[3] = Off_Isa;
   	   	 		SendData[4] = K_Isc;
   	    		SendData[5] = Off_Isc;
   	   	 		//SendData[6] = K_IsbB;
   	    		//SendData[7] = Off_IsbB;
   	 			SendData[8] = K_Idc;
   	    		SendData[9] = Off_Idc;
		   	    //SendData[10] = K_IscB;
		   	    //SendData[11] = Off_IscB;
				SendData[12] = PWMA_a;
				SendData[13] = PWMA_b;
				SendData[14] = PWMA_c;
				//SendData[15] = PWMB_a;
				//SendData[16] = PWMB_b;
				//SendData[17] = PWMB_c;
				//SendData[18] = Sys_Flag.bit.HARM_Flag;
				//SendData[19] = Freq_Min_G;
				//SendData[20] = Freq_Min_K;
   	 			SendData[22] = Us_M;
   	   	 		SendData[23] = K_Isb;
   	    		SendData[24] = Off_Isb;
   	   	 		//SendData[25] = K_IsaB;
   	    		//SendData[26] = Off_IsaB;

   	    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	    		{
   	 	   		CAN_DataPackage_Tx(SendData,0x0081,27);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	   	}
   	    		break;
   	    	   	//---------------------------------------------------------------------------
   	    	//---------------------------------------------------------------------------
   	 	case 0x0012:		//写控制参数指令		lxy
   	    	   	CAN_DataCombine_Rx(RX_BUFFER);

   	    	Temperature1_K = ReceData[0];
   	 		UdcLimit1_Set = ReceData[1];
   	 		//TemperatureB1_K = ReceData[2];
   	 		//Harm_LdivR = ReceData[3];
   	 		ProtectNum = ReceData[4];
   	 		UsSet = ReceData[5];
   	 		//Ctrl_Flag.bit.CanCtrl_Flag = ReceData[6];
   	 		TorqueAngle = ReceData[7];
			//TemperatureB5_K = ReceData[8];
			//TemperatureB5_Off = ReceData[9];
   	 		Temperature3_K = ReceData[10];
   	 		//TemperatureB3_K = ReceData[11];
   	 		Ctrl_Flag.bit.RotorWay_Flag = ReceData[12];
			//Sys_Flag.bit.RotorWay_Flag = ReceData[13];
   	 		UdcLimit_Set = ReceData[14];
   	 		Velo_Start = ReceData[15];	// 启动速度
   	 		Vel_Speed = ReceData[16];
   	 		Ctrl_Flag.bit.VelWay_Flag = ReceData[17];
   	  		TorquetoIs = ReceData[18];	// 转矩电流比
   	 		IdProtectNum = ReceData[19];          //????
   	 		VeloProtectNum = ReceData[20];      //???
   	 		Velo_Calc_Num = ReceData[21];
   	  		Temperature1_Off = ReceData[22];
   	  		//TemperatureB1_Off = ReceData[23];
   	  		Temperature3_Off = ReceData[24];
   	  		//TemperatureB3_Off = ReceData[25];
			//CtrlB_Flag.bit.VelWay_Flag = ReceData[26];
   	 		Ctrl_Flag.bit.UsCtrl_Flag = ReceData[27];
   	 		Temperature5_K = ReceData[28];
   	 		Temperature5_Off = ReceData[29];
   	  		IstoTorque = _IQdiv(_IQ(1),TorquetoIs);   //????
   	  	    K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);//?????
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,Velo_Duty0);   //定时器周期为100us
   			K_Velo_Cal = _IQdiv(K_Velo_Cal,Velo_Calc_Num<<20);
   	   		K_Velo_Cal = _IQmpy(K_Velo_Cal,p);
    		if(CANB.FLAG.bit.TXBO_FLAG ==1)
    		{
   	 	   		CAN_DataPackage_Tx(SendData,0x0082,0);
   	 	   		CANB.FLAG.bit.TXBO_FLAG = 0;
   	 	   	}

   	    		break;
   	    	//---------------------------------------------------------------------------
   	   	//---------------------------------------------------------------------------

   	 	case 0x0013:		//读参数指令		lxy
   			SendData[0] = Temperature1_K;
   			SendData[1] = UdcLimit1_Set;
   			//SendData[2] = TemperatureB1_K;
   			//SendData[3] = Harm_LdivR;
   			SendData[4] = ProtectNum;
   			SendData[5] = UsSet;
   			//SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
   			SendData[7] = TorqueAngle;
   			//SendData[8] = TemperatureB5_K;
   			//SendData[9] = TemperatureB5_Off;
   	   		SendData[10] = Temperature3_K;
   	   		//SendData[11] = TemperatureB3_K;
   			SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
   			//SendData[13] = Sys_Flag.bit.RotorWay_Flag;
   			SendData[14] = UdcLimit_Set;
   			SendData[15] = Velo_Start;	// 启动速度
   			SendData[16] = Vel_Speed;
   			SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
   	 		SendData[18] = TorquetoIs;	// 转矩电流比
   			SendData[19] = IdProtectNum;
   			SendData[20] = VeloProtectNum;
   			SendData[21] = Velo_Calc_Num;
   			SendData[22] = Temperature1_Off;
   			//SendData[23] = TemperatureB1_Off;
   			SendData[24] = Temperature3_Off;
   			//SendData[25] = TemperatureB3_Off;
   			//SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
   			SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
   			SendData[28] = Temperature5_K;
   			SendData[29] = Temperature5_Off;


   	   		if(CANB.FLAG.bit.TXBO_FLAG ==1)
   	   		{
   		   		CAN_DataPackage_Tx(SendData,0x0083,30);
   		   		CANB.FLAG.bit.TXBO_FLAG = 0;
   		   	}
   	   		break;

   	   	

		case 0x0060:            //写入EEPROM指令

			WriteEEProm_Flag = 1;
			//Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
	    	//Forbid_PWMA();

			if(CANB.FLAG.bit.TXBO_FLAG ==1)
			{
				CAN_DataPackage_Tx(SendData,0x00D0,0);
				CANB.FLAG.bit.TXBO_FLAG = 0;
			}
			/*while(CANB.FLAG.bit.TXBO_FLAG == 0)
			{
				Can_Tx_Data();
			}
			DINT;   
			RDC_DIS_B;
			RDC_DIS_A;
					DIS_WP = 1;
		            SendData[0] = Rs;
		            SendData[1] = Ld;
		            SendData[2] = Lq;
		            SendData[3] = PHI;
		            SendData[4] = Mp;
		            SendData[5] = Rp;
		            SendData[6]	= Udc_Setg;
		            SendData[7] = U_Base;
		            SendData[8] = I_Base;
		            SendData[9] = V_Base;			// 电械速度基值(rad/s)
		            SendData[10] = Angle_Init_Digital;
		            SendData[11] = Velo_Max;
		            SendData[12] = id_Max;
		            SendData[13] = Is_Add;
		            SendData[14] = Control_Period;
		            SendData[15] = id_Filter;
		            SendData[16] = Udc_Filter;
	             	SendData[17] = Velo_Filter;
	             	SendData[18] = Deadtime;
		            SendData[19] = Velo_Add;
		            SendData[20] = Is_DownAdd;
		            SendData[21] = Temp_Filter;
		            SendData[22] = Angle_Init_Fa;
		            SendData[23] = Com_angle_U;
		            SendData[24] = Com_angle_I;
		            SendData[25] = idSet_Max;
		            //SendData[26] = Freq_cut;
		            SendData[27] = IsLimit_Set;
		            SendData[28] = MotorTM;
		            SendData[29] = IgbtTM;

		            CANEEPROM_DataPackage(SendData,30);
		   	   		for(i = 0;i < 127;i++)
						WRITE_X25040(&RX_BUFFER[0+i],128+i,1,0);


		   	   		SendData[0] 	= 	pi_velo.Kp_reg;
		   	   		SendData[3] 	= 	pi_velo.Ki_reg;
		   	   		SendData[6] 	= 	pi_velo.pi_out_max;
		   	   		SendData[8] 	= 	pi_velo.i_period;
		   	   		SendData[9] 	= 	pi_velo.ctrl_period;

		   	   		SendData[1] 	= 	pi_usLimit.Kp_reg;
		   	   		SendData[2] 	= 	pi_usLimit.Ki_reg;
		   	   		SendData[7] 	= 	-pi_usLimit.pi_out_min;
		   	   		SendData[5] 	= 	pi_usLimit.i_period;
		   	   		SendData[4] 	= 	pi_usLimit.ctrl_period;

		   	   		SendData[10]	= 	pi_Pos.Kp_reg;
		   	   		SendData[13] 	= 	pi_Pos.Ki_reg;
		   	   		SendData[16] 	= 	pi_Pos.pi_out_max;
		   	   		SendData[18] 	= 	pi_Pos.i_period ;
		   	   		SendData[19] 	= 	pi_Pos.ctrl_period;

	   	  	 		SendData[11] 	= 	Idq_Kc;
	   	   		 	SendData[12] 	= 	Radq;
	   				SendData[17] 	= 	pi_Id_Kc.pi_out_max;
	   				SendData[15] 	= 	pi_Id_Kc.i_period;
	   	   		 	SendData[14] 	= 	pi_Id_Kc.ctrl_period;

	   				//SendData[21] 	= 	HarmCtrl_Kp;
	   	   			//SendData[22] 	= 	HarmCtrl_Ki;
	   		   		//SendData[27] 	= 	pi_Id_5.pi_out_max;
	   	   			//SendData[25] 	=	pi_Id_5.i_period;
	   	   			//SendData[24] 	=	pi_Id_5.ctrl_period;

		   	   	    CANEEPROM_DataPackage(SendData,30);
		   	     	for(i = 0;i < 127;i++)
						WRITE_X25040(&RX_BUFFER[0+i],0+i,1,0);


		   	     	SendData[0] = K_Udc;
		   	     	SendData[1] = Off_Udc;
		   	     	SendData[2] = K_Isa;
		   	     	SendData[3] = Off_Isa;
		   	     	SendData[4] = K_Isc;
		   	     	SendData[5] = Off_Isc;
   	   	 			//SendData[6] = K_IsbB;
   	    			//SendData[7] = Off_IsbB;
   	 				SendData[8] = K_Idc;
   	    			SendData[9] = Off_Idc;
		   	     	//SendData[10] = K_IscB;
		   	     	//SendData[11] = Off_IscB;
					SendData[12] = PWMA_a;
					SendData[13] = PWMA_b;
					SendData[14] = PWMA_c;
					//SendData[15] = PWMB_a;
					//SendData[16] = PWMB_b;
					//SendData[17] = PWMB_c;
					//SendData[18] = Sys_Flag.bit.HARM_Flag;
					//SendData[19] = Freq_Min_G;
					//SendData[20] = Freq_Min_K;
		   	     	SendData[22] = Us_M;
		   	     	SendData[23] = K_Isb;
		   	     	SendData[24] = Off_Isb;
		   	     	//SendData[25] = K_IsaB;
		   	     	//SendData[26] = Off_IsaB;

		   	        CANEEPROM_DataPackage(SendData,27);
		   	 		for(i = 0;i < 127;i++)
						WRITE_X25040(&RX_BUFFER[0+i],0+i,1,8);

		   	 		SendData[0] = Temperature1_K;
		   	 		SendData[1] = UdcLimit1_Set;
		   	 		//SendData[2] = TemperatureB1_K;
		   	 		//SendData[3] = Harm_LdivR;
		   	 		SendData[4] = ProtectNum;
		   	 		SendData[5] = UsSet;
		   	 		//SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
		   	 		SendData[7] = TorqueAngle;
		   			//SendData[8] = TemperatureB5_K;
		   			//SendData[9] = TemperatureB5_Off;
		   	 		SendData[10] = Temperature3_K;
		   	 		//SendData[11] = TemperatureB3_K;
		   	 		SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
   					//SendData[13] = Sys_Flag.bit.RotorWay_Flag;
		   	 		SendData[14] = UdcLimit_Set;
		   	 		SendData[15] = Velo_Start;	// 启动速度
		   	 		SendData[16] = Vel_Speed;
		   	 		SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
		   	 		SendData[18] = TorquetoIs;	// 转矩电流比
		   	 		SendData[19] = IdProtectNum;
		   	 		SendData[20] = VeloProtectNum;
		   	 		SendData[21] = Velo_Calc_Num;
		   	 		SendData[22] = Temperature1_Off;
		   	 		//SendData[23] = TemperatureB1_Off;
		   	 		SendData[24] = Temperature3_Off;
		   	 		//SendData[25] = TemperatureB3_Off;
   					//SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
		   	 		SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
		   	 		SendData[28] = Temperature5_K;
		   	 		SendData[29] = Temperature5_Off;

		   	 	    CANEEPROM_DataPackage(SendData,30);
		   	 	    for(i = 0;i < 127;i++)
						WRITE_X25040(&RX_BUFFER[0+i],128+i,1,8);
		   	 	    EN_WP = 1;	
  		 			EINT;*/

	        break;

   	   	default :
   	   		break;


   	   	}
		while(CANB.FLAG.bit.TXBO_FLAG == 0)
		//while(CANB.FLAG.bit.TXBO_FLAG == 0 && (command != 0x0060))
		{
			Can_Tx_Data();
		}

   	}

void Can_Receive(void)
{

	if(SCIB.FLAG.bit.TXBO_FLAG ==1)
	{
		SCIB.p_Tx_Buffer = SCI_TX_BUFFER;	
		SCIB.m_Tx_Length = SCI_TX_BUFFER[4] + 7;
   		SCIB.FLAG.bit.TXBO_FLAG = 0;
   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
		//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
	}
}

void Can_Send(void)
{
	volatile	Uint16	command;
	command = (SCI_RX_BUFFER[2]);

	if(command == 0)
   	{
   		SendData[0] = Board_Num;
		SendData[1] = BORAD_NUM;
   		if(CANB2.FLAG.bit.TXBO_FLAG ==1)
   		{
	   	  	CAN2_DataPackage_Tx(SendData,0x0000,2);
   		}
   	}
	else
	{
		CANB2.p_Tx_Buffer = (Uint16 *)SCI_RX_BUFFER;
		CANB2.m_Tx_Length = SCI_RX_BUFFER[4] + 7;
	}
	CANB2.FLAG.bit.TXBO_FLAG = 0;

	while(CANB2.FLAG.bit.TXBO_FLAG == 0)
	{
		Can_Tx2_Data();
	}
}


/*===========================================================================
 No more.
===========================================================================*/






