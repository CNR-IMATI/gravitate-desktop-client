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
#ifndef PROPERTYRENDERING_H
#define PROPERTYRENDERING_H

#include "scalarfieldproperty.h"
#include <vtkSmartPointer.h>
#include <vtkScalarsToColors.h>
#include "qudt_units.h"
#include "core/mesh.h"
#include <QString>

class PropertyColoring;

class AbstractPropertyRendering
{
public:
    AbstractPropertyRendering(PropertyColoring *coloring);

    void apply(const ScalarFieldProperty &property, qudt::Unit unit);

    virtual vtkSmartPointer<vtkScalarsToColors> createLut(Mesh *mesh, qudt::Unit unit) = 0;

    virtual QString name() = 0;

    virtual QString formatString();

    virtual void onAfterPropertyApplied();

    virtual std::vector<double> preprocessData(const std::vector<double> &data, qudt::Unit unit);

protected:
    PropertyColoring *m_coloring;
};

class MeanCurvaturePropertyRendering : public AbstractPropertyRendering
{
public:
    MeanCurvaturePropertyRendering(PropertyColoring *coloring);

    vtkSmartPointer<vtkScalarsToColors> createLut(Mesh *mesh, qudt::Unit unit) override;

    QString name() override;
};

class ShapeIndexPropertyRendering : public AbstractPropertyRendering
{
public:
    ShapeIndexPropertyRendering(PropertyColoring *coloring);

    vtkSmartPointer<vtkScalarsToColors> createLut(Mesh *mesh, qudt::Unit unit) override;

    QString name() override;

};

class SDFPropertyRendering : public AbstractPropertyRendering
{
public:
    SDFPropertyRendering(PropertyColoring *coloring);

    // AbstractPropertyRendering interface
public:
    void onAfterPropertyApplied() override;

    vtkSmartPointer<vtkScalarsToColors> createLut(Mesh *mesh, qudt::Unit unit) override;

    QString name() override;

    QString formatString() override;

    std::vector<double> preprocessData(const std::vector<double> &data, qudt::Unit unit) override;
};

class LightnessPropertyRendering : public AbstractPropertyRendering
{
public:
    LightnessPropertyRendering(PropertyColoring *coloring);

    vtkSmartPointer<vtkScalarsToColors> createLut(Mesh *mesh, qudt::Unit unit) override;

    QString name() override;
};


#endif // PROPERTYRENDERING_H
