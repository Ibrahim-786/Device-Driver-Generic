/**
* @file hello.c
* @author Ibrahim pasha 
* @date 10 Sept 2022
* @version 0.1
* @brief An introductory "Hello World!" loadable kernel
* module (LKM) that can display a message in the /var/log/kern.log
* file when the module is loaded and removed. The module can accept
* an argument when it is loaded -- the name, which appears in the
* kernel log files.
*/
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/init.h>  /* Needed for the macros */

///< The license type -- this affects runtime behavior
MODULE_LICENSE("GPL");

///< The author -- visible when you use modinfo
MODULE_AUTHOR("Ibrahim pasha");

///< The description -- see modinfo
MODULE_DESCRIPTION("A simple Hello world LKM!");

///< The version of the module
MODULE_VERSION("0.1");

static int __init hello_start(void)
{
   printk("<1> Loading hello module...\n");
   printk("Hello world\n");
   return 0;
}

static void __exit hello_end(void)
{
   printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
