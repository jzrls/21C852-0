/* 
//====================================================================================
//
// 文件名:	 App_Marco.h
//
// 功能描述: 主程序常数宏定义，包括各个程序所用的宏定义  
//
// 编写者:	 刘刃 
// 编写日期: 2021-05-10
//
//====================================================================================
*/
#ifndef APP_MACRO_H
#define APP_MACRO_H

#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
/*  系统相关的宏定义		*/
//=============================================================================
	#define PLLCR_RATIO   10       //设置相应的PLL时钟倍频倍数，当晶振为30MHZ时，为VCOCLK=30*PLLCR_RATIO=300MHZ 
	#define PI _IQ(3.1415926535897932384626433832795)		// QG
// 指定CPU(SYSCLKOUT) 的时钟 (nS)，后面L表示告诉编译器把它当作是一个64bit的值
	#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
// A为延时时间(us)
	#define DELAY_US(A) ( F28335_usDelay((((float64) A * 1000.0L/(float64)CPU_RATE) - 9.0L) / 5.0L) )
// 当前晶振为30MHz,5倍频==>30*(10/2)=150MHz
	#define VCOCLK_DIVSEL   2       // Enable /2 for SYSCLKOUT, SYSCLKOUT=VCOCLK/2=150MHZ
// HISPCP分频系数,(EV,ADC), High Speed Clock = SYSCLKOUT/(2*HISPCP_RATIO)=75MHz
	#define	HISPCP_RATIO	0x0001
// LOSPCP分频系数,(SCI,SPI,McBSP),Low Speed Clock = SYSCLKOUT/(2*LOSPCP_RATIO)=37.5MHz
	//#define	LOSPCP_RATIO	0x0001
	#define	LOSPCP_RATIO	0x0002
	#define STATUS_FAIL          0
	#define STATUS_SUCCESS       1       //解锁CSM，解锁成功返回1
//  系统相关的宏定义结束  
//==============================================================================  

//-------------------------------------------------------------------
	#define RUN_XINTF	0
	#define RUN_RAM		0
	#define	RUN_FLASH	1
	#define RUN_MODE	RUN_FLASH  		

	#define BORAD_NUM	3	//板号
	//#define BORAD_NUM	0	//板号	阀1主
	//#define BORAD_NUM	1	//板号	阀1从
	//#define BORAD_NUM	2	//板号	泵1主
	//#define BORAD_NUM	3	//板号	泵1从
	//#define BORAD_NUM	4	//板号	阀2主
	//#define BORAD_NUM	5	//板号	阀2从	
//--------------------------------------------------------------------

//======================================================================================================
/*  CpuTimer0相关的宏定义		*/
//======================================================================================================
	//#define 	CPUTIMER0_PRD 	0x7530		// 时基对应频率为150MHz，200us 
	//#define 	CPUTIMER0_PRD 	0xFFFF		// 时基对应频率为150MHz，65535/150
	#define 	CPUTIMER0_PRD 	1500000		// 时基对应频率为150MHz，10ms 
	#define 	PWM_DeadTime 288	/*3.84us*/
//======================================================================================================
/*  CpuTimer0相关的宏定义结束	*/
//====================================================================================


//======================================================================================================
/*  SCI相关的宏定义		*/
//======================================================================================================
//SCI宏定义
	#define		RX_BUFFER_LENGTH	220	 	// 发送数据缓冲最大长度
	#define		TX_BUFFER_LENGTH	220	 	// 接收数据缓冲最大长度
	#define		RX_ALLBUFFER_LENGTH	220	 	// 接收数据缓冲最大长度
	//#define 	SCI_BAUD			0x01E7	// SYSCLKOUT/(2*LOSPCP_RATIO*9600bps*8)-1
	//#define 	SCI_BAUD			0x50	// SYSCLKOUT/(2*LOSPCP_RATIO*57600bps*8)-1
	#define 	SCI_BAUD			0x28	// SYSCLKOUT/(2*LOSPCP_RATIO*115200bps*8)-1
// SCI相关的宏定义结束
//======================================================================================================


//======================================================================================================
/*  ADC相关的宏定义		*/
//======================================================================================================
//ADC宏定义
	#define 	ADC_usDELAY 	5000L		// 延时5ms
	#define		SMODE			0			// 1并发采样,0连续采样
	#define 	ADC_MAX_CON   	7			// 若并发,转换组数据,连续单个通道:4个转换
	#define 	ADC_CLKPS     	5     		// ADCCLK = HSPCLK/[2*ADC_CLKPS*(ADC_CPS+1)]
	#define 	ADC_SHCLK 		1     		// 采样窗口的大小(ADC_SHCLK+1)*ADCCLK
// ADC相关的宏定义结束
//======================================================================================================


//======================================================================================================
/*  PWM相关的宏定义		*/
//======================================================================================================
// 载波频率对应的T1PR和T3PR写入值，为真实值的一半，时基为 75MHz。
  	#define		CARRY_A_P		7500			// EVA, 5k,	Q0
	#define		HALFA_DUTY		_IQ(0.5)		// QG 0.5
// PWM相关的宏定义结束
//======================================================================================================


//======================================================================================================
/*  各基值相关参数的宏定义		*/
//======================================================================================================
	
	#define 	U_BASE 			 _IQ(500)			// QG 电压基值:500V 
	#define 	I_BASE			 _IQ(2000)			// QG 电流基值:2000A
	#define		V_BASE			 _IQ(1920)			//QG 电速度基值 

	
	//-----------------------------------------------
	// Zb = Ub/Ib = 500/2000 =0.25 ou
	// Lb =	Zb/(P*V_M_b) = 0.25/(1920) = 0.13021mH
	// PHIb = U/(P*V_M_b) = 500/(1920) = 0.26042Wb
	//------------------------------------------------
																					
// 各基值相关参数的宏定义结束
//======================================================================================================


//======================================================================================================
/* 电机相关相关参数标幺值的宏定义		*/
//======================================================================================================
	//-----------------------------------------------
	// 电机的相关参数

	#define		K_LD			_IQ(0.0767990)	    // QG 电机Ld标幺值:0.03760194685489119202875647793439 mH	
	#define 	K_LQ			_IQ(0.3839950)		// QG 电机Lq标幺值:0.03760194685489119202875647793439 mH	
	#define 	K_RS			_IQ(0.2)			// QG 电机定子电阻标幺值:0.036 欧
	#define		K_PHI			_IQ(1.5890029518) 	// GQ 永磁体的磁链标幺值:0.27515Wb	0.41072243378553635037131293773552

	#define		K_MP			_IQ(10)				// 电机的极对数
	#define		K_RP			_IQ(5)				// 旋变的极对数
	#define		K_P				_IQ(2)				// 电机与旋变的极?

// 网侧整流器,电嗖问幺值的宏定义结束
//======================================================================================================


//======================================================================================================
/*  PI相关参数的宏定义		*/
//======================================================================================================
	//-------------------------------------------------------------
	// 控制PI参数宏定义
			
	// 速度环控制PI参数宏定义
	#define		VELO_I_NUM			100				// Q0 ????,???????????		
	#define		KP_VELO			    _IQ(0.1)		// QG ????PI?????
	#define		KI_VELO				_IQ(0.001)		// QG ????PI?????
	#define		KI_VELO_OUT_MAX		_IQ(0.025)		// QG ????PI????????
	#define		KI_VELO_OUT_MIN		_IQ(-0.025)		// QG ????PI????????
	#define		PI_VELO_OUT_MAX		_IQ(0.025)		// QG ????PI??????
	#define		PI_VELO_OUT_MIN		_IQ(-0.025)		// QG ????PI??????
    #define		VELO_CONTROL_PERIOD	  	10			// Q0???????????

	// 电流环Iq控制PI参数宏定义
	#define		ID_I_NUM			30				// G0 ????,???????????		
	#define		KP_ID			    _IQ(0.2)		// QG ID?PI?????
	#define		KI_ID				_IQ(0.03)		// QG ID?PI?????
	#define		KI_ID_OUT_MAX		_IQ(7.8)		// QG ID?PI????????
	#define		KI_ID_OUT_MIN		_IQ(-7.8)		// QG ID?PI????????
	#define		PI_ID_OUT_MAX		_IQ(8)			// QG ID?PI??????
	#define		PI_ID_OUT_MIN		_IQ(-8)			// QG ID?PI??????
	
	#define		ID_CONTROL_PERIOD	1				// Q0???????????
	
	// 电流环Id控制PI参数宏定义
	#define		IQ_I_NUM			30				// G0 ????,???????????		
	#define		KP_IQ				_IQ(0.2)		// QG IQ?PI?????
	#define		KI_IQ				_IQ(0.03)		// QG IQ?PI?????
	#define		KI_IQ_OUT_MAX		_IQ(7.8)		// QG IQ?PI????????
	#define		KI_IQ_OUT_MIN		_IQ(-7.8)		// QG IQ?PI????????
	#define		PI_IQ_OUT_MAX		_IQ(8)			// QG IQ?PI??????
	#define		PI_IQ_OUT_MIN		_IQ(-8)			// QG IQ?PI??????
	
	#define		IQ_CONTROL_PERIOD	1				// Q0???????????
	

//===============================================================================
// 调试相关定义
	#define		FALSE	0
	#define		TRUE	1
//------------------------------------------------
	#ifndef  FORBIDPWMA
		#define  FORBIDPWMA 	0			// 0 开放PWM; 1 关闭PWM
	#endif	
	
	#ifndef  FORBIDPWMB
		#define  FORBIDPWMB	    0
	#endif
//-----------------------------------------------
//------------------------------------------------------

//-------------------------------------------------------
	#define		RUN_LED_PERIOD	 		10000	 // 运行灯闪烁次数(多少个控制周期闪烁一次)
//---------------------------------------------------
//----------------------------------------------------
	#define		INIT_ANGLE		38616	// 20110412使用电机 初始位置角
//-------------------------------------------------------

//------------------------------------------------------

	
	#define		K_VELO_CALC			_IQ(3.2724923475)
													// v_pu = (Delta_Angle*2*pi/(T*N))/(v_base) 
													// 2*pi/(v_b*T*N) = 32.724923475/N	
//保护函数中的相关宏定义																		
	#define 	IsMAXNUM		30					// Id电流保护最大计数个数
	#define 	IsMAX			_IQ(0.2)//_IQ(0.45)			// Id电流保护最大值  489A
	#define 	VELO_MAXNUM		600					// 速度保护最大计数个数
	#define 	Velo_MAX		_IQ(1.4180800172453927812504987493969)				// 速度保护最大值
	#define		UdcMAX			_IQ(2.15)			//Udc1 800V 过压保护最大值   1075V
	#define 	UdcMAXNUM		30000					// Udc3电流保护最大计数个数	
	#define		PROTECT_NUM	(1000)			
														
	#define 	POSMAX		65000
	#define 	POSMIN		500
//=========================================================================================

//====================================================================================
//相关I/O口宏定义
//--------------------------------------------------------------------------
	#define 	BC_SLT_IN		GpioDataRegs.GPADAT.bit.GPIO6		//输入
	#define 	ET_IO_IN1		GpioDataRegs.GPADAT.bit.GPIO7		//输入
	#define 	ET_IO_IN2		GpioDataRegs.GPADAT.bit.GPIO8		//输入

	#define	 	SYN_CLKOUT_H	(GpioDataRegs.GPASET.bit.GPIO9 = 1)		//输出高
	#define	 	SYN_CLKOUT_L	(GpioDataRegs.GPACLEAR.bit.GPIO9 = 1)	//输出低 

	#define 	SYN_CLK_IN		GpioDataRegs.GPADAT.bit.GPIO10		//输入
	#define 	MS_SLT_IN		GpioDataRegs.GPADAT.bit.GPIO11		//输入

	#define	 	OPEN_INT1_H		(GpioDataRegs.GPASET.bit.GPIO12 = 1)		//输出高
	#define	 	OPEN_INT1_L		(GpioDataRegs.GPACLEAR.bit.GPIO12 = 1)		//输出低

	#define 	INT_IGBT		GpioDataRegs.GPADAT.bit.GPIO13		//输入

	#define	 	EN_WORK_H		(GpioDataRegs.GPASET.bit.GPIO16 = 1)		//输出高
	#define	 	EN_WORK_L		(GpioDataRegs.GPACLEAR.bit.GPIO16 = 1)		//输出低 

	#define	 	IGBT_PCTRL_H	(GpioDataRegs.GPASET.bit.GPIO17 = 1)		//输出高
	#define	 	IGBT_PCTRL_L	(GpioDataRegs.GPACLEAR.bit.GPIO17 = 1)		//输出低 
	  
	#define 	VALVE_CTRL_H		(GpioDataRegs.GPASET.bit.GPIO20 = 1)		//输出高  有效 
	#define 	VALVE_CTRL_L		(GpioDataRegs.GPACLEAR.bit.GPIO20 = 1)		//输出低

	#define 	I_O_IN1		GpioDataRegs.GPADAT.bit.GPIO25		//输入
	#define 	I_O_IN2		GpioDataRegs.GPADAT.bit.GPIO26		//输入

	#define I_O_OUT1_H			(GpioDataRegs.GPASET.bit.GPIO27 = 1)
	#define I_O_OUT1_L			(GpioDataRegs.GPACLEAR.bit.GPIO27 = 1)
	#define I_O_OUT2_H			(GpioDataRegs.GPBSET.bit.GPIO32 = 1)
	#define I_O_OUT2_L			(GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1)

	#define ET_IO_OUT1_H			(GpioDataRegs.GPBSET.bit.GPIO58 = 1)
	#define ET_IO_OUT1_L			(GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1)
	#define ET_IO_OUT2_H			(GpioDataRegs.GPBSET.bit.GPIO59 = 1)
	#define ET_IO_OUT2_L			(GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1)

	#define LED2			GpioDataRegs.GPBTOGGLE.bit.GPIO33
	#define LED1			GpioDataRegs.GPBTOGGLE.bit.GPIO49

	#define EN_WP		GpioDataRegs.GPBCLEAR.bit.GPIO53
	#define DIS_WP		GpioDataRegs.GPBSET.bit.GPIO53

	#define EN_X25040		GpioDataRegs.GPBCLEAR.bit.GPIO61
	#define DIS_X25040		GpioDataRegs.GPBSET.bit.GPIO61

	#define RDC_A0_H			(GpioDataRegs.GPASET.bit.GPIO21 = 1)
	#define RDC_A0_L			(GpioDataRegs.GPACLEAR.bit.GPIO21 = 1)
	#define RDC_A1_H			(GpioDataRegs.GPASET.bit.GPIO22 = 1)
	#define RDC_A1_L			(GpioDataRegs.GPACLEAR.bit.GPIO22 = 1)


	#define	RDC_DIS_B		(GpioDataRegs.GPASET.bit.GPIO24 = 1)		//输出高
	#define	RDC_EN_B		(GpioDataRegs.GPACLEAR.bit.GPIO24 = 1)		//输出低
	#define	RDC_DIS_A		(GpioDataRegs.GPBSET.bit.GPIO52 = 1)		//输出高
	#define	RDC_EN_A		(GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1)		//输出低
	#define RDC_RESET_H		(GpioDataRegs.GPBSET.bit.GPIO48 = 1)
	#define RDC_RESET_L		(GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1)
	#define RDC_RES0_H		(GpioDataRegs.GPBSET.bit.GPIO50 = 1)
	#define RDC_RES0_L		(GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)
	#define RDC_RES1_H		(GpioDataRegs.GPBSET.bit.GPIO51 = 1)
	#define RDC_RES1_L		(GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1)
	#define	RDC_FSYNC_H	(GpioDataRegs.GPBSET.bit.GPIO57 = 1)		//输出高
	#define	RDC_FSYNC_L	(GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1)		//输出低  有效
	#define RDC_SAMPLE_H		(GpioDataRegs.GPBSET.bit.GPIO60 = 1)
	#define RDC_SAMPLE_L		(GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1)

//--------------------------------------------------------------------------
//I/O口相关宏定义结束
//====================================================================================


//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    //APP_MACRO_H 定义结束
//===========================================================================
// End of file.
//===========================================================================

//===========================================================================
