/*
// TI File $Revision: /main/11 $
// Checkin $Date: April 15, 2009   09:57:28 $
//###########################################################################
//
// FILE:    28335_RAM_lnk.cmd
//
// TITLE:   Linker Command File For 28335 examples that run out of RAM
//
//          This ONLY includes all SARAM blocks on the 28335 device.
//          This does not include flash or OTP.
//
//          Keep in mind that L0 and L1 are protected by the code
//          security module.
//
//          What this means is in most cases you will want to move to
//          another memory map file which has more memory defined.
//
//###########################################################################
// $TI Release:   $
// $Release Date:   $
//###########################################################################
*/

/* ======================================================
// For Code Composer Studio V2.2 and later
// ---------------------------------------
// In addition to this memory linker command file,
// add the header linker command file directly to the project.
// The header linker command file is required to link the
// peripheral structures to the proper locations within
// the memory map.
//
// The header linker files are found in <base>\DSP2833x_Headers\cmd
//
// For BIOS applications add:      DSP2833x_Headers_BIOS.cmd
// For nonBIOS applications add:   DSP2833x_Headers_nonBIOS.cmd
========================================================= */

/* ======================================================
// For Code Composer Studio prior to V2.2
// --------------------------------------
// 1) Use one of the following -l statements to include the
// header linker command file in the project. The header linker
// file is required to link the peripheral structures to the proper
// locations within the memory map                                    */

/* Uncomment this line to include file only for non-BIOS applications */
/* -l DSP2833x_Headers_nonBIOS.cmd */

/* Uncomment this line to include file only for BIOS applications */
/* -l DSP2833x_Headers_BIOS.cmd */

/* 2) In your project add the path to <base>\DSP2833x_headers\cmd to the
   library search path under project->build options, linker tab,
   library search path (-i).
/*========================================================= */

/* Define the memory block start/length for the F28335
   PAGE 0 will be used to organize program sections
   PAGE 1 will be used to organize data sections

   Notes:
         Memory blocks on F28335 are uniform (ie same
         physical memory) in both PAGE 0 and PAGE 1.
         That is the same memory region should not be
         defined for both PAGE 0 and PAGE 1.
         Doing so will result in corruption of program
         and/or data.

         L0/L1/L2 and L3 memory blocks are mirrored - that is
         they can be accessed in high memory or low memory.
         For simplicity only one instance is used in this
         linker file.

         Contiguous SARAM memory blocks can be combined
         if required to create a larger memory block.
*/


MEMORY
{
PAGE 0:         /* Program Memory */
   BEGIN_M0        : origin = 0x000000, length = 0x000002     /* Part of M0SARAM.  Used for "Boot to M0" bootloader mode. */
   L0123SARAM      : origin = 0x008000, length = 0x004000     /* SARAM, L0 through L3 combined.  CSM secure */
   RESET           : origin = 0x3FFFC0, length = 0x000002     /* Part of Boot ROM */

 PAGE 1:        /* Data Memory */
   M0SARAM         : origin = 0x000002, length = 0x0003FE     /* 1Kw M0 SARAM */
   M1SARAM         : origin = 0x000400, length = 0x000400     /* 1Kw M1 SARAM */
   L4SARAM         : origin = 0x00C000, length = 0x001000     /* 4Kw L4 SARAM, DMA accessible */
   L5SARAM         : origin = 0x00D000, length = 0x001000     /* 4Kw L5 SARAM, DMA accessible */
   L6SARAM         : origin = 0x00E000, length = 0x001000     /* 4Kw L6 SARAM, DMA accessible, 1 WS prog access */
   L7SARAM         : origin = 0x00F000, length = 0x001000     /* 4Kw L7 SARAM, DMA accessible 1 WS prog access */
}


SECTIONS
{
/*** Compiler Required Sections ***/

  /* Program memory (PAGE 0) sections */
   .text             : > L0123SARAM,            PAGE = 0
   .cinit            : > L0123SARAM,            PAGE = 0
   .const            : > L0123SARAM,            PAGE = 0
   .econst           : > L0123SARAM,            PAGE = 0
   .pinit            : > L0123SARAM,            PAGE = 0
   .reset            : > RESET,                 PAGE = 0, TYPE = DSECT  /* Not using the .reset section */
   .switch           : > L0123SARAM,            PAGE = 0

  /* Data Memory (PAGE 1) sections */
   .bss              : > L4SARAM,               PAGE = 1
   .ebss             : > L4SARAM,               PAGE = 1
   .cio              : > M0SARAM,               PAGE = 1
   .stack            : > M1SARAM,               PAGE = 1
   .sysmem           : > L4SARAM,               PAGE = 1
   .esysmem          : > L4SARAM,               PAGE = 1

/*** User Defined Sections ***/
   codestart         : > BEGIN_M0,              PAGE = 0                /* Used by file CodeStartBranch.asm */
}

/******************* end of file ************************/
