obj-m:=module1/add.o module2/avg.o
KD =/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules

install:
	$(MAKE) -C $(KD) M=$(PWD) modules_install
	depmod -a

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean


