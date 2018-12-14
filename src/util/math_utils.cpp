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
#include "math_utils.h"
#include <QDebug>
#include <QString>
#include <exception>
#include <algorithm>

QVector<double> enhance_saturation(QVector<double> field, double tau)
{
    // This function performs a transformation of the scalar field similar to
    // https://it.mathworks.com/matlabcentral/fileexchange/5355-toolbox-graph/content/toolbox_graph/perform_saturation.m

    double sum = std::accumulate(field.constBegin(), field.constEnd(), 0.0);
    double mean = sum / field.size();

    QVector<double> temp(field.size());

    // step 1: translate all values by the mean
    std::transform(field.constBegin(), field.constEnd(), temp.begin(),
                   [=] (double v ) { return v - mean; }
    );
    // step 2: compute Mean Absolute Deviation
    double madTimes2 = mean_absolute_deviation(temp) * 2;

    // step 3: divide all values by 2*mad AND clamp between -tau and tau
    std::for_each(temp.begin(), temp.end(),
                  [=] (double &value) {
        value = clamp(value / madTimes2, -tau, tau);

    });

    qDebug() << "enhancing saturation...";
    qDebug() << "tau: " << tau;
    qDebug() << "mean: " << mean;
    qDebug() << "mad: " << (madTimes2 / 2.0);


    return temp;
}

double mean_absolute_deviation(QVector<double> field)
{
    double sum = std::accumulate(field.constBegin(), field.constEnd(), 0.0);
    double mean = sum / field.size();

    double abs_sum = std::accumulate(field.begin(), field.end(), 0.0,
                                      [&] (double v1, double v2) {
        return v1 + std::abs(v2 - mean);
    });
    return abs_sum / field.size();
}

double clamp(double value, double low, double high)
{
    return std::min(high, std::max(value, low));
}

