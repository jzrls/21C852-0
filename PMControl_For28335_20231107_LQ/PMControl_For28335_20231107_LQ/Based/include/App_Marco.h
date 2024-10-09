/* 
//====================================================================================
//
// �ļ���:	 App_Marco.h
//
// ��������: ���������궨�壬���������������õĺ궨��  
//
// ��д��:	 ���� 
// ��д����: 2021-05-10
//
//====================================================================================
*/
#ifndef APP_MACRO_H
#define APP_MACRO_H

#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
/*  ϵͳ��صĺ궨��		*/
//=============================================================================
	#define PLLCR_RATIO   10       //������Ӧ��PLLʱ�ӱ�Ƶ������������Ϊ30MHZʱ��ΪVCOCLK=30*PLLCR_RATIO=300MHZ 
	#define PI _IQ(3.1415926535897932384626433832795)		// QG
// ָ��CPU(SYSCLKOUT) ��ʱ�� (nS)������L��ʾ���߱���������������һ��64bit��ֵ
	#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
// AΪ��ʱʱ��(us)
	#define DELAY_US(A) ( F28335_usDelay((((float64) A * 1000.0L/(float64)CPU_RATE) - 9.0L) / 5.0L) )
// ��ǰ����Ϊ30MHz,5��Ƶ==>30*(10/2)=150MHz
	#define VCOCLK_DIVSEL   2       // Enable /2 for SYSCLKOUT, SYSCLKOUT=VCOCLK/2=150MHZ
// HISPCP��Ƶϵ��,(EV,ADC), High Speed Clock = SYSCLKOUT/(2*HISPCP_RATIO)=75MHz
	#define	HISPCP_RATIO	0x0001
// LOSPCP��Ƶϵ��,(SCI,SPI,McBSP),Low Speed Clock = SYSCLKOUT/(2*LOSPCP_RATIO)=37.5MHz
	//#define	LOSPCP_RATIO	0x0001
	#define	LOSPCP_RATIO	0x0002
	#define STATUS_FAIL          0
	#define STATUS_SUCCESS       1       //����CSM�������ɹ�����1
//  ϵͳ��صĺ궨�����  
//==============================================================================  

//-------------------------------------------------------------------
	#define RUN_XINTF	0
	#define RUN_RAM		0
	#define	RUN_FLASH	1
	#define RUN_MODE	RUN_FLASH  		

	#define BORAD_NUM	3	//���
	//#define BORAD_NUM	0	//���	��1��
	//#define BORAD_NUM	1	//���	��1��
	//#define BORAD_NUM	2	//���	��1��
	//#define BORAD_NUM	3	//���	��1��
	//#define BORAD_NUM	4	//���	��2��
	//#define BORAD_NUM	5	//���	��2��	
//--------------------------------------------------------------------

//======================================================================================================
/*  CpuTimer0��صĺ궨��		*/
//======================================================================================================
	//#define 	CPUTIMER0_PRD 	0x7530		// ʱ����ӦƵ��Ϊ150MHz��200us 
	//#define 	CPUTIMER0_PRD 	0xFFFF		// ʱ����ӦƵ��Ϊ150MHz��65535/150
	#define 	CPUTIMER0_PRD 	1500000		// ʱ����ӦƵ��Ϊ150MHz��10ms 
	#define 	PWM_DeadTime 288	/*3.84us*/
//======================================================================================================
/*  CpuTimer0��صĺ궨�����	*/
//====================================================================================


//======================================================================================================
/*  SCI��صĺ궨��		*/
//======================================================================================================
//SCI�궨��
	#define		RX_BUFFER_LENGTH	220	 	// �������ݻ�����󳤶�
	#define		TX_BUFFER_LENGTH	220	 	// �������ݻ�����󳤶�
	#define		RX_ALLBUFFER_LENGTH	220	 	// �������ݻ�����󳤶�
	//#define 	SCI_BAUD			0x01E7	// SYSCLKOUT/(2*LOSPCP_RATIO*9600bps*8)-1
	//#define 	SCI_BAUD			0x50	// SYSCLKOUT/(2*LOSPCP_RATIO*57600bps*8)-1
	#define 	SCI_BAUD			0x28	// SYSCLKOUT/(2*LOSPCP_RATIO*115200bps*8)-1
// SCI��صĺ궨�����
//======================================================================================================


//======================================================================================================
/*  ADC��صĺ궨��		*/
//======================================================================================================
//ADC�궨��
	#define 	ADC_usDELAY 	5000L		// ��ʱ5ms
	#define		SMODE			0			// 1��������,0��������
	#define 	ADC_MAX_CON   	7			// ������,ת��������,��������ͨ��:4��ת��
	#define 	ADC_CLKPS     	5     		// ADCCLK = HSPCLK/[2*ADC_CLKPS*(ADC_CPS+1)]
	#define 	ADC_SHCLK 		1     		// �������ڵĴ�С(ADC_SHCLK+1)*ADCCLK
// ADC��صĺ궨�����
//======================================================================================================


//======================================================================================================
/*  PWM��صĺ궨��		*/
//======================================================================================================
// �ز�Ƶ�ʶ�Ӧ��T1PR��T3PRд��ֵ��Ϊ��ʵֵ��һ�룬ʱ��Ϊ 75MHz��
  	#define		CARRY_A_P		7500			// EVA, 5k,	Q0
	#define		HALFA_DUTY		_IQ(0.5)		// QG 0.5
// PWM��صĺ궨�����
//======================================================================================================


//======================================================================================================
/*  ����ֵ��ز����ĺ궨��		*/
//======================================================================================================
	
	#define 	U_BASE 			 _IQ(500)			// QG ��ѹ��ֵ:500V 
	#define 	I_BASE			 _IQ(2000)			// QG ������ֵ:2000A
	#define		V_BASE			 _IQ(1920)			//QG ���ٶȻ�ֵ 

	
	//-----------------------------------------------
	// Zb = Ub/Ib = 500/2000 =0.25 ou
	// Lb =	Zb/(P*V_M_b) = 0.25/(1920) = 0.13021mH
	// PHIb = U/(P*V_M_b) = 500/(1920) = 0.26042Wb
	//------------------------------------------------
																					
// ����ֵ��ز����ĺ궨�����
//======================================================================================================


//======================================================================================================
/* ��������ز�������ֵ�ĺ궨��		*/
//======================================================================================================
	//-----------------------------------------------
	// �������ز���

	#define		K_LD			_IQ(0.0767990)	    // QG ���Ld����ֵ:0.03760194685489119202875647793439 mH	
	#define 	K_LQ			_IQ(0.3839950)		// QG ���Lq����ֵ:0.03760194685489119202875647793439 mH	
	#define 	K_RS			_IQ(0.2)			// QG ������ӵ������ֵ:0.036 ŷ
	#define		K_PHI			_IQ(1.5890029518) 	// GQ ������Ĵ�������ֵ:0.27515Wb	0.41072243378553635037131293773552

	#define		K_MP			_IQ(10)				// ����ļ�����
	#define		K_RP			_IQ(5)				// ����ļ�����
	#define		K_P				_IQ(2)				// ���������ļ�?

// ����������,�����������ֵ�ĺ궨�����
//======================================================================================================


//======================================================================================================
/*  PI��ز����ĺ궨��		*/
//======================================================================================================
	//-------------------------------------------------------------
	// ����PI�����궨��
			
	// �ٶȻ�����PI�����궨��
	#define		VELO_I_NUM			100				// Q0 ????,???????????		
	#define		KP_VELO			    _IQ(0.1)		// QG ????PI?????
	#define		KI_VELO				_IQ(0.001)		// QG ????PI?????
	#define		KI_VELO_OUT_MAX		_IQ(0.025)		// QG ????PI????????
	#define		KI_VELO_OUT_MIN		_IQ(-0.025)		// QG ????PI????????
	#define		PI_VELO_OUT_MAX		_IQ(0.025)		// QG ????PI??????
	#define		PI_VELO_OUT_MIN		_IQ(-0.025)		// QG ????PI??????
    #define		VELO_CONTROL_PERIOD	  	10			// Q0???????????

	// ������Iq����PI�����궨��
	#define		ID_I_NUM			30				// G0 ????,???????????		
	#define		KP_ID			    _IQ(0.2)		// QG ID?PI?????
	#define		KI_ID				_IQ(0.03)		// QG ID?PI?????
	#define		KI_ID_OUT_MAX		_IQ(7.8)		// QG ID?PI????????
	#define		KI_ID_OUT_MIN		_IQ(-7.8)		// QG ID?PI????????
	#define		PI_ID_OUT_MAX		_IQ(8)			// QG ID?PI??????
	#define		PI_ID_OUT_MIN		_IQ(-8)			// QG ID?PI??????
	
	#define		ID_CONTROL_PERIOD	1				// Q0???????????
	
	// ������Id����PI�����궨��
	#define		IQ_I_NUM			30				// G0 ????,???????????		
	#define		KP_IQ				_IQ(0.2)		// QG IQ?PI?????
	#define		KI_IQ				_IQ(0.03)		// QG IQ?PI?????
	#define		KI_IQ_OUT_MAX		_IQ(7.8)		// QG IQ?PI????????
	#define		KI_IQ_OUT_MIN		_IQ(-7.8)		// QG IQ?PI????????
	#define		PI_IQ_OUT_MAX		_IQ(8)			// QG IQ?PI??????
	#define		PI_IQ_OUT_MIN		_IQ(-8)			// QG IQ?PI??????
	
	#define		IQ_CONTROL_PERIOD	1				// Q0???????????
	

//===============================================================================
// ������ض���
	#define		FALSE	0
	#define		TRUE	1
//------------------------------------------------
	#ifndef  FORBIDPWMA
		#define  FORBIDPWMA 	0			// 0 ����PWM; 1 �ر�PWM
	#endif	
	
	#ifndef  FORBIDPWMB
		#define  FORBIDPWMB	    0
	#endif
//-----------------------------------------------
//------------------------------------------------------

//-------------------------------------------------------
	#define		RUN_LED_PERIOD	 		10000	 // ���е���˸����(���ٸ�����������˸һ��)
//---------------------------------------------------
//----------------------------------------------------
	#define		INIT_ANGLE		38616	// 20110412ʹ�õ�� ��ʼλ�ý�
//-------------------------------------------------------

//------------------------------------------------------

	
	#define		K_VELO_CALC			_IQ(3.2724923475)
													// v_pu = (Delta_Angle*2*pi/(T*N))/(v_base) 
													// 2*pi/(v_b*T*N) = 32.724923475/N	
//���������е���غ궨��																		
	#define 	IsMAXNUM		30					// Id������������������
	#define 	IsMAX			_IQ(0.2)//_IQ(0.45)			// Id�����������ֵ  489A
	#define 	VELO_MAXNUM		600					// �ٶȱ�������������
	#define 	Velo_MAX		_IQ(1.4180800172453927812504987493969)				// �ٶȱ������ֵ
	#define		UdcMAX			_IQ(2.15)			//Udc1 800V ��ѹ�������ֵ   1075V
	#define 	UdcMAXNUM		30000					// Udc3������������������	
	#define		PROTECT_NUM	(1000)			
														
	#define 	POSMAX		65000
	#define 	POSMIN		500
//=========================================================================================

//====================================================================================
//���I/O�ں궨��
//--------------------------------------------------------------------------
	#define 	BC_SLT_IN		GpioDataRegs.GPADAT.bit.GPIO6		//����
	#define 	ET_IO_IN1		GpioDataRegs.GPADAT.bit.GPIO7		//����
	#define 	ET_IO_IN2		GpioDataRegs.GPADAT.bit.GPIO8		//����

	#define	 	SYN_CLKOUT_H	(GpioDataRegs.GPASET.bit.GPIO9 = 1)		//�����
	#define	 	SYN_CLKOUT_L	(GpioDataRegs.GPACLEAR.bit.GPIO9 = 1)	//����� 

	#define 	SYN_CLK_IN		GpioDataRegs.GPADAT.bit.GPIO10		//����
	#define 	MS_SLT_IN		GpioDataRegs.GPADAT.bit.GPIO11		//����

	#define	 	OPEN_INT1_H		(GpioDataRegs.GPASET.bit.GPIO12 = 1)		//�����
	#define	 	OPEN_INT1_L		(GpioDataRegs.GPACLEAR.bit.GPIO12 = 1)		//�����

	#define 	INT_IGBT		GpioDataRegs.GPADAT.bit.GPIO13		//����

	#define	 	EN_WORK_H		(GpioDataRegs.GPASET.bit.GPIO16 = 1)		//�����
	#define	 	EN_WORK_L		(GpioDataRegs.GPACLEAR.bit.GPIO16 = 1)		//����� 

	#define	 	IGBT_PCTRL_H	(GpioDataRegs.GPASET.bit.GPIO17 = 1)		//�����
	#define	 	IGBT_PCTRL_L	(GpioDataRegs.GPACLEAR.bit.GPIO17 = 1)		//����� 
	  
	#define 	VALVE_CTRL_H		(GpioDataRegs.GPASET.bit.GPIO20 = 1)		//�����  ��Ч 
	#define 	VALVE_CTRL_L		(GpioDataRegs.GPACLEAR.bit.GPIO20 = 1)		//�����

	#define 	I_O_IN1		GpioDataRegs.GPADAT.bit.GPIO25		//����
	#define 	I_O_IN2		GpioDataRegs.GPADAT.bit.GPIO26		//����

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


	#define	RDC_DIS_B		(GpioDataRegs.GPASET.bit.GPIO24 = 1)		//�����
	#define	RDC_EN_B		(GpioDataRegs.GPACLEAR.bit.GPIO24 = 1)		//�����
	#define	RDC_DIS_A		(GpioDataRegs.GPBSET.bit.GPIO52 = 1)		//�����
	#define	RDC_EN_A		(GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1)		//�����
	#define RDC_RESET_H		(GpioDataRegs.GPBSET.bit.GPIO48 = 1)
	#define RDC_RESET_L		(GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1)
	#define RDC_RES0_H		(GpioDataRegs.GPBSET.bit.GPIO50 = 1)
	#define RDC_RES0_L		(GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)
	#define RDC_RES1_H		(GpioDataRegs.GPBSET.bit.GPIO51 = 1)
	#define RDC_RES1_L		(GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1)
	#define	RDC_FSYNC_H	(GpioDataRegs.GPBSET.bit.GPIO57 = 1)		//�����
	#define	RDC_FSYNC_L	(GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1)		//�����  ��Ч
	#define RDC_SAMPLE_H		(GpioDataRegs.GPBSET.bit.GPIO60 = 1)
	#define RDC_SAMPLE_L		(GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1)

//--------------------------------------------------------------------------
//I/O����غ궨�����
//====================================================================================


//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    //APP_MACRO_H �������
//===========================================================================
// End of file.
//===========================================================================

//===========================================================================
