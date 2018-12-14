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
#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QObject>
#include "core/viewermodule.h"
#include "core/mesh.h"
#include <vtkInteractorObserver.h>
#include <vtkVector.h>
#include <vtkExtractSelection.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPoints.h>
#include <vtkDataSet.h>
#include <vtkSelectPolyData.h>
#include <vtkIdTypeArray.h>
#include <QActionGroup>
#include <vtkSelectionNode.h>
#include <QFile>

class SelectionTool : public ViewerModule
{
    Q_OBJECT

    enum class SelectionMode
    {
        SIMPLE, // TODO: rename to REPLACE
        ADD,
        SUBTRACT
    };

    enum class SelectionTarget
    {
        POINTS,
        EDGES,
        SURFACE
    };

public:
    explicit SelectionTool(Mesh *mesh, MeshViewWidget *parent = 0);

    /**
     * @brief setSelectedVertices
     * @param vertices
     *
     * NOTE: This function is currently used to display the feature recognition. Some
     * refactoring work will be necessary for using it to select vertices
     */
    void setSelectedVertices(QVector<unsigned int> vertices);

    void setSelectionTarget(SelectionTarget target);

    QVector<unsigned int> selectedVertices();

    QVector<unsigned int> selectedCells();

    void setEnabled(bool enabled) override;

    void saveSelection(const QString &fileName);

    void loadSelection(const QString &fileName);

public slots:
    void clearSelection();

signals:

protected:
    void onToolbarInit(QToolBar *toolBar) override;

private slots:
    void onModelLoaded();

private:

    void SelectionChangedCallbackFunction(vtkObject *caller, unsigned long eventId, void *callData);

    /**
     * @brief GetSelectionRectViewport returns the min/max coordinates of the viewport
     * @param rect The output rectangle
     */
    void GetSelectionRectViewport(unsigned int rect[]);

    /**
     * @brief GetSelectionRectPolygon returns the min/max coordinates of the selection polygon
     * @param points The vector of points in viewport coordinates
     * @param rect The output rectangle
     */
    void GetSelectionRectPolygon(std::vector<vtkVector2i> points, unsigned int rect[]);

    void updateAreaVisualization(vtkDataSet *extractedSelection);
    void visualizeEdgeLoop(vtkPolyData *edgeLoop);

    vtkSmartPointer<vtkPoints> pickedPointsCoordinates(std::vector<vtkVector2i> pointData);

    vtkSmartPointer<vtkIdTypeArray> extractVertexIdsFromLoopPolygon(std::vector<vtkVector2i> pointData);

    vtkSmartPointer<vtkSelectionNode> toCellSelectionNode(vtkSelectionNode *vertexSelectionNode);

    void updateCurrentSelection(vtkSmartPointer<vtkSelectionNode> selectionNode, SelectionMode mode);

    void PrintArrayInfo(vtkDataSet *polyData, QString datasetName = "");

    Mesh * m_mesh;

    vtkSmartPointer<vtkActor> m_selectionActor;
    vtkSmartPointer<vtkInteractorObserver> m_defaultStyle;
    vtkSmartPointer<vtkInteractorObserver> m_lassoStyle;
    vtkSmartPointer<vtkActor> m_boundaryActor;

    SelectionMode m_currentSelectionMode;
    SelectionTarget m_currentSelectionTarget;
    vtkSmartPointer<vtkSelectionNode> m_currentNode;

    QAction *action_Move_Mode;
    QAction *action_Simple_Selection;
    QAction *action_Lasso_Selection;
    QAction *action_Add_Selection;
    QAction *action_Subtract_Selection;

    vtkSmartPointer<vtkSelectionNode> createSelectionNode(const QVector<unsigned int> indices, vtkSelectionNode::SelectionField field);
    void setCurrentSelection();
};

#endif // SELECTIONTOOL_H
