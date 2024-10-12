/*
//==================================================================================
//
// ÎÄ¼şÃû:    App_TypeDef.h                     
//                   
// ¹¦ÄÜÃèÊö:  ËùÓĞĞÂµÄ½á¹¹ÀàĞÍ¶¨Òå,¶¨ÒåÀàĞÍºóÔÚApp_Main.hÖĞ¶¨Òå´ËÀàĞÍµÄ±äÁ¿
//			  	
// 
//±àĞ´Õß:    ÎéÔó¶«
//±àĞ´ÈÕÆÚ:  2011
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
/* F2812µÄPWMÇı¶¯Æ÷½á¹¹¶¨Òå		*/
//======================================================================================================
typedef struct
{	volatile Uint32	mHalfPeriod;		// ÊäÈë: »ùÓÚHISPCPÊ±ÖÓ·ÖÆµºóµÄPWMÖÜÆÚµÄÒ»°ë(Q0)  	
	volatile _iq		mDuty1;			// ÊäÈë: PWM 1&2 Õ¼¿Õ±È (QG)       		
	volatile _iq		mDuty2;			// ÊäÈë: PWM 3&4 Õ¼¿Õ±È (QG)       		
	volatile _iq		mDuty3;			// ÊäÈë: PWM 5&6 Õ¼¿Õ±È (QG)       		       
}PWMGEN;
//------------------------------------------------------------------------------
// Ä¬ÈÏµÄF2812 PWMGEN 
// -----------------------------------------------------------------------------
#define PWMAGEN_DEFAULTS {	CARRY_A_P,HALFA_DUTY,HALFA_DUTY, HALFA_DUTY }
//------------------------------------------------------------------------------
// º¯ÊıÔ­ĞÍÔÚ  App_EV.C
//------------------------------------------------------------------------------
// F2812µÄPWMÇı¶¯Æ÷½á¹¹¶¨Òå½áÊø
//======================================================================================================




//======================================================================================================
// F2812µÄRDC½á¹¹¶¨Òå
//======================================================================================================
typedef	struct  
{     
	volatile Uint16	 FAULT;		    	// ³ö´í±êÖ¾ (1ÎªÓĞ´í,0Î»ÎŞ´í)
    volatile Uint16  DATA_BUFFER;   	// ¶ÁÆğÊı¾İµÄ»º³å
}
RDC_DRIVER ;
//------------------------------------------------------------------------------
// Ä¬ÈÏµÄRDC_READ1 RDC_READ2
// ------------------------------------------------------------------------------
#define RDC_READ_DEFAULT   { 0,0 }

//-----------------------------------------------------------------------------
//º¯ÊıÔ­ĞÍ
//-----------------------------------------------------------------------------
void RDC_READ (void);

// F2812µÄRDC½á¹¹¶¨Òå½áÊø
//======================================================================================================
				


//======================================================================================================
/* ×ø±êÏµ±ä»»½á¹¹¶¨Òå      */
//======================================================================================================
//------------------------------------------------------------------------------
// ¾²Ö¹abcÈıÏà×ø±êÏµ±ä»»µ½Í¬²½dq×ø±êÏµ
//------------------------------------------------------------------------------
typedef struct 
{		
  		volatile  _iq  as;  		// ÊäÈë: aÏà±äÁ¿ 
		volatile  _iq  bs;			// ÊäÈë: bÏà±äÁ¿ 
		volatile  _iq  cs;			// ÊäÈë: cÏà±äÁ¿ 
		volatile  _iq  ang;			// ÊäÈë: µç½Ç¶È (pu) 
		volatile  _iq  dr;			// Êä³ö: Í¬²½×ø±êÏµdÖá±äÁ¿ 
		volatile  _iq  qr;			// Êä³ö: Í¬²½×ø±êÏµqÖá±äÁ¿ 
} 
Ref_ABCtoDQ;	            
                  
#define Ref_ABCtoDQ_DEFAULTS { 0,0,0,0,0,0 } // ¶¨Òå³õÊ¼»¯µÄRef_ABCtoDQ
void ABC2DQ_CALC (void);						 // º¯ÊıÔ­ĞÍ
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Í¬²½dq×ø±êÏµ±ä»»µ½¾²Ö¹abc×ø±êÏµ
//------------------------------------------------------------------------------
typedef struct 
{  		
	volatile _iq  as;  		// Êä³ö: aÏà±äÁ¿ 
	volatile _iq  bs;		// Êä³ö: bÏà±äÁ¿ 
	volatile _iq  cs;		// Êä³ö: cÏà±äÁ¿ 
	volatile _iq  ang;		// ÊäÈë: µç½Ç¶È (pu) 
	volatile _iq  dr;		// ÊäÈë: Í¬²½×ø±êÏµdÖá±äÁ¿ 
	volatile _iq  qr;		// ÊäÈë: Í¬²½×ø±êÏµqÖá±äÁ¿ 
} 
Ref_DQtoABC;	            
    
#define Ref_DQtoABC_DEFAULTS { 0,0,0,0,0,0 } // ¶¨Òå³õÊ¼»¯µÄRef_DQtoABC
void DQ2ABC_CALC (void);						 // º¯ÊıÔ­ĞÎ
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ¾²Ö¹abc×ø±êÏµ±ä»»µ½¾²Ö¹µÄ2Ïà¦Á£¬¦Â
//------------------------------------------------------------------------------
typedef struct 
{ 		
	volatile _iq  as;  		// ÊäÈë: aÏà±äÁ¿ 
	volatile _iq  bs;		// ÊäÈë: bÏà±äÁ¿
	volatile _iq  cs;		// ÊäÈë: cÏà±äÁ¿
	volatile _iq  ds;		// Êä³ö: ds±äÁ¿
	volatile _iq  qs;		// Êä³ö: qs±äÁ¿
} 
Ref_THREE2TWO;
	
#define Ref_THREE2TWO_DEFAULTS { 0,0,0,0,0 } // ¶¨Òå³õÊ¼»¯µÄRef_THREE2TWO
void THREE2TWO_CALC (void);						 // º¯ÊıÔ­ĞÎ
//-----------------------------------------------------------------------------

// ×ø±êÏµ±ä»»½á¹¹¶¨Òå½áÊø
//======================================================================================================


//====================================================================================
/* PIµ÷½ÚÆ÷Ïà¹Ø½á¹¹Ìå¶¨Òå		*/
//=====================================================================================
//------------------------------------------------------------------------------- 
// PI½á¹¹ÌåµÄ¶¨Òå
//-------------------------------------------------------------------------------
typedef struct 
{  
		volatile _iq  pi_ref_reg;   // ÊäÈë: ¸ø¶¨ÊäÈë
		volatile _iq  pi_fdb_reg;  	// ÊäÈë: ·´À¡ÊäÈë
	    volatile _iq  e_reg;		// ±äÁ¿: Îó²î
		volatile int32 loop_index;		
		volatile int32 i_period;		// ²ÎÊı: »ı·ÖÖÜÆÚ
		volatile _iq  Kp_reg;		// ²ÎÊı: ±ÈÀıÏµÊı
		volatile _iq  Ki_reg;		// ²ÎÊı: »ı·ÖÏµÊı
		volatile _iq  ui_out_max;	// ²ÎÊı: »ı·ÖÊä³öÉÏÏŞ·ù
		volatile _iq  ui_out_min;	// ²ÎÊı: »ı·ÖÊä³öÏÂÏŞ·ù
		volatile _iq  pi_out_max;	// ²ÎÊı: PIÊä³öÉÏÏŞ·ù
		volatile _iq  pi_out_min;	// ²ÎÊı: PIÊä³öÏÂÏŞ·ù
		volatile _iq  up_reg;		// ±äÁ¿: ±ÈÀıÊä³ö
		volatile _iq  ui_reg;		// ±äÁ¿: »ı·ÖÊä³ö
		volatile _iq  pi_out_reg;   // Êä³ö: PIÊä³ö
		volatile int32 ctrl_period;  // ²ÎÊı: ¿ØÖÆÖÜÆÚ
		volatile int32 ctrl_index;		// ±äÁ¿: ¿ØÖÆÖÜÆÚÖ¸Êı	
					  
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
// PI½á¹¹Ìå¶¨Òå½áÊø
//====================================================================================


//====================================================================================
// SCI½á¹¹ÌåµÄ¶¨Òå
//----------------------------------------------------------------------
// SCIÏà¹Ø²Ù×÷µÄµÄÎ»¶¨Òå 
struct	SCI_FLAG_BITS
{
	volatile Uint16	RX_FLAG:1;		// "1"±íÃ÷½ÓÊÕÓĞĞ§±êÖ¾
	volatile Uint16	TXBO_FLAG:1;	// ·¢ËÍÊı¾İ¿éÍê³É±êÖ¾
	volatile Uint16	RXTIME_FLAG:1;	// 1±íÊ¾ÕıÔÚ½âÎöÖ¸Áî
	volatile Uint16	PC_FLAG:1;		// 1±íÊ¾ÉÏÎ»»ú¿ØÖÆÄ£Ê½
	volatile Uint16	rsvd1:12;
};
union	SCI_FLAG_REG
{
		volatile Uint16	all;
		volatile struct	SCI_FLAG_BITS	bit;
};
//------------------------------------------------------------------------------
// SCI½á¹¹Ìå¶¨Òå
//-----------------------------------------------------------------------
typedef	struct
{
		volatile union		SCI_FLAG_REG	FLAG;	// SCIµÄ¸÷±êÖ¾
		volatile Uint16		m_Rx_Length;			// ½ÓÊÕÊı¾İ³¤¶È
		volatile Uint16		m_Tx_Length;			// ·¢ËÍÊı¾İ³¤¶È
		volatile Uint16		*p_Tx_Buffer;			// ·¢ËÍÊı¾İ»º³å
		volatile Uint16		*p_Rx_Buffer;			// ½ÓÊÕÊı¾İ»º³å
		volatile Uint16		*p_Rx_AllBuffer;		// È«²¿½ÓÊÕÊı¾İ»º³å
}		volatile SCIB_REGS;
//------------------------------------------------------------------------------
// Ä¬ÈÏµÄSCIB_REGS
//------------------------------------------------------------------------------
#define		SCIB_REGS_DEFAULTS	{{2},0,0,SCI_TX_BUFFER,SCI_RX_BUFFER,SCI_RX_ALLBUFFER}
// SCI½á¹¹¶¨Òå½áÊø
//======================================================================================


//====================================================================================
// CAN½á¹¹ÌåµÄ¶¨Òå
//----------------------------------------------------------------------
// CANÏà¹Ø²Ù×÷µÄµÄÎ»¶¨Òå
struct	CAN_FLAG_BITS
{
		volatile	Uint16	RX_FLAG:1;			// "1"±íÃ÷½ÓÊÕÓĞĞ§±êÖ¾
		volatile	Uint16	TXBO_FLAG:1;		// ·¢ËÍÊı¾İ¿éÍê³É±êÖ¾
		volatile	Uint16	TIMETX_FLAG:1;		// ¶¨Ê±·¢ËÍ
		volatile	Uint16	DIAGTX_FLAG:1;		// ¶¯Ì¬·¢ËÍ(°´Ğè·¢ËÍ)
		volatile	Uint16	rsvd1:12;
};
union	CAN_FLAG_REG
{
		volatile	Uint16	all;
		struct	CAN_FLAG_BITS	bit;
};
//------------------------------------------------------------------------------
// CAN½á¹¹Ìå¶¨Òå
//-----------------------------------------------------------------------
typedef	struct
{
		union		CAN_FLAG_REG	FLAG;		// SCIµÄ¸÷±êÖ¾
		volatile	Uint16		m_Rx_Length;				// ½ÓÊÕÊı¾İ³¤È
		volatile	Uint16		m_Tx_Length;				// ·¢ËÍÊı¾İ³¤¶È
		volatile	Uint16		*p_Tx_Buffer;				// ½ÓÊÕÊı¾İ»º³å
		volatile	Uint16		*p_Rx_Buffer;				// ·¢ËÍÊı¾İ»º³å
		volatile	Uint16		m_Time_Index;				// ¶¨Ê±Ö¸Õë
		volatile	Uint16		m_Time_Length;				// ¶¨Ê±³¤¶È
}		CAN_REGS;
//------------------------------------------------------------------------------
// Ä¬ÈÏµÄCAN_REGS
//------------------------------------------------------------------------------
#define		CAN_REGS_DEFAULTS	{2,0,0,TX_BUFFER,RX_BUFFER,0,500}


#define		CAN2_REGS_DEFAULTS	{2,0,0,SCI_RX_BUFFER,SCI_TX_BUFFER,0,500}



//======================================================================================================
/* ¹ÊÕÏ±êÖ¾Î»½á¹¹¶¨Òå		*/
//======================================================================================================
struct FAULT_FLAG_BITS
{
	volatile	Uint16 STOP_VELO_FLAG:1;		//"1"±íÊ¾Í£Ö¹ËÙ¶È¿ØÖÆ³ÌĞò
	volatile	Uint16 STOP_PWM_Flag:1;    	//¹ÊÕÏÍ£Ö¹
	volatile	Uint16 STOP_PWM_Flag_Id:1;		// "1"±íÃ÷µç»ú¹ıÁ÷±£»¤
	volatile	Uint16 STOP_PWM_Flag_Velo:1;	// "1"±íÃ÷³¬ËÙ±£»¤
	volatile	Uint16 STOP_PWM_Flag_Driv:1;	// "1"±íÃ÷PWMA¹¦ÂÊ±£»¤
	volatile	Uint16 UDC_FLAG:1;				// "1"±íÃ÷Ö±Á÷µçÑ¹¹ÊÕÏ
	volatile	Uint16 speedFluctuation:1;//µç»ú¹ıÎÂ1
	volatile	Uint16 Temp2Over_Flag:1;//Ä£¿é¹ıÎÂ
	volatile	Uint16 UdcLow_Flag:1;//Ö±Á÷Ç·Ñ¹
	volatile	Uint16 CANERROR_FLAG:1;//Í¨ĞÅ¹ÊÕÏ
	volatile	Uint16 RDC1_FAULT:1;
	volatile	Uint16 RDC2_FAULT:1;
	volatile	Uint16 POS_LIMIT:1;
	volatile	Uint16 RunAllow_Flag:1;
	volatile	Uint16 rsvd1:2;				// ±£Áô
};
union FAULT_FLAG_REG
{	volatile	Uint16	all;
	volatile	struct	FAULT_FLAG_BITS bit;
};
//=======================================================================================
// Ä¬ÈÏµÄ¹ÊÕÏ±êÖ¾Î»
//=======================================================================================
#define FAULT_FLAG_DEFAULT  {0x0};
// ¹ÊÕÏ±êÖ¾Î»½á¹¹Ìå¶¨Òå½áÊø
//====================================================================================



//======================================================================================================
/* ÏµÍ³±êÖ¾Î»½á¹¹¶¨Òå		*/
//======================================================================================================
struct SYS_FLAG_BITS
{
	volatile	Uint16 STOP_PWM_Flag_Id:1;		// "1"±íÃ÷µç»ú¹ıÁ÷±£»¤
	volatile	Uint16 Flag_Id:1;				// "1"±íÃ÷dÖáµçÁ÷»·»ı·ÖÏŞ·ù
	volatile	Uint16 STOP_PWM_Flag_Velo:1;	// "1"±íÃ÷³¬ËÙ±£»¤
	volatile	Uint16 STOP_PWM_Flag_Driv:1;	// "1"±íÃ÷PWMA¹¦ÂÊ±£»¤
	volatile	Uint16 Flag_Iq:1;				// "1"±íÃ÷qÖáµçÁ÷»·»ı·ÖÏŞ·ù
	volatile	Uint16 Flag_Us:1;				// "1"±íÃ÷×ÜµçÑ¹ÏŞ·ù
	volatile	Uint16 UDC_FLAG:1;				// "1"±íÃ÷Ö±Á÷µçÑ¹¹ÊÕÏ
	volatile	Uint16 CAN_FLAG:1;				// "1"±íÃ÷´Ó½ÓÊÕµ½¿ØÖÆÖ¸Áî
	volatile	Uint16 RDC_FAULT:8;				// ±£Áô
};
union SYS_FLAG_REG
{	volatile	Uint16	all;
	volatile	struct	SYS_FLAG_BITS bit;
};
//=======================================================================================
// Ä¬ÈÏµÄÏµÍ³±êÖ¾Î»
//=======================================================================================
#define SYS_FLAG_DEFAULT  {0x0};
// ÏµÍ³±êÖ¾Î»½á¹¹Ìå¶¨Òå½áÊø
//====================================================================================


//=======================================================================================
struct CTRL_FLAG_BITS
{
	volatile	Uint16	speedFluctuation:1;//µç»ú¹ıÎÂ1
	volatile	Uint16	Temp2Over_Flag:1;//Ä£¿é¹ıÎÂ
	volatile	Uint16	UdcLow_Flag:1;//Ö±Á÷Ç·Ñ¹
	volatile	Uint16	CANERROR_FLAG:1;//Í¨ĞÅ¹ÊÕÏ
	volatile	Uint16	STOP_PWM_Flag:1;//¹ÊÕÏÍ£Ö¹
	volatile	Uint16	CAN_Ctrl:1;				
	volatile	Uint16	FIRST_PWM_Flag:1;//Ê×´Î½øÈë¿ØÖÆ
	volatile	Uint16	VELO_CONTROL_FLAG:1;	//"1"±íÊ¾½øÈëËÙ¶È¿ØÖÆ³ÌĞò
	volatile	Uint16	TORQ_CONTROL_FLAG:1;	//"1"±íÊ¾½øÈë×ª¾Ø¿ØÖÆ³ÌĞò
	volatile	Uint16	OPEN_LOOP_FLAG:1;       //Openloop  618ÁõÈĞ¸Ä
	volatile	Uint16	LOCK_FLAG:1;			//"1"±íÊ¾LOCK  618ÁõÈĞ¸Ä
	volatile	Uint16	UsCtrl_Flag:1;//×Ô¶¯Èõ´Å
	volatile	Uint16	STOP_VELO_FLAG:1;		//"1"±íÊ¾Í£Ö¹ËÙ¶È¿ØÖÆ³ÌĞò
	volatile	Uint16	POS_CONTROL_FLAG:1;//"1"±íÊ¾½øÈëÎ»ÖÃ¿ØÖÆ³ÌĞò
	volatile	Uint16	RotorWay_Flag:1;//Ğı±ä·½Ïò
	volatile	Uint16	VelWay_Flag:1;//×ªËÙ·½Ïò
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

#endif    //APP_TYPEDEF_H ¶¨Òå½áÊø
//===========================================================================
// End of file.
//===========================================================================
