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
            // 填写 FIFO数据， 一次填写最多填写12个
            // 最多发送12个字符,16个容易丢字符
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
 * 串口数据解析
 */
void ScibRxCheck(void) {
    volatile Uint16 Fi_Number;
    volatile Uint16 Store_Temp = 1;
    volatile Uint16 Sum_Temp = 0;

    /* 数据接收间断 */
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
     * 至少接收到5个字节才进行数据解析，
     * 数据格式：包头1+包头2+包类型+包计数+数据长度+数据+...+校验位1+校验位2
     */
    Uint16 isNotInSciRx = SCIB.FLAG.bit.RX_FLAG == 0;
    Uint16 isSciTxFinish = SCIB.FLAG.bit.TXBO_FLAG == 1;
    Uint16 isSciRxHaveData = SCIB.p_Rx_AllBuffer > (Uint16 *)(SCI_RX_ALLBUFFER + 4);
    if (isNotInSciRx && isSciTxFinish && isSciRxHaveData) {
        SCIB.FLAG.bit.RXTIME_FLAG = 1; /* 正在进行数据解析 */

        /* 指令类型判断 */
        Uint16 isCustomCmd = SCI_RX_ALLBUFFER[2] == 0x0000;
        Uint16 isSelfCheckCmd = SCI_RX_ALLBUFFER[2] == 0x00A0;
        Uint16 isCtrlCmd = SCI_RX_ALLBUFFER[2] == 0x00A1;
        Uint16 isTelemetryCmd = SCI_RX_ALLBUFFER[2] == 0x00A2;
        if (isCustomCmd) {
            /* 自定义协议数据 */
            SCIB.FLAG.bit.PC_FLAG = 1;
        } else if (isSelfCheckCmd || isCtrlCmd || isTelemetryCmd) {
            /* 协议数据 */
            SCIB.FLAG.bit.PC_FLAG = 0;
        }

        if (SCI_RX_ALLBUFFER[0] != 0x00EB) {
            /* 如果不是包头1，向前移动缓冲区中的数据，为新数据腾出空间 */
            SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
            /* 循环将所有数据向前移动一位 */
            for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
            }
        } else if (SCI_RX_ALLBUFFER[1] != 0x0090) {
            /* 包头2 */
            SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
            for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
            }
        } else if (SCIB.FLAG.bit.PC_FLAG == 0 && !isSelfCheckCmd && !isCtrlCmd && !isTelemetryCmd) {
            /* 未起作用？？？ */
            SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
            for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
            }
        } else {
            /* 数据解析 */
            SCIB.m_Rx_Length = SCI_RX_ALLBUFFER[4];
            if ((SCI_RX_ALLBUFFER[2] == 0x00A0 || SCI_RX_ALLBUFFER[2] == 0x00A2) && (SCIB.m_Rx_Length != 1)) {
                /* 自检指令或遥测指令数据长度不正确，丢弃数据 */
                SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
                for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                    SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                }
            } else if (SCI_RX_ALLBUFFER[2] == 0x00A1 && SCIB.m_Rx_Length != 5) {
                /* 控制指令数据长度不正确，丢弃数据 */
                SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
                for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                    SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                }
            } else if (SCIB.m_Rx_Length > (Uint16)(RX_BUFFER_LENGTH - 0x0007)) {
                /* 数据长度过长，丢弃数据  */
                SCIB.p_Rx_AllBuffer = SCIB.p_Rx_AllBuffer - 1;
                for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                    SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + 1];
                }
            } else if (SCIB.p_Rx_AllBuffer >= (Uint16 *)(SCI_RX_ALLBUFFER + SCIB.m_Rx_Length + 7)) {
                /* 数据长度正确，将数据拷贝到缓冲区中， +7 =
                 * 5（包头1+包头2+包类型+包计数+数据长度） + 2（CRC）？？？ */
                for (Uint16 i = 0; i < SCIB.m_Rx_Length + 7; i++) {
                    SCI_RX_BUFFER[i] = SCI_RX_ALLBUFFER[i];
                }
                Sum_Temp = CalCRC16_Byte(SCI_RX_BUFFER, SCIB.m_Rx_Length + 0x0005);

                if (Sum_Temp == SCI_RX_BUFFER[SCIB.m_Rx_Length + 5] + (SCI_RX_BUFFER[SCIB.m_Rx_Length + 6] << 8)) {
                    /* CRC检验通过，进行数据分发 */
                    SCIB.FLAG.bit.RX_FLAG = 1;

                    /* 数据接收指针左移，并清除已解析数据 */
                    SCIB.p_Rx_AllBuffer = (SCIB.p_Rx_AllBuffer - SCIB.m_Rx_Length - 7);
                    for (Uint16 i = 0; i < (SCIB.p_Rx_AllBuffer - SCI_RX_ALLBUFFER); i++) {
                        SCI_RX_ALLBUFFER[i] = SCI_RX_ALLBUFFER[i + SCIB.m_Rx_Length + 7];
                    }
                } else {
                    /* CRC检验失败，丢弃数据 */
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
 发送数据打包程序
 将需要发送的数据按照通讯规约打包成数据桢
 入口参数:需要发送的数据地址、发送的命令字、发送的数据长度(字)
 出口参数:无
 ----------------------------------------------------------------------------------------*/
void Sci_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L, Uint16 m_tx_length_L) {
    volatile Uint16 i;
    volatile Uint16 Data_Temp;

    *SCIB.p_Tx_Buffer++ = 0x00EB;
    *SCIB.p_Tx_Buffer++ = 0x0090;
    *SCIB.p_Tx_Buffer++ = m_tx_command_L & 0x00FF;

    // 发送的数据字节数加上效验字节
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
    *SCIB.p_Tx_Buffer++ = (Data_Temp & 0x00FF); /* 累加求效验和*/
    *SCIB.p_Tx_Buffer = (Data_Temp >> 8);       /* 累加求效验和*/
    SCIB.p_Tx_Buffer = SCI_TX_BUFFER;
    SCIB.m_Tx_Length = (m_tx_length_L << 2) + 2 + 5;
}

/*--------------------------------------------------------------------------------
 串口发送中断服务子程序
 -------------------------------------------------------------------------------*/
interrupt void ScibTxIsr(void) {
    ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}

/*--------------------------------------------------------------------------------
 串口接收中断服务子程序
 -------------------------------------------------------------------------------*/
interrupt void ScibRxIsr(void) {
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}

/**
 * 将接收到的数据进行组合，每四个字节组合成一个Uint32类型的数据。
 *
 * @param p_Rx 指向接收数据的指针，其中p_Rx[4]指示了需要组合的数据组数。
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
// EPROM的打包程序
// 将需要写入EEPROM的数据按照通讯规约打包成数据桢
// 入口参数:需要写入的数据地址、发送的命令字、发送的数据长度(字)
// 出口参数:无
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
    case 0x0000: /*初始化检测控制器指令*/
        SCI_SendData[0] = Board_Num;
        SCI_SendData[1] = BORAD_NUM;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0070, 2);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0001:                             // 清除故障标志指令
        Sys_Flag.bit.STOP_PWM_Flag_Id = 0;   // "1"表明电机过流保护
        Sys_Flag.bit.STOP_PWM_Flag_Velo = 0; // "1"表明超速保护
        Sys_Flag.bit.UDC_FLAG = 0;           // "1"表明直流电压故障
        Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x0071, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x000D: // LOCK
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
    case 0x0002: // Read Position
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
    case 0x0003: // Openloop
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
    case 0x000E: // Release LOCK
        Choose_Mche = SCI_ReceData[0];
        Ctrl_Flag.bit.LOCK_FLAG = 0;
        Ctrl_Flag.bit.OPEN_LOOP_FLAG = 0;
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007E, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0004:
        /* 获取电机参数 */
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        getMotorParams(SCI_ReceData);

        p = _IQdiv(Mp, Rp); /* 电机与旋变的极对数比 */
        Udc_Mche_realvalue = _IQdiv(_IQ(1), Udc_Setg);
        Velo_PerAdd = _IQmpy(Velo_Add, Per_Ctrl);
        Velo_Duty = _IQdiv(3750000, Control_Period);
        K_Velo_Cal = _IQdiv(_IQ(62.83185307179586476925286766559), V_Base);
        K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0); // 定时器周期为100us
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
        SCI_SendData[9] = V_Base; // 电械速度基值(rad/s)
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
    case 0x0006: // 位置控制指令
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
    case 0x0007: // 速度控制指令
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
                Ctrl_Flag.bit.TORQ_CONTROL_FLAG = 1; // 置转矩控制标志
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
            // ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
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
            // ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
        }
        break;
    case 0x000B:
        Sci_DataCombine_Rx(SCI_RX_BUFFER);
        getControlParams(SCI_ReceData);
        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007B, 0);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
            // ScicRegs.SCITXBUF = *SCIB.p_Tx_Buffer++;
            // ScicRegs.SCIFFTX.bit.TXFFIENA = 1;//使能发送中断
        }
        break;
    case 0x000C:
        setControlParams(SCI_SendData);

        if (SCIB.FLAG.bit.TXBO_FLAG == 1) {
            Sci_DataPackage_Tx(SCI_SendData, 0x007C, 30);
            SCIB.FLAG.bit.TXBO_FLAG = 0;
        }
        break;
    case 0x0010: /* 获取模拟通道参数 */
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
        SCI_SendData[15] = Velo_Start; // 启动速度
        SCI_SendData[16] = maxSpeed;
        SCI_SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
        SCI_SendData[18] = TorquetoIs; // 转矩电流比
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
    case 0x0060: // 写入EEPROM指令
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
    SCI_SendData[9] = V_Base; // 电械速度基值(rad/s)
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
    for (i = 0; i < 127; i++)
        WRITE_X25040(&EEPROM_BUFFER[0 + i], 0 + i, 1, 0);

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
    SCI_SendData[15] = Velo_Start; // 启动速度
    SCI_SendData[16] = maxSpeed;
    SCI_SendData[17] = Ctrl_Flag.bit.VelWay_Flag;
    SCI_SendData[18] = TorquetoIs; // 转矩电流比
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
    for (i = 0; i < 127; i++)
        WRITE_X25040(&EEPROM_BUFFER[0 + i], 128 + i, 1, 8);
    EN_WP = 1;

    EINT;

    WriteEEProm_Flag = 0;
}
