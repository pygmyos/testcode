
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_hd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_audio.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_cmd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_com.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_debug.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_fpec.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_gui.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_rf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\pygmy_adc.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\STM32F100_64K_8K_FLASH_PygmyOS.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Test\Test.elf")
