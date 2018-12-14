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
#ifndef MEASURETOOL_H
#define MEASURETOOL_H

#include <QObject>
#include <vtkSmartPointer.h>
#include <vtkInteractorObserver.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation3D.h>
#include "viewermodule.h"
#include "core/mesh.h"

class QAction;
class MeshViewWidget;

class MeasureTool : public ViewerModule
{
    Q_OBJECT
public:
    explicit MeasureTool(MeshViewWidget *parent);

    void setActiveMesh(Mesh *mesh);

signals:

public slots:

    void setUnitOfMeasureString(const QString &unitString);

    // ViewerModule interface
protected:
    void onToolbarInit(QToolBar *toolBar) override;

    QAction *action_measureTool;

private:
    void setMeasuringMode(bool enabled);

    vtkSmartPointer<vtkDistanceRepresentation> createRepresentation();

    vtkSmartPointer<vtkInteractorObserver> m_defaultStyle;
    vtkSmartPointer<vtkDistanceWidget> distanceWidget;

    QString m_unitOfMeasureString = "";

    Mesh *m_mesh;
};

#endif // MEASURETOOL_H
