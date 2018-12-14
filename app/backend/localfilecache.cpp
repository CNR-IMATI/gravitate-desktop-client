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
#include "localfilecache.h"
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include "gravitatebackend.h"

LocalFileCache *LocalFileCache::m_instance = nullptr;

LocalFileCache::LocalFileCache()
{
}

QDir LocalFileCache::getCreateCacheDir()
{
    bool success = QDir().mkpath(m_cacheLocation);

    if(!success)
    {
        throw std::runtime_error("ERROR: could not create root cache directory");
    }

    return QDir(m_cacheLocation);
}

LocalFileCache *LocalFileCache::instance()
{
    if(m_instance == nullptr)
    {
        m_instance = new LocalFileCache();
    }
    return m_instance;
}

bool LocalFileCache::isCached(const QUrl &remoteUrl)
{
    return getLocalUrl(remoteUrl).isValid();
}

QUrl LocalFileCache::getLocalUrl(const QUrl &remoteUrl)
{
    try {
        QString cachedFilePath = getCachedFilePath(remoteUrl);

        QFileInfo info(cachedFilePath);
        if(info.exists())
        {
            return QUrl::fromLocalFile(cachedFilePath);
        }
        return QUrl();
    }
    catch(std::exception e)
    {
        qCritical() << e.what();
        return QUrl();
    }
}

QString LocalFileCache::getCachedFilePath(const QUrl &url)
{
    QDir dir = getCreateCacheDir();

    QString sep = QDir::separator();

    QString fileName = url.fileName();

    QString host = url.host();

    QString path = url.path().remove(fileName);

    QString finalPath = QDir::cleanPath(dir.absolutePath() + sep + host + sep + path);

    bool success = QDir().mkpath(finalPath);

    if(!success)
    {
        throw std::runtime_error("ERROR: could not create cache subdirectory");
    }

    QFileInfo info(finalPath, fileName);

    return info.absoluteFilePath();
}

void LocalFileCache::insert(const QUrl &remoteUrl, const QByteArray &data)
{
    // Generate the path for the cached file
    QString cachedFilePath = getCachedFilePath(remoteUrl);

    // Open a file handle
    QFile f(cachedFilePath);

    if(!f.open(QIODevice::WriteOnly))
    {
        qCritical() << "Could not open file" << f.fileName();
        return;
    }

    // Write the file
    qint64 retval = f.write(data);

    // Close the file handle
    f.close();

    if(retval < 0)
    {
        qCritical() << "Could not write file" << f.fileName();
        return;
    }

    QUrl cachedUrl = QUrl::fromLocalFile(cachedFilePath);
}

void LocalFileCache::setCacheLocation(const QString &path, const QString &defaultPath)
{
    QFileInfo info(path);

    bool valid = true;

    if(!info.isDir())
    {
        valid = false;
    }

    QString absDir;

    if(info.isAbsolute() && valid)
    {
        absDir = QDir(path).absolutePath();
    }
    else
    {
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        absDir = path.isEmpty() ?
              appDataPath : QDir::cleanPath(appDataPath + QDir::separator() + defaultPath);
    }

    m_cacheLocation = absDir;
    qInfo() << "Current cache location:" << m_cacheLocation;
}
