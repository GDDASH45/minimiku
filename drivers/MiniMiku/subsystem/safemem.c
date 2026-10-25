// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku Linux
 * drivers/MiniMiku/subsystem/safemem.c
 *
 * MiniMiku Safe Memory Subsystem
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/errno.h>
#include <linux/minimiku_safemem.h>

// ======= Safe memory subsystem =============

static int minimiku_safemem_show(struct seq_file *m, void *v)
{
    struct sysinfo info;

    si_meminfo(&info);

    seq_puts(m, "MiniMiku Safe Memory Status\n");
    seq_printf(m, "Total RAM: %lu kB\n",
               (info.totalram * info.mem_unit) / 1024);
    seq_printf(m, "Free RAM: %lu kB\n",
               (info.freeram * info.mem_unit) / 1024);
    seq_printf(m, "Shared RAM: %lu kB\n",
               (info.sharedram * info.mem_unit) / 1024);
    seq_printf(m, "Buffer RAM: %lu kB\n",
               (info.bufferram * info.mem_unit) / 1024);

    return 0;
}

static int minimiku_safemem_open(struct inode *inode, struct file *file)
{
    return single_open(file, minimiku_safemem_show, NULL);
}

static const struct proc_ops minimiku_safemem_ops = {
    .proc_open    = minimiku_safemem_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

// ======= Subsystem initialization =============

int minimiku_safemem_init(struct proc_dir_entry *minimiku_sys_dir)
{
    if (!proc_create("safemem", 0444, minimiku_sys_dir,
                     &minimiku_safemem_ops)) {
        pr_err("MiniMiku: Failed to create /proc/MiniMiku/sys/safemem\n");
        return -ENOMEM;
    }

    pr_info("MiniMiku: Safe memory subsystem initialized\n");

    return 0;
}