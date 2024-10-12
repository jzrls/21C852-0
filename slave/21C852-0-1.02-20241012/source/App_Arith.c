/*
================================================================================

 文件名:	  App_Arith.c

 功能描述:	
		 	  数学计算程序。
			  _iq	_IQasinPU(_iq PA);
			  _iq	_IQasin4PU(_iq real, _iq imag);

 编写者:    周求宽
 编写日期:  2005-3-6

===============================================================================
*/

#include "F28335_Device.h"     /* F2812 外设头文件*/
#include "App_UserDef.h"      /* F2812 应用项目头文件*/

#pragma CODE_SECTION(_IQasin4PU, "ramfuncs");
#pragma CODE_SECTION(_IQasinPU, "ramfuncs");
#pragma CODE_SECTION(AngleAdjust, "ramfuncs"); 
#pragma CODE_SECTION(Adc_Arith, "ramfuncs");  
#pragma CODE_SECTION(Adc_Temp, "ramfuncs");  

/*------------------------------------------------------------
函数：  _iq	_IQasin4PU(_iq real, _iq imag)
说明：  四象限反正弦函数
入口参数：实部 real、虚部 imag  QG
出口参数：返回值为角度的标幺值  QG  代表0～360°
--------------------------------------------------------------*/

 _iq	_IQasin4PU(_iq real, _iq imag)
 {
 	volatile _iq mag, par, thetaPU;			
 	
 	mag = _IQmag(real, imag);			/* mag = sqrt(real^2 + imag^2)*/
  	par = _IQdiv(_IQabs(imag), mag);	/* par = |imag|/mag, 即sin值*/
  	thetaPU = _IQasinPU(par);			/* thetaPU = acrsin(par)*/
  	
 	if( imag >= 0)
 	{
 		if(real > 0)
 		{ 
 			thetaPU = (thetaPU);				/*第一象限*/
 		}      		
 		else
 		{
 			thetaPU = (_IQ(0.5) - thetaPU); 	/*第二象限*/
 		}
 	
 	}
	else 
	{
		if(real >0)
		{
			thetaPU = (_IQ(1) - thetaPU);		/*第四象限*/
		}	
		else
		{
			thetaPU = (_IQ(0.5) + thetaPU);		/*第三象限*/
		} 
	}
	return (thetaPU);
}
/* 四象限反正弦函数结束
============================================================================ 
*/ 	
 	


/*------------------------------------------------------------
函数：  _iq	_IQasinPU(_iq PA)
说明：  查表求反正弦函数，表格为1024点，格式为Q30的标幺值
入口参数：正弦值  QG(0--1)
出口参数：返回值为角度的标幺值  QG  代表0～90°
--------------------------------------------------------------*/

_iq	_IQasinPU(_iq PA)
{
	volatile _iq 	out;					
	volatile _iq 	detaX, detaY;
	volatile int32	indexArcSin;
	volatile _iq30 	y1, y2;
	volatile Uint32 pa = (Uint32)PA;
 
 	indexArcSin = _IQint(pa << 10);			/*求出输入PA在表格中对应索引值的整数部分（Q0）*/
 	detaX = _IQfrac(pa << 10);				/*对应索引值的小数部分 QG*/
 	 	
 	y1 = *(pArcSin + indexArcSin);
 	y2 = *(pArcSin + indexArcSin + 1);				/* Q30*/
	
 	detaY = _IQmpyIQX((y2 - y1), Q30, detaX, QG);	
 	out = _IQ30toIQ(y1) + detaY;					
 	return (out);
 }
/* 查表求反正弦函数结束
============================================================================ 
*/


/*------------------------------------------------------------
函数：  _iq	AngleAdjust(_iq angle)
说明：  角度计算修正，因为计算用角度均为QG的标幺值，应为非负数。
入口参数：待修正角度值  QG
出口参数：返回值为修正角度的标幺值  QG  (000000h->0FFFFFh)代表0～360°
--------------------------------------------------------------*/

_iq	AngleAdjust(_iq angle)
{
	while(angle < 0)
	{
		angle += _IQ(1);
	}
	while(angle >= _IQ(1))
	{
		angle -= _IQ(1);
	}
	
 	return (angle);
 }
/*============================================================================ 
 角度计算修正函数结束
============================================================================*/ 

_iq Adc_Temp(volatile Uint16 AD_digital_value1)     //????
{
	volatile _iq real_value,adc_digital_value;


	adc_digital_value = (_iq)AD_digital_value1;
	real_value = _IQmpy(adc_digital_value, _IQ(48));   // (3V/65536)<<20=3*16

	return real_value;
}

_iq17 Adc_Arith(volatile Uint16 AD_digital_value1)
{
	volatile _iq17 real_value,adc_digital_value;	
	
	adc_digital_value = (_iq17)AD_digital_value1;

	real_value = _IQ17mpy(adc_digital_value, _IQ17(6));//(3V/65536)<<17=3*2

	return real_value;
}

/*============================================================================
 No More
===========================================================================*/


