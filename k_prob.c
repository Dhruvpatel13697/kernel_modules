#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/rcupdate.h>

typedef struct kprobe st_kp;
typedef struct pt_regs pt_reg;
typedef struct task_struct Task;
typedef struct pid Pid;  

static st_kp kp;

static int __init hook_kill(void);
static void __exit exit_hook(void);
static int handler_pre(st_kp *, pt_reg *);

static int handler_pre(st_kp *p, pt_reg *regs) {
    pid_t pid = (pid_t)regs->dx;;
    int sig = (int)regs->si;
    Task *task = NULL; 
    Pid *_pid_struct;

    // pr_info("Register Dump: di=%lx, si=%lx, ax=%lx, bx=%lx, dx=%lx, cx=%lx, r8=%lx, r9=%lx",
    //     (unsigned long)regs->di, (unsigned long)regs->si,
    //     (unsigned long)regs->ax, (unsigned long)regs->bx,
    //     (unsigned long)regs->dx, (unsigned long)regs->cx,
    //     (unsigned long)regs->r8, (unsigned long)regs->r9);

    // pr_info("Intercepted sys_kill: pid = %d, sig = %d", pid, sig);

    _pid_struct = find_get_pid(pid);
    if (_pid_struct) {
        task = pid_task(_pid_struct, PIDTYPE_PID);
        put_pid(_pid_struct); 

        if (task) {
            pr_info("Task found: %s [PID: %d]", task->comm, task->pid);
        } else {
            pr_info("Task struct not found for PID: %d", pid);
        }
    } else {
        pr_info("PID struct not found for PID: %d", pid);
    }

    // Uncomment below if you want to block a signal
    // if (sig == 62) {
    //     pr_info("Blocking signal 62 for PID %d", pid);
    //     return -EPERM;
    // }

    return 0;
}

static int __init hook_kill(void) {
    pr_info("Loading kprobe for sys_kill");

    kp.symbol_name = "__x64_sys_kill";
    kp.pre_handler = handler_pre;

    if (register_kprobe(&kp) < 0) {
        pr_err("Failed to register kprobe");
        return -1;
    }

    pr_info("kprobe registered at %s", kp.symbol_name);
    return 0;
}

static void __exit exit_hook(void) {
    unregister_kprobe(&kp);
    pr_info("Unregistered kprobe");
}

module_init(hook_kill);
module_exit(exit_hook);

MODULE_LICENSE("GPL");
