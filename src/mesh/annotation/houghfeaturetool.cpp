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
#include "houghfeaturetool.h"
#include "selectionutils.h"
#include <vtkWidgetRepresentation.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkMath.h>
#include <vtkTextProperty.h>
#include <QString>
#include <QRgb>
#include <cmath>
#include <vtkAssembly.h>
#include <vtkProp3D.h>
#include <vtkSelection.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <QDebug>
#include <ctime>

HoughFeatureTool::HoughFeatureTool(Mesh *mesh, MeshViewWidget *parent)
    : ViewerModule(parent), m_mesh(mesh)
{
    m_unitString = "??";
    m_axisColor = QColor(Qt::darkGreen);
}

void HoughFeatureTool::apply(const std::vector<HoughFeature> &featureVector, qudt::Unit unit)
{
    clear();

    auto info = qudt::getInfo(unit);

    m_unitString = QString::fromStdString(info.abbreviation);

    auto random = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    random->SetSeed(static_cast<int>(time(nullptr)));

    for(auto f : featureVector)
    {
        auto hue = random->GetValue();

        auto color = QColor::fromHsvF(hue, 1.0, 1.0);

        addFeature(f, color);

        random->Next();
    }

    viewerWidget()->update();
}

int HoughFeatureTool::featureCount()
{
    return m_items.count();
}

void HoughFeatureTool::setFeatureVisible(int index, bool visible)
{
    if(index < 0 || index > m_items.length() - 1)
    {
        return;
    }

    m_items.at(index).verticesProp->SetVisibility(visible);
    m_items.at(index).axesProp->SetVisibility(visible);
    m_items.at(index).labelProp->SetVisibility(visible);
}

void HoughFeatureTool::addFeature(const HoughFeature &feature, const QColor &color)
{
    auto axis1Actor = createAxisActor(feature.axis1, m_axisColor);
    auto axis2Actor = createAxisActor(feature.axis2, m_axisColor);
    auto measuresActor = createMeasuresActor(feature.axis1, feature.axis2);
    auto verticesActor = createVerticesActor(feature.vertices, color);

    // Part to be placed on the overlay
    auto axesActor = vtkSmartPointer<vtkAssembly>::New();

    axesActor->AddPart(axis1Actor);
    axesActor->AddPart(axis2Actor);

    auto renderer = viewerWidget()->renderer();
    renderer->AddActor(verticesActor);

    auto overlayRenderer = viewerWidget()->overlayRenderer();

    overlayRenderer->AddViewProp(axesActor);
    overlayRenderer->AddViewProp(measuresActor);

    Item item;
    item.verticesProp = verticesActor;
    item.axesProp = axesActor;
    item.labelProp = measuresActor;
    item.featureData = feature;
    m_items.append(item);
}

void HoughFeatureTool::initTextProperties(vtkTextProperty *property)
{
    double r,g,b;
    m_axisColor.getRgbF(&r,&g,&b);

    property->SetColor(r,g,b);
    //property->ShadowOn();
    //property->SetShadowOffset(1,-1);
    //property->SetBackgroundColor(0,0,0);
    //property->SetBackgroundOpacity(0.5);
    property->SetFontSize(18);
    property->BoldOn();

}

double HoughFeatureTool::length(HoughFeature::Axis axis)
{
    return std::sqrt(vtkMath::Distance2BetweenPoints(axis.p1.data(), axis.p2.data()));
}

void HoughFeatureTool::clear()
{
    for(auto item : m_items)
    {
        // TODO: should remove props from the associated renderers
        auto renderer = viewerWidget()->renderer();
        renderer->RemoveViewProp(item.verticesProp);

        auto overlayRenderer = viewerWidget()->overlayRenderer();

        overlayRenderer->RemoveViewProp(item.axesProp);
        overlayRenderer->RemoveViewProp(item.labelProp);
    }

    m_items.clear();
}

vtkSmartPointer<vtkProp3D> HoughFeatureTool::createVerticesActor(const std::vector<unsigned int> &vertices, const QColor &color)
{
    auto selectionList = SelectionUtils::toIdTypeArray(vertices);

    auto node = vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(vtkSelectionNode::POINT);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetSelectionList(selectionList);

    auto selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(node);

    auto extractFilter = vtkSmartPointer<vtkExtractSelection>::New();
    extractFilter->SetInputData(0, m_mesh->polyData());
    extractFilter->SetInputData(1, selection);

    auto glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilter->SetInputConnection(extractFilter->GetOutputPort());
    glyphFilter->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyphFilter->GetOutputPort());

    mapper->Update();

    auto actor = vtkSmartPointer<vtkActor>::New();

    double r, g, b;
    color.getRgbF(&r, &g, &b);

    actor->GetProperty()->SetColor(r, g, b);
    actor->GetProperty()->SetPointSize(5);
    actor->GetProperty()->RenderPointsAsSpheresOn();
    actor->SetMapper(mapper);

    return actor;
}

vtkSmartPointer<vtkProp3D> HoughFeatureTool::createAxisActor(const HoughFeature::Axis &axis, const QColor &color)
{
    auto ls = vtkSmartPointer<vtkLineSource>::New();
    ls->SetPoint1(axis.p1[0], axis.p1[1], axis.p1[2]);
    ls->SetPoint2(axis.p2[0], axis.p2[1], axis.p2[2]);

    ls->Update();
    auto poly = ls->GetOutput();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(poly);

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    double r,g,b;
    color.getRgbF(&r,&g,&b);
    actor->GetProperty()->SetColor(r,g,b);
    actor->GetProperty()->RenderLinesAsTubesOn();
    actor->GetProperty()->SetLineWidth(2.5);

    return actor;
}

vtkSmartPointer<vtkProp3D> HoughFeatureTool::createMeasuresActor(const HoughFeature::Axis &axis1, const HoughFeature::Axis &axis2)
{
    auto sizeLabelActor = vtkSmartPointer<vtkBillboardTextActor3D>::New();

    auto labelPos = axis1.p1.data();

    auto str = QString("%1 x %2 %3")
            .arg(length(axis1))
            .arg(length(axis2))
            .arg(m_unitString);
    auto stdStr = str.toStdString();

    sizeLabelActor->SetInput(stdStr.data());
    sizeLabelActor->SetPosition(const_cast<double*>(labelPos));

    initTextProperties(sizeLabelActor->GetTextProperty());

    return sizeLabelActor;
}

QColor HoughFeatureTool::getRandomColor()
{
    auto seq = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();

    //seq->SetSeed(static_cast<int>(time(nullptr)));

    auto hue = seq->GetRangeValue(0.0, 1.0);

    QColor color = QColor::fromHsvF(hue, 1.0, 1.0);

    qDebug() << hue;
    qDebug() << color;

    return color;
}
