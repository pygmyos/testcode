
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_ld.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_adc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_audio.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_cmd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_com.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_debug.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_fpec.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_gui.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_rf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\main.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\STM32F100_64K_8K_FLASH_PygmyOS.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\BootUpdater\BootUpdater.elf")
