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
#ifndef WIREFRAMEBOX_H
#define WIREFRAMEBOX_H

#include "core/meshviewwidget.h"
#include "core/viewermodule.h"

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkOutlineSource.h>
#include <vtkCubeAxesActor.h>
#include <vtkBillboardTextActor3D.h>
#include "boundingbox.h"
#include "qudt_units.h"
#include <QList>

class vtkAxisActor2D;

/**
 * @brief The WireframeBox class renders a wireframe box in the 3D viewport.
 *
 * A typical use of it is to visualize the 3D object bounding box
 */
class WireframeBox: public ViewerModule
{
    Q_OBJECT
public:
    explicit WireframeBox(MeshViewWidget *parent = 0);

    void apply(const BoundingBox &bb, qudt::Unit unit);

    bool isSet();

    void setVisible(bool visible);

public slots:
    void reset();

private:

    void setCorners(const double data[]);

    void setBillboardMeasures(const double data[], qudt::Unit unit);

    void transformToBase(const double A[3][3], const double in[3], double out[3]);

    void getPoint(int index, const double data[], double *x, double *y, double *z);
    void getPoint(int index, const double data[], double out[]);

    void computeMidPoint(const double p0[], const double p1[], double midpoint[]);

    vtkSmartPointer<vtkActor> m_bbActor;
    vtkSmartPointer<vtkOutlineSource> m_outline;

    vtkSmartPointer<vtkAxisActor> m_axisX;
    vtkSmartPointer<vtkAxisActor> m_axisY;
    vtkSmartPointer<vtkAxisActor> m_axisZ;

    QList<vtkSmartPointer<vtkBillboardTextActor3D>> m_lengthTextList;

    bool m_isLoaded;

    QString m_unitOfMeasure;
};

#endif // WIREFRAMEBOX_H
