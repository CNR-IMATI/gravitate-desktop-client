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
#include "selectiontool.h"

#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include "vtk/highlightinteractorstyle.h"
#include <vtkInteractorStyleDrawPolygon.h>
#include <QDebug>
#include <vtkNew.h>
#include <vtkExtractSelection.h>
#include <vtkFeatureEdges.h>
#include <vtkCellPicker.h>
#include <vtkWorldPointPicker.h>
#include <vtkSelectPolyData.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkCleanPolyData.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkClipPolyData.h>
#include <set>

#include "selectionutils.h"

#include <QFile>
#include <QTextStream>

// For debugging purposes
#include <vtkLookupTable.h>
#include <vtkTimerLog.h>

SelectionTool::SelectionTool(Mesh *mesh, MeshViewWidget *parent)
    : ViewerModule(parent), m_mesh(mesh)
{
    connect(m_mesh, &Mesh::modelLoaded,
            this, &SelectionTool::onModelLoaded);
}

void SelectionTool::setSelectedVertices(QVector<unsigned int> vertices)
{
    auto idArray = SelectionUtils::toIdTypeArray(vertices.toStdVector());

    auto node = vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(vtkSelectionNode::POINT);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetSelectionList(idArray);

    updateCurrentSelection(node, SelectionMode::SIMPLE);
}

QVector<unsigned int> SelectionTool::selectedVertices()
{
    QVector<unsigned int> indices;

    if(m_currentNode == nullptr)
    {
        return indices;
    }

    auto cellSelectionList = vtkIdTypeArray::FastDownCast(m_currentNode->GetSelectionList());

    if(cellSelectionList->GetNumberOfValues() == 0)
    {
        qWarning() << "Warning: the current selection is empty";
        return indices;
    }


    auto selectionList = SelectionUtils::cellsToVertices(m_mesh->polyData(), cellSelectionList);

    qDebug() << "vertexSelectionList:"
             << selectionList->GetNumberOfValues()
             << "elements";

    qDebug() << "cellSelectionList: "
             << cellSelectionList->GetNumberOfValues()
             << "elements";

    for(vtkIdType i = 0; i < selectionList->GetNumberOfValues(); i++)
    {
        indices.append(selectionList->GetValue(i));
    }
    return indices;
}

QVector<unsigned int> SelectionTool::selectedCells()
{
    QVector<unsigned int> indices;

    if(m_currentNode == nullptr)
    {
        return indices;
    }

    auto cellSelectionList = vtkIdTypeArray::FastDownCast(m_currentNode->GetSelectionList());

    if(cellSelectionList->GetNumberOfValues() == 0)
    {
        qWarning() << "Warning: the current selection is empty";
        return indices;
    }

    for(vtkIdType i = 0; i < cellSelectionList->GetNumberOfValues(); i++)
    {
        indices.append(cellSelectionList->GetValue(i));
    }
    return indices;
}

void SelectionTool::setEnabled(bool enabled)
{
    if(!enabled)
    {
        action_Move_Mode->trigger();
    }

    action_Add_Selection->setEnabled(enabled);
    action_Lasso_Selection->setEnabled(enabled);
    action_Move_Mode->setEnabled(enabled);
    action_Simple_Selection->setEnabled(enabled);
    action_Subtract_Selection->setEnabled(enabled);
}

void SelectionTool::saveSelection(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Could not open file " << fileName << "for writing";
        return;
    }

    auto indices = selectedCells();

    QTextStream out(&file);

    for(auto i : indices)
    {
        out << i << "\n";
    }
}

void SelectionTool::loadSelection(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "Could not open file " << fileName << "for reading";
        return;
    }

    QVector<unsigned int> indices;

    while(!file.atEnd())
    {
        QString line = file.readLine();
        bool ok;

        unsigned int index = line.toUInt(&ok);
        if(!ok) continue;

        indices.append(index);
    }

    auto node = createSelectionNode(indices, vtkSelectionNode::CELL);
    updateCurrentSelection(node, SelectionMode::SIMPLE);
}

void SelectionTool::clearSelection()
{
    auto node = vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(vtkSelectionNode::CELL);
    node->SetContentType(vtkSelectionNode::INDICES);

    auto empty_array = vtkSmartPointer<vtkIdTypeArray>::New();
    node->SetSelectionList(empty_array);

    updateCurrentSelection(node, SelectionMode::SIMPLE);

    viewerWidget()->update();
}

void SelectionTool::onModelLoaded()
{
    // Selection actor initialization
    m_selectionActor = vtkSmartPointer<vtkActor>::New();
    m_selectionActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    m_selectionActor->GetProperty()->SetPointSize(5);
    m_selectionActor->GetProperty()->RenderPointsAsSpheresOn();

    viewerWidget()->renderer()->AddActor(m_selectionActor);

    m_boundaryActor = vtkSmartPointer<vtkActor>::New();
    m_boundaryActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    viewerWidget()->renderer()->AddActor(m_boundaryActor);

    m_currentNode = vtkSmartPointer<vtkSelectionNode>::New();
}

void SelectionTool::SelectionChangedCallbackFunction(vtkObject *caller, unsigned long eventId, void *callData)
{
    qDebug().noquote() <<  "Callback:" << Q_FUNC_INFO;

    // Get the polygon points
    vtkInteractorStyleDrawPolygon *drawPolygon
            = static_cast<vtkInteractorStyleDrawPolygon*>(caller);
    auto points = drawPolygon->GetPolygonPoints();

    qDebug() << "Polygon has" << points.size() << "points";

    // If it is just a segment, no selection can happen
    if(points.size() < 3)
    {
        return;
    }

    // Extract the vertex ids corresponding to the internal points of
    // the selection
    auto idArray = extractVertexIdsFromLoopPolygon(points);

    // Create a selection node for visualization purposes
    auto node = vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(vtkSelectionNode::POINT);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetSelectionList(idArray);
    //node->GetProperties()->Set(vtkSelectionNode::CONTAINING_CELLS(), 1);

    auto cellSelectionNode = toCellSelectionNode(node);    
    updateCurrentSelection(cellSelectionNode, m_currentSelectionMode);

    // This is necessary to have an immediate update...
    viewerWidget()->update();
}

void SelectionTool::GetSelectionRectViewport(unsigned int rect[])
{
    auto renderer = viewerWidget()->renderer();

    rect[0] = static_cast<unsigned int>(renderer->GetOrigin()[0]);
    rect[1] = static_cast<unsigned int>(renderer->GetOrigin()[1]);
    rect[2] = static_cast<unsigned int>(renderer->GetSize()[0]);
    rect[3] = static_cast<unsigned int>(renderer->GetSize()[1]);
}

void SelectionTool::GetSelectionRectPolygon(std::vector<vtkVector2i> points, unsigned int rect[])
{
    int minX = 0, minY = 0, maxX = 0, maxY = 0;

    for(size_t i = 0; i < points.size(); i++)
    {
        const vtkVector2i &v = points[i];
        int pos[2] = {v[0], v[1]};
        if(i == 0)
        {
            minX = maxX = pos[0];
            minY = maxY = pos[1];
        }
        else
        {
            minX = std::min(pos[0], minX);
            maxX = std::max(pos[0], maxX);
            minY = std::min(pos[1], minY);
            maxY = std::max(pos[1], maxY);
        }
    }

    rect[0] = static_cast<unsigned int>(minX);
    rect[1] = static_cast<unsigned int>(minY);
    rect[2] = static_cast<unsigned int>(maxX);
    rect[3] = static_cast<unsigned int>(maxY);
}

void SelectionTool::updateAreaVisualization(vtkDataSet *extractedSelection)
{
    // Create a dataset mapper for the extraction object
    vtkSmartPointer<vtkDataSetMapper> dsm =
            vtkSmartPointer<vtkDataSetMapper>::New();
    dsm->ScalarVisibilityOff();
    dsm->SetInputData(extractedSelection);

    // Connect the actor to the mapper
    m_selectionActor->SetMapper(dsm);
    // Update the mapper (and the connected vtkExtractSelection)
    dsm->Update();
    viewerWidget()->update();
}

void SelectionTool::visualizeEdgeLoop(vtkPolyData *edgeLoop)
{
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(edgeLoop);
    mapper->ScalarVisibilityOff();

    m_boundaryActor->SetMapper(mapper);

}

vtkSmartPointer<vtkPoints> SelectionTool::pickedPointsCoordinates(std::vector<vtkVector2i> pointData)
{
    vtkSmartPointer<vtkPoints> points =
            vtkSmartPointer<vtkPoints>::New();

    auto picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->AddLocator(m_mesh->cellLocator());

    vtkSmartPointer<vtkWorldPointPicker> worldPicker =
            vtkSmartPointer<vtkWorldPointPicker>::New();

    auto renderer = viewerWidget()->renderer();

    for(int i = 0; i < pointData.size(); i++)
    {
        auto p = pointData[i];

        // if the point has been detected, save the point
        if(picker->Pick(p.GetX(), p.GetY(), 0, renderer))
        {
            double *pos = picker->GetPickPosition();
            points->InsertNextPoint(pos[0], pos[1], pos[2]);
        }
        else
        {
            worldPicker->Pick(p.GetX(), p.GetY(), 0, renderer);
            double *pos = worldPicker->GetPickPosition();
            points->InsertNextPoint(pos[0], pos[1], pos[2]);
        }
    }
    return points;
}

vtkSmartPointer<vtkIdTypeArray> SelectionTool::extractVertexIdsFromLoopPolygon(std::vector<vtkVector2i> pointData)
{
    /**
     * Implementation notice
     *
     * This function will extract a vertex selection (no triangles included)
     * The visualization of the corresponding cells/triangles is performed
     * separately.
     *
     * This is due mainly for two reasons:
     * 1) vtkSelectPolyData does not keep the OriginalIDs scalar value for
     *    cells (only for points)
     * 2) we store the annotation as a set of vertices anyway, so the triangles
     *    are not used
     *
     * The algorithm follows these steps
     *
     * - Get the input points in display space and project them to the mesh,
     *   obtaining a loop of vtkPoints
     * - vtkIdFilter to generate the original vertex/cell ids
     * - vtkSelectPolyData filter extracts the selection defined by the loop of
     *   vtkPoints
     * - Since vtkSelectPolyData keeps all the vertices and removes only the
     *   cells not belonging to the selection loop, we use vtkCleanPolyData
     *   to remove the unconnected points
     * - Return the selection of point ids as a vtkIdTypeArray
     */

    vtkNew<vtkTimerLog> timer;
    timer->StartTimer();

    // Use vtkIdFilter to generate the original ids
    vtkNew<vtkIdFilter> idFilter;
    idFilter->SetIdsArrayName("OriginalIDs");
    idFilter->PointIdsOn();
    idFilter->CellIdsOn();
    idFilter->SetInputData(m_mesh->polyData());

    idFilter->Update();

    // Get points from loop in display coordinates
    auto points = pickedPointsCoordinates(pointData);

    timer->StopTimer();
    qDebug() << "Get coordinates of points in the loop";
    qDebug() << "  Elapsed time: " << timer->GetElapsedTime();

    timer->StartTimer();

    // Set up selection filter
    auto select = vtkSmartPointer<vtkSelectPolyData>::New();

    select->SetLoop(points);
    select->SetSelectionModeToSmallestRegion();
    select->GenerateSelectionScalarsOff();
    select->SetInputConnection(idFilter->GetOutputPort());

    select->Update();

    timer->StopTimer();
    qDebug() << "Filtered points inside the loop";
    qDebug() << "  Elapsed time: " << timer->GetElapsedTime();

    timer->StartTimer();

    auto selectOutput = select->GetOutput();

    visualizeEdgeLoop(select->GetSelectionEdges());

    auto cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
    cleanFilter->SetInputData(selectOutput);
    cleanFilter->Update();

    timer->StopTimer();
    qDebug() << "Removed points outside the loop";
    qDebug() << "  Elapsed time: " << timer->GetElapsedTime();

    auto output = cleanFilter->GetOutput();

    // Add vertex/cell indices to the node
    auto originalIds = vtkIdTypeArray::SafeDownCast(output->GetPointData()->GetArray("OriginalIDs"));

    auto out_array = vtkSmartPointer<vtkIdTypeArray>::New();

    out_array->DeepCopy(originalIds);

    return out_array;
}

vtkSmartPointer<vtkSelectionNode> SelectionTool::toCellSelectionNode(vtkSelectionNode *vertexSelectionNode)
{
    // This function converts the selection node with "point" field type
    // to a vtkSelectionNode with "cell" field type

    auto vertexIds = vtkIdTypeArray::SafeDownCast(vertexSelectionNode->GetSelectionList());
    qDebug() << vertexIds->GetNumberOfValues() << "points selected";

    auto cellIds = SelectionUtils::verticesToCells(m_mesh->polyData(), vertexIds);

    qDebug() << cellIds->GetNumberOfValues() << "cells selected";

    auto out_node = vtkSmartPointer<vtkSelectionNode>::New();
    out_node->SetFieldType(vtkSelectionNode::CELL);
    out_node->SetContentType(vtkSelectionNode::INDICES);
    out_node->SetSelectionList(cellIds);

    return out_node;
}

void SelectionTool::updateCurrentSelection(vtkSmartPointer<vtkSelectionNode> selectionNode, SelectionMode mode)
{
    switch(mode)
    {
    case SelectionMode::SIMPLE:
        m_currentNode = selectionNode;
        break;
    case SelectionMode::ADD:
        m_currentNode->UnionSelectionList(selectionNode);
        break;
    case SelectionMode::SUBTRACT:
        m_currentNode->SubtractSelectionList(selectionNode);
        break;
    }

    auto selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(m_currentNode);

    // Use the vtkExtractSelection to extract the subset of the mesh
    // (vertices only)
    vtkSmartPointer<vtkExtractSelection> extract =
           vtkSmartPointer<vtkExtractSelection>::New();

    extract->SetInputData(0, m_mesh->polyData());
    extract->SetInputData(1, selection);

    extract->Update();

    // Convert to vtkUnstructuredGrid for visualization
    auto selected = vtkSmartPointer<vtkUnstructuredGrid>::New();

    selected->ShallowCopy(extract->GetOutput());
    updateAreaVisualization(selected);
}

void SelectionTool::PrintArrayInfo(vtkDataSet *polyData, QString datasetName)
{
    qDebug() << "=====" << datasetName << "=====";

    qDebug() << "#points:" << polyData->GetNumberOfPoints();
    qDebug() << "#cells:" << polyData->GetNumberOfCells();

    int nPointArrays = polyData->GetPointData()->GetNumberOfArrays();
    qDebug() << "Point Arrays:";
    for(int i = 0; i < nPointArrays; i++)
    {
        qDebug() << "- " << polyData->GetPointData()->GetArrayName(i) <<
                    polyData->GetPointData()->GetArray(i)->GetSize();
    }

    int nCellArrays = polyData->GetCellData()->GetNumberOfArrays();
    qDebug() << "Cell Arrays:";
    for(int i = 0; i < nCellArrays; i++)
    {
        qDebug() << "- " << polyData->GetCellData()->GetArrayName(i) <<
                    polyData->GetCellData()->GetArray(i)->GetSize();
    }
}

vtkSmartPointer<vtkSelectionNode> SelectionTool::createSelectionNode(const QVector<unsigned int> indices, vtkSelectionNode::SelectionField field)
{
    auto id_array = SelectionUtils::toIdTypeArray(indices.toStdVector());

    auto node = vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(field);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetSelectionList(id_array);
    return node;

}

void SelectionTool::onToolbarInit(QToolBar *toolBar)
{
    toolBar->addSeparator();

    auto interactor = viewerWidget()->renderWindowInteractor();

    m_defaultStyle = interactor->GetInteractorStyle();

    vtkSmartPointer<vtkInteractorStyleDrawPolygon> drawPolygon
            = vtkSmartPointer<vtkInteractorStyleDrawPolygon>::New();
    drawPolygon->AddObserver(vtkCommand::SelectionChangedEvent, this,
        &SelectionTool::SelectionChangedCallbackFunction );

    m_lassoStyle = drawPolygon;

    action_Move_Mode = new QAction("Move", this);
    action_Move_Mode->setCheckable(true);
    action_Move_Mode->setIcon(QIcon(":/images/noun_177497_cc.png"));
    toolBar->addAction(action_Move_Mode);

    connect(action_Move_Mode, &QAction::triggered,
            [this, interactor] {
        interactor->SetInteractorStyle(m_defaultStyle);
    });

    action_Lasso_Selection = new QAction("Lasso", this);
    action_Lasso_Selection->setCheckable(true);
    action_Lasso_Selection->setIcon(QIcon(":/images/noun_177503_cc.png"));

    connect(action_Lasso_Selection, &QAction::triggered,
            [this, interactor] {
        interactor->SetInteractorStyle(m_lassoStyle);
    });

    toolBar->addAction(action_Lasso_Selection);

    QActionGroup *modeGroup = new QActionGroup(this);
    modeGroup->addAction(action_Move_Mode);
    modeGroup->addAction(action_Lasso_Selection);
    action_Move_Mode->trigger();

    toolBar->addSeparator();

    action_Simple_Selection = new QAction("Select", this);
    action_Simple_Selection->setCheckable(true);
    action_Simple_Selection->setIcon(QIcon(":/images/noun_319823_cc.png"));
    toolBar->addAction(action_Simple_Selection);

    connect(action_Simple_Selection, &QAction::toggled,
            [this] () { m_currentSelectionMode = SelectionMode::SIMPLE; });

    action_Add_Selection = new QAction("Add Selection", this);
    action_Add_Selection->setCheckable(true);
    action_Add_Selection->setIcon(QIcon(":/images/noun_320139_cc.png"));

    toolBar->addAction(action_Add_Selection);

    connect(action_Add_Selection, &QAction::toggled,
            [this] () { m_currentSelectionMode = SelectionMode::ADD; });

    action_Subtract_Selection = new QAction("Subtract selection", this);
    action_Subtract_Selection->setCheckable(true);
    action_Subtract_Selection->setIcon(QIcon(":/images/noun_320133_cc.png"));

    connect(action_Subtract_Selection, &QAction::toggled,
            [this] () { m_currentSelectionMode = SelectionMode::SUBTRACT; });

    toolBar->addAction(action_Subtract_Selection);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(action_Simple_Selection);
    group->addAction(action_Add_Selection);
    group->addAction(action_Subtract_Selection);

    action_Simple_Selection->toggle();

    setEnabled(false);
}

