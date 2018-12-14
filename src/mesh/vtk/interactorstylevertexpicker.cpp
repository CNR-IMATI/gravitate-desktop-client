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
#include "interactorstylevertexpicker.h"
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkNew.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkProperty.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkDistanceToCamera.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>

vtkStandardNewMacro(InteractorStyleVertexPicker);

InteractorStyleVertexPicker::InteractorStyleVertexPicker()
{
    // Create a set of points
    vtkSmartPointer<vtkPoints> points =
      vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0,0,0);
    vtkSmartPointer<vtkPolyData> pointPolyData =
      vtkSmartPointer<vtkPolyData>::New();
    pointPolyData->SetPoints(points);

    // Calculate the distance to the camera of each point.
    vtkSmartPointer<vtkDistanceToCamera> distanceToCamera = vtkSmartPointer<
      vtkDistanceToCamera>::New();
    distanceToCamera->SetInputData(pointPolyData);
    distanceToCamera->SetScreenSize(100.0);

    // Glyph point with a sphere
    vtkSmartPointer<vtkSphereSource> sphere =
      vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(0.02);
    vtkSmartPointer<vtkGlyph3D> fixedGlyph = vtkSmartPointer<vtkGlyph3D>::New();
    fixedGlyph->SetInputConnection(distanceToCamera->GetOutputPort());
    fixedGlyph->SetSourceConnection(sphere->GetOutputPort());

    // Scale each point.
    fixedGlyph->SetScaleModeToScaleByScalar();
    fixedGlyph->SetInputArrayToProcess(0, 0, 0,
      vtkDataObject::FIELD_ASSOCIATION_POINTS, "DistanceToCamera");

    // Create a mapper.
    vtkSmartPointer<vtkPolyDataMapper> fixedMapper = vtkSmartPointer<
      vtkPolyDataMapper>::New();
    fixedMapper->SetInputConnection(fixedGlyph->GetOutputPort());
    fixedMapper->SetScalarVisibility(false);

    // Create an actor.
    vtkSmartPointer<vtkActor> fixedActor = vtkSmartPointer<vtkActor>::New();
    fixedActor->SetMapper(fixedMapper);
    fixedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);


    m_selectionActor = fixedActor;
    m_selectionMapper = fixedMapper;
    m_selectionPolydata = pointPolyData;
    m_distanceToCamera = distanceToCamera;
    m_pickedActor = nullptr;
}

void InteractorStyleVertexPicker::OnLeftButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

    auto renderer = this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();

    m_distanceToCamera->SetRenderer(renderer);

    int x, y;
    this->Interactor->GetEventPosition(x, y);

    vtkNew<vtkPointPicker> picker;
    picker->UseCellsOn();

    picker->Pick((double) x, (double) y, 0, renderer );

    vtkIdType pointId = picker->GetPointId();

    if(pointId < 0)
        return;

    auto actor = picker->GetActor();

    if(actor == nullptr)
        return;

    auto dataset = actor->GetMapper()->GetInputAsDataSet();
    auto polyData = vtkPolyData::SafeDownCast(dataset);

    if(polyData == nullptr)
        return;

    m_pickedActor = actor;
    m_pickedVertexId = pointId;


    // Place glyph in the coordinates of the picked vertex
    double pos[3];
    polyData->GetPoint(pointId, pos);

    m_selectionActor->SetPosition(pos);
    m_selectionActor->VisibilityOn();

    if(!renderer->HasViewProp(m_selectionActor))
    {
        renderer->AddViewProp(m_selectionActor);
    }
    renderer->ResetCameraClippingRange();

//    auto pointdata = polyData->GetPointData();
//    if(pointdata == nullptr)
//        return;

//    auto scalars = pointdata->GetScalars();

//    if(scalars == nullptr)
//        return;

//    m_currentValue = scalars->GetTuple1(pointId);
}

void InteractorStyleVertexPicker::OnLeftButtonUp()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

vtkActor *InteractorStyleVertexPicker::PickedActor()
{
    return m_pickedActor;
}

vtkIdType InteractorStyleVertexPicker::PickedVertexId()
{
    return m_pickedVertexId;
}

vtkIdType InteractorStyleVertexPicker::ScreenToPointId(int x, int y, vtkRenderer *ren)
{
    vtkNew<vtkPointPicker> picker;
    picker->UseCellsOn();

    picker->Pick((double) x, (double) y, 0, ren );

    return picker->GetPointId();
}
