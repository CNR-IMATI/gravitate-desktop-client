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
#include "propertyselector.h"
#include "gravitaterepoutils.h"

PropertySelector::PropertySelector(QWidget *parent)
    : QComboBox (parent)
{
    reset();

    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this] (int index) {
        auto prop = qvariant_cast<PropertyType>(itemData(index));
        emit propertyTypeChanged(prop);
    });

    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

PropertyType PropertySelector::currentProperty()
{
    if(currentData().isNull())
        return PropertyType::UNDEFINED;

    return qvariant_cast<PropertyType>(currentData());
}

void PropertySelector::addProperty(PropertyType type)
{
    addItem(GravitateRepoUtils::propertyName(type),
            qVariantFromValue(type)
            );
    update();
}

void PropertySelector::setCurrentProperty(PropertyType type)
{
    blockSignals(true);

    auto index = indexOf(type);

    if(index >= 0)
    {
        setCurrentIndex(index);
    }

    blockSignals(false);
}

bool PropertySelector::hasProperty(PropertyType type)
{
    return indexOf(type) != -1;
}

int PropertySelector::indexOf(PropertyType type)
{
    for(int i = 0; i < count(); i++)
    {
        QVariant value = itemData(i);

        if(value.isNull())
            continue;

        PropertyType current = qvariant_cast<PropertyType>(value);
        if(current == type)
        {
            return i;
        }
    }
    return -1;
}

void PropertySelector::reset()
{
    clear();
    addProperty(PropertyType::RGB_COLOR);
}
