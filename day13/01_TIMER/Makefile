obj-m:=timer.o
KD=/lib/modules/$(shell uname -r)/build/
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o timer_app timer_app.c
	
clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	rm timer_app

install:
	$(MAKE) -C $(KD) M=$(PWD) modules_install
