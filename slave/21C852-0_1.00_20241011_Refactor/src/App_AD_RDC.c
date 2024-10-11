#include "F28335_Device.h"
#include "App_UserDef.h"

#pragma CODE_SECTION(Init_AD1210, "ramfuncs");
#pragma CODE_SECTION(RDC_READ, "ramfuncs");

Uint16 *pRdcAddr1 = (Uint16 *)0x00240000;
Uint16 *pRdcAddr2 = (Uint16 *)0x00260000;

/* 旋变解码器工作在配置模式，配置励磁频率，分辨率，初始化 */
void Init_AD1210(void) {
    RDC_DIS_B;
    RDC_EN_A; // 选中A旋变

    RDC_SAMPLE_H; /*SAMPLE拉高，以便在控制周期中拉低，更新寄存器*/

    RDC_A0_H;
    RDC_A1_H; /*A0,A1配置为1、1，AD2S1210工作在配置模式*/

    RDC_RES0_L;
    RDC_RES1_H; /* 分辨率为12位*/

    RDC_RESET_L;
    DELAY_US(112L);
    RDC_RESET_H;

    RDC_A0_L;
    RDC_A1_L; /* A0,A1配置为0、0，AD2S1210工作在普通模式，输出位置信号*/

    RDC_DIS_A;
    RDC_EN_B; // 选中B旋变

    RDC_SAMPLE_H; /*SAMPLE拉高，以便在控制周期中拉低，更新寄存器*/

    RDC_A0_H;
    RDC_A1_H; /*A0,A1配置为1、1，AD2S1210工作在配置模式*/

    RDC_RES0_L;
    RDC_RES1_H; /* 分辨率为12位*/

    RDC_RESET_L;
    DELAY_US(112L);
    RDC_RESET_H;

    RDC_A0_L;
    RDC_A1_L; /* A0,A1配置为0、0，AD2S1210工作在普通模式，输出位置信号*/
}

void RDC_READ(void) {
    volatile Uint16 AD1210BUF;

    SpiaRegs.SPIBRR = 0x001F; /*波特率*/
    RDC_DIS_B;
    RDC_EN_A; // 选中A旋变

    RDC_SAMPLE_L; /*SAMPLE信号拉低，更新寄存器的值*/

    DELAY_US(1L); /*延时1uS*/
    RDC_A0_L;
    RDC_A1_L; /* A0,A1配置为0、0，AD2S1210工作在普通模式，输出位置信号*/

    asm(" RPT #9 || NOP");
    /*延时60nS*/

    RDC_FSYNC_L;

    asm(" RPT #9 || NOP");
    /*延时60nS*/
    SpiaRegs.SPITXBUF = 0; /*发送伪数据*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
    SpiaRegs.SPISTS.all = 0xFF;

    rdc1.DATA_BUFFER = (AD1210BUF << 1); /* 获取位置信息*/

    SpiaRegs.SPITXBUF = 0; /*发送伪数据*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
    SpiaRegs.SPISTS.all = 0xFF; /*清除标志位*/

    /* 获取位置信息*/
    rdc1.DATA_BUFFER = (AD1210BUF << 1) + (rdc1.DATA_BUFFER << 8);
    RDC_FSYNC_H;

    DELAY_US(0.11L);

    RDC_A0_H;
    RDC_A1_H; /*A0,A1配置为1、1，AD2S1210工作在配置模式*/

    RDC_FSYNC_L;
    SpiaRegs.SPITXBUF = 0xFF00; /*发送伪数据*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
    SpiaRegs.SPISTS.all = 0xFF; /*清除标志位*/
    RDC_FSYNC_H;

    DELAY_US(0.11L);

    RDC_FSYNC_L;
    SpiaRegs.SPITXBUF = 0xFF00; /*发送伪数据*/
    while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
    AD1210BUF = SpiaRegs.SPIRXBUF & 0xFF;
    SpiaRegs.SPISTS.all = 0xFF; /*清除标志位*/
    RDC_FSYNC_H;
    rdc1.FAULT = (AD1210BUF); /* 获取位置信息*/

    RDC_SAMPLE_H; /*SAMPLE拉高，以便在下一个控制周期中拉低，更新寄存器*/
    DELAY_US(1L); /*延时1uS*/

    /*因目前泵的从控制板的旋变接线接到了B旋变，因此此处包含了BORAD_NUM == 3，
     若按正常接线泵的从控制板的旋变接线应接到A旋变，此处应去掉BORAD_NUM == 3*/
    Uint16 isValveMainCpu = (BORAD_NUM == 0 || BORAD_NUM == 4) && BC_SLT_IN == 0;
    Uint16 isValveSubCpu = (BORAD_NUM == 1 || BORAD_NUM == 5) && BC_SLT_IN == 1;
    if (isValveMainCpu || isValveSubCpu) {
        RDC_DIS_A;
        RDC_EN_B; // 选中B旋变

        RDC_SAMPLE_L; /*SAMPLE信号拉低，更新寄存器的值*/

        DELAY_US(1L); /*延时1uS*/
        RDC_A0_L;
        RDC_A1_L; /* A0,A1配置为0、0，AD2S1210工作在普通模式，输出位置信号*/

        asm(" RPT #9 || NOP");
        /*延时60nS*/

        RDC_FSYNC_L;

        asm(" RPT #9 || NOP");
        /*延时60nS*/
        SpiaRegs.SPITXBUF = 0; /*发送伪数据*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {
        }
        AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
        SpiaRegs.SPISTS.all = 0xFF;

        rdc2.DATA_BUFFER = (AD1210BUF << 1); /* 获取位置信息*/

        SpiaRegs.SPITXBUF = 0; /*发送伪数据*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
        AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
        SpiaRegs.SPISTS.all = 0xFF;                                    /*清除标志位*/
        rdc2.DATA_BUFFER = (AD1210BUF << 1) + (rdc2.DATA_BUFFER << 8); /* 获取位置信息*/
        RDC_FSYNC_H;

        DELAY_US(0.11L);

        RDC_A0_H;
        RDC_A1_H; /*A0,A1配置为1、1，AD2S1210工作在配置模式*/

        RDC_FSYNC_L;
        SpiaRegs.SPITXBUF = 0xFF00; /*发送伪数据*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
        AD1210BUF = (SpiaRegs.SPIRXBUF & 0xFF);
        SpiaRegs.SPISTS.all = 0xFF; /*清除标志位*/
        RDC_FSYNC_H;

        DELAY_US(0.11L);

        RDC_FSYNC_L;
        SpiaRegs.SPITXBUF = 0xFF00; /*发送伪数据*/
        while (SpiaRegs.SPISTS.bit.INT_FLAG != 1) {}
        AD1210BUF = SpiaRegs.SPIRXBUF & 0xFF;
        SpiaRegs.SPISTS.all = 0xFF; /*清除标志位*/
        RDC_FSYNC_H;
        rdc2.FAULT = (AD1210BUF); /* 获取位置信息*/

        if (BORAD_NUM == 3 && BC_SLT_IN == 1) {
            rdc1.DATA_BUFFER = rdc2.DATA_BUFFER;
            rdc1.FAULT = rdc2.FAULT;
        }

        RDC_SAMPLE_H; /*SAMPLE拉高，以便在下一个控制周期中拉低，更新寄存器*/
    } else {
        rdc2.FAULT = 0;
    }

    SpiaRegs.SPIBRR = 0x0037; /*波特率*/
}
