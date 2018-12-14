# Installation instructions

## Software requirements

### Compatible operating systems

* Ubuntu 16.04 (Xenial Xerus) 64 bit
* Windows 7
* Windows 10

### Development environment

A C++ compiler compatible with the C++11 standard:

  * Unix-based systems: gcc **v4.8.1** or higher
  * Windows systems: Visual C++ in **Visual Studio 2015** or higher

### Library dependencies

* [Qt][qt-home] v5.9.1 for the user interface
* [VTK][vtk-home] v7.1.1 for the 3D graphics functionalities

Other dependencies, specifically [JSON For Modern C++][nlohmann-json],
[Serd][serd-home], [Sord][sord-home] [Fast C++ CSV Parser][csv-home] and
[Google Test][gtest-home] are installed as part of the build process,
so there is no need to install them manually.

[qt-home]: http://qt.io
[vtk-home]: http://www.vtk.org
[gtest-home]: https://github.com/google/googletest
[nlohmann-json]: https://nlohmann.github.io/json/
[serd-home]: https://drobilla.net/software/serd
[sord-home]: https://drobilla.net/software/sord
[csv-home]: https://github.com/ben-strasser/fast-cpp-csv-parser

## Compiling the source code

### Ubuntu 16.04

Note: With other distributions, the procedure should be the same,
except the package manager commands (e.g. APT on Ubuntu/Debian,
RPM on Fedora, and so on).

Please refer to the distribution-specific documentation about the package
manager you have on your system.

#### Step 1: Basic requirements

Run the following commands:

```
$ sudo apt update
$ sudo apt install build-essential libgl1-mesa-dev libxt-dev
```

#### Step 2: Installing Qt

Download the Qt online installer from:

https://www.qt.io/download-open-source/

Go to the directory containing the downloaded installer and run:

```
$ chmod +x qt-unified-linux-x64-3.0.1-online.run
./qt-unified-linux-x64-3.0.1-online.run
```
Please note: version number of the installer program may differ; this is
unrelated with the Qt version that will be chosen later.

Once the installer program has run, click on the "+" near "Qt v5.9.1" and tick the
"Desktop gcc 64 bit" package. Also, Qt Creator IDE should be included by default.
It can be found on the "Tools" section below.

Once you have selected the required components, click on "Next" and wait the
installation to be completed.

#### Step 3: Installing VTK

VTK is the Visualisation ToolKit, a framework for scientific data visualisation.

VTK provides a Qt component called `QVTKOpenGLWidget`, which integrates the
VTK viewport in a Qt application. `QVTKOpenGLWidget` is not provided in the standard
binary package, so the VTK library must be compiled from sources.

The steps described in this guide are from
[The official VTK guide](http://www.vtk.org/Wiki/VTK/Configure_and_Build):

##### Download and install CMake

```
$ sudo apt update
$ sudo apt install cmake cmake-curses-gui
```

##### Download the VTK Source code

VTK source code can be downloaded at:

https://www.vtk.org/download/

Download the **VTK-7.1.1.zip** or **VTK-7.1.1.tar.gz** package.

##### Configure VTK with CMake

Create a new empty directory outside the VTK sources (in the following example `/path/to/VTK-build`) and run the following commands:

```
$ cd /path/to/VTK-build
$ cmake -DVTK_QT_VERSION:STRING=5 \
     -DQT_QMAKE_EXECUTABLE:PATH=/path/to/Qt/5.9.1/gcc_64/bin/qmake \
     -DVTK_Group_Qt:BOOL=ON \
     -DCMAKE_PREFIX_PATH:PATH=/path/to/Qt/5.9.1/gcc_64/lib/cmake  \
     -DBUILD_SHARED_LIBS:BOOL=ON
     /path/to/VTK
```

Note on paths:

* `/path/to/VTK-build` is the directory where you want to build VTK
* `/path/to/Qt` is the directory where you installed Qt, by default it is `$HOME/Qt/`
* `/path/to/VTK` is the directory of the VTK sources

If the configuration has been successful, you should have a Makefile in the `/path/to/VTK-build` directory.

##### Compile VTK

Once the CMake configuration step has been performed, the compilation can be
started with:

```
$ cd /path/to/VTK-build
$ make
```

#### Step 5: Compiling the gravitate-client application

After Qt and VTK have been compiled successfully, you can open the project with
Qt Creator (installed by default with Qt), by selecting the menu:

File --> Open File Or Project

and selecting the `CMakeLists.txt` file in the main directory of the project.

Go to Projects mode and set the `VTK_DIR` variable to `/path/to/VTK-build`, then
push button "Apply Configuration Changes".

Then go to menu

Build -> Build All

to compile the application.

## Deployment

To produce a binary distribution of the GRAVITATE desktop client, go to the
**deployment** directory inside the build folder, and run the
`deploy-windows.cmd` script on Windows, or the `deploy-linux.sh` on Linux.

In the case of Windows, a gravitate-client subdirectory will be created, and
all the binary files necessary to run the client will be
copied there.

In the case of Linux, an AppImage package will be generated.
