#include "power_on_initialization.h"

#include "..\core\action.h"
#include "..\core\command.h"

void readMotorParams() {
    for (Uint16 i = 0; i < 128; i++) {
        READ_X25040(&SCI_RX_BUFFER[0 + i], 128 + i, 1, 0);
    }

    Sci_DataCombine_Rx(SCI_RX_BUFFER);
    getMotorParams(SCI_ReceData);

    p = _IQdiv(Mp, Rp); /* 电机与旋变的极对数比 */
    Udc_Mche_realvalue = _IQdiv(_IQ(1), Udc_Setg);
    Velo_Duty = _IQdiv(3750000, Control_Period);
    Per_Ctrl = _IQdiv(Control_Period, 37500000);
    Velo_PerAdd = _IQmpy(Velo_Add, Per_Ctrl);
    Is_PerAdd = _IQmpy(Is_Add, Per_Ctrl);
    Is_DownPerAdd = _IQmpy(Is_DownAdd, Per_Ctrl);
    pwma.mHalfPeriod = Control_Period;
}

void readControlParams() {
    for (Uint16 i = 0; i < 128; i++) {
        READ_X25040(&SCI_RX_BUFFER[0 + i], 0 + i, 1, 0);
    }
    Sci_DataCombine_Rx(SCI_RX_BUFFER);
    getControlParams(SCI_ReceData);
}

void readAnalogParams() {
    for (Uint16 i = 0; i < 128; i++) {
        READ_X25040(&SCI_RX_BUFFER[0 + i], 0 + i, 1, 8);
    }

    Sci_DataCombine_Rx(SCI_RX_BUFFER);
    getAnalogParams(SCI_ReceData);
}

void readOtherParams() {
    for (Uint16 i = 0; i < 128; i++) {
        READ_X25040(&SCI_RX_BUFFER[0 + i], 128 + i, 1, 8);
    }

    Sci_DataCombine_Rx(SCI_RX_BUFFER);
    getOtherParams(SCI_ReceData);
    Velo_Duty0 = Velo_Duty;
    K_Velo_Cal = _IQmpy(K_Velo_Cal, Velo_Duty0);
}

void initOtherParams() {
    Sys_Flag.bit.STOP_PWM_Flag_Driv = 0;

    ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 0;
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;  // Clear Overflow flag
    ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;  // Clear Interrupt flag
    ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;  // Clear Interrupt flag
    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;

    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;  // 清所有中断标志位
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanaRegs.CANRMP.bit.RMP0 = 1;
    ECanaRegs.CANRMP.bit.RMP3 = 1;
    ECanaRegs.CANRMP.bit.RMP5 = 1;
    ECanaRegs.CANRMP.bit.RMP6 = 1;
    ECanaRegs.CANRMP.bit.RMP7 = 1;
    ECanaRegs.CANRMP.bit.RMP8 = 1;
    ECanaRegs.CANRMP.bit.RMP9 = 1;
    ECanaRegs.CANRMP.bit.RMP10 = 1;
    ECanaRegs.CANRMP.bit.RMP12 = 1;
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;

    EALLOW;
    EPwm1Regs.ETCLR.bit.INT = 1;  // 清中断标志
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm2Regs.TZCLR.bit.INT = 1;
    EDIS;

    StartCpuTimer0();
    EINT;
}

void clearRxBuffer() {
    for (Uint16 i = 0; i < RX_BUFFER_LENGTH; i++) {
        EEPROM_BUFFER[i] = 0;
        RX_BUFFER[i] = 0;
        SCI_RX_BUFFER[i] = 0;
    }
}

void clearRxAllBuffer() {
    for (Uint16 i = 0; i < RX_ALLBUFFER_LENGTH; i++) {
        SCI_RX_ALLBUFFER[i] = 0;
    }
}

void clearTxBuffer() {
    for (Uint16 i = 0; i < TX_BUFFER_LENGTH; i++) {
        TX_BUFFER[i] = 0;
        SCI_TX_BUFFER[i] = 0;
    }
}

void clearCanBuffer() {
    for (Uint16 i = 0; i < 8; i++) {
        CAN_BUFFER_RX[i] = 0;
        CAN_BUFFER_CTRL[i] = 0;
        CAN_CHANGE_CTRL[i] = 0;
        CAN_SET_VALUE[i] = 0;
    }

    for (Uint16 i = 0; i < 6; i++) {
        for (Uint16 j = 0; i < 17; i++) {
            CAN_BUFFER_RT[i][j] = 0;
        }
    }

    for (Uint16 i = 0; i < 6; i++) {
        for (Uint16 j = 0; i < 8; i++) {
            CAN_BUFFER_CTRL_N[i][j] = 0;
        }
    }

    for (Uint16 i = 0; i < 52; i++) {
        CAN_BUFFER_TX[i] = 0;
    }
}

void clearSciBuffer() {
    for (Uint16 i = 0; i < 30; i++) {
        ReceData[i] = 0;
        SendData[i] = 0;
        SCI_ReceData[i] = 0;
        SCI_SendData[i] = 0;
    }
}

void initParams() {
    /* 对应上位机电机参数界面 */
    readControlParams();

    /* 对应上位机控制参数界面 */
    readMotorParams();

    /* 对应上位机模拟通道参数界面 */
    readAnalogParams();

    /* 对应上位机其他参数界面 */
    readOtherParams();
}

void clearBuffer() {
    clearRxBuffer();
    clearRxAllBuffer();
    clearTxBuffer();
    clearCanBuffer();
    clearSciBuffer();
}

void setInterruptServiceRoutine() {
    /*XINT1 外部中断1*/
    EALLOW;
    PieVectTable.XINT1 = &XINT1_Fault_Isr;
    EDIS;
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
    IER |= M_INT1;

    /*TZ2中断*/
    EALLOW;
    PieVectTable.EPWM2_TZINT = &TZ2_Fault_Isr;
    EDIS;
    PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
    IER |= M_INT2;

    // EV 系统控制周期
    EALLOW;
    PieVectTable.EPWM1_INT = &EPWM1_INT_ISR;
    EDIS;

    IER |= M_INT3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    /*CPUTimer0 系统控制周期中*/
    EALLOW;
    PieVectTable.TINT0 = &Cpu_Timer0_ISR;
    EDIS;

    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    /*ECAN接收中断*/
    EALLOW;
    PieVectTable.ECAN0INTA = &ECana_T;
    EDIS;

    //*********************
    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;  // PIE Group 9, INT9
    IER |= M_INT9;                      // Enable CPU INT

    /*串口中断*/
    EALLOW; /* This is needed to write to EALLOW protected registers  */
    PieVectTable.SCIRXINTC = &ScibRxIsr;
    PieVectTable.SCITXINTC = &ScibTxIsr;
    EDIS;                              /* Enable interrupts required for this example  */
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1; /* PIE Group 8, INT5  */
    PieCtrlRegs.PIEIER8.bit.INTx6 = 1; /* PIE Group 8, INT6 */
    IER |= M_INT8;                     /* Enable CPU INT */
}

void powerOnInitialization() {
    /* 关闭总中断,清除所有CPU中断标志,及关闭CPU级中断*/
    DINT;
    IER = 0x0000;
    IFR = 0x0000;

    /*初始化系统控制，PLL,WatchDog，使能外设时钟*/
    InitSysCtrl();
    if (RUN_MODE == RUN_FLASH) {
        MemCopy(&RamfuncsLoadStart0, &RamfuncsLoadEnd0, &RamfuncsRunStart0);
        MemCopy(&RamfuncsLoadStart1, &RamfuncsLoadEnd1, &RamfuncsRunStart1);
    }

    /* 初始化各外设*/
    InitPeripherals();
    IGBT_PCTRL_L;
    EN_WORK_H;
    OPEN_INT1_L;

    if (BORAD_NUM == 0 || BORAD_NUM == 2 || BORAD_NUM == 4) {
        ET_IO_OUT2_L;
    }

    if (BORAD_NUM == 2) {
        ET_IO_OUT1_L;
    }

    if (BORAD_NUM == 3) {
        ET_IO_OUT2_L;
    }

    /*延时1s*/
    for (Uint16 i = 0; i < 2000; i++) {
        DELAY_US(1000);
    }

    /* 初始化PIE控制寄存器(所有的PIE中断关闭中断标志清除)*/
    InitPieCtrl();

    /* 初始化PIE中断向量表 指向默认的中断服务程序(用于调试)*/
    InitPieVectTable();

    disableResolver();
    initParams();
    clearBuffer();
    readResolver();
    initOtherParams();
}
