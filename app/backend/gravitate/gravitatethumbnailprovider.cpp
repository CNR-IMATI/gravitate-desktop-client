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
#include "gravitatethumbnailprovider.h"
#include "gravitatebackendconf.h"
#include "gravitatesparql.h"
#include <QPainter>

GravitateThumbnailProvider::GravitateThumbnailProvider(BackendProvider *backend, AbstractDownloader *downloader, QObject *parent)
    : AbstractThumbnailProvider (parent), m_backend(backend), m_downloader(downloader)
{
    m_emptyPixmap = QPixmap(m_pixmapSize, m_pixmapSize);
    m_emptyPixmap.fill(Qt::gray);
}

bool GravitateThumbnailProvider::hasThumbnail(const ArtefactId &artefactId)
{
    return m_loadedPixmaps.contains(artefactId);
}

QPixmap GravitateThumbnailProvider::getPixmap(const ArtefactId &artefactId)
{
    if(!m_requestedPixmaps.contains(artefactId))
    {
        requestArtefactThumbnail(artefactId);
    }

    return m_loadedPixmaps.value(artefactId, m_emptyPixmap);
}

void GravitateThumbnailProvider::requestArtefactThumbnail(const ArtefactId &artefactId)
{
//    requestThumbnailSparql(artefactId);
    requestThumbnailApi(artefactId);
    m_requestedPixmaps.append(artefactId);
}

void GravitateThumbnailProvider::requestThumbnailApi(const ArtefactId &artefactId)
{
    if(m_backend == nullptr)
    {
        qCritical() << "BackendProvider not initialized";
        return;
    }

    if(m_downloader == nullptr)
    {
        qCritical() << "Downloader not initialized";
        return;
    }

    QUrlQuery query;
    query.addQueryItem("repository", "default");

    QJsonDocument doc;

    doc.setArray(QJsonArray::fromVariantList({ artefactId.uri.toString(QUrl::FullyEncoded) }));

    auto reply = m_backend->apiPostJson(doc, GravitateBackendConf::ThumbnailEndpoint, query);

    connect(reply, &QNetworkReply::finished,
    [this, reply, artefactId] {

        if(reply->error() != QNetworkReply::NoError)
        {
            return;
        }

        auto data = reply->readAll();

        if(data.isEmpty())
        {
            return;
        }

        auto doc = QJsonDocument::fromJson(data);

        auto map = doc.toVariant().toMap();

        bool found = false;
        for(auto id : map.keys())
        {
            auto thumbUrl = map.value(id).toUrl();
            if(isImageFile(thumbUrl))
            {
                auto artefactId = ArtefactId::fromUri(QUrl(id));

                handleThumbnailUrl(artefactId, thumbUrl);
                found = true;
                break;
            }

            if(thumbUrl.fileName().endsWith(".ply"))
            {
                qCritical() << artefactId.toString() << "thumbnail is a PLY file";
                qCritical() << "   " << thumbUrl.toString();
            }

            if(thumbUrl.isEmpty())
            {
                qCritical() << artefactId.toString() << "has no thumbnail";
            }

        }

//        if(!found)
//        {
//            qWarning().noquote() << "Thumbnail not found for artefact" << artefactId.toString();
//        }
    });
}

void GravitateThumbnailProvider::requestThumbnailSparql(const ArtefactId &artefactId)
{
    QString queryString = GravitateSparql::thumbnailQuery(artefactId);

    QNetworkReply *reply = m_backend->sparqlQuery(queryString);
    connect(reply, &QNetworkReply::finished,
            [this, reply, artefactId] () {

        auto jsonReply = reply->readAll();
        auto res = SparqlResults::parse(jsonReply);

        if(res.count() == 0)
        {
            reply->deleteLater();
            qCritical() << "No results from query" << Q_FUNC_INFO;
            return;
        }

        // TODO complete
        auto thumbUrl = res[0].toMap().value("url", QUrl()).toUrl();

        if(thumbUrl.isValid())
        {
            handleThumbnailUrl(artefactId, thumbUrl);
        }
        else
        {
            qCritical().noquote() << artefactId.toString()
                                  << ";"
                                  << thumbUrl.toString();
        }

        reply->deleteLater();
    });

}

QPixmap GravitateThumbnailProvider::createSquarePixmap(const QPixmap &pixmap, int size)
{
    QPixmap outPix(size, size);
    outPix.fill(Qt::transparent);

    QPixmap resized = pixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPainter painter(&outPix);

    int hpad = 0, vpad = 0;

    if(resized.width() > resized.height())
    {
        vpad = (size - resized.height()) / 2;
    }
    else
    {
        hpad = (size - resized.width()) / 2;
    }

    painter.drawPixmap(hpad, vpad, resized.width(), resized.height(), pixmap);

    return outPix;
}

void GravitateThumbnailProvider::handleThumbnailUrl(const ArtefactId &artefactId, const QUrl &thumbUrl)
{
    if(!isImageFile(thumbUrl))
    {
        qCritical() << "Thumbnail for artefact" << artefactId.toString()
                    << "is not an image file:" << thumbUrl.toString();
        return;
    }

    bool ok;

    auto thumbData = m_downloader->getFile(thumbUrl, &ok);

    if(!ok)
    {
        qCritical() << "Error downloading thumbnail file:" << thumbUrl.toString();
        return;
    }

    QPixmap pixmap;
    if(!pixmap.loadFromData(thumbData))
    {
        qCritical() << "Error with loading pixmap:" << thumbUrl.toString();
        return;
    }

    auto resizedPixmap = createSquarePixmap(pixmap, m_pixmapSize);
    m_loadedPixmaps.insert(artefactId, resizedPixmap);

    // FIXME: thumbnailReceived has a QPixmap argument
    // Should it be changed to QIcon?
    emit thumbnailReceived(pixmap, artefactId);

}

bool GravitateThumbnailProvider::isImageFile(const QUrl &thumbUrl)
{
    return thumbUrl.fileName().endsWith(".png") ||
           thumbUrl.fileName().endsWith(".jpg");
}
