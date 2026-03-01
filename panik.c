#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include "git.h"

#define MAX_MSG 256

static int __init panik_init(void)
{
    struct file *f;
    char *buf;
    ssize_t ret;
    char *argv_sync[] = {"/bin/sync", NULL};
    char *envp_sync[] = {"HOME=/", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};

    /* allocate buffer */
    buf = kmalloc(MAX_MSG, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    /* open /tmp/panik */
    f = filp_open("/tmp/panik", O_RDONLY, 0);
    if (IS_ERR(f)) {
        printk(KERN_INFO "panik: /tmp/panik not found, doing nothing\n");
        kfree(buf);
        return 0;  // file missing → do nothing
    }

    /* read file */
    ret = kernel_read(f, buf, MAX_MSG - 1, 0);
    filp_close(f, NULL);

    if (ret <= 0) {
        printk(KERN_INFO "panik: /tmp/panik empty, doing nothing\n");
        kfree(buf);
        return 0;
    }

    buf[ret] = '\0';  // null terminate

    printk(KERN_ALERT "panik: syncing disks before panic\n");

    /* flush filesystems via user-space sync */
    call_usermodehelper(argv_sync[0], argv_sync, envp_sync, UMH_WAIT_PROC);

    /* trigger panic */
    panic("%s", buf);

    kfree(buf);
    return 0;  // never reached
}

module_init(panik_init);
MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("TheOddCell <me@oddcell.ca>");
MODULE_DESCRIPTION("I mispronounced KERNEL PANIC");
MODULE_VERSION(GIT_VERSION);
MODULE_ALIAS("panik");
