#!/bin/bash
sudo cp sysoff /etc/init.d
sudo chmod +x /etc/init.d/sysoff
sudo rm /etc/rc0.d/K01sysoff
sudo ln -s /etc/init.d/sysoff /etc/rc0.d/K01sysoff
sudo update-rc.d sysoff defaults 1 0
cd
sudo apt-get install g++ libusb-1.0-0-dev
cd mp751
sudo make clean
sudo make
sudo rm mp751_POFF.o hid-libusb.o