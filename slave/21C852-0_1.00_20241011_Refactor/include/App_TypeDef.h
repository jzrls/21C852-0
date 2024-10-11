#ifndef APP_TYPEDEF_H
#define APP_TYPEDEF_H

//======================================================================================================
/* F2812��PWM�������ṹ����		*/
//======================================================================================================
typedef struct {
    volatile Uint32 mHalfPeriod; // ����: ����HISPCPʱ�ӷ�Ƶ���PWM���ڵ�һ��(Q0)
    volatile _iq mDuty1;         // ����: PWM 1&2 ռ�ձ� (QG)
    volatile _iq mDuty2;         // ����: PWM 3&4 ռ�ձ� (QG)
    volatile _iq mDuty3;         // ����: PWM 5&6 ռ�ձ� (QG)
} PWMGEN;
//------------------------------------------------------------------------------
// Ĭ�ϵ�F2812 PWMGEN
// -----------------------------------------------------------------------------
#define PWMAGEN_DEFAULTS \
    {CARRY_A_P, HALFA_DUTY, HALFA_DUTY, HALFA_DUTY}
//------------------------------------------------------------------------------
// ����ԭ����  App_EV.C
//------------------------------------------------------------------------------
// F2812��PWM�������ṹ�������
//======================================================================================================

//======================================================================================================
// F2812��RDC�ṹ����
//======================================================================================================
typedef struct {
    volatile Uint16 FAULT;       // �����־ (1Ϊ�д�,0λ�޴�)
    volatile Uint16 DATA_BUFFER; // �������ݵĻ���
} RDC_DRIVER;
//------------------------------------------------------------------------------
// Ĭ�ϵ�RDC_READ1 RDC_READ2
// ------------------------------------------------------------------------------
#define RDC_READ_DEFAULT \
    {0, 0}

//-----------------------------------------------------------------------------
// ����ԭ��
//-----------------------------------------------------------------------------
void RDC_READ(void);

// F2812��RDC�ṹ�������
//======================================================================================================

//======================================================================================================
/* ����ϵ�任�ṹ����      */
//======================================================================================================
//------------------------------------------------------------------------------
// ��ֹabc��������ϵ�任��ͬ��dq����ϵ
//------------------------------------------------------------------------------
typedef struct {
    volatile _iq as;  // ����: a�����
    volatile _iq bs;  // ����: b�����
    volatile _iq cs;  // ����: c�����
    volatile _iq ang; // ����: ��Ƕ� (pu)
    volatile _iq dr;  // ���: ͬ������ϵd�����
    volatile _iq qr;  // ���: ͬ������ϵq�����
} Ref_ABCtoDQ;

#define Ref_ABCtoDQ_DEFAULTS \
    {0, 0, 0, 0, 0, 0}  // �����ʼ����Ref_ABCtoDQ
void ABC2DQ_CALC(void); // ����ԭ��
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ͬ��dq����ϵ�任����ֹabc����ϵ
//------------------------------------------------------------------------------
typedef struct {
    volatile _iq as;  // ���: a�����
    volatile _iq bs;  // ���: b�����
    volatile _iq cs;  // ���: c�����
    volatile _iq ang; // ����: ��Ƕ� (pu)
    volatile _iq dr;  // ����: ͬ������ϵd�����
    volatile _iq qr;  // ����: ͬ������ϵq�����
} Ref_DQtoABC;

#define Ref_DQtoABC_DEFAULTS \
    {0, 0, 0, 0, 0, 0}  // �����ʼ����Ref_DQtoABC
void DQ2ABC_CALC(void); // ����ԭ��
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ��ֹabc����ϵ�任����ֹ��2�������
//------------------------------------------------------------------------------
typedef struct {
    volatile _iq as; // ����: a�����
    volatile _iq bs; // ����: b�����
    volatile _iq cs; // ����: c�����
    volatile _iq ds; // ���: ds����
    volatile _iq qs; // ���: qs����
} Ref_THREE2TWO;

#define Ref_THREE2TWO_DEFAULTS \
    {0, 0, 0, 0, 0}        // �����ʼ����Ref_THREE2TWO
void THREE2TWO_CALC(void); // ����ԭ��

//-------------------------------------------------------------------------------
// PI�ṹ��Ķ���
//-------------------------------------------------------------------------------
typedef struct {
    volatile _iq pi_ref_reg; // ����: ��������
    volatile _iq pi_fdb_reg; // ����: ��������
    volatile _iq e_reg;      // ����: ���
    volatile int32 loop_index;
    volatile int32 i_period;    // ����: ��������
    volatile _iq Kp_reg;        // ����: ����ϵ��
    volatile _iq Ki_reg;        // ����: ����ϵ��
    volatile _iq ui_out_max;    // ����: ����������޷�
    volatile _iq ui_out_min;    // ����: ����������޷�
    volatile _iq pi_out_max;    // ����: PI������޷�
    volatile _iq pi_out_min;    // ����: PI������޷�
    volatile _iq up_reg;        // ����: �������
    volatile _iq ui_reg;        // ����: �������
    volatile _iq pi_out_reg;    // ���: PI���
    volatile int32 ctrl_period; // ����: ��������
    volatile int32 ctrl_index;  // ����: ��������ָ��

    void (*calc)(); /* Pointer to calculation function */
} PIREG;

typedef PIREG *PIREG_handle;

#define PIREG_VELO_DEFAULTS                                         \
    {                                                               \
        0, 0, 0, 0, VELO_I_NUM, KP_VELO, KI_VELO, KI_VELO_OUT_MAX,  \
        KI_VELO_OUT_MIN, PI_VELO_OUT_MAX, PI_VELO_OUT_MIN, 0, 0, 0, \
        VELO_CONTROL_PERIOD, 0, (void (*)(int32))pi_reg_calc}

#define PIREG_ID_DEFAULTS                                                 \
    {                                                                     \
        0, 0, 0, 0, ID_I_NUM, KP_ID, KI_ID, KI_ID_OUT_MAX, KI_ID_OUT_MIN, \
        PI_ID_OUT_MAX, PI_ID_OUT_MIN, 0, 0, 0, ID_CONTROL_PERIOD, 0,      \
        (void (*)(int32))pi_reg_calc}

#define PIREG_IQ_DEFAULTS                                                 \
    {                                                                     \
        0, 0, 0, 0, IQ_I_NUM, KP_IQ, KI_IQ, KI_IQ_OUT_MAX, KI_IQ_OUT_MIN, \
        PI_IQ_OUT_MAX, PI_IQ_OUT_MIN, 0, 0, 0, IQ_CONTROL_PERIOD, 0,      \
        (void (*)(int32))pi_reg_calc}

void pi_reg_calc(PIREG *PIREG_handle);
// PI�ṹ�嶨�����
//====================================================================================

//====================================================================================
// SCI�ṹ��Ķ���
//----------------------------------------------------------------------
// SCI��ز����ĵ�λ����
struct SCI_FLAG_BITS {
    volatile Uint16 RX_FLAG : 1;     // "1"����������Ч��־
    volatile Uint16 TXBO_FLAG : 1;   // �������ݿ���ɱ�־
    volatile Uint16 RXTIME_FLAG : 1; // 1��ʾ���ڽ���ָ��
    volatile Uint16 PC_FLAG : 1;     // 1��ʾ��λ������ģʽ
    volatile Uint16 rsvd1 : 12;
};

union SCI_FLAG_REG {
    volatile Uint16 all;
    volatile struct SCI_FLAG_BITS bit;
};

//------------------------------------------------------------------------------
// SCI�ṹ�嶨��
//-----------------------------------------------------------------------
typedef struct {
    volatile union SCI_FLAG_REG FLAG; // SCI�ĸ���־
    volatile Uint16 m_Rx_Length;      // �������ݳ���
    volatile Uint16 m_Tx_Length;      // �������ݳ���
    volatile Uint16 *p_Tx_Buffer;     // �������ݻ���
    volatile Uint16 *p_Rx_Buffer;     // �������ݻ���
    volatile Uint16 *p_Rx_AllBuffer;  // ȫ���������ݻ���
} volatile SCIB_REGS;

//------------------------------------------------------------------------------
// Ĭ�ϵ�SCIB_REGS
//------------------------------------------------------------------------------
#define SCIB_REGS_DEFAULTS \
    {{2}, 0, 0, SCI_TX_BUFFER, SCI_RX_BUFFER, SCI_RX_ALLBUFFER}
// SCI�ṹ�������
//======================================================================================

//====================================================================================
// CAN�ṹ��Ķ���
//----------------------------------------------------------------------
// CAN��ز����ĵ�λ����
struct CAN_FLAG_BITS {
    volatile Uint16 RX_FLAG : 1;     // "1"����������Ч��־
    volatile Uint16 TXBO_FLAG : 1;   // �������ݿ���ɱ�־
    volatile Uint16 TIMETX_FLAG : 1; // ��ʱ����
    volatile Uint16 DIAGTX_FLAG : 1; // ��̬����(���跢��)
    volatile Uint16 rsvd1 : 12;
};

union CAN_FLAG_REG {
    volatile Uint16 all;
    struct CAN_FLAG_BITS bit;
};

//------------------------------------------------------------------------------
// CAN�ṹ�嶨��
//-----------------------------------------------------------------------
typedef struct {
    union CAN_FLAG_REG FLAG;       // SCI�ĸ���־
    volatile Uint16 m_Rx_Length;   // �������ݳ�?
    volatile Uint16 m_Tx_Length;   // �������ݳ���
    volatile Uint16 *p_Tx_Buffer;  // �������ݻ���
    volatile Uint16 *p_Rx_Buffer;  // �������ݻ���
    volatile Uint16 m_Time_Index;  // ��ʱָ��
    volatile Uint16 m_Time_Length; // ��ʱ����
} CAN_REGS;

//------------------------------------------------------------------------------
// Ĭ�ϵ�CAN_REGS
//------------------------------------------------------------------------------
#define CAN_REGS_DEFAULTS \
    {2, 0, 0, TX_BUFFER, RX_BUFFER, 0, 500}

#define CAN2_REGS_DEFAULTS \
    {2, 0, 0, SCI_RX_BUFFER, SCI_TX_BUFFER, 0, 500}

//======================================================================================================
/* ���ϱ�־λ�ṹ����		*/
//======================================================================================================
struct FAULT_FLAG_BITS {
    volatile Uint16 STOP_VELO_FLAG : 1;     //"1"��ʾֹͣ�ٶȿ��Ƴ���
    volatile Uint16 STOP_PWM_Flag : 1;      // ����ֹͣ
    volatile Uint16 STOP_PWM_Flag_Id : 1;   // "1"���������������
    volatile Uint16 STOP_PWM_Flag_Velo : 1; // "1"�������ٱ���
    volatile Uint16 STOP_PWM_Flag_Driv : 1; // "1"����PWMA���ʱ��� , Ӳ�����ϣ�ĸ�߹�����
    volatile Uint16 UDC_FLAG : 1;           // "1"����ֱ����ѹ����
    volatile Uint16 Temp1Over_Flag : 1;     // �������1
    volatile Uint16 Temp2Over_Flag : 1;     // ģ�����

    volatile Uint16 UdcLow_Flag : 1;   // ֱ��Ƿѹ
    volatile Uint16 CANERROR_FLAG : 1; // ͨ�Ź���
    volatile Uint16 RDC1_FAULT : 1;
    volatile Uint16 RDC2_FAULT : 1;
    volatile Uint16 POS_LIMIT : 1;
    volatile Uint16 RunAllow_Flag : 1;
    volatile Uint16 rsvd1 : 2; // ����
};

union FAULT_FLAG_REG {
    volatile Uint16 all;
    volatile struct FAULT_FLAG_BITS bit;
};

//=======================================================================================
// Ĭ�ϵĹ��ϱ�־λ
//=======================================================================================
#define FAULT_FLAG_DEFAULT {0x0}
// ���ϱ�־λ�ṹ�嶨�����
//====================================================================================

//======================================================================================================
/* ϵͳ��־λ�ṹ����		*/
//======================================================================================================
struct SYS_FLAG_BITS {
    volatile Uint16 STOP_PWM_Flag_Id : 1;   // "1"���������������
    volatile Uint16 Flag_Id : 1;            // "1"����d������������޷�
    volatile Uint16 STOP_PWM_Flag_Velo : 1; // "1"�������ٱ���
    volatile Uint16 STOP_PWM_Flag_Driv : 1; // "1"����PWMA���ʱ���
    volatile Uint16 Flag_Iq : 1;            // "1"����q������������޷�
    volatile Uint16 Flag_Us : 1;            // "1"�����ܵ�ѹ�޷�
    volatile Uint16 UDC_FLAG : 1;           // "1"����ֱ����ѹ����
    volatile Uint16 CAN_FLAG : 1;           // "1"�����ӽ��յ�����ָ��
    volatile Uint16 RDC_FAULT : 8;          // ����
};

union SYS_FLAG_REG {
    volatile Uint16 all;
    volatile struct SYS_FLAG_BITS bit;
};

//=======================================================================================
// Ĭ�ϵ�ϵͳ��־λ
//=======================================================================================
#define SYS_FLAG_DEFAULT {0x0}
// ϵͳ��־λ�ṹ�嶨�����
//====================================================================================

//=======================================================================================
struct CTRL_FLAG_BITS {
    volatile Uint16 Temp1Over_Flag : 1; // �������1
    volatile Uint16 Temp2Over_Flag : 1; // ģ�����
    volatile Uint16 UdcLow_Flag : 1;    // ֱ��Ƿѹ
    volatile Uint16 CANERROR_FLAG : 1;  // ͨ�Ź���
    volatile Uint16 STOP_PWM_Flag : 1;  // ����ֹͣ
    volatile Uint16 CAN_Ctrl : 1;
    volatile Uint16 FIRST_PWM_Flag : 1;    // �״ν������
    volatile Uint16 VELO_CONTROL_FLAG : 1; //"1"��ʾ�����ٶȿ��Ƴ���
    volatile Uint16 TORQ_CONTROL_FLAG : 1; //"1"��ʾ����ת�ؿ��Ƴ���
    volatile Uint16 OPEN_LOOP_FLAG : 1;    // Openloop  618���и�
    volatile Uint16 LOCK_FLAG : 1;         //"1"��ʾLOCK  618���и�
    volatile Uint16 UsCtrl_Flag : 1;       // �Զ�����
    volatile Uint16 STOP_VELO_FLAG : 1;    //"1"��ʾֹͣ�ٶȿ��Ƴ���
    volatile Uint16 POS_CONTROL_FLAG : 1;  //"1"��ʾ����λ�ÿ��Ƴ���
    volatile Uint16 RotorWay_Flag : 1;     // ���䷽��
    volatile Uint16 VelWay_Flag : 1;       // ת�ٷ���
};

union CTRL_FLAG_REG {
    volatile Uint16 all;
    volatile struct CTRL_FLAG_BITS bit;
};

#define CTRL_FLAG_DEFAULT {0x1000}

#endif
