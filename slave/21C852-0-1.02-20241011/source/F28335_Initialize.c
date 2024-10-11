/*
###########################################################################

 ÎÄ¼şÃû:	  F28335_Initialize.c

 ¹¦ÄÜÃèÊö:	
		 	  F28335 ³õÊ¼»¯µ½Ä¬ÈÏ×´Ì¬.Íê³ÉÏµÍ³µÄ³õÊ¼»¯ÒÔ¼°Íâ²¿Éè±¸µÄ³õÊ¼»¯

 ±àĞ´Õß:    ÁõÈĞ
 ±àĞ´ÈÕÆÚ:  2021-4-08
###########################################################################
*/

/*---------------------------------------------------------------------------*/
#include "F28335_Device.h"     /* F28335ÍâÉèÍ·ÎÄ¼ş*/
#include "App_UserDef.h"      /* F28335 Ó¦ÓÃÏîÄ¿Í·ÎÄ¼ş*/
/*---------------------------------------------------------------------------*/

/* ¸Ãº¯Êı¼ÓÔØÔÚÍâ²¿´æ´¢Çø,µ«È´ÔËĞĞÔÚÄÚ²¿RAM(²ì¿´CMDÎÄ¼şÖĞramfuncsµÄÅäÖÃ)*/
#pragma CODE_SECTION(InitXintf, "ramfuncs");
#pragma CODE_SECTION(InitFlash, "ramfuncs");

/*=========================================================================
 ¹¦ÄÜÃèÊö:	
		 	ÄÚ´æ¿½±´º¯Êı.
          Õâ¸öº¯Êı½«¿½±´Ö¸¶¨µØÖ·ÖĞµÄÄÚÈİµ½ÁíÍâÒ»¸öÖ¸¶¨µØÖ·
========================================================================*/
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr <= (SourceEndAddr - 0x0001))
    { 
       (*DestAddr) = (*SourceAddr);
	   DestAddr++;
	   SourceAddr++;
    }
    return;
}


/*#############################################################################
ÏµÍ³³õÊ¼»¯³ÌĞò
-----------------------------------------------------------------------------
 ³õÊ¼»¯ÏµÍ³¿ØÖÆ¼Ä´æÆ÷:
 - ¹Ø±Õ¿´ÃÅ¹·
 - ÉèÖÃPLLCRÊ¹SYSCLKOUTÆµÂÊºÏÊÊ
 - ÉèÖÃHISPCP¡¢LOSPCPºÏÊÊµÄpre-scaler 
 - Ê¹ÄÜÍâÉèÊ±ÖÓ */
void InitSysCtrl(void)
{

   /*¹Ø±Õ¿´ÃÅ¹·*/
   DisableDog();

   /*PLLCRÉèÖÃ*/
   InitPll(PLLCR_RATIO,VCOCLK_DIVSEL);

   /*³õÊ¼»¯Íâ²¿Ê±ÖÓ*/
   InitPeripheralClocks();
}

/*----------------------------------------------
 º¯ÊıÃû: ServiceDog: 
----------------------------------------------
 ¸Ãº¯Êı¸´Î»¿´ÃÅ¹·¶¨Ê±Æ÷.*/
void ServiceDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

/*----------------------------------------------
 º¯ÊıÃû: DisableDog: 
----------------------------------------------
 ¸Ãº¯Êı¹Ø±Õ¿´ÃÅ¹·¶¨Ê±Æ÷.*/
void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

/*-----------------------------------------------
 º¯ÊıÃû: InitPll: 
-----------------------------------------------
 ¸Ãº¯Êı³õÊ¼»¯ PLLCR ¼Ä´æÆ÷.*/
void InitPll(Uint16 val, Uint16 divsel)
{

   /* È·ÈÏPLLÃ»ÓĞ¹¤×÷ÔÚlimpÄ£Ê½ÏÂ*/
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
   {
      ESTOP0;
   }
	
   /* ¸Ä±äPLLCRÖ®Ç°±ØĞë½«DIVELÖÃ0.Íâ²¿¸´Î»Ê±£¬¸ÃÎ»ÖÃ0*/
   if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
   {
       EALLOW;
       SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
       EDIS;
   }

   /* ĞŞ¸Ä±¶ÆµÏµÊı*/
   if (SysCtrlRegs.PLLCR.bit.DIV != val)
   {

      EALLOW;
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;   /* ¸ÄĞ´PLLCRÖ®Ç°¹Ø±Õ¶ªÊ§Ê±ÖÓ¼ì²âÂß¼­*/
      SysCtrlRegs.PLLCR.bit.DIV = val;
      EDIS;

      DisableDog();                 /*¹Ø±Õ¿´ÃÅ¹·*/

      while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1){}    /*µÈ´ıPLLÎÈ¶¨*/

      EALLOW;
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
      EDIS;
    }

    if((divsel == 1)||(divsel == 2))
    {
      EALLOW;
      SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
      EDIS;
    }
    if(divsel == 3)
    {
      EALLOW;
      SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
      DELAY_US(50L);
      SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
      EDIS;
    }
}

/*---------------------------------------------
 º¯ÊıÃû: InitPeripheralClocks: 
---------------------------------------------
 ¸Ãº¯Êı³õÊ¼»¯ÍâÉèÊ±ÖÓ.Îª¼õÉÙ¹¦ºÄ,¹Ø±ÕÃ»ÓĞÊ¹ÓÃµÄÊÇÍâÉèÊ±ÖÓ¡£
 ×¢Òâ: Èç¹ûÄ³ÍâÉèÊ±ÖÓÃ»ÓĞÊ¹ÄÜ£¬Ôò²»ÄÜ¶Á»òÕßĞ´¸ÃÍâÉèµÄ¼Ä´æÆ÷*/
void InitPeripheralClocks(void)
{
   EALLOW;

    /* HISPCP/LOSPCP ·ÖÆµ¶¨±ê¼Ä´æÆ÷ÉèÖÃ¡£*/
   SysCtrlRegs.HISPCP.all = HISPCP_RATIO;
   SysCtrlRegs.LOSPCP.all = LOSPCP_RATIO;


   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    /* ADC*/

   /* IMPORTANT  ADC_calº¯Êı½«OTP´æ´¢Æ÷ÖĞµÄADC±ê¶¨Öµ¿½±´µ½ADCREFSELºÍADCOFFTRIM
	ÕâÁ½¸ö¼Ä´æÆ÷ÖĞ¡£ÔÚµ÷ÓÃ¸Ãº¯ÊıÖ®Ç°±ØĞëÊ¹ÄÜADCÊ±ÖÓ¡£*/

   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;   /* I2C*/
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   /* SCI-A*/
   SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   /* SCI-B*/
   SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;   /* SCI-C*/
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   /* SPI-A*/
   SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 0; /* McBSP-A*/
   SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 0; /* McBSP-B*/
   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;    /* eCAN-A*/
   SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=0;    /* eCAN-B*/

   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   /* Disable TBCLK within the ePWM*/
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;  /* ePWM1*/
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;  /* ePWM2*/
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;  /* ePWM3*/
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;  /* ePWM4*/
   SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;  /* ePWM5*/
   SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;  /* ePWM6*/
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   /* Enable TBCLK within the ePWM*/

   SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  /* eCAP3*/
   SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  /* eCAP4*/
   SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 0;  /* eCAP5*/
   SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 0;  /* eCAP6*/
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;  /* eCAP1*/
   SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;  /* eCAP2*/
   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;  /* eQEP1*/
   SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;  /* eQEP2*/

   SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; /* CPU Timer 0*/
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; /* CPU Timer 1*/
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; /* CPU Timer 2*/

   SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;       /* DMA Clock*/
   SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;     /* XTIMCLK*/
   SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    /* GPIO input clock*/

   EDIS;
}

/*---------------------------------------------
 º¯ÊıÃû: CsmUnlock: 
---------------------------------------------
Õâ¸öº¯Êı½âËøCSM£¬ÓÃ»§±ØĞëÓÃ0xFFFFÈ¡´úDSPµÄµ±Ç°ÃÜÂë£¬½âËø³É¹¦·µ»Ø1*/
Uint16 CsmUnlock(void)
{
    volatile Uint16 temp;

    /*µ±Ç°ÃÜÂëÔØÈëÃÜÂë¼Ä´æÆ÷£¬0xFFFFÎªĞéÄâÃÜÂë£¬ÓÃ»§±ØĞëÓÃËüÀ´Ìæ´úDSPµÄµ±Ç°ÃÜÂë*/
    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;
    /* Perform a dummy read of the password locations
     if they match the key values, the CSM will unlock*/
    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;
    /* Èç¹ûCSM½âËø£¬·µ»Ø³É¹¦£¬·ñÔòÊ§°Ü*/
    if (CsmRegs.CSMSCR.bit.SECURE == 0)
	{
		temp = STATUS_SUCCESS;
	}
    else 
	{
		temp = STATUS_FAIL;
	}
	return temp;
}

/*-----------------------------------------------------------------------------
ÏµÍ³²¿·Ö³õÊ¼»¯Íê³É
#############################################################################

#############################################################################
ÍâÉè³õÊ¼»¯³ÌĞò
-----------------------------------------------------------------------------
---------------------------------------------------------------------------
 ³õÊ¼»¯ÍâÉè:
---------------------------------------------------------------------------
 ¸Ãº¯Êı³õÊ¼»¯ÍâÉèµ½Ä¬ÈÏµÄ×´Ì¬.Ëüµ÷¸÷¸öÍâÉèµÄ³õÊ¼»¯º¯Êı¡£
 ¸Ãº¯ÊıÔÚÆô¶¯»òÕßÈí¼ş¸´Î»Ê±µ÷ÓÃ¡£*/

void InitPeripherals(void)
{

   /* ³õÊ¼»¯Íâ²¿½Ó¿Úµ½Ä¬ÈÏ×´Ì¬:*/
    InitXintf();
    
   /* ³õÊ¼»¯ÄÚ²¿FLASHµ½Ä¬ÈÏ×´Ì¬:*/
    InitFlash();  
    
   /* ³õÊ¼»¯ GPIO*/
    InitGpio();  
		 
   /* ³õÊ¼»¯CPU¶¨Ê±Æ÷µ½Ä¬ÈÏ×´Ì¬:*/
    InitCpuTimers();

   /* ³õÊ¼»¯EPwmµ½Ä¬ÈÏ×´Ì¬:*/
    InitEPwm();

   /* ³õÊ¼»¯ECapµ½Ä¬ÈÏ×´Ì¬:*/
    InitECap();

   /* ³õÊ¼»¯EQepµ½Ä¬ÈÏ×´Ì¬:*/
    InitEQep();

   /* ³õÊ¼»¯ ADC µ½Ä¬ÈÏ×´Ì¬:*/
    InitAdc();
	
   /* ³õÊ¼»¯ SCI µ½Ä¬ÈÏ×´Ì¬:*/
	InitSci();

   /* ³õÊ¼»¯ SPI ¬ÈÏ×´Ì¬:*/
    InitSpi();

   /* ³õÊ¼»¯ ECan µ½Ä¬ÈÏ×´Ì¬:*/
    InitECan();

   /* ³õÊ¼»¯Íâ²¿ÖĞ¶Ïµ½Ä¬ÈÏ×´Ì¬*/
    InitXIntrupt();

	Init_AD1210();
}

/*-------------------------------------------
 ¸Ãº¯Êı³õÊ¼»¯Íâ²¿½Ó¿ÚµÄ
 ĞŞ¸ÄXINTF¼Ä´æÆ÷Ê±,³ÌĞòÎñ±Ø²»ÄÜÔÚÍâ²¿ÔËĞĞ
-------------------------------------------*/

void InitXintf(void)
{
    /* All Zones---------------------------------
    Timing for all zones based on XTIMCLK = 1/2 SYSCLKOUT*/

    EALLOW;
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
    XintfRegs.XINTCNF2.bit.WRBUFF = 0;     /* No write buffering*/
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;     /* XCLKOUT is enabled*/
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;    /* XCLKOUT = XTIMCLK/2*/

    /* Zone 0------------------------------------*/
    /* Zone write timing*/
    XintfRegs.XTIMING0.bit.XWRLEAD = 3;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 3;
    /* Zone read timing*/
    XintfRegs.XTIMING0.bit.XRDLEAD = 3;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 3;
    /* double all Zone read/write lead/active/trail timing*/
    XintfRegs.XTIMING0.bit.X2TIMING = 1;
    /* Zone will sample XREADY signal*/
    XintfRegs.XTIMING0.bit.USEREADY = 0;
    XintfRegs.XTIMING0.bit.READYMODE = 1;  /* sample asynchronous*/
    /* Size must be either: 0,1 = x32 or 1,1 = x16 other values are reserved*/
    XintfRegs.XTIMING0.bit.XSIZE = 3;

    /* Zone 6------------------------------------*/
    /* Zone write timing*/
    XintfRegs.XTIMING6.bit.XWRLEAD = 3;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 3;
    /* Zone read timing*/
    XintfRegs.XTIMING6.bit.XRDLEAD = 3;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 3;
    /* double all Zone read/write lead/active/trail timing*/
    XintfRegs.XTIMING6.bit.X2TIMING = 1;
    /* Zone will sample XREADY signal*/
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.READYMODE = 1;  /* sample asynchronous*/
    /* Size must be either: 0,1 = x32 or 1,1 = x16 other values are reserved*/
    XintfRegs.XTIMING6.bit.XSIZE = 3;

    /* Zone 7------------------------------------*/
    /* Zone write timing*/
    XintfRegs.XTIMING7.bit.XWRLEAD = 3;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 3;
    /* Zone read timing*/
    XintfRegs.XTIMING7.bit.XRDLEAD = 3;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 3;
    /* double all Zone read/write lead/active/trail timing*/
    XintfRegs.XTIMING7.bit.X2TIMING = 1;
    /* Zone will sample XREADY signal*/
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 1;  /* sample asynchronous*/
    /* Size must be either: 0,1 = x32 or 1,1 = x16 other values are reserved*/
    XintfRegs.XTIMING7.bit.XSIZE = 3;

    XintfRegs.XBANK.bit.BANK = 7;
    XintfRegs.XBANK.bit.BCYC = 7;
    EDIS;

    asm(" RPT #7 || NOP");
}

/*-------------------------------------------
 ¸Ãº¯Êı³õÊ¼»¯FLASH¡£ ¸Ãº¯Êı±ØĞëÔÚÄÚ²¿RAMÖĞÔËĞĞ¡£ÔÚOTP/Flash½«
 »á²úÉú²»¿ÉÔ¤ÖªµÄ´íÎó¡£
-------------------------------------------*/

void InitFlash(void)
{
   EALLOW;
   FlashRegs.FOPT.bit.ENPIPE = 1;       /* Ê¹ÄÜFlashµÄÁ÷Ë®ÏßÄ£Ê½*/

   /* ×îĞ¡µÄÊ±¼äµÈ´ı*/
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5;  /* ÉèÖÃÒ³·ÃÎÊµÈ´ıÊ±¼ä*/
   FlashRegs.FBANKWAIT.bit.RANDWAIT = 5;  /*ÉèÖÃËæ»ú·ÃÎÊµÈ´ıÊ±¼ä*/

   FlashRegs.FOTPWAIT.bit.OTPWAIT = 8;    /*ÉèÖÃOTP·ÃÎÊµÈ´ıÊ±¼ä */ 

   FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
   FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
   EDIS;

   asm(" RPT #7 || NOP");
}

/*--------------------------------------------
 CpuTimer0 ³õÊ¼»¯,ÓÃ×÷ÏµÍ³¿ØÖÆÖÜÆÚÊ±»ù(100us)
--------------------------------------------*/

void InitCpuTimer0(void)
{
    /* ³õÊ¼»¯ÖÜÆÚ:*/	
    CpuTimer0Regs.PRD.all  = CPUTIMER0_PRD;
    /* ³õÊ¼»¯Ô¤¶¨±ê (SYSCLKOUT):*/	
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;
    /* È·±£¶¨Ê±ÊÇÍ£Ö¹µÄ:*/
    CpuTimer0Regs.TCR.bit.TSS = 1;
    /* °ÑÖÜÆÚÖµ¼ÓÔØµ½¼ÆÊı¼Ä´æÆ÷:*/
    CpuTimer0Regs.TCR.bit.TRB = 1;
    /* ·ÂÕæÁ¢¼´Í£Ö¹     */
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.FREE = 0; 
    /* ¶¨Ê±Æ÷ÖĞ¶ÏÊ¹ÄÜ   */
    CpuTimer0Regs.TCR.bit.TIE = 1;      /*0½ûÖ¹ÖĞ¶Ï£¬1Ê¹ÄÜ¶¨Ê±Æ÷ÖĞ¶Ï  */
}
void InitCpuTimers(void)
{
     InitCpuTimer0();
}
/*-------------------------------------------
-------------------------------------------
 ³õÊ¼»¯QepÄ£¿é: 
-------------------------------------------*/

void InitEQep(void)
{
    
}
/*-------------------------------------------
 ³õÊ¼»¯capÄ£¿é: ÓĞËÄÖÖ²Ù×÷Ä£Ê½
Ë«ÅÅĞòÆ÷Á¬Ğø£ºSEQ1ÊÂ¼şÆô¶¯SEQ1£¬SEQ2ÊÂ¼şÆô¶¯SEQ2£¬MAXCONV0-2±íÊ¾SEQ1×ª»»ĞòÁĞ¸öÊı£¬MAXCONV4-6±íÊ¾SEQ2×ª»»ĞòÁĞ¸öÊı¡£
Ë«ÅÅĞòÆ÷²¢ÁĞ£ºSEQ1ÊÂ¼şÆô¶¯SEQ1£¬SEQ2ÊÂ¼şÆô¶¯SEQ2£¬SEQ1½öCONV00-03ÓĞĞ§£¬ÒÀ´Î×ª»»A0¡¢B0¡¢A1¡¢B1µÈ8¸öÍ¨µÀ£¬SEQ2½öCONV08-0BÓĞĞ§£¬MAXCONV×î´óÉèÎª0x33¡£
¼¶ÁªÅÅĞòÆ÷Á¬Ğø£ºSEQ1ÊÂ¼şÆô¶¯16¸öÍ¨µÀ£¬MAXCONV0-3±íÊ¾×ª»»ĞòÁĞ¸öÊı¡£
¼¶ÁªÅÅĞòÆ÷²¢ÁĞ£ºSEQ1ÊÂ¼şÆô¶¯16¸öÍ¨µÀ£¬MAXCONV0-3×î´óÉèÎª0x7£¬ÒÀ´Î×ª»»A0¡¢B0¡¢A1¡¢B1µÈ16¸öÍ¨µÀ£¬½á¹û±£ÒÀ´Î±£´æÔÚresult0-result15¡£
-------------------------------------------*/

/*-------------------------------------------
 ³õÊ¼»¯ADCÄ£¿é: 
-------------------------------------------*/

void InitAdc(void)
{

    EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    /*µ÷ÓÃĞ£Õıº¯ÊıÇ°ĞèÈ·±£È«ÄÜADCÊ±ÖÓ*/
	ADC_cal();
	EDIS;

    AdcRegs.ADCREFSEL.bit.REF_SEL = 0;           /*²ÉÓÃÄÚ²¿²Î¿¼µçÔ´*/
    AdcRegs.ADCTRL3.all = 0x00E0;  /* ´øÏ¶¡¢²Î¿¼µçÂ·¡¢ÆäËûÄ£ÄâµçÂ·µçÔ´ÉÏµç*/
    DELAY_US(ADC_usDELAY);         /* ÑÓÊ±*/
        
    AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CLKPS;   /* ADCCLK = HSPCLK/[2*ADCCLKPS*(ADCTRL1[7]+1)]	ADCTRL1[7]=0*/	
	AdcRegs.ADCTRL3.bit.SMODE_SEL = SMODE;		/* 1²¢·¢²ÉÑù,0Á¬Ğø²ÉÑù*/
	AdcRegs.ADCTRL1.bit.CPS = 0;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        	/* 0:Ë«ÅÅĞòÆ÷Ä£Ê½  1:¼¶Áª·½Ê½*/
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;       	/* 0 ÆôÍ£Ä£Ê½£¬1 Á¬ĞøÔËĞĞ*/
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;		/* ²É±£Ê±¼äÎªADC_SHCLK*ADCCLK*/
	
	AdcRegs.ADCTRL2.all = 0x0000;				
    
   	AdcRegs.ADCMAXCONV.all = 0x0007;       /* ×ª»»Êı  2-0:SEQ1  6-4:SEQ2  3-0:SEQ(¼¶ÁªÊ±£©*/
   	AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x0;
   	AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x1;
   	AdcRegs.ADCCHSELSEQ1.bit.CONV02=0x2;
   	AdcRegs.ADCCHSELSEQ1.bit.CONV03=0x3;
   	AdcRegs.ADCCHSELSEQ2.bit.CONV04=0x4;
   	AdcRegs.ADCCHSELSEQ2.bit.CONV05=0x5;
   	AdcRegs.ADCCHSELSEQ2.bit.CONV06=0x6;
   	AdcRegs.ADCCHSELSEQ2.bit.CONV07=0x7;
}

/*-------------------------------------------
 ´®¿Ú³õÊ¼»¯º¯Êı
-------------------------------------------*/

void InitSci(void)
{
	//ScicRegs.SCIFFTX.all = 0x8000;				/* ´Î»FIFO's ½ûÖ¹FIFO*/
    //ScicRegs.SCICCR.all = 0x0007;				/* ½ûÖ¹Ğ§Ñé£¬¿ÕÏĞÏßÄ£Ê½£¬8Î»·½Ê½´øÒ»¸öÍ£Ö¹Î»*/
    ScicRegs.SCICCR.all = 0x0027;				/* ½ûÖ¹Ğ§Ñé£¬¿ÕÏĞÏßÄ£Ê½£¬8Î»·½Ê½´øÒ»¸öÍ£Ö¹Î»,Ê¹ÄÜÆæĞ§Ñé*/
    ScicRegs.SCICTL1.all = 0x0003; 				/* ½ûÖ¹½ÓÊÕ´íÎóÖĞ¶ÏÊ¹ÄÜ£¬½ûÓÃĞİÃß£¬Ê¹ÄÜ·¢ËÍºÍ½ÓÊÕ*/
    ScicRegs.SCICTL2.all = 0x0003;				/* Ê¹ÄÜ½ÓÊÕÖĞ¶ÏºÍ·¢ËÍÖĞ¶Ï*/
	
	ScicRegs.SCIHBAUD = (SCI_BAUD >> 8) & 0x00FF;	/* ¼ÓÔØ²¨ÌØÂÊ*/
	ScicRegs.SCILBAUD = (SCI_BAUD) & 0x00FF;

	ScicRegs.SCIFFTX.all=0xC001;//Ê¹ÄÜFIFO£¬½ûÖ¹·¢ËÍÖĞ¶Ï£¬·¢ËÍÉî¶È1
	ScicRegs.SCIFFRX.all=0x0001;//½ûÖ¹½ÓÊÕÖĞ¶Ï£¬½ÓÊÕÉî¶È1
    //ScicRegs.SCIFFRX.all=0x0021;//Ê¹ÄÜ½ÓÊÕÖĞ¶Ï£¬½ÓÊÕÉî¶È1
    ScicRegs.SCIFFCT.all=0x00;

	ScicRegs.SCIPRI.all = 0x0000;  				/* ·ÂÕæ¹ÒÆğÊ±Í£Ö¹ */
	ScicRegs.SCICTL1.all = 0x0023;				/* ÖÃÎ»Èí¸´Î»£¬´®¿Ú³õÊ¼»¯Íê³É*/

    //ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    //ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
}


/*-------------------------------------------
 ¸Ãº¯Êı³õÊ¼»¯Íâ²¿ÖĞ¶Ï¡£
-------------------------------------------*/

void InitXIntrupt(void)
{
	EALLOW;
   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 10;   /* Xint1 is GPIO10*/
   EDIS;

	XIntruptRegs.XINT1CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/

	XIntruptRegs.XINT2CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT2CR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
	
	XIntruptRegs.XINT3CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT3CR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
	
	XIntruptRegs.XINT4CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT4CR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
	
	XIntruptRegs.XINT5CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT5CR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
	
	XIntruptRegs.XINT6CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT6CR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
	
	XIntruptRegs.XINT7CR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XINT7CR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
	
	XIntruptRegs.XNMICR.bit.POLARITY = 0;		/*00:ÏÂ½µÑØ²úÉúÖĞ¶Ï 01£ºÉÏÉıÑØ 10£ºÏÂ½µÑØ 11£ºBothÉÏÉıÑØÓëÏÂ½µÑØ*/
	XIntruptRegs.XNMICR.bit.SELECT = 1;			/*0:Timer 1 connected To INT13   1£ºXNMI_XINT13 connected To INT13*/
	XIntruptRegs.XNMICR.bit.ENABLE = 0;		/*0:½ûÖ¹  1£ºÊ¹ÄÜ*/
}


void InitECap(void)
{
}

/*ÍâÉè³õÊ¼»¯Íê³É
----------------------------------------------------------------------------*/

/*===========================================================================
 No more.
===========================================================================*/
