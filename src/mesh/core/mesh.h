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
#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkAbstractCellLocator.h>
#include "viewermodule.h"

#include <array>

#include "meshviewwidget.h"

class Mesh : public ViewerModule
{
    Q_OBJECT
public:
    explicit Mesh(MeshViewWidget *widget);

    vtkActor *actor() const;

    vtkPolyData *polyData() const;

    vtkPolyDataMapper *polyDataMapper() const;

    vtkAbstractCellLocator *cellLocator() const;

    int vertexCount();

    int triangleCount();

    QStringList vertexFields();

    void setAffineTransform(std::array<double, 16> transform);

    bool isLoaded();

signals:
    void modelLoaded();

    void propertyLoaded();

    void modelUpdated();

public slots:
    void loadModel(const QString &fileName);

    void loadVertexProperty(const QString &name, const QVector<double> &values);
    void loadCellProperty(const QString &name, const QVector<double> &values);

    void onSetVisualizationMode(const MeshVisualizationMode &mode);

    void printAvailableProperties();

private:
    vtkSmartPointer<vtkActor> m_actor;
    vtkSmartPointer<vtkPolyData> m_polydata;
    vtkSmartPointer<vtkPolyDataMapper> m_polydataMapper;

    vtkSmartPointer<vtkAbstractCellLocator> m_cellLocator;
};
#endif // MESH_H
