#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
#include  <linux/err.h>

/* The code was inspired from https://gist.github.com/BtbN/9665914 
 * https://embetronicx.com/tutorials/linux/device-drivers/sysfs-in-linux-kernel/
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andreas Mrsaiba");
MODULE_DESCRIPTION("Userspace PWM management device");

//dev_t dev = 0;
//static struct class *dev_class;
//static struct cdev etx_cdev;
//struct kobject *kobj_ref;
/*
** Function Prototypes
*/
static int      __init pwm_driver_init(void);
static void     __exit pwm_driver_exit(void);
/* TO DO
 * -> create a /sys/kernel/ entry for the driver
   -> write it's fops functions
 */


static int	__init pwm_driver_init(void) {
	printk("Init\n");
	return 0;
}

static void 	__exit pwm_driver_exit(void) {
	printk("Exiting\n");
	return;
}

module_init(pwm_driver_init);
module_exit(pwm_driver_exit);
