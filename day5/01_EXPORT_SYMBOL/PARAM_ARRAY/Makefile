
  obj-m :=param_arr.o

  KD =/lib/modules/$(shell uname -r)/build
  PWD :=$(shell pwd)

all:
	$(MAKE) -C $(KD) M=$(PWD) modules

clean:
	$(MAKE) -C $(KD) M=$(PWD) clean


