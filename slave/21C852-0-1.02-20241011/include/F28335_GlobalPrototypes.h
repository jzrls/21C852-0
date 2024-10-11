/*
//================================================================================
//
// 文件名:   	F28335_GlobalPrototypes.h
//
// 功能描述:  
//		    	F28335 公共的全局函数申明
//
// 编写者:    	伍泽东
// 编写日期:  	2011-3-28
//
//================================================================================
*/

#ifndef DSP28335_GLOBALPROTOTYPES_H
#define DSP28335_GLOBALPROTOTYPES_H


#ifdef __cplusplus
extern "C" {
#endif

/*---- 公共的全局函数申明（外设初始化） --------------------------------*/
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
//             注意
// 这个函数必须在 RAM运行. 在 OTP/Flash 
// 运行将产生不可预知的结果
extern void InitFlash(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);


//---------------------------------------------------------------------------
// 连接命令文件产生的外部标签
// F2812将利用这些标签重新分配从Flash或者XINTF加载
// 的代码到内部RAM不同的地方运行
extern Uint16 RamfuncsLoadStart0;
extern Uint16 RamfuncsLoadEnd0;
extern Uint16 RamfuncsRunStart0;


extern Uint16 RamfuncsLoadStart1;
extern Uint16 RamfuncsLoadEnd1;
extern Uint16 RamfuncsRunStart1;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // DSP28335_GLOBALPROTOTYPES_H 定义结束

//===========================================================================
// End of file.
//===========================================================================
