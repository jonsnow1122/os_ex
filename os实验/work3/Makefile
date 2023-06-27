# Makefile5.3
obj-m := chardev.o
PWD  := $(shell pwd)
KVER ?= $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD) #注意这里不是空格,是tab
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.mod *.symvers *.order

