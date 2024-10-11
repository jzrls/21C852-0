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
    /*设置比较寄存器的值*/
    EPwm1Regs.CMPA.half.CMPA = pwma.mHalfPeriod / 2; /* Set compare EPWM1A value*/
    EPwm2Regs.CMPA.half.CMPA = pwma.mHalfPeriod / 2; /* Set compare EPWM2A value*/
    EPwm3Regs.CMPA.half.CMPA = pwma.mHalfPeriod / 2; /* Set compare EPWM3A value*/

    /*输出强制低*/
    EPwm1Regs.AQSFRC.bit.RLDCSF = 3;
    /*PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定*/
    EPwm1Regs.AQCSFRC.bit.CSFA = 1;
    EPwm1Regs.AQCSFRC.bit.CSFB = 1; /*00：无效 01：强制低 10：强制高 11：无效*/
    /**
     * 00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块
     * 11:A上什沿B下降沿均延时
     **/
    EPwm1Regs.DBCTL.bit.OUT_MODE = 0;

    EPwm2Regs.AQSFRC.bit.RLDCSF = 3;
    /*PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定*/
    EPwm2Regs.AQCSFRC.bit.CSFA = 1;
    EPwm2Regs.AQCSFRC.bit.CSFB = 1; /*00：无效 01：强制低 10：强制高 11：无效*/
    /**
     * 00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块
     * 11:A上什沿B下降沿均延时
     **/
    EPwm2Regs.DBCTL.bit.OUT_MODE = 0;

    EPwm3Regs.AQSFRC.bit.RLDCSF = 3;
    /*PWMA输出 00：无效 01：强制低 10：强制高 11：无效,输出状态由AQ、DB共同决定*/
    EPwm3Regs.AQCSFRC.bit.CSFA = 1;
    EPwm3Regs.AQCSFRC.bit.CSFB = 1; /*00：无效 01：强制低 10：强制高 11：无效*/
    /**
     * 00:死区无效 01:PWMA直接通过DB模块 10:PWMB直接通过DB模块
     * 11:A上什沿B下降沿均延时
     */
    EPwm3Regs.DBCTL.bit.OUT_MODE = 0;
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; /* Start all the TB clocks*/
    EDIS;
}

void InitEPwmTimer(void) {
    /*EPWM1配置*/
    /* Setup TBCLK*/
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;  /* Clock ratio to SYSCLKOUT 75M*/
    EPwm1Regs.TBCTL.bit.CLKDIV = 1;     /* TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV*2）*/
    EPwm1Regs.TBPRD = pwma.mHalfPeriod; /* Set timer period（TBCLKs)*/
    EPwm1Regs.TBCTR = 0x0000;           /* Clear counter      */
    /* Setup counter mode*/
    /*00：增计数 01：减计数 10：增-减计数 11：stop freeze*/
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    /* 0：不从TBPHS下载到TBCTR 1：同步时从TBPHS下载到TBCTR*/
    EPwm1Regs.TBCTL.bit.PHSEN = 1;
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000; /* Phase is 0*/
    /*同步信号输出选择　00:EPWMxSYNC 01:CTR=ZERO 10:CTR=CMPB 11:Disable*/
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;
    /* Setup compare*/
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0; /* 0:Enable影子比较寄存器  1:Disable*/
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = 0;
    /*下载影子比较寄存器 00:CTR=ZERO 01:CTR=PRD 10:CTR=ZERO or CTR=PRD 11:FREEZE*/
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = 0;
    /* Set actions*/
    /* PWMA比较匹配输出状态 00：不动作 01：低 10：高 11：取反*/
    EPwm1Regs.AQCTLA.bit.CAU = 2;
    EPwm1Regs.AQCTLA.bit.CAD = 1;
    /* PWMB比较匹配输出状态 00：不动作 01：低 10：高 11：取反*/
    EPwm1Regs.AQCTLB.bit.CAU = 2;
    EPwm1Regs.AQCTLB.bit.CAD = 1;
    /*set  deadtime*/
    /**
     * 00:A的上升沿与下降沿 01:B上升沿与A下降沿 10:A上升沿与B下降沿
     * 11:B的上升沿与下降沿
     **/
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;

    /**
     * 00:A、B都不反向　01:A反向 10:B反向 11:A、B都反向
     * (选择2为高有效，选择1为低有效)
     **/
    EPwm1Regs.DBCTL.bit.POLSEL = 2;

    /**
     * 00:死区无效
     * 01:PWMA直接通过DB模块
     * 10:PWMB直接通过DB模块
     * 11:A上什沿B下降沿均延时
     **/
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm1Regs.DBRED = PWM_DeadTime; /*上升沿延时死区时间：DBRED*TBCLK*/
    EPwm1Regs.DBFED = PWM_DeadTime; /*下降沿延时死区时间：DBFED*TBCLK*/

    /*EPWM2配置*/

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

    /*EPWM3配置*/
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
    EPwm1Regs.ETSEL.bit.INTEN = 0; /*禁止中断*/

    /**
     * 001:TBCTR=ZERO
     * 010:TBCTR=PRD
     * 100:TBCTR=CMPAU
     * 101:TBCTR=CMPAD
     * 110:TBCTR=CMPBU
     * 111:TBCTR=CMPBD
     **/
    EPwm1Regs.ETSEL.bit.INTSEL = 0x001;
    EPwm1Regs.ETPS.bit.INTPRD = 1; /*0次、1次、2次或3次事件产生中断*/
    EPwm1Regs.ETPS.bit.INTCNT = 0;
    EPwm1Regs.ETSEL.bit.INTEN = 1;  // 使能中断
}
