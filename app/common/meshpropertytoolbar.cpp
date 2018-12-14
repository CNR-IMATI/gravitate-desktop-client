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
#include <QLabel>
#include "meshpropertytoolbar.h"
#include "gravitaterepoutils.h"
#include "helphandler.h"

MeshPropertyToolbar::MeshPropertyToolbar(QWidget *parent) : QToolBar(parent)
{
    auto propertyLabel = new QLabel(tr("Property:"));
    addWidget(propertyLabel);

    propertySelector = new PropertySelector(this);
    addWidget(propertySelector);

    addSeparator();

    facetCheckBox = new QCheckBox(this);
    facetCheckBox->setText(tr("Facets"));
    addWidget(facetCheckBox);

    // FIXME: annotationCheckBox not used (still using "Annotation Mode" button)
    annotationCheckBox = new QCheckBox(this);
    annotationCheckBox->setText(tr("Annotations"));
    addWidget(annotationCheckBox);

    boundingBoxCheckBox = new QCheckBox(this);
    boundingBoxCheckBox->setText(tr("Minimal Bounding Box"));
    addWidget(boundingBoxCheckBox);
}

void MeshPropertyToolbar::resetState()
{
    propertySelector->reset();

    resetCheckbox(facetCheckBox);
    resetCheckbox(boundingBoxCheckBox);
    resetCheckbox(annotationCheckBox);
}

void MeshPropertyToolbar::resetCheckbox(QCheckBox *checkBox)
{
    checkBox->blockSignals(true);
    checkBox->setEnabled(false);
    checkBox->setChecked(false);
    checkBox->blockSignals(false);
}

void MeshPropertyToolbar::initHelp(HelpHandler *handler)
{
    propertySelector->setWhatsThis(handler->getText("property-selector"));
    propertySelector->setToolTip(tr("Property Selector"));

    annotationCheckBox->setWhatsThis(handler->getText("annotation-checkbox"));
    facetCheckBox->setToolTip(handler->getText("facet-checkbox"));
    boundingBoxCheckBox->setToolTip(handler->getText("bounding-box-checkbox"));

}
