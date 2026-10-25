// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku logging device
 *
 * Exposes MiniMiku-prefixed kernel messages through /dev/mlog.
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MLOG_NAME "mlog"
#define MLOG_CLASS "MiniMiku"
#define MLOG_PREFIX "MiniMiku:"

static dev_t mlog_dev;
static struct cdev mlog_cdev;
static struct class *mlog_class;
static struct device *mlog_device;

static int mlog_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int mlog_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations mlog_fops = {
	.owner = THIS_MODULE,
	.open = mlog_open,
	.release = mlog_release,
	.llseek = noop_llseek,
};

static int __init mlog_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&mlog_dev, 0, 1, MLOG_NAME);
	if (ret)
		return ret;

	cdev_init(&mlog_cdev, &mlog_fops);
	mlog_cdev.owner = THIS_MODULE;

	ret = cdev_add(&mlog_cdev, mlog_dev, 1);
	if (ret)
		goto err_unregister;

	mlog_class = class_create(MLOG_CLASS);
	if (IS_ERR(mlog_class)) {
		ret = PTR_ERR(mlog_class);
		goto err_cdev;
	}

	mlog_device = device_create(mlog_class, NULL, mlog_dev,
				    NULL, MLOG_NAME);
	if (IS_ERR(mlog_device)) {
		ret = PTR_ERR(mlog_device);
		goto err_class;
	}

	pr_info("MiniMiku: /dev/mlog initialized\n");
	return 0;

err_class:
	class_destroy(mlog_class);
err_cdev:
	cdev_del(&mlog_cdev);
err_unregister:
	unregister_chrdev_region(mlog_dev, 1);
	return ret;
}

static void __exit mlog_exit(void)
{
	device_destroy(mlog_class, mlog_dev);
	class_destroy(mlog_class);
	cdev_del(&mlog_cdev);
	unregister_chrdev_region(mlog_dev, 1);
}

module_init(mlog_init);
module_exit(mlog_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MiniMiku logging device");