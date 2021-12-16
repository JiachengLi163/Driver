#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


static int major = 0;

static ssize_t hello_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
//	int err;
	printk("%s %s line=%d\n", __FILE__, __func__, __LINE__);
//	err = copy_to_user(buf, kernel_buf, MIN(1024,size));
//	return MIN(1024, size);
	return 1;
}

static ssize_t hello_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
//	int err;
	printk("%s %s line=%d\n", __FILE__, __func__, __LINE__);
//	err = copy_from_user(kernel_buf, buf, MIN(1024,size));
//	return MIN(1024, size);
	return 1;
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
	return 0;
}
module_init(hello_init);

void hello_exit(void)
{
	printk("hello drv exit \n");
	unregister_chrdev(major, "hello_drv");
}
module_exit(hello_exit);

MODULE_LICENSE("GPL");
