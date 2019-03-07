#!/bin/bash

# get hterm
wget http://www.der-hammer.info/terminal/hterm.tar.gz
tar xvfz hterm.tar.gz

# get libpng12
wget http://mirrors.kernel.org/ubuntu/pool/main/libp/libpng/libpng12-0_1.2.54-1ubuntu1_i386.deb
wget http://mirrors.kernel.org/ubuntu/pool/main/libp/libpng/libpng12-0_1.2.54-1ubuntu1_amd64.deb
sudo dpkg -i libpng12-0_1.2.54-1ubuntu1_*
rm libpng12-0_1.2.54-1ubuntu1_*
rm hterm.tar.gz* LICENSE.TXT changelog.txt

# fix missing dependencies
# sudo apt-get install -f

# install 32bit libs
sudo apt install libc6:i386 libstdc++6:i386 libgtk2.0-0:i386 libatk1.0-0:i386 libgdk-pixbuf2.0-0:i386 libfontconfig1:i386 libxext6:i386 libxrender1:i386 libxi6:i386 libxrandr2:i386 libxcursor1:i386 libxfixes3:i386 libpango-1.0-0:i386 libx11-6:i386 libglib2.0-0:i386 libxinerama1:i386 libsm6:i386 libexpat1:i386 zlib1g:i386

# install gtk2 stuff
sudo apt install gtk2-engines-pixbuf:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 libatk-adaptor:i386
