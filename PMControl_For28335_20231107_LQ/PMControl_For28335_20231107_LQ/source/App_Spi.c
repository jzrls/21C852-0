/*
=================================================================================
 �ļ���:     	App_SPI.c   
                 
 �������� :   ����SPI25040����
				 
 ��д��:	 ����
 ��д����: 2010/06/08
        
=================================================================================
*/

/*--------------------------------------------------------------------------*/
#include "F28335_Device.h"     /* F28335 ����ͷ�ļ�*/
#include "App_UserDef.h"      /* F28335 Ӧ����Ŀͷ�ļ�*/
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
   SpiaRegs.SPICCR.bit.SPISWRESET = 0;                /*��λ�������أ�8λ�ַ�����*/

   SpiaRegs.SPICCR.all=0x0007;                        /*��λ�������أ�8λ�ַ�����*/
   SpiaRegs.SPICTL.all=0x000E;                        /*0000 1110 ����ʱ������ʽ����ֹSPI�ж�*/
   SpiaRegs.SPIBRR=0x0037;                            /*������*/
   SpiaRegs.SPIPRI.all=0x0040;                            /*���ж����ȼ�*/
   SpiaRegs.SPICCR.bit.SPISWRESET = 1;                       
}

Uint16 RDSR_X25040(void)
{

   
	EN_X25040 = 1;
   DELAY_US(0.8L);       /*��ʱ800nS*/

   SpiaRegs.SPITXBUF =0x0500;                         /*���Ͷ�����*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}           
   RxBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                              /*�����־λ*/
   
   SpiaRegs.SPITXBUF=0;                               /*����α����*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SRBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;
   DIS_X25040 = 1;
   return SRBUF;
}

void WREN_X25040(void)                                /*дʹ��*/
{  

   DELAY_US(0.1L);       
	EN_X25040 = 1;
   SpiaRegs.SPITXBUF = 0x0600;                         /*����дʹ������*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
       SRBUF=SpiaRegs.SPIRXBUF;
       SpiaRegs.SPISTS.all=0xFF;                         /*�����־λ*/
   DIS_X25040 = 1;
   DELAY_US(0.1L);       
}

void WRDI_X25040(void)                                /*дʹ��*/
{  
   DELAY_US(0.1L);       
	EN_X25040 = 1;
   SpiaRegs.SPITXBUF = 0x0400;                         /*����дʹ������*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
       SRBUF=SpiaRegs.SPIRXBUF;
       SpiaRegs.SPISTS.all=0xFF;                         /*�����־λ*/
   DIS_X25040 = 1;
   DELAY_US(0.1L);       
}

void WRSR_X25040(Uint16 COM)                       /*д״̬*/
{
   WREN_X25040();                                        /*дʹ��*/
	EN_X25040 = 1;
   SpiaRegs.SPITXBUF = COM | 0x0100;                        /*����д״̬����*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
       SRBUF=SpiaRegs.SPIRXBUF;
       SpiaRegs.SPISTS.all=0xFF;                             /*�����־λ*/
   DIS_X25040 = 1;

   WRDI_X25040();                                        /*дʹ��*/
}

/*дEEPROM  RA_ADDRΪRAM�����׵�ַ��EEP_ADDRΪEEP��ַ��N<4����8λ��HΪ8ѡ��ߵ�ַ*/
void WRITE_X25040(Uint16 *RA_ADDR,Uint16 EEP_ADDR,Uint16 N,Uint16 H)
{
   Uint16 I;
   SRBUF=RDSR_X25040();
   while((SRBUF & 0x0001) != 0x0000)
   {
   		SRBUF=RDSR_X25040();
   }        /*�ȴ��ڲ�д����*/
   WREN_X25040();      
   EN_X25040 = 1;
   SPIBUF=((H | 0x02) << 8);
   SpiaRegs.SPITXBUF = SPIBUF;                                  /*����д����*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                    /*�����־λ*/
   SpiaRegs.SPITXBUF = (EEP_ADDR<< 8 );                         /*���͵�ַ*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                 /*�����־λ*/
   for (I=0;I<N;I++)
   {
      SpiaRegs.SPITXBUF =( *(RA_ADDR+I) << 8);                       /*��������*/
      while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
      SPIBUF=SpiaRegs.SPIRXBUF;
      SpiaRegs.SPISTS.all=0xFF;                          /*�����־λ*/

   }
   DIS_X25040 = 1;
}

/*��EEPROM RA_ADDRΪRAM�����׵�ַ��EEP_ADDRΪEEP��ַ��NΪ�ַ�����8λ��HΪ8ѡ��ߵ�ַ*/
void READ_X25040(Uint16 *RA_ADDR,Uint16 EEP_ADDR,Uint16 N,Uint16 H)
{
   Uint16 I;
   SRBUF=RDSR_X25040();
   /*�ȴ��ڲ�д����*/
   while ((SRBUF & 0x0001) != 0x0000)
   {
   		SRBUF=RDSR_X25040();
   }           
   EN_X25040 = 1;
   SpiaRegs.SPITXBUF = (H | 0x03) << 8;                          /*���Ͷ�����*/
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                     /*�����־λ*/
   SpiaRegs.SPITXBUF = EEP_ADDR<< 8;                             /*���͵�ַ*/ 
   while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
   SPIBUF=SpiaRegs.SPIRXBUF;
   SpiaRegs.SPISTS.all=0xFF;                                    /*�����־λ*/
   for (I=0;I<N;I++)
   {
      SpiaRegs.SPITXBUF = 0;                                     /*����α����*/
      while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){}
      *(RA_ADDR+I) = SpiaRegs.SPIRXBUF & 0xff;
      SpiaRegs.SPISTS.all=0xFF;                                /*�����־λ*/

   }
   DIS_X25040 = 1;
}





/*===========================================================================
 No more.
===========================================================================*/
