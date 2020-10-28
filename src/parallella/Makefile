EPIPHANY_HOME=/opt/adapteva/esdk
#host compiler path
LCC=/opt/linaro/bin/arm-linux-gnueabihf-gcc
#device compiler path
CC=e-gcc
#FreeRTOS dependencies
CFLAGS=-I.
FREERTOSSRC=../../RTFP_RTOS_KERNEL_PATCHED/FreeRTOS/Source
INCLUDES= -g -I$(FREERTOSSRC)/include -I$(FREERTOSSRC)/portable/GCC/Epiphany -I.
DEPS = $(FREERTOSSRC)/portable/GCC/Epiphany/portmacro.h Makefile RTFParallellaConfig.h FreeRTOSConfig.h c2c.h debugFlags.h AmaltheaConverter.h taskCode.h ParallellaUtils.h trace_utils_BTF.h
DEPSHOST = RTFParallellaConfig.h model_enumerations.h host_utils.h trace_utils_BTF.h
#Epiphany SDK dependencies
ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host.armv7l/lib
EINCS=${ESDK}/tools/host.armv7l/include
ELDF=${ESDK}/bsps/current/fast.ldf
EHDF=${EPIPHANY_HDF}
#search paths for C source code files
vpath %.c .:$(FREERTOSSRC)/:$(FREERTOSSRC)/portable/MemMang:$(FREERTOSSRC)/portable/GCC/Epiphany:/
#search path for assembly listings
vpath %.s $(FREERTOSSRC)/portable/GCC/Epiphany
#main target
all: host_main_example1 armcode core0_main.elf core1_main.elf
	@echo build status : successful
run: armcode core0_main.elf core1_main.elf
	@echo build status : successful

example1: host_main_example1 core0_main.elf core1_main.elf
	@echo build status : successful

#rule for every device target
%.elf: $(ELDF) tasks.o queue.o list.o portasm.o port.o heap_1.o c2c.o debugFlags.o AmaltheaConverter.o taskCode.o ParallellaUtils.o shared_comms.o label_man_core0.o %.o
	$(CC) -g -T$< -Wl,--gc-sections -o $@ $(filter-out $<,$^) -le-lib

#host target
armcode: armcode.c $(DEPSHOST) 
	$(LCC) $< -o $@  host_utils.c model_enumerations.c trace_utils_BTF.c -I ${EINCS} -L ${ELIBS} -lpal -le-hal -le-loader -lpthread 
host_main_example1: host_main_example1.c $(DEPSHOST) 
	$(LCC) $< -o $@ host_utils.c model_enumerations.c trace_utils_BTF.c -I ${EINCS} -L ${ELIBS} -lpal -le-hal -le-loader -lpthread 

#clean target
clean:
	rm -f *.o *.srec *.elf armcode host_main_example1

.SECONDARY:
%.o: %.c $(DEPS)
	$(CC) -fdata-sections -ffunction-sections  -c -D__EPIPHANY_DEVICE__ -o $@ $< $(INCLUDES)

%.o: %.s $(DEPS)
	$(CC) -c -o $@ $< $(INCLUDES)

