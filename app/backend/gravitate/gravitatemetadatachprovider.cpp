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
#include "gravitatemetadatachprovider.h"
#include "gravitatebackendconf.h"

GravitateMetadataChProvider::GravitateMetadataChProvider(BackendProvider *backend)
    : m_backend(backend)
{

}

void GravitateMetadataChProvider::requestMetadata(const ArtefactId &artefactId)
{
    if(m_backend == nullptr)
    {
        qCritical() << "BackendProvider is not initialized";
        return;
    }

    QUrlQuery query;
    query.addQueryItem("uri", artefactId.uri.toString(QUrl::FullyEncoded));

    auto reply = m_backend->apiGet(GravitateBackendConf::MetadataChEndpoint, query);

    connect(reply, &QNetworkReply::finished,
            [this, reply] {
        QJsonParseError err;
        auto data = reply->readAll();
        auto doc = QJsonDocument::fromJson(data, &err);
        if(err.error != QJsonParseError::NoError)
        {
            qDebug() << "Error in parsing the metadata reply: "
                     << err.errorString();
            return;
        }

        auto varmap = doc.object().toVariantMap();

        emit received(varmap);
    });

}

GravitateMetadataChProviderSparql::GravitateMetadataChProviderSparql(BackendProvider *backend)
    : m_backend(backend)
{

}

void GravitateMetadataChProviderSparql::requestMetadata(const ArtefactId &artefactId)
{
    if(m_backend == nullptr)
    {
        return;
    }

    QString queryString =
    R"(
    PREFIX rso: <http://www.researchspace.org/ontology/>
    SELECT ?desc WHERE {
        ?id rso:PX_display_wrap ?desc .
        VALUES ?id { <%1> }
    })";

    queryString = queryString.arg(artefactId.toString());

    QNetworkReply *reply = m_backend->sparqlQuery(queryString);

//    auto parentItem = addCategory("Cultural Heritage Object", invisibleRootItem());

    connect(reply, &QNetworkReply::finished,
        [=] {
        if(reply->error() != QNetworkReply::NoError)
        {
            reply->deleteLater();
            return;
        }

        // - Read SPARQL results in a result set object

        auto jsonResults = reply->readAll();
        auto resultSet = SparqlResults::parse(jsonResults);

        QVariantMap map;

        for(auto r : resultSet)
        {


            QString row = r.toMap().value("desc").toString();

            QStringList outlist = row.split("::");

            auto property = outlist[0].trimmed();
            auto value = outlist[1].trimmed();
            map.insert(property, value);
//            this->addMetadataItem(property, value, parentItem );
        }

        emit received(map);
    });

//    parentItem = addCategory("3D Model", invisibleRootItem());
//    parentItem = addCategory("Acquisition Type", parentItem);

}
