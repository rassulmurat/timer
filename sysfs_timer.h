#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/timer.h>

int timerNum;
struct kobject *kobj;
static struct timer_list my_timer;

int convert(const char *buf, int count, int *res);

static int __init timer_init(void);
static void __exit timer_clean(void);