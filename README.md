# GRAVITATE desktop client

## Introduction

The GRAVITATE desktop client is one of the two client applications that compose
the GRAVITATE platform, the other being the web UI client.

The use of this client is aimed at researchers and curators in the Cultural
Heritage field, which are interested in exploring a large repository of 3D
scanned fragments with the purpose of performing the three main GRAVITATE
activities: ReAssembly, ReUnification and ReAssociation.

The GRAVITATE desktop client is a desktop application, organised as a dashboard
 for the activities related to the GRAVITATE project.

Changes between versions are documented in the [CHANGELOG](CHANGELOG.md) file

The GRAVITATE client is implemented in C++ language, with the following
dependencies:

* [Qt][qt-home] v5.9.1+ for the user interface under the [LGPL v3][lgpl3-license] license
* [VTK][vtk-home] v7.1.1+ for the 3D graphics functionalities under the [BSD 3-clause][bsd3-license] license
* [JSON for Modern C++][nlohmann-json] for reading/writing data in JSON syntax under the [MIT][mit-license] license
* [Serd][serd-home] and [Sord][sord-home] to parse RDF under the [ISC][isc-license] license
* [Fast C++ CSV Parser][csv-home] to parse CSV files under the [BSD 3-clause][bsd3-license] license
* Some classes from the [Qxt library][qxt-home] (Qt extensions) under the [BSD 3-clause][bsd3-license] license

Automated tests are implemented using the [Google Test][gtest-home] library.

[qt-home]: http://qt.io
[vtk-home]: http://www.vtk.org
[gtest-home]: https://github.com/google/googletest
[nlohmann-json]: https://nlohmann.github.io/json/
[serd-home]: https://drobilla.net/software/serd
[sord-home]: https://drobilla.net/software/sord
[csv-home]: https://github.com/ben-strasser/fast-cpp-csv-parser
[qxt-home]: https://bitbucket.org/libqxt/libqxt/wiki/Home

[lgpl3-license]: https://www.gnu.org/licenses/lgpl-3.0.en.html
[bsd3-license]: https://opensource.org/licenses/BSD-3-Clause
[mit-license]: https://opensource.org/licenses/MIT
[isc-license]: http://www.isc.org/downloads/software-support-policy/isc-license/

## Hardware and software requirements

A PC with a OpenGL-compatible graphics card with 3D acceleration.

Note for Linux users: 3D acceleration is better supported on Nvidia graphics
cards; however, Intel and AMD cards should work as well.

The GRAVITATE client has been tested on the following operating systems:

- Ubuntu Linux 16.04 LTS 64-bit
- Ubuntu Linux 18.04 LTS 64-bit
- Windows 7 64-bit
- Windows 10 64-bit

## Running the client

### Run on Windows

Unzip the **gravitate-client-Win64.zip** package, and run **gravitate-client.exe**.

### Run on Linux

Open a terminal and run the following commands:

```
$ chmod +x gravitate-client-x86_64.AppImage
$ ./gravitate-client-x86_64.AppImage
```

## Copyright and software license

Copyright(C) 2018: IMATI-GE / CNR

Consiglio Nazionale delle Ricerche
Istituto di Matematica Applicata e Tecnologie Informatiche
Sezione di Genova
IMATI-GE / CNR

All rights reserved.

This program is dual-licensed as follows:

1. You may use GRAVITATE Desktop Client as free software; you can
redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later  version.
In this case the program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License (http://www.gnu.org/licenses/gpl.txt)
for more details.                                            
2. You may use GRAVITATE Desktop Client as part of a commercial
software. In this case a proper agreement must be reached with the
Authors and with IMATI-GE/CNR based on a proper licensing contract.

## Compiling the source code

Instructions for compiling the source code are in the [INSTALL.md](INSTALL.md) file
