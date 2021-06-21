#!/bin/sh
echo "Updating system resources ..."
sudo apt update -y
#sudo apt upgrade -y
echo "Done."

echo "Installing Dependencies ..."
sudo apt install qt5-default qtcreator python3-pyqt5-dbg libqt5svg5 libqt5svg5-dev python3-pyqt5.qtsvg qtsvg5-doc libqt5websockets5 libqt5websockets5-dev ttf-wqy-zenhei -y
sudo apt-get install qml-module-qtquick-controls2 -y
sudo apt-get install qtdeclarative5-dev -y
echo "Done."

sudo /bin/sh autologin.sh
sudo /bin/sh automount.sh
sudo /bin/sh GL.sh


#Switch user roles
su -l pi << EOF
#Add sudo access for wireless configurations
echo "Setting up Sudo for wifi configuration..."
#sudo touch /etc/sudoers.d/OctoBTT
#sudo echo "pi ALL=(ALL) NOPASSWD: /sbin/iwlist, /sbin/wpa_cli, /sbin/iwconfig, /sbins/ifconfig" > /etc/sudoers.d/OctoBTT
sudo cp OctoBTT/OctoBTT.sudoers.d /etc/sudoders.d/OctoBTT
echo "Done."

#echo "Download OctoBTT ..."
#cd /home/pi
#rm -rf OctoBTT
#git clone https://github.com/bigtreetech/OctoBTT.git
#git clone https://gitee.com/Shine6Z/OctoBTT.git
#echo "Done."

echo "Installing OctoBTT ..."
cd /home/pi/OctoBTT
#mkdir projectfile
#make clean
#cp OctoBTT.pro projectfile/OctoBTT.pro
qmake OctoBTT.pro
#cp projectfile/OctoBTT.pro OctoBTT.pro
#rm -r -f projectfile
if [ ! -f "$OctoBTT" ]; then
 rm OctoBTT
 make clean
fi
make -j$(cat /proc/cpuinfo | grep processor | wc -l) && make clean
EOF

echo "Setting up Autostart ..."
#sudo sed -i '/exit 0/ i su -l pi -c "startx -- -nocursor"' /etc/rc.local
sudo sed -i 's/sudo \/bin\/sh \/home\/pi\/OctoBTT\/shell\/install.sh/su -l pi -c \"cd \/home\/pi\/OctoBTT \&\& .\/OctoBTT -platform eglfs\"/g' /etc/rc.local
echo "Done."

sudo reboot
