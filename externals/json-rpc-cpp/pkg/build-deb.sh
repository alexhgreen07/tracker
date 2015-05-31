#!/bin/bash

make
sudo make install

mkdir ./debian
cp -r --parents /usr/include/json-rpc-cpp ./debian
cp -r --parents /usr/lib/json-rpc-cpp ./debian

mkdir ./debian/DEBIAN
cp deb-control ./debian/DEBIAN/control

sudo chmod -R 755 *

fakeroot dpkg-deb --build debian
mv debian.deb libjson-rpc-cpp-0.1-1_amd64.deb


