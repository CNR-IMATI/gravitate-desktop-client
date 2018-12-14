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
#ifndef ABSTRACTDOWNLOADER_H
#define ABSTRACTDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include "authenticator.h"
#include "localfilecache.h"

class AbstractDownloader : public QObject
{
    Q_OBJECT
public:
    virtual QByteArray getFile(const QUrl &url, bool *ok = Q_NULLPTR) = 0;
    virtual QString getLocalPath(const QUrl &url) = 0;

public slots:
    virtual void cancel();

protected:
    explicit AbstractDownloader(QObject * parent = Q_NULLPTR);
    virtual ~AbstractDownloader() {}

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished(bool success, const QString &errMsg);
    void canceled();
};

class NetworkDownloader : public AbstractDownloader
{
    Q_OBJECT
public:
    explicit NetworkDownloader(QNetworkAccessManager *qnam, QObject *parent = Q_NULLPTR);
    void setAuthenticator(Authenticator *auth);

private:
    QNetworkAccessManager *m_qnam;
    Authenticator *m_auth;

    // AbstractDownloader interface
public:
    QByteArray getFile(const QUrl &url, bool *ok) override;
    QString getLocalPath(const QUrl &url) override;
};

class CachedDownloader : public AbstractDownloader
{
    Q_OBJECT
public:
    explicit CachedDownloader(AbstractDownloader *downloader, LocalFileCache *cache, QObject *parent = Q_NULLPTR);

    // AbstractDownloader interface
public:
    QByteArray getFile(const QUrl &url, bool *ok) override;
    QString getLocalPath(const QUrl &url) override;

private:
    AbstractDownloader *m_downloader;
    LocalFileCache *m_cache;
};

#endif // ABSTRACTDOWNLOADER_H
