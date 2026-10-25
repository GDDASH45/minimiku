/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MINIMIKU_SAFEMEM_H
#define _LINUX_MINIMIKU_SAFEMEM_H

#include <linux/proc_fs.h>

int minimiku_safemem_init(struct proc_dir_entry *minimiku_sys_dir);

#endif