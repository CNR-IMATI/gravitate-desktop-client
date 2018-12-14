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
#include <vtkSmartPointer.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientationMarkerWidget.h>

#include "meshviewwidget.h"
#include "coordinateaxes.h"

// === CoordinateAxes ===

CoordinateAxes::CoordinateAxes(MeshViewWidget *parent)
    : ViewerModule(parent)
{
    if(parent == nullptr)
    {
        return;
    }

    m_axes = vtkSmartPointer<vtkAxesActor>::New();

    m_markerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    m_markerWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    m_markerWidget->SetOrientationMarker(m_axes);
    m_markerWidget->SetInteractor(parent->renderWindowInteractor());
    m_markerWidget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
    m_markerWidget->SetEnabled( 1 );
    m_markerWidget->InteractiveOff();

    parent->resetCameraToDefault();
}
