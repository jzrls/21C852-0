#ifndef __ACTION_H__
#define __ACTION_H__

#include "F28335_Device.h"
#include "App_UserDef.h"

extern Uint16 getFrameCount();

void disableResolver();
void readResolver();

extern void hardwareProtection();

extern void writeParamsIntoEeprom();

extern Uint16 getCpuStatus();
extern Uint16 getPowerSupplyStatus();
extern Uint16 getDriverStatus();

void _getValve1Params();
void _getValve2Params();
void _getPumpParams();
extern void getValve1RunningParams();
extern void getValve2RunningParams();
extern void getPumpRunningParams();

void _setTelemeterResponseData();
extern void selfCheckResponse();
extern void controlResponse();
extern void telemeteringResponse();
extern void otherResponse();

extern void canReceiveHandle();
extern void canTransmitHandle();
extern void canBufferCtrlNum();

#endif
