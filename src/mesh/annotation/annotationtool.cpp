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
#include "annotationtool.h"

#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>

#include "selectionutils.h"

#include <QDebug>

AnnotationTool::AnnotationTool(Mesh *mesh, MeshViewWidget *parent)
    : ViewerModule(parent), m_mesh(mesh)
{
    connect(m_mesh, &Mesh::modelLoaded,
            this, &AnnotationTool::clearAnnotations);

    action_Annotate = new QAction("Annotate current selection", this);
    action_Annotate->setIcon(QIcon(":/images/noun_1202927_cc.png"));
}

void AnnotationTool::onToolbarInit(QToolBar *toolBar)
{
    connect(action_Annotate, &QAction::triggered,
            this, &AnnotationTool::annotationRequested);

    toolBar->addAction(action_Annotate);

    setEnabled(false);
}

void AnnotationTool::addAnnotation(const PartAnnotation anno)
{
    auto label = QString::fromStdString(anno.label);
    auto indices = QVector<unsigned int>::fromStdVector(anno.indices);
    auto color = QColor(anno.color.r, anno.color.g, anno.color.b);

    addAnnotation(label, indices, color);
}

void AnnotationTool::addAnnotation(const QString &label, const QVector<unsigned int> &indices, const QColor &color)
{
    auto annotationActor = create3DActor(indices, color);

    // Add to current renderer
    auto renderer = viewerWidget()->renderer();

    renderer->AddActor(annotationActor);

    // (optional)
    // add to list of "temp actors" to be deleted when resetting the UI

    m_actors.append(annotationActor);

    viewerWidget()->update();
}

void AnnotationTool::removeAnnotation(int index)
{
    if(index < 0 || index >= m_actors.size())
    {
        qCritical() << "Invalid index in" << Q_FUNC_INFO;
        return;
    }

    auto renderer = viewerWidget()->renderer();
    auto actor = m_actors.at(index);

    renderer->RemoveActor(actor);
    m_actors.removeAt(index);

    viewerWidget()->update();
}

void AnnotationTool::setEnabled(bool enabled)
{    
    action_Annotate->setEnabled(enabled);
}

void AnnotationTool::setAnnotationsVisible(bool visible)
{
    for(auto actor : m_actors)
    {
        actor->SetVisibility(visible);
    }
    viewerWidget()->update();
}

void AnnotationTool::setReadOnly(bool value)
{
    action_Annotate->setVisible(!value);
    m_readOnly = value;
}

bool AnnotationTool::isReadOnly()
{
    return m_readOnly;
}

void AnnotationTool::clearAnnotations()
{
    auto renderer = viewerWidget()->renderer();

    for(auto actor: m_actors)
    {
        renderer->RemoveActor(actor);
    }
    m_actors.clear();
}

vtkSmartPointer<vtkActor> AnnotationTool::create3DActor(const QVector<unsigned int> &indices, const QColor &color)
{
    auto pointIds = SelectionUtils::toIdTypeArray(indices.toStdVector());

    // Get cell indices from vertex indices
    auto cellIds = SelectionUtils::verticesToCells(m_mesh->polyData(), pointIds);

    // Create cell selection
    auto node = vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(vtkSelectionNode::CELL);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetSelectionList(cellIds);

    auto selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(node);

    // Extract selection
    auto extractFilter = vtkSmartPointer<vtkExtractSelection>::New();
    extractFilter->SetInputData(0, m_mesh->polyData());
    extractFilter->SetInputData(1, selection);

    extractFilter->Update();

    auto dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
    dataset->ShallowCopy(extractFilter->GetOutput());

    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(dataset);
    mapper->ScalarVisibilityOff();

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    actor->GetProperty()->SetColor( //(R,G,B)
        color.redF(), color.greenF(), color.blueF()
    );

    actor->GetProperty()->SetOpacity(0.5);

    return actor;
}
