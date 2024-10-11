#ifndef APP_USERDEF_H
#define APP_USERDEF_H

//------------------------------------------------------------------------------
#include "App_Marco.h"
#include "App_TypeDef.h"
//------------------------------------------------------------------------------

extern void ScibRxCheck(void);
extern void ScibTxCheck(void);
extern void WriteEEProm(void);
extern volatile Uint16 WriteEEProm_Flag;
extern volatile Uint16 RunAllow_Flag;
//==============================================================================
// 外部函数声明
//==============================================================================
extern Uint16 g_pusCrc16[256];

//--------------------------------------------------------------
extern void Control(void);
extern void Ctrl_Change(void);

extern _iq RCFilter(volatile _iq filtout, volatile _iq filtin,
                    volatile _iq Freq_cut);

//-----------------------------------------------------------
extern _iq _IQasinPU(_iq PA);
extern _iq _IQasin4PU(_iq real, _iq imag);
extern _iq AngleAdjust(_iq angle);
extern _iq17 Adc_Arith(volatile Uint16 AD_digital_value);
extern _iq Adc_Temp(volatile Uint16 AD_digital_value1);

//------------------------------------------------------------
extern void SVPWMA_Gen(_iq Alfa, _iq Beta);
extern void PWMA_Update(void);
extern void Forbid_PWMA(void);

//-------------------------------------------------------------
interrupt void Cpu_Timer0_ISR(void);
interrupt void EPWM1_INT_ISR(void);

//--------------------------------------------------------------
extern void CAN_DataCombine_Rx(Uint16 *p_Rx);
extern void CAN2_DataCombine_Rx(Uint16 *p_Rx);

extern Uint16 RDSR_X25040(void);
extern void WREN_X25040(void);
extern void WRSR_X25040(Uint16 COM);
extern void WRITE_X25040(Uint16 *RA_ADDR, Uint16 EEP_ADDR, Uint16 N, Uint16 H);
extern void READ_X25040(Uint16 *RA_ADDR, Uint16 EEP_ADDR, Uint16 N, Uint16 H);

extern void Init_AD1210(void);
extern void RDC_READ(void);
extern void Can_Tx_Data();
extern void Can_Tx2_Data();
extern void Can_Tx4_Data();
extern void Can_Tx11_Data();
extern void Can_Tx13_Data();
extern void Can_Ctrl(Uint16 Board);
extern Uint16 CalCRC16_Byte(Uint16 *pbDataBuf, Uint32 dwNumOfBytes);
extern Uint16 Float_Hex(_iq f_in);
extern _iq Hex_Double(Uint16 ft1, Uint16 ft2, Uint16 ft3, Uint16 ft4);
extern _iq Hex_Float(Uint16 ft1, Uint16 ft2, Uint16 ft3, Uint16 ft4);
//===================================================================
// 中断服务函数声明
//===================================================================
interrupt void TZ2_Fault_Isr(void);
interrupt void XINT1_Fault_Isr(void);
//===================================================================
//======================================================================================================
// 外部函数声明结束

extern void Sci_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L,
                               Uint16 m_tx_length_L);
extern void Sci_DataCombine_Rx(Uint16 *p_Rx);
interrupt void ScibTxIsr(void);
interrupt void ScibRxIsr(void);
extern SCIB_REGS SCIB;
extern Uint16 SCI_RX_BUFFER[RX_BUFFER_LENGTH];       // 串口接收缓冲区
extern Uint16 SCI_RX_ALLBUFFER[RX_ALLBUFFER_LENGTH]; // 串口接收缓冲区
extern Uint16 SCI_TX_BUFFER[TX_BUFFER_LENGTH];       // 串口发送缓冲区
extern _iq SCI_ReceData[30];                         // 串口接收的数据
extern _iq SCI_SendData[30];                         // 串口发送的数据
extern volatile Uint32 Time_Cal;

//======================================================================================================

extern _iq30 *pArcSin; // 指向反正弦函数表，Q30

extern Uint16 EEPROM_BUFFER[RX_BUFFER_LENGTH]; // EEPROM数据缓冲区
extern volatile Uint16 *p_EEProm_Buffer;
extern Uint16 RX_BUFFER[RX_BUFFER_LENGTH]; // 串口接收缓冲区
extern Uint16 TX_BUFFER[TX_BUFFER_LENGTH]; // 串口发送缓冲区
extern _iq ReceData[30];                   // 串口接收的数据
extern _iq SendData[30];                   // 串口发送的数据

extern Uint16 CAN_BUFFER_RX[8];     // CAN缓冲区
extern Uint16 CAN_BUFFER_RT[6][17]; // CAN缓冲区
extern Uint16 CAN_BUFFER_TX[52];    // CAN缓冲区

extern Uint16 CAN_BUFFER_CTRL[8]; // CAN缓冲区
extern Uint16 CAN_BUFFER_CTRL_N[6][8];
extern Uint16 CAN_BUFFER_CTRL_N_index;
extern Uint16 CAN_BUFFER_CTRL_index;
extern Uint16 CAN_BUFFER_CTRL_Num;
extern Uint16 CAN_CHANGE_CTRL[8]; // CAN缓冲区
extern Uint16 CAN_SET_VALUE[8];   // CAN缓冲区
extern Uint16 CAN_CONTROL;        // 控制状态

extern volatile struct ECAN_REGS ECanaShadow;

extern interrupt void ECana_T(void);
extern void Can_Deal(void);
extern void Cmd_Deal(void);
extern void Can_Receive(void);
extern void Can_Send(void);

extern CAN_REGS CANB;
extern CAN_REGS CANB2;
extern volatile Uint16 CAN_Index;
extern volatile Uint16 CanStop_Time;
extern volatile Uint16 Choose_Mche; // 选择电机
extern volatile Uint16 Choose_Mode; // 选择模式

extern volatile Uint16 Board_Num; // 控制板

extern volatile Uint16 Board2_Num; // 控制板

extern RDC_DRIVER rdc1;
extern RDC_DRIVER rdc2;

extern PWMGEN pwma;

extern Ref_ABCtoDQ abctodq; // ABC->DQ
extern Ref_DQtoABC dqtoabc; // DQ->ABC
extern Ref_THREE2TWO phase_trans;

extern PIREG pi_Pos;   // 位置环的PI
extern PIREG pi_velo;  // 速度PI
extern PIREG pi_Id_Kc; // IdPI
extern PIREG pi_Iq_Kc; // IqPI
extern PIREG pi_usLimit;
extern volatile _iq Idq_Kc;
extern volatile _iq Radq;

extern volatile Uint32 VeloProtect1;    // QG 保护用Iq标子
extern volatile Uint32 IdProtect1;      // QG 保护用Id标子
extern volatile Uint32 UdcOverProtect1; // QG 保护用Id标子
extern volatile Uint32 IdcOverProtect1; // QG 保护用Id标子
extern volatile _iq Is;                 // QG Id标幺值
extern volatile _iq maxSpeed;

extern _iq m_PT_V[31];
extern _iq m_PT_T[31];

extern volatile Uint16 Control_Period; // Q16 控制周期计数值
extern volatile Uint16 Deadtime;       // Q16 死区计数值
extern volatile _iq Velo_Duty;         // QG 转速载波系数
extern volatile _iq Velo_Duty0;        // QG 转速载波系数
extern volatile _iq MotorTM;
extern volatile _iq igbtTemperatureProtectionValue;

//======================================================================================================
/*  主要相关全局变量的定义		*/
//======================================================================================================
//------------------------------------------------------
// 频率、速度以及角度
extern volatile Uint16 Angle_Q16;  // Q16 旋变测取的当前位置角HEX
extern volatile Uint16 Angle0_Q16; // Q16 旋变测取的上一次位置角HEX
extern volatile Uint16 Angle_Init_Digital; // Q0 用于旋变测量的转子初位置角(HEX)
extern volatile Uint16 Angle_Init_Fa; // Q0 用于旋变测量的转子初位置角(HEX)
extern volatile Uint16 Angle_Fa_Q16; // Q16 旋变测取的当前位置角HEX

extern volatile _iq Angle_ElecOpen; // QG 转子位置电角的标幺值
extern volatile _iq Angle_Elec;     // QG 转子位置电角的标幺值
extern volatile _iq Com_angle_I;    // 计算dq轴电流时的补偿角
extern volatile _iq Com_angle_U;    // 计算dq轴电压时的补偿角

extern volatile _iq Velo_Elec;     // QG 检测得到的转子速度的实际值
extern volatile _iq Velo_Elec_Cal; // QG 检测得到的转子速度的计算值
extern volatile _iq Velo_Set;      // QG 给定的转子速度的角频率表示
extern volatile _iq Pos_Set;       // QG 给定的位置

extern volatile Uint16 Run_Led_Index; // 运行灯运行的控浦芷诘闹甘?

//----------------------------------------------------------
// 用于存储直接ADC采样的到数字量,需要进行校正
extern volatile Uint16 ADc_Isa;
extern volatile Uint16 ADc_Isb;
extern volatile Uint16 ADc_Isc;
extern volatile Uint16 ADc_Idc;
extern volatile Uint16 ADc_Udc;
extern volatile Uint16 ADc_Temp0;
extern volatile Uint16 ADc_Temp2;
extern volatile Uint16 ADc_Temp4;
//------------------------------------------------------------

// 电机定子电压缌鞯谋赙?
extern volatile _iq Isa;      // QG Isa的标幺?
extern volatile _iq Isb;      // QG Isb的标幺值
extern volatile _iq Isc;      // QG Isc的标幺值
extern volatile _iq Usa;      // QG Usa标幺值
extern volatile _iq Usb;      // QG Usb的壑?
extern volatile _iq Usc;      // QG Usc的标幺
extern volatile _iq Udc_Mche; // QG 直流侧电压的倒上位机给的）
extern volatile _iq Udc_Mche_realvalue; /// QG 直流侧电压的实际值（上位机给的）

extern volatile _iq Udc_Mche_1; // QG 直流侧电压的测值
extern volatile _iq Udc_Mche0;  // QG 直流侧电压的测?

extern volatile _iq Idc;  // QG Idc的标幺值
extern volatile _iq Idc0; // QG Idc的标幺值

extern volatile _iq Id;     // QG Id标幺值
extern volatile _iq Iq;     // QG Iq标幺值
extern volatile _iq Id_Set; // QG Id设定的标幺值
extern volatile _iq Iq_Set; // QG Iq设定的标幺值

extern volatile _iq Ud;           // QG 控制得出的Ud
extern volatile _iq Uq;           // QG 控制得出的Uq
extern volatile _iq Us_Max;       // QG Ud，Uq的限? k*0.612Udc (恒功率浠?
extern volatile _iq Us_Alfa_Mche; // QG a轴电压的标幺值
extern volatile _iq Us_Beta_Mche; // QG b轴电压的标幺值

//------------------------------------------------------------
extern volatile Uint16 IdProtect;         // QG 保护用Id标子
extern volatile Uint16 VeloProtect;       // QG 保护用Iq标子
extern volatile Uint16 IdProtectNum;      // QG 保护用Id标子
extern volatile Uint16 VeloProtectNum;    // QG 保护用Iq标子
extern volatile Uint16 UdcLowProtect;     // QG 保护用Id标子
extern volatile Uint32 UdcLowEnable;      // QG 保护用Id标子
extern volatile Uint16 UdcOverProtect;    // QG 保护用Id标子
extern volatile Uint16 IdcOverProtect;    // QG 保护用Id标子
extern volatile Uint16 Temp1OverProtect;  // QG 保护用Id标子
extern volatile Uint16 Temp2OverProtect;  // QG 保护用Id标子
extern volatile Uint32 Temp1OverProtect1; // QG 保护用Id标子
extern volatile Uint32 Temp2OverProtect1; // QG 保护用Id标子
extern volatile Uint32 IGBTProtect1;      // QG 保护用Id标子
extern volatile Uint16 RdcProtect;        // QG 保护用Id标子
extern volatile Uint32 RdcProtect1;       // QG 保护用Id标子
extern volatile Uint16 Rdc2Protect;       // QG 保护用Id标子
extern volatile Uint32 Rdc2Protect1;      // QG 保护用Id标子

extern volatile Uint32 ProtectNum; // QG 故障恢复时间

//------------------------------------------------------------
// 用于滤波程序的量
extern volatile _iq Id0;        // QG 上次的Id标幺值
extern volatile _iq Iq0;        // QG 上次的Iq标幺值
extern volatile _iq Velo_Elec0; // QG 上次计算的电速度的标幺值

//------------------------------------------------------------
//----------------------------------------

// 电机参数
extern volatile _iq Ld;         // 电机d轴电感的标幺值
extern volatile _iq Lq;         // 电机q轴电感的标幺值
extern volatile _iq PHI;        // 电机磁链的标幺值
extern volatile _iq Rs;         // 电机电阻的标幺值
extern volatile _iq Mp;         // 电机的极对数
extern volatile _iq Rp;         // 电机的极对数
extern volatile _iq p;          // 电机与旋变的极对数比
                                // 基值系统
extern volatile _iq U_Base;     // 电压基值
extern volatile _iq I_Base;     // 电流基值
extern volatile _iq V_Base;     // 速度基值
extern volatile _iq K_Velo_Cal; // 速度计算参数

// 主要相关全局变量的定义结束
//======================================================================================================
//----------------------------------------------------------

//----------------------------------------------------------
extern volatile union FAULT_FLAG_REG Fault_Flag;
extern volatile union SYS_FLAG_REG Sys_Flag;
extern volatile union CTRL_FLAG_REG Ctrl_Flag;
//====================================================================================

/*id跟随变量定义*/
extern volatile _iq id_Max;
extern volatile _iq idc_Max;
extern volatile _iq idSet_Max;

/*滤波截止频率变量定义*/
extern volatile _iq id_Filter;
extern volatile _iq Velo_Filter;
extern volatile _iq Udc_Filter;
//------------------------------------------------------------------------------------

extern volatile _iq Velo_Max;

extern volatile _iq Usa_Lock;
extern volatile _iq Usb_Lock;
extern volatile _iq Usc_Lock;
extern volatile _iq Udc_Lock;
extern volatile _iq Um_OpenLoop;
extern volatile _iq Freq_OpenLoop;
extern volatile _iq Udc_OpenLoop;

extern volatile _iq17 Off_Isc;
extern volatile _iq17 K_Isc;
extern volatile _iq17 Off_Isb;
extern volatile _iq17 K_Isb;
extern volatile _iq17 Off_Isa;
extern volatile _iq17 K_Isa;
extern volatile _iq17 Off_Udc;
extern volatile _iq17 K_Udc;
extern volatile _iq17 Off_Idc;
extern volatile _iq17 K_Idc;

extern volatile _iq Us;       // QG Us=mag(ud,uq)的标幺值
extern volatile _iq Udc_Setg; // QG 直流侧电压的测量值
extern volatile _iq I_Add;
extern volatile _iq Velo_Start;

extern volatile _iq Isdq_Set;
extern volatile _iq Isd_Set; // 上位机发送过来的id;
extern volatile _iq Isdq_Set1;
extern volatile _iq Isdq_Set2;

extern volatile _iq IstoTorque;
extern volatile _iq TorquetoIs;
extern volatile _iq TorqueAngle;  // 30度
extern volatile _iq TorqueAngleA; // 0度
extern volatile _iq Is_Add;
extern volatile _iq Is_PerAdd;
extern volatile _iq Veloover_Set;
extern volatile _iq Velo_Add;
extern volatile _iq Velo_PerAdd;
extern volatile _iq Per_Ctrl; // 控制周期

extern volatile _iq PWMA_a;        // QG 控制得出的Ud
extern volatile _iq PWMA_b;        // QG 控制得出的Uq
extern volatile _iq PWMA_c;        // QG 控制得出的Uq
extern volatile _iq Velo_Elec_abs; // QG 电速度谋赙壑

extern volatile _iq Temperature1;     //
extern volatile _iq Temperature3;     //
extern volatile _iq Temperature5;     //
extern volatile _iq Temperature10;    //
extern volatile _iq Temperature30;    //
extern volatile _iq Temperature50;    //
extern volatile _iq Temperature1_Off; //
extern volatile _iq Temperature3_Off; //
extern volatile _iq Temperature5_Off; //
extern volatile _iq Temperature1_K;   //
extern volatile _iq Temperature3_K;   //
extern volatile _iq Temperature5_K;   //

extern volatile long Velo_Calc_Num;    // 速度平均次数
extern volatile Uint16 Velo_Avg_Index; // 速度平均指数
extern volatile _iq Velotmp;
extern volatile Uint16 Velo_Calc_flag;
extern volatile _iq UdcLimit_Set;  // QG 直流侧电压的测值
extern volatile _iq UdcLimit1_Set; // QG 直流侧电压的测值
extern volatile _iq IdSet;
extern volatile _iq UsSet;
extern volatile _iq IdZ_Set; // QG Id设定的标幺值
extern volatile _iq IqZ_Set; // QG Iq设定的标幺值

extern volatile _iq Is_DownAdd;
extern volatile _iq Is_DownPerAdd;
extern volatile _iq Temp_Filter;
extern volatile _iq IsSet_Filter;
extern volatile _iq IsLimit_Set;
extern volatile _iq Us_M;

#endif
