#del.sh   shell程序（卸载字符设备）

sudo rm /dev/chardev0
sudo rm /dev/chardev1
sudo rm /dev/chardev2
sudo rm /dev/chardev3
sudo rm /dev/chardev5
sudo rm /dev/chardev6
sudo rm /dev/chardev7
sudo rm /dev/chardev8
sudo rm /dev/chardev9

sudo rmmod chardev
dmesg  | tail -1   #倒数1条信息


