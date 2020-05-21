#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/i2c.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Quiic Accelerometer Sysfs Module");
MODULE_VERSION("0.1");

struct i2c_adapter* i2c_dev;
struct i2c_client* i2c_client;

static struct i2c_board_info __initdata board_info[] =  {
	{ I2C_BOARD_INFO("MMA8452Q", 0x1d), }
};

enum { FOO_SIZE_MAX = 4 };
static int foo_size;
static char foo_tmp[FOO_SIZE_MAX];

//This is a function that will handle the file read request
static ssize_t read_data(struct kobject *kobj, struct kobj_attribute *attr,
        char *buff)
{
    strncpy(buff, foo_tmp, foo_size);
    return foo_size;
}

//This is a function that will handle the file write request
static ssize_t store_data(struct  kobject *kobj, struct kobj_attribute *attr,
        const char *buff, size_t count)
{
    foo_size = min(count, (size_t)FOO_SIZE_MAX);
    strncpy(foo_tmp, buff, foo_size);
    return count;
}

static struct task_struct *thread_st;
// Function executed by kernel thread
static int thread_fn(void *i2c_client)
{
    while (1)
    {
        //TEST ACCELEROMETER READ
	    xAxisValue = i2c_smbus_read_byte_data(i2c_client, 0x01);
	    printk(KERN_DEBUG "x-axis value: 0x%x\n", xAxisValue);
    }
    printk(KERN_INFO "Thread Stopping\n");
    return 0;
}

static struct kobj_attribute x_attribute =
    __ATTR(x, S_IRUGO | S_IWUSR, read_data, store_data);

static struct kobj_attribute y_attribute =
    __ATTR(y, S_IRUGO | S_IWUSR, read_data, store_data);

static struct kobj_attribute z_attribute =
    __ATTR(z, S_IRUGO | S_IWUSR, read_data, store_data);

static struct attribute *attrs[] = {
    &x_attribute.attr,
    &y_attribute.attr,
    &z_attribute.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *kobj;

static int __init accel_init(void) {
	s32 whoAmIResult;
	s32 xAxisValue;
    //INITIALIZE ADAPTOR CONNECTION
	printk(KERN_DEBUG "accelerometer init\n");
	i2c_dev = i2c_get_adapter(1);
	if(!i2c_dev) {
		printk(KERN_INFO "FAIL: could not get i2c adapter\n");
		goto exit;
	}
	i2c_client = i2c_new_device(i2c_dev, board_info);
	if(!i2c_client) {
		printk(KERN_INFO "FAIL: could not get i2c client\n");
		goto exit;
	}
    //TEST ACCELEROMETER READ
	i2c_smbus_write_byte_data(i2c_client, 0x2A, 0x01);
	whoAmIResult = i2c_smbus_read_byte_data(i2c_client, 0x0D);
	printk(KERN_DEBUG "who am I result: 0x%x\n", whoAmIResult);

	int ret;
    kobj = kobject_create_and_add("accel_data", kernel_kobj);
    if (!kobj)
        return -ENOMEM;
    ret = sysfs_create_group(kobj, &attr_group);
    if (ret)
        kobject_put(kobj);
    
    //CREATE THREAD
    printk(KERN_INFO "Creating Thread\n");
    thread_st = kthread_run(thread_fn, i2c_client, "x_thread");
    if (thread_st)
        printk(KERN_INFO "Thread Created successfully\n");
    else
        printk(KERN_ERR "Thread creation failed\n");

    exit:
	    return 0;
}

static void __exit accel_exit(void){
	kobject_put(kobj);
    if (thread_st) { kthread_stop(thread_st); }
    kthread_stop(thread_st);
    i2c_unregister_device(i2c_client);
	printk(KERN_DEBUG "accelerometer exit\n");
}

module_init(accel_init);
module_exit(accel_exit);