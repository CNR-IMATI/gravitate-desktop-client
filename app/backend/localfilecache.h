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
#ifndef LOCALFILECACHE_H
#define LOCALFILECACHE_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QMap>
#include <QByteArray>
#include <QDir>
#include <QJsonDocument>

class LocalFileCache
{
public:
    static LocalFileCache *instance();

    bool isCached(const QUrl &remoteUrl);

    QUrl getLocalUrl(const QUrl &remoteUrl);

    void insert(const QUrl &remoteUrl, const QByteArray &data);

    void setCacheLocation(const QString &path, const QString &defaultPath = "cache");

protected:
    LocalFileCache();

private:
    static LocalFileCache * m_instance;

    QDir getCreateCacheDir();

    QString getCachedFilePath(const QUrl &url);

    QString m_cacheLocation;

};

#endif // LOCALFILECACHE_H
