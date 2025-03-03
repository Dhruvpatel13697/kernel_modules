/* hello world kernel module*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

static int __init setup(void){
    pr_info("This is init kernel function");
    return 0;
}

static void __exit unload(void){
    pr_info("kernel module unload");
}

module_init(setup);
module_exit(unload);

MODULE_LICENSE("GPL");