#include "F28335_Device.h"
#include "App_UserDef.h"

/*--------------------------------------------------------------------------*/

void InitECan(void) {
    /*-------------------------------------------
    初始化CANA控制器
    -------------------------------------------*/
    EALLOW;
    ECanaRegs.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANRIOC.bit.RXFUNC = 1; /*配置eCAN的RX和TX分别为eCAN的接收和发送引脚*/
    ECanaRegs.CANMC.bit.SCB = 1;      /*配置eCAN工作在增强功能，32个邮箱全部可以访问*/
    ECanaRegs.CANMC.bit.DBO = 0;      /*配置为首先发送或接收数据的最高有效位*/
    ECanaRegs.CANMC.bit.STM = 0;      /*自测试使能位，0为正常模式*/
    ECanaRegs.CANMC.bit.ABO = 1;      /*自动恢复总线*/

    ECanaRegs.CANMC.bit.CCR = 1; /*工作于初始化模式*/
    while (ECanaRegs.CANES.bit.CCE != 1) {
    } /*等待CCE位置位*/

    ECanaRegs.CANBTC.bit.BRPREG = 9; /*设置波特率为500kbps*/
    // ECanaRegs.CANBTC.bit.BRPREG=19;      /*设置波特率为250kbps*/
    ECanaRegs.CANBTC.bit.TSEG2REG = 2;
    ECanaRegs.CANBTC.bit.TSEG1REG = 10;

    ECanaRegs.CANMC.bit.CCR = 0;
    while (ECanaRegs.CANES.bit.CCE != 0) {
    } /*等待CCE位清0，等待初始化完成*/

    ECanaRegs.CANME.all = 0; /*禁止所有邮箱*/

    /*中断设置  */
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /*清所有中断标志位*/
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all; /*设置全局中断屏蔽寄存器*/
    ECanaShadow.CANGIM.bit.GIL = 0;
    ECanaShadow.CANGIM.bit.I0EN = 1;
    ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;

    ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all; /*开邮箱中断屏蔽寄存器*/
    ECanaShadow.CANMIM.all = 0x000FD7E9;
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

    ECanaShadow.CANMIL.all = ECanaRegs.CANMIL.all; /*邮箱中断产生在中断线0*/
    ECanaShadow.CANMIL.all = 0x00000000;
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;

    EDIS;

    /*CAN控制器初始化完成*/

    /*邮箱初始化*/
    ECanaRegs.CANMC.bit.CDR = 1; /*请求改变邮箱数据区*/

    /*邮箱1初始化*/
    ECanaMboxes.MBOX0.MSGID.all = 0x48C0000; /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX0.MSGID.bit.IDE = 1;     /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX0.MSGID.bit.AME = 0;     /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX0.MSGID.bit.AAM = 0;     /*自动应答位，0为标准模式，不自动应答*/

    ECanaMboxes.MBOX0.MSGCTRL.bit.TPL = 31;   /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX0.MSGCTRL.bit.RTR = 0;    /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;    /*数据长度，最大数据长度为8字节*/
                                              /*邮箱2初始化*/
    ECanaMboxes.MBOX1.MSGID.all = 0x11580000; /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX1.MSGID.bit.IDE = 1;      /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX1.MSGID.bit.AME = 0;      /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX1.MSGID.bit.AAM = 0;      /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX1.MSGCTRL.bit.TPL = 30; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX1.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/

    /*邮箱3初始化*/
    ECanaMboxes.MBOX2.MSGID.all = 0x48C0000; /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX2.MSGID.bit.IDE = 1;     /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX2.MSGID.bit.AME = 0;     /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX2.MSGID.bit.AAM = 0;     /*自动应答位，0为标准模式，不自动应答*/

    ECanaMboxes.MBOX2.MSGCTRL.bit.TPL = 29;   /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX2.MSGCTRL.bit.RTR = 0;    /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;    /*数据长度，最大数据长度为8字节*/
                                              /*邮箱4初始化*/
    ECanaMboxes.MBOX3.MSGID.all = 0x11580000; /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX3.MSGID.bit.IDE = 1;      /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX3.MSGID.bit.AME = 0;      /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX3.MSGID.bit.AAM = 0;      /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX3.MSGCTRL.bit.TPL = 28; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX3.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/

    /*邮箱5初始化*/
    ECanaMboxes.MBOX4.MSGID.all = (0x100 + BORAD_NUM); /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX4.MSGID.bit.IDE = 1;               /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX4.MSGID.bit.AME = 0;               /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX4.MSGID.bit.AAM = 0;               /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX4.MSGCTRL.bit.TPL = 27; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX4.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                            /*邮箱6初始化*/
    ECanaMboxes.MBOX5.MSGID.all = 0x100;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX5.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX5.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX5.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX5.MSGCTRL.bit.TPL = 26; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX5.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                            /*邮箱7初始化*/
    ECanaMboxes.MBOX6.MSGID.all = 0x101;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX6.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX6.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX6.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX6.MSGCTRL.bit.TPL = 25; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX6.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                            /*邮箱8初始化*/
    ECanaMboxes.MBOX7.MSGID.all = 0x102;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX7.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX7.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX7.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX7.MSGCTRL.bit.TPL = 24; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX7.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX7.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                            /*邮箱9初始化*/
    ECanaMboxes.MBOX8.MSGID.all = 0x103;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX8.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX8.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX8.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX8.MSGCTRL.bit.TPL = 23; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX8.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX8.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                            /*邮箱10初始化*/
    ECanaMboxes.MBOX9.MSGID.all = 0x104;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX9.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX9.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX9.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX9.MSGCTRL.bit.TPL = 22; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX9.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX9.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                            /*邮箱11初始化*/
    ECanaMboxes.MBOX10.MSGID.all = 0x105;   /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX10.MSGID.bit.IDE = 1;   /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX10.MSGID.bit.AME = 0;   /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX10.MSGID.bit.AAM = 0;   /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX10.MSGCTRL.bit.TPL = 21; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX10.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX10.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/

    /*邮箱12初始化*/
    ECanaMboxes.MBOX11.MSGID.all = 0x200; /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX11.MSGID.bit.IDE = 1; /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX11.MSGID.bit.AME = 0; /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX11.MSGID.bit.AAM = 0; /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX11.MSGCTRL.bit.TPL = 20; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX11.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱13初始化*/
    ECanaMboxes.MBOX12.MSGID.all = 0x200;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX12.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX12.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX12.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX12.MSGCTRL.bit.TPL = 19; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX12.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX12.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/

    /*邮箱14初始化*/
    ECanaMboxes.MBOX13.MSGID.all = (0x300 + BORAD_NUM); /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX13.MSGID.bit.IDE = 1;               /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX13.MSGID.bit.AME = 0;               /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX13.MSGID.bit.AAM = 0;               /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX13.MSGCTRL.bit.TPL = 18; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX13.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱15初始化*/
    ECanaMboxes.MBOX14.MSGID.all = 0x300;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX14.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX14.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX14.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX14.MSGCTRL.bit.TPL = 17; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX14.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX14.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱16初始化*/
    ECanaMboxes.MBOX15.MSGID.all = 0x301;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX15.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX15.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX15.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX15.MSGCTRL.bit.TPL = 16; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX15.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX15.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱17初始化*/
    ECanaMboxes.MBOX16.MSGID.all = 0x302;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX16.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX16.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX16.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX16.MSGCTRL.bit.TPL = 15; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱18初始化*/
    ECanaMboxes.MBOX17.MSGID.all = 0x303;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX17.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX17.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX17.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX17.MSGCTRL.bit.TPL = 14; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX17.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX17.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱19初始化*/
    ECanaMboxes.MBOX18.MSGID.all = 0x304;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX18.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX18.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX18.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX18.MSGCTRL.bit.TPL = 13; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX18.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX18.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/
                                             /*邮箱20初始化*/
    ECanaMboxes.MBOX19.MSGID.all = 0x305;    /*29位或11位邮箱ID,标准帧11位使用ID[28-18],扩展帧29位ID[28-0]*/
    ECanaMboxes.MBOX19.MSGID.bit.IDE = 1;    /*扩展标识符，为1时采用扩展帧*/
    ECanaMboxes.MBOX19.MSGID.bit.AME = 0;    /*接收邮箱屏蔽使能位，0为不使能屏蔽，该位只在接收邮箱有效*/
    ECanaMboxes.MBOX19.MSGID.bit.AAM = 0;    /*自动Υ鹞唬?为标准模式，不自动应答*/

    ECanaMboxes.MBOX19.MSGCTRL.bit.TPL = 12; /*发送优先级5位，数越大优先级越高*/
    ECanaMboxes.MBOX19.MSGCTRL.bit.RTR = 0;  /*远程发送请求位，0为没有远程帧请求*/
    ECanaMboxes.MBOX19.MSGCTRL.bit.DLC = 8;  /*数据长度，最大数据长度为8字节*/

    /*配置邮箱为发送或接收邮箱：*/
    ECanaRegs.CANMD.bit.MD0 = 1; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD1 = 0; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD2 = 0; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD3 = 1; /*1为接收邮箱，0为发送邮箱*/

    ECanaRegs.CANMD.bit.MD4 = 0;  /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD5 = 1;  /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD6 = 1;  /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD7 = 1;  /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD8 = 1;  /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD9 = 1;  /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD10 = 1; /*1为接收邮箱，0为发送邮箱*/

    ECanaRegs.CANMD.bit.MD11 = 0; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD12 = 1; /*1为接收邮箱，0为发送邮箱*/

    ECanaRegs.CANMD.bit.MD13 = 0; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD14 = 1; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD15 = 1; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD16 = 1; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD17 = 1; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD18 = 1; /*1为接收邮箱，0为发送邮箱*/
    ECanaRegs.CANMD.bit.MD19 = 1; /*1为接收邮箱，0为发送邮箱*/
                                  /*使能邮箱*/
    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.bit.ME0 = 1;
    ECanaShadow.CANME.bit.ME1 = 1;
    ECanaShadow.CANME.bit.ME2 = 1;
    ECanaShadow.CANME.bit.ME3 = 1;

    ECanaShadow.CANME.bit.ME4 = 1;
    ECanaShadow.CANME.bit.ME5 = 1;
    ECanaShadow.CANME.bit.ME6 = 1;
    ECanaShadow.CANME.bit.ME7 = 1;
    ECanaShadow.CANME.bit.ME8 = 1;
    ECanaShadow.CANME.bit.ME9 = 1;
    ECanaShadow.CANME.bit.ME10 = 1;

    ECanaShadow.CANME.bit.ME11 = 1;
    ECanaShadow.CANME.bit.ME12 = 1;

    ECanaShadow.CANME.bit.ME13 = 1;
    ECanaShadow.CANME.bit.ME14 = 1;
    ECanaShadow.CANME.bit.ME15 = 1;
    ECanaShadow.CANME.bit.ME16 = 1;
    ECanaShadow.CANME.bit.ME17 = 1;
    ECanaShadow.CANME.bit.ME18 = 1;
    ECanaShadow.CANME.bit.ME19 = 1;
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}

/*===========================================================================
 No more.
===========================================================================*/
