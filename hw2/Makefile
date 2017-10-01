ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := hello_p1.o hello_p2.o
ccflags-y := -Iinc
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif