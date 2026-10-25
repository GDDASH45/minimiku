// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku Linux
 * MiniMiku driver initialization
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init minimiku_init(void)
{
	pr_info("MiniMiku: Initializing MiniMiku kernel components\n");
	pr_info("MiniMiku: Brave Diva\n");

	return 0;
}

static void __exit minimiku_exit(void)
{
	pr_info("MiniMiku: Kernel components unloaded\n");
}

module_init(minimiku_init);
module_exit(minimiku_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MiniMiku kernel initialization");
MODULE_AUTHOR("MiniMiku developers");