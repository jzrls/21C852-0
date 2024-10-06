#include "F28335_Device.h"
#include "App_UserDef.h"

#include ".\core\action.h"

#pragma CODE_SECTION(Control, "ramfuncs");
#pragma CODE_SECTION(ControlA, "ramfuncs");
#pragma CODE_SECTION(Ready_Data, "ramfuncs");
#pragma CODE_SECTION(Ready_DataA, "ramfuncs");
#pragma CODE_SECTION(Main_ControlA, "ramfuncs");
#pragma CODE_SECTION(MeasureInitDigital, "ramfuncs");
#pragma CODE_SECTION(OpenLoop, "ramfuncs");
#pragma CODE_SECTION(RunLed, "ramfuncs");
#pragma CODE_SECTION(RCFilter, "ramfuncs");
#pragma CODE_SECTION(Ctrl_Change, "ramfuncs");
#pragma CODE_SECTION(Loop_Ctrl, "ramfuncs");

void Loop_Ctrl(Uint16 Board) {
    volatile _iq tmp = 0;

    Uint16 isStopPwm = Ctrl_Flag.bit.STOP_PWM_Flag == 1;
    Uint16 isFristEnterPwm = Ctrl_Flag.bit.FIRST_PWM_Flag == 1;
    Uint16 isCurrentSpeedOverStartSpeed = Velo_Elec_abs > Velo_Start;

    if (Board == 0) {
        Uint16 isStopValve1 = CAN_CHANGE_CTRL[2] == 0x11 && CAN_CHANGE_CTRL[3] == 0x5A;
        if (isStopValve1 || isStopPwm || (isFristEnterPwm && isCurrentSpeedOverStartSpeed)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_CHANGE_CTRL[2] == 0x10 && CAN_CHANGE_CTRL[3] == 0xA5) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_CHANGE_CTRL[2] == 0x12) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
            Isd_Set = 0;
            tmp = Hex_Float(CAN_CHANGE_CTRL[3], CAN_CHANGE_CTRL[4], CAN_CHANGE_CTRL[5], CAN_CHANGE_CTRL[6]);

            // 65536/360>>15
            tmp = _IQmpy(tmp, _IQ(0.0055555555555555555555555555555556));
            Pos_Set = tmp;
            if (Pos_Set > POSMAX) {
                Pos_Set = POSMAX;
            }
            if (Pos_Set < POSMIN) {
                Pos_Set = POSMIN;
            }

            TorqueAngleA = 0;
            if (RunAllow_Flag == 1) {
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
        }
    }

    if (Board == 2) {
        Uint16 isStopPumpCmd = CAN_CHANGE_CTRL[2] == 0x11 && CAN_CHANGE_CTRL[3] == 0xA5;
        if (isStopPumpCmd || isStopPwm || (isFristEnterPwm && isCurrentSpeedOverStartSpeed)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_CHANGE_CTRL[2] == 0x10 && CAN_CHANGE_CTRL[3] == 0x55) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_CHANGE_CTRL[2] == 0x14) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
            tmp = Hex_Float(CAN_CHANGE_CTRL[3], CAN_CHANGE_CTRL[4], CAN_CHANGE_CTRL[5], CAN_CHANGE_CTRL[6]);
            Veloover_Set = _IQdiv(tmp, _IQmpy((V_Base >> 5), _IQdiv(_IQ(9.5492965855137201461330258023509), Mp)));

            if (Veloover_Set > maxSpeed) {
                Veloover_Set = maxSpeed;
            }
            if (Ctrl_Flag.bit.VelWay_Flag == 1) {
                Veloover_Set = -Veloover_Set;
            }

            Isd_Set = 0;
            TorqueAngleA = 0;
            if (RunAllow_Flag == 1) {
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
        }
    }

    if (Board == 4) {
        Uint16 isStopValve2 = CAN_CHANGE_CTRL[2] == 0x11 && CAN_CHANGE_CTRL[3] == 0x55;
        if (isStopValve2 || isStopPwm || (isFristEnterPwm && isCurrentSpeedOverStartSpeed)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_CHANGE_CTRL[2] == 0x10 && CAN_CHANGE_CTRL[3] == 0xAA) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_CHANGE_CTRL[2] == 0x13) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
            Isd_Set = 0;
            tmp = Hex_Float(CAN_CHANGE_CTRL[3], CAN_CHANGE_CTRL[4], CAN_CHANGE_CTRL[5], CAN_CHANGE_CTRL[6]);
            // 65536/360>>15
            tmp = _IQmpy(tmp, _IQ(0.0055555555555555555555555555555556));
            Pos_Set = tmp;
            if (Pos_Set > POSMAX)
                Pos_Set = POSMAX;
            if (Pos_Set < POSMIN)
                Pos_Set = POSMIN;

            TorqueAngleA = 0;
            if (RunAllow_Flag == 1) {
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
        }
    }
}

//-------------------------------------------------------------------------
void Ctrl_Change(void) {
    volatile Uint16 CAN_CONTROL_PRE = 0x07; // 0000,0111

    /* CAN_CHANGE_CTRL[2] 为命令码， 0x11：停止 */
    if (CAN_CHANGE_CTRL[2] == 0x11 || CAN_CHANGE_CTRL[2] == 0x00) {
        return;
    }

    // 先选择从板,如果主板无故障，且从板不在运行中，则选择主板运行
    // CAN_BUFFER_RT[BORAD_NUM][2] + CAN_BUFFER_RT[BORAD_NUM][3]: 故障位
    if ((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01) {
        CAN_CONTROL_PRE &= 0xFE; // 0000,0110
    }

    if ((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01) {
        CAN_CONTROL_PRE &= 0xFD; // 0000,0101
    }

    if ((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01) {
        CAN_CONTROL_PRE &= 0xFB; // 0000,0011
    }

    /* 蝶阀1主板运行 */
    if ((CAN_CONTROL_PRE & 0x01) == 0x01 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
        if (BORAD_NUM == 0) {
            ET_IO_OUT1_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }

        if (BORAD_NUM == 1) {
            ET_IO_OUT2_L;
            if (BC_SLT_IN == 1) {
                Loop_Ctrl(0);
            }
        }
    }

    /* 蝶阀1副板运行 */
    if ((CAN_CONTROL_PRE & 0x01) == 0x00 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
        if (BORAD_NUM == 0) {
            ET_IO_OUT1_H;

            if (BC_SLT_IN == 0) {
                Loop_Ctrl(0);
            }
        }

        if (BORAD_NUM == 1) {
            ET_IO_OUT2_H;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }
    }

    /* 泵电机主板运行 */
    if ((CAN_CONTROL_PRE & 0x02) == 0x02 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
        if (BORAD_NUM == 2) {
            ET_IO_OUT1_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }

        if (BORAD_NUM == 3) {
            ET_IO_OUT2_L;
            Loop_Ctrl(2);
        }
    }

    /* 泵电机副板运行 */
    if ((CAN_CONTROL_PRE & 0x02) == 0x00 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
        if (BORAD_NUM == 2) {
            ET_IO_OUT1_L;
            Loop_Ctrl(2);
        }

        if (BORAD_NUM == 3) {
            ET_IO_OUT2_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }
    }

    /* 蝶阀2主板运行 */
    if ((CAN_CONTROL_PRE & 0x04) == 0x04 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
        if (BORAD_NUM == 4) {
            ET_IO_OUT1_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }

        if (BORAD_NUM == 5) {
            ET_IO_OUT2_L;
            if (BC_SLT_IN == 1) {
                Loop_Ctrl(4);
            }
        }
    }

    /* 蝶阀2副板运行 */
    if ((CAN_CONTROL_PRE & 0x04) == 0x00 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
        if (BORAD_NUM == 4) {
            ET_IO_OUT1_H;

            if (BC_SLT_IN == 0) {
                Loop_Ctrl(4);
            }
        }

        if (BORAD_NUM == 5) {
            ET_IO_OUT2_H;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }
    }
    CAN_CONTROL = CAN_CONTROL_PRE;
}

void RunLed(void) {
    volatile Uint16 i, Fi_Number;
    volatile Uint16 Store_Temp = 0;
    if (Run_Led_Index < RUN_LED_PERIOD) {
        Run_Led_Index++;
    } else {
        Run_Led_Index = 0;
        LED1 = 1;
    }

    /* 接收串口数据，所有数据存放于SCI_RX_ALLBUFFER[]数组中 */
    if (ScicRegs.SCIFFRX.bit.RXFFST > 0 && SCIB.FLAG.bit.RXTIME_FLAG == 0) {
        Fi_Number = ScicRegs.SCIFFRX.bit.RXFFST;
        for (i = 0; i < Fi_Number; i++) {
            /* 检测接收缓冲区是否溢出，溢出将接收指针指向SCI_RX_ALLBUFFER[]数组的起始位置
             */
            if (SCIB.p_Rx_AllBuffer >= (Uint16 *)(SCI_RX_ALLBUFFER + RX_ALLBUFFER_LENGTH)) {
                SCIB.p_Rx_AllBuffer = SCI_RX_ALLBUFFER;
            }
            Store_Temp = ScicRegs.SCIRXBUF.bit.RXDT;
            *SCIB.p_Rx_AllBuffer = (Store_Temp & 0x00FF);
            SCIB.p_Rx_AllBuffer++;
        }

        ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1; // Clear Overflow flag
        ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1; // Clear Interrupt flag
        PieCtrlRegs.PIEACK.bit.ACK8 = 1;
    }
}
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// 一阶RC滤波程序
// 算法 Y(k) = a*X(k)+(1-a)*Y(k-1);
// 截止频率 f=filta/(2*pi*T)
//---------------------------------------------------------------------------------
_iq RCFilter(volatile _iq filtout, volatile _iq filtin, volatile _iq Freq_cut) {
    volatile _iq filta = 0, tmp = 0;
    // 控制周期为	100us
    filta = _IQmpy(Freq_cut, _IQmpy(_IQ(6.283185307179586476925286766559), Per_Ctrl));

    if (filta >= _IQ(1)) {
        filta = _IQ(1);
    }

    filta = _IQmpy(filta, filtin - filtout);
    tmp = (filtin - filtout);
    if (filta == 0 && _IQabs(tmp) > _IQ(0.000001)) {
        if (tmp > 0) {
            return (0x1 + filtout);
        } else {
            return (-0x1 + filtout);
        }
    } else {
        return (filta + filtout);
    }
}
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 测量旋变的初始位置角
//----------------------------------------------------------------------
void MeasureInitDigital(void) {
    Udc_Mche = Udc_Lock;
    phase_trans.as = Usa_Lock;
    phase_trans.bs = Usb_Lock;
    phase_trans.cs = Usc_Lock;
    THREE2TWO_CALC();
    Us_Alfa_Mche = phase_trans.ds;
    Us_Beta_Mche = phase_trans.qs;

    SVPWMA_Gen(Us_Alfa_Mche, Us_Beta_Mche);
}

//----------------------------------------------------------------------
// Open_Loop
//----------------------------------------------------------------------
void OpenLoop(void) {
    volatile _iq sin_anga, sin_angb, sin_angc;

    // Per_Ctrl是控制周期，f*T
    Angle_ElecOpen += _IQmpy(Freq_OpenLoop, Per_Ctrl);
    Angle_ElecOpen = AngleAdjust(Angle_ElecOpen);
    sin_anga = _IQcosPU(Angle_ElecOpen);

    // 这里的1/3对应120度
    sin_angb = _IQcosPU(Angle_ElecOpen - _IQ(0.3333333333));
    sin_angc = _IQcosPU(Angle_ElecOpen + _IQ(0.3333333333));

    //    Udc_OpenLoop应为倒数
    pwma.mDuty1 = _IQ(0.5) + _IQmpy(_IQmpy(Um_OpenLoop, sin_anga), Udc_OpenLoop);
    if (pwma.mDuty1 > _IQ(1)) {
        pwma.mDuty1 = _IQ(1);
    } else if (pwma.mDuty1 < 0) {
        pwma.mDuty1 = 0;
    }

    pwma.mDuty2 = _IQ(0.5) + _IQmpy(_IQmpy(Um_OpenLoop, sin_angb), Udc_OpenLoop);
    if (pwma.mDuty2 > _IQ(1)) {
        pwma.mDuty2 = _IQ(1);
    } else if (pwma.mDuty2 < 0) {
        pwma.mDuty2 = 0;
    }

    pwma.mDuty3 = _IQ(0.5) + _IQmpy(_IQmpy(Um_OpenLoop, sin_angc), Udc_OpenLoop);
    if (pwma.mDuty3 > _IQ(1)) {
        pwma.mDuty3 = _IQ(1);
    } else if (pwma.mDuty3 < 0) {
        pwma.mDuty3 = 0;
    }
}

_iq __getCurrent(Uint16 currentAdcValue, _iq17 sacle, _iq17 offset) {
    _iq17 current = Adc_Arith(currentAdcValue);
    current = _IQ17mpy((current - offset), sacle);
    current = _IQmpy(current << 3, I_Base); // Isa->标幺值
    return current;
}

void getAdc() {
    ADc_Isa = AdcRegs.ADCRESULT0;   /* a相电流 */
    ADc_Isb = AdcRegs.ADCRESULT1;   /* b相电流 */
    ADc_Isc = AdcRegs.ADCRESULT2;   /* c相电流 */
    ADc_Udc = AdcRegs.ADCRESULT3;   /* 母线电压 */
    ADc_Temp4 = AdcRegs.ADCRESULT4; /* 驱动板温度 */
    ADc_Idc = AdcRegs.ADCRESULT5;   /* 母线电流 */

    Isa = __getCurrent(ADc_Isa, K_Isa, Off_Isa); /* 获取A相电流 */
    Isb = __getCurrent(ADc_Isb, K_Isb, Off_Isb); /* 获取B相电流 */
    Isc = __getCurrent(ADc_Isc, K_Isc, Off_Isc); /* 获取C相电流 */
    /* 获取母线电压,同电流比例还原 */
    Udc_Mche_1 = __getCurrent(ADc_Udc, K_Udc, Off_Udc);
    Idc = __getCurrent(ADc_Idc, K_Idc, Off_Idc); /* 获取母线电流 */

    /* 滤波处理 */
    Udc_Mche_1 = RCFilter(Udc_Mche0, Udc_Mche_1, Udc_Filter);
    Udc_Mche0 = Udc_Mche_1;
    Idc = RCFilter(Idc0, Idc, Udc_Filter);
    Idc0 = Idc;

    Temperature5 = Adc_Temp(ADc_Temp4);
    Temperature5 = _IQmpy((Temperature5 - Temperature5_Off), Temperature5_K);
    Temperature5 = RCFilter(Temperature50, Temperature5, Temp_Filter);
    Temperature50 = Temperature5;
    if (Temperature5 <= m_PT_V[0]) {
        Temperature5 = m_PT_T[0];
    } else if (Temperature5 >= m_PT_V[30]) {
        Temperature5 = m_PT_T[30];
    } else {
        for (Uint16 i = 1; i < 31; i++) {
            if (Temperature5 <= m_PT_V[i]) {
                Temperature5 = _IQdiv(_IQmpy(m_PT_T[i] - m_PT_T[i - 1], Temperature5 - m_PT_V[i - 1]), m_PT_V[i] - m_PT_V[i - 1]) + m_PT_T[i - 1];
                break;
            }
        }
    }
}

void getAngle() {
    /* Angle_Q16 = 泵电机实时角度 */
    _iq angle = (long)(Angle_Q16);
    if (Ctrl_Flag.bit.RotorWay_Flag == 1) {
        // 电角度:单对极,p=电机极对数;多对极p=1
        angle = _IQmpy(_IQ16toIQ(angle), p);
    } else {
        // 电角度:单约?,p=电机极对数;多对极p=1
        angle = -_IQmpy(_IQ16toIQ(angle), p);
    }

    Angle_Elec = AngleAdjust(angle);
}

void getMotorSpeed() {
    /*Angle_Q16：当前角度，Angle0_Q16：上次角度*/
    Velo_Calc_flag = Angle_Q16 - Angle0_Q16;
    // 这里两个无符号的16位数相减，反转为负？若在正转时变化0x8000（即32768）
    // 7000r/min对应的是0.07330383rad/100us， 与pi相差42倍！！！
    Velo_Calc_flag = Velo_Calc_flag & 0x8000 == 0 ? 0 : 1;
    if (Velo_Calc_flag == 0) {
        // 正转
        Velo_Calc_flag = 0;
    } else {
        // 反转
        Velo_Calc_flag = 1;
    }

    Uint32 tmp2 = 0;
    if (Velo_Calc_flag > 0) {
        tmp2 = ((Uint32)(Angle0_Q16 - Angle_Q16)) & 0xffff;
    } else {
        tmp2 = ((Uint32)(Angle_Q16 - Angle0_Q16)) & 0xffff;
    }
    Angle0_Q16 = Angle_Q16;

    tmp2 = (tmp2 << 4); // TODO：左移4位的目的???

    // 位置的累加
    if (Ctrl_Flag.bit.RotorWay_Flag == 1 && Velo_Calc_flag > 0) {
        /* 旋变方向与转速方向都为正向 */
        Velotmp -= (_iq)tmp2;
    } else if (Ctrl_Flag.bit.RotorWay_Flag == 0 && Velo_Calc_flag == 0) {
        /* 旋变方向与转速方向都为反向 */
        Velotmp -= (_iq)tmp2;
    } else {
        Velotmp += (_iq)tmp2;
    }

    // 判断是否需要计算速度，Velo_Calc_Num不是Q20的数！！！
    Velo_Avg_Index++;
    if (Velo_Avg_Index >= Velo_Calc_Num) {
        Velo_Avg_Index = 0;
        Velo_Elec_Cal = _IQmpy(Velotmp, K_Velo_Cal);
        Velotmp = _IQ(0);

        /* 超速保护 */
        if (Velo_Elec_abs >= Velo_Max) {
            VeloProtect++;
            if (VeloProtect >= VeloProtectNum) {
                Sys_Flag.bit.STOP_PWM_Flag_Velo = 1;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.STOP_PWM_Flag = 1;
            }
        } else {
            VeloProtect = 0;
        }

        /* 超速保护恢复 */
        Uint16 isSpeedOverProtection = Sys_Flag.bit.STOP_PWM_Flag_Velo == 1;
        Uint16 isSpeedLessMaxSpeed = Velo_Elec_abs < Velo_Max;
        Uint16 isSpeedLessStartSpeed = Velo_Elec_abs < Velo_Start;
        if (isSpeedOverProtection && isSpeedLessMaxSpeed && isSpeedLessStartSpeed) {
            VeloProtect1++;
            if (VeloProtect1 >= (ProtectNum >> 4)) {
                Sys_Flag.bit.STOP_PWM_Flag_Velo = 0;
                VeloProtect1 = 0;
            }
        } else {
            VeloProtect1 = 0;
        }
    }

    // 速度滤波
    Velo_Elec = RCFilter(Velo_Elec0, Velo_Elec_Cal, Velo_Filter);
    Velo_Elec0 = Velo_Elec;
    Velo_Elec_abs = _IQabs(Velo_Elec);
}

void getDqCurrent() {
    abctodq.as = Isa;
    abctodq.bs = Isb;
    abctodq.cs = Isc;
    abctodq.ang = Angle_Elec + _IQmpy(Com_angle_I, Velo_Elec);
    ABC2DQ_CALC(); /* clark+park, 求Id, Iq */
    Id = abctodq.dr;
    Iq = abctodq.qr;
    Id = RCFilter(Id0, Id, id_Filter);
    Id0 = Id;
    Iq = RCFilter(Iq0, Iq, id_Filter);
    Iq0 = Iq;
}

void idcOverProtection() {
    Is = _IQmag(Id, Iq);
    if ((BORAD_NUM == 2 || BORAD_NUM == 3)) {
        /* id_Max:电流保护值，idSet_Max:最大电流 */
        Uint16 isIsOverIdProtectionValue = Is >= id_Max;
        Uint16 isIsOverIdMax = _IQmag(IdZ_Set, IqZ_Set) >= (idSet_Max >> 3);
        Uint16 isIsOweIdSetMax = Is <= (idSet_Max >> 5);
        if (isIsOverIdProtectionValue || (isIsOverIdMax && isIsOweIdSetMax)) {
            IdProtect++;
            if (IdProtect >= IdProtectNum) {
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Sys_Flag.bit.STOP_PWM_Flag_Id = 1;
                Ctrl_Flag.bit.STOP_PWM_Flag = 1;
            }
        } else {
            IdProtect = 0;
        }
    } else {
        Uint16 isIsOverIdProtectionValue = Is >= id_Max;
        Uint16 isIsOverIdMax = _IQmag(IdZ_Set, IqZ_Set) >= (idSet_Max >> 1);
        Uint16 isIsOweIdSetMax = Is <= (idSet_Max >> 3);
        if (isIsOverIdProtectionValue || (isIsOverIdMax && isIsOweIdSetMax)) {
            IdProtect++;
            if (IdProtect >= IdProtectNum) {
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Sys_Flag.bit.STOP_PWM_Flag_Id = 1;
                Ctrl_Flag.bit.STOP_PWM_Flag = 1;
            }
        } else {
            IdProtect = 0;
        }
    }

    /* 过流恢复 */
    Uint16 isIdOverProtection = Sys_Flag.bit.STOP_PWM_Flag_Id == 1;
    Uint16 isIsLessIdMax = Is < id_Max;
    Uint16 isSpeedLessStartSpeed = Velo_Elec_abs < Velo_Start;
    if (isIdOverProtection && isIsLessIdMax && isSpeedLessStartSpeed) {
        IdProtect1++;
        if (IdProtect1 >= ProtectNum) {
            Sys_Flag.bit.STOP_PWM_Flag_Id = 0;
            IdProtect1 = 0;
        }
    } else {
        IdProtect1 = 0;
    }
}

void udcOverProtection() {
    if (_IQabs(Udc_Mche_1) >= UdcLimit_Set) {
        UdcOverProtect++;
        if (UdcOverProtect >= PROTECT_NUM) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Sys_Flag.bit.UDC_FLAG = 1;
            Ctrl_Flag.bit.STOP_PWM_Flag = 1;
        }
    } else {
        UdcOverProtect = 0;
    }

    /**
     * 母线过压恢复
     */
    if (Sys_Flag.bit.UDC_FLAG == 1 && _IQabs(Udc_Mche_1) < UdcLimit_Set && Velo_Elec_abs < Velo_Start) {
        UdcOverProtect1++;
        if (UdcOverProtect1 >= PROTECT_NUM) {
            Sys_Flag.bit.UDC_FLAG = 0;
            UdcOverProtect1 = 0;
        }
    } else {
        UdcOverProtect1 = 0;
    }
}

void udcOweProtection() {
    if (_IQabs(Udc_Mche_1) < UdcLimit1_Set) {
        UdcLowProtect++;
        if (UdcLowProtect >= PROTECT_NUM) {
            if (Ctrl_Flag.bit.STOP_VELO_FLAG == 0)
                Ctrl_Flag.bit.STOP_PWM_Flag = 1;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.UdcLow_Flag = 1;
        }
    } else {
        UdcLowProtect = 0;
    }

    /* 母线欠压恢复 */
    if (Ctrl_Flag.bit.UdcLow_Flag == 1 && _IQabs(Udc_Mche_1) > UdcLimit1_Set && Velo_Elec_abs < Velo_Start) {
        UdcLowEnable++;
        if (UdcLowEnable >= PROTECT_NUM) {
            Ctrl_Flag.bit.UdcLow_Flag = 0;
            UdcLowEnable = 0;
        }
    } else {
        UdcLowEnable = 0;
    }
}

void controllerTempOverProtection() {
    if (Temperature1 >= MotorTM || Temperature3 >= MotorTM) {
        Temp1OverProtect++;
        if (Temp1OverProtect >= PROTECT_NUM) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.Temp1Over_Flag = 1;
            Ctrl_Flag.bit.STOP_PWM_Flag = 1;
        }
    } else {
        Temp1OverProtect = 0;
    }

    if (Ctrl_Flag.bit.Temp1Over_Flag == 1 && Temperature1 < MotorTM && Temperature3 < MotorTM) {
        Temp1OverProtect1++;
        if (Temp1OverProtect1 >= ProtectNum) {
            Ctrl_Flag.bit.Temp1Over_Flag = 0;
            Temp1OverProtect1 = 0;
        }
    } else {
        Temp1OverProtect1 = 0;
    }
}

void driverTempOverProtection() {
    if (Temperature5 >= igbtTemperatureProtectionValue) {
        Temp2OverProtect++;
        if (Temp2OverProtect >= PROTECT_NUM) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.Temp2Over_Flag = 1;
            Ctrl_Flag.bit.STOP_PWM_Flag = 1;
        }
    } else {
        Temp2OverProtect = 0;
    }

    if (Ctrl_Flag.bit.Temp2Over_Flag == 1 && Temperature5 < igbtTemperatureProtectionValue) {
        Temp2OverProtect1++;
        if (Temp2OverProtect1 >= ProtectNum) {
            Ctrl_Flag.bit.Temp2Over_Flag = 0;
            Temp2OverProtect1 = 0;
        }
    } else {
        Temp2OverProtect1 = 0;
    }

    if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1 && INT_IGBT == 1) {
        IGBTProtect1++;
        if (IGBTProtect1 >= ProtectNum) {
            Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;
            IGBTProtect1 = 0;
        }
    } else {
        IGBTProtect1 = 0;
    }
}

void rdcFaultDection() {
    // 旋变故障
    if ((rdc1.FAULT & 0x40) == 0x40) {
        RdcProtect++;
        if (RdcProtect >= IdProtectNum) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Fault_Flag.bit.RDC1_FAULT = 1;
            Ctrl_Flag.bit.STOP_PWM_Flag = 1;
        }
    } else {
        RdcProtect = 0;
    }

    if (Fault_Flag.bit.RDC1_FAULT == 1 && (rdc1.FAULT & 0x40) == 0x00 && Velo_Elec_abs < Velo_Start) {
        RdcProtect1++;
        if (RdcProtect1 >= ProtectNum) {
            Fault_Flag.bit.RDC1_FAULT = 0;
            RdcProtect1 = 0;
        }
    } else {
        RdcProtect1 = 0;
    }

    // 旋变故障
    if ((rdc2.FAULT & 0x40) == 0x40) {
        Rdc2Protect++;
        if (Rdc2Protect >= IdProtectNum) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Fault_Flag.bit.RDC2_FAULT = 1;
            Ctrl_Flag.bit.STOP_PWM_Flag = 1;
        }
    } else {
        Rdc2Protect = 0;
    }

    if (Fault_Flag.bit.RDC2_FAULT == 1 && (rdc2.FAULT & 0x40) == 0x00 && Velo_Elec_abs < Velo_Start) {
        Rdc2Protect1++;
        if (Rdc2Protect1 >= ProtectNum) {
            Fault_Flag.bit.RDC2_FAULT = 0;
            Rdc2Protect1 = 0;
        }
    } else {
        Rdc2Protect1 = 0;
    }
}

//---------------------------------------------------------------------------
// 控制周期程序需要的电压、电流、速度、角度的采集计算。
// 求得所需数据的标幺值
//---------------------------------------------------------------------------
void Ready_DataA(void) {
    /* 获取电机角度 */
    getAngle();

    // 计算速度
    getMotorSpeed();

    /* 执行AD采集，获取三相、母线电流、母线电压、驱动板温度 */
    getAdc();

    /* clark + park 变换，求Id、Iq */
    getDqCurrent();

    /* 故障检测与保护 */
    idcOverProtection();
    udcOverProtection();
    udcOweProtection();
    controllerTempOverProtection();
    driverTempOverProtection();
    rdcFaultDection();
}

void Ready_Data(void) {
    AdcRegs.ADCTRL2.all = 0x2000; // Start SEQ1

    // Wait for INT1
    while (AdcRegs.ADCST.bit.INT_SEQ1 == 0) {}

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;

    RDC_READ(); // 读旋变位置
    Angle_Q16 = rdc1.DATA_BUFFER - Angle_Init_Digital;
    Angle_Fa_Q16 = rdc2.DATA_BUFFER - Angle_Init_Fa;

    Ready_DataA();
}

void positionControl() {
    if (Ctrl_Flag.bit.POS_CONTROL_FLAG == 0) {
        return;
    }

    pi_Pos.ctrl_index++;
    if (pi_Pos.ctrl_index >= pi_Pos.ctrl_period) {
        pi_Pos.ctrl_index = 0;
        pi_Pos.pi_ref_reg = Pos_Set;
        pi_Pos.pi_fdb_reg = Angle_Fa_Q16;
        pi_Pos.calc(&pi_Pos);
    }
    Veloover_Set = pi_Pos.pi_out_reg;
}

void speedControl() {
    /* 速度环 */
    /* Veloover_Set：目标转速 */
    if (Veloover_Set - Velo_Set > Velo_PerAdd) {
        Velo_Set = Velo_Set + Velo_PerAdd;
    } else if (Velo_Set - Veloover_Set > Velo_PerAdd) {
        Velo_Set = Velo_Set - Velo_PerAdd;
    } else {
        Velo_Set = Veloover_Set;
    }

    pi_velo.ctrl_index++;
    /* pi_velo.ctrl_period = 10， 10 * 100us = 1ms控制一次？？？ */
    if (pi_velo.ctrl_index >= pi_velo.ctrl_period) {
        pi_velo.ctrl_index = 0;
        pi_velo.pi_ref_reg = Velo_Set;
        pi_velo.pi_fdb_reg = Velo_Elec;
        pi_velo.calc(&pi_velo);
    }

    // 控制电流上升、下降斜率
    if (IdSet >= 0) {
        // TODO: Isd_Set === 0 ???
        if (Isd_Set - IdSet > Is_PerAdd) {
            IdSet = IdSet + Is_PerAdd;
        } else if (IdSet - Isd_Set > Is_DownPerAdd) {
            IdSet = IdSet - Is_DownPerAdd;
        } else {
            IdSet = Isd_Set;
        }
    } else {
        if (Isd_Set - IdSet > Is_DownPerAdd) {
            IdSet = IdSet + Is_DownPerAdd;
        } else if (IdSet - Isd_Set > Is_PerAdd) {
            IdSet = IdSet - Is_PerAdd;
        } else {
            IdSet = Isd_Set;
        }
    }

    // TODO: TorqueAngleA === 0 ???， Id_Set === 0 ???
    Id_Set = -_IQmpy(_IQabs(pi_velo.pi_out_reg), _IQsinPU(TorqueAngleA)) + IdSet;

    // TODO:: Iq_Set = 速度环Pi输出
    Iq_Set = -_IQmpy(pi_velo.pi_out_reg, _IQcosPU(TorqueAngleA));
    Isdq_Set2 = Iq_Set; // Isdq_Set2 只在__speedControl()中使用
}

void __speedControl() {
    Velo_Set = Velo_Elec;
    Isdq_Set = Isdq_Set1;

    if (Isdq_Set2 >= 0) {
        // 控制电流上升、下降斜率
        if (Isdq_Set - Isdq_Set2 > Is_PerAdd) {
            Isdq_Set2 = Isdq_Set2 + Is_PerAdd;
        } else if (Isdq_Set2 - Isdq_Set > Is_DownPerAdd) {
            Isdq_Set2 = Isdq_Set2 - Is_DownPerAdd;
        } else {
            Isdq_Set2 = Isdq_Set;
        }
    } else {
        if (Isdq_Set - Isdq_Set2 > Is_DownPerAdd) {
            Isdq_Set2 = Isdq_Set2 + Is_DownPerAdd;
        } else if (Isdq_Set2 - Isdq_Set > Is_PerAdd) {
            Isdq_Set2 = Isdq_Set2 - Is_PerAdd;
        } else {
            Isdq_Set2 = Isdq_Set;
        }
    }

    if (IdSet >= 0) {
        // 控制电流上升、下降斜率
        if (Isd_Set - IdSet > Is_PerAdd) {
            IdSet = IdSet + Is_PerAdd;
        } else if (IdSet - Isd_Set > Is_DownPerAdd) {
            IdSet = IdSet - Is_DownPerAdd;
        } else {
            IdSet = Isd_Set;
        }
    } else {
        if (Isd_Set - IdSet > Is_DownPerAdd) {
            IdSet = IdSet + Is_DownPerAdd;
        } else if (IdSet - Isd_Set > Is_PerAdd) {
            IdSet = IdSet - Is_PerAdd;
        } else {
            IdSet = Isd_Set;
        }
    }

    /// 这里的TorqueAngle要不要调整
    Id_Set = -_IQmpy(_IQabs(Isdq_Set2), _IQsinPU(TorqueAngleA)) + IdSet;

    // 这里的TorqueAngle要不要调整
    Iq_Set = _IQmpy(Isdq_Set2, _IQcosPU(TorqueAngleA));
}

void fieldWeakeningControl(void) {
    pi_usLimit.ctrl_index++;
    if (pi_usLimit.ctrl_index >= pi_usLimit.ctrl_period) {
        pi_usLimit.ctrl_index = 0;
        // 这里相乘是什么意思？？
        pi_usLimit.pi_ref_reg = _IQmpy(Us_Max, UsSet);
        pi_usLimit.pi_fdb_reg = Us;
        pi_usLimit.calc(&pi_usLimit);
    }
}

void torqueContorl() {
    IqZ_Set = Iq_Set; // TODO: Iq_Set = 速度环Pi输出 ？？？

    // TODO： pi_usLimit.pi_out_reg === 0，Id_Set === 0 ?？？， Idz_Set === 0 ?？？
    IdZ_Set = Id_Set + pi_usLimit.pi_out_reg; // pi_usLimit：弱磁环输出，没有调用

    // idSet_Max: 最大电流，18
    if (_IQmag(IdZ_Set, IqZ_Set) > idSet_Max) {
        // 四象限的反正弦函数
        _iq Temp_Q = _IQasin4PU(IdZ_Set, IqZ_Set);

        // 按设定限的最大值输出Ud
        IdZ_Set = _IQmpy(idSet_Max, _IQcosPU(Temp_Q));

        // 按设定限幅的最大值输出Uq
        IqZ_Set = _IQmpy(idSet_Max, _IQsinPU(Temp_Q));
    }

    // Id的PI调节器
    pi_Id_Kc.ctrl_index++;
    /* pi_Id_Kc.ctrl_period = 1，每次都计算（100us周期） */
    if (pi_Id_Kc.ctrl_index >= pi_Id_Kc.ctrl_period) {
        pi_Id_Kc.ctrl_index = 0;
        pi_Id_Kc.pi_ref_reg = IdZ_Set;
        pi_Id_Kc.pi_fdb_reg = Id; /* clark + park 变换后的 d 轴电流 */
        pi_Id_Kc.calc(&pi_Id_Kc);

        Uint16 isIntGreaterMaxInt = pi_Id_Kc.ui_out_max <= pi_Id_Kc.ui_reg;
        Uint16 isIntLessMinInt = pi_Id_Kc.ui_reg <= pi_Id_Kc.ui_out_min;
        if (isIntGreaterMaxInt || isIntLessMinInt) {
            Sys_Flag.bit.Flag_Id = 1; /* Id限幅 */
        } else {
            Sys_Flag.bit.Flag_Id = 0;
        }
    }

    //--------------------------------------------------------------
    // Iq的PI调节器
    pi_Iq_Kc.ctrl_index++;
    /* pi_Iq_Kc.ctrl_period = 1，每次都计算（100us周期） */
    if (pi_Iq_Kc.ctrl_index >= pi_Iq_Kc.ctrl_period) {
        pi_Iq_Kc.ctrl_index = 0;
        pi_Iq_Kc.pi_ref_reg = IqZ_Set;
        pi_Iq_Kc.pi_fdb_reg = Iq;
        pi_Iq_Kc.calc(&pi_Iq_Kc);

        Uint16 isIntGreaterMaxInt = pi_Iq_Kc.ui_out_max <= pi_Iq_Kc.ui_reg;
        Uint16 isIntLessMinInt = pi_Iq_Kc.ui_reg <= pi_Iq_Kc.ui_out_min;
        if (isIntGreaterMaxInt || isIntLessMinInt) {
            Sys_Flag.bit.Flag_Iq = 1; /* Iq 限幅 */
        } else {
            Sys_Flag.bit.Flag_Iq = 0;
        }
    }

    // 控制方程 求得给出的Ud,Uq 电压方程式,得到Vd,Vq
    Ud = _IQmpy(pi_Id_Kc.ui_reg, Radq) + _IQmpy(Velo_Elec, _IQmpy(Lq, pi_Iq_Kc.ui_reg)) + _IQmpy(pi_Id_Kc.ui_reg, Rs) + pi_Id_Kc.up_reg - _IQmpy(Id, Radq);
    Uq = _IQmpy(pi_Iq_Kc.ui_reg, Radq) - _IQmpy(Velo_Elec, PHI + _IQmpy(Ld, pi_Id_Kc.ui_reg)) + _IQmpy(pi_Iq_Kc.ui_reg, Rs) + pi_Iq_Kc.up_reg - _IQmpy(Iq, Radq);

    //	Ud,Uq限幅
    Udc_Mche = Udc_Setg; // Udc_Setg是上位机设置的直流电压的倒数

    Us = _IQmag(Ud, Uq);
    Us_Max = _IQmpy(Udc_Mche_realvalue, _IQ(0.70710678118654752440084436210485));
    Us_Max = _IQmpy(Us_Max, Us_M);

    if (Us > Us_Max) {
        _iq Temp_Q = _IQasin4PU(Ud, Uq);       // 四象限的反正弦函数
        Ud = _IQmpy(Us_Max, _IQcosPU(Temp_Q)); // 按设定限幅的最大值输出Ud
        Uq = _IQmpy(Us_Max, _IQsinPU(Temp_Q)); // 按设定限幅的最大值输出Uq
        Us = _IQmag(Ud, Uq);
        Sys_Flag.bit.Flag_Us = 1;
    } else {
        Sys_Flag.bit.Flag_Us = 0;
    }

    dqtoabc.dr = Ud;
    dqtoabc.qr = Uq;
    dqtoabc.ang = Angle_Elec + _IQmpy(Com_angle_U, Velo_Elec);

    DQ2ABC_CALC();
    Usa = dqtoabc.as;
    Usb = dqtoabc.bs;
    Usc = dqtoabc.cs;

    phase_trans.as = Usa;
    phase_trans.bs = Usb;
    phase_trans.cs = Usc;
    THREE2TWO_CALC();
    Us_Alfa_Mche = phase_trans.ds;
    Us_Beta_Mche = phase_trans.qs;

    SVPWMA_Gen(Us_Alfa_Mche, Us_Beta_Mche);
}

//--------------------------------------------------------------------------
// 电动状态下的控制程序
//--------------------------------------------------------------------------
void Main_ControlA(void) {
    /* 进入位置控制程序 */
    Uint16 isPositionControl = Ctrl_Flag.bit.POS_CONTROL_FLAG == 1;
    if (isPositionControl) {
        positionControl();
    }

    /* 进入速度控制程序 */
    Uint16 isSpeedControl = Ctrl_Flag.bit.VELO_CONTROL_FLAG == 1;
    if (isSpeedControl) {
        speedControl();
    } else {
        // TODO： 条件达不到？？？
        __speedControl();
    }

    /* 弱磁控制 */
    Uint16 isFieldWeakeningControl = Ctrl_Flag.bit.UsCtrl_Flag == 1;
    if (isFieldWeakeningControl) {
        fieldWeakeningControl();
    }

    /* 转矩控制标志 */
    Uint16 isTorqueControl = Ctrl_Flag.bit.TORQ_CONTROL_FLAG == 1;
    if (isTorqueControl) {
        torqueContorl();
    }
}

Uint16 isClearStopPwmFlag() {
    Uint16 isCanError = Ctrl_Flag.bit.CANERROR_FLAG == 0;
    Uint16 isStopPWMFlagVelocity = Sys_Flag.bit.STOP_PWM_Flag_Velo == 0;
    Uint16 isTemp1Over = Ctrl_Flag.bit.Temp1Over_Flag == 0;
    Uint16 isTemp2Over = Ctrl_Flag.bit.Temp2Over_Flag == 0;
    Uint16 isIdcOver = Sys_Flag.bit.STOP_PWM_Flag_Id == 0;
    Uint16 isUdcFault = Sys_Flag.bit.UDC_FLAG == 0;
    Uint16 isStopPWMFlagDriv = Sys_Flag.bit.STOP_PWM_Flag_Driv == 0;
    Uint16 isUdcLow = Ctrl_Flag.bit.UdcLow_Flag == 0;
    Uint16 isRdc1 = Fault_Flag.bit.RDC1_FAULT == 0;
    Uint16 isRdc2 = Fault_Flag.bit.RDC2_FAULT == 0;

    Uint16 isCtrlFault = isCanError && isTemp1Over && isTemp2Over && isUdcLow;
    Uint16 isSysFault = isStopPWMFlagVelocity && isIdcOver && isUdcFault;
    Uint16 isFault = isRdc1 && isRdc2;

    return isCtrlFault || isSysFault || isFault;
}

// 控制周期计算程序 (100uS)  电动状态
void ControlA(void) {
    hardwareProtection(); // 硬件保护

    /* 清除故障停机标志 */
    if (isClearStopPwmFlag()) {
        Ctrl_Flag.bit.STOP_PWM_Flag = 0;
    }

    /* 进入停机程序， 无法进入？？？ */
    if (Ctrl_Flag.bit.STOP_VELO_FLAG == 1) {
        Uint16 isLock = Ctrl_Flag.bit.LOCK_FLAG == 1;
        Uint16 isOverSpeedProtect = Sys_Flag.bit.STOP_PWM_Flag_Velo == 0;
        Uint16 isIdOverProtect = Sys_Flag.bit.STOP_PWM_Flag_Id == 0;
        Uint16 isPowerProtect = Sys_Flag.bit.STOP_PWM_Flag_Driv == 0;
        Uint16 isOpenLoop = Ctrl_Flag.bit.OPEN_LOOP_FLAG == 1;

        if (isLock && !isOverSpeedProtect && !isIdOverProtect && !isPowerProtect) {
            PWMA_Update();
            MeasureInitDigital();
        } else if (isOpenLoop && !isOverSpeedProtect && !isIdOverProtect && !isPowerProtect) {
            PWMA_Update();
            OpenLoop();
        } else {
            pwma.mDuty1 = _IQ(0.5);
            pwma.mDuty2 = _IQ(0.5);
            pwma.mDuty3 = _IQ(0.5);
            Forbid_PWMA();
            Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 0;
            Ctrl_Flag.bit.VELO_CONTROL_FLAG = 0;
            Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.LOCK_FLAG = 0;
            Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
            Id_Set = _IQ(0);
            Iq_Set = _IQ(0);
            IdZ_Set = _IQ(0);
            IqZ_Set = _IQ(0);
            Isd_Set = IsLimit_Set;
            IdSet = _IQ(0);
            Isdq_Set = _IQ(0);
            Isdq_Set1 = _IQ(0);
            Isdq_Set2 = _IQ(0);
            Velo_Set = Velo_Elec;
            Veloover_Set = _IQ(0);
            TorqueAngleA = TorqueAngle;
            pi_usLimit.pi_out_reg = _IQ(0);
            pi_Pos.pi_out_reg = _IQ(0);
            pi_velo.pi_out_reg = _IQ(0); // 速度环的PI
            pi_Id_Kc.pi_out_reg = _IQ(0);
            pi_Iq_Kc.pi_out_reg = _IQ(0);

            pi_usLimit.ui_reg = _IQ(0); // Vdc环的PI
            pi_Pos.ui_reg = _IQ(0);
            pi_velo.ui_reg = _IQ(0); // 速度环的PI
            pi_Id_Kc.ui_reg = _IQ(0);
            pi_Iq_Kc.ui_reg = _IQ(0);
        }
        Ctrl_Flag.bit.FIRST_PWM_Flag = 1;
    } else {
        Ctrl_Flag.bit.FIRST_PWM_Flag = 0;
        Ctrl_Flag.bit.LOCK_FLAG = 0;
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
        Main_ControlA();
        PWMA_Update();
    }
}

void Control(void) {
    if (Ctrl_Flag.bit.STOP_VELO_FLAG == 1) {
        if (Ctrl_Flag.bit.LOCK_FLAG == 0 && Ctrl_Flag.bit.OPEN_LOOP_FLAG == 0) {
            pwma.mDuty1 = _IQ(0.5);
            pwma.mDuty2 = _IQ(0.5);
            pwma.mDuty3 = _IQ(0.5);
            Forbid_PWMA();
            pwma.mHalfPeriod = Control_Period;
            EPwm1Regs.TBPRD = (Uint16)Control_Period;
            EPwm2Regs.TBPRD = (Uint16)Control_Period;
            EPwm3Regs.TBPRD = (Uint16)Control_Period;
            EPwm1Regs.DBRED = Deadtime; /*上升沿延时死区时间：DBRED*TBCLK*/
            EPwm1Regs.DBFED = Deadtime; /*下降沿延时死区时间：DBFED*TBCLK*/
            EPwm2Regs.DBRED = Deadtime; /*上升沿延时死区时间：DBRED*TBCLK*/
            EPwm2Regs.DBFED = Deadtime; /*下降沿延时死区时间：DBFED*TBCLK*/
            EPwm3Regs.DBRED = Deadtime; /*上升沿延时死区时间：DBRED*TBCLK*/
            EPwm3Regs.DBFED = Deadtime; /*下降沿延时死区时间：DBFED*TBCLK*/
            if (Velo_Duty0 != Velo_Duty) {
                Velo_Duty0 = Velo_Duty;
                K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
                K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0);
                K_Velo_Cal = _IQdiv(K_Velo_Cal, Velo_Calc_Num << 20);
                K_Velo_Cal = _IQmpy(K_Velo_Cal, p);

                Per_Ctrl = _IQdiv(Control_Period, 37500000);
                Velo_PerAdd = _IQmpy(Velo_Add, Per_Ctrl);
                Is_PerAdd = _IQmpy(Is_Add, Per_Ctrl);
                Is_DownPerAdd = _IQmpy(Is_DownAdd, Per_Ctrl);

                pi_Id_Kc.Ki_reg = _IQmpy(V_Base, Per_Ctrl);
                pi_Id_Kc.Ki_reg = _IQmpy(pi_Id_Kc.Ki_reg, (pi_Id_Kc.i_period << 20));
                pi_Id_Kc.Ki_reg = _IQmpy(Idq_Kc, pi_Id_Kc.Ki_reg);
                pi_Iq_Kc.Ki_reg = pi_Id_Kc.Ki_reg;
            }
        }
    }

    Ready_Data();
    ControlA();
    RunLed();
}
