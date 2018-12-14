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

#include "selectionutils.h"
#include <set>
#include <iostream>

namespace SelectionUtils {

vtkSmartPointer<vtkIdTypeArray> verticesToCells(vtkPolyData * polyData, vtkIdTypeArray *vertexIds)
{
    // Initialize the set of the internal points for efficient search
    std::set<vtkIdType> internalPoints;
    for(vtkIdType i = 0; i < vertexIds->GetNumberOfValues(); i++)
    {
        internalPoints.insert(vertexIds->GetValue(i));
    }

    auto isCellInside = [internalPoints, polyData] (vtkIdType cellId)
    {
        auto cell = polyData->GetCell(cellId);
        auto ids = cell->GetPointIds();

        for(vtkIdType i = 0; i < ids->GetNumberOfIds(); i++)
        {
            if( internalPoints.count( ids->GetId(i) ) == 0 )
                return false;
        }

        return true;
    };

    // This ensures that the ids are unique
    std::set<vtkIdType> visited;

    auto ptCells = vtkSmartPointer<vtkIdList>::New();
    auto out_cells = vtkSmartPointer<vtkIdTypeArray>::New();
    vtkIdType cellId;

    for(vtkIdType i = 0; i < vertexIds->GetNumberOfValues(); i++)
    {
        polyData->GetPointCells(vertexIds->GetValue(i), ptCells);

        for (vtkIdType j = 0; j < ptCells->GetNumberOfIds(); j++)
        {
            cellId = ptCells->GetId(j);
            if(isCellInside(cellId) && visited.count(cellId) == 0)
            {
                out_cells->InsertNextValue(cellId);
                visited.insert(cellId);
            }
        }
    }

    return out_cells;
}

vtkSmartPointer<vtkIdTypeArray> cellsToVertices(vtkPolyData * polyData, vtkIdTypeArray *cellIds)
{
    auto out_list = vtkSmartPointer<vtkIdTypeArray>::New();

    std::set<vtkIdType> visited;

    for(vtkIdType i = 0; i < cellIds->GetNumberOfValues(); i++)
    {
        auto points = vtkSmartPointer<vtkIdList>::New();

        polyData->GetCellPoints(cellIds->GetValue(i), points);
        for(vtkIdType j = 0; j < points->GetNumberOfIds(); j++)
        {
            auto pointId = points->GetId(j);
            if(visited.count(pointId) == 0)
            {
                out_list->InsertNextValue(pointId);
            }
        }
    }

    return out_list;
}

vtkSmartPointer<vtkIdList> toIdList(const std::vector<unsigned int> &ids)
{
    auto out_list = vtkSmartPointer<vtkIdList>::New();

    for(auto id : ids)
    {
        out_list->InsertNextId(id);
    }

    return out_list;
}

vtkSmartPointer<vtkIdTypeArray> toIdTypeArray(const std::vector<unsigned int> &ids)
{
    auto out_array = vtkSmartPointer<vtkIdTypeArray>::New();

    for(auto id : ids)
    {
        out_array->InsertNextValue(id);
    }

    return out_array;
}

vtkSmartPointer<vtkIdTypeArray> toIdTypeArray(vtkIdList *ids)
{
    auto out_array = vtkSmartPointer<vtkIdTypeArray>::New();
    for(vtkIdType i = 0; i < ids->GetNumberOfIds(); i++)
    {
        out_array->InsertNextValue(ids->GetId(i));
    }
    return out_array;
}

}
