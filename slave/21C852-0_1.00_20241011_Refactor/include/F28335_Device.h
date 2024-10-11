#ifndef DSP28335_DEVICE_H
#define DSP28335_DEVICE_H


#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// 为了方面,定义下列数据类型
// 定义16-bit 和 32-Bit 有符号/无符号 整形数据:
//

#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int                int16;
typedef long               int32;
typedef long long          int64;
typedef unsigned int       Uint16;
typedef unsigned long      Uint32;
typedef unsigned long long Uint64;
typedef float              float32;
typedef long double        float64;
#endif


//---------------------------------------------------------------------------
// Common CPU Definitions:
//

extern cregister volatile Uint16 IFR;
extern cregister volatile Uint16 IER;

#define  EINT   {asm(" clrc INTM");}
#define  DINT   {asm(" setc INTM");}
#define  ERTM   {asm(" clrc DBGM");}
#define  DRTM   {asm(" setc DBGM");}
#define  EALLOW {asm(" EALLOW");}
#define  EDIS   {asm(" EDIS");}
#define  ESTOP0 {asm(" ESTOP0");}

#define M_INT1  0x0001
#define M_INT2  0x0002
#define M_INT3  0x0004
#define M_INT4  0x0008
#define M_INT5  0x0010
#define M_INT6  0x0020
#define M_INT7  0x0040
#define M_INT8  0x0080
#define M_INT9  0x0100
#define M_INT10 0x0200
#define M_INT11 0x0400
#define M_INT12 0x0800
#define M_INT13 0x1000
#define M_INT14 0x2000
#define M_DLOG  0x4000
#define M_RTOS  0x8000

#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000



//---------------------------------------------------------------------------
// 包含所有外设寄存器头文件:
//

#include "F28335_Adc.h"                // ADC Registers
#include "F28335_DevEmu.h"             // Device Emulation Registers
#include "F28335_CpuTimers.h"          // 32-bit CPU Timers
#include "F28335_ECan.h"               // Enhanced eCAN Registers
#include "F28335_ECap.h"               // Enhanced Capture
#include "F28335_DMA.h"                // DMA Registers
#include "F28335_EPwm.h"               // Enhanced PWM
#include "F28335_EQep.h"               // Enhanced QEP
#include "F28335_Gpio.h"               // General Purpose I/O Registers
#include "F28335_I2c.h"                // I2C Registers
#include "F28335_McBSP.h"              // McBSP
#include "F28335_PieCtrl.h"            // PIE Control Registers
#include "F28335_PieVect.h"            // PIE Vector Table
#include "F28335_Spi.h"                // SPI Registers
#include "F28335_Sci.h"                // SCI Registers
#include "F28335_SysCtrl.h"            // System Control/Power Modes
#include "F28335_XIntrupt.h"           // External Interrupts
#include "F28335_Xintf.h"              // XINTF External Interface
#include "F28335_SWPrioritizedIsrLevels.h" 	// 中断服务程序软件优先级
#include "F28335_DefaultISR.h"			  	// 默认的中断服务程序,用于调试
#include "F28335_GlobalPrototypes.h"       	// C程序中全局函数的原型
#include "IQmathLib.h"						// IQmath库头文件 

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // DSP28335_DEVICE_H 定义结束


//===========================================================================
// End of file.
//===========================================================================
