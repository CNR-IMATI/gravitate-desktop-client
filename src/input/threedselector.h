/****************************************************************************
* GRAVITATE Desktop Client                                                  *
*                                                                           *
* Consiglio Nazionale delle Ricerche                                        *
* Istituto di Matematica Applicata e Tecnologie Informatiche                *
* Sezione di Genova                                                         *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Authors: Andrea Repetto                                                   *
* Copyright(C) 2018: IMATI-GE / CNR                                         *
* All rights reserved.                                                      *
*                                                                           *
* This program is dual-licensed as follows:                                 *
*                                                                           *
* (1) You may use GRAVITATE Desktop Client as free software; you can        *
* redistribute it and/or modify it under the terms of the GNU General       *
* Public License as published by the Free Software Foundation; either       *
* version 3 of the License, or (at your option) any later  version.         *
* In this case the program is distributed in the hope that it will be       *
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
* (2) You may use GRAVITATE Desktop Client as part of a commercial          *
* software. In this case a proper agreement must be reached with the        *
* Authors and with IMATI-GE/CNR based on a proper licensing contract.       *
*                                                                           *
****************************************************************************/
#ifndef THREEDSELECTOR_H
#define THREEDSELECTOR_H

#include <iostream>
#include <vector>

enum class IndexCompression
{
    NONE,
    RLE
};

enum class Orientation
{
    CW,
    CCW
};

class ThreeDSelectorReader
{
public:
    virtual void loadJson(std::istream &is) = 0;
};

class ThreeDSelectorWriter
{
public:
    virtual void saveJson(std::ostream &os) = 0;
};

struct ThreeDSurfaceSelector
{
    std::vector<size_t> indices;
};

struct ThreeDLineSelector
{
    struct Line
    {
        Orientation orientation;
        std::vector<size_t> indices;
    };

    std::vector<Line> lines;
};

struct ThreeDPointSelector
{
    std::vector<size_t> indices;
};

class ThreeDSurfaceSelectorWriter
{
public:
    ThreeDSurfaceSelectorWriter(ThreeDSurfaceSelector &selector, IndexCompression compression = IndexCompression::NONE);

    void saveJson(const std::string &fileName);

    void saveJson(std::ostream &os);

private:
    IndexCompression m_compression;
    ThreeDSurfaceSelector m_selector;
};

#endif // THREEDSELECTOR_H
