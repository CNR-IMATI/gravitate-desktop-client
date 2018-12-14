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
#ifndef TRANSFERFUNCTIONFACTORY_H
#define TRANSFERFUNCTIONFACTORY_H

#include "meshpropertyreader.h"
#include "core/mesh.h"
#include <vtkSmartPointer.h>
#include <vtkScalarsToColors.h>
#include <vtkColorSeries.h>
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>

class TransferFunctionFactory
{
public:
    TransferFunctionFactory();

    vtkSmartPointer<vtkScalarsToColors> createLut(PropertyType type, Mesh *mesh);

    vtkSmartPointer<vtkColorTransferFunction> makeCtfDiverging(vtkColorSeries *colorSeries, double range, double midpoint, double compress, bool invert = false);
    vtkSmartPointer<vtkColorTransferFunction> makeCtf(vtkColorSeries *colorSeries, double min, double max, bool invert = false);
    vtkSmartPointer<vtkColorTransferFunction> makeBwLut(double min, double max);

};

#endif // TRANSFERFUNCTIONFACTORY_H
