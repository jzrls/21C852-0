/*
//================================================================================
//
// 文件名:    F28335_CpuTimers.h
//
// 功能描述:   F28335 CPU 32-bit 定时器寄存器定义.
//		CpuTimer1和CpuTimer2 用于DSP BIOS和其他实时操作系统
//           	如果要用集成的DSP-BIOS或者其他实时操作系统,就不能再使用这些定时器.   
//
// 编写者:   伍泽东
// 编写日期: 2011-3-28
//
//================================================================================
*/
#ifndef DSP28335_CPU_TIMERS_H
#define DSP28335_CPU_TIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// CPU Timer Register Bit Definitions:
//
//
// TCR: Control register bit definitions:
struct  TCR_BITS {          // bits  description
   Uint16    rsvd1:4;       // 3:0   reserved
   Uint16    TSS:1;         // 4     Timer Start/Stop
   Uint16    TRB:1;         // 5     Timer reload
   Uint16    rsvd2:4;       // 9:6   reserved
   Uint16    SOFT:1;        // 10    Emulation modes
   Uint16    FREE:1;        // 11
   Uint16    rsvd3:2;       // 12:13 reserved
   Uint16    TIE:1;         // 14    Output enable
   Uint16    TIF:1;         // 15    Interrupt flag
};

union TCR_REG {
   Uint16           all;
   struct TCR_BITS  bit;
};

// TPR: Pre-scale low bit definitions:
struct  TPR_BITS {        // bits  description
   Uint16     TDDR:8;     // 7:0   Divide-down low
   Uint16     PSC:8;      // 15:8  Prescale counter low
};

union TPR_REG {
   Uint16           all;
   struct TPR_BITS  bit;
};

// TPRH: Pre-scale high bit definitions:
struct  TPRH_BITS {       // bits  description
   Uint16     TDDRH:8;      // 7:0   Divide-down high
   Uint16     PSCH:8;       // 15:8  Prescale counter high
};

union TPRH_REG {
   Uint16           all;
   struct TPRH_BITS bit;
};

// TIM, TIMH: Timer register definitions:
struct TIM_REG {
   Uint16  LSW;
   Uint16  MSW;
};

union TIM_GROUP {
   Uint32          all;
   struct TIM_REG  half;
};

// PRD, PRDH: Period register definitions:
struct PRD_REG {
   Uint16  LSW;
   Uint16  MSW;
};

union PRD_GROUP {
   Uint32          all;
   struct PRD_REG  half;
};

//---------------------------------------------------------------------------
// CPU Timer Register File:
//
struct CPUTIMER_REGS {
   union TIM_GROUP TIM;   // Timer counter register
   union PRD_GROUP PRD;   // Period register
   union TCR_REG   TCR;   // Timer control register
   Uint16          rsvd1; // reserved
   union TPR_REG   TPR;   // Timer pre-scale low
   union TPRH_REG  TPRH;  // Timer pre-scale high
};

//---------------------------------------------------------------------------
// CPU Timer Support Variables:
//
struct CPUTIMER_VARS {
   volatile struct  CPUTIMER_REGS  *RegsAddr;
   Uint32    InterruptCount;
   float32   CPUFreqInMHz;
   float32   PeriodInUSec;
};

//---------------------------------------------------------------------------
// 函数原型和外部定义 :
//
void InitCpuTimers(void);

extern volatile struct CPUTIMER_REGS CpuTimer0Regs;
extern volatile struct CPUTIMER_REGS CpuTimer1Regs;
extern volatile struct CPUTIMER_REGS CpuTimer2Regs;


//---------------------------------------------------------------------------
// 有用的定时器操作
//
// 启动定时器:
#define StartCpuTimer0()   (CpuTimer0Regs.TCR.bit.TSS = 0)
#define StartCpuTimer1()   (CpuTimer1Regs.TCR.bit.TSS = 0)
#define StartCpuTimer2()   (CpuTimer2Regs.TCR.bit.TSS = 0)

// 停止定时器:
#define StopCpuTimer0()   (CpuTimer0Regs.TCR.bit.TSS = 1)
#define StopCpuTimer1()   (CpuTimer1Regs.TCR.bit.TSS = 1)
#define StopCpuTimer2()   (CpuTimer2Regs.TCR.bit.TSS = 1)

// 重载定时器周期值:
#define ReloadCpuTimer0() (CpuTimer0Regs.TCR.bit.TRB = 1)
#define ReloadCpuTimer1() (CpuTimer1Regs.TCR.bit.TRB = 1)
#define ReloadCpuTimer2() (CpuTimer2Regs.TCR.bit.TRB = 1)

// 读32位定时器值:
#define ReadCpuTimer0Counter() (CpuTimer0Regs.TIM.all)
#define ReadCpuTimer1Counter() (CpuTimer1Regs.TIM.all)
#define ReadCpuTimer2Counter() (CpuTimer2Regs.TIM.all)

// 读 32-Bit 定时周期值:
#define ReadCpuTimer0Period() (CpuTimer0Regs.PRD.all)
#define ReadCpuTimer1Period() (CpuTimer1Regs.PRD.all)
#define ReadCpuTimer2Period() (CpuTimer2Regs.PRD.all)


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // F28335_CPU_TIMERS_H 定义结束


//===========================================================================
// No more.
//===========================================================================
