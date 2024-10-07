/* 
//================================================================================
//
// 文件名:	 App_Main.h
//
// 功能描述: 主程序变量即所有全局变量(结构体)定义。
//			 定义完后须在App_UserDef.h 中申明为外部变量,以便所有文件均可访问。
//
// 编写者:   刘刃
// 编写日期: 2021-05-10
//
//=================================================================================
*/
#ifndef APP_MAIN_H
#define APP_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


//=====================================================================================
// 各变量的存储区域分配制定
//=====================================================================================
//----------------------------------------------------------------------------

#pragma DATA_SECTION(m_PT_V,".dataBuffer1");
#pragma DATA_SECTION(m_PT_T,".dataBuffer1");
#pragma DATA_SECTION(g_pusCrc16,".dataBuffer1");

volatile Uint16 WriteEEProm_Flag = 0;
//volatile Uint16 Delay_Flag = 0;
//volatile Uint16 Delay_Index = 0;
volatile Uint16 RunAllow_Flag = 0;

Uint16 g_pusCrc16[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

//======================================================================================================
/*  SCI变量定义		*/
//======================================================================================================
SCIB_REGS  SCIB = SCIB_REGS_DEFAULTS;
Uint16	SCI_RX_BUFFER[RX_BUFFER_LENGTH] = {0};	// 串口接收缓冲区
Uint16	SCI_RX_ALLBUFFER[RX_ALLBUFFER_LENGTH] = {0};	// 串口接收缓冲区
Uint16	SCI_TX_BUFFER[TX_BUFFER_LENGTH]= {0};	// 串口发送缓冲区
_iq	SCI_ReceData[30];		//串口接收的数据
_iq	SCI_SendData[30];		//串口发送的数据
volatile Uint32 Time_Cal = 0;									//控制算法时间

//======================================================================================================
/*  CAN变量定义		*/
//======================================================================================================
	//全局变量或结构体的定义

	Uint16	EEPROM_BUFFER[RX_BUFFER_LENGTH] = {0};	// EEPROM数据缓冲区
	volatile	Uint16		*p_EEProm_Buffer = EEPROM_BUFFER;
	
	Uint16	RX_BUFFER[RX_BUFFER_LENGTH] = {0};	// 串口接收缓冲区
	Uint16	TX_BUFFER[TX_BUFFER_LENGTH]= {0};	// 串口发送缓冲区
	_iq	ReceData[30];		//串口接收的数据
	_iq	SendData[30];		//串口发送的数据

    volatile struct ECAN_REGS       ECanaShadow = {0};
	CAN_REGS  CANB = CAN_REGS_DEFAULTS; 
	CAN_REGS  CANB2 = CAN2_REGS_DEFAULTS; 
	volatile 	Uint16  CanStop_Time = 13333;//150us*13333=2s，can超时时间2s

	Uint16	CAN_BUFFER_RX[8]= {0};	//CAN缓冲区
	Uint16	CAN_BUFFER_RT[6][17]= {0};	//CAN缓冲区
	Uint16	CAN_BUFFER_TX[52]= {0};	//CAN缓冲区

	Uint16	CAN_BUFFER_CTRL_N[6][8]= {0};	//CAN缓冲区
	Uint16	CAN_BUFFER_CTRL_N_index = 0;
	Uint16	CAN_BUFFER_CTRL_index = 0;
	Uint16	CAN_BUFFER_CTRL_Num = 0;
	Uint16	CAN_BUFFER_CTRL[8]= {0};	//CAN缓冲区
	Uint16	CAN_CHANGE_CTRL[8]= {0};	//CAN缓冲区
	Uint16	CAN_SET_VALUE[8]= {0};	//CAN缓冲区
	Uint16	CAN_CONTROL = 0;	//控制状态
	
	volatile 	Uint16  CAN_Index = 0;
	volatile 	_iq    	Vel_Speed = _IQ(0);			//高速模式转速
	volatile 	Uint16	Choose_Mche = 0;			//选择电机
	volatile 	Uint16	Choose_Mode = 0;			//选择模式

	volatile 	Uint16	Board_Num = 0;			//控制板

	volatile 	Uint16	Board2_Num = 0;			//控制板
	// CAN变量定义结束
//======================================================================================================
//======================================================================================================
/* RDC相关的变量定义	*/
//======================================================================================================
//全局变量或结构体的定义
	_iq30 *pArcSin = (_iq30 *)0x00318000;		// 指向反正弦函数表，Q30
	RDC_DRIVER rdc1 = RDC_READ_DEFAULT;
	RDC_DRIVER rdc2 = RDC_READ_DEFAULT;
// RDC相关的变量定义结束
//======================================================================================================


//======================================================================================================
/*  PWM相关变量定义		*/
//======================================================================================================
//定义PWMA和PWMB驱动器
	PWMGEN	pwma = PWMAGEN_DEFAULTS;		// 用于逆变器
	volatile Uint16		Control_Period = CARRY_A_P;	// Q16 控制周期计数值
	volatile Uint16		Deadtime = PWM_DeadTime;	// Q16 死区计数值
	volatile _iq		Velo_Duty = _IQ(1000);				// QG 转速载波系数
	volatile _iq		Velo_Duty0 = _IQ(1000);				// QG 转速载波系数
 	volatile _iq		K_Velo_Cal = K_VELO_CALC;	// 速度计算参数

// PWM相关变量定义结束
//======================================================================================================

//======================================================================================================
/*  坐标变换变量定义		*/
//======================================================================================================
//全局变量或结构体的定义
	Ref_ABCtoDQ abctodq = Ref_ABCtoDQ_DEFAULTS;			// ABC->DQ
	Ref_DQtoABC dqtoabc = Ref_DQtoABC_DEFAULTS;			// DQ->ABC
	Ref_THREE2TWO phase_trans = Ref_THREE2TWO_DEFAULTS;	// ABC->a,b
//  坐标变换变量定义结束
//======================================================================================================


//======================================================================================================
/*  PI变量定义		*/
//======================================================================================================
//全局变量或结构体的定义
	PIREG pi_Pos 	= 	PIREG_VELO_DEFAULTS;			// 位置环的PI
	PIREG pi_velo 	= 	PIREG_VELO_DEFAULTS;			// 速度环的PI
	PIREG pi_Id_Kc 	=  	PIREG_ID_DEFAULTS;				// Id电压PI
	PIREG pi_Iq_Kc 	=  	PIREG_IQ_DEFAULTS;				// Iq电压PI
	PIREG pi_usLimit 	=  	PIREG_IQ_DEFAULTS;
	volatile _iq		Idq_Kc = 0;
	volatile _iq		Radq = 0;
// PI变量定义结束
//======================================================================================================

//======================================================================================================
/*  主要相关全局变量的定义		*/
//======================================================================================================
	volatile Uint16	 	Run_Led_Index = 0;				// 运行灯运行的控制周期的指数

	_iq m_PT_V[31]={_IQ(0.564516129),_IQ(0.600483437),_IQ(0.636107418),_IQ(0.671392966),_IQ(0.706344880),
					_IQ(0.740967870),_IQ(0.775266557),_IQ(0.809245477),_IQ(0.842909081),_IQ(0.876261737),
					_IQ(0.909307734),_IQ(0.942051281),_IQ(0.974496513),_IQ(1.006647488),_IQ(1.038508190),
					_IQ(1.070082532),_IQ(1.101374359),_IQ(1.132387446),_IQ(1.163125499),_IQ(1.193592162),
					_IQ(1.223791012),_IQ(1.253725567),_IQ(1.283399279),_IQ(1.312815544),_IQ(1.341977699),
					_IQ(1.370889020),_IQ(1.399552731),_IQ(1.427971999),_IQ(1.456149937),_IQ(1.484089608),
					_IQ(1.511794019)};

	_iq m_PT_T[31]={_IQ(0),
					_IQ(5),_IQ(10),_IQ(15),_IQ(20),_IQ(25),_IQ(30),_IQ(35),_IQ(40),_IQ(45),_IQ(50),
					_IQ(55),_IQ(60),_IQ(65),_IQ(70),_IQ(75),_IQ(80),_IQ(85),_IQ(90),_IQ(95),_IQ(100),
					_IQ(105),_IQ(110),_IQ(115),_IQ(120),_IQ(125),_IQ(130),_IQ(135),_IQ(140),_IQ(145),_IQ(150)};

	volatile _iq	MotorTM = 0;
	volatile _iq	IgbtTM = 0;
	volatile _iq	Velo_Max = Velo_MAX;
	volatile   _iq   id_Max = IsMAX;
	volatile   _iq   idc_Max = IsMAX;
	volatile   _iq   idSet_Max = IsMAX;
	volatile   	_iq    	UdcLimit_Set = 0;		// QG 直流侧电压的设定值
	volatile   	_iq    	UdcLimit1_Set = 0;		// QG 直流侧电压的设定值

	volatile   	_iq    	IsLimit_Set = 0;
	volatile 	_iq 	Us_M = _IQ(1.1);
	volatile 	_iq    	I_Add = _IQ(0.000004);
	volatile 	_iq    	Velo_Start = _IQ(0.00157);//10r/min
	volatile	_iq		UsSet = _IQ(0);

	volatile 	_iq    	Is_DownAdd = _IQ(0.000004);
	volatile 	_iq    	Is_DownPerAdd = _IQ(0.000004);

 	volatile 	_iq    	Udc_Setg = _IQ(1);				// QG 直流侧电压的测量值
	volatile 	_iq    	IstoTorque = _IQ(0.78);
	volatile 	_iq    	TorquetoIs = _IQ(1.2820512820512820512820512820513);
	volatile 	_iq    	TorqueAngle = _IQ(0.52359877559829887307710723054658);//  pi/6
	volatile 	_iq    	TorqueAngleA = _IQ(0);//  pi/6
	volatile 	_iq    	Is_Add = _IQ(0.000004);
	volatile 	_iq    	Is_PerAdd = _IQ(0.000004);
	volatile 	_iq    	Velo_Add = _IQ(0.00001);
	volatile 	_iq    	Velo_PerAdd = _IQ(0.00001);
 	volatile 	_iq		Per_Ctrl = _IQ(0.0001);	// 控制周期

/*滤波截止频率变量定义*/
	volatile 	_iq    	Temp_Filter = _IQ(1);			//
	volatile 	_iq    	IsSet_Filter = _IQ(1);
	volatile   _iq   id_Filter = _IQ(20);
	volatile   _iq   Velo_Filter = _IQ(20);
	volatile   _iq   Udc_Filter = _IQ(5);
	 	// 用于滤波程序的量
	volatile 	_iq		Id0 = 0;					// QG 上次的Id标幺值
	volatile 	_iq     Iq0 = 0;					// QG 上次的Iq标幺值
	volatile 	_iq		Velo_Elec0 = 0;				// QG 上次计算的电速度的标幺值

	volatile 	_iq   Usa_Lock = 0;
	volatile 	_iq   Usb_Lock = 0;
 	volatile 	_iq   Usc_Lock = 0;
	volatile 	_iq	  Udc_Lock = 0;
 	volatile 	_iq	  Angle_ElecOpen = 0;				// QG 转子位置电角的标幺值
	volatile 	_iq   Um_OpenLoop = 0;
	volatile 	_iq	  Freq_OpenLoop = 0;
	volatile 	_iq	  Udc_OpenLoop = 0;

	volatile  	Uint32	VeloProtect1 = 0;		    	// QG 保护用Iq标子
	volatile    Uint32	IdProtect1 = 0;			    	// QG 保护用Id标子
	volatile    Uint32	UdcOverProtect1 = 0;				// QG 保护用Id标子
	volatile    Uint32	IdcOverProtect1 = 0;				// QG 保护用Id标子
	volatile    Uint16	IdProtect = 0;			    	// QG 保护用Id标子
	volatile  	Uint16	VeloProtect = 0;		    	// QG 保护用Iq标子
	volatile    Uint16	IdProtectNum = IsMAXNUM;		// QG 保护用Id标子
	volatile  	Uint16	VeloProtectNum = VELO_MAXNUM;	// QG 保护用Iq标子
	volatile    Uint16	UdcLowProtect = 0;				// QG 保护用Id标子
	volatile    Uint32	UdcLowEnable = 0;				// QG 保护用Id标子
	volatile    Uint16	UdcOverProtect = 0;				// QG 保护用Id标子
	volatile    Uint16	IdcOverProtect = 0;				// QG 保护用Id标子
	volatile    Uint16	Temp1OverProtect = 0;			// QG 保护用Id标子
	volatile    Uint16	Temp2OverProtect = 0;			// QG 保护用Id标子
	volatile    Uint32	Temp1OverProtect1 = 0;			// QG 保护用Id标子
	volatile    Uint32	Temp2OverProtect1 = 0;			// QG 保护用Id标子
	volatile    Uint32	IGBTProtect1 = 0;			// QG 保护用Id标子
	volatile    Uint16	RdcProtect = 0;				// QG 保护用Id标子
	volatile    Uint32	RdcProtect1 = 0;				// QG 保护用Id标子
	volatile    Uint16	Rdc2Protect = 0;				// QG 保护用Id标子
	volatile    Uint32	Rdc2Protect1 = 0;				// QG 保护用Id标子

	volatile    Uint32	ProtectNum = PROTECT_NUM;		// QG 故障恢复时间

//------------------------------------------------------
// 频率、速度以及角度
	volatile Uint16		Angle_Q16 = 0;					// Q16 旋变测取的当前位置角HEX
	volatile Uint16		Angle0_Q16 = 0;					// Q16 旋变测取的上一次位置角HEX
	volatile Uint16	    Angle_Init_Digital = INIT_ANGLE;	// Q0 用于旋变测量的转子初位置角(HEX)
	volatile Uint16	    Angle_Init_Fa = 0;	// Q0 用于旋变测量的转子初位置角(HEX)
	volatile Uint16		Angle_Fa_Q16 = 0;					// Q16 旋变测取的当前位置角HEX

	volatile _iq		Angle_Elec = 0;					// QG 转子位置电角的标幺值
	volatile _iq        Com_angle_I = 0;                //计算dq轴电流时的补偿角
	volatile _iq        Com_angle_U = 0;               //计算dq轴电压时的补偿角

	volatile _iq		Velo_Elec = 0;					// QG 检测得到的转子速度的实际值
	volatile _iq		Velo_Elec_Cal = 0;				// QG 检测得到的转子速度的计算值
	volatile    _iq		Velo_Elec_abs = 0;				// QG 电速度的标幺值
	volatile _iq		Velo_Set=0;                  	 // QG 给定的转子速度的角频率表示
	volatile 	_iq    	Veloover_Set = _IQ(0);

	volatile _iq setSpeed = _IQ(0); /* 李盛 20241007新增 */

	volatile _iq		Pos_Set=0;                  	 // QG 给定的位置
	
	volatile 	long	Velo_Calc_Num = 10;     //速度平均次数
	volatile 	Uint16	Velo_Avg_Index = 0;     //速度平均指数
	volatile	_iq		Velotmp = _IQ(0);
	volatile	Uint16	Velo_Calc_flag = 0;
//----------------------------------------------------------
// 用于存储直接ADC采样的到数字量,需要进行校正

 	volatile 	Uint16	ADc_Isa = 0;
 	volatile 	Uint16	ADc_Isb = 0;
 	volatile 	Uint16	ADc_Isc = 0;
 	volatile 	Uint16	ADc_Idc = 0;
 	volatile 	Uint16	ADc_Udc = 0;
 	volatile 	Uint16	ADc_Temp0 = 0;
 	volatile 	Uint16	ADc_Temp2 = 0;
 	volatile 	Uint16	ADc_Temp4 = 0;
//------------------------------------------------------------



   	// 电机定子电压、电流的标幺值
   	volatile 	_iq		Isa = 0;					// QG Isa的标幺值
 	volatile 	_iq		Isb = 0;					// QG Isb的标幺值
 	volatile 	_iq		Isc = 0;					// QG Isc的标幺值
	volatile   	_iq		Is = 0;						// QG Id标幺值

 	volatile 	_iq		Usa = 0;					// QG Usa的标幺值
 	volatile 	_iq		Usb = 0;					// QG Usb的标幺值
 	volatile 	_iq		Usc = 0;					// QG Usc的标幺值
	volatile 	_iq		Us = 0;					        // QG Us=mag(ud,uq)的标幺值

 	volatile 	_iq    	Udc_Mche = _IQ(1);			// QG 直流侧电压的测量值,上位机给过来的是直流侧电压给定值的倒数
 	volatile 	_iq    	Udc_Mche_realvalue = _IQ(1); // QG 直流侧电压的测量值,
	volatile 	_iq    	Udc_Mche_1 = _IQ(1);		// QG 直流侧电压的测量值
 	volatile 	_iq    	Udc_Mche0 = 0;				// QG 直流侧电压的测量值
   	volatile 	_iq		Idc = 0;					// QG Idc的标幺值
   	volatile 	_iq		Idc0 = 0;					// QG Idc的标幺值

	volatile   	_iq		Id = 0;						// QG Id标幺值
	volatile 	_iq     Iq = 0;						// QG Iq标幺值
	volatile   	_iq		Id_Set = 0;					// QG d轴电流给定的标幺值
	volatile 	_iq		Iq_Set = 0;					// QG Iq的设定标幺值
	volatile 	_iq    	Isdq_Set = _IQ(0);
	volatile 	_iq    	Isd_Set = _IQ(0);      //上位机发送过来的id
	volatile 	_iq    	Isdq_Set1 = _IQ(0);
	volatile 	_iq    	Isdq_Set2 = _IQ(0);
	volatile	_iq		IdSet = _IQ(0);

	volatile   	_iq		IdZ_Set = 0;			// QG d轴电流给定的标幺值
	volatile 	_iq		IqZ_Set = 0;			// QG Iq的设定标幺值

	volatile   	_iq    	Ud = 0;						// QG 控制得出的Ud
	volatile 	_iq		Uq = 0;						// QG 控制得出的Uq
	volatile 	_iq		Us_Max = 0;					// QG Ud，Uq的限幅 k*0.612Udc (恒功率变换)
	volatile 	_iq		Us_Alfa_Mche = 0;			// QG a轴电压的标幺值
	volatile	_iq		Us_Beta_Mche = 0;			// QG b轴电压的标幺值
	//-------------------------------------------------------------


 	// 电机参数
	volatile    _iq  		Ld = K_LD;			       // 电机d轴电感的标幺值
	volatile    _iq  		Lq = K_LQ;			       // 电机q轴电感的标幺值
	volatile    _iq  		PHI = K_PHI;	           // 电机磁链的标幺值
	volatile    _iq  		Rs = K_RS;			       // 电机电阻的标幺值
	volatile    _iq  		Mp = K_MP;			       // 电机的极对数
	volatile    _iq  		Rp = K_RP;			       // 电机的极对数
	volatile    _iq  		p = K_P;			       // 电机与旋变的极对数比
	// 基值系统
 	volatile   _iq		U_Base = U_BASE;			// 电压基值
 	volatile   _iq		I_Base = I_BASE;			// 电流基值
 	volatile   _iq		V_Base = V_BASE;			// 速度基值


// 主要相关全局变量的定义结束
//======================================================================================================

//======================================================================================================
/*  系统标志变量定义		*/
//======================================================================================================
	volatile union FAULT_FLAG_REG		Fault_Flag = FAULT_FLAG_DEFAULT
	volatile union SYS_FLAG_REG		Sys_Flag = SYS_FLAG_DEFAULT
	volatile union CTRL_FLAG_REG	Ctrl_Flag = CTRL_FLAG_DEFAULT
//  系统标志变量定义结束
//====================================================================================





	volatile 	_iq17 	Off_Isc = _IQ17(0);
	volatile 	_iq17 	K_Isc = _IQ17(1);
	volatile 	_iq17 	Off_Isb = _IQ17(0);
	volatile 	_iq17 	K_Isb = _IQ17(1);
	volatile 	_iq17 	Off_Isa = _IQ17(0);
	volatile 	_iq17 	K_Isa = _IQ17(1);
	volatile 	_iq17 	Off_Udc = _IQ17(0);
	volatile 	_iq17 	K_Udc = _IQ17(1);
	volatile 	_iq17 	Off_Idc = _IQ17(0);
	volatile 	_iq17 	K_Idc = _IQ17(1);

	volatile   	_iq    	PWMA_a = _IQ(0);				// QG a相补偿
	volatile 	_iq		PWMA_b = _IQ(0);					// QG b相补偿
	volatile 	_iq		PWMA_c = _IQ(0);					// QG c相补偿

	volatile	_iq 	Temperature1 = 0;		// 温度信号1
	volatile	_iq 	Temperature3 = 0;		// 温度信号3
	volatile	_iq 	Temperature5 = 0;		// 温度信号4
	volatile	_iq 	Temperature10 = 0;		// 温度藕�1
	volatile	_iq 	Temperature30 = 0;		// 温度信号3
	volatile	_iq 	Temperature50 = 0;		// 温度信号4
	volatile	_iq 	Temperature1_Off = 0;	// 温度信号1
	volatile	_iq 	Temperature3_Off = 0;	// 温度信号3
	volatile	_iq 	Temperature5_Off = 0;	// 温度信号4
	volatile	_iq 	Temperature1_K = 0;		// 温度信号1
	volatile	_iq 	Temperature3_K = 0;		// 温度信号3
	volatile	_iq 	Temperature5_K = 0;		// 温度信号4




#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    //APP_MAIN_H 定义结束
//=========================================================================================
// No more.
//=========================================================================================

