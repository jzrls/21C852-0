/*
//###########################################################################
//
// 文件名:  F28335_FLASH.cmd
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
PAGE 0:    /* Program Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */

   PRAML01234   : origin = 0x008000, length = 0x005000     /* on-chip RAM block L0 L1 L2 L3 16K*/
   
/*   FLASHH      : origin = 0x300000, length = 0x008000 */     /* on-chip FLASH */
/*   FLASHG      : origin = 0x308000, length = 0x008000 */    /* on-chip FLASH */
/*   FLASHF      : origin = 0x310000, length = 0x008000 */    /* on-chip FLASH */

   FLASHE      : origin = 0x318000, length = 0x008000     /* on-chip FLASH 使用，存储反正弦表 */
   FLASHCD     : origin = 0x320000, length = 0x010000     /* on-chip FLASH 用于程序区64K*/

   FLASHA      : origin = 0x338000, length = 0x007F80     /* on-chip FLASH */
   CSM_RSVD    : origin = 0x33FF80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   BEGIN       : origin = 0x33FFF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
   CSM_PWL     : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */
   OTP         : origin = 0x380400, length = 0x000400     /* on-chip OTP */
   ADC_CAL     : origin = 0x380080, length = 0x000009     /* ADC_cal function in Reserved memory */
   
   IQTABLES    : origin = 0x3FE000, length = 0x000b50     /* IQ Math Tables in Boot ROM */
   IQTABLES2   : origin = 0x3FEB50, length = 0x00008c     /* IQ Math Tables in Boot ROM */  
   FPUTABLES   : origin = 0x3FEBDC, length = 0x0006A0     /* FPU Tables in Boot ROM */

   ROM         : origin = 0x3FF27C, length = 0x000D44     /* Boot ROM */        
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */


PAGE 1 :   /* Data Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE0 for program allocation */
           /* Registers remain on PAGE1                                                  */
   
   BOOT_RSVD   : origin = 0x000000, length = 0x000050     /* Part of M0, BOOT rom will use this for stack */
   RAMM0       : origin = 0x000050, length = 0x0003B0     /* on-chip RAM block M0 */
   RAMM1       : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 堆栈区*/
   DRAML5   : origin = 0x00D000, length = 0x001000        /*C全局变量区 8K */	
   DRAML6L7   : origin = 0x00E000, length = 0x002000	 /*数据缓冲区   8K */ 
   DATA_ZONE6 : origin = 0x100000, length = 0x080000     /* XINTF zone 6 - data space   512k*/
   DATA_ZONE7 : origin = 0x220000, length = 0x080000     /* XINTF zone 7 - data space   512k*/
}

/* Allocate sections to memory blocks.
   Note:
         codestart user defined section in DSP28_CodeStartBranch.asm used to redirect code 
                   execution when booting to flash
         ramfuncs  user defined section to store functions that will be copied from Flash into RAM
*/ 
 
SECTIONS
{
 
   /* Allocate program areas: */

   codestart           : > BEGIN       PAGE = 0

   ramfuncs            : LOAD = FLASHCD, 
                         RUN = PRAML01234, 
                         LOAD_START(_RamfuncsLoadStart0),
                         LOAD_END(_RamfuncsLoadEnd0),
                         RUN_START(_RamfuncsRunStart0),
                         PAGE = 0
   
   IQmath              : LOAD = FLASHCD, 
                         RUN = PRAML01234, 
                         LOAD_START(_RamfuncsLoadStart1),
                         LOAD_END(_RamfuncsLoadEnd1),
                         RUN_START(_RamfuncsRunStart1),
                         PAGE = 0

   .cinit              : > FLASHCD      PAGE = 0
   .pinit              : > FLASHCD      PAGE = 0
   .text               : > FLASHCD      PAGE = 0
   .switch             : > FLASHCD      PAGE = 0  

   csmpasswds          : > CSM_PWL     PAGE = 0
   csm_rsvd            : > CSM_RSVD    PAGE = 0
   

   /*反正弦表区*/  
   .table			: > FLASHE,		PAGE = 0/*, TYPE = NOLOAD*/ 
    
   /* Allocate IQ math areas: */
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
   /* .reset is a standard section used by the compiler.  It contains the */ 
   /* the address of the start of _c_int00 for C Code.   /*
   /* When using the boot ROM this section and the CPU vector */
   /* table is not needed.  Thus the default type is set here to  */
   /* DSECT  */ 
   .reset              : > RESET,      PAGE = 0, TYPE = DSECT
   vectors             : > VECTORS     PAGE = 0, TYPE = DSECT
   
   /* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
   .adc_cal     : load = ADC_CAL,   PAGE = 0, TYPE = NOLOAD
   

/* PAGE 1*/         
   /* Allocate uninitalized data sections: */
   .stack              : > RAMM1       PAGE = 1

   .ebss               : > DRAML6L7       PAGE = 1
   .econst             : > DRAML6L7       PAGE = 1
   .esysmem            : > DRAML6L7       PAGE = 1


  .dataBuffer1     : > DRAML5,  PAGE = 1     /*内部数据缓冲区*/

  .dataBuffer2     : > DATA_ZONE7, PAGE = 1    /*外部数据缓冲区*/

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/

