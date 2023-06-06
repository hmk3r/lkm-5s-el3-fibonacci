#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR("Lyubomir Kyorovski");
MODULE_DESCRIPTION("iPhone 5s EL3 demo");
MODULE_LICENSE("GPL");

#undef pr_fmt
#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

int init_module(void) {
    pr_info("Making SMC Call...\n");
    asm("smc #0");
    pr_info("Didn't crash!\n");
    return 0;
}

void cleanup_module(void) {
    pr_info("SMC module exiting.\n");
}
