/*
//=================================================================================
//
// �ļ���:     	App_ref_trans.c
//                   
//�������� :   ���ļ���������任
//				 
//��д: 	   �����
//��д����:    2005-4-6  
//        
//=================================================================================
*/

//---------------------------------------------------------------------------------
#include "F28335_Device.h"       // F28335ͷ�ļ�
#include "App_UserDef.h"        // F28335 Ӧ����Ŀͷ�ļ�


#pragma CODE_SECTION(ABC2DQ_CALC, "ramfuncs");  
#pragma CODE_SECTION(DQ2ABC_CALC, "ramfuncs"); 
#pragma CODE_SECTION(THREE2TWO_CALC, "ramfuncs");

//-------------------------------------------------------------------------------
/* sqrt(2)/sqrt(3) = 0.81649658093 */
/* 1/sqrt(2) = 0.7071067811865 */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ������������:
//				a��b��c���ྲֹ����ϵת��Ϊ��ת��d��q����ϵ�����õ��Ǻ㹦�ʱ任��
//				dr = sqrt(2/3)*[as*cos(ang) + bs*cos(ang-120) + cs*cos(ang+120)]
//				qr = sqrt(2/3)*[as*sin(ang) + bs*sin(ang-120) + cs*sin(ang+120)]
//				ע:�˱任����ѡȡq������ת�����ͺ�d��90��.
// ��ڲ���:
// ���ڲ���:
// ����˵��:
//------------------------------------------------------------------------------
void ABC2DQ_CALC()
{
   volatile _iq cos_anga,sin_anga,cos_angb,sin_angb,cos_angc,sin_angc;
   
    /* Using look-up IQ sine table */  
     sin_anga = _IQsinPU(abctodq.ang);
     cos_anga = _IQcosPU(abctodq.ang);
     sin_angb = _IQsinPU(abctodq.ang - _IQ(0.3333333333));
     cos_angb = _IQcosPU(abctodq.ang - _IQ(0.3333333333));
     sin_angc = _IQsinPU(abctodq.ang + _IQ(0.3333333333));
     cos_angc = _IQcosPU(abctodq.ang + _IQ(0.3333333333));
     
 	 abctodq.dr = _IQmpy((_IQmpy(abctodq.as,cos_anga) + _IQmpy(abctodq.bs,cos_angb)+_IQmpy(abctodq.cs,cos_angc)),_IQ(0.81649658093));
     abctodq.qr = _IQmpy((_IQmpy(abctodq.as,sin_anga) + _IQmpy(abctodq.bs,sin_angb)+_IQmpy(abctodq.cs,sin_angc)),_IQ(0.81649658093));
    
  }

//------------------------------------------------------------------------------
  
//------------------------------------------------------------------------------
// ������������:
//				��ת��d��q����ϵת��Ϊa��b��c���ྲֹ����ϵ�����õ��Ǻ㹦�ʱ任��
//				as = sqrt(2/3)*[dr*cos(ang) + qr*sin(ang)]
//				bs = sqrt(2/3)*[dr*cos(ang-120) + qr*sin(ang-120)]
//				cs = sqrt(2/3)*[dr*cos(ang+120) + qr*sin(ang+120)]
//				ע:�˱任����ѡȡq������ת�����ͺ�d��90��.
// ��ڲ���:
// ���ڲ���:
// ����˵��:
//------------------------------------------------------------------------------
void DQ2ABC_CALC ()
{
    volatile _iq cos_anga,sin_anga,cos_angb,sin_angb,cos_angc,sin_angc,Temp_Q;
      	
    /* Using look-up IQ sine table */  
     sin_anga = _IQsinPU(dqtoabc.ang);
     cos_anga = _IQcosPU(dqtoabc.ang);
     
     Temp_Q = dqtoabc.ang - _IQ(0.3333333333);
     Temp_Q = AngleAdjust(Temp_Q);							
     sin_angb = _IQsinPU(Temp_Q);
     cos_angb = _IQcosPU(Temp_Q);
     
     Temp_Q = dqtoabc.ang + _IQ(0.3333333333);
     Temp_Q = AngleAdjust(Temp_Q);							
     sin_angc = _IQsinPU(Temp_Q);
     cos_angc = _IQcosPU(Temp_Q);
     
     dqtoabc.as = _IQmpy((_IQmpy(dqtoabc.dr, cos_anga) + _IQmpy(dqtoabc.qr, sin_anga)), _IQ(0.81649658093));
     dqtoabc.bs = _IQmpy((_IQmpy(dqtoabc.dr, cos_angb) + _IQmpy(dqtoabc.qr, sin_angb)), _IQ(0.81649658093));
     dqtoabc.cs = _IQmpy((_IQmpy(dqtoabc.dr, cos_angc) + _IQmpy(dqtoabc.qr, sin_angc)), _IQ(0.81649658093));
}
//---------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ������������:
//				a��b��c���ྲֹ����ϵת��Ϊ��ֹ��2����ϵ�����õ��Ǻ㹦�ʱ任��
//				ds = sqrt(2/3)*[as - (1/2)bs - (1/2)cs]
//				qs = sqrt(2/3)*[bs*sqrt(3)/2 - cs*sqrt(3)/2]
//
// ��ڲ���:
// ���ڲ���:
// ����˵��:
//------------------------------------------------------------------------------
void THREE2TWO_CALC ()
{
        
    phase_trans.ds = _IQmpy(_IQ(0.81649658093), (phase_trans.as- (phase_trans.bs >> 1) - (phase_trans.cs >> 1)));
	phase_trans.qs = _IQmpy(_IQ(0.7071067811865), (phase_trans.bs - phase_trans.cs));

}
//---------------------------------------------------------------------------------


//=================================================================================
// No More
//=================================================================================























