obj-m += enable_ccr.o

all:
	make -C /usr/src/linux-headers-$(shell uname -r)/ M=$(PWD) modules

clean:
	make -C /usr/src/linux-headers-$(shell uname -r)/ M=$(PWD) clean

install: all
	@echo "Enabling access to performance counters; ideally, you should know what you are doing" 
	sudo insmod enable_ccr.ko
