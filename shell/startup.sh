su -l pi << EOF
cd
#wget --no-check-certificate https://raw.githubusercontent.com/bigtreetech/OctoBTT/master/install.sh
rm -rf OctoBTT
git clone https://github.com/bigtreetech/OctoBTT_V2_GUI_Sigma.git OctoBTT
sudo sed -i '/exit 0/ i sudo \/bin\/sh \/home\/pi\/OctoBTT\/shell\/install.sh' /etc/rc.local
sudo reboot
EOF
