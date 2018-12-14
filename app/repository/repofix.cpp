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

#include "repofix.h"
#include "gravitaterepoconf.h"
#include <QRegularExpression>

QString RepoFix::fixQudtUrl(const QString &unitUrl)
{
    // URLs in the repository use fragment
    // e.g. http://qudt.org/vocab/unit#m
    // The correct form is:
    // http://qudt.org/vocab/unit/M
    // (slash followed by uppercase letters)

    QRegularExpression re(R"(http:\/\/qudt\.org\/vocab\/unit#(\w+))");
    if(!re.isValid())
        return unitUrl;

    auto m = re.match(unitUrl);
    if(!m.hasMatch())
        return unitUrl;

    auto unit = m.captured(1);
    return "http://qudt.org/vocab/unit/" + unit.toUpper();
    // auto suffix = QString(unitUrl).replace("http://qudt.org/vocab/unit#", "");
}

QString RepoFix::getTypeUriByResourceUrl(const QString &fileUrl)
{
    if(fileUrl.endsWith("area"))
        return GravitateRepo::ID_AREA;
    if(fileUrl.endsWith("volume"))
        return GravitateRepo::ID_VOLUME;
    if(fileUrl.endsWith("LAB.txt"))
        return GravitateRepo::ID_LAB_FINAL;

    return QString();
}

QString RepoFix::fixResourceType(const QString &typeUri, const QString &resourceUri)
{
    if(GravitateRepo::AllowedPropertyTypeUri.contains(typeUri))
        return typeUri;

    return getTypeUriByResourceUrl(resourceUri);
}
