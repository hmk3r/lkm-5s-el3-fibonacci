#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>


MODULE_AUTHOR("Lyubomir Kyorovski");
MODULE_DESCRIPTION("iPhone 5s EL3 fibonacci demo");
MODULE_LICENSE("GPL");

#undef pr_fmt
#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

#define BUF_SIZE 200

static struct proc_dir_entry *ent;

uint64_t last_n;
uint64_t last_res;
uint64_t last_param_reg_check;

int el3_fib_svc_call(uint64_t n) {
    last_n = n;
    asm volatile ("mov x1, %0" ::"r"(last_n) :);

    __asm__ ("smc #0");

    asm volatile ("mov %0, x2" : "=r"(last_res) ::);
    asm volatile ("mov %0, x1" : "=r"(last_param_reg_check) ::);

    return 0;
}


static ssize_t proc_fs_write_handler(struct file *file, const char __user *user_buf, size_t length, loff_t *offset) 
{
    int n_params;
    uint64_t n;
    ssize_t len;
    char buf[BUF_SIZE];

    pr_info("ProcFS write handler invoked");

    if(*offset > 0 || length > BUF_SIZE) return EFAULT;

    if(copy_from_user(buf, user_buf, length)) return EFAULT;
    
    n_params = sscanf(buf, "%llu", &n);
    if(n_params != 1) return EFAULT;

    pr_info("Calling EL3 service for fibonacci computation");

    el3_fib_svc_call(n);

    pr_info("The %llu-th fibonacci number is %llu\n", n, last_res);
    pr_info("Parameter register(x1) check after EL change:\n");
    pr_info("\tBefore SMC call: 0x%016llX\n", last_n);
    pr_info("\tAfter SMC call: 0x%016llX\n", last_param_reg_check);
    
    len = strlen(buf);
    *offset = len;
    return len;

}

static ssize_t proc_fs_read_handler(struct file *file, char __user *user_buf, size_t length, loff_t *offset) 
{
    char buf[BUF_SIZE];
	ssize_t len = 0;
    pr_info("ProcFS read handler invoked");

    if(*offset > 0 || length < BUF_SIZE) return 0;

    len += sprintf(buf, "F(%llu) = %llu\n", last_n, last_res);
    len += sprintf(buf + len, "Param reg check(x1):\n");
    len += sprintf(buf + len, "\tPre-SMC:  0x%016llX\n", last_n);
    len += sprintf(buf + len, "\tPost-SMC: 0x%016llX\n", last_param_reg_check);
    
	
	if(copy_to_user(user_buf, buf, len)) return EFAULT;
    
	*offset = len;

	return len;
}


static struct proc_ops proc_fs_ops = 
{
	.proc_read = proc_fs_read_handler,
	.proc_write = proc_fs_write_handler,
};

int init_module(void) {
    last_n = 0;
    last_res = 0;
    last_param_reg_check = 0;

    pr_info("EL3 fibonacci service module init\n");

    ent = proc_create("el3_fib_svc", 0660, NULL, &proc_fs_ops);
    return 0;
}

void cleanup_module(void) {
    proc_remove(ent);
    pr_info("EL3 fibonacci service module exiting.\n");
}
