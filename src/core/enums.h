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
#ifndef ENUMS_H
#define ENUMS_H

#include <QMetaType>

enum class PropertyType
{
    RGB_COLOR,
    MEAN_CURVATURE,
    SHAPE_INDEX,
    LIGHTNESS,
    SDF,
    UNDEFINED
};
Q_DECLARE_METATYPE(PropertyType)


enum class PropertyTarget
{
    VERTICES,
    TRIANGLES
};
Q_DECLARE_METATYPE(PropertyTarget)

enum class ModelResolution
{
    RES_ORIGINAL,
    RES_FULL,
    RES_1M,
    RES_100K,
    RES_50K
};
Q_DECLARE_METATYPE(ModelResolution)


enum class MultiResProperty
{
    MESH,
    FACETS,
    MEAN_CURVATURE,
    SHAPE_INDEX,
    LAB_COLOR,
    SDF,
    MINIMAL_BOUNDING_BOX
};
Q_DECLARE_METATYPE(MultiResProperty)


#endif // ENUMS_H
