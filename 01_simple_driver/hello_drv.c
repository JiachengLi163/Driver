#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

static int major = 0;
static int ker_val = 123;
static struct class *class_for_hello;

static ssize_t hello_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
//	int err;
	printk("%s %s line=%d\n", __FILE__, __func__, __LINE__);
	copy_to_user(buf, &ker_val, 4);
//	return MIN(1024, size);
	return 4;
}

static ssize_t hello_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
//	int err;
	printk("%s %s line=%d\n", __FILE__, __func__, __LINE__);
	copy_from_user(&ker_val, buf, 4);
//	return MIN(1024, size);
	return 4;
}

static struct file_operations hello_fops = {
	.owner	= THIS_MODULE,
	.read 	= hello_read,
	.write	= hello_write
};

int hello_init(void)
{
	printk("hello drv init \n");
	major = register_chrdev(0, "hello_drv", &hello_fops); //注册一个字符设备
	class_for_hello = class_create(THIS_MODULE,"hello_class"); /*创建类*/
	device_create(class_for_hello, NULL, MKDEV(major, 0), NULL, "myhello"); /*dev/myhello被创建*/  /*创建设备*/
	return 0;
}
module_init(hello_init);

void hello_exit(void)
{
	printk("hello drv exit \n");
	device_destroy(class_for_hello, MKDEV(major, 0)); /*dev/myhello被创建*/
	class_destroy(class_for_hello);
	unregister_chrdev(major, "hello_drv");

}
module_exit(hello_exit);

MODULE_LICENSE("GPL");
