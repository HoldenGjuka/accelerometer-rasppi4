# Minimal sysfs example

## Usage
#### build
`make`

#### install kernel module
`sudo insmod sysfs.ko`

#### store something in the kernel to sysfs path (requires sudo permissions)
`sudo sh -c 'printf 1234 > /sys/kernel/lkmc_sysfs/foo'`

#### retreive what was stored in the kernel
`sudo cat /sys/kernel/lkmc_sysfs/foo`

#### remove kernel module
`sudo rmmod sysfs`
