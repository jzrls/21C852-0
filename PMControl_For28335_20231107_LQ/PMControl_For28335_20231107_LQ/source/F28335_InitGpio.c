/*--------------------------------------------------------------------------*/
#include "F28335_Device.h"     /* F28335 外设头文件*/
#include "App_UserDef.h"      /* F28335 应用项目头文件*/
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 该函数初始化GPIO,  正确配置IO
 - 特殊功能按照要求配置(GPAMUX)
 GPxPUD：0:Enable pullup  1:Disable；除EPWM口外均可使能（默认状态）
 GPxDIR: 0:input  1:output
 GPxQSELx配置：0:Synch to SYSCLOUT(默认)  1:3*SYSCLOUT Sample  3:6*SYSCLOUT Sample 3:Asynch(仅对功能口有效）;
				SCI、CAN输入脚均为异步；SPI脚均为异步；_TZx脚为异步，ECAP、EQEP均为同步
				数据总线配置为异步，地址总线不用
				与GPxCTRL配合得实际采样窗口为:2*2*GPxCTRL[QUALPRDn]*SYSCLKOUT（GPxQSELx=1） 或 5*2*GPxCTRL[QUALPRDx]*SYSCLKOUT（GPxQSELx=2）
----------------------------------------------------------------------------*/
void InitGpio(void)
{
   EALLOW;
/*=========================================================================
GPIOA0-15初始化配置
=========================================================================*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;  /* 00:GPIO0  01:EPWM1A  10:XX  11:XX*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;  /*  00:GPIO1  01:EPWM1B  10:ECAP6  11:MFSRB*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;  /*  00:GPIO2  01:EPWM2A  10:XX  11:XX*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;  /*  00:GPIO3  01:EPWM2B  10:ECAP5  11:MCLKRB*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;  /*  00:GPIO4  01:EPWM3A  10:XX  11:XX*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;  /*  00:GPIO5  01:EPWM3B  10:MFSRA  11:ECAP1*/

   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;   /*0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;  /*  00:GPIO6  01:EPWM4A  10:EPWMSYNCI  11:EPWMSYNCO*/
   GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;   /* 0:input  1:output*/
   GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 2; /* Synch to SYSCLOUT*/

   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;   /*0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;  /*  00:GPIO7  01:EPWM4B  10:MCLKRA  11:ECAP2*/
   GpioCtrlRegs.GPADIR.bit.GPIO7 = 0;   /* 0:input  1:output*/
   GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 2; /* Synch to SYSCLOUT*/
   
   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   /*0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  /*  00:GPIO8  01:EPWM5A  10:CANTXB  11:_ADCSOCAO*/
   GpioCtrlRegs.GPADIR.bit.GPIO8 = 0;   /* 0:input  1:output*/
   GpioCtrlRegs.GPAQSEL1.bit.GPIO8 = 2; /* Synch to SYSCLOUT*/
   
   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;   /*0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO9 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;  /* 00:GPIO9  01:EPWM5B  10:SCITXDB  11:ECAP3*/
   GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;   /* 0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;   /*0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;  /* 00:GPIO10  01:EPWM6A  10:CANRXB  11:_ADCSOCB0*/
   GpioCtrlRegs.GPADIR.bit.GPIO10 = 0;   /* 0:input  1:output*/
   GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 2; /* Synch to SYSCLOUT*/

   GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;   /*0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;  /* 00:GPIO11  01:EPWM6B  10:SCIRXDB  11:ECAP4*/
   GpioCtrlRegs.GPADIR.bit.GPIO11 = 0;   /* 0:input  1:output*/
   GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 2; /* Synch to SYSCLOUT*/

   GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   /*0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO12 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  /* 00:GPIO12  01:_TZ1  10:CANTXB  11:MDXB*/
   GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;   /* 0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;  /* 00:GPIO13  01:_TZ2  10:CANRXB  11:MDRB*/
   GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;   /*  0:input  1:output*/
   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 2; /* Synch to SYSCLOUT*/

   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;  /* 00:GPIO14  01:_TZ3/XHOLD  10:SCITXDB  11:MCLKXB*/
   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;  /* 00:GPIO15  01:_TZ4/XHOLDA  10:SCIRXDB  11:MFSXB*/

   GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0x19;	

/*=========================================================================
GPIOA16-31初始化配置
=========================================================================*/

   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO16 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;  /* 00:GPIO16  01:SPISIMOA  10:CANTXB  11:_TZ5*/
   GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO17 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;  /* 00:GPIO17  01:SPIS0MIA  10:CANRXB  11:_TZ6*/
   GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  /* 00:GPIO18  01:SPICLKA  10:SCITXDB  11:CANRXA*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;  /* 00:GPIO19  01:_SPISTEA  10:SCIRXDB  11:CANTXA*/

   GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO20 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;  /* 00:GPIO20  01:EQEP1A  10:MDXA  11:CANTXB*/
   GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   /*0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO21 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;  /*  00:GPIO21  01:EQEP1B  10:MDRA  11:CANRXB*/
   GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO22 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;  /* 00:GPIO22  01:EQEP1S  10:MCLKXA  11:SCITXDB*/
   GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;   /*  0:input  1:output*/
   
   GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO23 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;  /* 00:GPIO23  01:EQEP1I  10:MFSXA  11:SCIRXDB*/
   GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO24 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;  /* 00:GPIO24  01:ECAP1  10:EQEP2A  11:MDXB*/
   GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;  /* 00:GPIO25  01:ECAP2  10:EQEP2B  11:MDRB*/
   GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;   /*  0:input  1:output*/
   GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 2; /* Synch to SYSCLOUT*/

   GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  /* 00:GPIO26  01:ECAP3  10:EQEP2I  11:MCLKXB*/
   GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;   /*  0:input  1:output*/
   GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 2; /* Synch to SYSCLOUT*/

   GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPASET.bit.GPIO27 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;  /* 00:GPIO27  01:ECAP4  10:EQEP2S  11:MFSXB*/
   GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;  /* 00:GPIO28  01:SCIRXDA  10:_XZCS6  11:XX*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;  /*  00:GPIO29  01:SCITXDA  10:XA19  11:XX*/

   GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 2;  /* 00:GPIO30  01:CANRXA  10:XA18  11:XX*/
   GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 2;  /* 00:GPIO31  01:CANTXA  10:XA17  11:XX*/


/*=========================================================================
GPIOB32-47初始化配置
=========================================================================*/

   GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO32 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;  /* 00:GPIO32  01:SDAA  10:EPWMSYNCI  11:_ADCSOCAO*/
   GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO33 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;  /* 00:GPIO33  01:SCLA  10:EPWMSYNCO  11:_ADCSOCBO*/
   GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 2;  /* 00:GPIO34  01:ECAP1  10:XREADY  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 2;  /*00:GPIO35  01:SCITXDA  10:XR/_W  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 2;  /* 00:GPIO36  01:SCIRXDA  10:_XZCS0  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 2;  /* 00:GPIO37  01:ECAP2  10:_XZCS7  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 2;  /* 00:GPIO38  01:XX  10:_XWE0  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 2;  /* 00:GPIO39  01:XX  10:XA16  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 2;  /* 00:GPIO40  01:XX  10:XA0/_XWE1  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 2;  /* 00:GPIO41  01:XX  10:XA1  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 2;  /* 00:GPIO42  01:XX  10:XA2  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 2;  /* 00:GPIO43  01:XX  10:XA3  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 2;  /*  00:GPIO44  01:XX  10:XA4  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 2;  /*  00:GPIO45  01:XX  10:XA5  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 2;  /*  00:GPIO46  01:XX  10:XA6  11:同10*/
   GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 2;  /*  00:GPIO47  01:XX  10:XA7  11:同10*/

/*=========================================================================
GPIOB48-63初始化配置
=========================================================================*/

   GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;   /*0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO48 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;  /*  00:GPIO48  01:ECAP5  10:XD31  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO49 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  /* 00:GPIO49  01:ECAP6  10:XD30  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO50 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  /* 00:GPIO50  01:EQEP1A  10:XD29  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO51 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  /* 00:GPIO51  01:EQEP1B  10:XD28  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO52 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO52 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  /* 00:GPIO52  01:EQEP1S  10:XD27  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;   /* 0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO53 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;  /*  00:GPIO53  01:EQEP1I  10:XD26  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;   /* 0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;  /* 00:GPIO54  01:SPISIMOA  10:XD25  11:同10*/
   GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; /* ASynch to SYSCLOUT*/

   GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;  /* 00:GPIO55  01:SPISOMIA  10:XD24  11:同10*/
   GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; /* ASynch to SYSCLOUT*/

   GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;  /* 00:GPIO56  01:SPICLKA  10:XD23  11:同10*/
   GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; /* ASynch to SYSCLOUT*/

   GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO57 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;  /* 00:GPIO57  01:_SPISTEA  10:XD22  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO58 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;  /* 00:GPIO58  01:MCLKRA  10:XD21  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO59 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;  /* 00:GPIO59  01:MFSRA  10:XD20  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO60 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;  /* 00:GPIO60  01:MCLKRB  10:XD19  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;   /* 0:Enable pullup  1:Disable*/
   GpioDataRegs.GPBSET.bit.GPIO61 = 1;   /* Load output latch*/
   GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;  /* 00:GPIO61  01:MFSRB  10:XD18  11:同10*/
   GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;   /*  0:input  1:output*/

   GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;  /* 00:GPIO62  01:SCIRXDC  10:XD17  11:同10*/
  GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;  /* 00:GPIO63  01:SCITXDC  10:XD16  11:同10*/

/*=========================================================================
GPIOC64-79初始化配置
=========================================================================*/
   GpioCtrlRegs.GPCMUX1.all = 0xFFFFFFFF;     /*功能口*/
   GpioCtrlRegs.GPCMUX2.all = 0xFFFFFFFF;	  /*功能口*/
   EDIS;

}
