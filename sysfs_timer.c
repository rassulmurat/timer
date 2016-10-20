#include <linux/sysfd.h>
#include <linux/init.h>
#include <linux/module.h>

int __init timer_init(void)
{

}

int __exit timer_clean(void)
{

}

module_init(timer_init);
module_exit(timer_clean);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Rassul Murat");