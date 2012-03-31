
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_hd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_adc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_audio.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_cmd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_com.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_debug.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_file.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_fpec.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_gui.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_rf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\varitronix128x128_parallel.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\gems.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\pygmy_jpeg.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\mpl115a2.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\hih5030.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\at24hc02b.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\STM32F103_1M_96K_FLASH_PygmyOS.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\Gems\Gems.elf")
