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
        Forbid_PWMA(); /* 禁止PWM输出 */

        Ctrl_Flag.bit.STOP_VELO_FLAG = 1;    /* 停止速度控制标志位， 1有效 */
        Sys_Flag.bit.STOP_PWM_Flag_Driv = 1; /* 硬件故障标志位，1有效 */
        Ctrl_Flag.bit.STOP_PWM_Flag = 1;     /* 故障停止标志，1有效*/
    }
}

void writeParamsIntoEeprom() {
    if (WriteEEProm_Flag == 1 && SCIB.FLAG.bit.TXBO_FLAG == 1 && CANB.FLAG.bit.TXBO_FLAG == 1) {
        WriteEEProm();
    }
}

Uint16 getCpuStatus() {
    Uint16 status = 0;

    /* 蝶阀1主控CPU状态 */
    if (CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x02) == 0x00) {
        status = 0x0001;
    }

    /* 蝶阀1副CPU状态 */
    if (CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x02) == 0x00) {
        status += 0x0004;
    }

    /* 蝶阀2主控CPU状态 */
    if (CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x02) == 0x00) {
        status += 0x0010;
    }

    /* 蝶阀2副CPU状态 */
    if (CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x02) == 0x00) {
        status += 0x0040;
    }

    /* 泵电机主控CPU状态 */
    if (CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x02) == 0x00) {
        status += 0x0100;
    }

    /* 泵电机副CPU状态 */
    if (CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x02) == 0x00) {
        status += 0x0400;
    }

    return status;
}

Uint16 getPowerSupplyStatus() {
    Uint16 status = 0;

    /* 28V电源状态1 */
    Uint16 v11 = (CAN_BUFFER_RT[0][2] & 0x20) == 0x00;
    Uint16 v12 = (CAN_BUFFER_RT[0][3] & 0x01) == 0x00;
    Uint16 v13 = (CAN_BUFFER_RT[1][2] & 0x20) == 0x00;
    Uint16 v14 = (CAN_BUFFER_RT[1][3] & 0x01) == 0x00;
    if (v11 && v12 && v13 && v14) {
        status = 0x0001;
    }

    /* 28V电源状态2 */
    Uint16 v21 = (CAN_BUFFER_RT[4][2] & 0x20) == 0x00;
    Uint16 v22 = (CAN_BUFFER_RT[4][3] & 0x01) == 0x00;
    Uint16 v23 = (CAN_BUFFER_RT[5][2] & 0x20) == 0x00;
    Uint16 v24 = (CAN_BUFFER_RT[5][3] & 0x01) == 0x00;
    if (v21 && v22 && v23 && v24) {
        status += 0x0004;
    }

    /* 270V电源状态1 */
    Uint16 v31 = (CAN_BUFFER_RT[2][2] & 0x20) == 0x00;
    Uint16 v32 = (CAN_BUFFER_RT[2][3] & 0x01) == 0x00;
    if (v31 && v32) {
        status += 0x0010;
    }

    /* 270V电源状态2 */
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
            *SCIB.p_Tx_Buffer++ = 0x00EB;          /* 包头1 */
            *SCIB.p_Tx_Buffer++ = 0x0090;          /* 包头2 */
            *SCIB.p_Tx_Buffer++ = 0x0050;          /* 包类型 */
            *SCIB.p_Tx_Buffer++ = getFrameCount(); /* 包计数 */
            *SCIB.p_Tx_Buffer++ = 0x0008;          /* 包长度*/

            /*CPU 状态*/
            Uint16 cpuStatus = getCpuStatus();
            *SCIB.p_Tx_Buffer++ = cpuStatus & 0x00FF;
            *SCIB.p_Tx_Buffer++ = (cpuStatus >> 8);

            /* 蝶阀1状态 */
            *SCIB.p_Tx_Buffer++ = 0x0001;

            /* 蝶阀2状态 */
            *SCIB.p_Tx_Buffer++ = 0x0001;

            /* 供电状态 */
            *SCIB.p_Tx_Buffer++ = getPowerSupplyStatus();

            /* 软件版本: 2.05.A */
            *SCIB.p_Tx_Buffer++ = 0x0005;
            *SCIB.p_Tx_Buffer++ = 0x0041;

            /* 研制单位：林泉电机（FA）*/
            *SCIB.p_Tx_Buffer++ = 0x00FA;

            /* CRC16校验和 */
            Uint16 crc = CalCRC16_Byte(SCI_TX_BUFFER, 13);
            *SCIB.p_Tx_Buffer++ = (crc & 0x00FF); /* 累加求效验和*/
            *SCIB.p_Tx_Buffer = (crc >> 8);       /* 累加求效验和*/

            SCIB.p_Tx_Buffer = SCI_TX_BUFFER; /* ? */
            SCIB.m_Tx_Length = 15;            /* 自检响应帧15个字节（包括CRC）*/
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
    }
}

Uint16 getDriverStatus() {
    Uint16 status = 0;

    /* 蝶阀1主驱动状态 */
    if (CAN_BUFFER_RT[0][8] == 0 && (CAN_BUFFER_RT[0][2] & 0x01) == 0x00) {
        status = 0x0001;
    }

    /* 蝶阀1副驱动状态 */
    if (CAN_BUFFER_RT[1][8] == 0 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x00) {
        status += 0x0002;
    }

    /* 蝶阀2主驱动状态 */
    if (CAN_BUFFER_RT[4][8] == 0 && (CAN_BUFFER_RT[4][2] & 0x01) == 0x00) {
        status += 0x0004;
    }

    /* 蝶阀2副驱动状态 */
    if (CAN_BUFFER_RT[5][8] == 0 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x00) {
        status += 0x0008;
    }

    /* 泵电机主驱动状态 */
    if (CAN_BUFFER_RT[2][8] == 0 && (CAN_BUFFER_RT[2][2] & 0x01) == 0x00) {
        status += 0x0010;
    }

    /* 泵电机副驱动状态 */
    if (CAN_BUFFER_RT[3][8] == 0 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x00) {
        status += 0x0020;
    }

    return status;
}

void _getValve1Params(Uint16 index) {
    /* 蝶阀1 实际角度 */
    CAN_BUFFER_TX[2] = CAN_BUFFER_RT[index][0];
    CAN_BUFFER_TX[3] = CAN_BUFFER_RT[index][1];

    /* 蝶阀1 CPU 温度 */
    CAN_BUFFER_TX[6 + 6] = CAN_BUFFER_RT[index][4];
    CAN_BUFFER_TX[7 + 6] = CAN_BUFFER_RT[index][5];

    /* 蝶阀1 母线电压 */
    CAN_BUFFER_TX[12 + 6] = CAN_BUFFER_RT[index][6];
    CAN_BUFFER_TX[13 + 6] = CAN_BUFFER_RT[index][7];

    /* 蝶阀1 母线电流*/
    CAN_BUFFER_TX[18 + 6] = CAN_BUFFER_RT[index][9];
    CAN_BUFFER_TX[19 + 6] = CAN_BUFFER_RT[index][10];

    /* 蝶阀1 A相电流 */
    CAN_BUFFER_TX[24 + 6] = CAN_BUFFER_RT[index][11];
    CAN_BUFFER_TX[25 + 6] = CAN_BUFFER_RT[index][12];

    /* 蝶阀1 B相电流 */
    CAN_BUFFER_TX[26 + 6] = CAN_BUFFER_RT[index][13];
    CAN_BUFFER_TX[27 + 6] = CAN_BUFFER_RT[index][14];

    /* 蝶阀1 C相电流 */
    CAN_BUFFER_TX[28 + 6] = CAN_BUFFER_RT[index][15];
    CAN_BUFFER_TX[29 + 6] = CAN_BUFFER_RT[index][16];

    /* 蝶阀1 故障字 */
    CAN_BUFFER_TX[42 + 6] = 0;
    CAN_BUFFER_TX[43 + 6] = 0;
    CAN_BUFFER_TX[44 + 6] = 0;

    /* 蝶阀1 28V过压 */
    if ((CAN_BUFFER_RT[index][2] & 0x20) == 0x00) {
        CAN_BUFFER_TX[42 + 6] = 0x0001;
    }

    /* 蝶阀1 28V欠压 */
    if ((CAN_BUFFER_RT[index][3] & 0x01) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0008;
    }

    /* 蝶阀1 28V过流 */
    if ((CAN_BUFFER_RT[index][2] & 0x04) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0040;
    }

    /* 蝶阀1 硬件保护 */
    if ((CAN_BUFFER_RT[index][2] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0002;
    }

    /* 蝶阀1 软件限位 */
    if ((CAN_BUFFER_RT[index][3] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0010;
    }

    /* 蝶阀1 主旋变超限 */
    if ((CAN_BUFFER_RT[index][3] & 0x04) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0040;
    }

    /* 蝶阀1 副旋变超限 */
    if ((CAN_BUFFER_RT[index][3] & 0x08) == 0x00) {
        CAN_BUFFER_TX[44 + 6] += 0x0001;
    }

    /* 蝶阀1 机组旋变超限 */
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
    /* 蝶阀2实际角度 */
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

    /* 蝶阀2旋变超限 */
    if ((CAN_BUFFER_RT[index][3] & 0x04) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0080;
    }

    /* 机组旋变故障 */
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
    /* 泵电机 实际转速 */
    CAN_BUFFER_TX[10] = CAN_BUFFER_RT[index][0];
    CAN_BUFFER_TX[11] = CAN_BUFFER_RT[index][1];

    /* 泵电机 CPU温度 */
    CAN_BUFFER_TX[10 + 6] = CAN_BUFFER_RT[index][4];
    CAN_BUFFER_TX[11 + 6] = CAN_BUFFER_RT[index][5];

    /* 泵电机 母线电压 */
    CAN_BUFFER_TX[16 + 6] = CAN_BUFFER_RT[index][6];
    CAN_BUFFER_TX[17 + 6] = CAN_BUFFER_RT[index][7];

    /* 泵电机 母线电流 */
    CAN_BUFFER_TX[22 + 6] = CAN_BUFFER_RT[index][9];
    CAN_BUFFER_TX[23 + 6] = CAN_BUFFER_RT[index][10];

    /* 泵电机 A相电流 */
    CAN_BUFFER_TX[36 + 6] = CAN_BUFFER_RT[index][11];
    CAN_BUFFER_TX[37 + 6] = CAN_BUFFER_RT[index][12];

    /* 泵电机 B相电流 */
    CAN_BUFFER_TX[38 + 6] = CAN_BUFFER_RT[index][13];
    CAN_BUFFER_TX[39 + 6] = CAN_BUFFER_RT[index][14];

    /* 泵电机 C相电流 */
    CAN_BUFFER_TX[40 + 6] = CAN_BUFFER_RT[index][15];
    CAN_BUFFER_TX[41 + 6] = CAN_BUFFER_RT[index][16];

    /* 泵电机 母线过压 */
    if ((CAN_BUFFER_RT[index][2] & 0x20) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0004;
    }

    /* 泵电机 母线欠压 */
    if ((CAN_BUFFER_RT[index][3] & 0x01) == 0x00) {
        CAN_BUFFER_TX[42 + 6] += 0x0020;
    }

    /* 泵电机 母线过流 */
    if ((CAN_BUFFER_RT[index][2] & 0x04) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0001;
    }

    /* 泵电机 硬件保护 */
    if ((CAN_BUFFER_RT[index][2] & 0x10) == 0x00) {
        CAN_BUFFER_TX[43 + 6] += 0x0008;
    }

    /* 泵电机 主旋变故障 */
    if ((CAN_BUFFER_RT[index][3] & 0x04) == 0x00) {
        CAN_BUFFER_TX[44 + 6] += 0x0002;
    }

    /* 泵电机 副旋变故障 */
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
    *SCIB.p_Tx_Buffer++ = 0x00EB;          /* 包头1 */
    *SCIB.p_Tx_Buffer++ = 0x0090;          /* 包头2 */
    *SCIB.p_Tx_Buffer++ = 0x0052;          /* 帧类型 */
    *SCIB.p_Tx_Buffer++ = getFrameCount(); /* 包计数 */
    *SCIB.p_Tx_Buffer++ = 0x0036;          /* 包长度 */

    /* 驱动状态位 */
    Uint16 driverStatus = getDriverStatus();
    *SCIB.p_Tx_Buffer++ = driverStatus & 0x00FF;
    *SCIB.p_Tx_Buffer++ = (driverStatus >> 8);

    CAN_CONTROL = 0x07;
    // 先选择从板,如果主板无故障，且从板不在运行中，则选择主板运行
    if ((CAN_BUFFER_RT[0][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[1][2] & 0x01) == 0x01) {
        CAN_CONTROL &= 0xFE;
    }

    if ((CAN_BUFFER_RT[2][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[3][2] & 0x01) == 0x01) {
        CAN_CONTROL &= 0xFD;
    }

    if ((CAN_BUFFER_RT[4][2] & 0x02) == 0x00 && (CAN_BUFFER_RT[5][2] & 0x01) == 0x01) {
        CAN_CONTROL &= 0xFB;
    }

    /* 蝶阀1设定角度 */
    CAN_BUFFER_TX[0] = CAN_SET_VALUE[0];
    CAN_BUFFER_TX[1] = CAN_SET_VALUE[1];

    /* 蝶阀2设定角度*/
    CAN_BUFFER_TX[4] = CAN_SET_VALUE[2];
    CAN_BUFFER_TX[5] = CAN_SET_VALUE[3];

    /* 泵电机设定转速 */
    CAN_BUFFER_TX[8] = CAN_SET_VALUE[4];
    CAN_BUFFER_TX[9] = CAN_SET_VALUE[5];

    /* 获取蝶阀1运行参数 */
    getValve1RunningParams();

    /* 获取蝶阀2运行参数 */
    getValve2RunningParams();

    /* 获取泵电机运行参数 */
    getPumpRunningParams();

    CAN_BUFFER_TX[51] = 0;
    for (Uint16 i = 0; i < 52; i++) {
        *SCIB.p_Tx_Buffer++ = CAN_BUFFER_TX[i];
    }

    /* CRC 校验 */
    Uint16 crc = CalCRC16_Byte(SCI_TX_BUFFER, 53 + 6);
    *SCIB.p_Tx_Buffer++ = (crc & 0x00FF); /* 累加求效验和*/
    *SCIB.p_Tx_Buffer = (crc >> 8);       /* 累加求效验和*/

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

    /* CAN通讯最初8个字节数据 */
    for (Uint16 i = 0; i < 8; i++) {
        CAN_BUFFER_RX[i] = 0;
    }
    CAN_BUFFER_RX[0] = SCI_RX_BUFFER[2]; /* 指令类型 */

<<<<<<< HEAD
    CAN_BUFFER_RX[1] = 0x07;             // 先选择从板
    == == == =
                 CAN_BUFFER_RX[1] = 0x07; // 先选择从板
>>>>>>> refs/remotes/origin/main
    // 如果主板无故障，且从板不在运行中，则选择主板运行
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
                 CAN_CONTROL = CAN_BUFFER_RX[1]; // 板卡选择
>>>>>>> refs/remotes/origin/main
    for (Uint16 i = 2; i < 7; i++) {
        CAN_BUFFER_RX[i] = SCI_RX_BUFFER[3 + i]; /* 命令码+命令参数 */
    }

    if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x5A) {
        /* 停止蝶阀1 */
        CAN_SET_VALUE[0] = 0;
        CAN_SET_VALUE[1] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xA5) {
        /* 启动蝶阀1 */
        CAN_SET_VALUE[0] = 0;
        CAN_SET_VALUE[1] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x12) {
        /* 设置蝶阀1角度  */
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
        /* 停止油泵电机 */
        CAN_SET_VALUE[4] = 0;
        CAN_SET_VALUE[5] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0x55) {
        /* 启动油泵电机 */
        CAN_SET_VALUE[4] = 0;
        CAN_SET_VALUE[5] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x14) {
        /* 设置油泵电机转速 */
        _iq speed = Hex_Float(CAN_BUFFER_RX[3], CAN_BUFFER_RX[4], CAN_BUFFER_RX[5], CAN_BUFFER_RX[6]);
        speed = (speed >> 15);
        if (speed >= 65535) {
            speed = 65535;
        }
        CAN_SET_VALUE[4] = speed & 0x00FF;
        CAN_SET_VALUE[5] = (speed >> 8) & 0x00FF;
    }

    if (CAN_BUFFER_RX[2] == 0x11 && CAN_BUFFER_RX[3] == 0x55) {
        /* 停止蝶阀2 */
        CAN_SET_VALUE[2] = 0;
        CAN_SET_VALUE[3] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x10 && CAN_BUFFER_RX[3] == 0xAA) {
        /* 启动蝶阀2  */
        CAN_SET_VALUE[2] = 0;
        CAN_SET_VALUE[3] = 0;
    } else if (CAN_BUFFER_RX[2] == 0x13) {
        /* 设置蝶阀2角度 */
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

    /* 油泵电机主CPU切换至副CPU */
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

    /* 油泵电机副CPU切换至主CPU */
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
            *SCIB.p_Tx_Buffer++ = 0x00EB;          /* 包头1 */
            *SCIB.p_Tx_Buffer++ = 0x0090;          /* 包头2 */
            *SCIB.p_Tx_Buffer++ = 0x0051;          /* 包类型 */
            *SCIB.p_Tx_Buffer++ = getFrameCount(); /* 包计数 */
            *SCIB.p_Tx_Buffer++ = 0x0005;          /* 包长度 */

            /* 命令码和命令参数，收到什么回传什么 */
            for (Uint16 i = 0; i < 5; i++) {
                *SCIB.p_Tx_Buffer++ = (SCI_RX_BUFFER[5 + i] & 0x00FF);
            }

            /* CRC 校验*/
            Uint16 crc = CalCRC16_Byte(SCI_TX_BUFFER, 10);
            *SCIB.p_Tx_Buffer++ = (crc & 0x00FF); /* 累加求效验和 */
            *SCIB.p_Tx_Buffer = (crc >> 8);       /* 累加求效验和 */

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

    // 通过CAN进行控制
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
