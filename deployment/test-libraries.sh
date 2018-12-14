#!/bin/bash

rm -rf dist
mkdir dist

BUILD_DIR=../../build-imati-desktop-client-Desktop_Qt_5_7_0_GCC_64bit-Release
QT_DIR=~/Qt/5.7/gcc_64
VTK_DIR=~/vtk_build

./create-package.sh "$BUILD_DIR" "$QT_DIR"
mv $QT_DIR "$QT_DIR-disabled"
mv $VTK_DIR "$VTK_DIR-disabled"

cd dist
chmod +x gravitate-client.sh
./gravitate-client.sh

mv "$VTK_DIR-disabled" $VTK_DIR
mv "$QT_DIR-disabled" $QT_DIR
