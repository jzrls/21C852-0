#include "F28335_Device.h"
#include "App_UserDef.h"

#pragma CODE_SECTION(pi_reg_calc, "ramfuncs");

void pi_reg_calc(PIREG *v) {
    v->e_reg = v->pi_ref_reg - v->pi_fdb_reg;  // err = ref-fbd
    v->loop_index++;

    // 判断积分时间是否到
    if (v->loop_index >= v->i_period) {
        v->ui_reg = v->ui_reg + _IQmpy(v->Ki_reg, v->e_reg);  // 积分累加
        if (v->ui_reg < v->ui_out_min) {
            // 积分下限幅
            v->ui_reg = v->ui_out_min;
        } else if (v->ui_reg > v->ui_out_max) {
            // 积分上限幅
            v->ui_reg = v->ui_out_max;
        }
        v->loop_index = 0;
    }

    v->up_reg = _IQmpy(v->Kp_reg, v->e_reg);  // Kp*err
    v->pi_out_reg = v->ui_reg + v->up_reg;    // PI_OUT
    if (v->pi_out_reg > v->pi_out_max) {
        // PI输出上限幅
        v->pi_out_reg = v->pi_out_max;
    } else if (v->pi_out_reg < v->pi_out_min) {
        // PI输出下限幅
        v->pi_out_reg = v->pi_out_min;
    }
}
