#include "F28335_Device.h"
#include "App_UserDef.h"

#pragma CODE_SECTION(Init_AD1210, "ramfuncs");
#pragma CODE_SECTION(RDC_READ, "ramfuncs");

Uint16 *pRdcAddr1 = (Uint16 *)0x00240000;
Uint16 *pRdcAddr2 = (Uint16 *)0x00260000;

/* �������������������ģʽ����������Ƶ�ʣ��ֱ��ʣ���ʼ�� */
void Init_AD1210(void) {
    RDC_DIS_B;
    RDC_EN_A; // ѡ��A����

    RDC_SAMPLE_H; /*SAMPLE���ߣ��Ա��ڿ������������ͣ����¼Ĵ���*/

    RDC_A0_H;
    RDC_A1_H; /*A0,A1����Ϊ1��1��AD2S1210����������ģʽ*/

    RDC_RES0_L;
    RDC_RES1_H; /* �ֱ���Ϊ12λ*/

    RDC_RESET_L;
    DELAY_US(112L);
    RDC_RESET_H;

    RDC_A0_L;
    RDC_A1_L; /* A0,A1����Ϊ0��0��AD2S1210��������ͨģʽ�����λ���ź�*/

    RDC_DIS_A;
    RDC_EN_B; // ѡ��B����

    RDC_SAMPLE_H; /*SAMPLE���ߣ��Ա��ڿ������������ͣ����¼Ĵ���*/

    RDC_A0_H;
    RDC_A1_H; /*A0,A1����Ϊ1��1��AD2S1210����������ģʽ*/

    RDC_RES0_L;
    RDC_RES1_H; /* �ֱ���Ϊ12λ*/

    RDC_RESET_L;
    DELAY_US(112L);
    RDC_RESET_H;

    RDC_A0_L;
    RDC_A1_L; /* A0,A1����Ϊ0��0��AD2S1210��������ͨģʽ�����λ���ź�*/
}

void RDC_READ(void) {
    volatile Uint16 AD1210BUF;

    SpiaRegs.SPIBRR = 0x001F; /*������*/
    RDC_DIS_B;
    RDC_EN_A; // ѡ��A����

    RDC_SAMPLE_L; /*SAMPLE�ź����ͣ����¼Ĵ�����ֵ*/

    DELAY_US(1L); /*��ʱ1uS*/
    RDC_A0_L;
    RDC_A1_L; /* A0,A1����Ϊ0��0��AD2S1210��������ͨģʽ�����λ���ź�*/

    asm(" RPT #9 || NOP");
    /*��ʱ60nS*/

    RDC_FSYNC_L;

    asm(" RPT #9 || NOP");
    /*��ʱ60nS*/
    SpiaRegs.SPITXBUF = 0; /*����α����*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
    SpiaRegs.SPISTS.all = 0xFF;

    rdc1.DATA_BUFFER = (AD1210BUF << 1); /* ��ȡλ����Ϣ*/

    SpiaRegs.SPITXBUF = 0; /*����α����*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
    SpiaRegs.SPISTS.all = 0xFF; /*�����־λ*/

    /* ��ȡλ����Ϣ*/
    rdc1.DATA_BUFFER = (AD1210BUF << 1) + (rdc1.DATA_BUFFER << 8);
    RDC_FSYNC_H;

    DELAY_US(0.11L);

    RDC_A0_H;
    RDC_A1_H; /*A0,A1����Ϊ1��1��AD2S1210����������ģʽ*/

    RDC_FSYNC_L;
    SpiaRegs.SPITXBUF = 0xFF00; /*����α����*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
    SpiaRegs.SPISTS.all = 0xFF; /*�����־λ*/
    RDC_FSYNC_H;

    DELAY_US(0.11L);

    RDC_FSYNC_L;
    SpiaRegs.SPITXBUF = 0xFF00; /*����α����*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = SpiaRegs.SPIRXBUF & 0xFF;
    SpiaRegs.SPISTS.all = 0xFF; /*�����־λ*/
    RDC_FSYNC_H;
    rdc1.FAULT = (AD1210BUF); /* ��ȡλ����Ϣ*/

    RDC_SAMPLE_H; /*SAMPLE���ߣ��Ա�����һ���������������ͣ����¼Ĵ���*/
    DELAY_US(1L); /*��ʱ1uS*/

    /*��Ŀǰ�õĴӿ��ư��������߽ӵ���B���䣬��˴˴�������BORAD_NUM == 3��
     �����������߱õĴӿ��ư���������Ӧ�ӵ�A���䣬�˴�Ӧȥ��BORAD_NUM == 3*/
    Uint16 isValveMainCpu = (BORAD_NUM == 0 || BORAD_NUM == 4) && BC_SLT_IN == 0;
    Uint16 isValveSubCpu = (BORAD_NUM == 1 || BORAD_NUM == 5) && BC_SLT_IN == 1;
    if (isValveMainCpu || isValveSubCpu) {
        RDC_DIS_A;
        RDC_EN_B; // ѡ��B����

        RDC_SAMPLE_L; /*SAMPLE�ź����ͣ����¼Ĵ�����ֵ*/

        DELAY_US(1L); /*��ʱ1uS*/
        RDC_A0_L;
        RDC_A1_L; /* A0,A1����Ϊ0��0��AD2S1210��������ͨģʽ�����λ���ź�*/

        asm(" RPT #9 || NOP");
        /*��ʱ60nS*/

        RDC_FSYNC_L;

        asm(" RPT #9 || NOP");
        /*��ʱ60nS*/
        SpiaRegs.SPITXBUF = 0; /*����α����*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {
        }
        AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
        SpiaRegs.SPISTS.all = 0xFF;

        rdc2.DATA_BUFFER = (AD1210BUF << 1); /* ��ȡλ����Ϣ*/

        SpiaRegs.SPITXBUF = 0; /*����α����*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
        AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
        SpiaRegs.SPISTS.all = 0xFF;                                    /*�����־λ*/
        rdc2.DATA_BUFFER = (AD1210BUF << 1) + (rdc2.DATA_BUFFER << 8); /* ��ȡλ����Ϣ*/
        RDC_FSYNC_H;

        DELAY_US(0.11L);

        RDC_A0_H;
        RDC_A1_H; /*A0,A1����Ϊ1��1��AD2S1210����������ģʽ*/

        RDC_FSYNC_L;
        SpiaRegs.SPITXBUF = 0xFF00; /*����α����*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
        AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
        SpiaRegs.SPISTS.all = 0xFF; /*�����־λ*/
        RDC_FSYNC_H;

        DELAY_US(0.11L);

        RDC_FSYNC_L;
        SpiaRegs.SPITXBUF = 0xFF00; /*����α����*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
        AD1210BUF = SpiaRegs.SPIRXBUF & 0xFF;
        SpiaRegs.SPISTS.all = 0xFF; /*�����־λ*/
        RDC_FSYNC_H;
        rdc2.FAULT = (AD1210BUF); /* ��ȡλ����Ϣ*/

        if (BORAD_NUM == 3 && BC_SLT_IN == 1) {
            rdc1.DATA_BUFFER = rdc2.DATA_BUFFER;
            rdc1.FAULT = rdc2.FAULT;
        }

        RDC_SAMPLE_H; /*SAMPLE���ߣ��Ա�����һ���������������ͣ����¼Ĵ���*/
    } else {
        rdc2.FAULT = 0;
    }

    SpiaRegs.SPIBRR = 0x0037; /*������*/
}
