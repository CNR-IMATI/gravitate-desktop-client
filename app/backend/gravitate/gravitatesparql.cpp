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
#include "gravitatesparql.h"

#include <QString>
#include <QFile>
#include <QUrl>
#include <QDebug>

QString GravitateSparql::metadataQuery(const ArtefactId &artefactId)
{
    QList<ArtefactId> l;
    l.append(artefactId);
    return metadataQuery(l);
}

QString GravitateSparql::metadataQuery(const QList<ArtefactId> &artefacts)
{
    QString query =
    R"sparql(
    PREFIX crm: <http://www.cidoc-crm.org/cidoc-crm/>
    PREFIX rso: <http://www.researchspace.org/ontology/>
    PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
    PREFIX grav: <http://gravitate-project.eu/>

    SELECT ?id ?file ?type ?value ?unit
    {
        {
            ?id crm:P138i_has_representation ?file
        }
        UNION
        {
            ?id crm:P39i_was_measured_by ?event .
            ?event crm:P40_observed_dimension ?file .
            OPTIONAL { ?file crm:P90_has_value ?value }
            OPTIONAL { ?file crm:P91_has_unit ?unit }
        }
        OPTIONAL { ?file crm:P2_has_type ?type . }
        VALUES ?id { <%1> }
    }
    )sparql";

    auto artefactStringList = QUrl::toStringList(toQUrlList(artefacts), QUrl::FullyEncoded);
    return query.arg(artefactStringList.join(">\n<"));
}

QString GravitateSparql::thumbnailQuery(const ArtefactId &artefactId)
{
    QString queryString = R"sparql(
        PREFIX crm: <http://www.cidoc-crm.org/cidoc-crm/>
        PREFIX rso: <http://www.researchspace.org/ontology/>
        PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
        PREFIX grav: <http://gravitate-project.eu/>

        SELECT ?id ?url
        {
        ?id rso:PX_has_main_representation ?url .
        VALUES ?id { <%1> }
        }
    )sparql";

    return queryString.arg(artefactId.uri.toString(QUrl::FullyEncoded));
}

QList<QUrl> GravitateSparql::toQUrlList(const QList<ArtefactId> &artefacts)
{
    QList<QUrl> urls;
    for(auto a : artefacts)
    {
        urls.append(a.uri);
    }
    return urls;
}
