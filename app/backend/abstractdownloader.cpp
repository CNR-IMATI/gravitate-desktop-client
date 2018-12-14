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
#include "abstractdownloader.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QEventLoop>

NetworkDownloader::NetworkDownloader(QNetworkAccessManager *qnam, QObject *parent)
    : AbstractDownloader (parent), m_qnam(qnam), m_auth(Q_NULLPTR)
{

}

void NetworkDownloader::setAuthenticator(Authenticator *auth)
{
    m_auth = auth;
}

QByteArray NetworkDownloader::getFile(const QUrl &url, bool *ok)
{
    if(m_qnam == Q_NULLPTR)
    {
        emit finished(false,
            tr("QNetworkAccessManager has not been initialized"));
        return QByteArray();
    }

    QNetworkRequest request(url);

    if(m_auth != Q_NULLPTR)
    {
        m_auth->addAuthenticationHeader(request);
    }


    auto reply = m_qnam->get(request);

    QByteArray data;

    bool success = false;

    connect(reply, &QNetworkReply::downloadProgress,
            this, &AbstractDownloader::downloadProgress);

    connect(reply, &QNetworkReply::finished,
            [this, reply, &data, &success] {
        success = reply->error() == QNetworkReply::NoError;
        QString errorMsg;

        if(success)
        {
            data = reply->readAll();
        }
        else
        {
            errorMsg = reply->errorString();
            qCritical() << errorMsg;
        }
        emit finished(success, errorMsg);
    });

    QEventLoop loop;

    connect(reply, &QNetworkReply::finished,
            &loop, &QEventLoop::quit);

    connect(this, &AbstractDownloader::canceled,
            reply, &QNetworkReply::abort);

    loop.exec();

    // TODO add timeout
    if(ok != nullptr)
    {
        *ok = success;
    }
    return data;
}

QString NetworkDownloader::getLocalPath(const QUrl &url)
{
    return QString();
}

void AbstractDownloader::cancel()
{
    emit canceled();
}

AbstractDownloader::AbstractDownloader(QObject *parent)
    : QObject(parent)
{

}

CachedDownloader::CachedDownloader(AbstractDownloader *downloader, LocalFileCache *cache, QObject *parent)
    : AbstractDownloader(parent), m_downloader(downloader), m_cache(cache)
{
    connect(downloader, &AbstractDownloader::downloadProgress,
            this, &AbstractDownloader::downloadProgress);

    connect(downloader, &AbstractDownloader::finished,
            this, &AbstractDownloader::finished);

    connect(this, &AbstractDownloader::canceled,
            downloader, &AbstractDownloader::canceled);
}

QByteArray CachedDownloader::getFile(const QUrl &url, bool *ok)
{
    QFile file(getLocalPath(url));

    if(!file.open(QIODevice::ReadOnly))
    {
        if(ok != nullptr)
            *ok = false;
        return QByteArray();
    }
    if(ok != nullptr)
        *ok = true;
    return file.readAll();
}

QString CachedDownloader::getLocalPath(const QUrl &url)
{
    if(!m_cache->isCached(url))
    {
        bool ok;


        QByteArray data = m_downloader->getFile(url, &ok);

        if(!ok)
        {
            return QString();
        }

        m_cache->insert(url, data);
    }

    auto localUrl = m_cache->getLocalUrl(url);

    return localUrl.toLocalFile();

}
