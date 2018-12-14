#!/bin/bash

# Inspired from:
# https://github.com/freemountain/quark/blob/master/tools/deploy_linux.sh

# Exits when a subcommand or pipeline returns a non-zero status
set -e

# General info
BUILD_DIR=@BUILD_DIR@
QT_PATH=@QTDIR@
VTK_PATH=@VTK_DIR@

# Check input parameters
if [[ -z $BUILD_DIR || -z $QT_PATH || -z $VTK_PATH ]]; then
  echo "ERROR: Please check your environment variables"
  echo "BUILD_DIR: $BUILD_DIR"
  echo "QT_PATH: $QT_PATH main directory of the Qt binaries (e.g. ~/Qt/5.7/gcc_64)"
  echo "VTK_PATH: $VTK_PATH"
  exit
fi

function make_app_image
{
  TARGET_APP="$1"

  # linuxdeployqt download
  LDQ_FILE="linuxdeployqt-continuous-x86_64.AppImage"
  LDQ_URL="https://github.com/probonopd/linuxdeployqt/releases/download/continuous/$LDQ_FILE"
  LDQ="$BUILD_DIR/deployment/tools/$LDQ_FILE"

  wget -P tools/ -N $LDQ_URL
  chmod a+x $LDQ

  DEPLOY_PATH="$BUILD_DIR/deployment"
  DIST_PATH="$DEPLOY_PATH/dist/linux"
  TARGET_PATH="$DIST_PATH/$TARGET_APP"
  TARGET_BIN="$DIST_PATH/$TARGET_APP"
  DEPLOY_CMD="$LDQ $TARGET_BIN -no-strip -bundle-non-qt-libs"

  rm -rf "$DIST_PATH"

  # # Copies only needed files from the build directory
  mkdir -p "$DIST_PATH"
  cp "$BUILD_DIR/$TARGET_APP"  "$DIST_PATH/$TARGET_APP"
  cp -r "$BUILD_DIR/python_modules" "$DIST_PATH/"
  cp -r "$BUILD_DIR/demo-data" "$DIST_PATH/"

  # Adds icon
  cp gravitate-icon.png "$DIST_PATH/icon.png"

  # Adds desktop file
cat << EOF > "$DIST_PATH/$TARGET_APP.desktop"
[Desktop Entry]
Type=Application
Name=$TARGET_APP
Exec=./AppRun %F
Icon=icon
Comment=Edit this default file
Terminal=true
EOF

  chmod +x "$DIST_PATH/$TARGET_APP.desktop"

  export PATH="$QT_PATH/bin:$PATH"
  export LD_LIBRARY_PATH="$VTK_PATH/lib"
  pushd "$DIST_PATH"
  $DEPLOY_CMD
  $DEPLOY_CMD -appimage
  popd

  DATE=`date +%Y%m%d`
  ARCH=`uname -i`
  mv "$DIST_PATH/$TARGET_APP-$ARCH.AppImage" "$PWD/$TARGET_APP-$DATE-$ARCH.AppImage"
}

make_app_image "gravitate-client"
make_app_image "standalone-mesh-viewer"
