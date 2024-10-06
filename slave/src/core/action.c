#include "action.h"

Uint16 getFrameCount() {
    static Uint16 frameCount = 0;

    Uint16 _frameCount = frameCount;

    frameCount++;
    if (frameCount > 255) {
        frameCount = 0;
    }

    return _frameCount;
}

void disableResolver() {
    /* RDC: Resolver Digital Converter */
    RDC_DIS_B;
    RDC_DIS_A;
}

void readResolver() {
    RDC_READ();
    Angle0_Q16 = rdc1.DATA_BUFFER - Angle_Init_Digital;
    Angle_Fa_Q16 = rdc2.DATA_BUFFER - Angle_Init_Fa;
}

void hardwareProtection() {
    if (INT_IGBT == 0) {
        Forbid_PWMA(); /* ��ֹPWM��� */

        Ctrl_Flag.bit.STOP_VELO_FLAG = 1;    /* ֹͣ�ٶȿ��Ʊ�־λ�� 1��Ч */
        Sys_Flag.bit.STOP_PWM_Flag_Driv = 1; /* Ӳ�����ϱ�־λ��1��Ч */
        Ctrl_Flag.bit.STOP_PWM_Flag = 1;     /* ����ֹͣ��־��1��Ч*/
    }
}

void writeParamsIntoEeprom() {
    if (WriteEEProm_Flag == 1 && SCIB.FLAG.bit.TXBO_FLAG == 1 && CANB.FLAG.bit.TXBO_FLAG == 1) {
        WriteEEProm();
    }
}

Uint16 getCpuStatus() {
    Uint16 status = 0;

    /* ����1����CPU״̬ */
    if (CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x02) == 0x00) {
        status = 0x0001;
    }

    /* ����1��CPU״̬ */
    if (CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x02) == 0x00) {
        status += 0x0004;
    }

    /* ����2����CPU״̬ */
    if (CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x02) == 0x00) {
        status += 0x0010;
    }

    /* ����2��CPU״̬ */
    if (CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x02) == 0x00) {
        status += 0x0040;
    }

    /* �õ������CPU״̬ */
    if (CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x02) == 0x00) {
        status += 0x0100;
    }

    /* �õ����CPU״̬ */
    if (CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x02) == 0x00) {
        status += 0x0400;
    }

    return status;
}

Uint16 getPowerSupplyStatus() {
    Uint16 status = 0;

    /* 28V��Դ״̬1 */
    Uint16 v11 = (CAN_BUFFER_RT[0][2] & 0x20) == 0x00;
    Uint16 v12 = (CAN_BUFFER_RT[0][3] & 0x01) == 0x00;
    Uint16 v13 = (CAN_BUFFER_RT[1][2] & 0x20) == 0x00;
    Uint16 v14 = (CAN_BUFFER_RT[1][3] & 0x01) == 0x00;
    if (v11 && v12 && v13 && v14) {
        status = 0x0001;
    }

    /* 28V��Դ״̬2 */
    Uint16 v21 = (CAN_BUFFER_RT[4][2] & 0x20) == 0x00;
    Uint16 v22 = (CAN_BUFFER_RT[4][3] & 0x01) == 0x00;
    Uint16 v23 = (CAN_BUFFER_RT[5][2] & 0x20) == 0x00;
    Uint16 v24 = (CAN_BUFFER_RT[5][3] & 0x01) == 0x00;
    if (v21 && v22 && v23 && v24) {
        status += 0x0004;
    }

    /* 270V��Դ״̬1 */
    Uint16 v31 = (CAN_BUFFER_RT[2][2] & 0x20) == 0x00;
    Uint16 v32 = (CAN_BUFFER_RT[2][3] & 0x01) == 0x00;
    if (v31 && v32) {
        status += 0x0010;
    }

    /* 270V��Դ״̬2 */
    Uint16 v41 = (CAN_BUFFER_RT[3][2] & 0x20) == 0x00;
    Uint16 v42 = (CAN_BUFFER_RT[3][3] & 0x01) == 0x00;
    if (v41 && v42) {
        status += 0x0040;
    }

    return status & 0x00FF;
}

void selfCheckResponse() {
    Uint16 isMainCpu = (BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0;
    Uint16 isSubCpu = (BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1;
    if (isMainCpu || isSubCpu) {
        Ctrl_Flag.bit.CANERROR_FLAG = 0;
        CAN_Index = 0;
        for (Uint16 i = 0; i < 8; i++) {
            CAN_BUFFER_RX[i] = 0;
        }
        CAN_BUFFER_RX[0] = SCI_RX_BUFFER[2];
        Can_Tx11_Data();

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            *SCIB.p_Tx_Buffer++ = 0x00EB;          /* ��ͷ1 */
            *SCIB.p_Tx_Buffer++ = 0x0090;          /* ��ͷ2 */
            *SCIB.p_Tx_Buffer++ = 0x0050;          /* ������ */
            *SCIB.p_Tx_Buffer++ = getFrameCount(); /* ������ */
            *SCIB.p_Tx_Buffer++ = 0x0008;          /* ������*/

            /*CPU ״̬*/
            Uint16 cpuStatus = getCpuStatus();
            *SCIB.p_Tx_Buffer++ = cpuStatus & 0x00FF;
            *SCIB.p_Tx_Buffer++ = (cpuStatus >> 8);

            /* ����1״̬ */
            *SCIB.p_Tx_Buffer++ = 0x0001;

            /* ����2״̬ */
            *SCIB.p_Tx_Buffer++ = 0x0001;

            /* ����״̬ */
            *SCIB.p_Tx_Buffer++ = getPowerSupplyStatus();

            /* ����汾: 2.05.A */
            *SCIB.p_Tx_Buffer++ = 0x0005;
            *SCIB.p_Tx_Buffer++ = 0x0041;

            /* ���Ƶ�λ����Ȫ�����FA��*/
            *SCIB.p_Tx_Buffer++ = 0x00FA;

            /* CRC16У��� */
            Uint16 crc = CalCRC16_Byte(SCI_TX_BUFFER, 13);
            *SCIB.p_Tx_Buffer++ = (crc & 0x00FF); /* �ۼ���Ч���*/
            *SCIB.p_Tx_Buffer = (crc >> 8);       /* �ۼ���Ч���*/

            SCIB.p_Tx_Buffer = SCI_TX_BUFFER; /* ? */
            SCIB.m_Tx_Length = 15;            /* �Լ���Ӧ֡15���ֽڣ�����CRC��*/
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
    }
}

Uint16 getDriverStatus() {
    Uint16 status = 0;

    /* ����1������״̬ */
    if (CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x01) == 0x00) {
        status = 0x0001;
    }

    /* ����1������״̬ */
    if (CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x00) {
        status += 0x0002;
    }

    /* ����2������״̬ */
    if (CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x01) == 0x00) {
        status += 0x0004;
    }

    /* ����2������״̬ */
    if (CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x00) {
        status += 0x0008;
    }

    /* �õ��������״̬ */
    if (CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x01) == 0x00) {
        status += 0x0010;
    }

    /* �õ��������״̬ */
    if (CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x00) {
        status += 0x0020;
    }

    return status;
}

void _getValve1Params(Uint16 index) {
    /* ����1 ʵ�ʽǶ� */
    CAN_BUFFER_TX[2] = CAN_BUFFER_RT[index][0];
    CAN_BUFFER_TX[3] = CAN_BUFFER_RT[index][1];

    /* ����1 CPU �¶� */
    CAN_BUFFER_TX[6 + 6] = CAN_BUFFER_RT[index][4];
    CAN_BUFFER_TX[7 + 6] = CAN_BUFFER_RT[index][5];

    /* ����1 ĸ�ߵ�ѹ */
    CAN_BUFFER_TX[12 + 6] = CAN_BUFFER_RT[index][6];
    CAN_BUFFER_TX[13 + 6] = CAN_BUFFER_RT[index][7];

    /* ����1 ĸ�ߵ���*/
    CAN_BUFFER_TX[18 + 6] = CAN_BUFFER_RT[index][9];
    CAN_BUFFER_TX[19 + 6] = CAN_BUFFER_RT[index][10];

    /* ����1 A����� */
    CAN_BUFFER_TX[24 + 6] = CAN_BUFFER_RT[index][11];
    CAN_BUFFER_TX[25 + 6] = CAN_BUFFER_RT[index][12];

    /* ����1 B����� */
    CAN_BUFFER_TX[26 + 6] = CAN_BUFFER_RT[index][13];
    CAN_BUFFER_TX[27 + 6] = CAN_BUFFER_RT[index][14];

    /* ����1 C����� */
    CAN_BUFFER_TX[28 + 6] = CAN_BUFFER_RT[index][15];
    CAN_BUFFER_TX[29 + 6] = CAN_BUFFER_RT[index][16];

    /* ����1 ������ */
    CAN_BUFFER_TX[42 + 6] = 0;
    CAN_BUFFER_TX[43 + 6] = 0;
    CAN_BUFFER_TX[44 + 6] = 0;

    /* ����1 28V��ѹ */
    if ((CAN_BUFFER_RT[index][2] & 0x20) == 0x00) {
        CAN_BUFFER_TX[42 + 6] = 0x0001;
    }

    /* ����1 28VǷѹ */
    if ((CAN_BUFFER_RT[index][3] & 0x01) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0008;
    }

    /* ����1 28V���� */
    if ((CAN_BUFFER_RT[index][2] & 0x04) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0040;
    }

    /* ����1 Ӳ������ */
    if ((CAN_BUFFER_RT[index][2] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0002;
    }

    /* ����1 �����λ */
    if ((CAN_BUFFER_RT[index][3] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0010;
    }

    /* ����1 �����䳬�� */
    if ((CAN_BUFFER_RT[index][3] & 0x04) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0040;
    }

    /* ����1 �����䳬�� */
    if ((CAN_BUFFER_RT[index][3] & 0x08) == 0x00) {
        CAN_BUFFER_TX[44 + 6] += 0x0001;
    }

    /* ����1 �������䳬�� */
    if ((CAN_BUFFER_RT[index][3] & 0x20) == 0x20) {
        CAN_BUFFER_TX[44 + 6] += 0x0004;
    }
}

void getValve1RunningParams() {
    if ((CAN_CONTROL & 0x01) == 0x00) {
        _getValve1Params(0);
    } else {
        _getValve1Params(1);
    }
}

void _getValve2Params(Uint16 index) {
    /* ����2ʵ�ʽǶ� */
    CAN_BUFFER_TX[6] = CAN_BUFFER_RT[index][0];
    CAN_BUFFER_TX[7] = CAN_BUFFER_RT[index][1];

    CAN_BUFFER_TX[8 + 6] = CAN_BUFFER_RT[index][4];
    CAN_BUFFER_TX[9 + 6] = CAN_BUFFER_RT[index][5];
    CAN_BUFFER_TX[14 + 6] = CAN_BUFFER_RT[index][6];
    CAN_BUFFER_TX[15 + 6] = CAN_BUFFER_RT[index][7];
    CAN_BUFFER_TX[20 + 6] = CAN_BUFFER_RT[index][9];
    CAN_BUFFER_TX[21 + 6] = CAN_BUFFER_RT[index][10];
    CAN_BUFFER_TX[30 + 6] = CAN_BUFFER_RT[index][11];
    CAN_BUFFER_TX[31 + 6] = CAN_BUFFER_RT[index][12];
    CAN_BUFFER_TX[32 + 6] = CAN_BUFFER_RT[index][13];
    CAN_BUFFER_TX[33 + 6] = CAN_BUFFER_RT[index][14];
    CAN_BUFFER_TX[34 + 6] = CAN_BUFFER_RT[index][15];
    CAN_BUFFER_TX[35 + 6] = CAN_BUFFER_RT[index][16];

    if ((CAN_BUFFER_RT[index][2] & 0x20) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0002;
    }

    if ((CAN_BUFFER_RT[index][3] & 0x01) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0010;
    }

    if ((CAN_BUFFER_RT[index][2] & 0x04) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0080;
    }

    if ((CAN_BUFFER_RT[index][2] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0004;
    }

    if ((CAN_BUFFER_RT[index][3] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0020;
    }

    /* ����2���䳬�� */
    if ((CAN_BUFFER_RT[index][3] & 0x04) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0080;
    }

    /* ����������� */
    if ((CAN_BUFFER_RT[index][3] & 0x20) == 0x20) {
        CAN_BUFFER_TX[44 + 6] += 0x0008;
    }
}

void getValve2RunningParams() {
    if ((CAN_CONTROL & 0x04) == 0x00) {
        _getValve2Params(4);
    } else {
        _getValve2Params(5);
    }
}

void _getPumpParams(Uint16 index) {
    /* �õ�� ʵ��ת�� */
    CAN_BUFFER_TX[10] = CAN_BUFFER_RT[index][0];
    CAN_BUFFER_TX[11] = CAN_BUFFER_RT[index][1];

    /* �õ�� CPU�¶� */
    CAN_BUFFER_TX[10 + 6] = CAN_BUFFER_RT[index][4];
    CAN_BUFFER_TX[11 + 6] = CAN_BUFFER_RT[index][5];

    /* �õ�� ĸ�ߵ�ѹ */
    CAN_BUFFER_TX[16 + 6] = CAN_BUFFER_RT[index][6];
    CAN_BUFFER_TX[17 + 6] = CAN_BUFFER_RT[index][7];

    /* �õ�� ĸ�ߵ��� */
    CAN_BUFFER_TX[22 + 6] = CAN_BUFFER_RT[index][9];
    CAN_BUFFER_TX[23 + 6] = CAN_BUFFER_RT[index][10];

    /* �õ�� A����� */
    CAN_BUFFER_TX[36 + 6] = CAN_BUFFER_RT[index][11];
    CAN_BUFFER_TX[37 + 6] = CAN_BUFFER_RT[index][12];

    /* �õ�� B����� */
    CAN_BUFFER_TX[38 + 6] = CAN_BUFFER_RT[index][13];
    CAN_BUFFER_TX[39 + 6] = CAN_BUFFER_RT[index][14];

    /* �õ�� C����� */
    CAN_BUFFER_TX[40 + 6] = CAN_BUFFER_RT[index][15];
    CAN_BUFFER_TX[41 + 6] = CAN_BUFFER_RT[index][16];

    /* �õ�� ĸ�߹�ѹ */
    if ((CAN_BUFFER_RT[index][2] & 0x20) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0004;
    }

    /* �õ�� ĸ��Ƿѹ */
    if ((CAN_BUFFER_RT[index][3] & 0x01) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0020;
    }

    /* �õ�� ĸ�߹��� */
    if ((CAN_BUFFER_RT[index][2] & 0x04) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0001;
    }

    /* �õ�� Ӳ������ */
    if ((CAN_BUFFER_RT[index][2] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0008;
    }

    /* �õ�� ��������� */
    if ((CAN_BUFFER_RT[index][3] & 0x04) == 0x00) {
        CAN_BUFFER_TX[44 + 6] += 0x0002;
    }

    /* �õ�� ��������� */
    if ((CAN_BUFFER_RT[index][3] & 0x20) == 0x20) {
        CAN_BUFFER_TX[44 + 6] += 0x0010;
    }
}

void getPumpRunningParams() {
    if ((CAN_CONTROL & 0x02) == 0x00) {
        _getPumpParams(2);
    } else {
        _getPumpParams(3);
    }
}

void _setTelemeterResponseData() {
    *SCIB.p_Tx_Buffer++ = 0x00EB;          /* ��ͷ1 */
    *SCIB.p_Tx_Buffer++ = 0x0090;          /* ��ͷ2 */
    *SCIB.p_Tx_Buffer++ = 0x0052;          /* ֡���� */
    *SCIB.p_Tx_Buffer++ = getFrameCount(); /* ������ */
    *SCIB.p_Tx_Buffer++ = 0x0036;          /* ������ */

    /* ����״̬λ */
    Uint16 driverStatus = getDriverStatus();
    *SCIB.p_Tx_Buffer++ = driverStatus & 0x00FF;
    *SCIB.p_Tx_Buffer++ = (driverStatus >> 8);

    CAN_CONTROL = 0x07;
    // ��ѡ��Ӱ�,��������޹��ϣ��ҴӰ岻�������У���ѡ����������
    if ((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01) {
        CAN_CONTROL &= 0xFE;
    }

    if ((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01) {
        CAN_CONTROL &= 0xFD;
    }

    if ((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01) {
        CAN_CONTROL &= 0xFB;
    }

    /* ����1�趨�Ƕ� */
    CAN_BUFFER_TX[0] = CAN_SET_VALUE[0];
    CAN_BUFFER_TX[1] = CAN_SET_VALUE[1];

    /* ����2�趨�Ƕ�*/
    CAN_BUFFER_TX[4] = CAN_SET_VALUE[2];
    CAN_BUFFER_TX[5] = CAN_SET_VALUE[3];

    /* �õ���趨ת�� */
    CAN_BUFFER_TX[8] = CAN_SET_VALUE[4];
    CAN_BUFFER_TX[9] = CAN_SET_VALUE[5];

    /* ��ȡ����1���в��� */
    getValve1RunningParams();

    /* ��ȡ����2���в��� */
    getValve2RunningParams();

    /* ��ȡ�õ�����в��� */
    getPumpRunningParams();

    CAN_BUFFER_TX[51] = 0;
    for (Uint16 i = 0; i < 52; i++) {
        *SCIB.p_Tx_Buffer++ = CAN_BUFFER_TX[i];
    }

    /* CRC У�� */
    Uint16 crc = CalCRC16_Byte(SCI_TX_BUFFER, 53 + 6);
    *SCIB.p_Tx_Buffer++ = (crc & 0x00FF); /* �ۼ���Ч���*/
    *SCIB.p_Tx_Buffer = (crc >> 8);       /* �ۼ���Ч���*/

    SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
    SCIB.m_Tx_Length = 61;
    SCIB.FLAG.bit.TXBO_FLAG = 0;
}

void telemeteringResponse() {
    volatile Uint16 Data_Temp = 0;

    Uint16 isMainCpu = (BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0;
    Uint16 isSubCpu = (BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1;
    if (isMainCpu || isSubCpu) {
        Ctrl_Flag.bit.CANERROR_FLAG = 0;
        CAN_Index = 0;
        for (Uint16 i = 0; i < 8; i++)
            CAN_BUFFER_RX[i] = 0;
        CAN_BUFFER_RX[0] = SCI_RX_BUFFER[2];
        Can_Tx11_Data();

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            _setTelemeterResponseData();
        }
    }
}

void controlResponse() {
    volatile _iq Set_Temp = 0;

    /* CANͨѶ���8���ֽ����� */
    for (Uint16 i = 0; i < 8; i++) {
        CAN_BUFFER_RX[i] = 0;
    }
    CAN_BUFFER_RX[0] = SCI_RX_BUFFER[2]; /* ָ������ */

<<<<<<< HEAD
    CAN_BUFFER_RX[1] = 0x07;             // ��ѡ��Ӱ�
    == == == =
                 CAN_BUFFER_RX[1] = 0x07; // ��ѡ��Ӱ�
>>>>>>> refs/remotes/origin/main
    // ��������޹��ϣ��ҴӰ岻�������У���ѡ����������
    if ((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01) {
        CAN_BUFFER_RX[1] &= 0xFE;
    }

    if ((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01) {
        CAN_BUFFER_RX[1] &= 0xFD;
    }

    if ((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01) {
        CAN_BUFFER_RX[1] &= 0xFB;
    }

<<<<<<< HEAD
    CAN_CONTROL = CAN_BUFFER_RX[1];
    == == == =
                 CAN_CONTROL = CAN_BUFFER_RX[1]; // �忨ѡ��
>>>>>>> refs/remotes/origin/main
    for (Uint16 i = 2; i < 7; i++) {
        CAN_BUFFER_RX[i] = SCI_RX_BUFFER[3 + i]; /* ������+������� */
    }

    if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A) {
        /* ֹͣ����1 */
        CAN_SET_VALUE[0] = 0;
        CAN_SET_VALUE[1] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5) {
        /* ��������1 */
        CAN_SET_VALUE[0] = 0;
        CAN_SET_VALUE[1] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x12) {
        /* ���õ���1�Ƕ�  */
        _iq angle = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);
<<<<<<< HEAD
        angle = _IQmpy(angle, 3200); // 100>>15
        == == == =
                     angle = _IQmpy(angle, 3200); // 100>>15
>>>>>>> refs/remotes/origin/main
        if (angle >= 65535) {
            angle = 65535;
        }
        CAN_SET_VALUE[0] = angle & 0x00FF;
        CAN_SET_VALUE[1] = (angle >> 8) & 0x00FF;
    }

    if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0xA5) {
        /* ֹͣ�ͱõ�� */
        CAN_SET_VALUE[4] = 0;
        CAN_SET_VALUE[5] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55) {
        /* �����ͱõ�� */
        CAN_SET_VALUE[4] = 0;
        CAN_SET_VALUE[5] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x14) {
        /* �����ͱõ��ת�� */
        _iq speed = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);
        speed = (speed >> 15);
        if (speed >= 65535) {
            speed = 65535;
        }
        CAN_SET_VALUE[4] = speed & 0x00FF;
        CAN_SET_VALUE[5] = (speed >> 8) & 0x00FF;
    }

    if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55) {
        /* ֹͣ����2 */
        CAN_SET_VALUE[2] = 0;
        CAN_SET_VALUE[3] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA) {
        /* ��������2  */
        CAN_SET_VALUE[2] = 0;
        CAN_SET_VALUE[3] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x13) {
        /* ���õ���2�Ƕ� */
        _iq angle = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);
<<<<<<< HEAD
        angle = _IQmpy(angle, 3200); // 100>>15
        == == == =
                     angle = _IQmpy(angle, 3200); // 100>>15
>>>>>>> refs/remotes/origin/main
        if (angle >= 65535) {
            angle = 65535;
        }
        CAN_SET_VALUE[2] = angle & 0x00FF;
        CAN_SET_VALUE[3] = (angle >> 8) & 0x00FF;
    }

    for (Uint16 i = 0; i < 8; i++) {
        CAN_CHANGE_CTRL[i] = CAN_BUFFER_RX[i];
    }

    Uint16 isValve1 = BORAD_NUM == 0 || BORAD_NUM == 1;
    if (isValve1) {
        if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A) {
            RunAllow_Flag = 0;
        } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5) {
            RunAllow_Flag = 1;
        }
    }

    Uint16 isPump = BORAD_NUM == 2 || BORAD_NUM == 3;
    if (isPump) {
        if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0xA5) {
            RunAllow_Flag = 0;
        } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55) {
            RunAllow_Flag = 1;
        }
    }

    Uint16 isValve2 = BORAD_NUM == 4 || BORAD_NUM == 5;
    if (isValve2) {
        if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55) {
            RunAllow_Flag = 0;
        } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA) {
            RunAllow_Flag = 1;
        }
    }

    if ((CAN_BUFFER_RX[1] & 0x01) == 0x01 && isValve1) {
        if (BORAD_NUM == 0) {
            ET_IO_OUT1_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }

        if (BORAD_NUM == 1) {
            ET_IO_OUT2_L;
            if (BC_SLT_IN == 1) {
                Can_Ctrl(0);
            }
        }
    }

    if ((CAN_BUFFER_RX[1] & 0x01) == 0x00 && isValve1) {
        if (BORAD_NUM == 0) {
            ET_IO_OUT1_H;
            if (BC_SLT_IN == 0) {
                Can_Ctrl(0);
            }
        }

        if (BORAD_NUM == 1) {
            ET_IO_OUT2_H;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }
    }

    /* �ͱõ����CPU�л�����CPU */
    if ((CAN_BUFFER_RX[1] & 0x02) == 0x02 && isPump) {
        if (BORAD_NUM == 2) {
            ET_IO_OUT1_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }

        if (BORAD_NUM == 3) {
            ET_IO_OUT2_L;
            Can_Ctrl(2);
        }
    }

    /* �ͱõ����CPU�л�����CPU */
    if ((CAN_BUFFER_RX[1] & 0x02) == 0x00 && isPump) {
        if (BORAD_NUM == 2) {
            ET_IO_OUT1_L;
            Can_Ctrl(2);
        }

        if (BORAD_NUM == 3) {
            ET_IO_OUT2_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }
    }

    if ((CAN_BUFFER_RX[1] & 0x04) == 0x04 && isValve2) {
        if (BORAD_NUM == 4) {
            ET_IO_OUT1_L;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }

        if (BORAD_NUM == 5) {
            ET_IO_OUT2_L;
            if (BC_SLT_IN == 1)
                Can_Ctrl(4);
        }
    }

    if ((CAN_BUFFER_RX[1] & 0x04) == 0x00 && isValve2) {
        if (BORAD_NUM == 4) {
            ET_IO_OUT1_H;
            if (BC_SLT_IN == 0)
                Can_Ctrl(4);
        }

        if (BORAD_NUM == 5) {
            ET_IO_OUT2_H;
            Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            Ctrl_Flag.bit.CAN_Ctrl = 0;
        }
    }

    Uint16 isMainCpu = (BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0;
    Uint16 isSubCpu = (BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1;
    if (isMainCpu || isSubCpu) {
        Ctrl_Flag.bit.CANERROR_FLAG = 0;
        CAN_Index = 0;
        Can_Tx11_Data();

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            *SCIB.p_Tx_Buffer++ = 0x00EB;          /* ��ͷ1 */
            *SCIB.p_Tx_Buffer++ = 0x0090;          /* ��ͷ2 */
            *SCIB.p_Tx_Buffer++ = 0x0051;          /* ������ */
            *SCIB.p_Tx_Buffer++ = getFrameCount(); /* ������ */
            *SCIB.p_Tx_Buffer++ = 0x0005;          /* ������ */

            /* �����������������յ�ʲô�ش�ʲô */
            for (Uint16 i = 0; i < 5; i++) {
                *SCIB.p_Tx_Buffer++ = (SCI_RX_BUFFER[5 + i] & 0x00FF);
            }

            /* CRC У��*/
            Uint16 crc = CalCRC16_Byte(SCI_TX_BUFFER, 10);
            *SCIB.p_Tx_Buffer++ = (crc & 0x00FF); /* �ۼ���Ч��� */
            *SCIB.p_Tx_Buffer = (crc >> 8);       /* �ۼ���Ч��� */

            SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
            SCIB.m_Tx_Length = 5 + 7;
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
    }
}

void otherResponse() {
    if (SCI_RX_BUFFER[2] == 0) {
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        Board_Num = SCI_ReceData[0];
        if (Board_Num == 1 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (BORAD_NUM == 0) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }

            if (BORAD_NUM == 1) {
                ET_IO_OUT2_L;
            }
        }

        if (Board_Num == 0 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (BORAD_NUM == 0) {
                ET_IO_OUT1_H;
            }

            if (BORAD_NUM == 1) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }
        }

        if (Board_Num == 3 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (BORAD_NUM == 2) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }

            if (BORAD_NUM == 3) {
                ET_IO_OUT2_L;
            }
        }

        if (Board_Num == 2 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (BORAD_NUM == 2) {
                ET_IO_OUT1_H;
            }

            if (BORAD_NUM == 3) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }
        }

        if (Board_Num == 5 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (BORAD_NUM == 4) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }

            if (BORAD_NUM == 5) {
                ET_IO_OUT2_L;
            }
        }

        if (Board_Num == 4 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (BORAD_NUM == 4) {
                ET_IO_OUT1_H;
            }

            if (BORAD_NUM == 5) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }
        }

        DELAY_US(1000);
    }

    if (((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0) ||
        ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1)) {
        if (Board_Num == BORAD_NUM) {
            Cmd_Deal();
        } else {
            Can_Send();
        }
    }
}

void canReceiveHandle() {
    if (RX_BUFFER[2] == 0) {
        CAN_DataCombine_Rx(RX_BUFFER);
        Board_Num = ReceData[0];
        if (Board_Num == 1 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (BORAD_NUM == 0) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }

            if (BORAD_NUM == 1) {
                ET_IO_OUT2_L;
            }
        }

        if (Board_Num == 0 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (BORAD_NUM == 0) {
                ET_IO_OUT1_H;
            }

            if (BORAD_NUM == 1) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }
        }

        if (Board_Num == 3 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (BORAD_NUM == 2) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }

            if (BORAD_NUM == 3) {
                ET_IO_OUT2_L;
            }
        }

        if (Board_Num == 2 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (BORAD_NUM == 2) {
                ET_IO_OUT1_H;
            }

            if (BORAD_NUM == 3) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }
        }

        if (Board_Num == 5 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (BORAD_NUM == 4) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }

            if (BORAD_NUM == 5) {
                ET_IO_OUT2_L;
            }
        }

        if (Board_Num == 4 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (BORAD_NUM == 4) {
                ET_IO_OUT1_H;
            }

            if (BORAD_NUM == 5) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
            }
        }

        DELAY_US(1000);
    }
}

void canTransmitHandle() {
    volatile Uint16 Data_Temp = 0;
    volatile Uint16 ComNum = 0;

    Can_Tx4_Data();
    Can_Tx13_Data();

    if (((BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) && BC_SLT_IN == 0) ||
        ((BORAD_NUM == 1 || BORAD_NUM == 3 || BORAD_NUM == 5) && BC_SLT_IN == 1)) {
        if (SCIB.FLAG.bit.TXBO_FLAG == 1 && SCIB.FLAG.bit.RX_FLAG == 0 && CANB2.FLAG.bit.TXBO_FLAG == 1 &&
            CANB2.FLAG.bit.RX_FLAG == 0 && CANB.FLAG.bit.TXBO_FLAG == 1 && CANB.FLAG.bit.RX_FLAG == 0) {
            _setTelemeterResponseData();
        }
    }
}

void canBufferCtrlNum() {
    Can_Tx4_Data();
    Can_Tx13_Data();
    Ctrl_Flag.bit.CANERROR_FLAG = 0;
    CAN_Index = 0;
    CAN_BUFFER_CTRL[0] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][0];
    CAN_BUFFER_CTRL[1] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][1];
    CAN_BUFFER_CTRL[2] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][2];
    CAN_BUFFER_CTRL[3] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][3];
    CAN_BUFFER_CTRL[4] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][4];
    CAN_BUFFER_CTRL[5] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][5];
    CAN_BUFFER_CTRL[6] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][6];
    CAN_BUFFER_CTRL[7] = CAN_BUFFER_CTRL_N[CAN_BUFFER_CTRL_index][7];
    CAN_BUFFER_CTRL_index++;
    if (CAN_BUFFER_CTRL_index > 5) {
        CAN_BUFFER_CTRL_index = 0;
    }
    CAN_BUFFER_CTRL_Num--;

    // ͨ��CAN���п���
    if (CAN_BUFFER_CTRL[0] == 0xA1) {
<<<<<<< HEAD
        for (Uint16 i = 0; i < 8; i++)
            CAN_CHANGE_CTRL[i] = CAN_BUFFER_CTRL[i];
        == == == =
                     for (Uint16 i = 0; i < 8; i++)
                         CAN_CHANGE_CTRL[i] = CAN_BUFFER_CTRL[i];
>>>>>>> refs/remotes/origin/main
        if ((BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x5A) {
                RunAllow_Flag = 0;
            } else if (CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xA5) {
                RunAllow_Flag = 1;
            }
        }

        if ((BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0xA5) {
                RunAllow_Flag = 0;
            } else if (CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0x55) {
                RunAllow_Flag = 1;
            }
        }

        if ((BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (CAN_BUFFER_CTRL[2] == 0x11 && CAN_BUFFER_CTRL[3] == 0x55) {
                RunAllow_Flag = 0;
            } else if (CAN_BUFFER_CTRL[2] == 0x10 && CAN_BUFFER_CTRL[3] == 0xAA) {
                RunAllow_Flag = 1;
            }
        }

        CAN_CONTROL = CAN_BUFFER_CTRL[1];
        if ((CAN_BUFFER_CTRL[1] & 0x01) == 0x01 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (BORAD_NUM == 0) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.CAN_Ctrl = 0;
            }

            if (BORAD_NUM == 1) {
                ET_IO_OUT2_L;
                if (BC_SLT_IN == 1)
                    Can_Ctrl(1);
            }
        }

        if ((CAN_BUFFER_CTRL[1] & 0x01) == 0x00 && (BORAD_NUM == 0 || BORAD_NUM == 1)) {
            if (BORAD_NUM == 0) {
                ET_IO_OUT1_H;
                if (BC_SLT_IN == 0)
                    Can_Ctrl(1);
            }

            if (BORAD_NUM == 1) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.CAN_Ctrl = 0;
            }
        }

        if ((CAN_BUFFER_CTRL[1] & 0x02) == 0x02 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (BORAD_NUM == 2) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.CAN_Ctrl = 0;
            }

            if (BORAD_NUM == 3) {
                ET_IO_OUT2_L;
                Can_Ctrl(3);
            }
        }

        if ((CAN_BUFFER_CTRL[1] & 0x02) == 0x00 && (BORAD_NUM == 2 || BORAD_NUM == 3)) {
            if (BORAD_NUM == 2) {
                ET_IO_OUT1_L;
                Can_Ctrl(3);
            }

            if (BORAD_NUM == 3) {
                ET_IO_OUT2_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.CAN_Ctrl = 0;
            }
        }

        if ((CAN_BUFFER_CTRL[1] & 0x04) == 0x04 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (BORAD_NUM == 4) {
                ET_IO_OUT1_L;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.CAN_Ctrl = 0;
            }

            if (BORAD_NUM == 5) {
                ET_IO_OUT2_L;
                if (BC_SLT_IN == 1)
                    Can_Ctrl(5);
            }
        }

        if ((CAN_BUFFER_CTRL[1] & 0x04) == 0x00 && (BORAD_NUM == 4 || BORAD_NUM == 5)) {
            if (BORAD_NUM == 4) {
                ET_IO_OUT1_H;
                if (BC_SLT_IN == 0) {
                    Can_Ctrl(5);
                }
            }

            if (BORAD_NUM == 5) {
                ET_IO_OUT2_H;
                Ctrl_Flag.bit.STOP_VELO_FLAG = 1;
                Ctrl_Flag.bit.CAN_Ctrl = 0;
            }
        }
    }
}

_iq Hex_Double(Uint16 ft1, Uint16 ft2, Uint16 ft3, Uint16 ft4) {
    Uint16 ft[4];
    float64 f = 0;
    float32 fout = 0;
    _iq q = 0;
    ft[0] = 0;
    ft[1] = 0;
    ft[2] = (ft2 << 8) + ft1;
    ft[3] = (ft4 << 8) + ft3;
    f = *(float64 *)ft;
    fout = f;
    q = (long)(fout * 32768.0);
    // q = _IQ15(fout);
    return q;
}

_iq Hex_Float(Uint16 ft1, Uint16 ft2, Uint16 ft3, Uint16 ft4) {
    Uint16 ft[2];
    float32 fout = 0;
    _iq q = 0;
    ft[0] = (ft2 << 8) + ft1;
    ft[1] = (ft4 << 8) + ft3;
    fout = *(float32 *)ft;
    q = (long)(fout * 32768.0);
    return q;
}

Uint16 Float_Hex(_iq f_in) {
    Uint16 ft[2];
    float32 f = 0;
    f = _IQ15toF(f_in);
    *(float32 *)ft = f;

    return ft[1];
}

//--------------------------------------------------
// Function instantiation
//--------------------------------------------------
/*
 * @Brief:  CRC funcation
 * @Name:   CalCRC16_Byte
 * @Param:  pbDataBuf[In] , byte data  point
 * @Param:  dwNumOfBytes[In] , byte data  cnt
 * @Return: CRC  rslt
 * @Record: 2023-7-13 Ztp  creat this file
 */
Uint16 CalCRC16_Byte(Uint16 *pbDataBuf, Uint32 dwNumOfBytes) {
    Uint16 bData = 0;
    Uint32 dwLen = dwNumOfBytes;
    Uint16 wCRC = 0x0000;

    while (0 != (dwLen--)) {
        bData = (wCRC >> 8) & 0x00FF;
        wCRC <<= 8;

        wCRC = wCRC ^ g_pusCrc16[bData ^ ((*pbDataBuf) & 0x00FF)];

        pbDataBuf++;
    }

    return wCRC;
}
