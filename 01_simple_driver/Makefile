obj-m := hello_drv.o
KDIR = /lib/modules/5.4.0-81-generic/build/

all:
		$(MAKE) -C $(KDIR) M=$(PWD)

.PHONY:clean
clean:
		rm -f *.mod.c *.mod.o *.ko *.o *.order *.symvers
