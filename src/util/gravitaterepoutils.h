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
#ifndef GRAVITATEREPOUTILS_H
#define GRAVITATEREPOUTILS_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QMap>
#include "enums.h"

namespace GravitateRepoUtils
{
    struct PropertyInfo
    {
        QString name;
        PropertyTarget target;
        QString formatString;
    };

    const QMap<PropertyType, MultiResProperty> PropertyFileMap = {
        { PropertyType::MEAN_CURVATURE, MultiResProperty::MEAN_CURVATURE},
        { PropertyType::SHAPE_INDEX, MultiResProperty::SHAPE_INDEX},
        { PropertyType::LIGHTNESS, MultiResProperty::LAB_COLOR},
        { PropertyType::SDF, MultiResProperty::SDF}
    };

    const QList<ModelResolution> AllModelResolutions  = {
        ModelResolution::RES_ORIGINAL,
        ModelResolution::RES_FULL,
        ModelResolution::RES_1M,
        ModelResolution::RES_100K,
        ModelResolution::RES_50K
    };

    PropertyType propertyType(const QString &fileName);

    PropertyType getSelectPropertyType(QWidget * parent = nullptr);

    QString propertyName(PropertyType type);

    PropertyInfo propertyInfo(PropertyType type);

    QString modelResolutionName(ModelResolution resolution);

    PropertyTarget propertyTarget(PropertyType type);
}

#endif // GRAVITATEREPOUTILS_H
