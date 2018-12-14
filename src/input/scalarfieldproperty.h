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
#ifndef SCALARFIELDPROPERTY_H
#define SCALARFIELDPROPERTY_H

#include <string>
#include "enums.h"

class ScalarFieldProperty
{
public:
    PropertyType type;
    std::vector<double> data;
    PropertyTarget target;
};

class MeanCurvatureProperty : public ScalarFieldProperty
{
public:
    MeanCurvatureProperty();
};

class ShapeIndexProperty : public ScalarFieldProperty
{
public:
    ShapeIndexProperty();
};

class SDFProperty : public ScalarFieldProperty
{
public:
    SDFProperty();

    float averageThichkness = 0.0f;
};

class LightnessProperty : public ScalarFieldProperty
{
public:
    LightnessProperty();
};

void load(const std::string &file, ScalarFieldProperty &property);


// propertyType function
template<class P>
PropertyType propertyType()
{
    return PropertyType::UNDEFINED;
}

template<>
inline PropertyType propertyType<SDFProperty>()
{
    return PropertyType::SDF;
}

template<>
inline PropertyType propertyType<MeanCurvatureProperty>()
{
    return PropertyType::MEAN_CURVATURE;
}

template<>
inline PropertyType propertyType<ShapeIndexProperty>()
{
    return PropertyType::SHAPE_INDEX;
}


template<>
inline PropertyType propertyType<LightnessProperty>()
{
    return PropertyType::LIGHTNESS;
}


#endif // SCALARFIELDPROPERTY_H
