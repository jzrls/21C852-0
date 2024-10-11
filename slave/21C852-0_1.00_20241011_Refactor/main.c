#include "F28335_Device.h"
#include "App_UserDef.h"
#include "App_Main.h"

#include "src\core\action.h"
#include "src\core\command.h"
#include "src\task\power_on_initialization.h"

void main(void) {
    volatile Uint16 Data_Temp = 0;
    volatile _iq Set_Temp = 0;

    powerOnInitialization();

    for (;;) {
        /* 硬件保护？*/
        hardwareProtection();

        /* ? */
        if (Sys_Flag.bit.STOP_PWM_Flag_Driv == 0) {
            EALLOW;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm2Regs.TZCLR.bit.INT = 1;
            EDIS;
        }

        /* 串口通讯 */
        ScibRxCheck();
        ScibTxCheck();

        /* 上位机参数写入E2PROM */
        writeParamsIntoEeprom();

        /* 指令应答 */
        if (SCIB.FLAG.bit.RX_FLAG == 1) {
            Uint16 command = SCI_RX_BUFFER[2];
            if (command == SELF_CHECK_COMMAND) {
                selfCheckResponse();
            } else if (command == CONTROL_COMMAND) {
                controlResponse();
            } else if (command == TELEMETERING_COMMAND) {
                telemeteringResponse();
            } else {
                otherResponse();
            }
            SCIB.FLAG.bit.RX_FLAG = 0; // 接收命令处理完毕应答
        }

        if (CANB.FLAG.bit.RX_FLAG == 1) {
            canReceiveHandle();

            if (Board_Num == BORAD_NUM) {
                Can_Deal(); /* 自定义指令 */
            }
            CANB.FLAG.bit.RX_FLAG = 0; // 接收命令处理完毕应答
        }

        if (CANB2.FLAG.bit.RX_FLAG == 1) {
            if (SCI_TX_BUFFER[2] == 0x70) {
                CAN2_DataCombine_Rx(SCI_TX_BUFFER);
                Board2_Num = ReceData[0];
            }

            if (Board2_Num == BORAD_NUM) {
                Can_Receive();
            }

            CANB2.FLAG.bit.RX_FLAG = 0; // 接收命令处理完毕应答
        }

        if (CANB.FLAG.bit.TIMETX_FLAG == 1) {
            canTransmitHandle();
            CANB.FLAG.bit.TIMETX_FLAG = 0;
        }

        if (Ctrl_Flag.bit.STOP_PWM_Flag == 1 && CANB.FLAG.bit.DIAGTX_FLAG == 0) {
            Can_Tx4_Data();
            Can_Tx13_Data();
            CANB.FLAG.bit.DIAGTX_FLAG = 1;
        }

        if (Ctrl_Flag.bit.STOP_PWM_Flag == 0 && CANB.FLAG.bit.DIAGTX_FLAG == 1) {
            Can_Tx4_Data();
            Can_Tx13_Data();
            CANB.FLAG.bit.DIAGTX_FLAG = 0;
        }

        Ctrl_Change();
        if (CAN_BUFFER_CTRL_Num > 0) {
            canBufferCtrlNum();
        }
    }
}
