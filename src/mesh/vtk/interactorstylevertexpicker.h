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
#ifndef INTERACTORSTYLEVERTEXPICKER_H
#define INTERACTORSTYLEVERTEXPICKER_H

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkDistanceToCamera.h>

class InteractorStyleVertexPicker : public vtkInteractorStyleTrackballCamera
{
public:

    static InteractorStyleVertexPicker* New();
    vtkTypeMacro(InteractorStyleVertexPicker,vtkInteractorStyleTrackballCamera);

    InteractorStyleVertexPicker();

    virtual void OnLeftButtonDown() override;

    virtual void OnLeftButtonUp() override;

    vtkActor *PickedActor();

    vtkIdType PickedVertexId();

protected:
    virtual vtkIdType ScreenToPointId(int x, int y, vtkRenderer *ren);

private:
    vtkSmartPointer<vtkPolyData> m_selectionPolydata;
    vtkSmartPointer<vtkPolyDataMapper> m_selectionMapper;
    vtkSmartPointer<vtkActor> m_selectionActor;
    vtkSmartPointer<vtkPolyDataAlgorithm> m_selectionGlyph;
    vtkSmartPointer<vtkDistanceToCamera> m_distanceToCamera;

    vtkRenderer * m_defaultRenderer;

    vtkSmartPointer<vtkActor> m_pickedActor;
    vtkIdType m_pickedVertexId;
};

#endif // VTKINTERACTORSTYLEVERTEXPICKER_H
