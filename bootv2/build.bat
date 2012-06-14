arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb main.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_sys.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_com.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_port.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_rtc.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_xmodem.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_string.c
arm-none-eabi-gcc -I. -I C:/Users/MightyDubster/Documents/Projects/pygmyos/core -I C:/Users/MightyDubster/Documents/Projects/pygmyos -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb C:/Users/MightyDubster/Documents/Projects/pygmyos/core/pygmy_cmd.c

arm-none-eabi-ld -v -TNebula.ld -nostartfiles -o main.elf main.o pygmy_sys.o pygmy_com.o pygmy_port.o pygmy_rtc.o pygmy_xmodem.o pygmy_string.o pygmy_cmd.o
arm-none-eabi-objcopy -Oihex main.elf main.hex
