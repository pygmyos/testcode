
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\lib\ARM\startup_stm32f10x_hd.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\main.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_fpec.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_rf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_sys.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_xmodem.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_rtc.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_com.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_port.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_string.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_nvic.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_socket.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\sst25vf.o")
INPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\pygmy_pfat.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\STM32F103_1M_96K_FLASH_PygmyOS.ld"
OUTPUT("C:\Users\MightyDubster\Documents\Projects\pygmyos.code\bootv2\Boot.elf")
