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
#include "resolutionselector.h"
#include "gravitaterepoutils.h"

ResolutionSelector::ResolutionSelector(QWidget *parent)
    : QComboBox(parent)
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this] (int index) {
        auto res = qvariant_cast<ModelResolution>(itemData(index));
        emit resolutionChanged(res);
    });
}

ModelResolution ResolutionSelector::currentResolution()
{
    auto current = currentIndex();
    return qvariant_cast<ModelResolution>(itemData(current));
}

void ResolutionSelector::initOptions(QList<ModelResolution> resolutions)
{
    m_resolutions = resolutions;

    clear();

    for(auto meshRes : resolutions)
    {
        QString itemName = GravitateRepoUtils::modelResolutionName(meshRes);
        addItem(itemName, qVariantFromValue(meshRes));
    }
}

void ResolutionSelector::setResolution(ModelResolution resolution)
{
    auto index = m_resolutions.indexOf(resolution);

    setCurrentIndex(index);
}
