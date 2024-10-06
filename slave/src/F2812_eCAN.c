#include "F28335_Device.h"
#include "App_UserDef.h"

#include "core\command.h"

#pragma CODE_SECTION(ECana_T, "ramfuncs");
#pragma CODE_SECTION(Can_Tx_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx2_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx4_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx11_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Tx13_Data, "ramfuncs");
#pragma CODE_SECTION(Can_Rt_Data, "ramfuncs");

void Can_Rt_Data(Uint16 Num) {
    volatile _iq tmp = 0;
    static _iq sumSpeed = 0;
    static Uint16 index = 0;
    _iq speed = 0;

    static _iq speedArray[16] = {0};

    if (Num == 1) {
        Uint16 isPump = BORAD_NUM == 2 || BORAD_NUM == 3;
        if (isPump) {
            // ��ת�ٱ���ֵת��ΪIQ15��ʵ��ת��ֵ����ת��Ϊ16λ����,���ֽ���ǰ
            tmp = _IQmpy(Velo_Elec_abs, _IQ(9.5492965855137201461330258023509));
            tmp = _IQdiv(tmp, Mp);
            tmp = _IQmpy(tmp, V_Base >> 5);
            tmp = (tmp >> 15);
            if (tmp >= 65535) {
                tmp = 65535;
            }

            /**
             * ��tmp����16�ξ�ֵ�˲�����
             */
            // sumSpeed += tmp;
            // index++;
            // if (index >= 16) {
            //     speed = sumSpeed >> 4;
            //     sumSpeed = 0;
            //     index = 0;
            // }

            if (index < 16) {
                speedArray[index] = tmp;
                index++;

                for (int i = 0; i < index - 1; i++) {
                    sumSpeed += speedArray[i];
                }
                speed = sumSpeed / index;

            } else {
                for (int i = 0; i < 15; i++) {
                    speedArray[i] = speedArray[i + 1];
                }
                speedArray[15] = tmp;
                for (int i = 0; i < index - 1; i++) {
                    sumSpeed += speedArray[i];
                }
                speed = sumSpeed / index;
            }

            CAN_BUFFER_RT[BORAD_NUM][0] = speed & 0x00FF;
            CAN_BUFFER_RT[BORAD_NUM][1] = (speed >> 8) & 0x00FF;

            Fault_Flag.bit.POS_LIMIT = 0;
        } else {
            /**
             * ����λ��
             *
             * ��16λ��λ��ת��Ϊ�Ƕ�ֵ����*100ת��Ϊ16λ����,���ֽ���ǰ
             *
             * 360���Ӧ65535��360 / 65535
             * ʵ�ʽǶ������Ƕ���100�Ķ���ϵ��
             */
            tmp = Angle_Fa_Q16;
            tmp = _IQmpy(tmp, _IQ(0.54931640625)); // 360/65536*100
            if (tmp >= 65535) {
                tmp = 65535;
            }
            CAN_BUFFER_RT[BORAD_NUM][0] = tmp & 0x00FF;
            CAN_BUFFER_RT[BORAD_NUM][1] = (tmp >> 8) & 0x00FF;

            /**
             * �����λ
             *
             * ����65000����С��500
             */
            if ((Angle_Fa_Q16 > POSMAX) || (Angle_Fa_Q16 < POSMIN)) {
                Fault_Flag.bit.POS_LIMIT = 1;
            } else {
                Fault_Flag.bit.POS_LIMIT = 0;
            }
        }

        /**
         * ����λ
         */
        Fault_Flag.bit.STOP_VELO_FLAG = Ctrl_Flag.bit.STOP_VELO_FLAG;
        Fault_Flag.bit.STOP_PWM_Flag = Ctrl_Flag.bit.STOP_PWM_Flag;

        /* ĸ�߹������� */
        Fault_Flag.bit.STOP_PWM_Flag_Id = Sys_Flag.bit.STOP_PWM_Flag_Id;

        /* ���ٹ��� */
        Fault_Flag.bit.STOP_PWM_Flag_Velo = Sys_Flag.bit.STOP_PWM_Flag_Velo;

        /* Ӳ������ */
        Fault_Flag.bit.STOP_PWM_Flag_Driv = Sys_Flag.bit.STOP_PWM_Flag_Driv;

        Fault_Flag.bit.UDC_FLAG = Sys_Flag.bit.UDC_FLAG; /* ĸ�߹�ѹ���� */
        Fault_Flag.bit.Temp1Over_Flag = Ctrl_Flag.bit.Temp1Over_Flag;
        Fault_Flag.bit.Temp2Over_Flag = Ctrl_Flag.bit.Temp2Over_Flag;

        Fault_Flag.bit.UdcLow_Flag = Ctrl_Flag.bit.UdcLow_Flag; /* Ƿѹ���� */
        Fault_Flag.bit.CANERROR_FLAG = Ctrl_Flag.bit.CANERROR_FLAG;
        Fault_Flag.bit.RunAllow_Flag = RunAllow_Flag;
        CAN_BUFFER_RT[BORAD_NUM][2] = (Fault_Flag.all & 0x00FF);
        CAN_BUFFER_RT[BORAD_NUM][3] = ((Fault_Flag.all) >> 8) & 0x00FF;

        /**
         * �������¶�
         *
         * �¶�ֵת��ΪIQ15��ʵ��ֵ���ٽ���ת��Ϊ�����ȸ�������Ӧ��ʮ�����Ʊ�ʾ
         */
        if (Temperature5 <= _IQ(0)) {
            tmp = 0;
        } else {
            tmp = _IQmpy(Temperature5, 100);
        }

        if (tmp >= 65535) {
            tmp = 65535;
        }
        CAN_BUFFER_RT[BORAD_NUM][4] = tmp & 0x00FF;
        CAN_BUFFER_RT[BORAD_NUM][5] = (tmp >> 8) & 0x00FF;

        // ��ĸ�ߵ�ѹ����ֵת��ΪIQ15��ʵ��ֵ���ٽ���ת��Ϊ�����ȸ�������Ӧ��ʮ�����Ʊ�ʾ
        if (Udc_Mche_1 <= _IQ(0)) {
            tmp = 0;
        } else {
            tmp = _IQdiv(Udc_Mche_1, U_Base);
            tmp = _IQmpy(tmp, 100);
        }

        if (tmp >= 65535) {
            tmp = 65535;
        }
        CAN_BUFFER_RT[BORAD_NUM][6] = tmp & 0x00FF;
        CAN_BUFFER_RT[BORAD_NUM][7] = (tmp >> 8) & 0x00FF;

        CAN_BUFFER_RT[BORAD_NUM][8] = 0;
    }

    if (Num == 2) {
        // ��ĸ�ߵ�������ֵת��ΪIQ15��ʵ��ֵ���ٽ���ת��Ϊ�����ȸ�������Ӧ��ʮ�����Ʊ�ʾ
        tmp = _IQdiv(_IQabs(Idc), I_Base);
        tmp = _IQmpy(tmp, 100);
        if (tmp >= 65535) {
            tmp = 65535;
        }
        CAN_BUFFER_RT[BORAD_NUM][9] = tmp & 0x00FF;
        CAN_BUFFER_RT[BORAD_NUM][10] = (tmp >> 8) & 0x00FF;

        // ��A���������ֵת��ΪIQ15��ʵ��ֵ���ٽ���ת��Ϊ�����ȸ�������Ӧ��ʮ�����Ʊ�ʾ
        tmp = _IQdiv(_IQabs(Isa), I_Base);
        tmp = _IQmpy(tmp, 100);
        if (tmp >= 65535) {
            tmp = 65535;
        }
        CAN_BUFFER_RT[BORAD_NUM][11] = tmp & 0x00FF;
        CAN_BUFFER_RT[BORAD_NUM][12] = (tmp >> 8) & 0x00FF;

        // ��B���������ֵת��ΪIQ15��ʵ��ֵ���ٽ���ת��Ϊ�����ȸ�������Ӧ��ʮ�����Ʊ�ʾ
        tmp = _IQdiv(_IQabs(Isb), I_Base);
        tmp = _IQmpy(tmp, 100);
        if (tmp >= 65535) {
            tmp = 65535;
        }
        CAN_BUFFER_RT[BORAD_NUM][13] = tmp & 0x00FF;
        CAN_BUFFER_RT[BORAD_NUM][14] = (tmp >> 8) & 0x00FF;

        // ��C���������ֵת��ΪIQ15��ʵ��ֵ���ٽ���ת��Ϊ�����ȸ�������Ӧ��ʮ�����Ʊ�ʾ
        tmp = _IQdiv(_IQabs(Isc), I_Base);
        tmp = _IQmpy(tmp, 100);
        if (tmp >= 65535) {
            tmp = 65535;
        }
        CAN_BUFFER_RT[BORAD_NUM][15] = tmp & 0x00FF;
        CAN_BUFFER_RT[BORAD_NUM][16] = (tmp >> 8) & 0x00FF;
    }
}

void Can_Ctrl(Uint16 Board) {
    volatile _iq tmp = 0;

    if (Board == 0) {
        if ((CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A) || Ctrl_Flag.bit.STOP_PWM_Flag == 1 ||
            (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_BUFFER_RX[2] == 0x12) {
            Isd_Set = 0;
            tmp = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);
            tmp = _IQmpy(tmp,
                         _IQ(0.0055555555555555555555555555555556)); // 65536/360>>15
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

    if (Board == 1) {
        if ((CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x5A) || Ctrl_Flag.bit.STOP_PWM_Flag == 1 ||
            (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xA5) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_BUFFER_CTRL[2] == 0x12) {
            Isd_Set = 0;
            tmp = Hex_Float(CAN_BUFFER_CTRL[3], CAN_BUFFER_CTRL[4], CAN_BUFFER_CTRL[5], CAN_BUFFER_CTRL[6]);

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

    /* �ͱõ����CPUִ�п��ƹ��� */
    if (Board == 2) {
        Uint16 isStopPump = CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0xA5;
        Uint16 isStopPwm = Ctrl_Flag.bit.STOP_PWM_Flag == 1;
        Uint16 isFristPwm = Ctrl_Flag.bit.FIRST_PWM_Flag == 1;
        Uint16 isCurrentSpeedOverStartSpeed = Velo_Elec_abs > Velo_Start;
        if (isStopPump || isStopPwm || (isFristPwm && isCurrentSpeedOverStartSpeed)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55) {
            /* ������� */
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_BUFFER_RX[2] == 0x14) {
            /* ���õ��ת�� */
            Isd_Set = 0;
            TorqueAngleA = 0;
            _iq speed = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);
            Veloover_Set = _IQdiv(speed, _IQmpy((V_Base >> 5), _IQdiv(_IQ(9.5492965855137201461330258023509), Mp)));
            if (Veloover_Set > maxSpeed) {
                Veloover_Set = maxSpeed;
            }

            /* ���з��� */
            if (Ctrl_Flag.bit.VelWay_Flag == 1) {
                Veloover_Set = -Veloover_Set;
            }

            /* �������б�־ */
            if (RunAllow_Flag == 1) {
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
        }
    }

    if (Board == 3) {
        if ((CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0xA5) || Ctrl_Flag.bit.STOP_PWM_Flag == 1 ||
            (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0x55) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_BUFFER_CTRL[2] == 0x14) {
            Isd_Set = 0;
            TorqueAngleA = 0;
            tmp = Hex_Float(CAN_BUFFER_CTRL[3], CAN_BUFFER_CTRL[4], CAN_BUFFER_CTRL[5], CAN_BUFFER_CTRL[6]);
            Veloover_Set = _IQdiv(tmp, _IQmpy((V_Base >> 5), _IQdiv(_IQ(9.5492965855137201461330258023509), Mp)));
            if (Veloover_Set > maxSpeed) {
                Veloover_Set = maxSpeed;
            }

            if (Ctrl_Flag.bit.VelWay_Flag == 1) {
                Veloover_Set = -Veloover_Set;
            }

            if (RunAllow_Flag == 1) {
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
        }
    }

    if (Board == 4) {
        if ((CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55) || Ctrl_Flag.bit.STOP_PWM_Flag == 1 ||
            (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_BUFFER_RX[2] == 0x13) {
            Isd_Set = 0;
            tmp = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);

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

    if (Board == 5) {
        if ((CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x55) || Ctrl_Flag.bit.STOP_PWM_Flag == 1 ||
            (Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        } else if (CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xAA) {
            Ctrl_Flag.bit.CAN_Ctrl = 1;
        } else if (CAN_BUFFER_CTRL[2] == 0x13) {
            Isd_Set = 0;
            tmp = Hex_Float(CAN_BUFFER_CTRL[3], CAN_BUFFER_CTRL[4], CAN_BUFFER_CTRL[5], CAN_BUFFER_CTRL[6]);

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
}
/*--------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 can���ͳ���
 -------------------------------------------------------------------------------*/
void Can_Tx_Data() {
    DELAY_US(1000);
    if (ECanaRegs.CANTA.bit.TA1 == 1) {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
        ECanaShadow.CANTA.bit.TA1 = 1;
        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
    }
    ECanaRegs.CANMC.bit.CDR = 1;

    if (CANB.FLAG.bit.TXBO_FLAG == 0) {
        if (CANB.m_Tx_Length > 0) {
            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDL.byte.BYTE4 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDL.byte.BYTE3 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDL.byte.BYTE2 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDL.byte.BYTE1 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDH.byte.BYTE8 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDH.byte.BYTE7 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDH.byte.BYTE6 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            if (CANB.m_Tx_Length > 0) {
                ECanaMboxes.MBOX1.MDH.byte.BYTE5 = *CANB.p_Tx_Buffer++;
                CANB.m_Tx_Length--;
            }

            ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
            ECanaRegs.CANMC.bit.CDR = 0;
            ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
            ECanaShadow.CANTRS.all = 0;
            ECanaShadow.CANTRS.bit.TRS1 = 1;
            ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
        } else {
            CANB.FLAG.bit.TXBO_FLAG = 1;
            CANB.p_Tx_Buffer = TX_BUFFER;
        }
    }
}
void Can_Tx2_Data() {
    DELAY_US(1000);
    if (ECanaRegs.CANTA.bit.TA2 == 1) {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
        ECanaShadow.CANTA.bit.TA2 = 1;
        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
    }
    ECanaRegs.CANMC.bit.CDR = 1;

    if (CANB2.FLAG.bit.TXBO_FLAG == 0) {
        if (CANB2.m_Tx_Length > 0) {
            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDL.byte.BYTE4 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDL.byte.BYTE3 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDL.byte.BYTE2 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDL.byte.BYTE1 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDH.byte.BYTE8 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDH.byte.BYTE7 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDH.byte.BYTE6 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            if (CANB2.m_Tx_Length > 0) {
                ECanaMboxes.MBOX2.MDH.byte.BYTE5 = *CANB2.p_Tx_Buffer++;
                CANB2.m_Tx_Length--;
            }

            ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
            ECanaRegs.CANMC.bit.CDR = 0;
            ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
            ECanaShadow.CANTRS.all = 0;
            ECanaShadow.CANTRS.bit.TRS2 = 1;
            ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
        } else {
            CANB2.FLAG.bit.TXBO_FLAG = 1;
            CANB2.p_Tx_Buffer = SCI_RX_BUFFER;
        }
    }
}

void Can_Tx4_Data() {
    if (ECanaRegs.CANTA.bit.TA4 == 1) {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
        ECanaShadow.CANTA.bit.TA4 = 1;
        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
    }
    ECanaRegs.CANMC.bit.CDR = 1;

    Can_Rt_Data(1);
    ECanaMboxes.MBOX4.MDL.byte.BYTE4 = CAN_BUFFER_RT[BORAD_NUM][0];
    ECanaMboxes.MBOX4.MDL.byte.BYTE3 = CAN_BUFFER_RT[BORAD_NUM][1];
    ECanaMboxes.MBOX4.MDL.byte.BYTE2 = CAN_BUFFER_RT[BORAD_NUM][2];
    ECanaMboxes.MBOX4.MDL.byte.BYTE1 = CAN_BUFFER_RT[BORAD_NUM][3];
    ECanaMboxes.MBOX4.MDH.byte.BYTE8 = CAN_BUFFER_RT[BORAD_NUM][4];
    ECanaMboxes.MBOX4.MDH.byte.BYTE7 = CAN_BUFFER_RT[BORAD_NUM][5];
    ECanaMboxes.MBOX4.MDH.byte.BYTE6 = CAN_BUFFER_RT[BORAD_NUM][6];
    ECanaMboxes.MBOX4.MDH.byte.BYTE5 = CAN_BUFFER_RT[BORAD_NUM][7];

    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
    ECanaRegs.CANMC.bit.CDR = 0;
    ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    ECanaShadow.CANTRS.all = 0;
    ECanaShadow.CANTRS.bit.TRS4 = 1;
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
}

void Can_Tx13_Data() {
    if (ECanaRegs.CANTA.bit.TA13 == 1) {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
        ECanaShadow.CANTA.bit.TA13 = 1;
        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
    }
    ECanaRegs.CANMC.bit.CDR = 1;

    Can_Rt_Data(2);
    ECanaMboxes.MBOX13.MDL.byte.BYTE4 = CAN_BUFFER_RT[BORAD_NUM][9];
    ECanaMboxes.MBOX13.MDL.byte.BYTE3 = CAN_BUFFER_RT[BORAD_NUM][10];
    ECanaMboxes.MBOX13.MDL.byte.BYTE2 = CAN_BUFFER_RT[BORAD_NUM][11];
    ECanaMboxes.MBOX13.MDL.byte.BYTE1 = CAN_BUFFER_RT[BORAD_NUM][12];
    ECanaMboxes.MBOX13.MDH.byte.BYTE8 = CAN_BUFFER_RT[BORAD_NUM][13];
    ECanaMboxes.MBOX13.MDH.byte.BYTE7 = CAN_BUFFER_RT[BORAD_NUM][14];
    ECanaMboxes.MBOX13.MDH.byte.BYTE6 = CAN_BUFFER_RT[BORAD_NUM][15];
    ECanaMboxes.MBOX13.MDH.byte.BYTE5 = CAN_BUFFER_RT[BORAD_NUM][16];

    ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
    ECanaRegs.CANMC.bit.CDR = 0;
    ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    ECanaShadow.CANTRS.all = 0;
    ECanaShadow.CANTRS.bit.TRS13 = 1;
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
}

void Can_Tx11_Data() {
    if (ECanaRegs.CANTA.bit.TA11 == 1) {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
        ECanaShadow.CANTA.bit.TA11 = 1;
        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
    }
    ECanaRegs.CANMC.bit.CDR = 1;

    Can_Rt_Data(1);
    Can_Rt_Data(2);

    ECanaMboxes.MBOX11.MDL.byte.BYTE4 = CAN_BUFFER_RX[0];
    ECanaMboxes.MBOX11.MDL.byte.BYTE3 = CAN_BUFFER_RX[1];
    ECanaMboxes.MBOX11.MDL.byte.BYTE2 = CAN_BUFFER_RX[2];
    ECanaMboxes.MBOX11.MDL.byte.BYTE1 = CAN_BUFFER_RX[3];
    ECanaMboxes.MBOX11.MDH.byte.BYTE8 = CAN_BUFFER_RX[4];
    ECanaMboxes.MBOX11.MDH.byte.BYTE7 = CAN_BUFFER_RX[5];
    ECanaMboxes.MBOX11.MDH.byte.BYTE6 = CAN_BUFFER_RX[6];
    ECanaMboxes.MBOX11.MDH.byte.BYTE5 = CAN_BUFFER_RX[7];

    ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
    ECanaRegs.CANMC.bit.CDR = 0;
    ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    ECanaShadow.CANTRS.all = 0;
    ECanaShadow.CANTRS.bit.TRS11 = 1;
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
}

interrupt void ECana_T(void) {
    volatile Uint16 i;
    volatile Uint16 Sum_Temp = 0;
    if (ECanaRegs.CANRMP.bit.RMP0 == 1) /*�ȴ�����RMP0��λ*/
    {
        if (CANB.FLAG.bit.RX_FLAG == 0) // && CANB.FLAG.bit.TXBO_FLAG == 1)
        {
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE4;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE3;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE2;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDL.byte.BYTE1;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE8;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE7;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE6;
            CANB.p_Rx_Buffer++;
            *CANB.p_Rx_Buffer = ECanaMboxes.MBOX0.MDH.byte.BYTE5;
            CANB.p_Rx_Buffer++;

            if (CANB.p_Rx_Buffer >= (Uint16 *)(RX_BUFFER + RX_BUFFER_LENGTH)) {
                CANB.p_Rx_Buffer = RX_BUFFER;
                CANB.m_Rx_Length = 0;
            }
            // ����ͷ�Ƿ�ΪAA55,�Լ���ȡ���ݳ���----------------------
            else if (RX_BUFFER[0] == 0x00EB && RX_BUFFER[1] == 0x0090 &&
                     RX_BUFFER[4] <= (Uint16)(RX_BUFFER_LENGTH - 7)) {
                CANB.m_Rx_Length = RX_BUFFER[4];
                if (CANB.p_Rx_Buffer >= (Uint16 *)(RX_BUFFER + CANB.m_Rx_Length + 7)) {
                    Sum_Temp = CalCRC16_Byte(RX_BUFFER, CANB.m_Rx_Length + 0x0005);

                    if (Sum_Temp == RX_BUFFER[CANB.m_Rx_Length + 5] + (RX_BUFFER[CANB.m_Rx_Length + 6] << 8)) {
                        CANB.FLAG.bit.RX_FLAG = 1;
                        CANB.p_Rx_Buffer = RX_BUFFER;
                        CANB.m_Rx_Length = 0;
                    } else {
                        CANB.p_Rx_Buffer = RX_BUFFER;
                        CANB.m_Rx_Length = 0;
                    }
                }
            } else {
                CANB.p_Rx_Buffer = RX_BUFFER;
                CANB.m_Rx_Length = 0;
            }
        }

        ECanaRegs.CANRMP.bit.RMP0 = 1; /*�������RMP0λ����ʼ������Ϣ*/
    }
    if (ECanaRegs.CANRMP.bit.RMP3 == 1) /*�ȴ�����RMP0��λ*/
    {
        if (CANB2.FLAG.bit.RX_FLAG == 0) // && CANB2.FLAG.bit.TXBO_FLAG == 1)
        {
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE4;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE3;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE2;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDL.byte.BYTE1;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE8;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE7;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE6;
            CANB2.p_Rx_Buffer++;
            *CANB2.p_Rx_Buffer = ECanaMboxes.MBOX3.MDH.byte.BYTE5;
            CANB2.p_Rx_Buffer++;

            if (CANB2.p_Rx_Buffer >= (Uint16 *)(SCI_TX_BUFFER + RX_BUFFER_LENGTH)) {
                CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
                CANB2.m_Rx_Length = 0;
            }
            // ����ͷ�Ƿ�ΪAA55,�Լ���ȡ���ݳ���----------------------
            else if (SCI_TX_BUFFER[0] == 0x00EB && SCI_TX_BUFFER[1] == 0x0090 &&
                     SCI_TX_BUFFER[4] <= (Uint16)(RX_BUFFER_LENGTH - 7)) {
                CANB2.m_Rx_Length = SCI_TX_BUFFER[4];
                if (CANB2.p_Rx_Buffer >= (Uint16 *)(SCI_TX_BUFFER + CANB2.m_Rx_Length + 7)) {
                    Sum_Temp = CalCRC16_Byte(SCI_TX_BUFFER, CANB2.m_Rx_Length + 0x0005);

                    if (Sum_Temp ==
                        SCI_TX_BUFFER[CANB2.m_Rx_Length + 5] + (SCI_TX_BUFFER[CANB2.m_Rx_Length + 6] << 8)) {
                        CANB2.FLAG.bit.RX_FLAG = 1;
                        CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
                        CANB2.m_Rx_Length = 0;
                    } else {
                        CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
                        CANB2.m_Rx_Length = 0;
                    }
                }
            } else {
                CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
                CANB2.m_Rx_Length = 0;
            }
        }

        ECanaRegs.CANRMP.bit.RMP3 = 1; /*�������RMP0λ����ʼ������Ϣ*/
    }
    if (ECanaRegs.CANRMP.bit.RMP5 == 1) /*�ȴ�����RMP5��λ*/
    {
        if (BORAD_NUM != 0) {
            CAN_BUFFER_RT[0][0] = ECanaMboxes.MBOX5.MDL.byte.BYTE4;
            CAN_BUFFER_RT[0][1] = ECanaMboxes.MBOX5.MDL.byte.BYTE3;
            CAN_BUFFER_RT[0][2] = ECanaMboxes.MBOX5.MDL.byte.BYTE2;
            CAN_BUFFER_RT[0][3] = ECanaMboxes.MBOX5.MDL.byte.BYTE1;
            CAN_BUFFER_RT[0][4] = ECanaMboxes.MBOX5.MDH.byte.BYTE8;
            CAN_BUFFER_RT[0][5] = ECanaMboxes.MBOX5.MDH.byte.BYTE7;
            CAN_BUFFER_RT[0][6] = ECanaMboxes.MBOX5.MDH.byte.BYTE6;
            CAN_BUFFER_RT[0][7] = ECanaMboxes.MBOX5.MDH.byte.BYTE5;
            CAN_BUFFER_RT[0][8] = 0;
        }
        ECanaRegs.CANRMP.bit.RMP5 = 1; /*�������RMP5λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP6 == 1) /*�ȴ�����RMP6��λ*/
    {
        if (BORAD_NUM != 1) {
            CAN_BUFFER_RT[1][0] = ECanaMboxes.MBOX6.MDL.byte.BYTE4;
            CAN_BUFFER_RT[1][1] = ECanaMboxes.MBOX6.MDL.byte.BYTE3;
            CAN_BUFFER_RT[1][2] = ECanaMboxes.MBOX6.MDL.byte.BYTE2;
            CAN_BUFFER_RT[1][3] = ECanaMboxes.MBOX6.MDL.byte.BYTE1;
            CAN_BUFFER_RT[1][4] = ECanaMboxes.MBOX6.MDH.byte.BYTE8;
            CAN_BUFFER_RT[1][5] = ECanaMboxes.MBOX6.MDH.byte.BYTE7;
            CAN_BUFFER_RT[1][6] = ECanaMboxes.MBOX6.MDH.byte.BYTE6;
            CAN_BUFFER_RT[1][7] = ECanaMboxes.MBOX6.MDH.byte.BYTE5;
            CAN_BUFFER_RT[1][8] = 0;
        }
        ECanaRegs.CANRMP.bit.RMP6 = 1; /*�������RMP6λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP7 == 1) /*�ȴ�����RMP7��λ*/
    {
        if (BORAD_NUM != 2) {
            CAN_BUFFER_RT[2][0] = ECanaMboxes.MBOX7.MDL.byte.BYTE4;
            CAN_BUFFER_RT[2][1] = ECanaMboxes.MBOX7.MDL.byte.BYTE3;
            CAN_BUFFER_RT[2][2] = ECanaMboxes.MBOX7.MDL.byte.BYTE2;
            CAN_BUFFER_RT[2][3] = ECanaMboxes.MBOX7.MDL.byte.BYTE1;
            CAN_BUFFER_RT[2][4] = ECanaMboxes.MBOX7.MDH.byte.BYTE8;
            CAN_BUFFER_RT[2][5] = ECanaMboxes.MBOX7.MDH.byte.BYTE7;
            CAN_BUFFER_RT[2][6] = ECanaMboxes.MBOX7.MDH.byte.BYTE6;
            CAN_BUFFER_RT[2][7] = ECanaMboxes.MBOX7.MDH.byte.BYTE5;
            CAN_BUFFER_RT[2][8] = 0;
        }
        ECanaRegs.CANRMP.bit.RMP7 = 1; /*�������RMP7λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP8 == 1) /*�ȴ�����RMP8��λ*/
    {
        if (BORAD_NUM != 3) {
            CAN_BUFFER_RT[3][0] = ECanaMboxes.MBOX8.MDL.byte.BYTE4;
            CAN_BUFFER_RT[3][1] = ECanaMboxes.MBOX8.MDL.byte.BYTE3;
            CAN_BUFFER_RT[3][2] = ECanaMboxes.MBOX8.MDL.byte.BYTE2;
            CAN_BUFFER_RT[3][3] = ECanaMboxes.MBOX8.MDL.byte.BYTE1;
            CAN_BUFFER_RT[3][4] = ECanaMboxes.MBOX8.MDH.byte.BYTE8;
            CAN_BUFFER_RT[3][5] = ECanaMboxes.MBOX8.MDH.byte.BYTE7;
            CAN_BUFFER_RT[3][6] = ECanaMboxes.MBOX8.MDH.byte.BYTE6;
            CAN_BUFFER_RT[3][7] = ECanaMboxes.MBOX8.MDH.byte.BYTE5;
            CAN_BUFFER_RT[3][8] = 0;
        }
        ECanaRegs.CANRMP.bit.RMP8 = 1; /*�������RMP8λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP9 == 1) /*�ȴ�����RMP9��λ*/
    {
        if (BORAD_NUM != 4) {
            CAN_BUFFER_RT[4][0] = ECanaMboxes.MBOX9.MDL.byte.BYTE4;
            CAN_BUFFER_RT[4][1] = ECanaMboxes.MBOX9.MDL.byte.BYTE3;
            CAN_BUFFER_RT[4][2] = ECanaMboxes.MBOX9.MDL.byte.BYTE2;
            CAN_BUFFER_RT[4][3] = ECanaMboxes.MBOX9.MDL.byte.BYTE1;
            CAN_BUFFER_RT[4][4] = ECanaMboxes.MBOX9.MDH.byte.BYTE8;
            CAN_BUFFER_RT[4][5] = ECanaMboxes.MBOX9.MDH.byte.BYTE7;
            CAN_BUFFER_RT[4][6] = ECanaMboxes.MBOX9.MDH.byte.BYTE6;
            CAN_BUFFER_RT[4][7] = ECanaMboxes.MBOX9.MDH.byte.BYTE5;
            CAN_BUFFER_RT[4][8] = 0;
        }
        ECanaRegs.CANRMP.bit.RMP9 = 1; /*�������RMP9λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP10 == 1) /*�ȴ�����RMP10��λ*/
    {
        if (BORAD_NUM != 5) {
            CAN_BUFFER_RT[5][0] = ECanaMboxes.MBOX10.MDL.byte.BYTE4;
            CAN_BUFFER_RT[5][1] = ECanaMboxes.MBOX10.MDL.byte.BYTE3;
            CAN_BUFFER_RT[5][2] = ECanaMboxes.MBOX10.MDL.byte.BYTE2;
            CAN_BUFFER_RT[5][3] = ECanaMboxes.MBOX10.MDL.byte.BYTE1;
            CAN_BUFFER_RT[5][4] = ECanaMboxes.MBOX10.MDH.byte.BYTE8;
            CAN_BUFFER_RT[5][5] = ECanaMboxes.MBOX10.MDH.byte.BYTE7;
            CAN_BUFFER_RT[5][6] = ECanaMboxes.MBOX10.MDH.byte.BYTE6;
            CAN_BUFFER_RT[5][7] = ECanaMboxes.MBOX10.MDH.byte.BYTE5;
            CAN_BUFFER_RT[5][8] = 0;
        }
        ECanaRegs.CANRMP.bit.RMP10 = 1; /*�������RMP10λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP12 == 1) /*�ȴ�����RMP10��λ*/
    {
        /*if(Sys_Flag.bit.CAN_FLAG == 0)
         {
         CAN_BUFFER_CTRL[0] = ECanaMboxes.MBOX12.MDL.byte.BYTE4;
         CAN_BUFFER_CTRL[1] = ECanaMboxes.MBOX12.MDL.byte.BYTE3;
         CAN_BUFFER_CTRL[2] = ECanaMboxes.MBOX12.MDL.byte.BYTE2;
         CAN_BUFFER_CTRL[3] = ECanaMboxes.MBOX12.MDL.byte.BYTE1;
         CAN_BUFFER_CTRL[4] = ECanaMboxes.MBOX12.MDH.byte.BYTE8;
         CAN_BUFFER_CTRL[5] = ECanaMboxes.MBOX12.MDH.byte.BYTE7;
         CAN_BUFFER_CTRL[6] = ECanaMboxes.MBOX12.MDH.byte.BYTE6;
         CAN_BUFFER_CTRL[7] = ECanaMboxes.MBOX12.MDH.byte.BYTE5;
         Sys_Flag.bit.CAN_FLAG = 1;
         }*/
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][0] = ECanaMboxes.MBOX12.MDL.byte.BYTE4;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][1] = ECanaMboxes.MBOX12.MDL.byte.BYTE3;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][2] = ECanaMboxes.MBOX12.MDL.byte.BYTE2;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][3] = ECanaMboxes.MBOX12.MDL.byte.BYTE1;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][4] = ECanaMboxes.MBOX12.MDH.byte.BYTE8;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][5] = ECanaMboxes.MBOX12.MDH.byte.BYTE7;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][6] = ECanaMboxes.MBOX12.MDH.byte.BYTE6;
        CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_N_index][7] = ECanaMboxes.MBOX12.MDH.byte.BYTE5;
        CAN_BUFFER_CTRL_N_index++;
        if (CAN_BUFFER_CTRL_N_index > 5)
            CAN_BUFFER_CTRL_N_index = 0;

        CAN_BUFFER_CTRL_Num++;

        ECanaRegs.CANRMP.bit.RMP12 = 1; /*�������RMP10λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP14 == 1) /*�ȴ�����RMP14��λ*/
    {
        if (BORAD_NUM != 0) {
            CAN_BUFFER_RT[0][9] = ECanaMboxes.MBOX14.MDL.byte.BYTE4;
            CAN_BUFFER_RT[0][10] = ECanaMboxes.MBOX14.MDL.byte.BYTE3;
            CAN_BUFFER_RT[0][11] = ECanaMboxes.MBOX14.MDL.byte.BYTE2;
            CAN_BUFFER_RT[0][12] = ECanaMboxes.MBOX14.MDL.byte.BYTE1;
            CAN_BUFFER_RT[0][13] = ECanaMboxes.MBOX14.MDH.byte.BYTE8;
            CAN_BUFFER_RT[0][14] = ECanaMboxes.MBOX14.MDH.byte.BYTE7;
            CAN_BUFFER_RT[0][15] = ECanaMboxes.MBOX14.MDH.byte.BYTE6;
            CAN_BUFFER_RT[0][16] = ECanaMboxes.MBOX14.MDH.byte.BYTE5;
        }
        ECanaRegs.CANRMP.bit.RMP14 = 1; /*�������RMP14λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP15 == 1) /*�ȴ�����RMP15��λ*/
    {
        if (BORAD_NUM != 1) {
            CAN_BUFFER_RT[1][9] = ECanaMboxes.MBOX15.MDL.byte.BYTE4;
            CAN_BUFFER_RT[1][10] = ECanaMboxes.MBOX15.MDL.byte.BYTE3;
            CAN_BUFFER_RT[1][11] = ECanaMboxes.MBOX15.MDL.byte.BYTE2;
            CAN_BUFFER_RT[1][12] = ECanaMboxes.MBOX15.MDL.byte.BYTE1;
            CAN_BUFFER_RT[1][13] = ECanaMboxes.MBOX15.MDH.byte.BYTE8;
            CAN_BUFFER_RT[1][14] = ECanaMboxes.MBOX15.MDH.byte.BYTE7;
            CAN_BUFFER_RT[1][15] = ECanaMboxes.MBOX15.MDH.byte.BYTE6;
            CAN_BUFFER_RT[1][16] = ECanaMboxes.MBOX15.MDH.byte.BYTE5;
        }
        ECanaRegs.CANRMP.bit.RMP15 = 1; /*�������RMP15λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP16 == 1) /*�ȴ�����RMP16��λ*/
    {
        if (BORAD_NUM != 2) {
            CAN_BUFFER_RT[2][9] = ECanaMboxes.MBOX16.MDL.byte.BYTE4;
            CAN_BUFFER_RT[2][10] = ECanaMboxes.MBOX16.MDL.byte.BYTE3;
            CAN_BUFFER_RT[2][11] = ECanaMboxes.MBOX16.MDL.byte.BYTE2;
            CAN_BUFFER_RT[2][12] = ECanaMboxes.MBOX16.MDL.byte.BYTE1;
            CAN_BUFFER_RT[2][13] = ECanaMboxes.MBOX16.MDH.byte.BYTE8;
            CAN_BUFFER_RT[2][14] = ECanaMboxes.MBOX16.MDH.byte.BYTE7;
            CAN_BUFFER_RT[2][15] = ECanaMboxes.MBOX16.MDH.byte.BYTE6;
            CAN_BUFFER_RT[2][16] = ECanaMboxes.MBOX16.MDH.byte.BYTE5;
        }
        ECanaRegs.CANRMP.bit.RMP16 = 1; /*�������RMP16λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP17 == 1) /*�ȴ�����RMP17��λ*/
    {
        if (BORAD_NUM != 3) {
            CAN_BUFFER_RT[3][9] = ECanaMboxes.MBOX17.MDL.byte.BYTE4;
            CAN_BUFFER_RT[3][10] = ECanaMboxes.MBOX17.MDL.byte.BYTE3;
            CAN_BUFFER_RT[3][11] = ECanaMboxes.MBOX17.MDL.byte.BYTE2;
            CAN_BUFFER_RT[3][12] = ECanaMboxes.MBOX17.MDL.byte.BYTE1;
            CAN_BUFFER_RT[3][13] = ECanaMboxes.MBOX17.MDH.byte.BYTE8;
            CAN_BUFFER_RT[3][14] = ECanaMboxes.MBOX17.MDH.byte.BYTE7;
            CAN_BUFFER_RT[3][15] = ECanaMboxes.MBOX17.MDH.byte.BYTE6;
            CAN_BUFFER_RT[3][16] = ECanaMboxes.MBOX17.MDH.byte.BYTE5;
        }
        ECanaRegs.CANRMP.bit.RMP17 = 1; /*�������RMP17λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP18 == 1) /*�ȴ�����RMP18��λ*/
    {
        if (BORAD_NUM != 4) {
            CAN_BUFFER_RT[4][9] = ECanaMboxes.MBOX18.MDL.byte.BYTE4;
            CAN_BUFFER_RT[4][10] = ECanaMboxes.MBOX18.MDL.byte.BYTE3;
            CAN_BUFFER_RT[4][11] = ECanaMboxes.MBOX18.MDL.byte.BYTE2;
            CAN_BUFFER_RT[4][12] = ECanaMboxes.MBOX18.MDL.byte.BYTE1;
            CAN_BUFFER_RT[4][13] = ECanaMboxes.MBOX18.MDH.byte.BYTE8;
            CAN_BUFFER_RT[4][14] = ECanaMboxes.MBOX18.MDH.byte.BYTE7;
            CAN_BUFFER_RT[4][15] = ECanaMboxes.MBOX18.MDH.byte.BYTE6;
            CAN_BUFFER_RT[4][16] = ECanaMboxes.MBOX18.MDH.byte.BYTE5;
        }
        ECanaRegs.CANRMP.bit.RMP18 = 1; /*�������RMP18λ����ʼ������Ϣ*/
    }

    if (ECanaRegs.CANRMP.bit.RMP19 == 1) /*�ȴ�����RMP19��λ*/
    {
        if (BORAD_NUM != 5) {
            CAN_BUFFER_RT[5][9] = ECanaMboxes.MBOX19.MDL.byte.BYTE4;
            CAN_BUFFER_RT[5][10] = ECanaMboxes.MBOX19.MDL.byte.BYTE3;
            CAN_BUFFER_RT[5][11] = ECanaMboxes.MBOX19.MDL.byte.BYTE2;
            CAN_BUFFER_RT[5][12] = ECanaMboxes.MBOX19.MDL.byte.BYTE1;
            CAN_BUFFER_RT[5][13] = ECanaMboxes.MBOX19.MDH.byte.BYTE8;
            CAN_BUFFER_RT[5][14] = ECanaMboxes.MBOX19.MDH.byte.BYTE7;
            CAN_BUFFER_RT[5][15] = ECanaMboxes.MBOX19.MDH.byte.BYTE6;
            CAN_BUFFER_RT[5][16] = ECanaMboxes.MBOX19.MDH.byte.BYTE5;
        }
        ECanaRegs.CANRMP.bit.RMP19 = 1; /*�������RMP19λ����ʼ������Ϣ*/
    }

    PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}

//----------------------------------------------------------------------------------------
// �������ݴ������
// ����Ҫ���͵����ݰ���ͨѶ��Լ���������?
// ��ڲ���:��Ҫ���͵����ݵ�ַ�����͵������֡����͵����ݳ���(��)
// ���ڲ���:��
//----------------------------------------------------------------------------------------
void CAN_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L) {
    volatile Uint16 i;
    volatile Uint16 Data_Temp;

    *CANB.p_Tx_Buffer++ = 0x00EB;
    *CANB.p_Tx_Buffer++ = 0x0090;
    *CANB.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

    // ���͵������ֽ�������Ч���ֽ�
    *CANB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) >> 8;
    *CANB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

    for (i = 0; i < m_tx_length_L; i++) {
        Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
        *CANB.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
        *CANB.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
        *CANB.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = (*p_tx_data_L++) & 0x00FF;
        *CANB.p_Tx_Buffer++ = Data_Temp;
    }

    Data_Temp = CalCRC16_Byte(TX_BUFFER, (m_tx_length_L << 2) + 5);
    *CANB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); /* �ۼ���Ч���*/
    *CANB.p_Tx_Buffer = (Data_Temp >> 8);       /* �ۼ��Ч��?*/
    CANB.p_Tx_Buffer = (Uint16 *)TX_BUFFER;
    CANB.m_Tx_Length = (m_tx_length_L << 2) + 2 + 5;
}

void CAN2_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L) {
    volatile Uint16 i;
    volatile Uint16 Data_Temp;

    *CANB2.p_Tx_Buffer++ = 0x00EB;
    *CANB2.p_Tx_Buffer++ = 0x0090;
    *CANB2.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

    // ���͵������ֽ�������Ч���ֽ�
    *CANB2.p_Tx_Buffer++ = ((m_tx_length_L << 2)) >> 8;
    *CANB2.p_Tx_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

    for (i = 0; i < m_tx_length_L; i++) {
        Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
        *CANB2.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
        *CANB2.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
        *CANB2.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = (*p_tx_data_L++) & 0x00FF;
        *CANB2.p_Tx_Buffer++ = Data_Temp;
    }

    Data_Temp = CalCRC16_Byte(SCI_RX_BUFFER, (m_tx_length_L << 2) + 5);
    *CANB2.p_Tx_Buffer++ = (Data_Temp & 0x00FF); /* �ۼ���Ч���*/
    *CANB2.p_Tx_Buffer = (Data_Temp >> 8);       /* �ۼ���Ч���*/
    CANB2.p_Tx_Buffer = (Uint16 *)SCI_RX_BUFFER;
    CANB2.m_Tx_Length = (m_tx_length_L << 2) + 2 + 5;
}

void CAN_DataCombine_Rx(Uint16 *p_Rx) {
    volatile Uint16 i = 0, j = 0;
    for (i = 1; i < p_Rx[4]; i += 4) {
        j = (i - 1) >> 2;
        ReceData[j] = p_Rx[i + 4];
        ReceData[j] = ReceData[j] << 8;
        ReceData[j] += p_Rx[i + 5];
        ReceData[j] = ReceData[j] << 8;
        ReceData[j] += p_Rx[i + 6];
        ReceData[j] = ReceData[j] << 8;
        ReceData[j] += p_Rx[i + 7];
    }

    CANB.p_Rx_Buffer = RX_BUFFER;
}

void CAN2_DataCombine_Rx(Uint16 *p_Rx) {
    volatile Uint16 i = 0, j = 0;
    for (i = 1; i < p_Rx[4]; i += 4) {
        j = (i - 1) >> 2;
        ReceData[j] = p_Rx[i + 4];
        ReceData[j] = ReceData[j] << 8;
        ReceData[j] += p_Rx[i + 5];
        ReceData[j] = ReceData[j] << 8;
        ReceData[j] += p_Rx[i + 6];
        ReceData[j] = ReceData[j] << 8;
        ReceData[j] += p_Rx[i + 7];
    }

    CANB2.p_Rx_Buffer = SCI_TX_BUFFER;
}

void Can_Deal(void) {
    volatile Uint16 command, i = 0;
    command = (RX_BUFFER[2]);

    switch (command) {
    case 0x0000: // ��ʼ����������ָ��			lxy
        SendData[0] = ReceData[1];
        SendData[1] = BORAD_NUM;
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0070, 2);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0001:                             // ������ϱ�־ָ��			lxy
        Sys_Flag.bit.STOP_PWM_Flag_Id = 0;   // "1"���������������
        Sys_Flag.bit.STOP_PWM_Flag_Velo = 0; // "1"�������ٱ���
        Sys_Flag.bit.UDC_FLAG = 0;           // "1"����ֱ����ѹ����
        Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0071, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000D: // LOCK
        CAN_DataCombine_Rx(RX_BUFFER);
        Usa_Lock = ReceData[0];
        Usb_Lock = ReceData[1];
        Usc_Lock = ReceData[2];
        Udc_Lock = ReceData[3];
        Choose_Mche = ReceData[4];
        Ctrl_Flag.bit.LOCK_FLAG = 1;
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x007D, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0002: // Read Position
        SendData[0] = Isa;
        SendData[1] = Isb;
        SendData[2] = Isc;
        SendData[3] = rdc1.DATA_BUFFER;
        SendData[4] = Idc;
        // SendData[5] = IsbB;
        // SendData[6] = IscB;
        SendData[7] = rdc2.DATA_BUFFER;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0072, 8);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0003: // Openloop

        CAN_DataCombine_Rx(RX_BUFFER);
        Um_OpenLoop = ReceData[0];
        Freq_OpenLoop = ReceData[1];
        Udc_OpenLoop = ReceData[2];
        Choose_Mche = ReceData[3];
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 1;
        Ctrl_Flag.bit.LOCK_FLAG = 0;
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0073, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000E: // Release LOCK

        CAN_DataCombine_Rx(RX_BUFFER);
        Choose_Mche = ReceData[0];
        Ctrl_Flag.bit.LOCK_FLAG = 0;
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x007E, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0004: // д��������ͻ�ֵϵͳָ��			lxy
        CAN_DataCombine_Rx(RX_BUFFER);
        getMotorParams(ReceData);

        Velo_PerAdd = _IQmpy(Velo_Add, Per_Ctrl);
        Velo_Duty = _IQdiv(3750000, Control_Period);

        p = _IQdiv(Mp, Rp); /* ���������ļ������� */
        Udc_Mche_realvalue = _IQdiv(_IQ(1), Udc_Setg);
        K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
        K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0); // ��ʱ������Ϊ100us
        K_Velo_Cal = _IQdiv(K_Velo_Cal, Velo_Calc_Num << 20);
        K_Velo_Cal = _IQmpy(K_Velo_Cal, p);
        Is_PerAdd = _IQmpy(Is_Add, Per_Ctrl);
        Is_DownPerAdd = _IQmpy(Is_DownAdd, Per_Ctrl);
        pi_Id_Kc.Kp_reg = _IQmpy(Idq_Kc, Ld);
        pi_Id_Kc.Ki_reg = _IQmpy(V_Base, Per_Ctrl);
        pi_Id_Kc.Ki_reg = _IQmpy(pi_Id_Kc.Ki_reg, (pi_Id_Kc.i_period << 20));
        pi_Id_Kc.Ki_reg = _IQmpy(Idq_Kc, pi_Id_Kc.Ki_reg);
        pi_Iq_Kc.Kp_reg = _IQmpy(Idq_Kc, Lq);
        pi_Iq_Kc.Ki_reg = pi_Id_Kc.Ki_reg;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0074, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }

        break;
    case 0x0005: // ����������ͻ�ֵϵͳָ��		lxy
        SendData[0] = Rs;
        SendData[1] = Ld;
        SendData[2] = Lq;
        SendData[3] = PHI;
        SendData[4] = Mp;
        SendData[5] = Rp;
        SendData[6] = Udc_Setg;
        SendData[7] = U_Base;
        SendData[8] = I_Base;
        SendData[9] = V_Base; // ��е�ٶȻ�ֵ(rad/s)
        SendData[10] = Angle_Init_Digital;
        SendData[11] = Velo_Max;
        SendData[12] = id_Max;
        SendData[13] = Is_Add;
        SendData[14] = Control_Period;
        SendData[15] = id_Filter;
        SendData[16] = Udc_Filter;
        SendData[17] = Velo_Filter;
        SendData[18] = Deadtime;
        SendData[19] = Velo_Add;
        SendData[20] = Is_DownAdd;
        SendData[21] = Temp_Filter;
        SendData[22] = Angle_Init_Fa;
        SendData[23] = Com_angle_U;
        SendData[24] = Com_angle_I;
        SendData[25] = idSet_Max;
        // SendData[26] = Freq_cut;
        SendData[27] = IsLimit_Set;
        SendData[28] = MotorTM;
        SendData[29] = igbtTemperatureProtectionValue;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0075, 30);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0006: // λ�ÿ���ָ��			lxy

        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1) {
            SendData[0] = 1;
        } else if ((Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            SendData[0] = 2;
        } else {
            CAN_DataCombine_Rx(RX_BUFFER);
            Isd_Set = ReceData[0];
            Pos_Set = ReceData[1];
            if (Pos_Set > POSMAX)
                Pos_Set = POSMAX;
            if (Pos_Set < POSMIN)
                Pos_Set = POSMIN;
            TorqueAngleA = ReceData[2];
            Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
            Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
            Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            SendData[0] = 0;
        }

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0076, 1);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }

        break;
    case 0x0007: // �ٶȿ���ָ��			lxy

        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1) {
            SendData[0] = 1;
        } else if ((Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            SendData[0] = 2;
        } else {
            CAN_DataCombine_Rx(RX_BUFFER);
            Isd_Set = ReceData[0];
            Veloover_Set = ReceData[1];
            if (Veloover_Set > maxSpeed) {
                Veloover_Set = maxSpeed;
            }
            if (Veloover_Set < -maxSpeed) {
                Veloover_Set = -maxSpeed;
            }
            TorqueAngleA = ReceData[2];
            Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
            Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
            Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            SendData[0] = 0;
        }

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0077, 1);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }

        break;
    case 0x0008: // ת�ؿ���ָ��			lxy
        CAN_DataCombine_Rx(RX_BUFFER);
        Choose_Mche = ReceData[0];
        Choose_Mode = ReceData[1];
        SendData[0] = 0;
        SendData[1] = 0;
        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1) {
            SendData[0] = 1;
        } else if ((Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            SendData[0] = 2;
        } else {
            if (Choose_Mode == 0) {
                Isd_Set = ReceData[2];
                Isdq_Set1 = -ReceData[3];
                TorqueAngleA = ReceData[4];
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1; // ��ת�ؿ��Ʊ�־
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
            } else if (Choose_Mode == 1) {
                Isd_Set = ReceData[2];
                Veloover_Set = ReceData[3];
                if (Veloover_Set > maxSpeed)
                    Veloover_Set = maxSpeed;
                if (Veloover_Set < -maxSpeed)
                    Veloover_Set = -maxSpeed;
                TorqueAngleA = ReceData[4];
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            } else {
                Isd_Set = ReceData[2];
                Pos_Set = ReceData[3];
                if (Pos_Set > POSMAX)
                    Pos_Set = POSMAX;
                if (Pos_Set < POSMIN)
                    Pos_Set = POSMIN;
                TorqueAngleA = ReceData[4];
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
            SendData[0] = 0;
        }
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0078, 2);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0009: // ������������ָ��			lxy
        CAN_DataCombine_Rx(RX_BUFFER);
        Choose_Mche = ReceData[0];
        Sys_Flag.bit.RDC_FAULT = rdc1.FAULT;
        SendData[0] = Velo_Elec;
        SendData[1] = Udc_Mche_1;
        SendData[2] = Id;
        SendData[3] = Iq;
        SendData[4] = Sys_Flag.all;
        SendData[5] = IdZ_Set;
        SendData[6] = IqZ_Set;
        SendData[7] = Us;
        SendData[8] = pi_Id_Kc.ui_reg;
        SendData[9] = pi_Iq_Kc.ui_reg;
        SendData[10] = Ud;
        SendData[11] = Uq;
        SendData[12] = Time_Cal;
        SendData[13] = Temperature1;
        SendData[14] = Veloover_Set;
        SendData[15] = Temperature3;
        SendData[17] = Ctrl_Flag.all;
        SendData[18] = Temperature5;
        SendData[19] = Idc;
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0079, 22);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000A: // ���ͣתָ��			lxy
        CAN_DataCombine_Rx(RX_BUFFER);
        Choose_Mche = ReceData[0];
        Ctrl_Flag.bit.STOP_VELO_FLAG = 1;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x007A, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000B: // д���Ʋ���ָ��
        CAN_DataCombine_Rx(RX_BUFFER);
        getControlParams(ReceData);

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x007B, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000C: // ������ָ��
        setControlParams(SendData);
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x007C, 30);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0010: // дģ��ָ��
        CAN_DataCombine_Rx(RX_BUFFER);
        getAnalogParams(ReceData);
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0080, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0011: // ������ָ��
        SendData[0] = K_Udc;
        SendData[1] = Off_Udc;
        SendData[2] = K_Isa;
        SendData[3] = Off_Isa;
        SendData[4] = K_Isc;
        SendData[5] = Off_Isc;
        SendData[8] = K_Idc;
        SendData[9] = Off_Idc;
        SendData[12] = PWMA_a;
        SendData[13] = PWMA_b;
        SendData[14] = PWMA_c;
        SendData[22] = Us_M;
        SendData[23] = K_Isb;
        SendData[24] = Off_Isb;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0081, 27);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0012: // д��������ָ��
        CAN_DataCombine_Rx(RX_BUFFER);
        getOtherParams(ReceData);
        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0082, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0013: // ����������ָ��
        SendData[0] = Temperature1_K;
        SendData[1] = UdcLimit1_Set;
        // SendData[2] = TemperatureB1_K;
        // SendData[3] = Harm_LdivR;
        SendData[4] = ProtectNum;
        SendData[5] = UsSet;
        // SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
        SendData[7] = TorqueAngle;
        // SendData[8] = TemperatureB5_K;
        // SendData[9] = TemperatureB5_Off;
        SendData[10] = Temperature3_K;
        // SendData[11] = TemperatureB3_K;
        SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
        // SendData[13] = Sys_Flag.bit.RotorWay_Flag;
        SendData[14] = UdcLimit_Set;
        SendData[15] = Velo_Start; // �����ٶ�
        SendData[16] = maxSpeed;
        SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
        SendData[18] = TorquetoIs; // ת�ص�����
        SendData[19] = IdProtectNum;
        SendData[20] = VeloProtectNum;
        SendData[21] = Velo_Calc_Num;
        SendData[22] = Temperature1_Off;
        // SendData[23] = TemperatureB1_Off;
        SendData[24] = Temperature3_Off;
        // SendData[25] = TemperatureB3_Off;
        // SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
        SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
        SendData[28] = Temperature5_K;
        SendData[29] = Temperature5_Off;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x0083, 30);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0060: // д��EEPROMָ��

        WriteEEProm_Flag = 1;

        if (CANB.FLAG.bit.TXBO_FLAG == 1) {
            CAN_DataPackage_Tx(SendData, 0x00D0, 0);
            CANB.FLAG.bit.TXBO_FLAG = 0;
        }

        break;

    default:
        break;
    }
    while (CANB.FLAG.bit.TXBO_FLAG == 0) {
        Can_Tx_Data();
    }
}

void Can_Receive(void) {
    if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
        SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
        SCIB.m_Tx_Length = SCI_TX_BUFFER[4] + 7;
        SCIB.FLAG.bit.TXBO_FLAG = 0;
    }
}

void Can_Send(void) {
    volatile Uint16 command;
    command = (SCI_RX_BUFFER[2]);

    if (command == 0) {
        SendData[0] = Board_Num;
        SendData[1] = BORAD_NUM;
        if (CANB2.FLAG.bit.TXBO_FLAG == 1) {
            CAN2_DataPackage_Tx(SendData, 0x0000, 2);
        }
    } else {
        CANB2.p_Tx_Buffer = (Uint16 *)SCI_RX_BUFFER;
        CANB2.m_Tx_Length = SCI_RX_BUFFER[4] + 7;
    }
    CANB2.FLAG.bit.TXBO_FLAG = 0;

    while (CANB2.FLAG.bit.TXBO_FLAG == 0) {
        Can_Tx2_Data();
    }
}
