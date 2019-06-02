EPIPHANY_HOME=/opt/adapteva/esdk
#host compiler path
LCC=/opt/linaro/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
#device compiler path
CC=e-gcc
#FreeRTOS dependencies
CFLAGS=-I.
FREERTOSSRC=../../Source
INCLUDES= -g -I$(FREERTOSSRC)/include -I$(FREERTOSSRC)/portable/GCC/Epiphany -I.
DEPS = $(FREERTOSSRC)/portable/GCC/Epiphany/portmacro.h Makefile FreeRTOSConfig.h c2c.h
DEPS_2 = printUtil.h
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
run: armcode main.elf main_rms.elf
	@echo build status : successful

#rule for every device target
%.elf: $(ELDF) tasks.o queue.o list.o port.o portasm.o heap_1.o c2c.o printUtil.o %.o
	$(CC) -g -T$< -o $@ $(filter-out $<,$^) -le-lib

#host target
armcode: armcode.c $(DEPS)
	$(LCC) $< -o $@  -I ${EINCS} -L ${ELIBS} -lpal -le-hal -le-loader -lpthread
#clean target
clean:
	rm -f *.o *.srec *.elf armcode

.SECONDARY:
%.o: %.c $(DEPS) $(DEPS2)
	$(CC) -fdata-sections -ffunction-sections -c -o $@ $< $(INCLUDES)

%.o: %.s $(DEPS)
	$(CC) -c -o $@ $< $(INCLUDES)
