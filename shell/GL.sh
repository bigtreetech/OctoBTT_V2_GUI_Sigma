echo "Updating system resources ..."
sudo apt update -y
#sudo apt upgrade -y
echo "Done."

echo "Installing Dependencies ..."
sudo apt-get install libboost1.58-all-dev libudev-dev libinput-dev libts-dev libmtdev-dev libjpeg-dev libfontconfig1-dev libssl-dev libdbus-1-dev libglib2.0-dev libxkbcommon-dev -y
sudo apt-get install libgbm-dev libgl1-mesa-dri -y
echo "Done."

echo "Check GLdriver ..."
for package in gldriver-test libgl1-mesa-dri; do
    if [ "$(dpkg -l "$package" 2> /dev/null | tail -n 1 | cut -d ' ' -f 1)" != "ii" ]; then
      missing_packages="$package $missing_packages"
    fi
done
apt-get install $missing_packages -y
echo "Done."

echo "Set render ..."
sudo gpasswd -a pi render
echo "Done."

#Switch user roles
#su -l pi << EOF
#sudo cat >> /etc/profile << EOF
cat << EOF >> /etc/profile
export QT_QPA_EGLFS_ALWAYS_SET_MODE=1
export QT_QPA_PLATFORM=eglfs
EOF

sudo sed '/\[all\]/adtoverlay=vc4-fkms-v3d\ngpu_mem=256' /boot/config.txt


