#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "hello_gki"

static ssize_t hello_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    static const char msg[] = "Halo dari modul kernel pertamaku!\n";
    size_t msg_len = sizeof(msg) - 1;
    if (*off >= msg_len) return 0;
    if (len > msg_len - *off) len = msg_len - *off;
    if (copy_to_user(buf, msg + *off, len)) return -EFAULT;
    *off += len;
    return len;
}

static const struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .read  = hello_read,
};

static struct miscdevice hello_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEVICE_NAME,
    .fops  = &hello_fops,
};

static int __init hello_init(void)
{
    int ret = misc_register(&hello_dev);
    if (ret)
        pr_err("hello_gki: gagal register, %d\n", ret);
    else
        pr_info("hello_gki: berhasil dimuat, baca lewat /dev/%s\n", DEVICE_NAME);
    return ret;
}

static void __exit hello_exit(void)
{
    misc_deregister(&hello_dev);
    pr_info("hello_gki: modul dibongkar\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modul belajar pertama untuk GKI 6.12.38");
