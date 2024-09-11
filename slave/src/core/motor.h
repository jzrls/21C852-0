#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "F28335_Device.h" /* F28335 外设头文件*/
#include "App_UserDef.h"   /* F28335 应用项目头文件*/

typedef struct {
    volatile _iq resistorValue;              /* 电机电阻值 */
    volatile _iq dInductanceValue;           /* D轴电感值 */
    volatile _iq qInductanceValue;           /* Q轴电感值 */
    volatile _iq fluxValue;                  /* 电机磁链 */
    volatile _iq motorPolePairsValue;        /* 电机极对数 */
    volatile _iq rdcPolePairsValue;          /* 旋变极对数 */
    volatile _iq udc;                        /* 直流电压 */
    volatile _iq udcBase;                    /* 电压基值 */
    volatile _iq idcBase;                    /* 电流基值 */
    volatile _iq velocityBase;               /* 电速度基值(rad/s) */
    volatile _iq velocityMaxValue;           /* 速度限幅值 */
    volatile _iq rdcPostion;                 /* 电机旋变安装位置 */
    volatile _iq isMaxValue;                 /* 电流限幅值 */
    volatile _iq isIncreaseStepLength;       /* 电流增加步长 */
    volatile _iq idcReduceStepLength;        /* 电流减少步长 */
    volatile _iq controlPeriod;              /* 载波周期(us)*37.5 */
    volatile _iq iqFilterFrequency;          /* Idq滤波频率 */
    volatile _iq velocityFilterFrequency;    /* 转速滤波频率 */
    volatile _iq deadZoneTime;               /* 死区时间(us)*75 */
    volatile _iq velocityIncreaseStepLength; /* 速度增加步长 */
    volatile _iq isDecreaseStepLength;       /* 三相合成电流减少步长 */
    volatile _iq temperatureFilterFrequency; /* 温度滤波频率 */
    volatile _iq zeroPositon;                /* 电机零位 */
    volatile _iq udcCompensationAngle;       /* 控制电压补偿角/360 */
    volatile _iq idcCompensationAngle;       /* 控制电流补偿角/360 */
    volatile _iq idcMaxValue;                /* 最大电流设定值 */
    volatile _iq isLimitValue;               /* 初始Id标幺值 */
    volatile _iq motorTemperatureProtectionValue;
    volatile _iq igbtTemperatureProtectionValue;
} MotorParams;
#endif
