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
#include "scalarfieldproperty.h"
#include "meshpropertyreader.h"
#include <QString>
#include <QVector>

void load(const std::string &file, ScalarFieldProperty &property)
{
    MeshPropertyReader reader(property.type);

    if(!reader.readTxt(QString::fromStdString(file)))
    {
        throw std::runtime_error("Error reading property file: " + file );
    }

    property.data = reader.data().toStdVector();
}


MeanCurvatureProperty::MeanCurvatureProperty()
{
    type = PropertyType::MEAN_CURVATURE;
    target = PropertyTarget::VERTICES;
}


ShapeIndexProperty::ShapeIndexProperty()
{
    type = PropertyType::SHAPE_INDEX;
    target = PropertyTarget::VERTICES;

}


SDFProperty::SDFProperty()
{
    type = PropertyType::SDF;
    target = PropertyTarget::TRIANGLES;

}


LightnessProperty::LightnessProperty()
{
    type = PropertyType::LIGHTNESS;
    target = PropertyTarget::VERTICES;
}
