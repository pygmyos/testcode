
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_md.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_adc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_com.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_debug.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_fpec.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_gui.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_rf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\pygmy_cmd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\varitronix128x128_parallel.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\voltShield.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\core\STM32F100_64K_8K_FLASH_PygmyOS.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.projects\Tutorial1\Tutorial1.elf")
