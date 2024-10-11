#include "F28335_Device.h"
#include "App_UserDef.h"

#pragma CODE_SECTION(ABC2DQ_CALC, "ramfuncs");
#pragma CODE_SECTION(DQ2ABC_CALC, "ramfuncs");
#pragma CODE_SECTION(THREE2TWO_CALC, "ramfuncs");

//-------------------------------------------------------------------------------
/* sqrt(2)/sqrt(3) = 0.81649658093 */
/* 1/sqrt(2) = 0.7071067811865 */
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 函数功能描述:
//	a、b、c三相静止坐标系转换为旋转的d、q坐标系，采用的是恒功率变换。
//	dr = sqrt(2/3)*[as*cos(ang) + bs*cos(ang-120) + cs*cos(ang+120)]
//	qr = sqrt(2/3)*[as*sin(ang) + bs*sin(ang-120) + cs*sin(ang+120)]
//	注:此变换是在选取q轴在旋转方向滞后d轴90度.
// 入口参数:
// 出口参数:
// 调用说明:
//------------------------------------------------------------------------------
void ABC2DQ_CALC() {
    volatile _iq cos_anga, sin_anga, cos_angb, sin_angb, cos_angc, sin_angc;

    /* Using look-up IQ sine table */
    sin_anga = _IQsinPU(abctodq.ang);
    cos_anga = _IQcosPU(abctodq.ang);
    sin_angb = _IQsinPU(abctodq.ang - _IQ(0.3333333333));
    cos_angb = _IQcosPU(abctodq.ang - _IQ(0.3333333333));
    sin_angc = _IQsinPU(abctodq.ang + _IQ(0.3333333333));
    cos_angc = _IQcosPU(abctodq.ang + _IQ(0.3333333333));

    _iq iaCos = _IQmpy(abctodq.as, cos_anga);
    _iq ibCos = _IQmpy(abctodq.bs, cos_angb);
    _iq icCos = _IQmpy(abctodq.cs, cos_angc);
    abctodq.dr = _IQmpy(iaCos + ibCos + icCos, _IQ(0.81649658093));

    _iq iaSin = _IQmpy(abctodq.as, sin_anga);
    _iq ibSin = _IQmpy(abctodq.bs, sin_angb);
    _iq icSin = _IQmpy(abctodq.cs, sin_angc);
    abctodq.qr = _IQmpy(iaSin + ibSin + icSin, _IQ(0.81649658093));
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 函数功能描述:
//	旋转的d、q坐标系转换为a、b、c三相静止坐标系，采用的是恒功率变换。
//	as = sqrt(2/3)*[dr*cos(ang) + qr*sin(ang)]
//	bs = sqrt(2/3)*[dr*cos(ang-120) + qr*sin(ang-120)]
//	cs = sqrt(2/3)*[dr*cos(ang+120) + qr*sin(ang+120)]
//	注:此变换是在选取q轴在旋转方向滞后d轴90度.
// 入口参数:
// 出口参数:
// 调用说明:
//------------------------------------------------------------------------------
void DQ2ABC_CALC() {
    volatile _iq cos_anga, sin_anga, cos_angb, sin_angb, cos_angc, sin_angc, Temp_Q;

    /* Using look-up IQ sine table */
    sin_anga = _IQsinPU(dqtoabc.ang);
    cos_anga = _IQcosPU(dqtoabc.ang);

    Temp_Q = dqtoabc.ang - _IQ(0.3333333333);
    Temp_Q = AngleAdjust(Temp_Q);
    sin_angb = _IQsinPU(Temp_Q);
    cos_angb = _IQcosPU(Temp_Q);

    Temp_Q = dqtoabc.ang + _IQ(0.3333333333);
    Temp_Q = AngleAdjust(Temp_Q);
    sin_angc = _IQsinPU(Temp_Q);
    cos_angc = _IQcosPU(Temp_Q);

    dqtoabc.as = _IQmpy((_IQmpy(dqtoabc.dr, cos_anga) + _IQmpy(dqtoabc.qr, sin_anga)), _IQ(0.81649658093));
    dqtoabc.bs = _IQmpy((_IQmpy(dqtoabc.dr, cos_angb) + _IQmpy(dqtoabc.qr, sin_angb)), _IQ(0.81649658093));
    dqtoabc.cs = _IQmpy((_IQmpy(dqtoabc.dr, cos_angc) + _IQmpy(dqtoabc.qr, sin_angc)), _IQ(0.81649658093));
}
//---------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 函数功能描述:
//	a、b、c三相静止坐标系转换为静止的2坐标系，采用的是恒功率变换。
//	ds = sqrt(2/3)*[as - (1/2)bs - (1/2)cs]
//	qs = sqrt(2/3)*[bs*sqrt(3)/2 - cs*sqrt(3)/2]
//
// 入口参数:
// 出口参数:
// 调用说明:
//------------------------------------------------------------------------------
void THREE2TWO_CALC() {
    phase_trans.ds = _IQmpy(_IQ(0.81649658093), (phase_trans.as - (phase_trans.bs >> 1) - (phase_trans.cs >> 1)));
    phase_trans.qs = _IQmpy(_IQ(0.7071067811865), (phase_trans.bs - phase_trans.cs));
}
