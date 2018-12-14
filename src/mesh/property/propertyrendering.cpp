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
#include "propertyrendering.h"
#include "transferfunctionfactory.h"
#include <vtkNew.h>
#include <vtkColorSeries.h>
#include "propertycoloring.h"
#include <QObject>
#include "unitofmeasure.h"

AbstractPropertyRendering::AbstractPropertyRendering(PropertyColoring *coloring)
    : m_coloring(coloring)
{

}

void AbstractPropertyRendering::apply(const ScalarFieldProperty &property, qudt::Unit unit)
{
    m_coloring->applyRendering(property, this, unit);
}

QString AbstractPropertyRendering::formatString()
{
    return "%#-6.3g";
}

void AbstractPropertyRendering::onAfterPropertyApplied()
{

}

std::vector<double> AbstractPropertyRendering::preprocessData(const std::vector<double> &data, qudt::Unit unit)
{
    return data;
}

MeanCurvaturePropertyRendering::MeanCurvaturePropertyRendering(PropertyColoring *coloring)
    : AbstractPropertyRendering (coloring)
{

}

vtkSmartPointer<vtkScalarsToColors> MeanCurvaturePropertyRendering::createLut(Mesh *mesh, qudt::Unit unit)
{
    vtkNew<vtkColorSeries> series;

    TransferFunctionFactory factory;

    // Automatic min and max;
    double level = 0.0;
    double window = 0.2;
    double contrast = 0.95;
    // Diverging color scale
    // Contrast = 0.95
    // Color scheme: diverging spectral ColorBrewer scheme (11 colors)
    series->SetColorScheme(vtkColorSeries::BREWER_DIVERGING_SPECTRAL_11);
    return factory.makeCtfDiverging(series.Get(), window, level, contrast);
}

QString MeanCurvaturePropertyRendering::name()
{
    return QObject::tr("Mean Curvature");
}

ShapeIndexPropertyRendering::ShapeIndexPropertyRendering(PropertyColoring *coloring)
    : AbstractPropertyRendering (coloring)
{

}

vtkSmartPointer<vtkScalarsToColors> ShapeIndexPropertyRendering::createLut(Mesh *mesh, qudt::Unit unit)
{
    vtkNew<vtkColorSeries> series;

    TransferFunctionFactory factory;

    double level = 0.0;
    double window = 1.0;
    double contrast = 0.0;
    // Diverging color scale
    // Contrast = 0
    // Color scheme: diverging spectral ColorBrewer scheme (11 colors)
    series->SetColorScheme(vtkColorSeries::BREWER_DIVERGING_SPECTRAL_11);
    return factory.makeCtfDiverging(series.Get(), window, level, contrast);

}

QString ShapeIndexPropertyRendering::name()
{
    return QObject::tr("Shape Index");
}

SDFPropertyRendering::SDFPropertyRendering(PropertyColoring *coloring)
    : AbstractPropertyRendering (coloring)
{

}

void SDFPropertyRendering::onAfterPropertyApplied()
{
}

vtkSmartPointer<vtkScalarsToColors> SDFPropertyRendering::createLut(Mesh *mesh, qudt::Unit unit)
{
    vtkNew<vtkColorSeries> series;

    TransferFunctionFactory factory;

    double min = 0;
    double max = 5;
    bool invert = true;
    series->SetColorScheme(vtkColorSeries::BREWER_SEQUENTIAL_YELLOW_ORANGE_BROWN_9);
    return factory.makeCtf(series.Get(), min, max, invert);
}

QString SDFPropertyRendering::name()
{
    return QObject::tr("Shape Diameter Function");
}

QString SDFPropertyRendering::formatString()
{
    return AbstractPropertyRendering::formatString() + " cm";
}

LightnessPropertyRendering::LightnessPropertyRendering(PropertyColoring *coloring)
    : AbstractPropertyRendering (coloring)
{

}

vtkSmartPointer<vtkScalarsToColors> LightnessPropertyRendering::createLut(Mesh *mesh, qudt::Unit unit)
{
    TransferFunctionFactory factory;

    // min = 10, max = 98
    // Linear color scale
    // Color scheme: black to white
    double min = 10;
    double max = 98;
    return factory.makeBwLut(min, max);
}

QString LightnessPropertyRendering::name()
{
    return QObject::tr("Lightness");
}



std::vector<double> SDFPropertyRendering::preprocessData(const std::vector<double> &data, qudt::Unit unit)
{
    bool ok;

    auto targetUnit = qudt::CENTIMETER;

    if(unit == qudt::UNKNOWN)
    {
        throw std::runtime_error("Unknown unit of measure, cannot visualize SDF");
    }

    auto out = convertUnitOfMeasure(data, unit, targetUnit, &ok);

    if(!ok)
    {
        throw std::runtime_error("Error in unit of measure conversion");
    }

    return out;
}
