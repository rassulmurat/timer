#include <linux/init.h>
#include <linux/module.h>
#include "sysfs_timer.h"

static ssize_t showFile(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("TimerNum: %d\n", timerNum);
	sprintf(buf,"%d", timerNum);
	return 0;
}

static ssize_t storeFile(struct device *dev, struct device_attribute *attr,
	 const char *buf, size_t count)
{
	convert(buf, count, &timerNum);
	return count;
}

int convert(const char *buf, int count, int *res)
{
    int i = 0;
    int tmp;
    int sum = 0;
    while (i < count) {
        int power = count - 1 - i;
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
	.show = showFile,
	.store = storeFile
};

static int __init timer_init(void)
{
	kobj = kobject_create_and_add("timer_prnt", NULL);
	if (!kobj) {
		printk("ERROR: Could not create kobject");
		return -ENOMEM;
	}
	int res = sysfs_create_file(kobj, &kobjAttr);
	if (res) {
		printk("ERROR: Could not create file");
		return -ENOMEM;
	}
	return 0;
}

static void __exit timer_clean(void)
{
	sysfs_remove_file(kobj, &kobjAttr.attr);
	kobject_del(kobj);
	kobject_put(kobj);
}

module_init(timer_init);
module_exit(timer_clean);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Rassul Murat");