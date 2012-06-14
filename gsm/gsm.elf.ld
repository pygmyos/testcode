
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("startup_stm32f10x_md.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_adc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_com.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\gsmModem.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\pygmy_cmd.o")
EXTERN( __io_putchar )
EXTERN( _write )
GROUP(
"STM32x_io_putchar_thumb.a"
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\STM32F100_64K_8K_FLASH.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\gsm\gsm.elf")
