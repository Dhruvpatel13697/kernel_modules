/* sys call table */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/dirent.h>
#include <linux/syscalls.h>
#include <linux/kprobes.h>
#include <asm/paravirt.h>
#include <linux/uaccess.h>
#include "sct.h"

unsigned long *
find_syscall_table(void)
{
    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);

    kallsyms_lookup_name_t kallsyms_lookup_name;

    register_kprobe(&kp);

    kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;

    unregister_kprobe(&kp);

    __syscall_table = (unsigned long*)kallsyms_lookup_name("sys_call_table");

    return __syscall_table;
}


asmlinkage int newkill(pid_t pid, int sig) {
    pr_info("Blocked kill syscall! pid=%d, sig=%d\n", pid, sig);
    return -EPERM; // Permission denied
}


static inline void remove_protection(void) {
    __asm volatile (
        "mov %%cr0, %0\n\t"
        "and $~(1 << 16), %0\n\t"
        "mov %0, %%cr0"
        : "=r" (cr0) :: "memory"
    );

    pr_info(" after remove protection cr0 = %lx", cr0);
}

static inline void restore_protection(void) {
    __asm volatile (
        "mov %%cr0, %0\n\t"
        "or $(1 << 16), %0\n\t"
        "mov %0, %%cr0"
        : "=r" (cr0) :: "memory"
    );

    pr_info("after restore protection cr0 = %lx", cr0);
}

static int __init setup(void) {
    pr_info("Hooking kill syscall...\n");

    sys_call_table = find_syscall_table();
    if (!sys_call_table) {
        pr_err("sys_call_table not found!\n");
        return -1;
    }
    
    pr_info("sys_call_table = %p", sys_call_table);

    original_kill_64 = (void *) sys_call_table[62];

    pr_info("Original kill syscall: %p\n", original_kill_64);

    remove_protection();
    sys_call_table[62] = (unsigned long) newkill;
    restore_protection();

    pr_info("Kill syscall hooked!\n");
    return 0;
}

static void __exit cleanup(void) {
    pr_info("Restoring original kill syscall...\n");

    remove_protection();
    sys_call_table[62] = (unsigned long) original_kill_64;
    restore_protection();

    pr_info("Kill syscall restored.\n");
}

module_init(setup);
module_exit(cleanup);

MODULE_LICENSE("GPL");
