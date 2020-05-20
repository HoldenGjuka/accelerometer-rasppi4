#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Quiic Accelerometer Sysfs Module");
MODULE_VERSION("0.1");

struct i2c_adapter* i2c_dev;
struct i2c_client* i2c_client;

static struct i2c_board_info __initdata board_info[] =  {
	{
		I2C_BOARD_INFO("MMA8452Q", 0x1d),
	}
};

static int __init accel_init(void) {
	s32 whoAmIResult;
	s32 xAxisValue;

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

exit:
	return 0;
}

static void __exit accel_exit(void){
	printk(KERN_DEBUG "accelerometer exit\n");
	i2c_unregister_device(i2c_client);
}

module_init(accel_init);
module_exit(accel_exit);
