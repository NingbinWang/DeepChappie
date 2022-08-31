#!/bin/bash
CURRENT_DIR=$(dirname $(readlink -f $0))
ARM_TOOLCHAIN=arm-ca9-linux-gnueabihf
if [ ! -d "lvgl-8.3.1" ];then
    unzip lvgl-8.3.1.zip
fi
cd lvgl-8.3.1
mkdir Build
cd Build
mkdir install
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$CURRENT_DIR/lvgl-8.3.1/Build/install -D SOFTFP=OFF -D CMAKE_CXX_COMPILER=$ARM_TOOLCHAIN-g++ -D CMAKE_C_COMPILER=$ARM_TOOLCHAIN-gcc ./..
make
cp $CURRENT_DIR/lvgl-8.3.1/Build/install/lib/*.so  $CURRENT_DIR/ -rf
