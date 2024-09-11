#include "F28335_Device.h"
#include "App_UserDef.h"

/*---------------------------------------------------------------------------*/

/* 该函数加载在外部存储区,但却运行在内部RAM(察看CMD文件中ramfuncs的配置)*/
#pragma CODE_SECTION(InitXintf, "ramfuncs");
#pragma CODE_SECTION(InitFlash, "ramfuncs");

/*=========================================================================
 功能描述:
            内存拷贝函数.
          这个函数将拷贝指定地址中的内容到另外一个指定地址
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
系统初始化程序
-----------------------------------------------------------------------------
 初始化系统控制寄存器:
 - 关闭看门狗
 - 设置PLLCR使SYSCLKOUT频率合适
 - 设置HISPCP、LOSPCP合适的pre-scaler
 - 使能外设时钟 */
void InitSysCtrl(void) {

    /*关闭看门狗*/
    DisableDog();

    /*PLLCR设置*/
    InitPll(PLLCR_RATIO, VCOCLK_DIVSEL);

    /*初始化外部时钟*/
    InitPeripheralClocks();
}

/*----------------------------------------------
 函数名: ServiceDog:
----------------------------------------------
 该函数复位看门狗定时器.*/
void ServiceDog(void) {
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

/*----------------------------------------------
 函数名: DisableDog:
----------------------------------------------
 该函数关闭看门狗定时器.*/
void DisableDog(void) {
    EALLOW;
    SysCtrlRegs.WDCR = 0x0068;
    EDIS;
}

/*-----------------------------------------------
 函数名: InitPll:
-----------------------------------------------
 该函数初始化 PLLCR 寄存器.*/
void InitPll(Uint16 val, Uint16 divsel) {

    /* 确认PLL没有工作在limp模式下*/
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0) {
        ESTOP0;
    }

    /* 改变PLLCR之前必须将DIVEL置0.外部复位时，该位置0*/
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0) {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    /* 修改倍频系数*/
    if (SysCtrlRegs.PLLCR.bit.DIV != val) {

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF =
            1; /* 改写PLLCR之前关闭丢失时钟检测逻辑*/
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        DisableDog(); /*关闭看门狗*/

        while (SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) {
        } /*等待PLL稳定*/

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
 函数名: InitPeripheralClocks:
---------------------------------------------
 该函数初始化外设时钟.为减少功耗,关闭没有使用的是外设时钟。
 注意: 如果某外设时钟没有使能，则不能读或者写该外设的寄存器*/
void InitPeripheralClocks(void) {
    EALLOW;

    /* HISPCP/LOSPCP 分频定标寄存器设置。*/
    SysCtrlRegs.HISPCP.all = HISPCP_RATIO;
    SysCtrlRegs.LOSPCP.all = LOSPCP_RATIO;

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; /* ADC*/

    /* IMPORTANT  ADC_cal函数将OTP存储器中的ADC标定值拷贝到ADCREFSEL和ADCOFFTRIM
     这两个寄存器中。在调用该函数之前必须使能ADC时钟。*/

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
 函数名: CsmUnlock:
---------------------------------------------
这个函数解锁CSM，用户必须用0xFFFF取代DSP的当前密码，解锁成功返回1*/
Uint16 CsmUnlock(void) {
    volatile Uint16 temp;

    /*当前密码载入密码寄存器，0xFFFF为虚拟密码，用户必须用它来替代DSP的当前密码*/
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
    /* 如果CSM解锁，返回成功，否则失败*/
    if (CsmRegs.CSMSCR.bit.SECURE == 0) {
        temp = STATUS_SUCCESS;
    } else {
        temp = STATUS_FAIL;
    }
    return temp;
}

/*-----------------------------------------------------------------------------
系统部分初始化完成
#############################################################################

#############################################################################
外设初始化程序
-----------------------------------------------------------------------------
---------------------------------------------------------------------------
 初始化外设:
---------------------------------------------------------------------------
 该函数初始化外设到默认的状态.它调各个外设的初始化函数。
 该函数在启动或者软件复位时调用。*/

void InitPeripherals(void) {

    /* 初始化外部接口到默认状态:*/
    InitXintf();

    /* 初始化内部FLASH到默认状态:*/
    InitFlash();

    /* 初始化 GPIO*/
    InitGpio();

    /* 初始化CPU定时器到默认状态:*/
    InitCpuTimers();

    /* 初始化EPwm到默认状态:*/
    InitEPwm();

    /* 初始化ECap到默认状态:*/
    InitECap();

    /* 初始化EQep到默认状态:*/
    InitEQep();

    /* 初始化 ADC 到默认状态:*/
    InitAdc();

    /* 初始化 SCI 到默认状态:*/
    InitSci();

    /* 初始化 SPI ?认状??:*/
    InitSpi();

    /* 初始化 ECan 到默认状态:*/
    InitECan();

    /* 初始化外部中断到默认状态*/
    InitXIntrupt();

    Init_AD1210();
}

/*-------------------------------------------
 该函数初始化外部接口的
 修改XINTF寄存器时,程序务必不能在外部运行
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
 该函数初始化FLASH。 该函数必须在内部RAM中运行。在OTP/Flash将
 会产生不可预知的错误。
-------------------------------------------*/

void InitFlash(void) {
    EALLOW;
    FlashRegs.FOPT.bit.ENPIPE = 1; /* 使能Flash的流水线模式*/

    /* 最小的时间等待*/
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5; /* 设置页访问等待时间*/
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 5; /*设置随机访问等待时间*/

    FlashRegs.FOTPWAIT.bit.OTPWAIT = 8; /*设置OTP访问等待时间 */

    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    asm(" RPT #7 || NOP");
}

/*--------------------------------------------
 CpuTimer0 初始化,用作系统控制周期时基(100us)
--------------------------------------------*/

void InitCpuTimer0(void) {
    /* 初始化周期:*/
    CpuTimer0Regs.PRD.all = CPUTIMER0_PRD;
    /* 初始化预定标 (SYSCLKOUT):*/
    CpuTimer0Regs.TPR.all = 0;
    CpuTimer0Regs.TPRH.all = 0;
    /* 确保定时是停止的:*/
    CpuTimer0Regs.TCR.bit.TSS = 1;
    /* 把周期值加载到计数寄存器:*/
    CpuTimer0Regs.TCR.bit.TRB = 1;
    /* 仿真立即停止     */
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.FREE = 0;
    /* 定时器中断使能   */
    CpuTimer0Regs.TCR.bit.TIE = 1; /*0禁止中断，1使能定时器中断  */
}
void InitCpuTimers(void) { InitCpuTimer0(); }
/*-------------------------------------------
-------------------------------------------
 初始化Qep模块:
-------------------------------------------*/

void InitEQep(void) {}
/*-------------------------------------------
 初始化cap模块: 有四种操作模式
双排序器连续：SEQ1事件启动SEQ1，SEQ2事件启动SEQ2，MAXCONV0-2表示SEQ1转换序列个数，MAXCONV4-6表示SEQ2转换序列个数。
双排序器并列：SEQ1事件启动SEQ1，SEQ2事件启动SEQ2，SEQ1仅CONV00-03有效，依次转换A0、B0、A1、B1等8个通道，SEQ2仅CONV08-0B有效，MAXCONV最大设为0x33。
级联排序器连续：SEQ1事件启动16个通道，MAXCONV0-3表示转换序列个数。
级联排序器并列：SEQ1事件启动16个通道，MAXCONV0-3最大设为0x7，依次转换A0、B0、A1、B1等16个通道，结果保依次保存在result0-result15。
-------------------------------------------*/

/*-------------------------------------------
 初始化ADC模块:
-------------------------------------------*/

void InitAdc(void) {

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; /*调用校正函数前需确保全能ADC时钟*/
    ADC_cal();
    EDIS;

    AdcRegs.ADCREFSEL.bit.REF_SEL = 0; /*采用内部参考电源*/
    AdcRegs.ADCTRL3.all = 0x00E0; /* 带隙、参考电路、其他模拟电路电源上电*/
    DELAY_US(ADC_usDELAY); /* 延时*/

    AdcRegs.ADCTRL3.bit.ADCCLKPS =
        ADC_CLKPS; /* ADCCLK = HSPCLK/[2*ADCCLKPS*(ADCTRL1[7]+1)]
                      ADCTRL1[7]=0*/
    AdcRegs.ADCTRL3.bit.SMODE_SEL = SMODE; /* 1并发采样,0连续采样*/
    AdcRegs.ADCTRL1.bit.CPS = 0;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1; /* 0:双排序器模式  1:级联方式*/
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0; /* 0 启停模式，1 连续运行*/
    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK; /* 采保时间为ADC_SHCLK*ADCCLK*/

    AdcRegs.ADCTRL2.all = 0x0000;

    AdcRegs.ADCMAXCONV.all =
        0x0007; /* 转换数  2-0:SEQ1  6-4:SEQ2  3-0:SEQ(级联时）*/
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
 串口初始化函数
-------------------------------------------*/

void InitSci(void) {
    // ScicRegs.SCIFFTX.all = 0x8000;				/* 次籉IFO's 禁止FIFO*/
    // ScicRegs.SCICCR.all = 0x0007;				/*
    // 禁止效验，空闲线模式，8位方式带一个停止位*/
    ScicRegs.SCICCR.all =
        0x0027; /* 禁止效验，空闲线模式，8位方式带一个停止位,使能奇效验*/
    ScicRegs.SCICTL1.all =
        0x0003; /* 禁止接收错误中断使能，禁用休眠，使能发送和接收*/
    ScicRegs.SCICTL2.all = 0x0003; /* 使能接收中断和发送中断*/

    ScicRegs.SCIHBAUD = (SCI_BAUD >> 8) & 0x00FF; /* 加载波特率*/
    ScicRegs.SCILBAUD = (SCI_BAUD) & 0x00FF;

    ScicRegs.SCIFFTX.all = 0xC001; // 使能FIFO，禁止发送中断，发送深度1
    ScicRegs.SCIFFRX.all = 0x0001; // 禁止接收中断，接收深度1
    // ScicRegs.SCIFFRX.all=0x0021;//使能接收中断，接收深度1
    ScicRegs.SCIFFCT.all = 0x00;

    ScicRegs.SCIPRI.all = 0x0000;  /* 仿真挂起时停止 */
    ScicRegs.SCICTL1.all = 0x0023; /* 置位软复位，串口初始化完成*/

    // ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    // ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
}

/*-------------------------------------------
 该函数初始化外部中断。
-------------------------------------------*/

void InitXIntrupt(void) {
    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 10; /* Xint1 is GPIO10*/
    EDIS;

    XIntruptRegs.XINT1CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XINT1CR.bit.ENABLE = 1; /*0:禁止  1：使能*/

    XIntruptRegs.XINT2CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上??? 10：下降沿
              11：Both上升沿与下降沿*/
    XIntruptRegs.XINT2CR.bit.ENABLE = 0; /*0:禁止  1：使能*/

    XIntruptRegs.XINT3CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XINT3CR.bit.ENABLE = 0; /*0:禁止  1：使能*/

    XIntruptRegs.XINT4CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XINT4CR.bit.ENABLE = 0; /*0:禁止  1：使能*/

    XIntruptRegs.XINT5CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XINT5CR.bit.ENABLE = 0; /*0:禁止  1：使能*/

    XIntruptRegs.XINT6CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XINT6CR.bit.ENABLE = 0; /*0:禁止  1：使能*/

    XIntruptRegs.XINT7CR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XINT7CR.bit.ENABLE = 0; /*0:禁止  1：使能*/

    XIntruptRegs.XNMICR.bit.POLARITY =
        0; /*00:下降沿产生中断 01：上升沿 10：下降沿 11：Both上升沿与下降沿*/
    XIntruptRegs.XNMICR.bit.SELECT =
        1; /*0:Timer 1 connected To INT13   1：XNMI_XINT13 connected To INT13*/
    XIntruptRegs.XNMICR.bit.ENABLE = 0; /*0:禁止  1：使能*/
}

void InitECap(void) {}

/*外设初始化完成
----------------------------------------------------------------------------*/

/*===========================================================================
 No more.
===========================================================================*/
