// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku automatic boot panic
 *
 * Parameters:
 *   minimiku.autopanic
 *   minimiku.autopanicmsg="This message is blank"
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/panic.h>
#include <linux/string.h>

#define MINIMIKU_AUTOPANICMSG_MAX 256

static bool minimiku_autopanic_enabled;
static char minimiku_autopanicmsg[MINIMIKU_AUTOPANICMSG_MAX] =
	"MiniMiku: automatic boot panic requested";

static int __init minimiku_autopanic_setup(char *str)
{
	minimiku_autopanic_enabled = true;
	return 1;
}
__setup("minimiku.autopanic", minimiku_autopanic_setup);

static int __init minimiku_autopanicmsg_setup(char *str)
{
	if (str)
		strscpy(minimiku_autopanicmsg, str,
			sizeof(minimiku_autopanicmsg));

	return 1;
}
__setup("minimiku.autopanicmsg=", minimiku_autopanicmsg_setup);

static int __init minimiku_autopanic_init(void)
{
	if (minimiku_autopanic_enabled)
		panic("%s", minimiku_autopanicmsg);

	return 0;
}
late_initcall(minimiku_autopanic_init);