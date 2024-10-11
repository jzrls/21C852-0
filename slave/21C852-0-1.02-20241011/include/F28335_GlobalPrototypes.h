/*
//================================================================================
//
// �ļ���:   	F28335_GlobalPrototypes.h
//
// ��������:  
//		    	F28335 ������ȫ�ֺ�������
//
// ��д��:    	����
// ��д����:  	2011-3-28
//
//================================================================================
*/

#ifndef DSP28335_GLOBALPROTOTYPES_H
#define DSP28335_GLOBALPROTOTYPES_H


#ifdef __cplusplus
extern "C" {
#endif

/*---- ������ȫ�ֺ��������������ʼ���� --------------------------------*/
extern void InitAdc(void);
extern void InitPeripherals(void);
extern void InitECan(void);
extern void InitECap(void);
extern void InitEPwm(void);
extern void InitEQep(void);
extern void InitGpio(void);
extern void InitMcbsp(void);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitSci(void);
extern void InitSpi(void);
extern void InitSysCtrl(void);
extern void InitXIntrupt(void);
extern void InitXintf(void);
extern void InitPll(Uint16 val, Uint16 divsel);
extern void InitPeripheralClocks(void);
extern void F28335_usDelay(Uint32 Count);
extern void ADC_cal (void);
extern void ServiceDog(void);
extern void DisableDog(void);
extern Uint16 CsmUnlock(void);

//---------------------------------------------------------------------------
//             ע��
// ������������� RAM����. �� OTP/Flash 
// ���н���������Ԥ֪�Ľ��
extern void InitFlash(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);


//---------------------------------------------------------------------------
// ���������ļ��������ⲿ��ǩ
// F2812��������Щ��ǩ���·����Flash����XINTF����
// �Ĵ��뵽�ڲ�RAM��ͬ�ĵط�����
extern Uint16 RamfuncsLoadStart0;
extern Uint16 RamfuncsLoadEnd0;
extern Uint16 RamfuncsRunStart0;


extern Uint16 RamfuncsLoadStart1;
extern Uint16 RamfuncsLoadEnd1;
extern Uint16 RamfuncsRunStart1;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // DSP28335_GLOBALPROTOTYPES_H �������

//===========================================================================
// End of file.
//===========================================================================
