su -l pi << EOF
sudo cp ~/sources.list /etc/apt/sources.list
sudo cp ~/raspi.list /etc/apt/sources.list.d/raspi.list

sudo apt update

sudo apt-get install ntfs-3g exfat-fuse usbmount -y

sudo sed -i 's/PrivateMounts[ ]*=[ ]*yes/PrivateMounts=no/g' /lib/systemd/system/systemd-udevd.service
EOF
