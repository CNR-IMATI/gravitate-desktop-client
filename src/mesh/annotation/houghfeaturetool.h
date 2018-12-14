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
#ifndef HOUGHFEATURETOOL_H
#define HOUGHFEATURETOOL_H

#include <QObject>
#include "core/mesh.h"
#include "core/viewermodule.h"
#include "houghfeature.h"
#include "qudt_units.h"
#include "selectiontool.h"

#include <vtkSmartPointer.h>

class HoughFeatureTool : public ViewerModule
{
public:
    explicit HoughFeatureTool(Mesh *mesh, MeshViewWidget *parent = nullptr);

    void apply(const std::vector<HoughFeature> &featureVector, qudt::Unit unit);

    int featureCount();

    void setFeatureVisible(int index, bool visible);

private:

    struct Item
    {
        vtkSmartPointer<vtkProp3D> verticesProp;
        vtkSmartPointer<vtkProp3D> axesProp;
        vtkSmartPointer<vtkProp3D> labelProp;
        HoughFeature featureData;
    };

    Mesh *m_mesh;

    QColor m_axisColor;

    QString m_unitString;

    QList<Item> m_items;


    void addFeature(const HoughFeature &feature, const QColor &color);

    void initTextProperties(vtkTextProperty *prop);

    static double length(HoughFeature::Axis axis);

    void clear();

    vtkSmartPointer<vtkProp3D> createVerticesActor(const std::vector<unsigned int> &vertices, const QColor &color);
    vtkSmartPointer<vtkProp3D> createAxisActor(const HoughFeature::Axis &axis, const QColor &color);
    vtkSmartPointer<vtkProp3D> createMeasuresActor(const HoughFeature::Axis &axis1, const HoughFeature::Axis &axis2);

    static QColor getRandomColor();

};

#endif // HOUGHFEATURETOOL_H
