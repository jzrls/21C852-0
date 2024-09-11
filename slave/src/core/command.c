#include "command.h"

void getResolver() {}

void setControlParams(_iq *sendArray) {
    sendArray[0] = pi_velo.Kp_reg;      /* �ٶȻ�����ϵ�� */
    sendArray[3] = pi_velo.Ki_reg;      /* �ٶȻ�����ϵ�� */
    sendArray[6] = pi_velo.pi_out_max;  /* �ٶȻ��޷�ֵ */
    sendArray[8] = pi_velo.i_period;    /* �ٶȻ��������� */
    sendArray[9] = pi_velo.ctrl_period; /* �ٶȻ��������� */

    sendArray[1] = pi_usLimit.Kp_reg;      /* �Զ����Ż�����ϵ�� */
    sendArray[2] = pi_usLimit.Ki_reg;      /* �Զ����Ż�����ϵ�� */
    sendArray[7] = -pi_usLimit.pi_out_min; /* �Զ����Ż��޷�ֵ */
    sendArray[5] = pi_usLimit.i_period;    /* �Զ����Ż��������� */
    sendArray[4] = pi_usLimit.ctrl_period; /* �Զ����Ż��������� */

    sendArray[10] = pi_Pos.Kp_reg;      /* λ�û�����ϵ�� */
    sendArray[13] = pi_Pos.Ki_reg;      /* λ�û�����ϵ�� */
    sendArray[16] = pi_Pos.pi_out_max;  /* λ�û��޷�ֵ */
    sendArray[18] = pi_Pos.i_period;    /* λ�û��������� */
    sendArray[19] = pi_Pos.ctrl_period; /* λ�û��������� */

    sendArray[11] = Idq_Kc;               /* ������Kcϵ�� */
    sendArray[12] = Radq;                 /* ����������ϵ�� */
    sendArray[17] = pi_Id_Kc.pi_out_max;  /* �������޷�ֵ */
    sendArray[15] = pi_Id_Kc.i_period;    /* �������������� */
    sendArray[14] = pi_Id_Kc.ctrl_period; /* �������������� */
}

void getControlParams(_iq *recv) {
    pi_velo.Kp_reg = recv[0];      /* �ٶȻ�����ϵ�� */
    pi_velo.Ki_reg = recv[3];      /* �ٶȻ�����ϵ�� */
    pi_velo.ui_out_max = recv[6];  /* �ٶȻ��޷�ֵ */
    pi_velo.pi_out_max = recv[6];  /* �ٶȻ��޷�ֵ */
    pi_velo.ui_out_min = -recv[6]; /* �ٶȻ��޷�ֵ */
    pi_velo.pi_out_min = -recv[6]; /* �ٶȻ��޷�ֵ */
    pi_velo.i_period = recv[8];    /* �ٶȻ��������� */
    pi_velo.ctrl_period = recv[9]; /* �ٶȻ��������� */

    pi_usLimit.Kp_reg = recv[1]; /* �Զ����Ż�����ϵ�� */
    pi_usLimit.Ki_reg = recv[2]; /* �Զ����Ż�����ϵ�� */
    pi_usLimit.ui_out_max = 0;
    pi_usLimit.pi_out_max = 0;
    pi_usLimit.ui_out_min = -recv[7]; /* �Զ����Ż��޷�ֵ */
    pi_usLimit.pi_out_min = -recv[7]; /* �Զ����Ż��޷�ֵ */
    pi_usLimit.i_period = recv[5];    /* �Զ����Ż��������� */
    pi_usLimit.ctrl_period = recv[4]; /* �Զ����Ż��������� */

    pi_Pos.Kp_reg = recv[10];      /* λ�û�����ϵ�� */
    pi_Pos.Ki_reg = recv[13];      /* λ�û�����ϵ�� */
    pi_Pos.ui_out_max = recv[16];  /* λ�û��޷�ֵ */
    pi_Pos.pi_out_max = recv[16];  /* λ�û��޷�ֵ */
    pi_Pos.ui_out_min = -recv[16]; /* λ�û��޷�ֵ */
    pi_Pos.pi_out_min = -recv[16]; /* λ�û��޷�ֵ */
    pi_Pos.i_period = recv[18];    /* λ�û��������� */
    pi_Pos.ctrl_period = recv[19]; /* λ�û��������� */

    Idq_Kc = recv[11];               /* ������Kcϵ�� */
    Radq = recv[12];                 /* ����������ϵ�� */
    pi_Id_Kc.i_period = recv[15];    /* �������������� */
    pi_Id_Kc.ctrl_period = recv[14]; /* �������������� */
    pi_Id_Kc.ui_out_max = recv[17];  /* �������޷�ֵ */
    pi_Id_Kc.pi_out_max = recv[17];  /* �������޷�ֵ */
    pi_Id_Kc.ui_out_min = -recv[17]; /* �������޷�ֵ */
    pi_Id_Kc.pi_out_min = -recv[17]; /* �������޷�ֵ */
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
    Rs = recv[0];                              /* ����������ֵ */
    Ld = recv[1];                              /* ���d���б���ֵ */
    Lq = recv[2];                              /* ���q���б���ֵ */
    PHI = recv[3];                             /* �����������ֵ */
    Mp = recv[4];                              /* ��������� */
    Rp = recv[5];                              /* ���伫���� */
    Udc_Setg = recv[6];                        /* ֱ����ѹ */
    U_Base = recv[7];                          /* ��ѹ��ֵ */
    I_Base = recv[8];                          /* ������ֵ */
    V_Base = recv[9];                          /* ���ٶȻ�ֵ(rad/s) */
    Angle_Init_Digital = recv[10];             /* ������䰲װλ�� */
    Velo_Max = recv[11];                       /* �ٶ��޷�ֵ */
    id_Max = recv[12];                         /* �����޷�ֵ */
    Is_Add = recv[13];                         /* ���������� */
    Control_Period = recv[14];                 /* �ز�����(us)*37.5 */
    id_Filter = recv[15];                      /* Idq�˲�Ƶ�� */
    Udc_Filter = recv[16];                     /* ֱ����ѹ�˲� */
    Velo_Filter = recv[17];                    /* ת���˲�Ƶ�� */
    Deadtime = recv[18];                       /* ����ʱ��(us)*75 */
    Velo_Add = recv[19];                       /* �ٶ������� */
    Is_DownAdd = recv[20];                     /* �����½��� */
    Temp_Filter = recv[21];                    /* �¶��˲�Ƶ�� */
    Angle_Init_Fa = recv[22];                  /* ������λ�趨λ�� */
    Com_angle_U = recv[23];                    /* ���Ƶ�ѹ������/360 */
    Com_angle_I = recv[24];                    /* ������ⲹ����/360*/
    idSet_Max = recv[25];                      /* �������趨ֵ */
    IsLimit_Set = recv[27];                    /* ��ʼId����ֵ */
    MotorTM = recv[28];                        /* ����¶ȱ���ֵ */
    igbtTemperatureProtectionValue = recv[29]; /* IGBT�¶ȱ���ֵ */
}

void getAnalogParams(_iq *recv) {
    K_Udc = recv[0];    /* Vdcͨ���Ŵ�ϵ�� */
    Off_Udc = recv[1];  /* Vdcͨ��ƫ���� */
    K_Isa = recv[2];    /* Isaͨ���Ŵ�ϵ�� */
    Off_Isa = recv[3];  /* Isaͨ��ƫ���� */
    K_Isc = recv[4];    /* Iscͨ���Ŵ�ϵ�� */
    Off_Isc = recv[5];  /* Iscͨ��ƫ���� */
    K_Idc = recv[8];    /* Idcͨ���Ŵ�ϵ�� */
    Off_Idc = recv[9];  /* Idcͨ��ƫ���� */
    PWMA_a = recv[12];  /* A��ռ�ձȵ��� */
    PWMA_b = recv[13];  /* B��ռ�ձȵ��� */
    PWMA_c = recv[14];  /* C��ռ�ձȵ��� */
    Us_M = recv[22];    /* ������ϵ�� */
    K_Isb = recv[23];   /* Isbͨ���Ŵ�ϵ�� */
    Off_Isb = recv[24]; /* Isbͨ��ƫ���� */
}

void getOtherParams(_iq *recv) {
    Temperature1_K = recv[0];               /* ����¶�1����У��ϵ�� */
    UdcLimit1_Set = recv[1];                /* ֱ����ѹ���� */
    ProtectNum = recv[4];                   /* ���ϻָ����� */
    UsSet = recv[5];                        /* �ܵ�ѹϵ�� */
    TorqueAngle = recv[7];                  /* �����Ƕ�/360(ת�ؽ�)  */
    Temperature3_K = recv[10];              /* ����¶�2����У��ϵ�� */
    Ctrl_Flag.bit.RotorWay_Flag = recv[12]; /* ���䷽���־ */
    UdcLimit_Set = recv[14];                /* ֱ����ѹ���� */
    Velo_Start = recv[15];                  /* �����ٶ� */
    maxSpeed = recv[16];                    /* ���ת�� */
    Ctrl_Flag.bit.VelWay_Flag = recv[17];   /* ת�ٿ��Ʒ�ʽ��־ */
    TorquetoIs = recv[18];                  /*ת�ص����� */
    IdProtectNum = recv[19];                /* ������������ */
    VeloProtectNum = recv[20];              /* ת�ٱ������� */
    Velo_Calc_Num = recv[21];               /* ת�ټ������ */
    Temperature1_Off = recv[22];            /* ����¶�1ƫ���� */
    Temperature3_Off = recv[24];            /* ����¶�2ƫ���� */
    Ctrl_Flag.bit.UsCtrl_Flag = recv[27];   /* �ܵ�ѹ��־ */
    Temperature5_K = recv[28];              /* �������¶ȱ���У��ϵ�� */
    Temperature5_Off = recv[29];            /* �������¶�ƫ���� */

    IstoTorque = _IQdiv(_IQ(1), TorquetoIs);
    K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
    K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0); // ��ʱ������Ϊ100us
    K_Velo_Cal = _IQdiv(K_Velo_Cal, Velo_Calc_Num << 20);
    K_Velo_Cal = _IQmpy(K_Velo_Cal, p);
}
