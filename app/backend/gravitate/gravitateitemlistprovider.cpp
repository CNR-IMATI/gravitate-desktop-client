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
#include "gravitateitemlistprovider.h"
#include <QUrl>
#include <QVariant>
#include <QVariantList>
#include "gravitatesparql.h"

GravitateItemListProvider::GravitateItemListProvider(BackendProvider *backend, QObject *parent)
    : ItemListProviderBase (parent)
{
    m_backend = backend;
}

void GravitateItemListProvider::requestItems(const QList<ArtefactId> &artefactIds)
{
    if(m_backend == nullptr)
    {
        qCritical() << "BackendProvider not set correctly;" << Q_FUNC_INFO;
    }

    auto reply = makeItemRequest(artefactIds);

    connect(reply, &QNetworkReply::finished,
        [this, reply] {
        if(reply->error() != QNetworkReply::NoError)
        {
            reply->deleteLater();
            qCritical() << reply->errorString();
            return;
        }

        auto jsonResults = reply->readAll();

        auto results = SparqlResults::parse(jsonResults);

        QVariantList resultList;

        if(results.count() == 0)
        {
            // If result set is empty, no update is needed
            emit itemsReceived(resultList);
            return;
        }

        for(auto resultRow : results)
        {
            auto row = resultRow.toMap();

            QVariantMap out;
            out["id"] = row.value("id").toUrl();
            out["inventoryId"] = row.value("inventoryId").toString();
            out["description"] = row.value("description").toString();

            resultList.append(row);
        }

        emit itemsReceived(resultList);
    });

}

QNetworkReply *GravitateItemListProvider::makeItemRequest(const QList<ArtefactId> &artefactIds)
{
    using namespace GravitateSparql;

    QString queryString =
        R"(
        PREFIX skos: <http://www.w3.org/2004/02/skos/core#>
        PREFIX crm: <http://www.cidoc-crm.org/cidoc-crm/>
        PREFIX rso: <http://www.researchspace.org/ontology/>
        PREFIX gra: <http://gravitate.org/id/identifier/>

        SELECT DISTINCT ?id ?inventoryId ?description
        WHERE
        {
            VALUES ?id { <%1> }
            OPTIONAL { ?id gra:has_inventory_id ?inventoryId }
            OPTIONAL { ?id rso:displayLabel ?description }
        })";

    auto urls = toQUrlList(artefactIds);

    QStringList l = QUrl::toStringList(urls, QUrl::FullyEncoded);

    queryString = queryString.arg(l.join(">\n<"));

    return m_backend->sparqlQuery(queryString);
}
