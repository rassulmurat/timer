#include <linux/init.h>
#include <linux/module.h>
#include "sysfs_timer.h"

static ssize_t showFile(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	sprintf(buf,"%d", timerNum);

	return 0;
}

static ssize_t storeFile(struct kobject *kobj, struct kobj_attribute *attr, 
			const char *buf, size_t count)
{
	convert(buf, count, &timerNum);
	printk("Timer will work %d times\n", timerNum);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
	return count;
}

void timer_callback(unsigned long data)
{
	if (timerNum > 0) {
		printk("The timer is still working: %d\n", timerNum);
		mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
		--timerNum;
	}
}

int convert(const char *buf, int count, int *res)
{
    int i = 0;
    int tmp;
    int sum = 0;
    while (i < count - 1) {
        int power = count - 2 - i;
        int rlpwr = 1;
        while (power > 0) {
            rlpwr = rlpwr * 10;
            --power;
        }
        tmp = buf[i] - '0';
        sum = sum + tmp*rlpwr;
        ++i;
    }
    *res = sum;
    return 0;
}

struct kobj_attribute kobjAttr = {
	.attr = {
		.name = "timer_prnt",
		.mode = 0777
	},
	.store = storeFile,
	.show = showFile,
};

static int __init timer_init(void)
{
	int res;
	kobj = kobject_create_and_add("timer_prnt", NULL);
	if (!kobj) {
		printk("ERROR: Could not create kobject");
		return -ENOMEM;
	}
	res = sysfs_create_file(kobj, &kobjAttr.attr);
	if (res) {
		printk("ERROR: Could not create file");
		return -ENOMEM;
	}
	setup_timer(&my_timer, timer_callback, 0);
	return 0;
}

static void __exit timer_clean(void)
{
	sysfs_remove_file(kobj, &kobjAttr.attr);
	kobject_del(kobj);
	kobject_put(kobj);
	del_timer(&my_timer);
}

module_init(timer_init);
module_exit(timer_clean);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Rassul Murat");