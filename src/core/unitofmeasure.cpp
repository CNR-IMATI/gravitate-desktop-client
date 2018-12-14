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
#include "unitofmeasure.h"
#include "qudt_units.h"

double convertUnit(double value, qudt::Unit source, qudt::Unit target, bool *ok)
{
    try {
        auto sourceToMeter = conversionMultiplier(source);
        auto targetToMeter = conversionMultiplier(target);

        if(targetToMeter == 0.0)
        {
            if(ok != nullptr)
                *ok = false;
            return 0;
        }

        auto conversionFactor = sourceToMeter / targetToMeter;

        if(ok != nullptr)
            *ok = true;

        return value * conversionFactor;

    } catch (std::exception) {

        if(ok != nullptr)
            *ok = false;
        return 0;
    }
}


double conversionMultiplier(qudt::Unit source)
{
    auto unit = getInfo(source);
    return unit.conversionMultiplier;
}

QVector<double> convertUnitOfMeasure(const QVector<double> &in, qudt::Unit source, qudt::Unit target, bool *ok)
{
    QVector<double> out;
    out.resize(in.size());
    bool transform_ok = true;

    std::transform(in.begin(), in.end(), out.begin(),
        [source, target, &transform_ok] (double value)
    {
        bool ok;
        auto converted = convertUnit(value, source, target, &ok);
        transform_ok = ok && transform_ok;
        return converted;
    });

    if(ok != nullptr)
    {
        *ok = transform_ok;
    }

    if(!transform_ok)
        return in;

    return out;
}
