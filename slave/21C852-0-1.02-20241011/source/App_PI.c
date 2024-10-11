/*
//=================================================================================
//
// 文件名:     	App_PI.c
//                   
//功能描述 :   该文件包含PI调节器
//				 
//编写: 	   周求宽
//编写日期:    2005-7-23  
//        
//=================================================================================
*/


#pragma CODE_SECTION(pi_reg_calc, "ramfuncs"); 

//---------------------------------------------------------------------------------
#include "F28335_Device.h"       // F2812头文件
#include "App_UserDef.h"        // F2812 应用项目头文件

//-------------------------------------------------------------------------------


void pi_reg_calc(PIREG *v)				
{
	v->e_reg = v->pi_ref_reg - v->pi_fdb_reg;						// err = ref-fbd
	v->loop_index++;
	if(v->loop_index >= v->i_period )								// 判断积分时间是否到
	{
		v->ui_reg = v->ui_reg + _IQmpy(v->Ki_reg, v->e_reg); 	// 积分累加
		if(v->ui_reg < v->ui_out_min)
			v->ui_reg = v->ui_out_min;							// 积分下限幅
		else if (v->ui_reg > v->ui_out_max)	
			v->ui_reg = v->ui_out_max;							// 积分上限幅
    	v->loop_index = 0;	
    }

    v->up_reg = _IQmpy(v->Kp_reg, v->e_reg);						// Kp*err
    v->pi_out_reg = v->ui_reg + v->up_reg;							// PI_OUT	
    if (v->pi_out_reg > v->pi_out_max)
    	v->pi_out_reg =  v->pi_out_max;								// PI输出上限幅
    else if (v->pi_out_reg < v->pi_out_min)
    	v->pi_out_reg =  v->pi_out_min; 							// PI输出下限幅
     
 }
    
    	
//===========================================================================
// No more.
//===========================================================================
