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

#include "meshpropertyreader.h"
#include "scalarfieldreader.h"

MeshPropertyReader::MeshPropertyReader(PropertyType type)
{
    switch (type) {
    case PropertyType::MEAN_CURVATURE:
    case PropertyType::SHAPE_INDEX:
    case PropertyType::SDF:
        m_columnNumber = 1;
        m_outputColumn = 0;
        break;
    case PropertyType::LIGHTNESS:
        m_columnNumber = 3;
        m_outputColumn = 0;
        break;
    default:
        break;
    }
}

bool MeshPropertyReader::readTxt(const QString &file)
{
    ScalarFieldReader reader(m_columnNumber);

    if(!reader.readTxt(file.toStdString()))
    {
        qCritical("Error reading file %s", qUtf8Printable(file));
        return false;
    }

    m_data = QVector<double>::fromStdVector(reader.getColumn(m_outputColumn));
    return true;
}

QVector<double> MeshPropertyReader::data()
{
    return m_data;
}
