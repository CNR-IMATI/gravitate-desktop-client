# Deployment

Once the gravitate client has been successfully compiled, it is possible to
generate a distributable package containing all the required dependencies.

Currently only Linux deployment is supported.

## Linux

The distributable package format for Linux is the  [AppImage](http://appimage.org/) format.

Although in theory this package is compatible with all reasonably recent
distributions, the executable has been tested only on
Ubuntu 16.04 LTS (Xenial Xerus) 64-bit.

To create the AppImage run:
```
$ ./deploy-linux.sh <build_dir> <qt_path> <vtk_path>
```

where:
- `build_dir` is the build root directory
- `qt_path` is the main directory of the Qt binaries (e.g. ~/Qt/5.7/gcc_64)
- `vtk_path` is the main directory of the VTK binaries

The package will be generated on the path dist/linux/gravitate-client-x86-64.AppImage

The AppImage can be run with:

` $ ./gravitate-client-x86-64.AppImage`

No root privileges are required.

Note: the `dist/linux` directory contains the uncompressed files that compose
the distributable package.
