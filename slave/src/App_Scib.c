#include "F28335_Device.h"
#include "App_UserDef.h"

#include "core\command.h"

#pragma CODE_SECTION(Sci_DataPackage_Tx, "ramfuncs");
#pragma CODE_SECTION(Sci_DataCombine_Rx, "ramfuncs");
#pragma CODE_SECTION(ScibTxIsr, "ramfuncs");
#pragma CODE_SECTION(ScibRxIsr, "ramfuncs");
#pragma CODE_SECTION(ScibRxCheck, "ramfuncs");
#pragma CODE_SECTION(ScibTxCheck, "ramfuncs");
#pragma CODE_SECTION(WriteEEProm, "ramfuncs");

void ScibTxCheck(void) {
    if (SCIB.FLAG.bit.TXBO_FLAG == 0) {
        if (SCIB.m_Tx_Length > 0) {
            // ��д FIFO���ݣ� һ����д�����д12��
            // ��෢��12���ַ�,16�����׶��ַ�
            for (Uint16 i = ScicRegs.SCIFFTX.bit.TXFFST; i < 12; i++) {
                ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer;
                SCIB.p_Tx_Buffer++;
                SCIB.m_Tx_Length--;
                if (SCIB.m_Tx_Length == 0) {
                    break;
                }
            }
        } else {
            SCIB.FLAG.bit.TXBO_FLAG = 1;
            SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
        }
    }

    ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}

/**
 * �������ݽ���
 */
void ScibRxCheck(void) {
    volatile Uint16 Fi_Number;
    volatile Uint16 Store_Temp = 1;
    volatile Uint16 Sum_Temp = 0;

    /* ���ݽ��ռ�� */
    if (ScicRegs.SCIRXST.bit.BRKDT == 1) {
        ScicRegs.SCICTL1.bit.SWRESET = 0;
        ScicRegs.SCICTL1.bit.SWRESET = 1;

        ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
        ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
        SCIB.FLAG.bit.TXBO_FLAG = 1;
        SCIB.FLAG.bit.RX_FLAG = 0;
        SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
        SCIB.p_Rx_AllBuffer = SCI_RX_ALLBUFFER;
        SCIB.m_Rx_Length = 0;
    }

    /**
     * ���ٽ��յ�5���ֽڲŽ������ݽ�����
     * ���ݸ�ʽ����ͷ1+��ͷ2+������+������+���ݳ���+����+...+У��λ1+У��λ2
     */
    Uint16 isNotInSciRx = SCIB.FLAG.bit.RX_FLAG == 0;
    Uint16 isSciTxFinish = SCIB.FLAG.bit.TXBO_FLAG == 1;
    Uint16 isSciRxHaveData = SCIB.p_Rx_AllBuffer > (Uint16 *)(SCI_RX_ALLBUFFER + 4);
    if (isNotInSciRx && isSciTxFinish && isSciRxHaveData) {
        SCIB.FLAG.bit.RXTIME_FLAG = 1; /* ���ڽ������ݽ��� */

        /* ָ�������ж� */
        Uint16 isCustomCmd = SCI_RX_ALLBUFFER[2] == 0x0000;
        Uint16 isSelfCheckCmd = SCI_RX_ALLBUFFER[2] == 0x00A0;
        Uint16 isCtrlCmd = SCI_RX_ALLBUFFER[2] == 0x00A1;
        Uint16 isTelemetryCmd = SCI_RX_ALLBUFFER[2] == 0x00A2;
        if (isCustomCmd) {
            /* �Զ���Э������ */
            SCIB.FLAG.bit.PC_FLAG = 1;
        } else if (isSelfCheckCmd || isCtrlCmd || isTelemetryCmd) {
            /* Э������ */
            SCIB.FLAG.bit.PC_FLAG = 0;
        }

        if (SCI_RX_ALLBUFFER[0] != 0x00EB) {
            /* ������ǰ�ͷ1����ǰ�ƶ��������е����ݣ�Ϊ�������ڳ��ռ� */
            SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
            /* ѭ��������������ǰ�ƶ�һλ */
            for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
            }
        } else if (SCI_RX_ALLBUFFER[1] != 0x0090) {
            /* ��ͷ2 */
            SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
            for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
            }
        } else if (SCIB.FLAG.bit.PC_FLAG == 0 && !isSelfCheckCmd && !isCtrlCmd && !isTelemetryCmd) {
            /* δ�����ã����� */
            SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
            for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
            }
        } else {
            /* ���ݽ��� */
            SCIB.m_Rx_Length = SCI_RX_ALLBUFFER[4];
            if ((SCI_RX_ALLBUFFER[2] == 0x00A0 || SCI_RX_ALLBUFFER[2] == 0x00A2) && (SCIB.m_Rx_Length != 1)) {
                /* �Լ�ָ���ң��ָ�����ݳ��Ȳ���ȷ���������� */
                SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
                for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                    SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                }
            } else if (SCI_RX_ALLBUFFER[2] == 0x00A1 && SCIB.m_Rx_Length != 5) {
                /* ����ָ�����ݳ��Ȳ���ȷ���������� */
                SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
                for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                    SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                }
            } else if (SCIB.m_Rx_Length > (Uint16)(RX_BUFFER_LENGTH - 0x0007)) {
                /* ���ݳ��ȹ�������������  */
                SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
                for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                    SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                }
            } else if (SCIB.p_Rx_AllBuffer >= (Uint16 *)(SCI_RX_ALLBUFFER + SCIB.m_Rx_Length + 7)) {
                /* ���ݳ�����ȷ�������ݿ������������У� +7 =
                 * 5����ͷ1+��ͷ2+������+������+���ݳ��ȣ� + 2��CRC�������� */
                for (Uint16 i = 0; i < SCIB.m_Rx_Length + 7; i++) {
                    SCI_RX_BUFFER[i] = SCI_RX_ALLBUFFER[i];
                }
                Sum_Temp = CalCRC16_Byte(SCI_RX_BUFFER, SCIB.m_Rx_Length + 0x0005);

                if (Sum_Temp == SCI_RX_BUFFER[SCIB.m_Rx_Length + 5] + (SCI_RX_BUFFER[SCIB.m_Rx_Length + 6] << 8)) {
                    /* CRC����ͨ�����������ݷַ� */
                    SCIB.FLAG.bit.RX_FLAG = 1;

                    /* ���ݽ���ָ�����ƣ�������ѽ������� */
                    SCIB.p_Rx_AllBuffer = (SCIB.p_Rx_AllBuffer - SCIB.m_Rx_Length - 7);
                    for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                        SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + SCIB.m_Rx_Length + 7];
                    }
                } else {
                    /* CRC����ʧ�ܣ��������� */
                    SCIB.p_Rx_AllBuffer = (SCIB.p_Rx_AllBuffer - 1);
                    for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                        SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                    }
                }
            }
        }

        SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
        SCIB.m_Rx_Length = 0;
        SCIB.FLAG.bit.RXTIME_FLAG = 0;
    }
}

/*----------------------------------------------------------------------------------------
 �������ݴ������
 ����Ҫ���͵����ݰ���ͨѶ��Լ�����������
 ��ڲ���:��Ҫ���͵����ݵ�ַ�����͵������֡����͵����ݳ���(��)
 ���ڲ���:��
 ----------------------------------------------------------------------------------------*/
void Sci_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L) {
    volatile Uint16 i;
    volatile Uint16 Data_Temp;

    *SCIB.p_Tx_Buffer++ = 0x00EB;
    *SCIB.p_Tx_Buffer++ = 0x0090;
    *SCIB.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

    // ���͵������ֽ�������Ч���ֽ�
    *SCIB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) >> 8;
    *SCIB.p_Tx_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

    for (i = 0; i < m_tx_length_L; i++) {
        Data_Temp = ((Uint32)(*p_tx_data_L) >> 24) & 0x00FF;
        *SCIB.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = ((Uint32)(*p_tx_data_L) >> 16) & 0x00FF;
        *SCIB.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = ((Uint32)(*p_tx_data_L) >> 8) & 0x00FF;
        *SCIB.p_Tx_Buffer++ = Data_Temp;
        Data_Temp = (*p_tx_data_L) & 0x00FF;
        p_tx_data_L++;
        *SCIB.p_Tx_Buffer = Data_Temp;
        SCIB.p_Tx_Buffer++;
    }

    Data_Temp = CalCRC16_Byte(SCI_TX_BUFFER, (m_tx_length_L << 2) + 5);
    *SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); /* �ۼ���Ч���*/
    *SCIB.p_Tx_Buffer = (Data_Temp >> 8);       /* �ۼ���Ч���*/
    SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
    SCIB.m_Tx_Length = (m_tx_length_L << 2) + 2 + 5;
}

/*--------------------------------------------------------------------------------
 ���ڷ����жϷ����ӳ���
 -------------------------------------------------------------------------------*/
interrupt void ScibTxIsr(void) {
    ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}

/*--------------------------------------------------------------------------------
 ���ڽ����жϷ����ӳ���
 -------------------------------------------------------------------------------*/
interrupt void ScibRxIsr(void) {
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}

/**
 * �����յ������ݽ�����ϣ�ÿ�ĸ��ֽ���ϳ�һ��Uint32���͵����ݡ�
 *
 * @param p_Rx ָ��������ݵ�ָ�룬����p_Rx[4]ָʾ����Ҫ��ϵ�����������
 */
void Sci_DataCombine_Rx(Uint16 *p_Rx) {
    volatile Uint16 i = 0, j = 0;
    for (i = 1; i < p_Rx[4]; i += 4) {
        SCI_ReceData[j] = p_Rx[i + 4];
        SCI_ReceData[j] = (Uint32)SCI_ReceData[j] << 8;
        SCI_ReceData[j] += p_Rx[i + 5];
        SCI_ReceData[j] = (Uint32)SCI_ReceData[j] << 8;
        SCI_ReceData[j] += p_Rx[i + 6];
        SCI_ReceData[j] = (Uint32)SCI_ReceData[j] << 8;
        SCI_ReceData[j] += p_Rx[i + 7];
        j = j + 1;
    }

    SCIB.p_Rx_Buffer = SCI_RX_BUFFER;
}

//----------------------------------------------------------------------------------------
// EPROM�Ĵ������
// ����Ҫд��EEPROM�����ݰ���ͨѶ��Լ�����������
// ��ڲ���:��Ҫд������ݵ�ַ�����͵������֡����͵����ݳ���(��)
// ���ڲ���:��
//----------------------------------------------------------------------------------------

void EEPROM_DataPackage(_iq *p_tx_data_L, Uint16 m_tx_length_L) {
    volatile Uint16 i;
    volatile Uint16 Data_Temp;
    p_EEProm_Buffer = (Uint16 *)EEPROM_BUFFER;
    p_EEProm_Buffer++;
    p_EEProm_Buffer++;
    p_EEProm_Buffer++;
    *p_EEProm_Buffer++ = ((m_tx_length_L << 2)) >> 8;
    *p_EEProm_Buffer++ = ((m_tx_length_L << 2)) & 0x00FF;

    for (i = 0; i < m_tx_length_L; i++) {
        Data_Temp = ((*p_tx_data_L) >> 24) & 0x00FF;
        *p_EEProm_Buffer++ = Data_Temp;
        Data_Temp = ((*p_tx_data_L) >> 16) & 0x00FF;
        *p_EEProm_Buffer++ = Data_Temp;
        Data_Temp = ((*p_tx_data_L) >> 8) & 0x00FF;
        *p_EEProm_Buffer++ = Data_Temp;
        Data_Temp = (*p_tx_data_L++) & 0x00FF;
        *p_EEProm_Buffer++ = Data_Temp;
    }

    p_EEProm_Buffer = (Uint16 *)EEPROM_BUFFER;
}

void Cmd_Deal(void) {
    Uint16 command = SCI_RX_BUFFER[2];

    switch (command) {
    case 0x0000: /*��ʼ����������ָ��*/
        SCI_SendData[0] = Board_Num;
        SCI_SendData[1] = BORAD_NUM;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0070, 2);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0001:                              // ������ϱ�־ָ��
        Sys_Flag.bit.STOP_PWM_Flag_Id = 0;    // "1"���������������
        Sys_Flag.bit.STOP_PWM_Flag_Velo = 0;  // "1"�������ٱ���
        Sys_Flag.bit.UDC_FLAG = 0;            // "1"����ֱ����ѹ����
        Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0071, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000D:  // LOCK
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        Usa_Lock = SCI_ReceData[0];
        Usb_Lock = SCI_ReceData[1];
        Usc_Lock = SCI_ReceData[2];
        Udc_Lock = SCI_ReceData[3];
        Choose_Mche = SCI_ReceData[4];
        Ctrl_Flag.bit.LOCK_FLAG = 1;
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007D, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0002:  // Read Position
        SCI_SendData[0] = Isa;
        SCI_SendData[1] = Isb;
        SCI_SendData[2] = Isc;
        SCI_SendData[3] = rdc1.DATA_BUFFER;
        SCI_SendData[4] = Idc;
        SCI_SendData[7] = rdc2.DATA_BUFFER;

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0072, 8);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0003:  // Openloop
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        Um_OpenLoop = SCI_ReceData[0];
        Freq_OpenLoop = SCI_ReceData[1];
        Udc_OpenLoop = SCI_ReceData[2];
        Choose_Mche = SCI_ReceData[3];
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 1;
        Ctrl_Flag.bit.LOCK_FLAG = 0;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0073, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000E:  // Release LOCK
        Choose_Mche = SCI_ReceData[0];
        Ctrl_Flag.bit.LOCK_FLAG = 0;
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007E, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0004:
        /* ��ȡ������� */
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        getMotorParams(SCI_ReceData);

        p = _IQdiv(Mp, Rp); /* ���������ļ������� */
        Udc_Mche_realvalue = _IQdiv(_IQ(1), Udc_Setg);
        Velo_PerAdd = _IQmpy(Velo_Add, Per_Ctrl);
        Velo_Duty = _IQdiv(3750000, Control_Period);
        K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
        K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0);  // ��ʱ������Ϊ100us
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

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0074, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }

        break;
    case 0x0005:
        SCI_SendData[0] = Rs;
        SCI_SendData[1] = Ld;
        SCI_SendData[2] = Lq;
        SCI_SendData[3] = PHI;
        SCI_SendData[4] = Mp;
        SCI_SendData[5] = Rp;
        SCI_SendData[6] = Udc_Setg;
        SCI_SendData[7] = U_Base;
        SCI_SendData[8] = I_Base;
        SCI_SendData[9] = V_Base;  // ��е�ٶȻ�ֵ(rad/s)
        SCI_SendData[10] = Angle_Init_Digital;
        SCI_SendData[11] = Velo_Max;
        SCI_SendData[12] = id_Max;
        SCI_SendData[13] = Is_Add;
        SCI_SendData[14] = Control_Period;
        SCI_SendData[15] = id_Filter;
        SCI_SendData[16] = Udc_Filter;
        SCI_SendData[17] = Velo_Filter;
        SCI_SendData[18] = Deadtime;
        SCI_SendData[19] = Velo_Add;
        SCI_SendData[20] = Is_DownAdd;
        SCI_SendData[21] = Temp_Filter;
        SCI_SendData[22] = Angle_Init_Fa;
        SCI_SendData[23] = Com_angle_U;
        SCI_SendData[24] = Com_angle_I;
        SCI_SendData[25] = idSet_Max;
        // SCI_SendData[26] = Freq_cut;
        SCI_SendData[27] = IsLimit_Set;
        SCI_SendData[28] = MotorTM;
        SCI_SendData[29] = igbtTemperatureProtectionValue;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0075, 30);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0006:  // λ�ÿ���ָ��
        Sci_DataCombine_Rx(SCI_RX_BUFFER);

        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1) {
            SCI_SendData[0] = 1;
        } else if ((Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            SCI_SendData[0] = 2;
        } else {
            Isd_Set = SCI_ReceData[0];
            Pos_Set = SCI_ReceData[1];
            if (Pos_Set > POSMAX) {
                Pos_Set = POSMAX;
            }
            if (Pos_Set < POSMIN) {
                Pos_Set = POSMIN;
            }
            TorqueAngleA = SCI_ReceData[2];
            Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
            Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
            Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            SCI_SendData[0] = 0;
        }
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0076, 1);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }

        break;
    case 0x0007:  // �ٶȿ���ָ��
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1) {
            SCI_SendData[0] = 1;
        } else if ((Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            SCI_SendData[0] = 2;
        } else {
            Isd_Set = SCI_ReceData[0];
            Veloover_Set = SCI_ReceData[1];
            if (Veloover_Set > maxSpeed) {
                Veloover_Set = maxSpeed;
            }
            if (Veloover_Set < -maxSpeed) {
                Veloover_Set = -maxSpeed;
            }
            TorqueAngleA = SCI_ReceData[2];
            Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
            Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
            Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            SCI_SendData[0] = 0;
        }
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0077, 1);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0008:
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        Choose_Mche = SCI_ReceData[0];
        Choose_Mode = SCI_ReceData[1];
        SCI_SendData[0] = 0;
        SCI_SendData[1] = 0;
        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 1) {
            SCI_SendData[0] = 1;
        } else if ((Ctrl_Flag.bit.FIRST_PWM_Flag == 1 && Velo_Elec_abs > Velo_Start)) {
            SCI_SendData[0] = 2;
        } else {
            if (Choose_Mode == 0) {
                Isd_Set = SCI_ReceData[2];
                Isdq_Set1 = -SCI_ReceData[3];
                TorqueAngleA = SCI_ReceData[4];
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;  // ��ת�ؿ��Ʊ�־
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
            } else if (Choose_Mode == 1) {
                Isd_Set = SCI_ReceData[2];
                Veloover_Set = SCI_ReceData[3];
                if (Veloover_Set > maxSpeed) {
                    Veloover_Set = maxSpeed;
                }
                if (Veloover_Set < -maxSpeed) {
                    Veloover_Set = -maxSpeed;
                }
                TorqueAngleA = SCI_ReceData[4];
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 0;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            } else {
                Isd_Set = SCI_ReceData[2];
                Pos_Set = SCI_ReceData[3];
                if (Pos_Set > POSMAX) {
                    Pos_Set = POSMAX;
                }
                if (Pos_Set < POSMIN) {
                    Pos_Set = POSMIN;
                }
                TorqueAngleA = SCI_ReceData[4];
                Ctrl_Flag.bit.VELO_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 0;
                Ctrl_Flag.bit.POS_CONTROL_FLAG = 1;
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1;
            }
            SCI_SendData[0] = 0;
        }

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0078, 2);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0009:
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        Choose_Mche = SCI_ReceData[0];
        Sys_Flag.bit.RDC_FAULT = rdc1.FAULT;
        SCI_SendData[0] = Velo_Elec;
        SCI_SendData[1] = Udc_Mche_1;
        SCI_SendData[2] = Id;
        SCI_SendData[3] = Iq;
        SCI_SendData[4] = Sys_Flag.all;
        SCI_SendData[5] = IdZ_Set;
        SCI_SendData[6] = IqZ_Set;
        SCI_SendData[7] = Us;
        SCI_SendData[8] = pi_Id_Kc.ui_reg;
        SCI_SendData[9] = pi_Iq_Kc.ui_reg;
        SCI_SendData[10] = Ud;
        SCI_SendData[11] = Uq;
        SCI_SendData[12] = Time_Cal;
        // SCI_SendData[12] = Id_5;
        SCI_SendData[13] = Temperature1;
        SCI_SendData[14] = Veloover_Set;
        SCI_SendData[15] = Temperature3;
        // SCI_SendData[16] = Iq_5;
        SCI_SendData[17] = (Ctrl_Flag.all & 0x000F) + (rdc2.FAULT << 4);
        SCI_SendData[18] = Temperature5;
        SCI_SendData[19] = Idc;
        // SCI_SendData[20] = Id_7;
        // SCI_SendData[21] = Iq_7;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0079, 22);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
            // ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
            // ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//ʹ�ܷ����ж�
        }
        break;
    case 0x000A:
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        Choose_Mche = SCI_ReceData[0];
        Ctrl_Flag.bit.STOP_VELO_FLAG = 1;

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007A, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
            // ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
            // ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//ʹ�ܷ����ж�
        }
        break;
    case 0x000B:
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        getControlParams(SCI_ReceData);
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007B, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
            // ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
            // ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//ʹ�ܷ����ж�
        }
        break;
    case 0x000C:
        setControlParams(SCI_SendData);

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007C, 30);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0010: /* ��ȡģ��ͨ������ */
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        getAnalogParams(SCI_ReceData);

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0080, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0011:
        SCI_SendData[0] = K_Udc;
        SCI_SendData[1] = Off_Udc;
        SCI_SendData[2] = K_Isa;
        SCI_SendData[3] = Off_Isa;
        SCI_SendData[4] = K_Isc;
        SCI_SendData[5] = Off_Isc;
        // SCI_SendData[6] = K_IsbB;
        // SCI_SendData[7] = Off_IsbB;
        SCI_SendData[8] = K_Idc;
        SCI_SendData[9] = Off_Idc;
        // SCI_SendData[10] = K_IscB;
        // SCI_SendData[11] = Off_IscB;
        SCI_SendData[12] = PWMA_a;
        SCI_SendData[13] = PWMA_b;
        SCI_SendData[14] = PWMA_c;
        // SCI_SendData[15] = PWMB_a;
        // SCI_SendData[16] = PWMB_b;
        // SCI_SendData[17] = PWMB_c;
        // SCI_SendData[18] = Sys_Flag.bit.HARM_Flag;
        // SCI_SendData[19] = Freq_Min_G;
        // SCI_SendData[20] = Freq_Min_K;
        SCI_SendData[22] = Us_M;
        SCI_SendData[23] = K_Isb;
        SCI_SendData[24] = Off_Isb;
        // SCI_SendData[25] = K_IsaB;
        // SCI_SendData[26] = Off_IsaB;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0081, 27);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0012:
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        getOtherParams(SCI_ReceData);
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0082, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0013:
        SCI_SendData[0] = Temperature1_K;
        SCI_SendData[1] = UdcLimit1_Set;
        // SCI_SendData[2] = TemperatureB1_K;
        // SCI_SendData[3] = Harm_LdivR;
        SCI_SendData[4] = ProtectNum;
        SCI_SendData[5] = UsSet;
        // SCI_SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
        SCI_SendData[7] = TorqueAngle;
        // SCI_SendData[8] = TemperatureB5_K;
        // SCI_SendData[9] = TemperatureB5_Off;
        SCI_SendData[10] = Temperature3_K;
        // SCI_SendData[11] = TemperatureB3_K;
        SCI_SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
        // SCI_SendData[13] = Sys_Flag.bit.RotorWay_Flag;
        SCI_SendData[14] = UdcLimit_Set;
        SCI_SendData[15] = Velo_Start;  // �����ٶ�
        SCI_SendData[16] = maxSpeed;
        SCI_SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
        SCI_SendData[18] = TorquetoIs;  // ת�ص�����
        SCI_SendData[19] = IdProtectNum;
        SCI_SendData[20] = VeloProtectNum;
        SCI_SendData[21] = Velo_Calc_Num;
        SCI_SendData[22] = Temperature1_Off;
        // SCI_SendData[23] = TemperatureB1_Off;
        SCI_SendData[24] = Temperature3_Off;
        // SCI_SendData[25] = TemperatureB3_Off;
        // SCI_SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
        SCI_SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
        SCI_SendData[28] = Temperature5_K;
        SCI_SendData[29] = Temperature5_Off;

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0083, 30);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0060:  // д��EEPROMָ��
        WriteEEProm_Flag = 1;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x00D0, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    default:
        break;
    }
}

void WriteEEProm(void) {
    volatile Uint16 i = 0;
    Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
    Forbid_PWMA();

    DINT;
    RDC_DIS_B;
    RDC_DIS_A;
    DIS_WP = 1;
    SCI_SendData[0] = Rs;
    SCI_SendData[1] = Ld;
    SCI_SendData[2] = Lq;
    SCI_SendData[3] = PHI;
    SCI_SendData[4] = Mp;
    SCI_SendData[5] = Rp;
    SCI_SendData[6] = Udc_Setg;
    SCI_SendData[7] = U_Base;
    SCI_SendData[8] = I_Base;
    SCI_SendData[9] = V_Base;  // ��е�ٶȻ�ֵ(rad/s)
    SCI_SendData[10] = Angle_Init_Digital;
    SCI_SendData[11] = Velo_Max;
    SCI_SendData[12] = id_Max;
    SCI_SendData[13] = Is_Add;
    SCI_SendData[14] = Control_Period;
    SCI_SendData[15] = id_Filter;
    SCI_SendData[16] = Udc_Filter;
    SCI_SendData[17] = Velo_Filter;
    SCI_SendData[18] = Deadtime;
    SCI_SendData[19] = Velo_Add;
    SCI_SendData[20] = Is_DownAdd;
    SCI_SendData[21] = Temp_Filter;
    SCI_SendData[22] = Angle_Init_Fa;
    SCI_SendData[23] = Com_angle_U;
    SCI_SendData[24] = Com_angle_I;
    SCI_SendData[25] = idSet_Max;
    // SCI_SendData[26] = Freq_cut;
    SCI_SendData[27] = IsLimit_Set;
    SCI_SendData[28] = MotorTM;
    SCI_SendData[29] = igbtTemperatureProtectionValue;

    EEPROM_DataPackage(SCI_SendData, 30);
    for (i = 0; i < 127; i++) {
        WRITE_X25040(&EEPROM_BUFFER[0 + i], 128 + i, 1, 0);
    }

    SCI_SendData[0] = pi_velo.Kp_reg;
    SCI_SendData[3] = pi_velo.Ki_reg;
    SCI_SendData[6] = pi_velo.pi_out_max;
    SCI_SendData[8] = pi_velo.i_period;
    SCI_SendData[9] = pi_velo.ctrl_period;

    SCI_SendData[1] = pi_usLimit.Kp_reg;
    SCI_SendData[2] = pi_usLimit.Ki_reg;
    SCI_SendData[7] = -pi_usLimit.pi_out_min;
    SCI_SendData[5] = pi_usLimit.i_period;
    SCI_SendData[4] = pi_usLimit.ctrl_period;

    SCI_SendData[10] = pi_Pos.Kp_reg;
    SCI_SendData[13] = pi_Pos.Ki_reg;
    SCI_SendData[16] = pi_Pos.pi_out_max;
    SCI_SendData[18] = pi_Pos.i_period;
    SCI_SendData[19] = pi_Pos.ctrl_period;

    SCI_SendData[11] = Idq_Kc;
    SCI_SendData[12] = Radq;
    SCI_SendData[17] = pi_Id_Kc.pi_out_max;
    SCI_SendData[15] = pi_Id_Kc.i_period;
    SCI_SendData[14] = pi_Id_Kc.ctrl_period;

    // SCI_SendData[21] 	= 	HarmCtrl_Kp;
    // SCI_SendData[22] 	= 	HarmCtrl_Ki;
    // SCI_SendData[27] 	= 	pi_Id_5.pi_out_max;
    // SCI_SendData[25] 	=	pi_Id_5.i_period;
    // SCI_SendData[24] 	=	pi_Id_5.ctrl_period;

    EEPROM_DataPackage(SCI_SendData, 30);
    for (i = 0; i < 127; i++) WRITE_X25040(&EEPROM_BUFFER[0 + i], 0 + i, 1, 0);

    SCI_SendData[0] = K_Udc;
    SCI_SendData[1] = Off_Udc;
    SCI_SendData[2] = K_Isa;
    SCI_SendData[3] = Off_Isa;
    SCI_SendData[4] = K_Isc;
    SCI_SendData[5] = Off_Isc;
    // SCI_SendData[6] = K_IsbB;
    // SCI_SendData[7] = Off_IsbB;
    SCI_SendData[8] = K_Idc;
    SCI_SendData[9] = Off_Idc;
    // SCI_SendData[10] = K_IscB;
    // SCI_SendData[11] = Off_IscB;
    SCI_SendData[12] = PWMA_a;
    SCI_SendData[13] = PWMA_b;
    SCI_SendData[14] = PWMA_c;
    // SCI_SendData[15] = PWMB_a;
    // SCI_SendData[16] = PWMB_b;
    // SCI_SendData[17] = PWMB_c;
    // SCI_SendData[18] = Sys_Flag.bit.HARM_Flag;
    // SCI_SendData[19] = Freq_Min_G;
    // SCI_SendData[20] = Freq_Min_K;
    SCI_SendData[22] = Us_M;
    SCI_SendData[23] = K_Isb;
    SCI_SendData[24] = Off_Isb;
    // SCI_SendData[25] = K_IsaB;
    // SCI_SendData[26] = Off_IsaB;

    EEPROM_DataPackage(SCI_SendData, 27);
    for (i = 0; i < 127; i++) {
        WRITE_X25040(&EEPROM_BUFFER[0 + i], 0 + i, 1, 8);
    }

    SCI_SendData[0] = Temperature1_K;
    SCI_SendData[1] = UdcLimit1_Set;
    // SCI_SendData[2] = TemperatureB1_K;
    // SCI_SendData[3] = Harm_LdivR;
    SCI_SendData[4] = ProtectNum;
    SCI_SendData[5] = UsSet;
    // SCI_SendData[6] = Ctrl_Flag.bit.CanCtrl_Flag;
    SCI_SendData[7] = TorqueAngle;
    // SCI_SendData[8] = TemperatureB5_K;
    // SCI_SendData[9] = TemperatureB5_Off;
    SCI_SendData[10] = Temperature3_K;
    // SCI_SendData[11] = TemperatureB3_K;
    SCI_SendData[12] = Ctrl_Flag.bit.RotorWay_Flag;
    // SCI_SendData[13] = Sys_Flag.bit.RotorWay_Flag;
    SCI_SendData[14] = UdcLimit_Set;
    SCI_SendData[15] = Velo_Start;  // �����ٶ�
    SCI_SendData[16] = maxSpeed;
    SCI_SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
    SCI_SendData[18] = TorquetoIs;  // ת�ص�����
    SCI_SendData[19] = IdProtectNum;
    SCI_SendData[20] = VeloProtectNum;
    SCI_SendData[21] = Velo_Calc_Num;
    SCI_SendData[22] = Temperature1_Off;
    // SCI_SendData[23] = TemperatureB1_Off;
    SCI_SendData[24] = Temperature3_Off;
    // SCI_SendData[25] = TemperatureB3_Off;
    // SCI_SendData[26] = CtrlB_Flag.bit.VelWay_Flag;
    SCI_SendData[27] = Ctrl_Flag.bit.UsCtrl_Flag;
    SCI_SendData[28] = Temperature5_K;
    SCI_SendData[29] = Temperature5_Off;

    EEPROM_DataPackage(SCI_SendData, 30);
    for (i = 0; i < 127; i++) WRITE_X25040(&EEPROM_BUFFER[0 + i], 128 + i, 1, 8);
    EN_WP = 1;

    EINT;

    WriteEEProm_Flag = 0;
}
