/*
//================================================================================
//
// 文件名:    F28335_PieVect.h
//
// 功能描述:  
//			  F28335 器件 中断向量表定义.
//
// 编写者:    伍泽东
// 编写日期:  2011-3-28
//
//================================================================================
*/
#ifndef DSP28335_PIE_VECT_H
#define DSP28335_PIE_VECT_H


#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// PIE 中断向量表定义:
// 定义个叫做PINT(指向中断)用户类型 :

typedef interrupt void(*PINT)(void);

// 定义向量表结构:
struct PIE_VECT_TABLE {

// 复位向量不从这个表中获取,它总是从0x3FFFC0在boot ROM 的这个地址获取
 

      PINT     PIE1_RESERVED;
      PINT     PIE2_RESERVED;
      PINT     PIE3_RESERVED;
      PINT     PIE4_RESERVED;
      PINT     PIE5_RESERVED;
      PINT     PIE6_RESERVED;
      PINT     PIE7_RESERVED;
      PINT     PIE8_RESERVED;
      PINT     PIE9_RESERVED;
      PINT     PIE10_RESERVED;
      PINT     PIE11_RESERVED;
      PINT     PIE12_RESERVED;
      PINT     PIE13_RESERVED;

// Non-Peripheral Interrupts:
      PINT     XINT13;    // XINT13 / CPU-Timer1
      PINT     TINT2;     // CPU-Timer2
      PINT     DATALOG;   // Datalogging interrupt
      PINT     RTOSINT;   // RTOS interrupt
      PINT     EMUINT;    // Emulation interrupt
      PINT     XNMI;      // Non-maskable interrupt
      PINT     ILLEGAL;   // Illegal operation TRAP
      PINT     USER1;     // User Defined trap 1
      PINT     USER2;     // User Defined trap 2
      PINT     USER3;     // User Defined trap 3
      PINT     USER4;     // User Defined trap 4
      PINT     USER5;     // User Defined trap 5
      PINT     USER6;     // User Defined trap 6
      PINT     USER7;     // User Defined trap 7
      PINT     USER8;     // User Defined trap 8
      PINT     USER9;     // User Defined trap 9
      PINT     USER10;    // User Defined trap 10
      PINT     USER11;    // User Defined trap 11
      PINT     USER12;    // User Defined trap 12

// Group 1 PIE Peripheral Vectors:
      PINT     SEQ1INT;
      PINT     SEQ2INT;
      PINT     rsvd1_3;
      PINT     XINT1;
      PINT     XINT2;
      PINT     ADCINT;    // ADC
      PINT     TINT0;     // Timer 0
      PINT     WAKEINT;   // WD

// Group 2 PIE Peripheral Vectors:
      PINT     EPWM1_TZINT; // EPWM-1
      PINT     EPWM2_TZINT; // EPWM-2
      PINT     EPWM3_TZINT; // EPWM-3
      PINT     EPWM4_TZINT; // EPWM-4
      PINT     EPWM5_TZINT; // EPWM-5
      PINT     EPWM6_TZINT; // EPWM-6
      PINT     rsvd2_7;
      PINT     rsvd2_8;

// Group 3 PIE Peripheral Vectors:
      PINT     EPWM1_INT;  // EPWM-1
      PINT     EPWM2_INT;  // EPWM-2
      PINT     EPWM3_INT;  // EPWM-3
      PINT     EPWM4_INT;  // EPWM-4
      PINT     EPWM5_INT;  // EPWM-5
      PINT     EPWM6_INT;  // EPWM-6
      PINT     rsvd3_7;
      PINT     rsvd3_8;

// Group 4 PIE Peripheral Vectors:
      PINT     ECAP1_INT; // ECAP-1
      PINT     ECAP2_INT; // ECAP-2
      PINT     ECAP3_INT; // ECAP-3
      PINT     ECAP4_INT; // ECAP-4
      PINT     ECAP5_INT; // ECAP-5
      PINT     ECAP6_INT; // ECAP-6
      PINT     rsvd4_7;
      PINT     rsvd4_8;

// Group 5 PIE Peripheral Vectors:
      PINT     EQEP1_INT; // EQEP-1
      PINT     EQEP2_INT; // EQEP-2
      PINT     rsvd5_3;
      PINT     rsvd5_4;
      PINT     rsvd5_5;
      PINT     rsvd5_6;
      PINT     rsvd5_7;
      PINT     rsvd5_8;

// Group 6 PIE Peripheral Vectors:
      PINT     SPIRXINTA; // SPI-A
      PINT     SPITXINTA; // SPI-A
      PINT     MRINTB;    // McBSP-B
      PINT     MXINTB;    // McBSP-B
	  PINT     MRINTA;    // McBSP-A
      PINT     MXINTA;    // McBSP-A
      PINT     rsvd6_7;
      PINT     rsvd6_8;

// Group 7 PIE Peripheral Vectors:
      PINT     DINTCH1;   // DMA
      PINT     DINTCH2;   // DMA
      PINT     DINTCH3;   // DMA
      PINT     DINTCH4;   // DMA
      PINT     DINTCH5;   // DMA
      PINT     DINTCH6;   // DMA
      PINT     rsvd7_7;
      PINT     rsvd7_8;

// Group 8 PIE Peripheral Vectors:
      PINT     I2CINT1A;  // I2C-A
      PINT     I2CINT2A;  // I2C-A
      PINT     rsvd8_3;
      PINT     rsvd8_4;
      PINT     SCIRXINTC;  // SCI-C
      PINT     SCITXINTC;  // SCI-C
      PINT     rsvd8_7;
      PINT     rsvd8_8;

// Group 9 PIE Peripheral Vectors:
      PINT     SCIRXINTA;  // SCI-A
      PINT     SCITXINTA;  // SCI-A
      PINT     SCIRXINTB;  // SCI-B
      PINT     SCITXINTB;  // SCI-B
      PINT     ECAN0INTA;  // eCAN-A
      PINT     ECAN1INTA;  // eCAN-A
      PINT     ECAN0INTB;  // eCAN-B
      PINT     ECAN1INTB;  // eCAN-B

// Group 10 PIE Peripheral Vectors:
      PINT     rsvd10_1;
      PINT     rsvd10_2;
      PINT     rsvd10_3;
      PINT     rsvd10_4;
      PINT     rsvd10_5;
      PINT     rsvd10_6;
      PINT     rsvd10_7;
      PINT     rsvd10_8;

// Group 11 PIE Peripheral Vectors:
      PINT     rsvd11_1;
      PINT     rsvd11_2;
      PINT     rsvd11_3;
      PINT     rsvd11_4;
      PINT     rsvd11_5;
      PINT     rsvd11_6;
      PINT     rsvd11_7;
      PINT     rsvd11_8;

// Group 12 PIE Peripheral Vectors:
      PINT     XINT3;      // External interrupt
      PINT     XINT4;
      PINT     XINT5;
      PINT     XINT6;
      PINT     XINT7;
      PINT     rsvd12_6;
      PINT     LVF;        // Latched overflow
      PINT     LUF;        // Latched underflow
};

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table External References & Function Declarations:
//
extern struct PIE_VECT_TABLE PieVectTable;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    //  DSP28335_PIE_VECT_H 定义结束

//===========================================================================
// End of file.
//===========================================================================
