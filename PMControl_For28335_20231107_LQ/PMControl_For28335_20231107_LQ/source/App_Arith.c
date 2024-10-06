/*
================================================================================

 �ļ���:	  App_Arith.c

 ��������:	
		 	  ��ѧ�������
			  _iq	_IQasinPU(_iq PA);
			  _iq	_IQasin4PU(_iq real, _iq imag);

 ��д��:    �����
 ��д����:  2005-3-6

===============================================================================
*/

#include "F28335_Device.h"     /* F2812 ����ͷ�ļ�*/
#include "App_UserDef.h"      /* F2812 Ӧ����Ŀͷ�ļ�*/

#pragma CODE_SECTION(_IQasin4PU, "ramfuncs");
#pragma CODE_SECTION(_IQasinPU, "ramfuncs");
#pragma CODE_SECTION(AngleAdjust, "ramfuncs"); 
#pragma CODE_SECTION(Adc_Arith, "ramfuncs");  
#pragma CODE_SECTION(Adc_Temp, "ramfuncs");  

/*------------------------------------------------------------
������  _iq	_IQasin4PU(_iq real, _iq imag)
˵����  �����޷����Һ���
��ڲ�����ʵ�� real���鲿 imag  QG
���ڲ���������ֵΪ�Ƕȵı���ֵ  QG  ����0��360��
--------------------------------------------------------------*/

 _iq	_IQasin4PU(_iq real, _iq imag)
 {
 	volatile _iq mag, par, thetaPU;			
 	
 	mag = _IQmag(real, imag);			/* mag = sqrt(real^2 + imag^2)*/
  	par = _IQdiv(_IQabs(imag), mag);	/* par = |imag|/mag, ��sinֵ*/
  	thetaPU = _IQasinPU(par);			/* thetaPU = acrsin(par)*/
  	
 	if( imag >= 0)
 	{
 		if(real > 0)
 		{ 
 			thetaPU = (thetaPU);				/*��һ����*/
 		}      		
 		else
 		{
 			thetaPU = (_IQ(0.5) - thetaPU); 	/*�ڶ�����*/
 		}
 	
 	}
	else 
	{
		if(real >0)
		{
			thetaPU = (_IQ(1) - thetaPU);		/*��������*/
		}	
		else
		{
			thetaPU = (_IQ(0.5) + thetaPU);		/*��������*/
		} 
	}
	return (thetaPU);
}
/* �����޷����Һ�������
============================================================================ 
*/ 	
 	


/*------------------------------------------------------------
������  _iq	_IQasinPU(_iq PA)
˵����  ��������Һ��������Ϊ1024�㣬��ʽΪQ30�ı���ֵ
��ڲ���������ֵ  QG(0--1)
���ڲ���������ֵΪ�Ƕȵı���ֵ  QG  ����0��90��
--------------------------------------------------------------*/

_iq	_IQasinPU(_iq PA)
{
	volatile _iq 	out;					
	volatile _iq 	detaX, detaY;
	volatile int32	indexArcSin;
	volatile _iq30 	y1, y2;
	volatile Uint32 pa = (Uint32)PA;
 
 	indexArcSin = _IQint(pa << 10);			/*�������PA�ڱ���ж�Ӧ����ֵ���������֣�Q0��*/
 	detaX = _IQfrac(pa << 10);				/*��Ӧ����ֵ��С������ QG*/
 	 	
 	y1 = *(pArcSin + indexArcSin);
 	y2 = *(pArcSin + indexArcSin + 1);				/* Q30*/
	
 	detaY = _IQmpyIQX((y2 - y1), Q30, detaX, QG);	
 	out = _IQ30toIQ(y1) + detaY;					
 	return (out);
 }
/* ��������Һ�������
============================================================================ 
*/


/*------------------------------------------------------------
������  _iq	AngleAdjust(_iq angle)
˵����  �Ƕȼ�����������Ϊ�����ýǶȾ�ΪQG�ı���ֵ��ӦΪ�Ǹ�����
��ڲ������������Ƕ�ֵ  QG
���ڲ���������ֵΪ�����Ƕȵı���ֵ  QG  (000000h->0FFFFFh)����0��360��
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
 �Ƕȼ���������������
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


