// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku Linux
 * drivers/MiniMiku/subsystem/stat.c
 *
 * MiniMiku status subsystem
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/errno.h>
#include <linux/utsname.h>

#include <linux/minimiku_stat.h>
#include <linux/minimiku_safemem.h>

// ======= Status subsystem =============

#define MINIMIKU_STAT_VERSION "1.0"
#define MINIMIKU_STAT_CODENAME "Brave Diva"

static struct proc_dir_entry *minimiku_sys_dir;

static int minimiku_stat_show(struct seq_file *m, void *v)
{
    seq_puts(m, "MiniMiku Linux Status\n");
    seq_printf(m, "Version: %s\n", MINIMIKU_STAT_VERSION);
    seq_printf(m, "Codename: %s\n", MINIMIKU_STAT_CODENAME);
    seq_printf(m, "Kernel: %s\n", init_uts_ns.name.release);
    seq_puts(m, "Status: Running\n");

    return 0;
}

static int minimiku_stat_open(struct inode *inode, struct file *file)
{
    return single_open(file, minimiku_stat_show, NULL);
}

static const struct proc_ops minimiku_stat_ops = {
    .proc_open    = minimiku_stat_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

// ======= Subsystem initialization =============

int minimiku_stat_init(struct proc_dir_entry *minimiku_proc_dir)
{
    int ret;

    minimiku_sys_dir = proc_mkdir("sys", minimiku_proc_dir);
    if (!minimiku_sys_dir) {
        pr_err("MiniMiku: Failed to create /proc/MiniMiku/sys\n");
        return -ENOMEM;
    }

    if (!proc_create("status", 0444, minimiku_sys_dir,
                     &minimiku_stat_ops)) {
        pr_err("MiniMiku: Failed to create /proc/MiniMiku/sys/status\n");
        ret = -ENOMEM;
        goto err_remove_sys;
    }

    ret = minimiku_safemem_init(minimiku_sys_dir);
    if (ret)
        goto err_remove_status;

    pr_info("MiniMiku: Status subsystem initialized\n");

    return 0;

err_remove_status:
    remove_proc_entry("status", minimiku_sys_dir);

err_remove_sys:
    remove_proc_entry("sys", minimiku_proc_dir);
    return ret;
}