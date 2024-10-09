/*
=================================================================================

 文件名:	  App_MainLoop.c

 功能描述:
		 	  项目测试文件.
			  void MainLoop(void);

 编写者:    伍泽东
 编写日期:  2011-9-1

=================================================================================
*/


#include "F28335_Device.h"     /* F28335 外设头文件*/
#include "App_UserDef.h"      /* F28335 应用项目头文件*/

void MainLoop(void)
{
	volatile Uint32  i=0,j=0;
	volatile Uint16	Data_Temp = 0;
	volatile Uint16 ComNum = 0;
	volatile _iq Set_Temp = 0;
//	SPI读写操作

	RDC_DIS_B;
	RDC_DIS_A;
	for(i = 0;i < 128;i++)
		READ_X25040(&SCI_RX_BUFFER[0+i],128+i,1,0);	// 对应上位机控制参数界面
	Sci_DataCombine_Rx(SCI_RX_BUFFER);
	Rs = SCI_ReceData[0];
	Ld = SCI_ReceData[1];
	Lq = SCI_ReceData[2];
	PHI= SCI_ReceData[3];
	Mp = SCI_ReceData[4];
	Rp = SCI_ReceData[5];
	p = _IQdiv(SCI_ReceData[4],SCI_ReceData[5]);
	Udc_Setg= SCI_ReceData[6];      //这里是上位机设置的直流电压的倒数
	Udc_Mche_realvalue = _IQdiv(_IQ(1),Udc_Setg);  //这里是上位机设置的直流电压的值

	U_Base = SCI_ReceData[7];
	I_Base = SCI_ReceData[8];
	V_Base = SCI_ReceData[9];		// 电速度基值(rad/s)
	Angle_Init_Digital = SCI_ReceData[10];
	Velo_Max = SCI_ReceData[11];
	id_Max = SCI_ReceData[12];
	Is_Add = SCI_ReceData[13];
	Control_Period = SCI_ReceData[14];
	if(Control_Period > 9375)
		Control_Period = 9375;
	else if(Control_Period < 3750)
		Control_Period = 3750;
	id_Filter = SCI_ReceData[15];
	Udc_Filter = SCI_ReceData[16];
	Velo_Filter = SCI_ReceData[17];
	Deadtime = SCI_ReceData[18];
	Velo_Add = SCI_ReceData[19];
	Is_DownAdd = SCI_ReceData[20];
	Temp_Filter = SCI_ReceData[21];
	Angle_Init_Fa = SCI_ReceData[22];
	Com_angle_U = SCI_ReceData[23];
	Com_angle_I = SCI_ReceData[24];
	idSet_Max = SCI_ReceData[25];
	//Freq_cut = SCI_ReceData[26];
	IsLimit_Set = SCI_ReceData[27];
	MotorTM = SCI_ReceData[28];
	IgbtTM = SCI_ReceData[29];
	Velo_Duty = _IQdiv(3750000,Control_Period);
	Per_Ctrl = _IQdiv(Control_Period,37500000);
	Velo_PerAdd = _IQmpy(Velo_Add,Per_Ctrl);
	Is_PerAdd = _IQmpy(Is_Add,Per_Ctrl);
	Is_DownPerAdd = _IQmpy(Is_DownAdd,Per_Ctrl);
	pwma.mHalfPeriod = Control_Period;

	for(i = 0;i < 128;i++)
		READ_X25040(&SCI_RX_BUFFER[0+i],0+i,1,0);	// 对应上位机控制参数界面
	Sci_DataCombine_Rx(SCI_RX_BUFFER);
	pi_velo.Kp_reg 		= SCI_ReceData[0];
	pi_velo.Ki_reg 		= SCI_ReceData[3];
	pi_velo.ui_out_max		= SCI_ReceData[6];
	pi_velo.pi_out_max		= SCI_ReceData[6];
	pi_velo.ui_out_min		= -SCI_ReceData[6];
	pi_velo.pi_out_min		= -SCI_ReceData[6];
	pi_velo.i_period		= SCI_ReceData[8];
	pi_velo.ctrl_period		= SCI_ReceData[9];
	
	pi_Pos.Kp_reg 		= SCI_ReceData[10];
   	pi_Pos.Ki_reg 		= SCI_ReceData[13];
   	pi_Pos.ui_out_max		= SCI_ReceData[16];
   	pi_Pos.pi_out_max		= SCI_ReceData[16];
   	pi_Pos.ui_out_min		= -SCI_ReceData[16];
   	pi_Pos.pi_out_min		= -SCI_ReceData[16];
   	pi_Pos.i_period 			= SCI_ReceData[18];
    pi_Pos.ctrl_period		= SCI_ReceData[19];

	pi_usLimit.Kp_reg 		= SCI_ReceData[1];
	pi_usLimit.Ki_reg		= SCI_ReceData[2];
	pi_usLimit.ui_out_max		= 0;
	pi_usLimit.pi_out_max		= 0;
	pi_usLimit.ui_out_min		= -SCI_ReceData[7];
	pi_usLimit.pi_out_min		= -SCI_ReceData[7];
	pi_usLimit.i_period 		= SCI_ReceData[5];
	pi_usLimit.ctrl_period		= SCI_ReceData[4];

	Idq_Kc = SCI_ReceData[11];
	Radq = SCI_ReceData[12];
	pi_Id_Kc.i_period 		= SCI_ReceData[15];
	pi_Id_Kc.ctrl_period	= SCI_ReceData[14];
	pi_Id_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Ld);

	pi_Id_Kc.Ki_reg		= _IQmpy(V_Base,Per_Ctrl);
	pi_Id_Kc.Ki_reg		= _IQmpy(pi_Id_Kc.Ki_reg,(pi_Id_Kc.i_period<<20));
	pi_Id_Kc.Ki_reg		= _IQmpy(Idq_Kc,pi_Id_Kc.Ki_reg);

	pi_Id_Kc.ui_out_max		= SCI_ReceData[17];
	pi_Id_Kc.pi_out_max		= SCI_ReceData[17];
	pi_Id_Kc.ui_out_min		= -SCI_ReceData[17];
	pi_Id_Kc.pi_out_min		= -SCI_ReceData[17];

	pi_Iq_Kc.i_period 		= pi_Id_Kc.i_period;
	pi_Iq_Kc.ctrl_period	= pi_Id_Kc.ctrl_period;
	pi_Iq_Kc.Kp_reg 	= _IQmpy(Idq_Kc,Lq);
	pi_Iq_Kc.Ki_reg		= pi_Id_Kc.Ki_reg;
	pi_Iq_Kc.ui_out_max		= pi_Id_Kc.ui_out_max;
	pi_Iq_Kc.pi_out_max		= pi_Id_Kc.pi_out_max;
	pi_Iq_Kc.ui_out_min		= pi_Id_Kc.ui_out_min;
	pi_Iq_Kc.pi_out_min		= pi_Id_Kc.pi_out_min;

	for(i = 0;i < 128;i++)
		READ_X25040(&SCI_RX_BUFFER[0+i],0+i,1,8);	// 对应上位机控制参数界面
	
	Sci_DataCombine_Rx(SCI_RX_BUFFER);
	K_Udc 	= SCI_ReceData[0];
	Off_Udc = SCI_ReceData[1];
	K_Isa 	= SCI_ReceData[2];
	Off_Isa = SCI_ReceData[3];
	K_Isc 	= SCI_ReceData[4];
	Off_Isc = SCI_ReceData[5];
	//K_IsbB 	= SCI_ReceData[6];
	//Off_IsbB = SCI_ReceData[7];
	K_Idc 	= SCI_ReceData[8];
	Off_Idc = SCI_ReceData[9];
	//K_IscB 	= SCI_ReceData[10];
	//Off_IscB = SCI_ReceData[11];
	PWMA_a = SCI_ReceData[12];
	PWMA_b = SCI_ReceData[13];
	PWMA_c = SCI_ReceData[14];
	//PWMB_a = SCI_ReceData[15];
	//PWMB_b = SCI_ReceData[16];
	//PWMB_c = SCI_ReceData[17];
	//Sys_Flag.bit.HARM_Flag = SCI_ReceData[18];
	//Freq_Min_G = SCI_ReceData[19];
	//Freq_Min_K = SCI_ReceData[20];
	Us_M = SCI_ReceData[22];
	K_Isb 	= SCI_ReceData[23];
	Off_Isb = SCI_ReceData[24];
	//K_IsaB 	= SCI_ReceData[25];
	//Off_IsaB = SCI_ReceData[26];

	for(i = 0;i < 128;i++)
		READ_X25040(&SCI_RX_BUFFER[0+i],128+i,1,8);	// 对应上位机控制参数界面
	
	Sci_DataCombine_Rx(SCI_RX_BUFFER);

	Temperature1_K = SCI_ReceData[0];
	UdcLimit1_Set = SCI_ReceData[1];
	//TemperatureB1_K = SCI_ReceData[2];
	//Harm_LdivR = SCI_ReceData[3];
	ProtectNum = SCI_ReceData[4];
	UsSet = SCI_ReceData[5];
	//Ctrl_Flag.bit.CanCtrl_Flag = SCI_ReceData[6];
	TorqueAngle = SCI_ReceData[7];
	//TemperatureB5_K = SCI_ReceData[8];
	//TemperatureB5_Off = SCI_ReceData[9];
	Temperature3_K = SCI_ReceData[10];
	//TemperatureB3_K = SCI_ReceData[11];
	Ctrl_Flag.bit.RotorWay_Flag = SCI_ReceData[12];
	//Sys_Flag.bit.RotorWay_Flag = SCI_ReceData[13];
	UdcLimit_Set = SCI_ReceData[14];
	Velo_Start = SCI_ReceData[15];
	Vel_Speed = SCI_ReceData[16];
	Ctrl_Flag.bit.VelWay_Flag = SCI_ReceData[17];
	TorquetoIs = SCI_ReceData[18];
	IdProtectNum = SCI_ReceData[19];
	VeloProtectNum = SCI_ReceData[20];
	Velo_Calc_Num = SCI_ReceData[21];
	Temperature1_Off = SCI_ReceData[22];
	//TemperatureB1_Off = SCI_ReceData[23];
	Temperature3_Off = SCI_ReceData[24];
	//TemperatureB3_Off = SCI_ReceData[25];
	//CtrlB_Flag.bit.VelWay_Flag = SCI_ReceData[26];
	Ctrl_Flag.bit.UsCtrl_Flag = SCI_ReceData[27];
	Temperature5_K = SCI_ReceData[28];
	Temperature5_Off = SCI_ReceData[29];
	IstoTorque = _IQdiv(_IQ(1),TorquetoIs);
//-------------------------------------------------------------------------------
	Velo_Duty0 = Velo_Duty;
	K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
	K_Velo_Cal = _IQmpy(K_Velo_Cal,Velo_Duty0);
	K_Velo_Cal = _IQdiv(K_Velo_Cal,Velo_Calc_Num<<20);
	K_Velo_Cal = _IQmpy(K_Velo_Cal,p);
	
  	/*串口读写缓冲区清零*/
  	for(i = 0; i < RX_BUFFER_LENGTH; i++)
    {
		EEPROM_BUFFER[i] = 0;
    	RX_BUFFER[i] = 0;		
    	SCI_RX_BUFFER[i] = 0;
    }
  	for(i = 0; i < RX_ALLBUFFER_LENGTH; i++)
    {
		SCI_RX_ALLBUFFER[i] = 0;
    }
  	for(i = 0; i < TX_BUFFER_LENGTH; i++)
    {
    	TX_BUFFER[i] = 0;
    	SCI_TX_BUFFER[i] = 0;
    }  

	for(i = 0; i < 8; i++)
    {
    	CAN_BUFFER_RX[i] = 0;
    	CAN_BUFFER_CTRL[i] = 0;
		CAN_CHANGE_CTRL[i] = 0;
		CAN_SET_VALUE[i] = 0;
    }  

	for(i = 0; i < 6; i++)
    {
    	for(j = 0; i < 17; i++)
    		CAN_BUFFER_RT[i][j] = 0;
    } 

    for(i = 0; i < 6; i++)
    {
    	for(j = 0; i < 8; i++)
    		CAN_BUFFER_CTRL_N[i][j] = 0;
    }  

	for(i = 0; i < 52; i++)
    {
    	CAN_BUFFER_TX[i] = 0;
    }

	for(i = 0; i < 30; i++)
    {
    	ReceData[i] = 0;
    	SendData[i] = 0;
    	SCI_ReceData[i] = 0;
    	SCI_SendData[i] = 0;
    }
   
	/* 读旋变位置*/
	RDC_READ();								
	Angle0_Q16 = rdc1.DATA_BUFFER - Angle_Init_Digital;
	Angle_Fa_Q16 = rdc2.DATA_BUFFER - Angle_Init_Fa;


	
   	/*XINT1 外部中断1*/
  	EALLOW;  
   	PieVectTable.XINT1 = &XINT1_Fault_Isr;
	EDIS;
   	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
	IER |= M_INT1;

	/*TZ2中断*/
  	EALLOW;  
   	PieVectTable.EPWM2_TZINT = &TZ2_Fault_Isr;
   	EDIS;    
   	PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
	IER |= M_INT2;

	//EV 系统控制周期
   	EALLOW;  
   	PieVectTable.EPWM1_INT = &EPWM1_INT_ISR;
  	EDIS;    
   
   	IER |= M_INT3;
   	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	   	
   	/*CPUTimer0 系统控制周期中*/
   	EALLOW;  
   	PieVectTable.TINT0 = &Cpu_Timer0_ISR;
  	EDIS;    
   
   	IER |= M_INT1;
   	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
   	
   	/*ECAN接收中断*/

   	 EALLOW;		   	
  	 PieVectTable.ECAN0INTA = &ECana_T;   	   	   	
   	 EDIS;      	    	  

	//*********************
  	 PieCtrlRegs.PIEIER9.bit.INTx5 = 1;     // PIE Group 9, INT9    	   	   	
   	 IER |= M_INT9;							// Enable CPU INT 

	/*串口中断*/
   	EALLOW;		/* This is needed to write to EALLOW protected registers  */
   	PieVectTable.SCIRXINTC = &ScibRxIsr;
   	PieVectTable.SCITXINTC = &ScibTxIsr;
   	EDIS;      	    	/* Enable interrupts required for this example  */
   	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;     /* PIE Group 8, INT5  */
   	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;     /* PIE Group 8, INT6 */
   	IER |= M_INT8;							/* Enable CPU INT */ 

   	
   	Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;

    ScicRegs.SCIFFTX.bit.TXFIFOXRESET=0;
    ScicRegs.SCIFFRX.bit.RXFIFORESET=0;
	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	ScicRegs.SCIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK8=1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;

   	ECanaRegs.CANGIF0.all=0xFFFFFFFF;       // 清所有中断标志位
    ECanaRegs.CANGIF1.all=0xFFFFFFFF;

    ECanaRegs.CANRMP.bit.RMP0 =1;
	ECanaRegs.CANRMP.bit.RMP3 =1;
	ECanaRegs.CANRMP.bit.RMP5 =1;
	ECanaRegs.CANRMP.bit.RMP6 =1;
	ECanaRegs.CANRMP.bit.RMP7 =1;
	ECanaRegs.CANRMP.bit.RMP8 =1;
	ECanaRegs.CANRMP.bit.RMP9 =1;
	ECanaRegs.CANRMP.bit.RMP10 =1;
	ECanaRegs.CANRMP.bit.RMP12 =1;
	PieCtrlRegs.PIEACK.bit.ACK9=1;	
	EALLOW;
	EPwm1Regs.ETCLR.bit.INT = 1;             //清中断标志
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm2Regs.TZCLR.bit.INT = 1;
    EDIS;
	StartCpuTimer0();
   	EINT;
	for(;;)
	{
		if(INT_IGBT == 0)
		{
			Forbid_PWMA();
			Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
		  	Sys_Flag.bit.STOP_PWM_Flag_Driv = 1;
			Ctrl_Flag.bit.STOP_PWM_Flag = 1;
		}
		if(Sys_Flag.bit.STOP_PWM_Flag_Driv == 0)
		{
			EALLOW;
			EPwm2Regs.TZCLR.bit.OST = 1;
			EPwm2Regs.TZCLR.bit.INT = 1;
			EDIS;
		}

		ScibRxCheck();
		ScibTxCheck();
		if(WriteEEProm_Flag == 1 && SCIB.FLAG.bit.TXBO_FLAG == 1 && CANB.FLAG.bit.TXBO_FLAG == 1)
		{
			WriteEEProm();
		}

		if(SCIB.FLAG.bit.RX_FLAG == 1)
		{
			if(SCI_RX_BUFFER[2] == 0xA0)
			{
				if( ((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0)
		 		 || ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1) )
				{
					//for(i = 0; i < 6; i++)
					//	CAN_BUFFER_RT[i][8]= 1;
	    			Ctrl_Flag.bit.CANERROR_FLAG = 0;   	
	    			CAN_Index = 0;
					for(i = 0; i < 8; i++)
						CAN_BUFFER_RX[i] = 0;
					CAN_BUFFER_RX[0] = 	SCI_RX_BUFFER[2];
					Can_Tx11_Data();
					//Delay_Flag = 0;
					//while(Delay_Flag == 0);//等待3ms
					if(SCIB.FLAG.bit.TXBO_FLAG ==1)
					{
						*SCIB.p_Tx_Buffer++ = 0x00EB;
						*SCIB.p_Tx_Buffer++ = 0x0090;
						*SCIB.p_Tx_Buffer++ = 0x0050;

						// 发送的数据字节数加上效验字节
						*SCIB.p_Tx_Buffer++ = ComNum;
						ComNum++;
						if(ComNum > 255)
							ComNum = 0;
						*SCIB.p_Tx_Buffer++ = 0x0008;

						Data_Temp = 0;
						if(CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x02) == 0x00)
							Data_Temp = 0x0001;
						if(CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x02) == 0x00)
							Data_Temp += 0x0004;
						if(CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x02) == 0x00)
							Data_Temp += 0x0010;
						if(CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x02) == 0x00)
							Data_Temp += 0x0040;
						if(CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x02) == 0x00)
							Data_Temp += 0x0100;
						if(CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x02) == 0x00)
							Data_Temp += 0x0400;

						*SCIB.p_Tx_Buffer++ = Data_Temp & 0x00FF;
						*SCIB.p_Tx_Buffer++ = (Data_Temp>>8);
						*SCIB.p_Tx_Buffer++ = 0x0001;
						*SCIB.p_Tx_Buffer++ = 0x0001;

						Data_Temp = 0;
						if((CAN_BUFFER_RT[0][2] & 0x20) == 0x00 && (CAN_BUFFER_RT[0][3] & 0x01) == 0x00 
						&& (CAN_BUFFER_RT[1][2] & 0x20) == 0x00 && (CAN_BUFFER_RT[1][3] & 0x01) == 0x00)
							Data_Temp = 0x0001;
						if((CAN_BUFFER_RT[4][2] & 0x20) == 0x00 && (CAN_BUFFER_RT[4][3] & 0x01) == 0x00 
						&& (CAN_BUFFER_RT[5][2] & 0x20) == 0x00 && (CAN_BUFFER_RT[5][3] & 0x01) == 0x00)
							Data_Temp += 0x0004;
						if((CAN_BUFFER_RT[2][2] & 0x20) == 0x00 && (CAN_BUFFER_RT[2][3] & 0x01) == 0x00)
							Data_Temp += 0x0010;
						if((CAN_BUFFER_RT[3][2] & 0x20) == 0x00 && (CAN_BUFFER_RT[3][3] & 0x01) == 0x00)
							Data_Temp += 0x0040;

						*SCIB.p_Tx_Buffer++ = Data_Temp & 0x00FF;
						*SCIB.p_Tx_Buffer++ = 0x0005;
						*SCIB.p_Tx_Buffer++ = 0x0041;
						*SCIB.p_Tx_Buffer++ = 0x00FA;
						
						Data_Temp = CalCRC16_Byte(SCI_TX_BUFFER, 13);
						
						*SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* 累加求效验和*/
						*SCIB.p_Tx_Buffer = (Data_Temp>>8); 					/* 累加求效验和*/
						SCIB.p_Tx_Buffer = SCI_TX_BUFFER;	
						SCIB.m_Tx_Length = 8+7;

						SCIB.FLAG.bit.TXBO_FLAG = 0;
				   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
						//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
					}
				}
			}

			else if(SCI_RX_BUFFER[2] == 0xA2)
			{
				if( ((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0)
		 		 || ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1) )
				{
					//for(i = 0; i < 6; i++)
					//	CAN_BUFFER_RT[i][8]= 1;
	    			Ctrl_Flag.bit.CANERROR_FLAG = 0;   	
	    			CAN_Index = 0;
					for(i = 0; i < 8; i++)
						CAN_BUFFER_RX[i] = 0;
					CAN_BUFFER_RX[0] = 	SCI_RX_BUFFER[2];
					Can_Tx11_Data();
					if(SCIB.FLAG.bit.TXBO_FLAG ==1)
					{
						*SCIB.p_Tx_Buffer++ = 0x00EB;
						*SCIB.p_Tx_Buffer++ = 0x0090;
						*SCIB.p_Tx_Buffer++ = 0x0052;

						// 发送的数据字节数加上效验字节
						*SCIB.p_Tx_Buffer++ = ComNum;
						ComNum++;
						if(ComNum > 255)
							ComNum = 0;
						*SCIB.p_Tx_Buffer++ = 0x0036;//48+6						

						Data_Temp = 0;
						if(CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x01) == 0x00)
							Data_Temp = 0x0001;
						if(CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x00)
							Data_Temp += 0x0002;
						if(CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x01) == 0x00)
							Data_Temp += 0x0004;
						if(CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x00)
							Data_Temp += 0x0008;
						if(CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x01) == 0x00)
							Data_Temp += 0x0010;
						if(CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x00)
							Data_Temp += 0x0020;
						*SCIB.p_Tx_Buffer++ = Data_Temp & 0x00FF;
						*SCIB.p_Tx_Buffer++ = (Data_Temp>>8);//2

						CAN_CONTROL = 0x07;
						//先选择从板,如果主板无故障，且从板不在运行中，则选择主板运行
						if((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01)
							CAN_CONTROL &=	0xFE;
						if((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01)
							CAN_CONTROL &=	0xFD;
						if((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01)
							CAN_CONTROL &=	0xFB;

						CAN_BUFFER_TX[0] = CAN_SET_VALUE[0];
						CAN_BUFFER_TX[1] = CAN_SET_VALUE[1];
						CAN_BUFFER_TX[4] = CAN_SET_VALUE[2];
						CAN_BUFFER_TX[5] = CAN_SET_VALUE[3];
						CAN_BUFFER_TX[8] = CAN_SET_VALUE[4];
						CAN_BUFFER_TX[9] = CAN_SET_VALUE[5];

						if((CAN_CONTROL & 0x01) == 0x00)
						{
							CAN_BUFFER_TX[2] = CAN_BUFFER_RT[0][0];
							CAN_BUFFER_TX[3] = CAN_BUFFER_RT[0][1];
							CAN_BUFFER_TX[6+6] = CAN_BUFFER_RT[0][4];
							CAN_BUFFER_TX[7+6] = CAN_BUFFER_RT[0][5];
							CAN_BUFFER_TX[12+6] = CAN_BUFFER_RT[0][6];
							CAN_BUFFER_TX[13+6] = CAN_BUFFER_RT[0][7];
							CAN_BUFFER_TX[18+6] = CAN_BUFFER_RT[0][9];
							CAN_BUFFER_TX[19+6] = CAN_BUFFER_RT[0][10];
							CAN_BUFFER_TX[24+6] = CAN_BUFFER_RT[0][11];
							CAN_BUFFER_TX[25+6] = CAN_BUFFER_RT[0][12];
							CAN_BUFFER_TX[26+6] = CAN_BUFFER_RT[0][13];
							CAN_BUFFER_TX[27+6] = CAN_BUFFER_RT[0][14];
							CAN_BUFFER_TX[28+6] = CAN_BUFFER_RT[0][15];
							CAN_BUFFER_TX[29+6] = CAN_BUFFER_RT[0][16];

							CAN_BUFFER_TX[42+6] = 0;
							CAN_BUFFER_TX[43+6] = 0;
							CAN_BUFFER_TX[44+6] = 0;
							if((CAN_BUFFER_RT[0][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] = 0x0001;
							if((CAN_BUFFER_RT[0][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0008;
							if((CAN_BUFFER_RT[0][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0040;
							if((CAN_BUFFER_RT[0][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0002;
							if((CAN_BUFFER_RT[0][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0010;
							if((CAN_BUFFER_RT[0][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0040;
							if((CAN_BUFFER_RT[0][3] & 0x08) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0001;
							if((CAN_BUFFER_RT[0][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0004;
						}
						else
						{
							CAN_BUFFER_TX[2] = CAN_BUFFER_RT[1][0];
							CAN_BUFFER_TX[3] = CAN_BUFFER_RT[1][1];
							CAN_BUFFER_TX[6+6] = CAN_BUFFER_RT[1][4];
							CAN_BUFFER_TX[7+6] = CAN_BUFFER_RT[1][5];
							CAN_BUFFER_TX[12+6] = CAN_BUFFER_RT[1][6];
							CAN_BUFFER_TX[13+6] = CAN_BUFFER_RT[1][7];
							CAN_BUFFER_TX[18+6] = CAN_BUFFER_RT[1][9];
							CAN_BUFFER_TX[19+6] = CAN_BUFFER_RT[1][10];
							CAN_BUFFER_TX[24+6] = CAN_BUFFER_RT[1][11];
							CAN_BUFFER_TX[25+6] = CAN_BUFFER_RT[1][12];
							CAN_BUFFER_TX[26+6] = CAN_BUFFER_RT[1][13];
							CAN_BUFFER_TX[27+6] = CAN_BUFFER_RT[1][14];
							CAN_BUFFER_TX[28+6] = CAN_BUFFER_RT[1][15];
							CAN_BUFFER_TX[29+6] = CAN_BUFFER_RT[1][16];

							CAN_BUFFER_TX[42+6] = 0;
							CAN_BUFFER_TX[43+6] = 0;
							CAN_BUFFER_TX[44+6] = 0;
							if((CAN_BUFFER_RT[1][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] = 0x0001;
							if((CAN_BUFFER_RT[1][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0008;
							if((CAN_BUFFER_RT[1][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0040;
							if((CAN_BUFFER_RT[1][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0002;
							if((CAN_BUFFER_RT[1][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0010;
							if((CAN_BUFFER_RT[1][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0040;
							if((CAN_BUFFER_RT[1][3] & 0x08) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0001;
							if((CAN_BUFFER_RT[1][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0004;
						}
						if((CAN_CONTROL & 0x04) == 0x00)
						{
							CAN_BUFFER_TX[6] = CAN_BUFFER_RT[4][0];
							CAN_BUFFER_TX[7] = CAN_BUFFER_RT[4][1];
							CAN_BUFFER_TX[8+6] = CAN_BUFFER_RT[4][4];
							CAN_BUFFER_TX[9+6] = CAN_BUFFER_RT[4][5];
							CAN_BUFFER_TX[14+6] = CAN_BUFFER_RT[4][6];
							CAN_BUFFER_TX[15+6] = CAN_BUFFER_RT[4][7];
							CAN_BUFFER_TX[20+6] = CAN_BUFFER_RT[4][9];
							CAN_BUFFER_TX[21+6] = CAN_BUFFER_RT[4][10];
							CAN_BUFFER_TX[30+6] = CAN_BUFFER_RT[4][11];
							CAN_BUFFER_TX[31+6] = CAN_BUFFER_RT[4][12];
							CAN_BUFFER_TX[32+6] = CAN_BUFFER_RT[4][13];
							CAN_BUFFER_TX[33+6] = CAN_BUFFER_RT[4][14];
							CAN_BUFFER_TX[34+6] = CAN_BUFFER_RT[4][15];
							CAN_BUFFER_TX[35+6] = CAN_BUFFER_RT[4][16];

							if((CAN_BUFFER_RT[4][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0002;
							if((CAN_BUFFER_RT[4][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0010;
							if((CAN_BUFFER_RT[4][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0080;
							if((CAN_BUFFER_RT[4][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0004;
							if((CAN_BUFFER_RT[4][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0020;
							if((CAN_BUFFER_RT[4][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0080;
							if((CAN_BUFFER_RT[4][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0008;
						}
						else
						{
							CAN_BUFFER_TX[6] = CAN_BUFFER_RT[5][0];
							CAN_BUFFER_TX[7] = CAN_BUFFER_RT[5][1];
							CAN_BUFFER_TX[8+6] = CAN_BUFFER_RT[5][4];
							CAN_BUFFER_TX[9+6] = CAN_BUFFER_RT[5][5];
							CAN_BUFFER_TX[14+6] = CAN_BUFFER_RT[5][6];
							CAN_BUFFER_TX[15+6] = CAN_BUFFER_RT[5][7];
							CAN_BUFFER_TX[20+6] = CAN_BUFFER_RT[5][9];
							CAN_BUFFER_TX[21+6] = CAN_BUFFER_RT[5][10];
							CAN_BUFFER_TX[30+6] = CAN_BUFFER_RT[5][11];
							CAN_BUFFER_TX[31+6] = CAN_BUFFER_RT[5][12];
							CAN_BUFFER_TX[32+6] = CAN_BUFFER_RT[5][13];
							CAN_BUFFER_TX[33+6] = CAN_BUFFER_RT[5][14];
							CAN_BUFFER_TX[34+6] = CAN_BUFFER_RT[5][15];
							CAN_BUFFER_TX[35+6] = CAN_BUFFER_RT[5][16];

							if((CAN_BUFFER_RT[5][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0002;
							if((CAN_BUFFER_RT[5][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0010;
							if((CAN_BUFFER_RT[5][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0080;
							if((CAN_BUFFER_RT[5][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0004;
							if((CAN_BUFFER_RT[5][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0020;
							if((CAN_BUFFER_RT[5][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0080;
							if((CAN_BUFFER_RT[5][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0008;	
						}
						if((CAN_CONTROL & 0x02) == 0x00)
						{
							CAN_BUFFER_TX[10] = CAN_BUFFER_RT[2][0];
							CAN_BUFFER_TX[11] = CAN_BUFFER_RT[2][1];
							CAN_BUFFER_TX[10+6] = CAN_BUFFER_RT[2][4];
							CAN_BUFFER_TX[11+6] = CAN_BUFFER_RT[2][5];
							CAN_BUFFER_TX[16+6] = CAN_BUFFER_RT[2][6];
							CAN_BUFFER_TX[17+6] = CAN_BUFFER_RT[2][7];
							CAN_BUFFER_TX[22+6] = CAN_BUFFER_RT[2][9];
							CAN_BUFFER_TX[23+6] = CAN_BUFFER_RT[2][10];
							CAN_BUFFER_TX[36+6] = CAN_BUFFER_RT[2][11];
							CAN_BUFFER_TX[37+6] = CAN_BUFFER_RT[2][12];
							CAN_BUFFER_TX[38+6] = CAN_BUFFER_RT[2][13];
							CAN_BUFFER_TX[39+6] = CAN_BUFFER_RT[2][14];
							CAN_BUFFER_TX[40+6] = CAN_BUFFER_RT[2][15];
							CAN_BUFFER_TX[41+6] = CAN_BUFFER_RT[2][16];

							if((CAN_BUFFER_RT[2][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0004;
							if((CAN_BUFFER_RT[2][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0020;
							if((CAN_BUFFER_RT[2][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0001;
							if((CAN_BUFFER_RT[2][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0008;
							if((CAN_BUFFER_RT[2][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0002;
							if((CAN_BUFFER_RT[2][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0010;

						}
						else
						{
							CAN_BUFFER_TX[10] = CAN_BUFFER_RT[3][0];
							CAN_BUFFER_TX[11] = CAN_BUFFER_RT[3][1];
							CAN_BUFFER_TX[10+6] = CAN_BUFFER_RT[3][4];
							CAN_BUFFER_TX[11+6] = CAN_BUFFER_RT[3][5];
							CAN_BUFFER_TX[16+6] = CAN_BUFFER_RT[3][6];
							CAN_BUFFER_TX[17+6] = CAN_BUFFER_RT[3][7];
							CAN_BUFFER_TX[22+6] = CAN_BUFFER_RT[3][9];
							CAN_BUFFER_TX[23+6] = CAN_BUFFER_RT[3][10];
							CAN_BUFFER_TX[36+6] = CAN_BUFFER_RT[3][11];
							CAN_BUFFER_TX[37+6] = CAN_BUFFER_RT[3][12];
							CAN_BUFFER_TX[38+6] = CAN_BUFFER_RT[3][13];
							CAN_BUFFER_TX[39+6] = CAN_BUFFER_RT[3][14];
							CAN_BUFFER_TX[40+6] = CAN_BUFFER_RT[3][15];
							CAN_BUFFER_TX[41+6] = CAN_BUFFER_RT[3][16];

							if((CAN_BUFFER_RT[3][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0004;
							if((CAN_BUFFER_RT[3][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0020;
							if((CAN_BUFFER_RT[3][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0001;
							if((CAN_BUFFER_RT[3][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0008;
							if((CAN_BUFFER_RT[3][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0002;
							if((CAN_BUFFER_RT[3][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0010;
						}
							CAN_BUFFER_TX[45+6] = 0;
						for(i = 0; i < 46+6; i++)
							*SCIB.p_Tx_Buffer++ = CAN_BUFFER_TX[i];
						Data_Temp = CalCRC16_Byte(SCI_TX_BUFFER, 53+6);
						
						*SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* 累加求效验和*/
						*SCIB.p_Tx_Buffer = (Data_Temp>>8); 					/* 累加求效验和*/
						SCIB.p_Tx_Buffer = SCI_TX_BUFFER;	
						SCIB.m_Tx_Length = 48+6+7;

						SCIB.FLAG.bit.TXBO_FLAG = 0;
				   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
						//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
					}
				}
			}

			else if(SCI_RX_BUFFER[2] == 0xA1)
			{	

				for(i = 0; i < 8; i++)
					CAN_BUFFER_RX[i] = 0;
				CAN_BUFFER_RX[0] = 	SCI_RX_BUFFER[2];

				CAN_BUFFER_RX[1] = 	0x07;//先选择从板
				//如果主板无故障，且从板不在运行中，则选择主板运行
				if((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01)
					CAN_BUFFER_RX[1] &=	0xFE;
				if((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01)
					CAN_BUFFER_RX[1] &=	0xFD;
				if((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01)
					CAN_BUFFER_RX[1] &=	0xFB;
				CAN_CONTROL = CAN_BUFFER_RX[1];
				for(i = 2; i < 7; i++)
					CAN_BUFFER_RX[i] = SCI_RX_BUFFER[3+i];				
					
					if(CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A)
					{
						CAN_SET_VALUE[0] = 0;
						CAN_SET_VALUE[1] = 0;
					}
					else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5)
					{
						CAN_SET_VALUE[0] = 0;
						CAN_SET_VALUE[1] = 0;
					}
					else if(CAN_BUFFER_RX[2] == 0x12)
					{
						Set_Temp = Hex_Float(CAN_BUFFER_RX[3],CAN_BUFFER_RX[4],CAN_BUFFER_RX[5],CAN_BUFFER_RX[6]);
						Set_Temp = _IQmpy(Set_Temp,3200);//100>>15
						if(Set_Temp >= 65535)
							Set_Temp = 65535;
						CAN_SET_VALUE[0] = Set_Temp & 0x00FF;
						CAN_SET_VALUE[1] = (Set_Temp>>8) & 0x00FF;
		
					}

					if(CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0xA5)
					{
					    setSpeed = 0;
						CAN_SET_VALUE[4] = 0;
						CAN_SET_VALUE[5] = 0;
					}
					else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55)
					{
					    setSpeed = 0;
						CAN_SET_VALUE[4] = 0;
						CAN_SET_VALUE[5] = 0;
					}
					else if(CAN_BUFFER_RX[2] == 0x14)
					{
						Set_Temp = Hex_Float(CAN_BUFFER_RX[3],CAN_BUFFER_RX[4],CAN_BUFFER_RX[5],CAN_BUFFER_RX[6]);
						setSpeed = _IQdiv(Set_Temp,_IQmpy((V_Base>>5),_IQdiv(_IQ(9.5492965855137201461330258023509),Mp)));
						Set_Temp = (Set_Temp>>15);
						if(Set_Temp >= 65535)
							Set_Temp = 65535;
						CAN_SET_VALUE[4] = Set_Temp & 0x00FF;
						CAN_SET_VALUE[5] = (Set_Temp>>8) & 0x00FF;
					}

					if(CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55)
					{
						CAN_SET_VALUE[2] = 0;
						CAN_SET_VALUE[3] = 0;
					}
					else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA)
					{
						CAN_SET_VALUE[2] = 0;
						CAN_SET_VALUE[3] = 0;
					}
					else if(CAN_BUFFER_RX[2] == 0x13)
					{
						Set_Temp = Hex_Float(CAN_BUFFER_RX[3],CAN_BUFFER_RX[4],CAN_BUFFER_RX[5],CAN_BUFFER_RX[6]);
						Set_Temp = _IQmpy(Set_Temp,3200);//100>>15
						if(Set_Temp >= 65535)
							Set_Temp = 65535;
						CAN_SET_VALUE[2] = Set_Temp & 0x00FF;
						CAN_SET_VALUE[3] = (Set_Temp>>8) & 0x00FF;
					}

					
					for(i = 0; i < 8; i++)
						CAN_CHANGE_CTRL[i] = CAN_BUFFER_RX[i];
					if((BORAD_NUM == 0 || BORAD_NUM == 1))
					{
						if(CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A)
						{
							RunAllow_Flag = 0;
						}
						else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5)
						{
							RunAllow_Flag = 1;
						}
					}
					if((BORAD_NUM == 2 || BORAD_NUM == 3))
					{
						if(CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0xA5)
						{
							RunAllow_Flag = 0;
						}
						else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55)
						{
							RunAllow_Flag = 1;
						}
					}
					if((BORAD_NUM == 4 || BORAD_NUM == 5))
					{
						if(CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55)
						{
							RunAllow_Flag = 0;
						}
						else if(CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA)
						{
							RunAllow_Flag = 1;
						}
					}

					if((CAN_BUFFER_RX[1] & 0x01) == 0x01 && (BORAD_NUM == 0 || BORAD_NUM == 1))
					{
						if(BORAD_NUM == 0)
						{
							ET_IO_OUT1_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
							Ctrl_Flag.bit.CAN_Ctrl = 0;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
						}
						if(BORAD_NUM == 1)
						{
							ET_IO_OUT2_L;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
							if(BC_SLT_IN == 1)
								Can_Ctrl(0);
						}
			   		}				
			   		if((CAN_BUFFER_RX[1] & 0x01) == 0x00 && (BORAD_NUM == 0 || BORAD_NUM == 1))
					{
						if(BORAD_NUM == 0)
						{
							ET_IO_OUT1_H;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
							if(BC_SLT_IN == 0)
								Can_Ctrl(0);
						}
						if(BORAD_NUM == 1)
						{
							ET_IO_OUT2_H;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
							Ctrl_Flag.bit.CAN_Ctrl = 0;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
						}
			   		}	
			   		if((CAN_BUFFER_RX[1] & 0x02) == 0x02 && (BORAD_NUM == 2 || BORAD_NUM == 3))
					{
						if(BORAD_NUM == 2)
						{
							ET_IO_OUT1_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
							Ctrl_Flag.bit.CAN_Ctrl = 0;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
						}
						if(BORAD_NUM == 3)
						{
							ET_IO_OUT2_L;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
							Can_Ctrl(2);
						}
			   		}				
			   		if((CAN_BUFFER_RX[1] & 0x02) == 0x00 && (BORAD_NUM == 2 || BORAD_NUM == 3))
					{
						if(BORAD_NUM == 2)
						{
							//ET_IO_OUT1_H;
							ET_IO_OUT1_L;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
							Can_Ctrl(2);
						}
						if(BORAD_NUM == 3)
						{
							//ET_IO_OUT2_H;
							ET_IO_OUT2_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
							Ctrl_Flag.bit.CAN_Ctrl = 0;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
						}
			   		}
			   		if((CAN_BUFFER_RX[1] & 0x04) == 0x04 && (BORAD_NUM == 4 || BORAD_NUM == 5))
					{
						if(BORAD_NUM == 4)
						{
							ET_IO_OUT1_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
							Ctrl_Flag.bit.CAN_Ctrl = 0;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
						}
						if(BORAD_NUM == 5)
						{
							ET_IO_OUT2_L;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
							if(BC_SLT_IN == 1)
								Can_Ctrl(4);
						}
			   		}				
			   		if((CAN_BUFFER_RX[1] & 0x04) == 0x00 && (BORAD_NUM == 4 || BORAD_NUM == 5))
					{
						if(BORAD_NUM == 4)
						{
							ET_IO_OUT1_H;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
							if(BC_SLT_IN == 0)
								Can_Ctrl(4);
						}
						if(BORAD_NUM == 5)
						{
							ET_IO_OUT2_H;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
							Ctrl_Flag.bit.CAN_Ctrl = 0;
							//Delay_Flag = 0;
							//while(Delay_Flag == 0);
						}
			   		}


	   			if( ((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0)
		 		 || ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1) )
				{
					//for(i = 0; i < 6; i++)
					//	CAN_BUFFER_RT[i][8]= 1;
	    			Ctrl_Flag.bit.CANERROR_FLAG = 0;   	
	    			CAN_Index = 0;
					Can_Tx11_Data();
					if(SCIB.FLAG.bit.TXBO_FLAG ==1)
					{
						*SCIB.p_Tx_Buffer++ = 0x00EB;
						*SCIB.p_Tx_Buffer++ = 0x0090;
						*SCIB.p_Tx_Buffer++ = 0x0051;

						// 发送的数据字节数加上效验字节
						*SCIB.p_Tx_Buffer++ = ComNum;
						ComNum++;
						if(ComNum > 255)
							ComNum = 0;
						*SCIB.p_Tx_Buffer++ = 0x0005;
							
						for(i = 0; i < 5; i++)
						{
							*SCIB.p_Tx_Buffer++ = (SCI_RX_BUFFER[5+i] & 0x00FF);
						}
						
						Data_Temp = CalCRC16_Byte(SCI_TX_BUFFER, 10);
						
						*SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* 累加求效验�*/
						*SCIB.p_Tx_Buffer = (Data_Temp>>8); 					/* 累加求效验和*/
						SCIB.p_Tx_Buffer = SCI_TX_BUFFER;	
						SCIB.m_Tx_Length = 5+7;

						SCIB.FLAG.bit.TXBO_FLAG = 0;
				   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
						//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
						//while(SCIB.FLAG.bit.TXBO_FLAG == 0)
						//	ScibTxCheck();
					}
				}

			}
			else
			{
				if(SCI_RX_BUFFER[2] == 0)
				{
			   		Sci_DataCombine_Rx(SCI_RX_BUFFER);
					Board_Num = SCI_ReceData[0];
			   		if(Board_Num == 1 && (BORAD_NUM == 0 || BORAD_NUM == 1))
					{
						if(BORAD_NUM == 0)
						{
							ET_IO_OUT1_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						}
						if(BORAD_NUM == 1)
							ET_IO_OUT2_L;
			   		}				
			   		if(Board_Num == 0 && (BORAD_NUM == 0 || BORAD_NUM == 1))
					{
						if(BORAD_NUM == 0)
							ET_IO_OUT1_H;
						if(BORAD_NUM == 1)
						{
							ET_IO_OUT2_H;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						}
			   		}
			   		if(Board_Num == 3 && (BORAD_NUM == 2 || BORAD_NUM == 3))
					{
						if(BORAD_NUM == 2)
						{
							ET_IO_OUT1_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						}
						if(BORAD_NUM == 3)
							ET_IO_OUT2_L;
			   		}				
			   		if(Board_Num == 2 && (BORAD_NUM == 2 || BORAD_NUM == 3))
					{
						if(BORAD_NUM == 2)
							ET_IO_OUT1_H;
						if(BORAD_NUM == 3)
						{
							ET_IO_OUT2_H;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						}
			   		}
			   		if(Board_Num == 5 && (BORAD_NUM == 4 || BORAD_NUM == 5))
					{
						if(BORAD_NUM == 4)
						{
							ET_IO_OUT1_L;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						}
						if(BORAD_NUM == 5)
							ET_IO_OUT2_L;
			   		}				
			   		if(Board_Num == 4 && (BORAD_NUM == 4 || BORAD_NUM == 5))
					{
						if(BORAD_NUM == 4)
							ET_IO_OUT1_H;
						if(BORAD_NUM == 5)
						{
							ET_IO_OUT2_H;
							Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						}
			   		}
	   				DELAY_US(1000);
				}
			
				if( ((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0)
		 		 || ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1) )
				{
					if(Board_Num == BORAD_NUM)
					{
				    	Cmd_Deal();
					}
					else
					{
						Can_Send();
					}
				}
			}
			SCIB.FLAG.bit.RX_FLAG = 0;		    // 接收命令处理完毕应答
		}

	    if(CANB.FLAG.bit.RX_FLAG == 1)
		{
			if(RX_BUFFER[2] == 0)
			{
		   		CAN_DataCombine_Rx(RX_BUFFER);				
				Board_Num = ReceData[0];
		   		if(Board_Num == 1 && (BORAD_NUM == 0 || BORAD_NUM == 1))
				{
					if(BORAD_NUM == 0)
					{
						ET_IO_OUT1_L;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
					}
					if(BORAD_NUM == 1)
						ET_IO_OUT2_L;
		   		}				
		   		if(Board_Num == 0 && (BORAD_NUM == 0 || BORAD_NUM == 1))
				{
					if(BORAD_NUM == 0)
						ET_IO_OUT1_H;
					if(BORAD_NUM == 1)
					{
						ET_IO_OUT2_H;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
					}
		   		}
		   		if(Board_Num == 3 && (BORAD_NUM == 2 || BORAD_NUM == 3))
				{
					if(BORAD_NUM == 2)
					{
						ET_IO_OUT1_L;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
					}
					if(BORAD_NUM == 3)
						ET_IO_OUT2_L;
		   		}				
		   		if(Board_Num == 2 && (BORAD_NUM == 2 || BORAD_NUM == 3))
				{
					if(BORAD_NUM == 2)
						ET_IO_OUT1_H;
					if(BORAD_NUM == 3)
					{
						ET_IO_OUT2_H;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
					}
		   		}
		   		if(Board_Num == 5 && (BORAD_NUM == 4 || BORAD_NUM == 5))
				{
					if(BORAD_NUM == 4)
					{
						ET_IO_OUT1_L;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
					}
					if(BORAD_NUM == 5)
						ET_IO_OUT2_L;
		   		}				
		   		if(Board_Num == 4 && (BORAD_NUM == 4 || BORAD_NUM == 5))
				{
					if(BORAD_NUM == 4)
						ET_IO_OUT1_H;
					if(BORAD_NUM == 5)
					{
						ET_IO_OUT2_H;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
					}
		   		}
   				DELAY_US(1000);

			}
			if(Board_Num == BORAD_NUM)
				Can_Deal();
			CANB.FLAG.bit.RX_FLAG = 0;		    // 接收命令处理完毕应答
		}



	    if(CANB2.FLAG.bit.RX_FLAG == 1)
		{
			if(SCI_TX_BUFFER[2] == 0x70)
			{
		   		CAN2_DataCombine_Rx(SCI_TX_BUFFER);				
				Board2_Num = ReceData[0];
			}
			if(Board2_Num == BORAD_NUM)
				Can_Receive();
			CANB2.FLAG.bit.RX_FLAG = 0;		    // 接收命令处理完毕应答
		}
		
		if(CANB.FLAG.bit.TIMETX_FLAG == 1)
		{
			Can_Tx4_Data();
			Can_Tx13_Data();
				if( ((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0)
		 		 || ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1) )
				{
					if(SCIB.FLAG.bit.TXBO_FLAG ==1 && SCIB.FLAG.bit.RX_FLAG ==0 
					&& CANB2.FLAG.bit.TXBO_FLAG ==1 && CANB2.FLAG.bit.RX_FLAG ==0 
					&& CANB.FLAG.bit.TXBO_FLAG ==1 && CANB.FLAG.bit.RX_FLAG ==0)
					{
						*SCIB.p_Tx_Buffer++ = 0x00EB;
						*SCIB.p_Tx_Buffer++ = 0x0090;
						*SCIB.p_Tx_Buffer++ = 0x0052;

						// 发送的数据字节数加上效验字节
						*SCIB.p_Tx_Buffer++ = ComNum;
						ComNum++;
						if(ComNum > 255)
							ComNum = 0;
						*SCIB.p_Tx_Buffer++ = 0x0036;//48+6						

						Data_Temp = 0;
						if(CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x01) == 0x00)
							Data_Temp = 0x0001;
						if(CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x00)
							Data_Temp += 0x0002;
						if(CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x01) == 0x00)
							Data_Temp += 0x0004;
						if(CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x00)
							Data_Temp += 0x0008;
						if(CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x01) == 0x00)
							Data_Temp += 0x0010;
						if(CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x00)
							Data_Temp += 0x0020;
						*SCIB.p_Tx_Buffer++ = Data_Temp & 0x00FF;
						*SCIB.p_Tx_Buffer++ = (Data_Temp>>8);//2

						CAN_CONTROL = 0x07;
						//先选择从板,如果主板无故障，且从板不在运行中，则选择主板运行
						if((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01)
							CAN_CONTROL &=	0xFE;
						if((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01)
							CAN_CONTROL &=	0xFD;
						if((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01)
							CAN_CONTROL &=	0xFB;

						CAN_BUFFER_TX[0] = CAN_SET_VALUE[0];
						CAN_BUFFER_TX[1] = CAN_SET_VALUE[1];
						CAN_BUFFER_TX[4] = CAN_SET_VALUE[2];
						CAN_BUFFER_TX[5] = CAN_SET_VALUE[3];
						CAN_BUFFER_TX[8] = CAN_SET_VALUE[4];
						CAN_BUFFER_TX[9] = CAN_SET_VALUE[5];

						if((CAN_CONTROL & 0x01) == 0x00)
						{
							CAN_BUFFER_TX[2] = CAN_BUFFER_RT[0][0];
							CAN_BUFFER_TX[3] = CAN_BUFFER_RT[0][1];
							CAN_BUFFER_TX[6+6] = CAN_BUFFER_RT[0][4];
							CAN_BUFFER_TX[7+6] = CAN_BUFFER_RT[0][5];
							CAN_BUFFER_TX[12+6] = CAN_BUFFER_RT[0][6];
							CAN_BUFFER_TX[13+6] = CAN_BUFFER_RT[0][7];
							CAN_BUFFER_TX[18+6] = CAN_BUFFER_RT[0][9];
							CAN_BUFFER_TX[19+6] = CAN_BUFFER_RT[0][10];
							CAN_BUFFER_TX[24+6] = CAN_BUFFER_RT[0][11];
							CAN_BUFFER_TX[25+6] = CAN_BUFFER_RT[0][12];
							CAN_BUFFER_TX[26+6] = CAN_BUFFER_RT[0][13];
							CAN_BUFFER_TX[27+6] = CAN_BUFFER_RT[0][14];
							CAN_BUFFER_TX[28+6] = CAN_BUFFER_RT[0][15];
							CAN_BUFFER_TX[29+6] = CAN_BUFFER_RT[0][16];

							CAN_BUFFER_TX[42+6] = 0;
							CAN_BUFFER_TX[43+6] = 0;
							CAN_BUFFER_TX[44+6] = 0;
							if((CAN_BUFFER_RT[0][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] = 0x0001;
							if((CAN_BUFFER_RT[0][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0008;
							if((CAN_BUFFER_RT[0][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0040;
							if((CAN_BUFFER_RT[0][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0002;
							if((CAN_BUFFER_RT[0][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0010;
							if((CAN_BUFFER_RT[0][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0040;
							if((CAN_BUFFER_RT[0][3] & 0x08) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0001;
							if((CAN_BUFFER_RT[0][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0004;
						}
						else
						{
							CAN_BUFFER_TX[2] = CAN_BUFFER_RT[1][0];
							CAN_BUFFER_TX[3] = CAN_BUFFER_RT[1][1];
							CAN_BUFFER_TX[6+6] = CAN_BUFFER_RT[1][4];
							CAN_BUFFER_TX[7+6] = CAN_BUFFER_RT[1][5];
							CAN_BUFFER_TX[12+6] = CAN_BUFFER_RT[1][6];
							CAN_BUFFER_TX[13+6] = CAN_BUFFER_RT[1][7];
							CAN_BUFFER_TX[18+6] = CAN_BUFFER_RT[1][9];
							CAN_BUFFER_TX[19+6] = CAN_BUFFER_RT[1][10];
							CAN_BUFFER_TX[24+6] = CAN_BUFFER_RT[1][11];
							CAN_BUFFER_TX[25+6] = CAN_BUFFER_RT[1][12];
							CAN_BUFFER_TX[26+6] = CAN_BUFFER_RT[1][13];
							CAN_BUFFER_TX[27+6] = CAN_BUFFER_RT[1][14];
							CAN_BUFFER_TX[28+6] = CAN_BUFFER_RT[1][15];
							CAN_BUFFER_TX[29+6] = CAN_BUFFER_RT[1][16];

							CAN_BUFFER_TX[42+6] = 0;
							CAN_BUFFER_TX[43+6] = 0;
							CAN_BUFFER_TX[44+6] = 0;
							if((CAN_BUFFER_RT[1][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] = 0x0001;
							if((CAN_BUFFER_RT[1][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0008;
							if((CAN_BUFFER_RT[1][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0040;
							if((CAN_BUFFER_RT[1][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0002;
							if((CAN_BUFFER_RT[1][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0010;
							if((CAN_BUFFER_RT[1][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0040;
							if((CAN_BUFFER_RT[1][3] & 0x08) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0001;
							if((CAN_BUFFER_RT[1][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0004;
						}
						if((CAN_CONTROL & 0x04) == 0x00)
						{
							CAN_BUFFER_TX[6] = CAN_BUFFER_RT[4][0];
							CAN_BUFFER_TX[7] = CAN_BUFFER_RT[4][1];
							CAN_BUFFER_TX[8+6] = CAN_BUFFER_RT[4][4];
							CAN_BUFFER_TX[9+6] = CAN_BUFFER_RT[4][5];
							CAN_BUFFER_TX[14+6] = CAN_BUFFER_RT[4][6];
							CAN_BUFFER_TX[15+6] = CAN_BUFFER_RT[4][7];
							CAN_BUFFER_TX[20+6] = CAN_BUFFER_RT[4][9];
							CAN_BUFFER_TX[21+6] = CAN_BUFFER_RT[4][10];
							CAN_BUFFER_TX[30+6] = CAN_BUFFER_RT[4][11];
							CAN_BUFFER_TX[31+6] = CAN_BUFFER_RT[4][12];
							CAN_BUFFER_TX[32+6] = CAN_BUFFER_RT[4][13];
							CAN_BUFFER_TX[33+6] = CAN_BUFFER_RT[4][14];
							CAN_BUFFER_TX[34+6] = CAN_BUFFER_RT[4][15];
							CAN_BUFFER_TX[35+6] = CAN_BUFFER_RT[4][16];

							if((CAN_BUFFER_RT[4][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0002;
							if((CAN_BUFFER_RT[4][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0010;
							if((CAN_BUFFER_RT[4][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0080;
							if((CAN_BUFFER_RT[4][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0004;
							if((CAN_BUFFER_RT[4][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0020;
							if((CAN_BUFFER_RT[4][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0080;
							if((CAN_BUFFER_RT[4][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0008;
						}
						else
						{
							CAN_BUFFER_TX[6] = CAN_BUFFER_RT[5][0];
							CAN_BUFFER_TX[7] = CAN_BUFFER_RT[5][1];
							CAN_BUFFER_TX[8+6] = CAN_BUFFER_RT[5][4];
							CAN_BUFFER_TX[9+6] = CAN_BUFFER_RT[5][5];
							CAN_BUFFER_TX[14+6] = CAN_BUFFER_RT[5][6];
							CAN_BUFFER_TX[15+6] = CAN_BUFFER_RT[5][7];
							CAN_BUFFER_TX[20+6] = CAN_BUFFER_RT[5][9];
							CAN_BUFFER_TX[21+6] = CAN_BUFFER_RT[5][10];
							CAN_BUFFER_TX[30+6] = CAN_BUFFER_RT[5][11];
							CAN_BUFFER_TX[31+6] = CAN_BUFFER_RT[5][12];
							CAN_BUFFER_TX[32+6] = CAN_BUFFER_RT[5][13];
							CAN_BUFFER_TX[33+6] = CAN_BUFFER_RT[5][14];
							CAN_BUFFER_TX[34+6] = CAN_BUFFER_RT[5][15];
							CAN_BUFFER_TX[35+6] = CAN_BUFFER_RT[5][16];

							if((CAN_BUFFER_RT[5][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0002;
							if((CAN_BUFFER_RT[5][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0010;
							if((CAN_BUFFER_RT[5][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0080;
							if((CAN_BUFFER_RT[5][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0004;
							if((CAN_BUFFER_RT[5][3] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0020;
							if((CAN_BUFFER_RT[5][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0080;
							if((CAN_BUFFER_RT[5][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0008;	
						}
						if((CAN_CONTROL & 0x02) == 0x00)
						{
							CAN_BUFFER_TX[10] = CAN_BUFFER_RT[2][0];
							CAN_BUFFER_TX[11] = CAN_BUFFER_RT[2][1];
							CAN_BUFFER_TX[10+6] = CAN_BUFFER_RT[2][4];
							CAN_BUFFER_TX[11+6] = CAN_BUFFER_RT[2][5];
							CAN_BUFFER_TX[16+6] = CAN_BUFFER_RT[2][6];
							CAN_BUFFER_TX[17+6] = CAN_BUFFER_RT[2][7];
							CAN_BUFFER_TX[22+6] = CAN_BUFFER_RT[2][9];
							CAN_BUFFER_TX[23+6] = CAN_BUFFER_RT[2][10];
							CAN_BUFFER_TX[36+6] = CAN_BUFFER_RT[2][11];
							CAN_BUFFER_TX[37+6] = CAN_BUFFER_RT[2][12];
							CAN_BUFFER_TX[38+6] = CAN_BUFFER_RT[2][13];
							CAN_BUFFER_TX[39+6] = CAN_BUFFER_RT[2][14];
							CAN_BUFFER_TX[40+6] = CAN_BUFFER_RT[2][15];
							CAN_BUFFER_TX[41+6] = CAN_BUFFER_RT[2][16];

							if((CAN_BUFFER_RT[2][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0004;
							if((CAN_BUFFER_RT[2][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0020;
							if((CAN_BUFFER_RT[2][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0001;
							if((CAN_BUFFER_RT[2][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0008;
							if((CAN_BUFFER_RT[2][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0002;
							if((CAN_BUFFER_RT[2][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0010;

						}
						else
						{
							CAN_BUFFER_TX[10] = CAN_BUFFER_RT[3][0];
							CAN_BUFFER_TX[11] = CAN_BUFFER_RT[3][1];
							CAN_BUFFER_TX[10+6] = CAN_BUFFER_RT[3][4];
							CAN_BUFFER_TX[11+6] = CAN_BUFFER_RT[3][5];
							CAN_BUFFER_TX[16+6] = CAN_BUFFER_RT[3][6];
							CAN_BUFFER_TX[17+6] = CAN_BUFFER_RT[3][7];
							CAN_BUFFER_TX[22+6] = CAN_BUFFER_RT[3][9];
							CAN_BUFFER_TX[23+6] = CAN_BUFFER_RT[3][10];
							CAN_BUFFER_TX[36+6] = CAN_BUFFER_RT[3][11];
							CAN_BUFFER_TX[37+6] = CAN_BUFFER_RT[3][12];
							CAN_BUFFER_TX[38+6] = CAN_BUFFER_RT[3][13];
							CAN_BUFFER_TX[39+6] = CAN_BUFFER_RT[3][14];
							CAN_BUFFER_TX[40+6] = CAN_BUFFER_RT[3][15];
							CAN_BUFFER_TX[41+6] = CAN_BUFFER_RT[3][16];

							if((CAN_BUFFER_RT[3][2] & 0x20) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0004;
							if((CAN_BUFFER_RT[3][3] & 0x01) == 0x00) 
								CAN_BUFFER_TX[42+6] += 0x0020;
							if((CAN_BUFFER_RT[3][2] & 0x04) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0001;
							if((CAN_BUFFER_RT[3][2] & 0x10) == 0x00) 
								CAN_BUFFER_TX[43+6] += 0x0008;
							if((CAN_BUFFER_RT[3][3] & 0x04) == 0x00) 
								CAN_BUFFER_TX[44+6] += 0x0002;
							if((CAN_BUFFER_RT[3][3] & 0x20) == 0x20) 
								CAN_BUFFER_TX[44+6] += 0x0010;
						}
							CAN_BUFFER_TX[45+6] = 0;
						for(i = 0; i < 46+6; i++)
							*SCIB.p_Tx_Buffer++ = CAN_BUFFER_TX[i];
						Data_Temp = CalCRC16_Byte(SCI_TX_BUFFER, 53+6);
						
						*SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); 					/* 累加求效验和*/
						*SCIB.p_Tx_Buffer = (Data_Temp>>8); 					/* 累加求效验和*/
						SCIB.p_Tx_Buffer = SCI_TX_BUFFER;	
						SCIB.m_Tx_Length = 48+6+7;

						SCIB.FLAG.bit.TXBO_FLAG = 0;
				   		//ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
						//ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
					}
				}

			CANB.FLAG.bit.TIMETX_FLAG = 0;
		}
		if(Ctrl_Flag.bit.STOP_PWM_Flag == 1 && CANB.FLAG.bit.DIAGTX_FLAG == 0)
		{
			Can_Tx4_Data();
			Can_Tx13_Data();
			CANB.FLAG.bit.DIAGTX_FLAG = 1;
		}
		if(Ctrl_Flag.bit.STOP_PWM_Flag == 0 && CANB.FLAG.bit.DIAGTX_FLAG == 1)
		{
			Can_Tx4_Data();
			Can_Tx13_Data();
			CANB.FLAG.bit.DIAGTX_FLAG = 0;
		}

		Ctrl_Change();
		if(CAN_BUFFER_CTRL_Num > 0)
		//if(Sys_Flag.bit.CAN_FLAG == 1)
		{
			Can_Tx4_Data();
			Can_Tx13_Data();
    		Ctrl_Flag.bit.CANERROR_FLAG = 0;   	
    		CAN_Index = 0;
			CAN_BUFFER_CTRL[0] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][0];
			CAN_BUFFER_CTRL[1] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][1];
			CAN_BUFFER_CTRL[2] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][2];
			CAN_BUFFER_CTRL[3] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][3];
			CAN_BUFFER_CTRL[4] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][4];
			CAN_BUFFER_CTRL[5] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][5];
			CAN_BUFFER_CTRL[6] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][6];
			CAN_BUFFER_CTRL[7] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][7];
			CAN_BUFFER_CTRL_index++;
			if(CAN_BUFFER_CTRL_index > 5)
				CAN_BUFFER_CTRL_index = 0;
			CAN_BUFFER_CTRL_Num--;

			//通过CAN进行控制
			if(CAN_BUFFER_CTRL[0] == 0xA1)
			{
				for(i = 0; i < 8; i++)
					CAN_CHANGE_CTRL[i] = CAN_BUFFER_CTRL[i];
				if((BORAD_NUM == 0 || BORAD_NUM == 1))
				{
					if(CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x5A)
					{
						RunAllow_Flag = 0;
					}
					else if(CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xA5)
					{
						RunAllow_Flag = 1;
					}
				}
				if((BORAD_NUM == 2 || BORAD_NUM == 3))
				{
					if(CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0xA5)
					{
						RunAllow_Flag = 0;
					}
					else if(CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0x55)
					{
						RunAllow_Flag = 1;
					}
				}
				if((BORAD_NUM == 4 || BORAD_NUM == 5))
				{
					if(CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x55)
					{
						RunAllow_Flag = 0;
					}
					else if(CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xAA)
					{
						RunAllow_Flag = 1;
					}
				}


				CAN_CONTROL = CAN_BUFFER_CTRL[1];
				if((CAN_BUFFER_CTRL[1] & 0x01) == 0x01 && (BORAD_NUM == 0 || BORAD_NUM == 1))
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
						//Delay_Flag = 0;
						//while(Delay_Flag == 0);
						if(BC_SLT_IN == 1)
							Can_Ctrl(1);
					}
		   		}				
		   		if((CAN_BUFFER_CTRL[1] & 0x01) == 0x00 && (BORAD_NUM == 0 || BORAD_NUM == 1))
				{
					if(BORAD_NUM == 0)
					{
						ET_IO_OUT1_H;
						//Delay_Flag = 0;
						//while(Delay_Flag == 0);
						if(BC_SLT_IN == 0)
							Can_Ctrl(1);
					}
					if(BORAD_NUM == 1)
					{
						ET_IO_OUT2_H;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						Ctrl_Flag.bit.CAN_Ctrl = 0;
					}
		   		}	
		   		if((CAN_BUFFER_CTRL[1] & 0x02) == 0x02 && (BORAD_NUM == 2 || BORAD_NUM == 3))
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
						//Delay_Flag = 0;
						//while(Delay_Flag == 0);
						Can_Ctrl(3);
					}
		   		}				
		   		if((CAN_BUFFER_CTRL[1] & 0x02) == 0x00 && (BORAD_NUM == 2 || BORAD_NUM == 3))
				{
					if(BORAD_NUM == 2)
					{
						ET_IO_OUT1_L;
						//ET_IO_OUT1_H;
						//Delay_Flag = 0;
						//while(Delay_Flag == 0);
						Can_Ctrl(3);
					}
					if(BORAD_NUM == 3)
					{
						ET_IO_OUT2_L;
						//ET_IO_OUT2_H;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						Ctrl_Flag.bit.CAN_Ctrl = 0;
					}
		   		}
		   		if((CAN_BUFFER_CTRL[1] & 0x04) == 0x04 && (BORAD_NUM == 4 || BORAD_NUM == 5))
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
						//Delay_Flag = 0;
						//while(Delay_Flag == 0);
						if(BC_SLT_IN == 1)
							Can_Ctrl(5);
					}
		   		}				
		   		if((CAN_BUFFER_CTRL[1] & 0x04) == 0x00 && (BORAD_NUM == 4 || BORAD_NUM == 5))
				{
					if(BORAD_NUM == 4)
					{
						ET_IO_OUT1_H;
						//Delay_Flag = 0;
						//while(Delay_Flag == 0);
						if(BC_SLT_IN == 0)
							Can_Ctrl(5);
					}
					if(BORAD_NUM == 5)
					{
						ET_IO_OUT2_H;
						Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
						Ctrl_Flag.bit.CAN_Ctrl = 0;
					}
		   		}	
			}
	   		//Sys_Flag.bit.CAN_FLAG = 0;
		}
		
	}
}


