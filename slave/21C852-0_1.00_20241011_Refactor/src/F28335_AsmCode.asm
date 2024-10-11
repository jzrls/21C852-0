*********************************************************************************
*																				*
* �ļ���:	  F28335_AsmCode.asm													*
*																				*
* ��������:  AsmԴ����															*
*																				* 
*																				*	
* ��д��:   ����																*
* ��д����: 2011-3-31															*
*********************************************************************************

*--------------------------------------------------------------------------		
	.def    _F28335_usDelay
*--------------------------------------------------------------------------

*--------------------------------------------------------------------------		
	.def    _ADC_cal
*--------------------------------------------------------------------------

*--------------------------------------------------------------------------		
	.ref _c_int00
*--------------------------------------------------------------------------


*========================================================================
* ����:
*		��λ��ת����
*========================================================================
   .sect "codestart"
   .global code_start
code_start:
	LB wd_disable       ;��ת���رտ��Ź�����
 ;end codestart section

    .text
wd_disable:
	SETC	OBJMODE        	;Set OBJMODE for 28x object code
    EALLOW              	;Enable EALLOW protected register access
    MOVZ	DP, #7029h>>6  	;Set data page for WDCR register
    MOV 	@7029h, #0068h 	;Set WDDIS bit in WDCR to disable WD
    EDIS                	;Disable EALLOW protected register access
    LB 		_c_int00        ;Branch to start of boot.asm in RTS library
;end wd_disable
*============================================================================

*========================================================================
* ����:
*		��ʱ����
*		��ÿ��ѭ��֮ǰ��Լ��9/10���ڣ�ÿ��ѭ����5������
* 		LoopCount�����й�ʽ����:
* 		DELAY_CPU_CYCLES = 9 + 5*LoopCount
* 		LoopCount = (DELAY_CPU_CYCLES - 9) / 5
*========================================================================
	    
	.sect 	"ramfuncs"
        .global  __F28335_usDelay
_F28335_usDelay:
   	SUB		ACC,	#1
 	BF		_F28335_usDelay,	GEQ		; Loop if ACC >= 0
 	LRETR 
;==========================================================================


*========================================================================
* ����:
*    This is the ADC cal routine.This routine is programmed into 
*  reserved memory by the factory. 0xAAAA and 0xBBBB are place holders for calibration data. 
*  The actual values programmed by TI are device specific. 
*  This function assumes that the clocks have been
*  enabled to the ADC module.
*========================================================================


       .asg "0x711C",   ADCREFSEL_LOC
       .sect ".adc_cal"
_ADC_cal
    MOVW  DP,   #ADCREFSEL_LOC >> 6
    MOV   @28,  #0xAAAA            ; actual value may not be 0xAAAA
    MOV   @29,  #0xBBBB             ; actual value may not be 0xBBBB
    LRETR
;==========================================================================

     .end


**********************************************************************************
* No more.
**********************************************************************************