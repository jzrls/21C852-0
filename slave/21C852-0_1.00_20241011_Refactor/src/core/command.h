#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "F28335_Device.h"
#include "App_UserDef.h"

#define CPU_0 0
#define CPU_1 1
#define CPU_2 2
#define CPU_3 3
#define CPU_4 4
#define CPU_5 5

#define VALVE1_MAIN_CPU CPU_0
#define VALVE1_SUB_CPU CPU_1
#define VALVE2_MAIN_CPU CPU_4
#define VALVE2_SUB_CPU CPU_5
#define PUMP_MAIN_CPU CPU_2
#define PUMP_SUB_CPU CPU_3

#define SELF_CHECK_COMMAND 0xA0
#define CONTROL_COMMAND 0xA1
#define TELEMETERING_COMMAND 0xA2

void getResolver();
void getCurrent();
void getRuningParams();
void setControlParams(_iq *sendArray);
void getControlParams(_iq *recv);
void setMotorParams();
void getMotorParams(_iq *recv);
void setAnalogParams();
void getAnalogParams(_iq *recv);
void setOtherParams();
void getOtherParams(_iq *recv);
void getEeprom();

void chooseCpu();
void clearFaultFlag();
void openControl();
void lockControl();
void speedControl();
void positionControl();
void currentContrl();
void stopOpenAndLockControl();
void stopMotor();

#endif
