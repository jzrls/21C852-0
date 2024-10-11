#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "F28335_Device.h" /* F28335 ����ͷ�ļ�*/
#include "App_UserDef.h"   /* F28335 Ӧ����Ŀͷ�ļ�*/

typedef struct {
    volatile _iq resistorValue;              /* �������ֵ */
    volatile _iq dInductanceValue;           /* D����ֵ */
    volatile _iq qInductanceValue;           /* Q����ֵ */
    volatile _iq fluxValue;                  /* ������� */
    volatile _iq motorPolePairsValue;        /* ��������� */
    volatile _iq rdcPolePairsValue;          /* ���伫���� */
    volatile _iq udc;                        /* ֱ����ѹ */
    volatile _iq udcBase;                    /* ��ѹ��ֵ */
    volatile _iq idcBase;                    /* ������ֵ */
    volatile _iq velocityBase;               /* ���ٶȻ�ֵ(rad/s) */
    volatile _iq velocityMaxValue;           /* �ٶ��޷�ֵ */
    volatile _iq rdcPostion;                 /* ������䰲װλ�� */
    volatile _iq isMaxValue;                 /* �����޷�ֵ */
    volatile _iq isIncreaseStepLength;       /* �������Ӳ��� */
    volatile _iq idcReduceStepLength;        /* �������ٲ��� */
    volatile _iq controlPeriod;              /* �ز�����(us)*37.5 */
    volatile _iq iqFilterFrequency;          /* Idq�˲�Ƶ�� */
    volatile _iq velocityFilterFrequency;    /* ת���˲�Ƶ�� */
    volatile _iq deadZoneTime;               /* ����ʱ��(us)*75 */
    volatile _iq velocityIncreaseStepLength; /* �ٶ����Ӳ��� */
    volatile _iq isDecreaseStepLength;       /* ����ϳɵ������ٲ��� */
    volatile _iq temperatureFilterFrequency; /* �¶��˲�Ƶ�� */
    volatile _iq zeroPositon;                /* �����λ */
    volatile _iq udcCompensationAngle;       /* ���Ƶ�ѹ������/360 */
    volatile _iq idcCompensationAngle;       /* ���Ƶ���������/360 */
    volatile _iq idcMaxValue;                /* �������趨ֵ */
    volatile _iq isLimitValue;               /* ��ʼId����ֵ */
    volatile _iq motorTemperatureProtectionValue;
    volatile _iq igbtTemperatureProtectionValue;
} MotorParams;
#endif
