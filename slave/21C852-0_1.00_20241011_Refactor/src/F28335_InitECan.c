#include "F28335_Device.h"
#include "App_UserDef.h"

/*--------------------------------------------------------------------------*/

void InitECan(void) {
    /*-------------------------------------------
    ��ʼ��CANA������
    -------------------------------------------*/
    EALLOW;
    ECanaRegs.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANRIOC.bit.RXFUNC = 1; /*����eCAN��RX��TX�ֱ�ΪeCAN�Ľ��պͷ�������*/
    ECanaRegs.CANMC.bit.SCB = 1;      /*����eCAN��������ǿ���ܣ�32������ȫ�����Է���*/
    ECanaRegs.CANMC.bit.DBO = 0;      /*����Ϊ���ȷ��ͻ�������ݵ������Чλ*/
    ECanaRegs.CANMC.bit.STM = 0;      /*�Բ���ʹ��λ��0Ϊ����ģʽ*/
    ECanaRegs.CANMC.bit.ABO = 1;      /*�Զ��ָ�����*/

    ECanaRegs.CANMC.bit.CCR = 1; /*�����ڳ�ʼ��ģʽ*/
    while (ECanaRegs.CANES.bit.CCE != 1) {
    } /*�ȴ�CCEλ��λ*/

    ECanaRegs.CANBTC.bit.BRPREG = 9; /*���ò�����Ϊ500kbps*/
    // ECanaRegs.CANBTC.bit.BRPREG=19;      /*���ò�����Ϊ250kbps*/
    ECanaRegs.CANBTC.bit.TSEG2REG = 2;
    ECanaRegs.CANBTC.bit.TSEG1REG = 10;

    ECanaRegs.CANMC.bit.CCR = 0;
    while (ECanaRegs.CANES.bit.CCE != 0) {
    } /*�ȴ�CCEλ��0���ȴ���ʼ�����*/

    ECanaRegs.CANME.all = 0; /*��ֹ��������*/

    /*�ж�����  */
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /*�������жϱ�־λ*/
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all; /*����ȫ���ж����μĴ���*/
    ECanaShadow.CANGIM.bit.GIL = 0;
    ECanaShadow.CANGIM.bit.I0EN = 1;
    ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;

    ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all; /*�������ж����μĴ���*/
    ECanaShadow.CANMIM.all = 0x000FD7E9;
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

    ECanaShadow.CANMIL.all = ECanaRegs.CANMIL.all; /*�����жϲ������ж���0*/
    ECanaShadow.CANMIL.all = 0x00000000;
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;

    EDIS;

    /*CAN��������ʼ�����*/

    /*�����ʼ��*/
    ECanaRegs.CANMC.bit.CDR = 1; /*����ı�����������*/

    /*����1��ʼ��*/
    ECanaMboxes.MBOX0.MSGID.all = 0x48C0000; /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX0.MSGID.bit.IDE = 1;     /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX0.MSGID.bit.AME = 0;     /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX0.MSGID.bit.AAM = 0;     /*�Զ�Ӧ��λ��0Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX0.MSGCTRL.bit.TPL = 31;   /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX0.MSGCTRL.bit.RTR = 0;    /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;    /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                              /*����2��ʼ��*/
    ECanaMboxes.MBOX1.MSGID.all = 0x11580000; /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX1.MSGID.bit.IDE = 1;      /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX1.MSGID.bit.AME = 0;      /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX1.MSGID.bit.AAM = 0;      /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX1.MSGCTRL.bit.TPL = 30; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX1.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/

    /*����3��ʼ��*/
    ECanaMboxes.MBOX2.MSGID.all = 0x48C0000; /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX2.MSGID.bit.IDE = 1;     /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX2.MSGID.bit.AME = 0;     /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX2.MSGID.bit.AAM = 0;     /*�Զ�Ӧ��λ��0Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX2.MSGCTRL.bit.TPL = 29;   /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX2.MSGCTRL.bit.RTR = 0;    /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;    /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                              /*����4��ʼ��*/
    ECanaMboxes.MBOX3.MSGID.all = 0x11580000; /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX3.MSGID.bit.IDE = 1;      /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX3.MSGID.bit.AME = 0;      /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX3.MSGID.bit.AAM = 0;      /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX3.MSGCTRL.bit.TPL = 28; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX3.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/

    /*����5��ʼ��*/
    ECanaMboxes.MBOX4.MSGID.all = (0x100 + BORAD_NUM); /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX4.MSGID.bit.IDE = 1;               /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX4.MSGID.bit.AME = 0;               /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX4.MSGID.bit.AAM = 0;               /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX4.MSGCTRL.bit.TPL = 27; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX4.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                            /*����6��ʼ��*/
    ECanaMboxes.MBOX5.MSGID.all = 0x100;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX5.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX5.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX5.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX5.MSGCTRL.bit.TPL = 26; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX5.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                            /*����7��ʼ��*/
    ECanaMboxes.MBOX6.MSGID.all = 0x101;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX6.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX6.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX6.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX6.MSGCTRL.bit.TPL = 25; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX6.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                            /*����8��ʼ��*/
    ECanaMboxes.MBOX7.MSGID.all = 0x102;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX7.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX7.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX7.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX7.MSGCTRL.bit.TPL = 24; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX7.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX7.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                            /*����9��ʼ��*/
    ECanaMboxes.MBOX8.MSGID.all = 0x103;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX8.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX8.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX8.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX8.MSGCTRL.bit.TPL = 23; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX8.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX8.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                            /*����10��ʼ��*/
    ECanaMboxes.MBOX9.MSGID.all = 0x104;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX9.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX9.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX9.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX9.MSGCTRL.bit.TPL = 22; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX9.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX9.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                            /*����11��ʼ��*/
    ECanaMboxes.MBOX10.MSGID.all = 0x105;   /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX10.MSGID.bit.IDE = 1;   /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX10.MSGID.bit.AME = 0;   /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX10.MSGID.bit.AAM = 0;   /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX10.MSGCTRL.bit.TPL = 21; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX10.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX10.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/

    /*����12��ʼ��*/
    ECanaMboxes.MBOX11.MSGID.all = 0x200; /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX11.MSGID.bit.IDE = 1; /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX11.MSGID.bit.AME = 0; /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX11.MSGID.bit.AAM = 0; /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX11.MSGCTRL.bit.TPL = 20; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX11.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����13��ʼ��*/
    ECanaMboxes.MBOX12.MSGID.all = 0x200;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX12.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX12.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX12.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX12.MSGCTRL.bit.TPL = 19; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX12.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX12.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/

    /*����14��ʼ��*/
    ECanaMboxes.MBOX13.MSGID.all = (0x300 + BORAD_NUM); /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX13.MSGID.bit.IDE = 1;               /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX13.MSGID.bit.AME = 0;               /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX13.MSGID.bit.AAM = 0;               /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX13.MSGCTRL.bit.TPL = 18; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX13.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����15��ʼ��*/
    ECanaMboxes.MBOX14.MSGID.all = 0x300;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX14.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX14.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX14.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX14.MSGCTRL.bit.TPL = 17; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX14.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX14.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����16��ʼ��*/
    ECanaMboxes.MBOX15.MSGID.all = 0x301;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX15.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX15.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX15.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX15.MSGCTRL.bit.TPL = 16; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX15.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX15.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����17��ʼ��*/
    ECanaMboxes.MBOX16.MSGID.all = 0x302;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX16.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX16.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX16.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX16.MSGCTRL.bit.TPL = 15; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����18��ʼ��*/
    ECanaMboxes.MBOX17.MSGID.all = 0x303;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX17.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX17.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX17.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX17.MSGCTRL.bit.TPL = 14; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX17.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX17.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����19��ʼ��*/
    ECanaMboxes.MBOX18.MSGID.all = 0x304;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX18.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX18.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX18.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX18.MSGCTRL.bit.TPL = 13; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX18.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX18.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/
                                             /*����20��ʼ��*/
    ECanaMboxes.MBOX19.MSGID.all = 0x305;    /*29λ��11λ����ID,��׼֡11λʹ��ID[28-18],��չ֡29λID[28-0]*/
    ECanaMboxes.MBOX19.MSGID.bit.IDE = 1;    /*��չ��ʶ����Ϊ1ʱ������չ֡*/
    ECanaMboxes.MBOX19.MSGID.bit.AME = 0;    /*������������ʹ��λ��0Ϊ��ʹ�����Σ���λֻ�ڽ���������Ч*/
    ECanaMboxes.MBOX19.MSGID.bit.AAM = 0;    /*�Զ����λ�?Ϊ��׼ģʽ�����Զ�Ӧ��*/

    ECanaMboxes.MBOX19.MSGCTRL.bit.TPL = 12; /*�������ȼ�5λ����Խ�����ȼ�Խ��*/
    ECanaMboxes.MBOX19.MSGCTRL.bit.RTR = 0;  /*Զ�̷�������λ��0Ϊû��Զ��֡����*/
    ECanaMboxes.MBOX19.MSGCTRL.bit.DLC = 8;  /*���ݳ��ȣ�������ݳ���Ϊ8�ֽ�*/

    /*��������Ϊ���ͻ�������䣺*/
    ECanaRegs.CANMD.bit.MD0 = 1; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD1 = 0; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD2 = 0; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD3 = 1; /*1Ϊ�������䣬0Ϊ��������*/

    ECanaRegs.CANMD.bit.MD4 = 0;  /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD5 = 1;  /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD6 = 1;  /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD7 = 1;  /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD8 = 1;  /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD9 = 1;  /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD10 = 1; /*1Ϊ�������䣬0Ϊ��������*/

    ECanaRegs.CANMD.bit.MD11 = 0; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD12 = 1; /*1Ϊ�������䣬0Ϊ��������*/

    ECanaRegs.CANMD.bit.MD13 = 0; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD14 = 1; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD15 = 1; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD16 = 1; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD17 = 1; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD18 = 1; /*1Ϊ�������䣬0Ϊ��������*/
    ECanaRegs.CANMD.bit.MD19 = 1; /*1Ϊ�������䣬0Ϊ��������*/
                                  /*ʹ������*/
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
