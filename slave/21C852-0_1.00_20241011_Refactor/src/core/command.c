#include "command.h"

void getResolver() {}

void setControlParams(_iq *sendArray) {
    sendArray[0] = pi_velo.Kp_reg;      /* 速度环比例系数 */
    sendArray[3] = pi_velo.Ki_reg;      /* 速度环积分系数 */
    sendArray[6] = pi_velo.pi_out_max;  /* 速度环限幅值 */
    sendArray[8] = pi_velo.i_period;    /* 速度环积分周期 */
    sendArray[9] = pi_velo.ctrl_period; /* 速度环控制周期 */

    sendArray[1] = pi_usLimit.Kp_reg;      /* 自动弱磁环比例系数 */
    sendArray[2] = pi_usLimit.Ki_reg;      /* 自动弱磁环积分系数 */
    sendArray[7] = -pi_usLimit.pi_out_min; /* 自动弱磁环限幅值 */
    sendArray[5] = pi_usLimit.i_period;    /* 自动弱磁环积分周期 */
    sendArray[4] = pi_usLimit.ctrl_period; /* 自动弱磁环控制周期 */

    sendArray[10] = pi_Pos.Kp_reg;      /* 位置环比例系数 */
    sendArray[13] = pi_Pos.Ki_reg;      /* 位置环积分系数 */
    sendArray[16] = pi_Pos.pi_out_max;  /* 位置环限幅值 */
    sendArray[18] = pi_Pos.i_period;    /* 位置环积分周期 */
    sendArray[19] = pi_Pos.ctrl_period; /* 位置环控制周期 */

    sendArray[11] = Idq_Kc;               /* 电流环Kc系数 */
    sendArray[12] = Radq;                 /* 电流环阻尼系数 */
    sendArray[17] = pi_Id_Kc.pi_out_max;  /* 电流环限幅值 */
    sendArray[15] = pi_Id_Kc.i_period;    /* 电流环积分周期 */
    sendArray[14] = pi_Id_Kc.ctrl_period; /* 电流环控制周期 */
}

void getControlParams(_iq *recv) {
    pi_velo.Kp_reg = recv[0];      /* 速度环比例系数 */
    pi_velo.Ki_reg = recv[3];      /* 速度环积分系数 */
    pi_velo.ui_out_max = recv[6];  /* 速度环限幅值 */
    pi_velo.pi_out_max = recv[6];  /* 速度环限幅值 */
    pi_velo.ui_out_min = -recv[6]; /* 速度环限幅值 */
    pi_velo.pi_out_min = -recv[6]; /* 速度环限幅值 */
    pi_velo.i_period = recv[8];    /* 速度环积分周期 */
    pi_velo.ctrl_period = recv[9]; /* 速度环控制周期 */

    pi_usLimit.Kp_reg = recv[1]; /* 自动弱磁环比例系数 */
    pi_usLimit.Ki_reg = recv[2]; /* 自动弱磁环积分系数 */
    pi_usLimit.ui_out_max = 0;
    pi_usLimit.pi_out_max = 0;
    pi_usLimit.ui_out_min = -recv[7]; /* 自动弱磁环限幅值 */
    pi_usLimit.pi_out_min = -recv[7]; /* 自动弱磁环限幅值 */
    pi_usLimit.i_period = recv[5];    /* 自动弱磁环积分周期 */
    pi_usLimit.ctrl_period = recv[4]; /* 自动弱磁环控制周期 */

    pi_Pos.Kp_reg = recv[10];      /* 位置环比例系数 */
    pi_Pos.Ki_reg = recv[13];      /* 位置环积分系数 */
    pi_Pos.ui_out_max = recv[16];  /* 位置环限幅值 */
    pi_Pos.pi_out_max = recv[16];  /* 位置环限幅值 */
    pi_Pos.ui_out_min = -recv[16]; /* 位置环限幅值 */
    pi_Pos.pi_out_min = -recv[16]; /* 位置环限幅值 */
    pi_Pos.i_period = recv[18];    /* 位置环积分周期 */
    pi_Pos.ctrl_period = recv[19]; /* 位置环控制周期 */

    Idq_Kc = recv[11];               /* 电流环Kc系数 */
    Radq = recv[12];                 /* 电流环阻尼系数 */
    pi_Id_Kc.i_period = recv[15];    /* 电流环积分周期 */
    pi_Id_Kc.ctrl_period = recv[14]; /* 电流环控制周期 */
    pi_Id_Kc.ui_out_max = recv[17];  /* 电流环限幅值 */
    pi_Id_Kc.pi_out_max = recv[17];  /* 电流环限幅值 */
    pi_Id_Kc.ui_out_min = -recv[17]; /* 电流环限幅值 */
    pi_Id_Kc.pi_out_min = -recv[17]; /* 电流环限幅值 */
    pi_Id_Kc.Kp_reg = _IQmpy(Idq_Kc, Ld);
    pi_Id_Kc.Ki_reg = _IQmpy(V_Base, Per_Ctrl);
    pi_Id_Kc.Ki_reg = _IQmpy(pi_Id_Kc.Ki_reg, (pi_Id_Kc.i_period << 20));
    pi_Id_Kc.Ki_reg = _IQmpy(Idq_Kc, pi_Id_Kc.Ki_reg);

    pi_Iq_Kc.i_period = pi_Id_Kc.i_period;
    pi_Iq_Kc.ctrl_period = pi_Id_Kc.ctrl_period;
    pi_Iq_Kc.Kp_reg = _IQmpy(Idq_Kc, Lq);
    pi_Iq_Kc.Ki_reg = pi_Id_Kc.Ki_reg;
    pi_Iq_Kc.ui_out_max = pi_Id_Kc.ui_out_max;
    pi_Iq_Kc.pi_out_max = pi_Id_Kc.pi_out_max;
    pi_Iq_Kc.ui_out_min = pi_Id_Kc.ui_out_min;
    pi_Iq_Kc.pi_out_min = pi_Id_Kc.pi_out_min;
}

void getMotorParams(_iq *recv) {
    Rs = recv[0];                              /* 电机电阻标幺值 */
    Ld = recv[1];                              /* 电机d轴电感标幺值 */
    Lq = recv[2];                              /* 电机q轴电感标幺值 */
    PHI = recv[3];                             /* 电机磁链标幺值 */
    Mp = recv[4];                              /* 电机极对数 */
    Rp = recv[5];                              /* 旋变极对数 */
    Udc_Setg = recv[6];                        /* 直流电压 */
    U_Base = recv[7];                          /* 电压基值 */
    I_Base = recv[8];                          /* 电流基值 */
    V_Base = recv[9];                          /* 电速度基值(rad/s) */
    Angle_Init_Digital = recv[10];             /* 电机旋变安装位置 */
    Velo_Max = recv[11];                       /* 速度限幅值 */
    id_Max = recv[12];                         /* 电流限幅值 */
    Is_Add = recv[13];                         /* 电流增加率 */
    Control_Period = recv[14];                 /* 载波周期(us)*37.5 */
    id_Filter = recv[15];                      /* Idq滤波频率 */
    Udc_Filter = recv[16];                     /* 直流电压滤波 */
    Velo_Filter = recv[17];                    /* 转速滤波频率 */
    Deadtime = recv[18];                       /* 死区时间(us)*75 */
    Velo_Add = recv[19];                       /* 速度增加率 */
    Is_DownAdd = recv[20];                     /* 电流下降率 */
    Temp_Filter = recv[21];                    /* 温度滤波频率 */
    Angle_Init_Fa = recv[22];                  /* 阀门零位设定位置 */
    Com_angle_U = recv[23];                    /* 控制电压补偿角/360 */
    Com_angle_I = recv[24];                    /* 电流检测补偿角/360*/
    idSet_Max = recv[25];                      /* 最大电流设定值 */
    IsLimit_Set = recv[27];                    /* 初始Id标幺值 */
    MotorTM = recv[28];                        /* 电机温度保护值 */
    igbtTemperatureProtectionValue = recv[29]; /* IGBT温度保护值 */
}

void getAnalogParams(_iq *recv) {
    K_Udc = recv[0];    /* Vdc通道放大系数 */
    Off_Udc = recv[1];  /* Vdc通道偏移量 */
    K_Isa = recv[2];    /* Isa通道放大系数 */
    Off_Isa = recv[3];  /* Isa通道偏移量 */
    K_Isc = recv[4];    /* Isc通道放大系数 */
    Off_Isc = recv[5];  /* Isc通道偏移量 */
    K_Idc = recv[8];    /* Idc通道放大系数 */
    Off_Idc = recv[9];  /* Idc通道偏移量 */
    PWMA_a = recv[12];  /* A相占空比调整 */
    PWMA_b = recv[13];  /* B相占空比调整 */
    PWMA_c = recv[14];  /* C相占空比调整 */
    Us_M = recv[22];    /* 过调制系数 */
    K_Isb = recv[23];   /* Isb通道放大系数 */
    Off_Isb = recv[24]; /* Isb通道偏移量 */
}

void getOtherParams(_iq *recv) {
    Temperature1_K = recv[0];               /* 电机温度1比例校正系数 */
    UdcLimit1_Set = recv[1];                /* 直流电压下限 */
    ProtectNum = recv[4];                   /* 故障恢复点数 */
    UsSet = recv[5];                        /* 总电压系数 */
    TorqueAngle = recv[7];                  /* 电流角度/360(转矩角)  */
    Temperature3_K = recv[10];              /* 电机温度2比例校正系数 */
    Ctrl_Flag.bit.RotorWay_Flag = recv[12]; /* 旋变方向标志 */
    UdcLimit_Set = recv[14];                /* 直流电压上限 */
    Velo_Start = recv[15];                  /* 启动速度 */
    maxSpeed = recv[16];                    /* 最高转速 */
    Ctrl_Flag.bit.VelWay_Flag = recv[17];   /* 转速控制方式标志 */
    TorquetoIs = recv[18];                  /*转矩电流比 */
    IdProtectNum = recv[19];                /* 电流保护点数 */
    VeloProtectNum = recv[20];              /* 转速保护点数 */
    Velo_Calc_Num = recv[21];               /* 转速计算点数 */
    Temperature1_Off = recv[22];            /* 电机温度1偏移量 */
    Temperature3_Off = recv[24];            /* 电机温度2偏移量 */
    Ctrl_Flag.bit.UsCtrl_Flag = recv[27];   /* 总电压标志 */
    Temperature5_K = recv[28];              /* 驱动板温度比例校正系数 */
    Temperature5_Off = recv[29];            /* 驱动板温度偏移量 */

    IstoTorque = _IQdiv(_IQ(1), TorquetoIs);
    K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
    K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0); // 定时器周期为100us
    K_Velo_Cal = _IQdiv(K_Velo_Cal, Velo_Calc_Num << 20);
    K_Velo_Cal = _IQmpy(K_Velo_Cal, p);
}
