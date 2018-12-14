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
#ifndef PROPERTYCOLORING_H
#define PROPERTYCOLORING_H

//#include "scalarfieldmodule.h"

#include <QObject>
#include <QComboBox>
#include <QString>
#include <QHash>

#include <vtkSmartPointer.h>
#include <vtkScalarBarActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkColorSeries.h>

#include "core/meshviewwidget.h"
#include "core/viewermodule.h"
#include "core/mesh.h"
#include "scalarfieldproperty.h"
#include "propertyrendering.h"
#include "meshpropertyreader.h"
#include "qudt_units.h"

class PropertyColoring : public ViewerModule
{
    Q_OBJECT
public:
    explicit PropertyColoring(Mesh * const mesh, MeshViewWidget *parent = 0);

    void applyRendering(const ScalarFieldProperty &property, AbstractPropertyRendering *ren, qudt::Unit unit);

    QList<PropertyType> loadedProperties();

    Mesh *mesh();

public slots:
    void reset();

    void applyPropertyColoring(vtkScalarsToColors *lut);

    void addProperty(PropertyType type, vtkSmartPointer<vtkScalarsToColors> lut);

    void selectProperty(PropertyType property);

private:

    struct Property
    {
        QString name;
        PropertyTarget target;
        vtkSmartPointer<vtkScalarsToColors> lut;
    };


    vtkSmartPointer<vtkScalarBarActor> m_scalarBar;

    Mesh *m_mesh;

    QMap<PropertyType, Property> m_lutMap;
};

#endif // PROPERTYCOLORING_H
