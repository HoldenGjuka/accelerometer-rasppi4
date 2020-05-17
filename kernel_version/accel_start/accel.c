#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Quiic Accelerometer Sysfs Module");
MODULE_VERSION("0.1");

struct i2c_adapter* i2c_dev;
struct i2c_client* i2c_client;

//NOTES:
//will need a separate file for each value, with only one value.
//Thus the module is dumb, does not perform analysis of any sort.

static struct i2c_board_info __initdata board_info[] =  {
	{
		I2C_BOARD_INFO("MMA8452Q", 0x1d),
	}
};

static int __init accel_init(void) {
	//Declare variables
	s32 whoAmIResult;
	s32 xAxisValue;
	char *dirname;
	char *fs;
	char *parent;
	struct kobject *example_kobject;
	struct kobject fs_kobject;
	struct kobject *parent_kobject;
	//int fd;

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

	i2c_smbus_write_byte_data(i2c_client, 0x2A, 0x01);
	whoAmIResult = i2c_smbus_read_byte_data(i2c_client, 0x0D);
	printk(KERN_DEBUG "who am I result: 0x%x\n", whoAmIResult);
	xAxisValue = i2c_smbus_read_byte_data(i2c_client, 0x01);
	printk(KERN_DEBUG "x-axis value: 0x%x\n", xAxisValue);
	
    dirname = "test_dir_TEST";
	
	//make parent kobject for fs folder (sys/fs)
	fs = "fs";
	parent = "sys";
	fs_kobject = {fs, parent};
	parent_kobject = &fs_kobject;
    
	example_kobject = kobject_create_and_add(dirname, fs_kobject);

	//fd = open("sys/fs/test_dir_TEST", O_WRONLY);

exit:
	return 0;
}

static void __exit accel_exit(void){
	printk(KERN_DEBUG "accelerometer exit\n");
	i2c_unregister_device(i2c_client);
}

module_init(accel_init);
module_exit(accel_exit);
