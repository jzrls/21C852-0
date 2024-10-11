/*
=================================================================================
 文件名:     	App_SPI.c   
                 
 功能描述 :   操作SPI25040函数
				 
 编写者:	 伍泽东
 编写日期: 2010/06/08
        
=================================================================================
*/

/*--------------------------------------------------------------------------*/
#include "F28335_Device.h"     /* F28335 外设头文件*/
#include "App_UserDef.h"      /* F28335 应用项目头文件*/
/*--------------------------------------------------------------------------*/

#pragma CODE_SECTION(RDSR_X25040, "ramfuncs");
#pragma CODE_SECTION(WREN_X25040, "ramfuncs");
#pragma CODE_SECTION(WRSR_X25040, "ramfuncs");
#pragma CODE_SECTION(WRITE_X25040, "ramfuncs");
#pragma CODE_SECTION(READ_X25040, "ramfuncs");
#pragma CODE_SECTION(WRDI_X25040, "ramfuncs");
#pragma CODE_SECTION(InitSpi, "ramfuncs");


Uint16 SRBUF;
Uint16 RxBUF;
Uint16 SPIBUF;

void InitSpi(void)
{
   SpiaRegs.SPICCR.bit.SPISWRESET = 0;                /*复位、上升沿，8位字符长度*/

   SpiaRegs.SPICCR.all=0x0007;                        /*复位、上升沿，8位字符长度*/
   SpiaRegs.SPICTL.all=0x000E;                        /*0000 1110 有延时，主方式，禁止SPI中断*/
   SpiaRegs.SPIBRR=0x0037;                            /*波特率*/
   SpiaRegs.SPIPRI.all=0x0040;                            /*低中断优先级*/
   SpiaRegs.SPICCR.bit.SPISWRESET = 1;                       
}

Uint16 RDSR_X25040(void)
{

   
	EN_X25040 = 1;
   DELAY_US(0.8L);       /*延时800nS*/

   SpiaRegs.SPITXBUF =0x0500;                         /*发送读命令*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}           
   RxBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                              /*清除标志位*/
   
   SpiaRegs.SPITXBUF=0;                               /*发送伪数据*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SRBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;
   DIS_X25040 = 1;
   return SRBUF;
}

void WREN_X25040(void)                                /*写使能*/
{  

   DELAY_US(0.1L);       
	EN_X25040 = 1;
   SpiaRegs.SPITXBUF = 0x0600;                         /*发送写使能命令*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
       SRBUF=SpiaRegs.SPIRXBUF;
       SpiaRegs.SPISTS.all=0xFF;                         /*清除标志位*/
   DIS_X25040 = 1;
   DELAY_US(0.1L);       
}

void WRDI_X25040(void)                                /*写使能*/
{  
   DELAY_US(0.1L);       
	EN_X25040 = 1;
   SpiaRegs.SPITXBUF = 0x0400;                         /*发送写使能命令*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
       SRBUF=SpiaRegs.SPIRXBUF;
       SpiaRegs.SPISTS.all=0xFF;                         /*清除标志位*/
   DIS_X25040 = 1;
   DELAY_US(0.1L);       
}

void WRSR_X25040(Uint16 COM)                       /*写状态*/
{
   WREN_X25040();                                        /*写使能*/
	EN_X25040 = 1;
   SpiaRegs.SPITXBUF = COM | 0x0100;                        /*发送写状态命令*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
       SRBUF=SpiaRegs.SPIRXBUF;
       SpiaRegs.SPISTS.all=0xFF;                             /*清除标志位*/
   DIS_X25040 = 1;

   WRDI_X25040();                                        /*写使能*/
}

/*写EEPROM  RA_ADDR为RAM数据首地址，EEP_ADDR为EEP地址，N<4个数8位，H为8选择高地址*/
void WRITE_X25040(Uint16 *RA_ADDR,Uint16 EEP_ADDR,Uint16 N,Uint16 H)
{
   Uint16 I;
   SRBUF=RDSR_X25040();
   while((SRBUF & 0x0001) != 0x0000)
   {
   		SRBUF=RDSR_X25040();
   }        /*等待内部写结束*/
   WREN_X25040();      
   EN_X25040 = 1;
   SPIBUF=((H | 0x02) << 8);
   SpiaRegs.SPITXBUF = SPIBUF;                                  /*发送写命令*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                    /*清除标志位*/
   SpiaRegs.SPITXBUF = (EEP_ADDR<< 8 );                         /*发送地址*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                 /*清除标志位*/
   for (I=0;I<N;I++)
   {
      SpiaRegs.SPITXBUF =( *(RA_ADDR+I) << 8);                       /*发送数据*/
      while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
      SPIBUF=SpiaRegs.SPIRXBUF;
      SpiaRegs.SPISTS.all=0xFF;                          /*清除标志位*/

   }
   DIS_X25040 = 1;
}

/*读EEPROM RA_ADDR为RAM数据首地址，EEP_ADDR为EEP地址，N为字符个数8位，H为8选择高地址*/
void READ_X25040(Uint16 *RA_ADDR,Uint16 EEP_ADDR,Uint16 N,Uint16 H)
{
   Uint16 I;
   SRBUF=RDSR_X25040();
   /*等待内部写结束*/
   while ((SRBUF & 0x0001) != 0x0000)
   {
   		SRBUF=RDSR_X25040();
   }           
   EN_X25040 = 1;
   SpiaRegs.SPITXBUF = (H | 0x03) << 8;                          /*发送读命令*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                     /*清除标志位*/
   SpiaRegs.SPITXBUF = EEP_ADDR<< 8;                             /*发送地址*/ 
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                    /*清除标志位*/
   for (I=0;I<N;I++)
   {
      SpiaRegs.SPITXBUF = 0;                                     /*发送伪数据*/
      while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
      *(RA_ADDR+I) = SpiaRegs.SPIRXBUF & 0xff;
      SpiaRegs.SPISTS.all=0xFF;                                /*清除标志位*/

   }
   DIS_X25040 = 1;
}





/*===========================================================================
 No more.
===========================================================================*/
