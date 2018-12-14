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

#include "valuepicker.h"
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <QIcon>
#include <QDebug>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkDistanceToCamera.h>
#include <vtkPointSet.h>
#include <vtkNew.h>
#include <vtkCellPicker.h>

#include "meshviewwidget.h"

ValuePicker::ValuePicker(MeshViewWidget *parent)
    : ViewerModule(parent)
{
    m_mesh = nullptr;

    m_cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    m_cellPicker->SetTolerance(0.1);

    action_valuePicker = new QAction("Value Picker Tool", this);
    action_valuePicker->setIcon(QIcon(":/images/noun_236332_cc.png"));
    action_valuePicker->setCheckable(true);

    connect(action_valuePicker, &QAction::toggled,
            this, &ValuePicker::setValuePickerModeEnabled);

    auto iren = parent->renderWindowInteractor();

    m_defaultInteractorStyle = iren->GetInteractorStyle();

    m_interactorStyle = vtkSmartPointer<vtkInteractorStyleUser>::New();
    m_interactorStyle->AddObserver(vtkCommand::MouseMoveEvent,
                this, &ValuePicker::onMouseMove);
    m_interactorStyle->AddObserver(vtkCommand::LeftButtonPressEvent,
                this, &ValuePicker::onLeftButtonClicked);

    m_pointGlyph = createSelectedPointGlyph();

    m_isValueSet = false;
    m_currentValue = 0.0;
}

void ValuePicker::setActiveMesh(Mesh *mesh)
{
    m_mesh = mesh;
    if(m_mesh != nullptr)
    {
        m_cellPicker->AddLocator(m_mesh->cellLocator());
    }
}

bool ValuePicker::isValueSet()
{
    return m_isValueSet;
}

double ValuePicker::currentValue()
{
    return m_currentValue;
}


void ValuePicker::onToolbarInit(QToolBar *toolBar)
{
    toolBar->addAction(action_valuePicker);
}

void ValuePicker::setValuePickerModeEnabled(bool enabled)
{
    auto iren = viewerWidget()->renderWindowInteractor();
    auto renWin = viewerWidget()->renderWindow();

    if(enabled)
    {
        iren->SetInteractorStyle(m_interactorStyle);

        renWin->SetCurrentCursor(VTK_CURSOR_CROSSHAIR);
    }
    else
    {
        iren->SetInteractorStyle(m_defaultInteractorStyle);
        renWin->SetCurrentCursor(VTK_CURSOR_DEFAULT);
    }
}

void ValuePicker::onMouseMove(vtkObject *caller, unsigned long eventId, void *callData)
{
    // Early exit from the function means that the
    // value has not been set correctly
    m_isValueSet = false;

    auto interactor = viewerWidget()->renderWindowInteractor();
    auto renderer = viewerWidget()->renderer();

    int x, y;

    interactor->GetEventPosition(x, y);

    auto picker = m_cellPicker;

    //picker->UseCellsOn();

    picker->Pick((double) x, (double) y, 0.0, renderer );

    vtkIdType pointId = picker->GetPointId();
    auto actor = picker->GetActor();

    handleCurrentVertex(actor, pointId);
    emit valueUpdated();
}

void ValuePicker::onLeftButtonClicked(vtkObject *caller, unsigned long eventId, void *callData)
{
    if(!isValueSet())
        return;

    auto ren = viewerWidget()->renderer();
    if(!ren->HasViewProp(m_pointGlyph))
    {
        ren->AddViewProp(m_pointGlyph);
    }

    m_pointGlyph->SetPosition(m_currentPosition);

    action_valuePicker->setChecked(false);

    viewerWidget()->update();
}

void ValuePicker::handleCurrentVertex(vtkActor *actor, vtkIdType pointId)
{
    if(actor == nullptr)
        return;

    auto dataset = actor->GetMapper()->GetInputAsDataSet();
    auto polydata = vtkPolyData::SafeDownCast(dataset);

    if(polydata == nullptr)
        return;

    polydata->GetPoint(pointId, m_currentPosition);

    auto pointdata = polydata->GetPointData();
    if(pointdata == nullptr)
        return;

    auto scalars = pointdata->GetScalars();

    if(scalars == nullptr)
        return;

    if(scalars->GetSize() == 0)
        return;

    if(scalars->GetNumberOfComponents() != 1)
        return;

    m_isValueSet = true;
    m_currentValue = scalars->GetTuple1(pointId);
}

void ValuePicker::hoverTest(vtkObject *caller, unsigned long eventId, void *callData)
{
    qDebug() << Q_FUNC_INFO << vtkCommand::GetStringFromEventId(eventId);
}

vtkSmartPointer<vtkActor> ValuePicker::createSelectedPointGlyph()
{
    // Create a set of points
    auto points = vtkSmartPointer<vtkPoints>::New();

    vtkIdType pid[1];
    pid[0] = points->InsertNextPoint(0,0,0);

    auto vertices = vtkSmartPointer<vtkCellArray>::New();
    vertices->InsertNextCell(1, pid);

    auto pointPolyData = vtkSmartPointer<vtkPolyData>::New();
    pointPolyData->SetPoints(points);
    pointPolyData->SetVerts(vertices);

    // Create a mapper.
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(pointPolyData);
    mapper->SetScalarVisibility(false);

    // Create an actor.
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    actor->GetProperty()->VertexVisibilityOn();
    actor->GetProperty()->RenderPointsAsSpheresOn();
    actor->GetProperty()->SetPointSize(5);


    return actor;

}
