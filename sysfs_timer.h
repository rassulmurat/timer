#include <linux/sysfs.h>
#include <linux/kobject.h>

int timerNum;
struct kobject *kobj;

int convert(const char *buf, int count, int *res);

static int __init timer_init(void);
static void __exit timer_clean(void);