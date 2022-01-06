obj-m:=spin_app.o
KD=/lib/modules/$(shell uname -r)/build/
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o app app.c

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	rm app

install:
	$(MAKE) -C $(KD) M=$(PWD) modules_install
