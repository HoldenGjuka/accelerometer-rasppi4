## Requires raspberry pi kernel headers to be installed
apt-get install raspberrypi-kernel-headers

#### To build kernel module
`make`

#### To install kernel module
`sudo insmod accel.ko`

#### To view kernel logs
`dmesg`

#### To remove kernel module
`sudo rmmod accel`