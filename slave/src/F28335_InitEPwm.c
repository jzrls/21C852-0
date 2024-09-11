#include "F28335_Device.h"
#include "App_UserDef.h"

/*---------------------------------------------------------------------------
 InitEPwm:
---------------------------------------------------------------------------
 This function initializes the ePWM(s) to a known state.
*/

extern void InitEPwmTimer(void);
extern void InitTzGpio(void);
extern void EPwmInt(void);

void InitEPwm(void) {
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0; /* Stop all the TB clocks*/
    EDIS;
    EPwmInt();

    InitTzGpio();
    InitEPwmTimer();
    /*���ñȽϼĴ�����ֵ*/
    EPwm1Regs.CMPA.half.CMPA = pwma.mHalfPeriod / 2; /* Set compare EPWM1A value*/
    EPwm2Regs.CMPA.half.CMPA = pwma.mHalfPeriod / 2; /* Set compare EPWM2A value*/
    EPwm3Regs.CMPA.half.CMPA = pwma.mHalfPeriod / 2; /* Set compare EPWM3A value*/

    /*���ǿ�Ƶ�*/
    EPwm1Regs.AQSFRC.bit.RLDCSF = 3;
    /*PWMA��� 00����Ч 01��ǿ�Ƶ� 10��ǿ�Ƹ� 11����Ч,���״̬��AQ��DB��ͬ����*/
    EPwm1Regs.AQCSFRC.bit.CSFA = 1;
    EPwm1Regs.AQCSFRC.bit.CSFB = 1; /*00����Ч 01��ǿ�Ƶ� 10��ǿ�Ƹ� 11����Ч*/
    /**
     * 00:������Ч 01:PWMAֱ��ͨ��DBģ�� 10:PWMBֱ��ͨ��DBģ��
     * 11:A��ʲ��B�½��ؾ���ʱ
     **/
    EPwm1Regs.DBCTL.bit.OUT_MODE = 0;

    EPwm2Regs.AQSFRC.bit.RLDCSF = 3;
    /*PWMA��� 00����Ч 01��ǿ�Ƶ� 10��ǿ�Ƹ� 11����Ч,���״̬��AQ��DB��ͬ����*/
    EPwm2Regs.AQCSFRC.bit.CSFA = 1;
    EPwm2Regs.AQCSFRC.bit.CSFB = 1; /*00����Ч 01��ǿ�Ƶ� 10��ǿ�Ƹ� 11����Ч*/
    /**
     * 00:������Ч 01:PWMAֱ��ͨ��DBģ�� 10:PWMBֱ��ͨ��DBģ��
     * 11:A��ʲ��B�½��ؾ���ʱ
     **/
    EPwm2Regs.DBCTL.bit.OUT_MODE = 0;

    EPwm3Regs.AQSFRC.bit.RLDCSF = 3;
    /*PWMA��� 00����Ч 01��ǿ�Ƶ� 10��ǿ�Ƹ� 11����Ч,���״̬��AQ��DB��ͬ����*/
    EPwm3Regs.AQCSFRC.bit.CSFA = 1;
    EPwm3Regs.AQCSFRC.bit.CSFB = 1; /*00����Ч 01��ǿ�Ƶ� 10��ǿ�Ƹ� 11����Ч*/
    /**
     * 00:������Ч 01:PWMAֱ��ͨ��DBģ�� 10:PWMBֱ��ͨ��DBģ��
     * 11:A��ʲ��B�½��ؾ���ʱ
     */
    EPwm3Regs.DBCTL.bit.OUT_MODE = 0;
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; /* Start all the TB clocks*/
    EDIS;
}

void InitEPwmTimer(void) {
    /*EPWM1����*/
    /* Setup TBCLK*/
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;  /* Clock ratio to SYSCLKOUT 75M*/
    EPwm1Regs.TBCTL.bit.CLKDIV = 1;     /* TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV*2��*/
    EPwm1Regs.TBPRD = pwma.mHalfPeriod; /* Set timer period��TBCLKs)*/
    EPwm1Regs.TBCTR = 0x0000;           /* Clear counter      */
    /* Setup counter mode*/
    /*00�������� 01�������� 10����-������ 11��stop freeze*/
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    /* 0������TBPHS���ص�TBCTR 1��ͬ��ʱ��TBPHS���ص�TBCTR*/
    EPwm1Regs.TBCTL.bit.PHSEN = 1;
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000; /* Phase is 0*/
    /*ͬ���ź����ѡ��00:EPWMxSYNC 01:CTR=ZERO 10:CTR=CMPB 11:Disable*/
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;
    /* Setup compare*/
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0; /* 0:EnableӰ�ӱȽϼĴ���  1:Disable*/
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = 0;
    /*����Ӱ�ӱȽϼĴ��� 00:CTR=ZERO 01:CTR=PRD 10:CTR=ZERO or CTR=PRD 11:FREEZE*/
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = 0;
    /* Set actions*/
    /* PWMA�Ƚ�ƥ�����״̬ 00�������� 01���� 10���� 11��ȡ��*/
    EPwm1Regs.AQCTLA.bit.CAU = 2;
    EPwm1Regs.AQCTLA.bit.CAD = 1;
    /* PWMB�Ƚ�ƥ�����״̬ 00�������� 01���� 10���� 11��ȡ��*/
    EPwm1Regs.AQCTLB.bit.CAU = 2;
    EPwm1Regs.AQCTLB.bit.CAD = 1;
    /*set  deadtime*/
    /**
     * 00:A�����������½��� 01:B��������A�½��� 10:A��������B�½���
     * 11:B�����������½���
     **/
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;

    /**
     * 00:A��B��������01:A���� 10:B���� 11:A��B������
     * (ѡ��2Ϊ����Ч��ѡ��1Ϊ����Ч)
     **/
    EPwm1Regs.DBCTL.bit.POLSEL = 2;

    /**
     * 00:������Ч
     * 01:PWMAֱ��ͨ��DBģ��
     * 10:PWMBֱ��ͨ��DBģ��
     * 11:A��ʲ��B�½��ؾ���ʱ
     **/
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm1Regs.DBRED = PWM_DeadTime; /*��������ʱ����ʱ�䣺DBRED*TBCLK*/
    EPwm1Regs.DBFED = PWM_DeadTime; /*�½�����ʱ����ʱ�䣺DBFED*TBCLK*/

    /*EPWM2����*/

    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;
    EPwm2Regs.TBCTL.bit.CLKDIV = 1;
    EPwm2Regs.TBPRD = pwma.mHalfPeriod;
    EPwm2Regs.TBCTR = 0x0000;

    EPwm2Regs.TBCTL.bit.CTRMODE = 2;
    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 1;

    EPwm2Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = 0;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = 0;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = 0;

    EPwm2Regs.AQCTLA.bit.CAU = 2;
    EPwm2Regs.AQCTLA.bit.CAD = 1;
    EPwm2Regs.AQCTLB.bit.CBU = 2;
    EPwm2Regs.AQCTLB.bit.CBD = 1;

    EPwm2Regs.DBCTL.bit.IN_MODE = 0;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm2Regs.DBRED = PWM_DeadTime;
    EPwm2Regs.DBFED = PWM_DeadTime;

    /*EPWM3����*/
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
    EPwm3Regs.TBCTL.bit.CLKDIV = 1;
    EPwm3Regs.TBPRD = pwma.mHalfPeriod;
    EPwm3Regs.TBCTR = 0x0000;

    EPwm3Regs.TBCTL.bit.CTRMODE = 2;
    EPwm3Regs.TBCTL.bit.PHSEN = 1;
    EPwm3Regs.TBPHS.half.TBPHS = 0x0000;
    EPwm3Regs.TBCTL.bit.SYNCOSEL = 1;

    EPwm3Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = 0;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = 0;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = 0;

    EPwm3Regs.AQCTLA.bit.CAU = 2;
    EPwm3Regs.AQCTLA.bit.CAD = 1;
    EPwm3Regs.AQCTLB.bit.CAU = 2;
    EPwm3Regs.AQCTLB.bit.CAD = 1;

    EPwm3Regs.DBCTL.bit.IN_MODE = 0;
    EPwm3Regs.DBCTL.bit.POLSEL = 2;
    EPwm3Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm3Regs.DBRED = PWM_DeadTime;
    EPwm3Regs.DBFED = PWM_DeadTime;
}

void InitTzGpio(void) {
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   /* Enable pull-up on GPIO13 (TZ2)*/
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; /*/ Asynch input GPIO13 (TZ2)*/
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  /* Configure GPIO13 as TZ2*/

    EDIS;

    /* Enable TZ1 and TZ2 as one shot trip sources*/
    EALLOW;
    EPwm2Regs.TZSEL.bit.OSHT2 = 1;

    /* What do we want the TZ1 and TZ2 to do*/
    EPwm2Regs.TZCTL.bit.TZA = 3;
    EPwm2Regs.TZCTL.bit.TZB = 3;

    /*Enable TZ interrupt*/
    EPwm2Regs.TZEINT.bit.OST = 1;
    EDIS;
}

void EPwmInt(void) {
    EPwm1Regs.ETSEL.bit.INTEN = 0; /*��ֹ�ж�*/

    /**
     * 001:TBCTR=ZERO
     * 010:TBCTR=PRD
     * 100:TBCTR=CMPAU
     * 101:TBCTR=CMPAD
     * 110:TBCTR=CMPBU
     * 111:TBCTR=CMPBD
     **/
    EPwm1Regs.ETSEL.bit.INTSEL = 0x001;
    EPwm1Regs.ETPS.bit.INTPRD = 1; /*0�Ρ�1�Ρ�2�λ�3���¼������ж�*/
    EPwm1Regs.ETPS.bit.INTCNT = 0;
    EPwm1Regs.ETSEL.bit.INTEN = 1;  // ʹ���ж�
}
