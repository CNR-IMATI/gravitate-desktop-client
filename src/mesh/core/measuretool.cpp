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
#include "measuretool.h"
#include <QAction>
#include <QDebug>
#include <vtkSmartPointer.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkHandleRepresentation.h>
#include <vtkCellPicker.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkCellLocator.h>

MeasureTool::MeasureTool(MeshViewWidget *parent)
    : ViewerModule(parent)
{
}

void MeasureTool::setActiveMesh(Mesh *mesh)
{
    m_mesh = mesh;
}

void MeasureTool::setUnitOfMeasureString(const QString &unitString)
{
    m_unitOfMeasureString = unitString;
    viewerWidget()->update();
}


void MeasureTool::onToolbarInit(QToolBar *toolBar)
{
    action_measureTool = new QAction("Measure Tool", this);
    action_measureTool->setIcon(QIcon(":/images/noun_326860_cc.png"));

    action_measureTool->setCheckable(true);

    toolBar->addAction(action_measureTool);

    connect(action_measureTool, &QAction::toggled,
            this, &MeasureTool::setMeasuringMode);

}

void MeasureTool::setMeasuringMode(bool enabled)
{
    if(enabled)
    {
        distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
        auto interactor = viewerWidget()->renderWindowInteractor();

        distanceWidget->SetInteractor(interactor);


        auto repr = createRepresentation();

        QString labelFormat = "%-#6.3g";

        if(!m_unitOfMeasureString.isEmpty())
        {
            labelFormat += " " + m_unitOfMeasureString;
        }
        auto label = labelFormat.toStdString();
        repr->SetLabelFormat(label.c_str());

        auto ren = viewerWidget()->renderer();
        repr->SetRenderer(ren);

        distanceWidget->SetRepresentation(repr);

    }

    distanceWidget->SetEnabled(enabled);
}

vtkSmartPointer<vtkDistanceRepresentation> MeasureTool::createRepresentation()
{

    auto pointPlacer = vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();

    pointPlacer->GetCellPicker()->RemoveAllLocators();

    if(m_mesh->cellLocator() != nullptr)
    {
        pointPlacer->GetCellPicker()->AddLocator(m_mesh->cellLocator());
    }

    auto props = viewerWidget()->renderer()->GetViewProps();

    props->InitTraversal();

    for(int i = 0; i < props->GetNumberOfItems(); i++)
    {
        pointPlacer->AddProp(props->GetNextProp());
    }

    auto repr = vtkSmartPointer<vtkDistanceRepresentation2D>::New();

    repr->InstantiateHandleRepresentation();

    repr->GetPoint1Representation()->SetPointPlacer(pointPlacer);
    repr->GetPoint2Representation()->SetPointPlacer(pointPlacer);

    return repr;
}

