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
#include "mesh.h"
#include <QDebug>
#include <QFile>

#include <vtkPLYReader.h>
#include <vtkCallbackCommand.h>
#include <vtkPolyDataMapper.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkProperty.h>
#include <vtkCellLocator.h>

#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkMatrix4x4.h>
#include <vtkNew.h>

#include "meshviewwidget.h"

Mesh::Mesh(MeshViewWidget *widget)
    : ViewerModule(widget)
{
    m_actor = vtkSmartPointer<vtkActor>::New();

    m_cellLocator = vtkSmartPointer<vtkCellLocator>::New();
    m_actor->GetProperty()->SetEdgeColor(0.2, 0.2, 0.2);

    connect(this, &Mesh::modelLoaded,
            widget, &MeshViewWidget::resetCameraToDefault);

    connect(widget, &MeshViewWidget::visualizationModeChanged,
            this, &Mesh::onSetVisualizationMode);

    connect(this, &Mesh::modelUpdated,
            widget, QOverload<>::of(&MeshViewWidget::update));
}

vtkActor *Mesh::actor() const
{
    return m_actor.Get();
}

vtkPolyData *Mesh::polyData() const
{
    return m_polydata.Get();
}

vtkPolyDataMapper *Mesh::polyDataMapper() const
{
    return m_polydataMapper.Get();
}

vtkAbstractCellLocator *Mesh::cellLocator() const
{
    return m_cellLocator.Get();
}

int Mesh::vertexCount()
{
    if(m_polydata == nullptr)
        return 0;

    return int(m_polydata->GetNumberOfPoints());
}

int Mesh::triangleCount()
{
    if(m_polydata == nullptr)
        return 0;

    return int(m_polydata->GetNumberOfCells());
}

QStringList Mesh::vertexFields()
{
    QStringList list;

    int nArrays = m_polydata->GetPointData()->GetNumberOfArrays();

    for(int i = 0; i < nArrays; i++)
    {
        list.append(m_polydata->GetPointData()->GetArrayName(i));
    }
    return list;
}

void Mesh::setAffineTransform(std::array<double, 16> matrix)
{
    vtkNew<vtkMatrix4x4> m;
    m->SetElement(0, 0, matrix[0]);
    m->SetElement(0, 1, matrix[1]);
    m->SetElement(0, 2, matrix[2]);
    m->SetElement(0, 3, matrix[3]);
    m->SetElement(1, 0, matrix[4]);
    m->SetElement(1, 1, matrix[5]);
    m->SetElement(1, 2, matrix[6]);
    m->SetElement(1, 3, matrix[7]);
    m->SetElement(2, 0, matrix[8]);
    m->SetElement(2, 1, matrix[9]);
    m->SetElement(2, 2, matrix[10]);
    m->SetElement(2, 3, matrix[11]);
    m->SetElement(3, 0, matrix[12]);
    m->SetElement(3, 1, matrix[13]);
    m->SetElement(3, 2, matrix[14]);
    m->SetElement(3, 3, matrix[15]);

    m_actor->SetUserMatrix(m.Get());

    emit modelUpdated();
}

bool Mesh::isLoaded()
{
    return m_polydata != nullptr;
}

void Mesh::loadModel(const QString &fileName)
{
    qDebug() << "Loading model" << fileName;

    if(!QFile::exists(fileName))
    {
        qDebug() << tr("File %1 not found").arg(fileName);
        return;
    }

    vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();

    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    m_polydataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    m_polydata = reader->GetOutput();
    auto nVertices = m_polydata->GetNumberOfPoints();
    auto nFaces = m_polydata->GetNumberOfCells();

    qDebug().noquote() << tr("Vertices: %1").arg(nVertices);
    qDebug().noquote() << tr("Faces: %1").arg(nFaces);


    if(nFaces == 0)
    {
        qDebug() << "Mesh contains only vertices. Converting to point cloud...";

        // The vtkVertexGlyphFilter takes a generic polydata and extracts only the points
        vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter =
                vtkSmartPointer<vtkVertexGlyphFilter>::New();
        vertexFilter->SetInputData(m_polydata);
        m_polydataMapper ->SetInputData(vertexFilter->GetOutput());

        vertexFilter->Update();
    }
    else
    {
        m_polydataMapper ->SetInputData(m_polydata);
    }

    m_actor->SetMapper( m_polydataMapper );

    viewerWidget()->renderer()->AddActor(m_actor);

    printAvailableProperties();

    m_cellLocator->SetDataSet(m_polydata);
    m_cellLocator->Update();

    emit modelLoaded();
}

void Mesh::loadVertexProperty(const QString &name, const QVector<double> &values)
{
    if(vertexCount() != values.count())
    {
        qDebug() << tr("Property size mismatch - number of vertices: %1, property size: %2")
                    .arg(vertexCount()).arg(values.count());
        return;
    }

    vtkSmartPointer<vtkDoubleArray> vertexProperty =
            vtkSmartPointer<vtkDoubleArray>::New();

    // This is safer than calling vertexProperty->setArray()
    for(double v : values)
    {
        vertexProperty->InsertNextValue(v);
    }

    auto propName = name.toStdString();

    vertexProperty->SetName(propName.c_str());

    auto pointData = m_polydata->GetPointData();
    pointData->AddArray(vertexProperty);

    printAvailableProperties();

    emit propertyLoaded();
}

void Mesh::loadCellProperty(const QString &name, const QVector<double> &values)
{
    if(triangleCount() != values.count())
    {
        qDebug() << "Property size mismatch -"
                 << "number of triangles:" << triangleCount()
                 << "property size:" << values.count();
        return;
    }

    auto property = vtkSmartPointer<vtkDoubleArray>::New();

    for(double v : values)
    {
        property->InsertNextValue(v);
    }

    auto propName = name.toStdString();
    property->SetName(propName.c_str());

    auto celldata = m_polydata->GetCellData();
    celldata->AddArray(property);


    emit propertyLoaded();
}

void Mesh::onSetVisualizationMode(const MeshVisualizationMode &mode)
{
    switch(mode)
    {
    case MeshVisualizationMode::SOLID:
        m_actor->GetProperty()->SetRepresentationToSurface();
        m_actor->GetProperty()->EdgeVisibilityOff();
        break;
    case MeshVisualizationMode::WIREFRAME:
        m_actor->GetProperty()->SetRepresentationToSurface();
        m_actor->GetProperty()->EdgeVisibilityOn();
        break;
    case MeshVisualizationMode::POINTS:
        m_actor->GetProperty()->SetRepresentationToPoints();
        m_actor->GetProperty()->EdgeVisibilityOff();
        break;
    default:
        return;
    }
    viewerWidget()->update();
}

void Mesh::printAvailableProperties()
{
    int nPointArrays = m_polydata->GetPointData()->GetNumberOfArrays();
    qDebug().noquote() << "Available vertex properties:";
    for(int i = 0; i < nPointArrays; i++)
    {
        qDebug().noquote() << " - " << m_polydata->GetPointData()->GetArrayName(i);
    }

    int nCellArrays = m_polydata->GetCellData()->GetNumberOfArrays();
    qDebug().noquote() << "Available cell properties:";
    for(int i = 0; i < nCellArrays; i++)
    {
        qDebug().noquote() << " - " << m_polydata->GetCellData()->GetArrayName(i);
    }

}

void loadModelProgress(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
    vtkPLYReader* reader = static_cast<vtkPLYReader*>(caller);
    qDebug() << reader->GetProgress();
}

