/*
//=================================================================================
//
// �ļ���:     	App_PI.c
//                   
//�������� :   ���ļ�����PI������
//				 
//��д: 	   �����
//��д����:    2005-7-23  
//        
//=================================================================================
*/


#pragma CODE_SECTION(pi_reg_calc, "ramfuncs"); 

//---------------------------------------------------------------------------------
#include "F28335_Device.h"       // F2812ͷ�ļ�
#include "App_UserDef.h"        // F2812 Ӧ����Ŀͷ�ļ�

//-------------------------------------------------------------------------------


void pi_reg_calc(PIREG *v)				
{
	v->e_reg = v->pi_ref_reg - v->pi_fdb_reg;						// err = ref-fbd
	v->loop_index++;
	if(v->loop_index >= v->i_period )								// �жϻ���ʱ���Ƿ�
	{
		v->ui_reg = v->ui_reg + _IQmpy(v->Ki_reg, v->e_reg); 	// �����ۼ�
		if(v->ui_reg < v->ui_out_min)
			v->ui_reg = v->ui_out_min;							// �������޷�
		else if (v->ui_reg > v->ui_out_max)	
			v->ui_reg = v->ui_out_max;							// �������޷�
    	v->loop_index = 0;	
    }

    v->up_reg = _IQmpy(v->Kp_reg, v->e_reg);						// Kp*err
    v->pi_out_reg = v->ui_reg + v->up_reg;							// PI_OUT	
    if (v->pi_out_reg > v->pi_out_max)
    	v->pi_out_reg =  v->pi_out_max;								// PI������޷�
    else if (v->pi_out_reg < v->pi_out_min)
    	v->pi_out_reg =  v->pi_out_min; 							// PI������޷�
     
 }
    
    	
//===========================================================================
// No more.
//===========================================================================
