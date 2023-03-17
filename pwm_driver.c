/* The code was inspired from https://gist.github.com/BtbN/9665914 
 * https://embetronicx.com/tutorials/linux/device-drivers/sysfs-in-linux-kernel/
 */

/*
** Function Prototypes
*/
//static int      __init pwm_driver_init(void);
//static void     __exit pwm_driver_exit(void);
 
/*************** Driver functions **********************/
/*static int      pwm_open(struct inode *inode, struct file *file);
static int      pwm_release(struct inode *inode, struct file *file);
static ssize_t  pwm_read(struct file *filp, 
                        char __user *buf, size_t len,loff_t * off);
static ssize_t  pwm_write(struct file *filp, 
                        const char *buf, size_t len, loff_t * off);
*/ 
/*************** Sysfs functions **********************/
/*static ssize_t  sysfs_show(struct kobject *kobj, 
                        struct kobj_attribute *attr, char *buf);
static ssize_t  sysfs_store(struct kobject *kobj, 
                        struct kobj_attribute *attr,const char *buf, size_t count);
*/
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

dev_t dev = 0;
static struct class *dev_class;
static struct cdev pwm_dev;
struct kobject *kobj_ref;

volatile int pwm_value = 0;

/* The sysfs functions are used when user space calls */

static ssize_t sysfs_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	printk("Reading from sysfs\n");
	return sprintf(buf, "%d\n", pwm_value); //return the current pwm_value 
}

static ssize_t sysfs_store(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	printk("Writing to sysfs\n");
	sscanf(buf, "%d", &pwm_value);
	return count; //return how much was bytes were read
}

struct kobj_attribute pwm_attr = __ATTR(pwm_value, 0660, sysfs_show, sysfs_store);

/* Driver functions */

/* This function will write to the device */
static ssize_t pwm_write(struct file *filp,
		const char __user *buf, size_t len, loff_t *off)
{	
	printk("User has written to the device file\n");
	return len;
}

/* This function will read from the device */
static ssize_t pwm_read(struct file *filp,
		char __user *buf, size_t len, loff_t *off)
{
	printk("User has read from the device file\n");
	return 0;
}
/* This function will open the device */
static ssize_t pwm_open(struct inode *inode, struct file *file)
{
	printk("Device file has been opened!\n");
	return 0;
}

/* This function will release the device */
static ssize_t pwm_release(struct inode *inode, struct file *file)
{
	printk("Device file has been released\n");
	return 0;
}

static struct file_operations pwmdev_fops =
{
	.owner          = THIS_MODULE,
        .read           = pwm_read,
        .write          = pwm_write,
        .open           = pwm_open,
        .release        = pwm_release,
};

static int	__init pwm_driver_init(void) 
{
	printk("Init\n");
	
	/* Allocate major number */
	if ((alloc_chrdev_region(&dev, 0, 1, "pwm_dev")) < 0)
	{
		printk("Cannot allocate major number\n");
		return -1;
	}
	printk("Major = %d Minor %d \n", MAJOR(dev), MINOR(dev));

	/* Creating cdev structure */
	cdev_init(&pwm_dev, &pwmdev_fops);
	
	/* Adding character device to the system */
	if ((cdev_add(&pwm_dev, dev, 1)) < 0)
	{
		printk("Cannot add the device to system\n");
		goto r_class;
	}
	
	/* Creating struct class */
	if (IS_ERR(dev_class = class_create(THIS_MODULE, "pwm_class")))
	{
		printk("Cannot create the struct class\n");
		goto r_class;
	}
	
	/* Creating device */
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "pwm_device")))
	{
		printk("Cannot create the device\n");
		goto r_dev;
	}
	
	/* Creating a directory in /sys/kernel*/
	kobj_ref = kobject_create_and_add("pwm_sysfs", kernel_kobj); 
	
	/* Create the sysfs file for pwm_value*/
	if (sysfs_create_file(kobj_ref, &pwm_attr.attr))
	{
		pr_err("Cannot create sysfs file\n");
		goto r_sysfs;
	}
	printk("Device driver for pwm insert done\n");
	return 0;

	r_sysfs:
		kobject_put(kobj_ref);
		sysfs_remove_file(kernel_kobj, &pwm_attr.attr);
	r_dev:
		class_destroy(dev_class);
	r_class:
		unregister_chrdev_region(dev, 1);
		cdev_del(&pwm_dev);
		return -1;
}

	

static void 	__exit pwm_driver_exit(void) 
{
	printk("Exiting\n");
	return;
}

module_init(pwm_driver_init);
module_exit(pwm_driver_exit);



MODULE_LICENSE("GPL");
