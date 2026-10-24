// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku /dev/dork
 *
 * Writing text to /dev/dork sends it to the kernel log.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/string.h>

#define DORK_MAX_WRITE 256

static ssize_t dork_write(struct file *file,
			  const char __user *buf,
			  size_t len,
			  loff_t *ppos)
{
	char message[DORK_MAX_WRITE];
	size_t count;

	if (!len)
		return 0;

	count = min(len, sizeof(message) - 1);

	if (copy_from_user(message, buf, count))
		return -EFAULT;

	message[count] = '\0';
	strim(message);

	pr_info("MiniMiku dork: %s\n", message);

	return count;
}

static const struct file_operations dork_fops = {
	.owner = THIS_MODULE,
	.write = dork_write,
};

static struct miscdevice dork_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dork",
	.fops = &dork_fops,
	.mode = 0666,
};

static int __init dork_init(void)
{
	int ret = misc_register(&dork_device);

	if (ret)
		pr_err("MiniMiku dork: registration failed: %d\n", ret);
	else
		pr_info("MiniMiku dork: registered\n");

	return ret;
}

static void __exit dork_exit(void)
{
	misc_deregister(&dork_device);
	pr_info("MiniMiku dork: unloaded\n");
}

module_init(dork_init);
module_exit(dork_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MiniMiku Developers");
MODULE_DESCRIPTION("MiniMiku /dev/dork text logging device");
MODULE_VERSION("1.0");