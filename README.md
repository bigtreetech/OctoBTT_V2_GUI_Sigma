# OctoBTT V2 Sigma Version 

![OctoBTT](assets/icon/BTT_Duck.svg "OctoBTT")

## Introduce
* New UI framework system, preview source code
* We are constantly adding background functionality.

## Software architecture
* Client and server architecture solution
* OctoPi -> OctoPrint service -> Printer

## Install the tutorial

> (The update and upgrade operations are not redundant)
### 1.  Install OctoPrint
* You can create a WPA profile in the boot directory **boot/wpa_supplicant.txt** to add WiFi connections

* If you are a BX machine model user, you will need to add the following fragment lines to the RaspberryPi **boot/config.txt** file to display them correctly on screen:
> * max_usb_current=1
> * hdmi_group=2
> * hdmi_mode=87
> * hdmi_cvt 1024 600 60
> * hdmi_drive=1

* SSH your PI on your computer for further action

### 2. Install OctoBTT : 
> Run Command (Installation):
* rm -f startup.sh ; wget --no-check-certificate https://raw.githubusercontent.com/bigtreetech/OctoBTT_V2_GUI_Sigma/master/shell/startup.sh && sudo /bin/sh startup.sh

> Contact Email: developer01@biqu3d.com

### 3. OctoBTT is entered automatically after you wait for the auto setup to complete and then restart

## Contribute

> 1. Fork the repository
> 2. New Feat_ XXX branch
> 3. Submit code
> 4. New Pull Request
