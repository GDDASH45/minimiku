// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku system hardware name
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#ifndef CONFIG_MINIMIKU_HWNAME
#define CONFIG_MINIMIKU_HWNAME ""
#endif

static struct kobject *minimiku_kobj;

static ssize_t systemname_show(struct kobject *kobj,
			       struct kobj_attribute *attr,
			       char *buf)
{
	return sysfs_emit(buf, "%s\n", CONFIG_MINIMIKU_HWNAME);
}

static struct kobj_attribute systemname_attr =
	__ATTR_RO(systemname);

static int __init minimiku_systemname_init(void)
{
	int ret;

	minimiku_kobj = kobject_create_and_add("MiniMiku", kernel_kobj);
	if (!minimiku_kobj)
		return -ENOMEM;

	ret = sysfs_create_file(minimiku_kobj, &systemname_attr.attr);
	if (ret) {
		kobject_put(minimiku_kobj);
		return ret;
	}

	return 0;
}
core_initcall(minimiku_systemname_init);