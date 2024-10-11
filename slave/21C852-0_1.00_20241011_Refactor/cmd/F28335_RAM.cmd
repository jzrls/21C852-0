/*
//###########################################################################
//
// 文件名:  F28335_Ram.cmd
//
// 功能描述: 
// 
//          F28335应用程序及数据区的内存分配，段定位配置 
//
// 编写者:  伍泽东
//
// 编写日期:2011-9-1
//       
//
//###########################################################################
*/



MEMORY
{
PAGE 0 :    /* 程序存储区   */
  
	 /* BEGIN is used for the "boot to SARAM" bootloader mode      */
   BEGIN      : origin = 0x000000, length = 0x000002     /* Boot to M0 will go here                      */
   RAMM0      : origin = 0x000050, length = 0x0003B0
   FLASHE     : origin = 0x318000, length = 0x008000  	   /* 反正弦数据表 */ 	 
   PRAML012345  : origin = 0x008000, length = 0x007000     /* 片上RAM L0、L1、L2、L3  16K */
   CSM_RSVD   : origin = 0x33FF80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   CSM_PWL    : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA            */
   ADC_CAL    : origin = 0x380080, length = 0x000009
   RESET      : origin = 0x3FFFC0, length = 0x000002
   IQTABLES   : origin = 0x3FE000, length = 0x000b50
   IQTABLES2  : origin = 0x3FEB50, length = 0x00008c
   FPUTABLES  : origin = 0x3FEBDC, length = 0x0006A0
   BOOTROM    : origin = 0x3FF27C, length = 0x000D44


PAGE 1 : /*数据存储区*/
   /* BOOT_RSVD is used by the boot ROM for stack.               */
   /* This section is only reserved to keep the BOOT ROM from    */
   /* corrupting this area during the debug process              */

   BOOT_RSVD  : origin = 0x000002, length = 0x00004E     /* Part of M0, BOOT rom will use this for stack */
   RAMM1      : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 堆栈区*/
   /*DRAML4L5   : origin = 0x00C000, length = 0x002000      C全局变量区 8K */	
   DRAML6L7   : origin = 0x00F000, length = 0x001000	 /*数据缓冲区   8K */ 
   DATA_ZONE6 : origin = 0x100000, length = 0x080000     /*XINTF zone 6 - data space   512k*/
   DATA_ZONE7 : origin = 0x220000, length = 0x080000     /*XINTF zone 6 - data space   512k*/
}


SECTIONS
{
   /* Setup for "boot to SARAM" mode:
      The codestart section (found in DSP28_CodeStartBranch.asm)
      re-directs execution to the start of user code.  */
/* PAGE 0*/

   codestart        : > BEGIN,     PAGE = 0

   ramfuncs         : > PRAML012345, PAGE = 0    /*存放功能代码*/

   .text            : > PRAML012345, PAGE = 0
   .cinit           : > PRAML012345, PAGE = 0
   .pinit           : > PRAML012345, PAGE = 0
   .switch          : > PRAML012345, PAGE = 0

   .table			: > FLASHE,		   PAGE = 0,	TYPE = NOLOAD

   IQmath           : > PRAML012345, PAGE = 0
   IQmathTables     : > IQTABLES,  PAGE = 0, TYPE = NOLOAD

   /* Uncomment the section below if calling the IQNexp() or IQexp()
      functions from the IQMath.lib library in order to utilize the
      relevant IQ Math table in Boot ROM (This saves space and Boot ROM
      is 1 wait-state). If this section is not uncommented, IQmathTables2
      will be loaded into other memory (SARAM, Flash, etc.) and will take
      up space, but 0 wait-state is possible.
   */
   /*
   IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD
   {

              IQmath.lib<IQNexpTable.obj> (IQmathTablesRam)

   }
   */
   FPUmathTables    : > FPUTABLES, PAGE = 0, TYPE = NOLOAD

   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used                    */
   csm_rsvd         : > CSM_RSVD   PAGE = 0, TYPE = DSECT /* not used for SARAM examples */
   csmpasswds       : > CSM_PWL    PAGE = 0, TYPE = DSECT /* not used for SARAM examples */

   /* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
   .adc_cal         : load = ADC_CAL,   PAGE = 0, TYPE = NOLOAD

/* PAGE 1*/

   .stack           : > RAMM1,     PAGE = 1
   .ebss            : > DRAML6L7,  PAGE = 1
   .econst          : > DRAML6L7,  PAGE = 1
   .esysmem         : > DRAML6L7,  PAGE = 1

   .dataBuffer1     : > DRAML6L7,  PAGE = 1     /*内部数据缓冲区*/
   .dataBuffer2     : > DATA_ZONE7,  PAGE = 1     /*外部数据缓冲区*/
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
