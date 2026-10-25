// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku Linux
 * fs/mikufs/mikufs.c
 *
 * MikuFS virtual filesystem
 */

#include <linux/fs.h>
#include <linux/fs_context.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pagemap.h>
#include <linux/errno.h>
#include <linux/string.h>

#define MIKUFS_MAGIC 0x4D494B55

// ======= File operations =============

static const char mikufs_hello[] =
    "Welcome to MikuFS!\n"
    "MiniMiku Linux virtual filesystem.\n";

static ssize_t mikufs_read_file(struct file *file,
                                char __user *buf,
                                size_t len,
                                loff_t *ppos)
{
    return simple_read_from_buffer(buf, len, ppos,
                                   mikufs_hello,
                                   sizeof(mikufs_hello) - 1);
}

static const struct file_operations mikufs_file_ops = {
    .read = mikufs_read_file,
    .llseek = default_llseek,
};

// ======= Superblock operations =============

static const struct super_operations mikufs_super_ops = {
    .statfs = simple_statfs,
};

// ======= Filesystem initialization =============

static struct inode *mikufs_make_inode(struct super_block *sb,
                                       umode_t mode)
{
    struct inode *inode;

    inode = new_inode(sb);
    if (!inode)
        return NULL;

    inode->i_ino = get_next_ino();
    inode->i_mode = mode;
    inode->i_uid = GLOBAL_ROOT_UID;
    inode->i_gid = GLOBAL_ROOT_GID;

    simple_inode_init_ts(inode);

    return inode;
}

static int mikufs_fill_super(struct super_block *sb,
                             struct fs_context *fc)
{
    struct inode *root_inode;
    struct inode *hello_inode;
    struct dentry *hello_dentry;

    sb->s_magic = MIKUFS_MAGIC;
    sb->s_op = &mikufs_super_ops;
    sb->s_maxbytes = MAX_LFS_FILESIZE;
    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;

    // ======= Root directory =============

    root_inode = mikufs_make_inode(sb, S_IFDIR | 0555);
    if (!root_inode)
        return -ENOMEM;

    root_inode->i_op = &simple_dir_inode_operations;
    root_inode->i_fop = &simple_dir_operations;

    sb->s_root = d_make_root(root_inode);
    if (!sb->s_root)
        return -ENOMEM;

    // ======= Hello file =============

    hello_dentry = d_alloc_name(sb->s_root, "hello");
    if (!hello_dentry)
        return -ENOMEM;

    hello_inode = mikufs_make_inode(sb, S_IFREG | 0444);
    if (!hello_inode) {
        dput(hello_dentry);
        return -ENOMEM;
    }

    hello_inode->i_fop = &mikufs_file_ops;
    i_size_write(hello_inode, sizeof(mikufs_hello) - 1);

    d_add(hello_dentry, hello_inode);

    return 0;
}

// ======= Mount API =============

static int mikufs_get_tree(struct fs_context *fc)
{
    return get_tree_nodev(fc, mikufs_fill_super);
}

static const struct fs_context_operations mikufs_context_ops = {
    .get_tree = mikufs_get_tree,
};

static int mikufs_init_fs_context(struct fs_context *fc)
{
    fc->ops = &mikufs_context_ops;
    return 0;
}

// ======= Filesystem registration =============

static struct file_system_type mikufs_type = {
    .owner = THIS_MODULE,
    .name = "mikufs",
    .init_fs_context = mikufs_init_fs_context,
    .kill_sb = kill_anon_super,
};

// ======= Module initialization =============

static int __init mikufs_init(void)
{
    int ret;

    ret = register_filesystem(&mikufs_type);
    if (ret) {
        pr_err("MikuFS: Failed to register filesystem: %d\n", ret);
        return ret;
    }

    pr_info("MikuFS: Filesystem registered\n");

    return 0;
}

static void __exit mikufs_exit(void)
{
    unregister_filesystem(&mikufs_type);

    pr_info("MikuFS: Filesystem unregistered\n");
}

module_init(mikufs_init);
module_exit(mikufs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GDDASH45");
MODULE_DESCRIPTION("MikuFS virtual filesystem for MiniMiku Linux");