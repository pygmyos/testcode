
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_md.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_adc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_audio.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_cmd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_debug.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_fpec.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_rf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\pygmy_com.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\STM32F100_64K_8K_FLASH_PygmyOS.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\FileTest\FileTest.elf")
