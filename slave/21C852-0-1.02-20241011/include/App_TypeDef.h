/*
//==================================================================================
//
// 文件名:    App_TypeDef.h
//                   
// 功能描述:  所有新的结构类型定义,定义类型后在App_Main.h中定义此类型的变量
//			  	
// 
//编写者:    伍泽东
//编写日期:  2011
//
//=====================================================================================
*/
#ifndef APP_TYPEDEF_H
#define APP_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------------


//======================================================================================================
/* F2812的PWM驱动器结构定义		*/
//======================================================================================================
typedef struct
{	volatile Uint32	mHalfPeriod;		// 输入: 基于HISPCP时钟分频后的PWM周期的一半(Q0)
	volatile _iq		mDuty1;			// 输入: PWM 1&2 占空比 (QG)
	volatile _iq		mDuty2;			// 输入: PWM 3&4 占空比 (QG)
	volatile _iq		mDuty3;			// 输入: PWM 5&6 占空比 (QG)
}PWMGEN;
//------------------------------------------------------------------------------
// 默认的F2812 PWMGEN
// -----------------------------------------------------------------------------
#define PWMAGEN_DEFAULTS {	CARRY_A_P,HALFA_DUTY,HALFA_DUTY, HALFA_DUTY }
//------------------------------------------------------------------------------
// 函数原型在  App_EV.C
//------------------------------------------------------------------------------
// F2812的PWM驱动器结构定义结束
//======================================================================================================




//======================================================================================================
// F2812的RDC结构定义
//======================================================================================================
typedef	struct  
{     
	volatile Uint16	 FAULT;		    	// 出错标志 (1为有错,0位无错)
    volatile Uint16  DATA_BUFFER;   	// 读起数据的缓冲
}
RDC_DRIVER ;
//------------------------------------------------------------------------------
// 默认的RDC_READ1 RDC_READ2
// ------------------------------------------------------------------------------
#define RDC_READ_DEFAULT   { 0,0 }

//-----------------------------------------------------------------------------
//函数原型
//-----------------------------------------------------------------------------
void RDC_READ (void);

// F2812的RDC结构定义结束
//======================================================================================================
				


//======================================================================================================
/* 坐标系变换结构定义      */
//======================================================================================================
//------------------------------------------------------------------------------
// 静止abc三相坐标系变换到同步dq坐标系
//------------------------------------------------------------------------------
typedef struct 
{		
  		volatile  _iq  as;  		// 输入: a相变量
		volatile  _iq  bs;			// 输入: b相变量
		volatile  _iq  cs;			// 输入: c相变量
		volatile  _iq  ang;			// 输入: 电角度 (pu)
		volatile  _iq  dr;			// 输出: 同步坐标系d轴变量
		volatile  _iq  qr;			// 输出: 同步坐标系q轴变量
} 
Ref_ABCtoDQ;	            
                  
#define Ref_ABCtoDQ_DEFAULTS { 0,0,0,0,0,0 } // 定义初始化的Ref_ABCtoDQ
void ABC2DQ_CALC (void);						 // 函数原型
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 同步dq坐标系变换到静止abc坐标系
//------------------------------------------------------------------------------
typedef struct 
{  		
	volatile _iq  as;  		// 输出: a相变量
	volatile _iq  bs;		// 输出: b相变量
	volatile _iq  cs;		// 输出: c相变量
	volatile _iq  ang;		// 输入: 电角度 (pu)
	volatile _iq  dr;		// 输入: 同步坐标系d轴变量
	volatile _iq  qr;		// 输入: 同步坐标系q轴变量
} 
Ref_DQtoABC;	            
    
#define Ref_DQtoABC_DEFAULTS { 0,0,0,0,0,0 } // 定义初始化的Ref_DQtoABC
void DQ2ABC_CALC (void);						 // 函数原形
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 静止abc坐标系变换到静止的2相α，β
//------------------------------------------------------------------------------
typedef struct 
{ 		
	volatile _iq  as;  		// 输入: a相变量
	volatile _iq  bs;		// 输入: b相变量
	volatile _iq  cs;		// 输入: c相变量
	volatile _iq  ds;		// 输出: ds变量
	volatile _iq  qs;		// 输出: qs变量
} 
Ref_THREE2TWO;
	
#define Ref_THREE2TWO_DEFAULTS { 0,0,0,0,0 } // 定义初始化的Ref_THREE2TWO
void THREE2TWO_CALC (void);						 // 函数原形
//-----------------------------------------------------------------------------

// 坐标系变换结构定义结束
//======================================================================================================


//====================================================================================
/* PI调节器相关结构体定义		*/
//=====================================================================================
//------------------------------------------------------------------------------- 
// PI结构体的定义
//-------------------------------------------------------------------------------
typedef struct 
{  
		volatile _iq  pi_ref_reg;   // 输入: 给定输入
		volatile _iq  pi_fdb_reg;  	// 输入: 反馈输入
	    volatile _iq  e_reg;		// 变量: 误差
		volatile int32 loop_index;		
		volatile int32 i_period;		// 参数: 积分周期
		volatile _iq  Kp_reg;		// 参数: 比例系数
		volatile _iq  Ki_reg;		// 参数: 积分系数
		volatile _iq  ui_out_max;	// 参数: 积分输出上限幅
		volatile _iq  ui_out_min;	// 参数: 积分输出下限幅
		volatile _iq  pi_out_max;	// 参数: PI输出上限幅
		volatile _iq  pi_out_min;	// 参数: PI输出下限幅
		volatile _iq  up_reg;		// 变量: 比例输出
		volatile _iq  ui_reg;		// 变量: 积分输出
		volatile _iq  pi_out_reg;   // 输出: PI输出
		volatile int32 ctrl_period;  // 参数: 控制周期
		volatile int32 ctrl_index;		// 变量: 控制周期指数
					  
		void  (*calc)();	/* Pointer to calculation function */ 
} 
PIREG;	            

typedef PIREG *PIREG_handle;
#define PIREG_VELO_DEFAULTS {  0,0,0,0,VELO_I_NUM,KP_VELO,KI_VELO,KI_VELO_OUT_MAX,KI_VELO_OUT_MIN,PI_VELO_OUT_MAX,PI_VELO_OUT_MIN,0,0,0,VELO_CONTROL_PERIOD,\
                           0,(void (*)(int32))pi_reg_calc }
#define PIREG_ID_DEFAULTS {  0,0,0,0,ID_I_NUM,KP_ID,KI_ID,KI_ID_OUT_MAX,KI_ID_OUT_MIN,PI_ID_OUT_MAX,PI_ID_OUT_MIN,0,0,0,ID_CONTROL_PERIOD,\
                           0,(void (*)(int32))pi_reg_calc }
#define PIREG_IQ_DEFAULTS {  0,0,0,0,IQ_I_NUM,KP_IQ,KI_IQ,KI_IQ_OUT_MAX,KI_IQ_OUT_MIN,PI_IQ_OUT_MAX,PI_IQ_OUT_MIN,0,0,0,IQ_CONTROL_PERIOD,\
                           0,(void (*)(int32))pi_reg_calc }
void pi_reg_calc(PIREG *PIREG_handle);
// PI结构体定义结束
//====================================================================================


//====================================================================================
// SCI结构体的定义
//----------------------------------------------------------------------
// SCI相关操作的的位定义
struct	SCI_FLAG_BITS
{
	volatile Uint16	RX_FLAG:1;		// "1"表明接收有效标志
	volatile Uint16	TXBO_FLAG:1;	// 发送数据块完成标志
	volatile Uint16	RXTIME_FLAG:1;	// 1表示正在解析指令
	volatile Uint16	PC_FLAG:1;		// 1表示上位机控制模式
	volatile Uint16	rsvd1:12;
};
union	SCI_FLAG_REG
{
		volatile Uint16	all;
		volatile struct	SCI_FLAG_BITS	bit;
};
//------------------------------------------------------------------------------
// SCI结构体定义
//-----------------------------------------------------------------------
typedef	struct
{
		volatile union		SCI_FLAG_REG	FLAG;	// SCI的各标志
		volatile Uint16		m_Rx_Length;			// 接收数据长度
		volatile Uint16		m_Tx_Length;			// 发送数据长度
		volatile Uint16		*p_Tx_Buffer;			// 发送数据缓冲
		volatile Uint16		*p_Rx_Buffer;			// 接收数据缓冲
		volatile Uint16		*p_Rx_AllBuffer;		// 全部接收数据缓冲
}		volatile SCIB_REGS;
//------------------------------------------------------------------------------
// 默认的SCIB_REGS
//------------------------------------------------------------------------------
#define		SCIB_REGS_DEFAULTS	{{2},0,0,SCI_TX_BUFFER,SCI_RX_BUFFER,SCI_RX_ALLBUFFER}
// SCI结构定义结束
//======================================================================================


//====================================================================================
// CAN结构体的定义
//----------------------------------------------------------------------
// CAN相关操作的的位定义
struct	CAN_FLAG_BITS
{
		volatile	Uint16	RX_FLAG:1;			// "1"表明接收有效标志
		volatile	Uint16	TXBO_FLAG:1;		// 发送数据块完成标志
		volatile	Uint16	TIMETX_FLAG:1;		// 定时发送
		volatile	Uint16	DIAGTX_FLAG:1;		// 动态发送(按需发送)
		volatile	Uint16	rsvd1:12;
};
union	CAN_FLAG_REG
{
		volatile	Uint16	all;
		struct	CAN_FLAG_BITS	bit;
};
//------------------------------------------------------------------------------
// CAN结构体定义
//-----------------------------------------------------------------------
typedef	struct
{
		union		CAN_FLAG_REG	FLAG;		// SCI的各标志
		volatile	Uint16		m_Rx_Length;				// 接收数据长�
		volatile	Uint16		m_Tx_Length;				// 发送数据长度
		volatile	Uint16		*p_Tx_Buffer;				// 接收数据缓冲
		volatile	Uint16		*p_Rx_Buffer;				// 发送数据缓冲
		volatile	Uint16		m_Time_Index;				// 定时指针
		volatile	Uint16		m_Time_Length;				// 定时长度
}		CAN_REGS;
//------------------------------------------------------------------------------
// 默认的CAN_REGS
//------------------------------------------------------------------------------
#define		CAN_REGS_DEFAULTS	{2,0,0,TX_BUFFER,RX_BUFFER,0,500}


#define		CAN2_REGS_DEFAULTS	{2,0,0,SCI_RX_BUFFER,SCI_TX_BUFFER,0,500}



//======================================================================================================
/* 故障标志位结构定义		*/
//======================================================================================================
struct FAULT_FLAG_BITS
{
	volatile	Uint16	STOP_VELO_FLAG:1;		//"1"表示停止速度控制程序
	volatile	Uint16	STOP_PWM_Flag:1;    	//故障停止
	volatile	Uint16 STOP_PWM_Flag_Id:1;		// "1"表明电机过流保护
	volatile	Uint16 STOP_PWM_Flag_Velo:1;	// "1"表明超速保护
	volatile	Uint16 STOP_PWM_Flag_Driv:1;	// "1"表明PWMA功率保护
	volatile	Uint16 UDC_FLAG:1;				// "1"表明直流电压故障
	volatile	Uint16	speedFluctuation:1;//电机过温1
	volatile	Uint16	Temp2Over_Flag:1;//模块过温
	volatile	Uint16	UdcLow_Flag:1;//直流欠压
	volatile	Uint16	CANERROR_FLAG:1;//通信故障
	volatile	Uint16 RDC1_FAULT:1;
	volatile	Uint16 RDC2_FAULT:1;
	volatile	Uint16 POS_LIMIT:1;
	volatile	Uint16 RunAllow_Flag:1;
	volatile	Uint16 rsvd1:2;				// 保留
};
union FAULT_FLAG_REG
{	volatile	Uint16	all;
	volatile	struct	FAULT_FLAG_BITS bit;
};
//=======================================================================================
// 默认的故障标志位
//=======================================================================================
#define FAULT_FLAG_DEFAULT  {0x0};
// 故障标志位结构体定义结束
//====================================================================================



//======================================================================================================
/* 系统标志位结构定义		*/
//======================================================================================================
struct SYS_FLAG_BITS
{
	volatile	Uint16 STOP_PWM_Flag_Id:1;		// "1"表明电机过流保护
	volatile	Uint16 Flag_Id:1;				// "1"表明d轴电流环积分限幅
	volatile	Uint16 STOP_PWM_Flag_Velo:1;	// "1"表明超速保护
	volatile	Uint16 STOP_PWM_Flag_Driv:1;	// "1"表明PWMA功率保护
	volatile	Uint16 Flag_Iq:1;				// "1"表明q轴电流环积分限幅
	volatile	Uint16 Flag_Us:1;				// "1"表明总电压限幅
	volatile	Uint16 UDC_FLAG:1;				// "1"表明直流电压故障
	volatile	Uint16 CAN_FLAG:1;				// "1"表明从接收到控制指令
	volatile	Uint16 RDC_FAULT:8;				// 保留
};
union SYS_FLAG_REG
{	volatile	Uint16	all;
	volatile	struct	SYS_FLAG_BITS bit;
};
//=======================================================================================
// 默认的系统标志位
//=======================================================================================
#define SYS_FLAG_DEFAULT  {0x0};
// 系统标志位结构体定义结束
//====================================================================================


//=======================================================================================
struct CTRL_FLAG_BITS
{
	volatile	Uint16	speedFluctuation:1;//电机过温1
	volatile	Uint16	Temp2Over_Flag:1;//模块过温
	volatile	Uint16	UdcLow_Flag:1;//直流欠压
	volatile	Uint16	CANERROR_FLAG:1;//通信故障
	volatile	Uint16	STOP_PWM_Flag:1;//故障停止
	volatile	Uint16	CAN_Ctrl:1;				
	volatile	Uint16	FIRST_PWM_Flag:1;//首次进入控制
	volatile	Uint16	VELO_CONTROL_FLAG:1;	//"1"表示进入速度控制程序
	volatile	Uint16	TORQ_CONTROL_FLAG:1;	//"1"表示进入转矩控制程序
	volatile	Uint16	OPEN_LOOP_FLAG:1;       //Openloop  618刘刃改
	volatile	Uint16	LOCK_FLAG:1;			//"1"表示LOCK  618刘刃改
	volatile	Uint16	UsCtrl_Flag:1;//自动弱磁
	volatile	Uint16	STOP_VELO_FLAG:1;		//"1"表示停止速度控制程序
	volatile	Uint16	POS_CONTROL_FLAG:1;//"1"表示进入位置控制程序
	volatile	Uint16	RotorWay_Flag:1;//旋变方向
	volatile	Uint16	VelWay_Flag:1;//转速方向
};
union	CTRL_FLAG_REG
{	volatile	Uint16	all;
	volatile	struct	CTRL_FLAG_BITS bit;
};

#define	CTRL_FLAG_DEFAULT	{0x1000};

//======================================================================================

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    //APP_TYPEDEF_H 定义结束
//===========================================================================
// End of file.
//===========================================================================
