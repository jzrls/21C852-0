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
// �ⲿ��������
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
// �жϷ���������
//===================================================================
interrupt void TZ2_Fault_Isr(void);
interrupt void XINT1_Fault_Isr(void);
//===================================================================
//======================================================================================================
// �ⲿ������������

extern void Sci_DataPackage_Tx(_iq *p_tx_data_L, Uint16 m_tx_command_L,
                               Uint16 m_tx_length_L);
extern void Sci_DataCombine_Rx(Uint16 *p_Rx);
interrupt void ScibTxIsr(void);
interrupt void ScibRxIsr(void);
extern SCIB_REGS SCIB;
extern Uint16 SCI_RX_BUFFER[RX_BUFFER_LENGTH];       // ���ڽ��ջ�����
extern Uint16 SCI_RX_ALLBUFFER[RX_ALLBUFFER_LENGTH]; // ���ڽ��ջ�����
extern Uint16 SCI_TX_BUFFER[TX_BUFFER_LENGTH];       // ���ڷ��ͻ�����
extern _iq SCI_ReceData[30];                         // ���ڽ��յ�����
extern _iq SCI_SendData[30];                         // ���ڷ��͵�����
extern volatile Uint32 Time_Cal;

//======================================================================================================

extern _iq30 *pArcSin; // ָ�����Һ�����Q30

extern Uint16 EEPROM_BUFFER[RX_BUFFER_LENGTH]; // EEPROM���ݻ�����
extern volatile Uint16 *p_EEProm_Buffer;
extern Uint16 RX_BUFFER[RX_BUFFER_LENGTH]; // ���ڽ��ջ�����
extern Uint16 TX_BUFFER[TX_BUFFER_LENGTH]; // ���ڷ��ͻ�����
extern _iq ReceData[30];                   // ���ڽ��յ�����
extern _iq SendData[30];                   // ���ڷ��͵�����

extern Uint16 CAN_BUFFER_RX[8];     // CAN������
extern Uint16 CAN_BUFFER_RT[6][17]; // CAN������
extern Uint16 CAN_BUFFER_TX[52];    // CAN������

extern Uint16 CAN_BUFFER_CTRL[8]; // CAN������
extern Uint16 CAN_BUFFER_CTRL_N[6][8];
extern Uint16 CAN_BUFFER_CTRL_N_index;
extern Uint16 CAN_BUFFER_CTRL_index;
extern Uint16 CAN_BUFFER_CTRL_Num;
extern Uint16 CAN_CHANGE_CTRL[8]; // CAN������
extern Uint16 CAN_SET_VALUE[8];   // CAN������
extern Uint16 CAN_CONTROL;        // ����״̬

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
extern volatile Uint16 Choose_Mche; // ѡ����
extern volatile Uint16 Choose_Mode; // ѡ��ģʽ

extern volatile Uint16 Board_Num; // ���ư�

extern volatile Uint16 Board2_Num; // ���ư�

extern RDC_DRIVER rdc1;
extern RDC_DRIVER rdc2;

extern PWMGEN pwma;

extern Ref_ABCtoDQ abctodq; // ABC->DQ
extern Ref_DQtoABC dqtoabc; // DQ->ABC
extern Ref_THREE2TWO phase_trans;

extern PIREG pi_Pos;   // λ�û���PI
extern PIREG pi_velo;  // �ٶ�PI
extern PIREG pi_Id_Kc; // IdPI
extern PIREG pi_Iq_Kc; // IqPI
extern PIREG pi_usLimit;
extern volatile _iq Idq_Kc;
extern volatile _iq Radq;

extern volatile Uint32 VeloProtect1;    // QG ������Iq����
extern volatile Uint32 IdProtect1;      // QG ������Id����
extern volatile Uint32 UdcOverProtect1; // QG ������Id����
extern volatile Uint32 IdcOverProtect1; // QG ������Id����
extern volatile _iq Is;                 // QG Id����ֵ
extern volatile _iq maxSpeed;

extern _iq m_PT_V[31];
extern _iq m_PT_T[31];

extern volatile Uint16 Control_Period; // Q16 �������ڼ���ֵ
extern volatile Uint16 Deadtime;       // Q16 ��������ֵ
extern volatile _iq Velo_Duty;         // QG ת���ز�ϵ��
extern volatile _iq Velo_Duty0;        // QG ת���ز�ϵ��
extern volatile _iq MotorTM;
extern volatile _iq igbtTemperatureProtectionValue;

//======================================================================================================
/*  ��Ҫ���ȫ�ֱ����Ķ���		*/
//======================================================================================================
//------------------------------------------------------
// Ƶ�ʡ��ٶ��Լ��Ƕ�
extern volatile Uint16 Angle_Q16;  // Q16 �����ȡ�ĵ�ǰλ�ý�HEX
extern volatile Uint16 Angle0_Q16; // Q16 �����ȡ����һ��λ�ý�HEX
extern volatile Uint16 Angle_Init_Digital; // Q0 �������������ת�ӳ�λ�ý�(HEX)
extern volatile Uint16 Angle_Init_Fa; // Q0 �������������ת�ӳ�λ�ý�(HEX)
extern volatile Uint16 Angle_Fa_Q16; // Q16 �����ȡ�ĵ�ǰλ�ý�HEX

extern volatile _iq Angle_ElecOpen; // QG ת��λ�õ�ǵı���ֵ
extern volatile _iq Angle_Elec;     // QG ת��λ�õ�ǵı���ֵ
extern volatile _iq Com_angle_I;    // ����dq�����ʱ�Ĳ�����
extern volatile _iq Com_angle_U;    // ����dq���ѹʱ�Ĳ�����

extern volatile _iq Velo_Elec;     // QG ���õ���ת���ٶȵ�ʵ��ֵ
extern volatile _iq Velo_Elec_Cal; // QG ���õ���ת���ٶȵļ���ֵ
extern volatile _iq Velo_Set;      // QG ������ת���ٶȵĽ�Ƶ�ʱ�ʾ
extern volatile _iq Pos_Set;       // QG ������λ��

extern volatile Uint16 Run_Led_Index; // ���е����еĿ�����ڵ�ָ�?

//----------------------------------------------------------
// ���ڴ洢ֱ��ADC�����ĵ�������,��Ҫ����У��
extern volatile Uint16 ADc_Isa;
extern volatile Uint16 ADc_Isb;
extern volatile Uint16 ADc_Isc;
extern volatile Uint16 ADc_Idc;
extern volatile Uint16 ADc_Udc;
extern volatile Uint16 ADc_Temp0;
extern volatile Uint16 ADc_Temp2;
extern volatile Uint16 ADc_Temp4;
//------------------------------------------------------------

// ������ӵ�ѹ������ı��?
extern volatile _iq Isa;      // QG Isa�ı���?
extern volatile _iq Isb;      // QG Isb�ı���ֵ
extern volatile _iq Isc;      // QG Isc�ı���ֵ
extern volatile _iq Usa;      // QG Usa����ֵ
extern volatile _iq Usb;      // QG Usb����?
extern volatile _iq Usc;      // QG Usc�ı���
extern volatile _iq Udc_Mche; // QG ֱ�����ѹ�ĵ�����λ�����ģ�
extern volatile _iq Udc_Mche_realvalue; /// QG ֱ�����ѹ��ʵ��ֵ����λ�����ģ�

extern volatile _iq Udc_Mche_1; // QG ֱ�����ѹ�Ĳ�ֵ
extern volatile _iq Udc_Mche0;  // QG ֱ�����ѹ�Ĳ�?

extern volatile _iq Idc;  // QG Idc�ı���ֵ
extern volatile _iq Idc0; // QG Idc�ı���ֵ

extern volatile _iq Id;     // QG Id����ֵ
extern volatile _iq Iq;     // QG Iq����ֵ
extern volatile _iq Id_Set; // QG Id�趨�ı���ֵ
extern volatile _iq Iq_Set; // QG Iq�趨�ı���ֵ

extern volatile _iq Ud;           // QG ���Ƶó���Ud
extern volatile _iq Uq;           // QG ���Ƶó���Uq
extern volatile _iq Us_Max;       // QG Ud��Uq����? k*0.612Udc (�㹦���?
extern volatile _iq Us_Alfa_Mche; // QG a���ѹ�ı���ֵ
extern volatile _iq Us_Beta_Mche; // QG b���ѹ�ı���ֵ

//------------------------------------------------------------
extern volatile Uint16 IdProtect;         // QG ������Id����
extern volatile Uint16 VeloProtect;       // QG ������Iq����
extern volatile Uint16 IdProtectNum;      // QG ������Id����
extern volatile Uint16 VeloProtectNum;    // QG ������Iq����
extern volatile Uint16 UdcLowProtect;     // QG ������Id����
extern volatile Uint32 UdcLowEnable;      // QG ������Id����
extern volatile Uint16 UdcOverProtect;    // QG ������Id����
extern volatile Uint16 IdcOverProtect;    // QG ������Id����
extern volatile Uint16 Temp1OverProtect;  // QG ������Id����
extern volatile Uint16 Temp2OverProtect;  // QG ������Id����
extern volatile Uint32 Temp1OverProtect1; // QG ������Id����
extern volatile Uint32 Temp2OverProtect1; // QG ������Id����
extern volatile Uint32 IGBTProtect1;      // QG ������Id����
extern volatile Uint16 RdcProtect;        // QG ������Id����
extern volatile Uint32 RdcProtect1;       // QG ������Id����
extern volatile Uint16 Rdc2Protect;       // QG ������Id����
extern volatile Uint32 Rdc2Protect1;      // QG ������Id����

extern volatile Uint32 ProtectNum; // QG ���ϻָ�ʱ��

//------------------------------------------------------------
// �����˲��������
extern volatile _iq Id0;        // QG �ϴε�Id����ֵ
extern volatile _iq Iq0;        // QG �ϴε�Iq����ֵ
extern volatile _iq Velo_Elec0; // QG �ϴμ���ĵ��ٶȵı���ֵ

//------------------------------------------------------------
//----------------------------------------

// �������
extern volatile _iq Ld;         // ���d���еı���ֵ
extern volatile _iq Lq;         // ���q���еı���ֵ
extern volatile _iq PHI;        // ��������ı���ֵ
extern volatile _iq Rs;         // �������ı���ֵ
extern volatile _iq Mp;         // ����ļ�����
extern volatile _iq Rp;         // ����ļ�����
extern volatile _iq p;          // ���������ļ�������
                                // ��ֵϵͳ
extern volatile _iq U_Base;     // ��ѹ��ֵ
extern volatile _iq I_Base;     // ������ֵ
extern volatile _iq V_Base;     // �ٶȻ�ֵ
extern volatile _iq K_Velo_Cal; // �ٶȼ������

// ��Ҫ���ȫ�ֱ����Ķ������
//======================================================================================================
//----------------------------------------------------------

//----------------------------------------------------------
extern volatile union FAULT_FLAG_REG Fault_Flag;
extern volatile union SYS_FLAG_REG Sys_Flag;
extern volatile union CTRL_FLAG_REG Ctrl_Flag;
//====================================================================================

/*id�����������*/
extern volatile _iq id_Max;
extern volatile _iq idc_Max;
extern volatile _iq idSet_Max;

/*�˲���ֹƵ�ʱ�������*/
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

extern volatile _iq Us;       // QG Us=mag(ud,uq)�ı���ֵ
extern volatile _iq Udc_Setg; // QG ֱ�����ѹ�Ĳ���ֵ
extern volatile _iq I_Add;
extern volatile _iq Velo_Start;

extern volatile _iq Isdq_Set;
extern volatile _iq Isd_Set; // ��λ�����͹�����id;
extern volatile _iq Isdq_Set1;
extern volatile _iq Isdq_Set2;

extern volatile _iq IstoTorque;
extern volatile _iq TorquetoIs;
extern volatile _iq TorqueAngle;  // 30��
extern volatile _iq TorqueAngleA; // 0��
extern volatile _iq Is_Add;
extern volatile _iq Is_PerAdd;
extern volatile _iq Veloover_Set;
extern volatile _iq Velo_Add;
extern volatile _iq Velo_PerAdd;
extern volatile _iq Per_Ctrl; // ��������

extern volatile _iq PWMA_a;        // QG ���Ƶó���Ud
extern volatile _iq PWMA_b;        // QG ���Ƶó���Uq
extern volatile _iq PWMA_c;        // QG ���Ƶó���Uq
extern volatile _iq Velo_Elec_abs; // QG ���ٶ�ı����

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

extern volatile long Velo_Calc_Num;    // �ٶ�ƽ������
extern volatile Uint16 Velo_Avg_Index; // �ٶ�ƽ��ָ��
extern volatile _iq Velotmp;
extern volatile Uint16 Velo_Calc_flag;
extern volatile _iq UdcLimit_Set;  // QG ֱ�����ѹ�Ĳ�ֵ
extern volatile _iq UdcLimit1_Set; // QG ֱ�����ѹ�Ĳ�ֵ
extern volatile _iq IdSet;
extern volatile _iq UsSet;
extern volatile _iq IdZ_Set; // QG Id�趨�ı���ֵ
extern volatile _iq IqZ_Set; // QG Iq�趨�ı���ֵ

extern volatile _iq Is_DownAdd;
extern volatile _iq Is_DownPerAdd;
extern volatile _iq Temp_Filter;
extern volatile _iq IsSet_Filter;
extern volatile _iq IsLimit_Set;
extern volatile _iq Us_M;

#endif
