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
#ifndef VALUEPICKER_H
#define VALUEPICKER_H

#include <vtkSmartPointer.h>
#include <vtkCallbackCommand.h>
#include <QAction>
#include "mesh.h"
#include <vtkObject.h>
#include <vtkEventQtSlotConnect.h>
#include "vtk/interactorstylevertexpicker.h"
#include <vtkInteractorStyleUser.h>
#include <vtkCellPicker.h>

class MeshViewWidget;

class ValuePicker : public ViewerModule
{
    Q_OBJECT
public:
    explicit ValuePicker(MeshViewWidget *parent);

    void setActiveMesh(Mesh *mesh);

    bool isValueSet();
    double currentValue();

    // ViewerModule interface
protected:
    void onToolbarInit(QToolBar *toolBar) override;


signals:
    void valueUpdated();

private slots:
    void setValuePickerModeEnabled(bool enabled);

    void onMouseMove(vtkObject *caller, unsigned long eventId, void *callData);

    void onLeftButtonClicked(vtkObject *caller, unsigned long eventId, void *callData);

    void handleCurrentVertex(vtkActor *actor, vtkIdType pointId);

    void hoverTest(vtkObject *caller, unsigned long eventId, void *callData);


private:
    QAction *action_valuePicker;
    int m_defaultCursor;
    bool m_isValueSet;
    double m_currentValue;
    double m_currentPosition[3];

    vtkSmartPointer<vtkActor> m_pointGlyph;

    vtkSmartPointer<vtkInteractorObserver> m_defaultInteractorStyle;
    vtkSmartPointer<vtkInteractorObserver> m_interactorStyle;

    vtkSmartPointer<vtkCellPicker> m_cellPicker;

    Mesh *m_mesh;

    vtkSmartPointer<vtkActor> createSelectedPointGlyph();
};

#endif // VALUEPICKER_H
