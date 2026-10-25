/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MINIMIKU_STAT_H
#define _LINUX_MINIMIKU_STAT_H

#include <linux/proc_fs.h>

int minimiku_stat_init(struct proc_dir_entry *minimiku_proc_dir);

#endif