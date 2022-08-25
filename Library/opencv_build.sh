#!/bin/bash
CURRENT_DIR=$(dirname $(readlink -f $0))
ARM_TOOLCHAIN=arm-ca9-linux-gnueabihf
if [ ! -d "opencv-4.5.4" ];then
    unzip opencv-4.5.4.zip
fi
cd opencv-4.5.4
mkdir Build
cd Build
mkdir install
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$CURRENT_DIR/opencv-4.5.4/Build/install -D SOFTFP=OFF -D CMAKE_CXX_COMPILER=$ARM_TOOLCHAIN-g++ -D CMAKE_C_COMPILER=$ARM_TOOLCHAIN-gcc ./..
make
cp $CURRENT_DIR/opencv-4.5.4/Build/install/lib/*.so  $CURRENT_DIR/ -rf