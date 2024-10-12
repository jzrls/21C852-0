/*
//===============================================================================
//
// ÎÄ¼şÃû:   App_UserDef.h
//
// ¹¦ÄÜÃèÊö: 
//			  ¸ÃÎÄ¼ş°üº¬CºÍ»ã±àµÄÈ«¾Ö±äÁ¿µÄÒıÓÃÉùÃ÷,½á¹¹ÌåºÍº¯ÊıµÄÉêÃ÷
//			  ÔÚËùÓĞµÄCÔ­³ÌĞòÖĞ±ØĞë°üº¬´ËÎÄ¼ş.
//
// ±àĞ´Õß:    ÎéÔó¶«
// ±àĞ´ÈÕÆÚ:  2011-3-30
//
//===============================================================================
*/
#ifndef APP_USERDEF_H
#define APP_USERDEF_H

#ifdef __cplusplus
extern "C" {
#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#include "App_Marco.h"
#include "App_TypeDef.h"
//------------------------------------------------------------------------------

extern  void ScibRxCheck(void);
extern  void ScibTxCheck(void);
extern  void WriteEEProm(void);
extern  volatile Uint16 WriteEEProm_Flag;
//extern volatile Uint16 Delay_Flag;
//extern volatile Uint16 Delay_Index;
extern volatile Uint16 RunAllow_Flag;
//==============================================================================
// Íâ²¿º¯ÊıÉùÃ÷
//==============================================================================
	extern Uint16 g_pusCrc16[256];
	//--------------------------------------------------------------
	extern	void MainLoop(void);
	extern void Control(void);
	extern void Ctrl_Change(void);

	extern _iq RCFilter(volatile _iq filtout,volatile _iq filtin,volatile _iq Freq_cut);
		
	//-----------------------------------------------------------
	extern	_iq	_IQasinPU(_iq PA);
	extern	_iq	_IQasin4PU(_iq real, _iq imag);
	extern	_iq	AngleAdjust(_iq angle);
	extern	_iq17 Adc_Arith(volatile Uint16 AD_digital_value);
	extern	_iq Adc_Temp(volatile Uint16 AD_digital_value1);

	//------------------------------------------------------------
	extern void SVPWMA_Gen(_iq Alfa,_iq Beta); 
	extern void PWMA_Update(void);
	extern void Forbid_PWMA(void); 
	
	//-------------------------------------------------------------
	interrupt void Cpu_Timer0_ISR(void);
	interrupt void EPWM1_INT_ISR(void);
	
	//--------------------------------------------------------------
	extern	void	CAN_DataCombine_Rx(Uint16 *p_Rx);
	extern	void	CAN2_DataCombine_Rx(Uint16 *p_Rx);
	
	extern Uint16 RDSR_X25040(void);
	extern void WREN_X25040(void);
	extern void WRSR_X25040(Uint16 COM);
	extern void WRITE_X25040(Uint16 *RA_ADDR,Uint16 EEP_ADDR,Uint16 N,Uint16 H);
	extern void READ_X25040(Uint16 *RA_ADDR,Uint16 EEP_ADDR,Uint16 N,Uint16 H);

	extern void Init_AD1210(void);
	extern void RDC_READ(void);
	extern void Can_Tx_Data();
	extern void Can_Tx2_Data();
	extern void Can_Tx4_Data();
	extern void Can_Tx11_Data();
	extern void Can_Tx13_Data();
	extern void Can_Ctrl(Uint16 Board);
	extern Uint16  CalCRC16_Byte(Uint16 *pbDataBuf, Uint32 dwNumOfBytes);
	extern Uint16  Float_Hex(_iq f_in);
	extern _iq  Hex_Double(Uint16 ft1,Uint16 ft2,Uint16 ft3,Uint16 ft4);
	extern _iq  Hex_Float(Uint16 ft1,Uint16 ft2,Uint16 ft3,Uint16 ft4);
//===================================================================
//ÖĞ¶Ï·şÎñº¯ÊıÉùÃ÷
//===================================================================
	interrupt void TZ2_Fault_Isr(void);
	interrupt void XINT1_Fault_Isr(void);
//===================================================================
//======================================================================================================
// Íâ²¿º¯ÊıÉùÃ÷½áÊø




	extern	void	Sci_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L);
	extern	void	Sci_DataCombine_Rx(Uint16 *p_Rx);
	interrupt void 	ScibTxIsr(void);
	interrupt void 	ScibRxIsr(void);
	extern	 SCIB_REGS  SCIB;
	extern	 Uint16	SCI_RX_BUFFER[RX_BUFFER_LENGTH];	// ´®¿Ú½ÓÊÕ»º³åÇø
	extern	 Uint16	SCI_RX_ALLBUFFER[RX_ALLBUFFER_LENGTH];	// ´®¿Ú½ÓÊÕ»º³åÇø
	extern	 Uint16	SCI_TX_BUFFER[TX_BUFFER_LENGTH];	// ´®¿Ú·¢ËÍ»º³åÇø
	extern	 _iq	SCI_ReceData[30];		//´®¿Ú½ÓÊÕµÄÊı¾İ
	extern 	 _iq	SCI_SendData[30];		//´®¿Ú·¢ËÍµÄÊı¾İ	
	extern volatile Uint32 Time_Cal;


//======================================================================================================


	extern	 _iq30 *pArcSin;		      				// Ö¸Ïò·´ÕıÏÒº¯Êı±í£¬Q30
	
	extern Uint16	EEPROM_BUFFER[RX_BUFFER_LENGTH];// EEPROMÊı¾İ»º³åÇø
	extern volatile	Uint16		*p_EEProm_Buffer;
	extern Uint16	RX_BUFFER[RX_BUFFER_LENGTH];	// ´®¿Ú½ÓÊÕ»º³åÇø
	extern Uint16	TX_BUFFER[TX_BUFFER_LENGTH];	// ´®¿Ú·¢ËÍ»º³åÇø
	extern _iq	    ReceData[30];		            //´®¿Ú½ÓÊÕµÄÊı¾İ
	extern _iq	    SendData[30];		            //´®¿Ú·¢ËÍµÄÊı¾İ

	extern Uint16	CAN_BUFFER_RX[8];	//CAN»º³åÇø
	extern Uint16	CAN_BUFFER_RT[6][17];	//CAN»º³åÇø
	extern Uint16	CAN_BUFFER_TX[52];	//CAN»º³åÇø

	extern Uint16	CAN_BUFFER_CTRL[8];	//CAN»º³åÇø
	extern Uint16	CAN_BUFFER_CTRL_N[6][8];
	extern Uint16	CAN_BUFFER_CTRL_N_index;
	extern Uint16	CAN_BUFFER_CTRL_index;
	extern Uint16	CAN_BUFFER_CTRL_Num;
	extern Uint16	CAN_CHANGE_CTRL[8];	//CAN»º³åÇø
	extern Uint16	CAN_SET_VALUE[8];	//CAN»º³åÇø
	extern Uint16	CAN_CONTROL;	//¿ØÖÆ×´Ì¬

		
    extern volatile struct ECAN_REGS    ECanaShadow;

	extern  interrupt void  ECana_T(void);
	extern	void Can_Deal(void);
	extern	void Cmd_Deal(void);
	extern	void Can_Receive(void);
	extern	void Can_Send(void);

	extern CAN_REGS  CANB;
	extern CAN_REGS  CANB2;
	extern volatile 	Uint16  CAN_Index;
	extern	volatile 	Uint16  CanStop_Time;
	extern volatile 	Uint16	Choose_Mche;			//Ñ¡Ôñµç»ú
	extern volatile 	Uint16	Choose_Mode;			//Ñ¡ÔñÄ£Ê½

	extern volatile 	Uint16	Board_Num;			//¿ØÖÆ°å

	extern volatile 	Uint16	Board2_Num;			//¿ØÖÆ°å

	extern 	 RDC_DRIVER rdc1;
	extern 	 RDC_DRIVER rdc2;

	extern	 PWMGEN		pwma;	

	extern	 Ref_ABCtoDQ abctodq;			// ABC->DQ
	extern 	 Ref_DQtoABC dqtoabc;			// DQ->ABC
	extern 	 Ref_THREE2TWO phase_trans;

	extern 	 PIREG pi_Pos;			// Î»ÖÃ»·µÄPI
	extern	 PIREG	 pi_velo;				// ËÙ¶ÈPI
	extern  PIREG          pi_Id_Kc;			// IdPI
	extern  PIREG          pi_Iq_Kc;			// IqPI
	extern  PIREG pi_usLimit;
	extern  volatile _iq		Idq_Kc;
	extern  volatile _iq		Radq;


	extern	volatile  	Uint32	VeloProtect1;				// QG ±£»¤ÓÃIq±ê×Ó
	extern	volatile    Uint32	IdProtect1;					// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	UdcOverProtect1;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	IdcOverProtect1;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile   	_iq		Is;						// QG Id±êçÛÖµ
	extern	volatile 	_iq    	Vel_Speed;

	extern _iq 	m_PT_V[31];
	extern _iq  m_PT_T[31];

	extern volatile Uint16		Control_Period;	// Q16 ¿ØÖÆÖÜÆÚ¼ÆÊıÖµ
	extern volatile Uint16		Deadtime;		// Q16 ËÀÇø¼ÆÊıÖµ
	extern volatile _iq		Velo_Duty;				// QG ×ªËÙÔØ²¨ÏµÊı
	extern volatile _iq		Velo_Duty0;				// QG ×ªËÙÔØ²¨ÏµÊı
	extern	volatile   	_iq    	MotorTM;
	extern	volatile	_iq		IgbtTM;


//======================================================================================================
/*  Ö÷ÒªÏà¹ØÈ«¾Ö±äÁ¿µÄ¶¨Òå		*/
//======================================================================================================
		//------------------------------------------------------
	// ÆµÂÊ¡¢ËÙ¶ÈÒÔ¼°½Ç¶È
	extern	volatile Uint16		Angle_Q16;					// Q16 Ğı±ä²âÈ¡µÄµ±Ç°Î»ÖÃ½ÇHEX
	extern	volatile Uint16		Angle0_Q16;					// Q16 Ğı±ä²âÈ¡µÄÉÏÒ»´ÎÎ»ÖÃ½ÇHEX
	extern	volatile Uint16	    Angle_Init_Digital;      	// Q0 ÓÃÓÚĞı±ä²âÁ¿µÄ×ª×Ó³õÎ»ÖÃ½Ç(HEX)
	extern	volatile Uint16	    Angle_Init_Fa;	// Q0 ÓÃÓÚĞı±ä²âÁ¿µÄ×ª×Ó³õÎ»ÖÃ½Ç(HEX)
	extern	volatile Uint16		Angle_Fa_Q16;					// Q16 Ğı±ä²âÈ¡µÄµ±Ç°Î»ÖÃ½ÇHEX


	extern	volatile _iq		Angle_ElecOpen;				// QG ×ª×ÓÎ»ÖÃµç½ÇµÄ±êçÛÖµ
	extern	volatile _iq		Angle_Elec;					// QG ×ª×ÓÎ»ÖÃµç½ÇµÄ±êçÛÖµ
	extern  volatile _iq        Com_angle_I;                //¼ÆËãdqÖáµçÁ÷Ê±µÄ²¹³¥½Ç
	extern  volatile _iq        Com_angle_U;               //¼ÆËãdqÖáµçÑ¹Ê±µÄ²¹³¥½Ç


	extern	volatile _iq		Velo_Elec;					// QG ¼ì²âµÃµ½µÄ×ª×ÓËÙ¶ÈµÄÊµ¼ÊÖµ
	extern	volatile _iq		Velo_Elec_Cal;				// QG ¼ì²âµÃµ½µÄ×ª×ÓËÙ¶ÈµÄ¼ÆËãÖµ
	extern	volatile _iq		Velo_Set;                  	// QG ¸ø¶¨µÄ×ª×ÓËÙ¶ÈµÄ½ÇÆµÂÊ±íÊ¾
	extern	volatile _iq		Pos_Set;                  	 // QG ¸ø¶¨µÄÎ»ÖÃ

	extern	volatile Uint16	 	Run_Led_Index;				// ÔËĞĞµÆÔËĞĞµÄ¿ØÆÖÜÆÚµÄÖ¸Êı

	//----------------------------------------------------------
	// ÓÃÓÚ´æ´¢Ö±½ÓADC²ÉÑùµÄµ½Êı×ÖÁ¿,ĞèÒª½øĞĞĞ£Õı

 	extern	volatile 	Uint16	ADc_Isa;
 	extern	volatile 	Uint16	ADc_Isb;
 	extern	volatile 	Uint16	ADc_Isc;
 	extern	volatile 	Uint16	ADc_Idc;
 	extern	volatile 	Uint16	ADc_Udc;
 	extern	volatile 	Uint16	ADc_Temp0;
 	extern	volatile 	Uint16	ADc_Temp2;
 	extern	volatile 	Uint16	ADc_Temp4;
 	//------------------------------------------------------------


	// µç»ú¶¨×ÓµçÑ¹¬µçÁ÷µÄ±êçÛ
 	extern	volatile   	_iq		Isa;					// QG IsaµÄ±êçÛµ
 	extern	volatile   	_iq		Isb;					// QG IsbµÄ±êçÛÖµ
 	extern	volatile   	_iq		Isc;					// QG IscµÄ±êçÛÖµ
 	extern	volatile   	_iq		Usa;					// QG Usa±êçÛÖµ
 	extern	volatile   	_iq		Usb;					// QG UsbµÄÛÖµ
 	extern	volatile   	_iq		Usc;					// QG UscµÄ±êçÛ
 	extern	volatile   	_iq    	Udc_Mche;				// QG Ö±Á÷²àµçÑ¹µÄµ¹ı¨ÉÏÎ»»ú¸øµÄ£©
 	extern	volatile   	_iq     Udc_Mche_realvalue;     ///QG Ö±Á÷²àµçÑ¹µÄÊµ¼ÊÖµ£¨ÉÏÎ»»ú¸øµÄ£©

 	extern	volatile   	_iq    	Udc_Mche_1;				// QG Ö±Á÷²àµçÑ¹µÄ²âÖµ
 	extern	volatile   	_iq    	Udc_Mche0;				// QG Ö±Á÷²àµçÑ¹µÄ²âµ

   	extern	volatile 	_iq		Idc;					// QG IdcµÄ±êçÛÖµ
   	extern	volatile 	_iq		Idc0;					// QG IdcµÄ±êçÛÖµ

 	extern	volatile   	_iq		Id;						// QG Id±êçÛÖµ
 	extern	volatile   	_iq     Iq;						// QG Iq±êçÛÖµ
   	extern	volatile   	_iq		Id_Set;					// QG IdÉè¶¨µÄ±êçÛÖµ
 	extern	volatile   	_iq     Iq_Set;					// QG IqÉè¶¨µÄ±êçÛÖµ

   	extern	volatile   	_iq    	Ud;						// QG ¿ØÖÆµÃ³öµÄUd
 	extern	volatile   	_iq		Uq;						// QG ¿ØÖÆµÃ³öµÄUq
 	extern	volatile   	_iq		Us_Max;					// QG Ud£¬UqµÄÏŞù k*0.612Udc (ºã¹¦ÂÊä»?
 	extern	volatile   	_iq		Us_Alfa_Mche;			// QG aÖáµçÑ¹µÄ±êçÛÖµ
	extern	volatile   	_iq		Us_Beta_Mche;			// QG bÖáµçÑ¹µÄ±êçÛÖµ

 	//------------------------------------------------------------
	extern	volatile    Uint16	IdProtect;					// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile  	Uint16	VeloProtect;				// QG ±£»¤ÓÃIq±ê×Ó
	extern	volatile    Uint16	IdProtectNum;					// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile  	Uint16	VeloProtectNum;				// QG ±£»¤ÓÃIq±ê×Ó
	extern	volatile    Uint16	UdcLowProtect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	UdcLowEnable;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint16	UdcOverProtect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint16	IdcOverProtect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint16	Temp1OverProtect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint16	Temp2OverProtect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	Temp1OverProtect1;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	Temp2OverProtect1;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	IGBTProtect1;			// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint16	RdcProtect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	RdcProtect1;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint16	Rdc2Protect;				// QG ±£»¤ÓÃId±ê×Ó
	extern	volatile    Uint32	Rdc2Protect1;				// QG ±£»¤ÓÃId±ê×Ó

	extern	volatile    Uint32	ProtectNum;		// QG ¹ÊÕÏ»Ö¸´Ê±¼ä


	//------------------------------------------------------------
	 	// ÓÃÓÚÂË²¨³ÌĞòµÄÁ¿
	 	extern	volatile   	_iq		Id0;					// QG ÉÏ´ÎµÄId±êçÛÖµ
	 	extern	volatile   	_iq     Iq0;					// QG ÉÏ´ÎµÄIq±êçÛÖµ
	 	extern	volatile   	_iq		Velo_Elec0;				// QG ÉÏ´Î¼ÆËãµÄµçËÙ¶ÈµÄ±êçÛÖµ

		//------------------------------------------------------------
	    //----------------------------------------

	 	// µç»ú²ÎÊı
	 	extern	volatile _iq  		Ld;			// µç»údÖáµç¸ĞµÄ±êçÛÖµ
		extern	volatile _iq  		Lq;			// µç»úqÖáµç¸ĞµÄ±êçÛÖµ
		extern	volatile _iq  		PHI;		// µç»ú´ÅÁ´µÄ±êçÛÖµ
		extern	volatile _iq  		Rs;			// µç»úµç×èµÄ±êçÛÖµ
		extern	volatile _iq  		Mp;			// µç»úµÄ¼«¶ÔÊı
		extern	volatile _iq  		Rp;			// µç»úµÄ¼«¶ÔÊı
		extern	volatile _iq  		p;			// µç»úÓëĞı±äµÄ¼«¶ÔÊı±È
		// »ùÖµÏµÍ³
	 	extern	volatile 	_iq		U_Base;					// µçÑ¹»ùÖµ
	 	extern	volatile 	_iq		I_Base;					// µçÁ÷»ùÖµ
	 	extern	volatile 	_iq		V_Base;					// ËÙ¶È»ùÖµ
	 	extern	volatile 	_iq		K_Velo_Cal;				// ËÙ¶È¼ÆËã²ÎÊı


// Ö÷ÒªÏà¹ØÈ«¾Ö±äÁ¿µÄ¶¨Òå½áÊø
//======================================================================================================
//----------------------------------------------------------


//----------------------------------------------------------
		extern volatile union FAULT_FLAG_REG	Fault_Flag;
		extern volatile union SYS_FLAG_REG	Sys_Flag;
		extern volatile union CTRL_FLAG_REG	Ctrl_Flag;
//====================================================================================

/*id¸úËæ±äÁ¿¶¨Òå*/
		extern volatile _iq id_Max;
		extern volatile _iq idc_Max;
		extern volatile _iq idSet_Max;

/*ÂË²¨½ØÖ¹ÆµÂÊ±äÁ¿¶¨Òå*/
		extern volatile _iq id_Filter;
		extern volatile _iq Velo_Filter;
		extern volatile _iq Udc_Filter;
//------------------------------------------------------------------------------------

		extern	volatile 	_iq    	Velo_Max;

			extern  volatile 	_iq 	Usa_Lock;
			extern  volatile 	_iq		Usb_Lock;
		 	extern  volatile 	_iq 	Usc_Lock;
			extern  volatile 	_iq		Udc_Lock;
		 	extern  volatile 	_iq 	Um_OpenLoop;
			extern  volatile 	_iq		Freq_OpenLoop;
			extern  volatile 	_iq		Udc_OpenLoop;

			extern	volatile 	_iq17 	Off_Isc;
			extern	volatile 	_iq17 	K_Isc;
			extern	volatile 	_iq17 	Off_Isb;
			extern	volatile 	_iq17 	K_Isb;
			extern	volatile 	_iq17 	Off_Isa;
			extern	volatile 	_iq17 	K_Isa;
			extern	volatile 	_iq17 	Off_Udc;
			extern	volatile 	_iq17 	K_Udc;
			extern	volatile 	_iq17 	Off_Idc;
			extern	volatile 	_iq17 	K_Idc;

			extern	volatile 	_iq		Us;					// QG Us=mag(ud,uq)µÄ±êçÛÖµ
		 	extern	volatile 	_iq    	Udc_Setg;			// QG Ö±Á÷²àµçÑ¹µÄ²âÁ¿Öµ
			extern	volatile 	_iq    	I_Add;
			extern	volatile 	_iq    	Velo_Start;

			extern	volatile 	_iq    	Isdq_Set;
			extern  volatile 	_iq    	Isd_Set;      //ÉÏÎ»»ú·¢ËÍ¹ıÀ´µÄid;
			extern	volatile 	_iq    	Isdq_Set1;
			extern	volatile 	_iq    	Isdq_Set2;

			extern	volatile 	_iq    	IstoTorque;
			extern	volatile 	_iq    	TorquetoIs;
			extern	volatile 	_iq    	TorqueAngle;//30¶È
			extern	volatile 	_iq    	TorqueAngleA;//0¶È
			extern	volatile 	_iq    	Is_Add;
			extern	volatile 	_iq    	Is_PerAdd;
			extern	volatile 	_iq    	Veloover_Set;

			extern volatile _iq setSpeed; /* ÀîÊ¢ 20241007ĞÂÔö */
            extern volatile _iq prevSpeed;

			extern	volatile 	_iq    	Velo_Add;
			extern	volatile 	_iq    	Velo_PerAdd;
		 	extern	volatile 	_iq		Per_Ctrl;	// ¿ØÖÆÖÜÆÚ

			extern volatile   	_iq    	PWMA_a;						// QG ¿ØÖÆµÃ³öµÄUd
			extern volatile 	_iq		PWMA_b;						// QG ¿ØÖÆµÃ³öµÄUq
			extern volatile 	_iq		PWMA_c;						// QG ¿ØÖÆµÃ³öµÄUq
			extern  volatile    _iq		Velo_Elec_abs;				// QG µçËÙ¶ÈÄ±êçÛÖ

			extern volatile	    _iq 	Temperature1;		//
			extern volatile	    _iq 	Temperature3;		//
			extern volatile	    _iq 	Temperature5;		//
			extern volatile     _iq 	Temperature10;		//
			extern volatile	    _iq 	Temperature30;		//
			extern volatile	    _iq 	Temperature50;		//
			extern volatile	    _iq 	Temperature1_Off;		//
			extern volatile	    _iq 	Temperature3_Off;		//
			extern volatile	    _iq 	Temperature5_Off;		//
			extern volatile	    _iq 	Temperature1_K;		//
			extern volatile 	_iq 	Temperature3_K;		//
			extern volatile	    _iq 	Temperature5_K;		//

			extern volatile 	long	Velo_Calc_Num;//ËÙ¶ÈÆ½¾ù´ÎÊı
			extern volatile 	Uint16	Velo_Avg_Index;//ËÙ¶ÈÆ½¾ùÖ¸Êı
			extern volatile		_iq		Velotmp;
			extern volatile		Uint16	Velo_Calc_flag;
			extern	volatile   	_iq    	UdcLimit_Set;				// QG Ö±Á÷²àµçÑ¹µÄ²âÖµ
			extern	volatile   	_iq    	UdcLimit1_Set;				// QG Ö±Á÷²àµçÑ¹µÄ²âÖµ
			extern	volatile	_iq		IdSet;
			extern	volatile	_iq		UsSet;
		   	extern	volatile   	_iq		IdZ_Set;					// QG IdÉè¶¨µÄ±êçÛÖµ
		 	extern	volatile   	_iq     IqZ_Set;					// QG IqÉè¶¨µÄ±êçÛÖµ

			extern	volatile 	_iq    	Is_DownAdd;
			extern	volatile 	_iq    	Is_DownPerAdd;
			extern	volatile 	_iq    	Temp_Filter;
			extern	volatile 	_iq    	IsSet_Filter;
			extern	volatile   	_iq    	IsLimit_Set;
			extern	volatile 	_iq 	Us_M;

			//====================================================================================


		
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    //APP_USERDEF_H ¶¨Òå½áÊø
//===========================================================================
// End of file.
//===========================================================================


