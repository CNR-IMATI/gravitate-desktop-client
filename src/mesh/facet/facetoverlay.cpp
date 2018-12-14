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
#include "facetoverlay.h"
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkInformation.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>

// v2 filters
#include <vtkIdFilter.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractUnstructuredGrid.h>
#include <QFile>

FacetOverlay::FacetOverlay(Mesh *mesh, MeshViewWidget *parent)
    : ViewerModule(parent), m_mesh(mesh)
{
    m_loaded = false;
}

void FacetOverlay::apply(const std::vector<Facet> &facets, qudt::Unit unit)
{
    m_facets = QList<Facet>::fromVector(
                QVector<Facet>::fromStdVector(
                    facets
                )
               );
    m_loaded = true;

    applyFacetColors();
}

bool FacetOverlay::loadFct(const QString &fileName)
{
    if(fileName.isEmpty())
    {
        setErrorMessage(tr("Empty file name"));
        return false;
    }

    QFile f(fileName);
    if(!f.exists())
    {
        setErrorMessage(tr("File '%1' does not exists").arg(fileName));
        return false;
    }

    FacetReader reader;
    if(! reader.readFct( fileName.toStdString() ) )
    {
        setErrorMessage(tr("Error parsing .fct file"));
        return false;
    }

    m_facets = QList<Facet>::fromVector( QVector<Facet>::fromStdVector( reader.getFacets() ) );
    m_loaded = true;

    applyFacetColors();
    return true;
}

void FacetOverlay::applyFacetColors()
{
    std::cout << "RegionId\t#Indices\t#Vertices\t#Cells" << std::endl;

    removeCurrentActors();

    for(int i = 0; i < m_facets.size(); i++)
    {
        // Insert the vertices of the selection in an index array
        vtkSmartPointer<vtkIdTypeArray> ids =
            vtkSmartPointer<vtkIdTypeArray>::New();

        ids->SetNumberOfComponents(1);

        Facet f = m_facets[i];

        for(auto j : f.boundaries)
        {
            //ids->InsertNextValue(j);
        }

        for(auto j : f.innerVertices)
        {
            ids->InsertNextValue(j);
        }

        // Create a selection object
        vtkSmartPointer<vtkSelectionNode> node =
                vtkSmartPointer<vtkSelectionNode>::New();
        node->SetFieldType(vtkSelectionNode::POINT);
        node->SetContentType(vtkSelectionNode::INDICES);
        node->SetSelectionList(ids);
        node->GetProperties()->Set(vtkSelectionNode::CONTAINING_CELLS(), 1);

        vtkSmartPointer<vtkSelection> selection =
            vtkSmartPointer<vtkSelection>::New();
        selection->AddNode(node);

        vtkSmartPointer<vtkExtractSelection> extractSelection =
          vtkSmartPointer<vtkExtractSelection>::New();

        extractSelection->SetInputData(0, m_mesh->polyData() );
        extractSelection->SetInputData(1, selection);

        extractSelection->Update();

        vtkSmartPointer<vtkUnstructuredGrid> selected =
          vtkSmartPointer<vtkUnstructuredGrid>::New();
        selected->ShallowCopy(extractSelection->GetOutput());

        vtkSmartPointer<vtkDataSetMapper> mapper =
                vtkSmartPointer<vtkDataSetMapper>::New();

        mapper->SetInputData(selected);
        mapper->ScalarVisibilityOff();

        vtkSmartPointer<vtkActor> selActor =
            vtkSmartPointer<vtkActor>::New();

        selActor->SetMapper(mapper);

        viewerWidget()->renderer()->AddActor(selActor);
        RGB color = m_colorMap.value(f.type);

        selActor->GetProperty()->SetColor(
                    color.r, color.g, color.b
        );

        selActor->GetProperty()->SetOpacity(0.5);

        std::cout << i << "\t"
                  << ids->GetSize() << "\t"
                  << selected->GetNumberOfPoints() << "\t"
                  << selected->GetNumberOfCells()
                  << std::endl;
        m_actors.append(selActor);
    }
    viewerWidget()->update();
}

void FacetOverlay::applyFacetColors_v2()
{
    // This array holds the numerical ID of the facet
    // for each cell (triangle) of the mesh

    int cellNum = m_mesh->polyData()->GetNumberOfCells();

    vtkSmartPointer<vtkIntArray> vtkArray
       = vtkSmartPointer<vtkIntArray>::New();
    vtkArray->SetNumberOfComponents(1);
    vtkArray->SetNumberOfValues(cellNum);

    for(int i = 0; i < m_facets.size(); i++)
    {
        std::vector<int> cellIds = extractCellIds(i);
        for(auto cellId : cellIds)
        {
            vtkArray->SetValue(cellId, i);
        }
    }
}

void FacetOverlay::removeCurrentActors()
{
    for(auto actor : m_actors)
    {
        viewerWidget()->renderer()->RemoveViewProp(actor);
    }

    m_actors.clear();
}

bool FacetOverlay::isLoaded()
{
    return m_loaded;
}

void FacetOverlay::setVisible(bool visible)
{
    for(auto actor: m_actors)
    {
        actor->SetVisibility(visible);
    }
    viewerWidget()->update();
}

void FacetOverlay::reset()
{
    auto renderer = viewerWidget()->renderer();
    for(auto actor : m_actors)
    {
        renderer->RemoveActor(actor);
    }
    m_loaded = false;
    viewerWidget()->update();
}

std::vector<int> FacetOverlay::extractCellIds(int facetId)
{
    vtkSmartPointer<vtkIdTypeArray> vertIds =
        vtkSmartPointer<vtkIdTypeArray>::New();

    vertIds->SetNumberOfComponents(1);

    // It seems that entering the inner vertices is enough
    for(auto i : m_facets[facetId].innerVertices)
    {
        vertIds->InsertNextValue(i);
    }

    // Create a selection object
    vtkSmartPointer<vtkSelectionNode> node =
            vtkSmartPointer<vtkSelectionNode>::New();
    node->SetFieldType(vtkSelectionNode::POINT);
    node->SetContentType(vtkSelectionNode::INDICES);
    node->SetSelectionList(vertIds);
    node->GetProperties()->Set(vtkSelectionNode::CONTAINING_CELLS(), 1);

    vtkSmartPointer<vtkSelection> selection =
        vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(node);

    vtkSmartPointer<vtkExtractSelection> extractSelection =
      vtkSmartPointer<vtkExtractSelection>::New();

    extractSelection->SetInputData(0, m_mesh->polyData() );
    extractSelection->SetInputData(1, selection);

    extractSelection->Update();

    vtkSmartPointer<vtkUnstructuredGrid> selected =
      vtkSmartPointer<vtkUnstructuredGrid>::New();
    selected->ShallowCopy(extractSelection->GetOutput());

    vtkIdTypeArray* originalCellIds = vtkIdTypeArray::SafeDownCast(selected->GetCellData()->GetArray("vtkOriginalCellIds"));

    std::vector<int> outCellIds;

    for(vtkIdType i = 0; i < originalCellIds->GetNumberOfTuples(); i++)
    {
        outCellIds.push_back( (int) originalCellIds->GetValue(i) );
    }

    return outCellIds;
}
