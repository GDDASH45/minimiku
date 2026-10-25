
// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku Linux
 * init/mini.c
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/errno.h>

// ======= Version subsystem =============

#define MINIMIKU_VERSION "1.0"

static int minimiku_version_show(struct seq_file *m, void *v)
{
    seq_printf(m, "MiniMiku Linux %s\n", MINIMIKU_VERSION);
    seq_puts(m, "Codename: Brave Diva\n");

    return 0;
}

static int minimiku_version_open(struct inode *inode, struct file *file)
{
	return single_open(file, minimiku_version_show, NULL);
}

static const struct proc_ops minimiku_version_ops = {
	.proc_open = minimiku_version_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

// ======= Kernel version subsystem =============

#define MINIMIKU_KERNELVER "7.3.0"

static int minimiku_kernelversion_show(struct seq_file *m, void *v)
{
    seq_printf(m, "MiniMiku Linux Kernel %s\n", MINIMIKU_KERNELVER);

    return 0;
}

static int minimiku_kernelversion_open(struct inode *inode, struct file *file)
{
	return single_open(file, minimiku_kernelversion_show, NULL);
}

static const struct proc_ops minimiku_kernelversion_ops = {
	.proc_open = minimiku_kernelversion_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

// ======= Subsystem initialization =============

static struct proc_dir_entry *minimiku_proc_dir;

// Initialize the version subsystem
static int __init minimiku_version_init(void)
{
    if (!proc_create("version", 0444, minimiku_proc_dir,
                     &minimiku_version_ops)) {
        pr_err("MiniMiku: Failed to create /proc/MiniMiku/version\n");
        return -ENOMEM;
    }

    return 0;
}

// Initialize the kernel version subsystem
static int __init minimiku_kernelversion_init(void)
{
    if (!proc_create("kernelversion", 0444, minimiku_proc_dir,
                     &minimiku_kernelversion_ops)) {
        pr_err("MiniMiku: Failed to create /proc/MiniMiku/kernelversion\n");
        return -ENOMEM;
    }

    return 0;
}

// ======= MiniMiku initialization =============

static int __init minimiku_init(void)
{
    int ret;

    pr_info("MiniMiku: Brave Diva\n");
    pr_info("MiniMiku: Starting MiniMiku subsystem\n");

    minimiku_proc_dir = proc_mkdir("MiniMiku", NULL);
    if (!minimiku_proc_dir) {
        pr_err("MiniMiku: Failed to create /proc/MiniMiku\n");
        return -ENOMEM;
    }

    ret = minimiku_version_init();
    if (ret)
        goto err_remove_dir;

    ret = minimiku_kernelversion_init();
    if (ret)
        goto err_remove_version;

    pr_info("MiniMiku: All subsystems initialized\n");

    return 0;

err_remove_version:
    remove_proc_entry("version", minimiku_proc_dir);

err_remove_dir:
    remove_proc_entry("MiniMiku", NULL);
    return ret;
}

early_initcall(minimiku_init);
