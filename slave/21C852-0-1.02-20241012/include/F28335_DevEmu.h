/*
//================================================================================
//
// 文件名:    F28335_DevEmu.h
//
// 功能描述:  
//			  F2812 器件 设备仿真寄存器定义.
//
// 编写者:    伍泽东
// 编写日期:  2011-3-28
//
//================================================================================
*/
#ifndef DSP28335_DEV_EMU_H
#define DSP28335_DEV_EMU_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Device Emulation Register Bit Definitions:
//
// Device Configuration Register Bit Definitions
struct DEVICECNF_BITS  {     // bits  description
   Uint16 rsvd1:3;           // 2:0   reserved
   Uint16 VMAPS:1;           // 3     VMAP Status
   Uint16 rsvd2:1;           // 4     reserved
   Uint16 XRSn:1;            // 5     XRSn Signal Status
   Uint16 rsvd3:10;          // 15:6
   Uint16 rsvd4:3;           // 18:16
   Uint16 ENPROT:1;          // 19    Enable/Disable pipeline protection
   Uint16 rsvd5:7;           // 26:20 reserved
   Uint16 TRSTN:1;           // 27    Status of TRSTn signal
   Uint16 rsvd6:4;           // 31:28 reserved
};

union DEVICECNF_REG {
   Uint32                 all;
   struct DEVICECNF_BITS  bit;
};

// CLASSID
struct CLASSID_BITS   {  // bits  description
   Uint16 CLASSNO:8;     // 7:0   Class Number
   Uint16 PARTTYPE:8;    // 15:8  Part Type
};

union CLASSID_REG {
   Uint16               all;
   struct CLASSID_BITS  bit;
};

struct DEV_EMU_REGS {
   union DEVICECNF_REG DEVICECNF;  // device configuration
   union CLASSID_REG   CLASSID;    // Class ID
   Uint16              REVID;      // Device ID
   Uint16              PROTSTART;  // Write-Read protection start
   Uint16              PROTRANGE;  // Write-Read protection range
   Uint16              rsvd2[202];
};

// PARTID
struct PARTID_BITS   {  // bits  description
   Uint16 PARTNO:8;     // 7:0   Part Number
   Uint16 PARTTYPE:8;   // 15:8  Part Type
};

union PARTID_REG {
   Uint16               all;
   struct PARTID_BITS   bit;
};

struct PARTID_REGS {
   union PARTID_REG PARTID; // Part ID
};



//---------------------------------------------------------------------------
// Device Emulation Register References & Function Declarations:
//
extern volatile struct DEV_EMU_REGS DevEmuRegs;
extern volatile struct PARTID_REGS PartIdRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // DSP28335_DEV_EMU_H 定义结束


