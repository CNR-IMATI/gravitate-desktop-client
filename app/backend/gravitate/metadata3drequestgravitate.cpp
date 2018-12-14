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
#include "metadata3drequestgravitate.h"
#include "gravitatesparql.h"
#include "gravitaterepoconf.h"
#include "repofix.h"
#include "gravitateextrametadata.h"

Metadata3DRequestGravitate::Metadata3DRequestGravitate(BackendProvider *backend, QObject *parent)
    : Metadata3DRequest(parent), m_backend(backend)
{
}

void Metadata3DRequestGravitate::requestMetadata(const ArtefactId &artefactId)
{
    QString queryString = GravitateSparql::metadataQuery(artefactId);

    QNetworkReply *reply = m_backend->sparqlQuery(queryString);

    qDebug() << reply->request().url().toString();

    connect(reply, &QNetworkReply::finished,
            [this, reply, artefactId] () {


        auto results = SparqlResults::parse(reply->readAll());

        if(results.count() == 0)
        {
            emit errorReceiveMetadata(QString("Error: no 3D metadata for this item"));
            return;
        }

        Artefact3DMetadata artefact;
        artefact.id = artefactId;

        handleQueryResults(artefact, results);
        emit receiveMetadata(artefact);

        reply->deleteLater();
    });
}

void Metadata3DRequestGravitate::handleQueryResults(Artefact3DMetadata &artefact, const QVariantList &results)
{
    using namespace GravitateRepo;

    for(auto entry : results)
    {
        auto row = entry.toMap();

        QUrl fileUrl = row.value("file").toUrl();
        QString type = row.value("type").toString();
        type = RepoFix::fixResourceType(type, fileUrl.toString());

        QString value = row.value("value").toString();
        QString unitUrl = row.value("unit").toString();
        unitUrl = RepoFix::fixQudtUrl(unitUrl);

        if(fileUrl.isEmpty())
        {
            qWarning() << "Empty value for " << type;
            continue;
        }

        if(type == ID_MBB)
        {
            artefact.mbbUrl = fileUrl;
        }

        // TODO: create a Measure class that encapsulates value and unit of measure
        if(type == ID_AREA)
        {
            artefact.area = value.toDouble();
            artefact.unit = Artefact3DMetadata::fromQudt(unitUrl);
            artefact.unitOfMeasure = Artefact3DMetadata::unitString(artefact.unit);
        }

        if(type == ID_VOLUME)
        {
            artefact.volume = value.toDouble();
        }

        if(PropertyResolutionMapping.contains(type))
        {
            auto pair = PropertyResolutionMapping.value(type);
            auto prop = pair.first;
            auto res = pair.second;
            artefact.setUrl(res, prop, fileUrl);
        }
    }

    addExtraMetadata(artefact);
}

void Metadata3DRequestGravitate::addExtraMetadata(Artefact3DMetadata &artefact)
{
    GravitateExtraMetadata extra;
    if(!extra.readCsv(":/metadata/extra-metadata.csv"))
    {
        qCritical() << Q_FUNC_INFO
                    << "Error while reading 'extra' metadata";
        return;
    }

    if(!extra.hasInfo(artefact.id))
    {
        return;
    }

    auto info = extra.get(artefact.id);

    artefact.avgthickness = convertUnit(info.thickness, artefact.unit, info.unit);
}
