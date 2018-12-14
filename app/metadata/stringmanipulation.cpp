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
#include "stringmanipulation.h"
#include <QRegularExpression>

QString fromCamelCase(const QString &s, const QChar separator)
{
    if(s.isEmpty() || s.isNull())
        return s;

    static QRegularExpression regExp1 {"(.)([A-Z][a-z]+)"};
    static QRegularExpression regExp2 {"([a-z0-9])([A-Z])"};
    static QRegularExpression regExp3 {"([0-9][a-z])"};

    // Replace the camelCase with spaces
    QString repString = QString("\\1%1\\2").arg(separator);
    QString result = s;
    result.replace(regExp1, repString);
    result.replace(regExp2, repString);

    // If a word has two letters, and the first is a number,
    // capitalize the second letter (e.g. "3d" to "3D")
    auto match = regExp3.match(result);
    if(match.hasMatch())
    {
        auto cap = match.captured(0);
        result.replace(regExp3, cap.toUpper());
    }

    // Capitalize the first letter
    result[0] = result[0].toUpper();

    return result;
}
