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
#include "propertycoloring.h"
#include <QDebug>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include "gravitaterepoutils.h"
#include "transferfunctionfactory.h"
#include "unitofmeasure.h"
#include <QVector>

PropertyColoring::PropertyColoring(Mesh * const mesh, MeshViewWidget *parent)
    : ViewerModule(parent), m_mesh(mesh)
{
    m_scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    reset();
}

void PropertyColoring::applyRendering(const ScalarFieldProperty &property, AbstractPropertyRendering *ren, qudt::Unit unit)
{
    // Load scalar field
    auto name = ren->name();
    auto type = property.type;
    auto target = property.target;

    auto propFormat = ren->formatString().toStdString();
    m_scalarBar->SetLabelFormat(propFormat.c_str());

    // Transform data to adapt to the rendering

    auto c_data = ren->preprocessData(property.data, unit);
    auto data = QVector<double>::fromStdVector(c_data);

    if(target == PropertyTarget::VERTICES)
    {
        m_mesh->loadVertexProperty(name, data);
    }
    else if(target == PropertyTarget::TRIANGLES)
    {
        m_mesh->loadCellProperty(name, data);
    }
    else
    {
        throw std::logic_error("Target for property is invalid (no vertex or cell)");
    }

    auto lut = ren->createLut(m_mesh, unit);
    addProperty(type, lut);

    selectProperty(type);

    ren->onAfterPropertyApplied();
}

void PropertyColoring::addProperty(PropertyType type, vtkSmartPointer<vtkScalarsToColors> lut)
{
    auto propName = GravitateRepoUtils::propertyName(type);
    auto target = GravitateRepoUtils::propertyTarget(type);

    m_lutMap[type] = {propName, target, lut};
}

void PropertyColoring::applyPropertyColoring(vtkScalarsToColors *lut)
{

    m_mesh->polyDataMapper()->SetLookupTable(lut);

    m_scalarBar->SetLookupTable(lut);
    viewerWidget()->renderer()->AddActor(m_scalarBar);
}

QList<PropertyType> PropertyColoring::loadedProperties()
{
    return m_lutMap.keys();
}

Mesh *PropertyColoring::mesh()
{
    return m_mesh;
}

void PropertyColoring::reset()
{    
    m_lutMap.clear();
    addProperty(PropertyType::RGB_COLOR, nullptr);
}

void PropertyColoring::selectProperty(PropertyType property)
{
    auto propName = GravitateRepoUtils::propertyName(property);

    if(!m_lutMap.contains(property))
    {
        qCritical() << "Property" << propName << "not loaded";
        return;
    }

    auto prop = m_lutMap.value(property);

    viewerWidget()->renderer()->RemoveActor(m_scalarBar);
    auto arrayName = prop.name.toStdString();

    // AR: arrayName and cname cannot be collapsed in a single statement
    // i.e. name.toStdString().c_str()
    // because, on Windows systems, the resulting string could be empty
    auto cname = arrayName.c_str();
    auto target = prop.target;
    auto lut = prop.lut;

    if(lut != nullptr)
    {
        applyPropertyColoring(lut);
    }

    if(target == PropertyTarget::VERTICES)
    {
        m_mesh->polyDataMapper()->SetScalarModeToUsePointData();
        m_mesh->polyData()->GetPointData()->SetActiveScalars(cname);
    }
    else if(target == PropertyTarget::TRIANGLES)
    {
        m_mesh->polyDataMapper()->SetScalarModeToUseCellData();
        m_mesh->polyData()->GetCellData()->SetActiveScalars(cname);
    }

    viewerWidget()->update();
}
