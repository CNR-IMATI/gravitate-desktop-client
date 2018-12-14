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
#include "wireframebox.h"

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <QDebug>

#include <vtkLabeledDataMapper.h>
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkAxisActor.h>
#include <vtkTextProperty.h>

WireframeBox::WireframeBox(MeshViewWidget *parent)
    : ViewerModule(parent)
{
    reset();
}

void WireframeBox::apply(const BoundingBox &bb, qudt::Unit unit)
{
    reset();

    setCorners(bb.vertices.data());

    setBillboardMeasures(bb.vertices.data(), unit);
}

void WireframeBox::setCorners(const double data[])
{
    for(int i = 0; i < 8; i++)
    {
        auto j = i * 3;

        qDebug() << "{"
                 << data[j] << ","
                 << data[j+1] << ","
                 << data[j+2] << "}";
    }

    auto renderer = viewerWidget()->renderer();
    renderer->AddActor(m_bbActor);

    m_outline->SetBoxTypeToOriented();
    m_outline->SetCorners(const_cast<double*>(data));
    m_outline->Update();

//    addBillboardMeasures(data);

    m_isLoaded = true;

    viewerWidget()->update();
}

bool WireframeBox::isSet()
{
    return m_isLoaded;
}

void WireframeBox::setVisible(bool visible)
{
    if(!isSet())
    {
        qWarning() << "Box coordinates not loaded; cannot change visibility state";
        return;
    }

    m_bbActor->SetVisibility(visible);

    for(auto text : m_lengthTextList)
    {
        text->SetVisibility(visible);
    }


//    m_axisX->SetVisibility(visible);
//    m_axisY->SetVisibility(visible);
//    m_axisZ->SetVisibility(visible);

    viewerWidget()->update();
}

void WireframeBox::reset()
{
    auto ren = viewerWidget()->renderer();
    ren->RemoveActor(m_bbActor);
    for(auto label : m_lengthTextList)
    {
        ren->RemoveViewProp(label);
    }
    m_lengthTextList.clear();

    m_bbActor = vtkSmartPointer<vtkActor>::New();
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_bbActor->SetMapper(mapper);

    m_outline = vtkSmartPointer<vtkOutlineSource>::New();
    mapper->SetInputConnection(m_outline->GetOutputPort());
    mapper->ScalarVisibilityOff();

    m_bbActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    m_isLoaded = false;
}

void WireframeBox::setBillboardMeasures(const double data[], qudt::Unit unit)
{
    int pointSequences[6][2] = {
        {0, 1},
        {0, 2},
        {0, 4},
        {7, 3},
        {7, 5},
        {7, 6}
    };

    auto info = qudt::getInfo(unit);

    QString format = QString::fromStdString("%1 " + info.abbreviation);

    for(auto pair : pointSequences)
    {
        qDebug() << pair[0] << pair[1];
        double p0[3], p1[3];
        getPoint(pair[0], data, p0);
        getPoint(pair[1], data, p1);

        double mid[3];
        computeMidPoint(p0, p1, mid);

        auto text = vtkSmartPointer<vtkBillboardTextActor3D>::New();
        text->SetPosition(mid);

        double size;
        size = std::sqrt(vtkMath::Distance2BetweenPoints(p0, p1));

        auto label = format.arg(size).toStdString();

        text->SetInput(label.c_str());

        auto renderer = viewerWidget()->renderer();
        renderer->AddViewProp(text);

        m_lengthTextList.append(text);
    }

    viewerWidget()->update();
}

void WireframeBox::computeMidPoint(const double p0[], const double p1[], double midpoint[])
{
    vtkMath::Add(p0, p1, midpoint);
    vtkMath::MultiplyScalar(midpoint, 0.5);
}

void WireframeBox::transformToBase(const double A[3][3], const double in[], double out[3])
{
    double AI[3][3];

    vtkMath::Invert3x3(A, AI);

    vtkMath::Multiply3x3(AI, in, out);
}

void WireframeBox::getPoint(int index, const double data[], double *x, double *y, double *z)
{
    *x = data[index * 3];
    *y = data[index * 3 + 1];
    *z = data[index * 3 + 2];

}

void WireframeBox::getPoint(int index, const double data[], double out[])
{
    out[0] = data[index * 3];
    out[1] = data[index * 3 + 1];
    out[2] = data[index * 3 + 2];

}
