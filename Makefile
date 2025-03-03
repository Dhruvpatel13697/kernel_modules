obj-m += hello.o
# obj-m += k_prob.o
# obj-m += sct.o

PWD := $(CURDIR)
module_dir := /lib/modules/$(shell uname -r)/build

all:
	make -C $(module_dir) M=$(PWD) modules

clean:
	make -C $(module_dir) M=$(PWD) clean