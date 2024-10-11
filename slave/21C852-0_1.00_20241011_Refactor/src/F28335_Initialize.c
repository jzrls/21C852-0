#include "F28335_Device.h"
#include "App_UserDef.h"

/*---------------------------------------------------------------------------*/

/* �ú����������ⲿ�洢��,��ȴ�������ڲ�RAM(�쿴CMD�ļ���ramfuncs������)*/
#pragma CODE_SECTION(InitXintf, "ramfuncs");
#pragma CODE_SECTION(InitFlash, "ramfuncs");

/*=========================================================================
 ��������:
            �ڴ濽������.
          �������������ָ����ַ�е����ݵ�����һ��ָ����ַ
========================================================================*/
void MemCopy(Uint16 *SourceAddr, Uint16 *SourceEndAddr, Uint16 *DestAddr) {
    while (SourceAddr <= (SourceEndAddr - 0x0001)) {
        (*DestAddr) = (*SourceAddr);
        DestAddr++;
        SourceAddr++;
    }
    return;
}

/*#############################################################################
ϵͳ��ʼ������
-----------------------------------------------------------------------------
 ��ʼ��ϵͳ���ƼĴ���:
 - �رտ��Ź�
 - ����PLLCRʹSYSCLKOUTƵ�ʺ���
 - ����HISPCP��LOSPCP���ʵ�pre-scaler
 - ʹ������ʱ�� */
void InitSysCtrl(void) {

    /*�رտ��Ź�*/
    DisableDog();

    /*PLLCR����*/
    InitPll(PLLCR_RATIO, VCOCLK_DIVSEL);

    /*��ʼ���ⲿʱ��*/
    InitPeripheralClocks();
}

/*----------------------------------------------
 ������: ServiceDog:
----------------------------------------------
 �ú�����λ���Ź���ʱ��.*/
void ServiceDog(void) {
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

/*----------------------------------------------
 ������: DisableDog:
----------------------------------------------
 �ú����رտ��Ź���ʱ��.*/
void DisableDog(void) {
    EALLOW;
    SysCtrlRegs.WDCR = 0x0068;
    EDIS;
}

/*-----------------------------------------------
 ������: InitPll:
-----------------------------------------------
 �ú�����ʼ�� PLLCR �Ĵ���.*/
void InitPll(Uint16 val, Uint16 divsel) {

    /* ȷ��PLLû�й�����limpģʽ��*/
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0) {
        ESTOP0;
    }

    /* �ı�PLLCR֮ǰ���뽫DIVEL��0.�ⲿ��λʱ����λ��0*/
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0) {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    /* �޸ı�Ƶϵ��*/
    if (SysCtrlRegs.PLLCR.bit.DIV != val) {

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF =
            1; /* ��дPLLCR֮ǰ�رն�ʧʱ�Ӽ���߼�*/
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        DisableDog(); /*�رտ��Ź�*/

        while (SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) {
        } /*�ȴ�PLL�ȶ�*/

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    if ((divsel == 1) || (divsel == 2)) {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }
    if (divsel == 3) {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
        DELAY_US(50L);
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
        EDIS;
    }
}

/*---------------------------------------------
 ������: InitPeripheralClocks:
---------------------------------------------
 �ú�����ʼ������ʱ��.Ϊ���ٹ���,�ر�û��ʹ�õ�������ʱ�ӡ�
 ע��: ���ĳ����ʱ��û��ʹ�ܣ����ܶ�����д������ļĴ���*/
void InitPeripheralClocks(void) {
    EALLOW;

    /* HISPCP/LOSPCP ��Ƶ����Ĵ������á�*/
    SysCtrlRegs.HISPCP.all = HISPCP_RATIO;
    SysCtrlRegs.LOSPCP.all = LOSPCP_RATIO;

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; /* ADC*/

    /* IMPORTANT  ADC_cal������OTP�洢���е�ADC�궨ֵ������ADCREFSEL��ADCOFFTRIM
     �������Ĵ����С��ڵ��øú���֮ǰ����ʹ��ADCʱ�ӡ�*/

    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;   /* I2C*/
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   /* SCI-A*/
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   /* SCI-B*/
    SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;   /* SCI-C*/
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   /* SPI-A*/
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 0; /* McBSP-A*/
    SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 0; /* McBSP-B*/
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;  /* eCAN-A*/
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;  /* eCAN-B*/

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  /* Disable TBCLK within the ePWM*/
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; /* ePWM1*/
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1; /* ePWM2*/
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1; /* ePWM3*/
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0; /* ePWM4*/
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0; /* ePWM5*/
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0; /* ePWM6*/
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;  /* Enable TBCLK within the ePWM*/

    SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0; /* eCAP3*/
    SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0; /* eCAP4*/
    SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 0; /* eCAP5*/
    SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 0; /* eCAP6*/
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0; /* eCAP1*/
    SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0; /* eCAP2*/
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0; /* eQEP1*/
    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0; /* eQEP2*/

    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; /* CPU Timer 0*/
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; /* CPU Timer 1*/
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; /* CPU Timer 2*/

    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;    /* DMA Clock*/
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;  /* XTIMCLK*/
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1; /* GPIO input clock*/

    EDIS;
}

/*---------------------------------------------
 ������: CsmUnlock:
---------------------------------------------
�����������CSM���û�������0xFFFFȡ��DSP�ĵ�ǰ���룬�����ɹ�����1*/
Uint16 CsmUnlock(void) {
    volatile Uint16 temp;

    /*��ǰ������������Ĵ�����0xFFFFΪ�������룬�û��������������DSP�ĵ�ǰ����*/
    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;
    /* Perform a dummy read of the password locations
     if they match the key values, the CSM will unlock*/
    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;
    /* ���CSM���������سɹ�������ʧ��*/
    if (CsmRegs.CSMSCR.bit.SECURE == 0) {
        temp = STATUS_SUCCESS;
    } else {
        temp = STATUS_FAIL;
    }
    return temp;
}

/*-----------------------------------------------------------------------------
ϵͳ���ֳ�ʼ�����
#############################################################################

#############################################################################
�����ʼ������
-----------------------------------------------------------------------------
---------------------------------------------------------------------------
 ��ʼ������:
---------------------------------------------------------------------------
 �ú�����ʼ�����赽Ĭ�ϵ�״̬.������������ĳ�ʼ��������
 �ú������������������λʱ���á�*/

void InitPeripherals(void) {

    /* ��ʼ���ⲿ�ӿڵ�Ĭ��״̬:*/
    InitXintf();

    /* ��ʼ���ڲ�FLASH��Ĭ��״̬:*/
    InitFlash();

    /* ��ʼ�� GPIO*/
    InitGpio();

    /* ��ʼ��CPU��ʱ����Ĭ��״̬:*/
    InitCpuTimers();

    /* ��ʼ��EPwm��Ĭ��״̬:*/
    InitEPwm();

    /* ��ʼ��ECap��Ĭ��״̬:*/
    InitECap();

    /* ��ʼ��EQep��Ĭ��״̬:*/
    InitEQep();

    /* ��ʼ�� ADC ��Ĭ��״̬:*/
    InitAdc();

    /* ��ʼ�� SCI ��Ĭ��״̬:*/
    InitSci();

    /* ��ʼ�� SPI ?��״??:*/
    InitSpi();

    /* ��ʼ�� ECan ��Ĭ��״̬:*/
    InitECan();

    /* ��ʼ���ⲿ�жϵ�Ĭ��״̬*/
    InitXIntrupt();

    Init_AD1210();
}

/*-------------------------------------------
 �ú�����ʼ���ⲿ�ӿڵ�
 �޸�XINTF�Ĵ���ʱ,������ز������ⲿ����
-------------------------------------------*/

void InitXintf(void) {
    /* All Zones---------------------------------
    Timing for all zones based on XTIMCLK = 1/2 SYSCLKOUT*/

    EALLOW;
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
    XintfRegs.XINTCNF2.bit.WRBUFF = 0;  /* No write buffering*/
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;  /* XCLKOUT is enabled*/
    XintfRegs.XINTCNF2.bit.CLKMODE = 1; /* XCLKOUT = XTIMCLK/2*/

    /* Zone 0------------------------------------*/
    /* Zone write timing*/
    XintfRegs.XTIMING0.bit.XWRLEAD = 3;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 3;
    /* Zone read timing*/
    XintfRegs.XTIMING0.bit.XRDLEAD = 3;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 3;
    /* double all Zone read/write lead/active/trail timing*/
    XintfRegs.XTIMING0.bit.X2TIMING = 1;
    /* Zone will sample XREADY signal*/
    XintfRegs.XTIMING0.bit.USEREADY = 0;
    XintfRegs.XTIMING0.bit.READYMODE = 1; /* sample asynchronous*/
    /* Size must be either: 0,1 = x32 or 1,1 = x16 other values are reserved*/
    XintfRegs.XTIMING0.bit.XSIZE = 3;

    /* Zone 6------------------------------------*/
    /* Zone write timing*/
    XintfRegs.XTIMING6.bit.XWRLEAD = 3;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 3;
    /* Zone read timing*/
    XintfRegs.XTIMING6.bit.XRDLEAD = 3;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 3;
    /* double all Zone read/write lead/active/trail timing*/
    XintfRegs.XTIMING6.bit.X2TIMING = 1;
    /* Zone will sample XREADY signal*/
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.READYMODE = 1; /* sample asynchronous*/
    /* Size must be either: 0,1 = x32 or 1,1 = x16 other values are reserved*/
    XintfRegs.XTIMING6.bit.XSIZE = 3;

    /* Zone 7------------------------------------*/
    /* Zone write timing*/
    XintfRegs.XTIMING7.bit.XWRLEAD = 3;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 3;
    /* Zone read timing*/
    XintfRegs.XTIMING7.bit.XRDLEAD = 3;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 3;
    /* double all Zone read/write lead/active/trail timing*/
    XintfRegs.XTIMING7.bit.X2TIMING = 1;
    /* Zone will sample XREADY signal*/
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 1; /* sample asynchronous*/
    /* Size must be either: 0,1 = x32 or 1,1 = x16 other values are reserved*/
    XintfRegs.XTIMING7.bit.XSIZE = 3;

    XintfRegs.XBANK.bit.BANK = 7;
    XintfRegs.XBANK.bit.BCYC = 7;
    EDIS;

    asm(" RPT #7 || NOP");
}

/*-------------------------------------------
 �ú�����ʼ��FLASH�� �ú����������ڲ�RAM�����С���OTP/Flash��
 ���������Ԥ֪�Ĵ���
-------------------------------------------*/

void InitFlash(void) {
    EALLOW;
    FlashRegs.FOPT.bit.ENPIPE = 1; /* ʹ��Flash����ˮ��ģʽ*/

    /* ��С��ʱ��ȴ�*/
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5; /* ����ҳ���ʵȴ�ʱ��*/
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 5; /*����������ʵȴ�ʱ��*/

    FlashRegs.FOTPWAIT.bit.OTPWAIT = 8; /*����OTP���ʵȴ�ʱ�� */

    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    asm(" RPT #7 || NOP");
}

/*--------------------------------------------
 CpuTimer0 ��ʼ��,����ϵͳ��������ʱ��(100us)
--------------------------------------------*/

void InitCpuTimer0(void) {
    /* ��ʼ������:*/
    CpuTimer0Regs.PRD.all = CPUTIMER0_PRD;
    /* ��ʼ��Ԥ���� (SYSCLKOUT):*/
    CpuTimer0Regs.TPR.all = 0;
    CpuTimer0Regs.TPRH.all = 0;
    /* ȷ����ʱ��ֹͣ��:*/
    CpuTimer0Regs.TCR.bit.TSS = 1;
    /* ������ֵ���ص������Ĵ���:*/
    CpuTimer0Regs.TCR.bit.TRB = 1;
    /* ��������ֹͣ     */
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.FREE = 0;
    /* ��ʱ���ж�ʹ��   */
    CpuTimer0Regs.TCR.bit.TIE = 1; /*0��ֹ�жϣ�1ʹ�ܶ�ʱ���ж�  */
}
void InitCpuTimers(void) { InitCpuTimer0(); }
/*-------------------------------------------
-------------------------------------------
 ��ʼ��Qepģ��:
-------------------------------------------*/

void InitEQep(void) {}
/*-------------------------------------------
 ��ʼ��capģ��: �����ֲ���ģʽ
˫������������SEQ1�¼�����SEQ1��SEQ2�¼�����SEQ2��MAXCONV0-2��ʾSEQ1ת�����и�����MAXCONV4-6��ʾSEQ2ת�����и�����
˫���������У�SEQ1�¼�����SEQ1��SEQ2�¼�����SEQ2��SEQ1��CONV00-03��Ч������ת��A0��B0��A1��B1��8��ͨ����SEQ2��CONV08-0B��Ч��MAXCONV�����Ϊ0x33��
����������������SEQ1�¼�����16��ͨ����MAXCONV0-3��ʾת�����и�����
�������������У�SEQ1�¼�����16��ͨ����MAXCONV0-3�����Ϊ0x7������ת��A0��B0��A1��B1��16��ͨ������������α�����result0-result15��
-------------------------------------------*/

/*-------------------------------------------
 ��ʼ��ADCģ��:
-------------------------------------------*/

void InitAdc(void) {

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; /*����У������ǰ��ȷ��ȫ��ADCʱ��*/
    ADC_cal();
    EDIS;

    AdcRegs.ADCREFSEL.bit.REF_SEL = 0; /*�����ڲ��ο���Դ*/
    AdcRegs.ADCTRL3.all = 0x00E0; /* ��϶���ο���·������ģ���·��Դ�ϵ�*/
    DELAY_US(ADC_usDELAY); /* ��ʱ*/

    AdcRegs.ADCTRL3.bit.ADCCLKPS =
        ADC_CLKPS; /* ADCCLK = HSPCLK/[2*ADCCLKPS*(ADCTRL1[7]+1)]
                      ADCTRL1[7]=0*/
    AdcRegs.ADCTRL3.bit.SMODE_SEL = SMODE; /* 1��������,0��������*/
    AdcRegs.ADCTRL1.bit.CPS = 0;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1; /* 0:˫������ģʽ  1:������ʽ*/
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0; /* 0 ��ͣģʽ��1 ��������*/
    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK; /* �ɱ�ʱ��ΪADC_SHCLK*ADCCLK*/

    AdcRegs.ADCTRL2.all = 0x0000;

    AdcRegs.ADCMAXCONV.all =
        0x0007; /* ת����  2-0:SEQ1  6-4:SEQ2  3-0:SEQ(����ʱ��*/
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4;
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5;
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6;
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7;
}

/*-------------------------------------------
 ���ڳ�ʼ������
-------------------------------------------*/

void InitSci(void) {
    // ScicRegs.SCIFFTX.all = 0x8000;				/* �λFIFO's ��ֹFIFO*/
    // ScicRegs.SCICCR.all = 0x0007;				/*
    // ��ֹЧ�飬������ģʽ��8λ��ʽ��һ��ֹͣλ*/
    ScicRegs.SCICCR.all =
        0x0027; /* ��ֹЧ�飬������ģʽ��8λ��ʽ��һ��ֹͣλ,ʹ����Ч��*/
    ScicRegs.SCICTL1.all =
        0x0003; /* ��ֹ���մ����ж�ʹ�ܣ��������ߣ�ʹ�ܷ��ͺͽ���*/
    ScicRegs.SCICTL2.all = 0x0003; /* ʹ�ܽ����жϺͷ����ж�*/

    ScicRegs.SCIHBAUD = (SCI_BAUD >> 8) & 0x00FF; /* ���ز�����*/
    ScicRegs.SCILBAUD = (SCI_BAUD) & 0x00FF;

    ScicRegs.SCIFFTX.all = 0xC001; // ʹ��FIFO����ֹ�����жϣ��������1
    ScicRegs.SCIFFRX.all = 0x0001; // ��ֹ�����жϣ��������1
    // ScicRegs.SCIFFRX.all=0x0021;//ʹ�ܽ����жϣ��������1
    ScicRegs.SCIFFCT.all = 0x00;

    ScicRegs.SCIPRI.all = 0x0000;  /* �������ʱֹͣ */
    ScicRegs.SCICTL1.all = 0x0023; /* ��λ��λ�����ڳ�ʼ�����*/

    // ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    // ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
}

/*-------------------------------------------
 �ú�����ʼ���ⲿ�жϡ�
-------------------------------------------*/

void InitXIntrupt(void) {
    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 10; /* Xint1 is GPIO10*/
    EDIS;

    XIntruptRegs.XINT1CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XINT1CR.bit.ENABLE = 1; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XINT2CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01����??? 10���½���
              11��Both���������½���*/
    XIntruptRegs.XINT2CR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XINT3CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XINT3CR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XINT4CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XINT4CR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XINT5CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XINT5CR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XINT6CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XINT6CR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XINT7CR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XINT7CR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/

    XIntruptRegs.XNMICR.bit.POLARITY =
        0; /*00:�½��ز����ж� 01�������� 10���½��� 11��Both���������½���*/
    XIntruptRegs.XNMICR.bit.SELECT =
        1; /*0:Timer 1 connected To INT13   1��XNMI_XINT13 connected To INT13*/
    XIntruptRegs.XNMICR.bit.ENABLE = 0; /*0:��ֹ  1��ʹ��*/
}

void InitECap(void) {}

/*�����ʼ�����
----------------------------------------------------------------------------*/

/*===========================================================================
 No more.
===========================================================================*/
