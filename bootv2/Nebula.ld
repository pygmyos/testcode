/*
!!! For Bootloader !!!
PygmyOS linker subscript for STM32F103 with 1024K Flash and 96K RAM
PygmyOS Bootloader has first 16KB reserved
PygmyOS Must be located at 0x08004000
*/
/* include the common STMxxx sub-script */
INCLUDE "STM32F10x_COMMON.ld"

/* include the memory spaces definitions sub-script */
/*
PygmyOS linker subscript for STM32F100 with 64K Flash and 8K RAM
PygmyOS Bootloader has first 8KB reserved
PygmyOS Must be located at 0x08002000
*/

/* Memory Spaces Definitions */

MEMORY
{
  RAM (xrw) : ORIGIN =  0x20000000, LENGTH = 8K
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
  HEAP (xrw) : ORIGIN = 0x20004000, LENGTH = 80K
  STARTFLASH (rx) : ORIGIN = 0x0, LENGTH = 0x0
  CRPPATCH (r) : ORIGIN = 0x0, LENGTH = 0
  FLASHPATCH (r) : ORIGIN = 0x00000000, LENGTH = 0
  ENDFLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  FLASHB1  (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB0 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx)  : ORIGIN = 0x00000000, LENGTH = 0
}

/* higher address of the user mode stack */
/* 0x20018000 for applications, 0x20002000 for bootloader */
_estack = 0x20014000;

/* include the sections management sub-script for FLASH mode */
INCLUDE "sections_FLASH.ld"



