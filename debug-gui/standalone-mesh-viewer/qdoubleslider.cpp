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
#include "qdoubleslider.h"

QDoubleSlider::QDoubleSlider(QWidget *parent)
    : QSlider(parent)
{

}

QDoubleSlider::QDoubleSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
    setMinimum(0);
    setMaximum(100);
}

double QDoubleSlider::doubleValue() const
{
    int min = minimum(),
        max = maximum();
    double k = double(value()) / 100.;
    double outVal = min + (max - min) * k;
    return outVal;
}

double QDoubleSlider::doubleMinimum() const
{
    return m_minValue;
}

double QDoubleSlider::doubleMaximum() const
{
    return m_maxValue;
}

void QDoubleSlider::setDoubleValue(double value)
{

}

void QDoubleSlider::setDoubleMinimum(double value)
{
    m_minValue = value;
}

void QDoubleSlider::setDoubleMaximum(double value)
{
    m_maxValue = value;
}
