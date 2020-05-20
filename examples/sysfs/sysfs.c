//borrowed from github: https://github.com/cirosantilli/linux-kernel-module-cheat/blob/41742640cf3e262213afd1f5f6ef92b15217f08d/kernel_module/sysfs.c
#include <linux/module.h>
#include <linux/kobject.h>

enum { FOO_SIZE_MAX = 4 };
static int foo_size;
static char foo_tmp[FOO_SIZE_MAX];

//This is a function that will handle the file read request
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
        char *buff)
{
    //this will just copy the internal kernel buffer over to the buffer provided by the userspace program
    strncpy(buff, foo_tmp, foo_size);
    //after this copy, we need to let the userspace program know how much data we put in it's buffer
    return foo_size;
}

//This is a function that will handle the file write request
static ssize_t foo_store(struct  kobject *kobj, struct kobj_attribute *attr,
        const char *buff, size_t count)
{
    //this kernel object only stores FOO_SIZE_MAX bytes, figure out which is smaller, FOO_SIZE_MAX or the data written to this file from userspace
    foo_size = min(count, (size_t)FOO_SIZE_MAX);
    //now we need to copy the buffer of bytes written to this file, into foo_tmp to store it in the kernel space
    strncpy(foo_tmp, buff, foo_size);
    return count;
}

//This is the actual attribute definition, this is being used to define the file name, and the READ/WRITE capabilities
static struct kobj_attribute foo_attribute =
    __ATTR(foo, S_IRUGO | S_IWUSR, foo_show, foo_store);
        //foo is the file name
        // S_IRUGO means it is readable by owner, group and all users
        // S_IWUSR means it is writable by the owner
        // foo_show is the function pointer to the function used to handle a file read request
        // foo_store is teh function pointer to the function used to handle a file write request

//This is an array of attributes, this only defines a single one, but you can add more
static struct attribute *attrs[] = {
    &foo_attribute.attr,
    //&some_other_attribute.attr, //like this - HINT HINT
    NULL,
};

//This is an attribute group, here it is just a container for an array of attributes
static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *kobj;

static int myinit(void)
{
    int ret;

    kobj = kobject_create_and_add("lkmc_sysfs", kernel_kobj);
    if (!kobj)
        return -ENOMEM;
    ret = sysfs_create_group(kobj, &attr_group);
    if (ret)
        kobject_put(kobj);
    return ret;
}

static void myexit(void)
{
    kobject_put(kobj);
}

module_init(myinit);
module_exit(myexit);
MODULE_LICENSE("GPL");