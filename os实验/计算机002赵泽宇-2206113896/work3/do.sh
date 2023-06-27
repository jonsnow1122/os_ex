#do.sh   shell程序（安装字符设备）

sudo insmod ./chardev.ko  #安装设备驱动模块
lsmod | grep chardev  #显示安装的设备驱动模块
sudo mknod /dev/chardev0 c 101 0  #创建设备文件,含以下共10个
sudo mknod /dev/chardev1 c 101 1
sudo mknod /dev/chardev2 c 101 2
sudo mknod /dev/chardev3 c 101 3
sudo mknod /dev/chardev4 c 101 4
sudo mknod /dev/chardev5 c 101 5
sudo mknod /dev/chardev6 c 101 6
sudo mknod /dev/chardev7 c 101 7
sudo mknod /dev/chardev8 c 101 8
sudo mknod /dev/chardev9 c 101 9
sudo chmod 666 /dev/chardev*    //设置用户可以读写权限

