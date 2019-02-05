
obj-m := assignment6.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(shell pwd)
	
#app: 
	gcc -o childproc childproc.c
	gcc -o lock1 lock1.c
	gcc -o lock2 lock2.c
	gcc -o lock3 lock3.c -pthread
	gcc -o lock4 lock4.c -pthread

clean:
	rm -rf lock1 lock2 lock3 lock4 childproc *.o *.ko *.mod.* *.symvers *.order *~ 
