
obj-m += pwm_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod* modules* Mo* .tmp_versions


