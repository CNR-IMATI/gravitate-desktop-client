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
#include "gravitaterepoutils.h"
#include <QInputDialog>
#include <QMap>

namespace GravitateRepoUtils
{

namespace {
    const QMap<ModelResolution, QString> ModelResolutionString = {
        { ModelResolution::RES_ORIGINAL, QObject::tr("Original model") },
        { ModelResolution::RES_FULL, QObject::tr("Full Res (clean)") },
        { ModelResolution::RES_1M, QObject::tr("1M Vertices")},
        { ModelResolution::RES_100K, QObject::tr("100K Vertices")},
        { ModelResolution::RES_50K, QObject::tr("50K Vertices")}
    };


    // This list contains the properties that can be chosen manually
    // for allowing the corresponding color transfer
    const QList<PropertyType> PropertiesForManualSelection = {
        PropertyType::MEAN_CURVATURE,
        PropertyType::SHAPE_INDEX,
        PropertyType::LIGHTNESS,
        PropertyType::SDF
    };


    const QMap<PropertyType, PropertyInfo> PropInfo = {
        { PropertyType::RGB_COLOR, { QObject::tr("RGB"), PropertyTarget::VERTICES, "% -6.3g" } },
        { PropertyType::MEAN_CURVATURE, { QObject::tr("Mean Curvature"), PropertyTarget::VERTICES, "% -6.3g" } },
        { PropertyType::SHAPE_INDEX, { QObject::tr("Shape Index"), PropertyTarget::VERTICES, "% -6.3g" } },
        { PropertyType::LIGHTNESS, { QObject::tr("Lightness"), PropertyTarget::VERTICES, "% -6.3g" } },
        { PropertyType::SDF, { QObject::tr("Shape Diameter Function"), PropertyTarget::TRIANGLES, "%#-6.3g cm" } }
    };

    const QMap<QString, PropertyType> PropertyDefaultSuffix = {
        { "_curvM", PropertyType::MEAN_CURVATURE },
        { "_SI", PropertyType::SHAPE_INDEX },
        { "_LAB", PropertyType::LIGHTNESS, },
        { "_SDF", PropertyType::SDF }
    };
}

PropertyType propertyType(const QString &fileName)
{
    for(auto suffix : PropertyDefaultSuffix.keys())
    {
        if(fileName.contains(suffix))
        {
            return PropertyDefaultSuffix.value(suffix);
        }
    }
    return PropertyType::UNDEFINED;
}

PropertyType getSelectPropertyType(QWidget *parent)
{

    QStringList items;
    for(auto p : PropertiesForManualSelection)
    {
        items << propertyName(p);
    }

    bool ok;
    QString selectedItem = QInputDialog::getItem(parent, "Select a property", "Property:", items, 0, false, &ok);

    if(!ok || selectedItem.isEmpty())
    {
        return PropertyType::UNDEFINED;
    }

    int index = items.indexOf(selectedItem);
    return PropertiesForManualSelection.value(index, PropertyType::UNDEFINED);
}

QString propertyName(PropertyType type)
{
    if(!PropInfo.contains(type))
        return QObject::tr("N/A");

    return PropInfo.value(type).name;
}

PropertyTarget propertyTarget(PropertyType type)
{
    // AR: Consider vertices as the default value
    if(!PropInfo.contains(type))
        return PropertyTarget::VERTICES;

    return PropInfo.value(type).target;
}

QString modelResolutionName(ModelResolution resolution)
{
    return ModelResolutionString.value(resolution, QObject::tr("N/A"));
}

PropertyInfo propertyInfo(PropertyType type)
{
    if(!PropInfo.contains(type))
        return PropertyInfo();

    return PropInfo.value(type);
}


}
