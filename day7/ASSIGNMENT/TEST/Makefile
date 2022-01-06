obj-m:=chardev.o 

KD=/lib/modules/$(shell uname -r)/build/

PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KD) M=$(PWD) modules
	gcc -o add add.c
	gcc -o sub sub.c
	gcc -o mul mul.c
	gcc -o div div.c

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean
	rm add sub mul div


