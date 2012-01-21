
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\boot")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_hd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\boot\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\boot\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\boot\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\boot\pygmy_fpec.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Ride7\Nebula\boot\STM32F100_64K_8K_FLASH.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\boot\Boot.elf")
