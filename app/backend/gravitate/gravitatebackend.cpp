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
#include "gravitatebackend.h"
#include <QSettings>
#include "authenticator.h"

BackendProvider * const GravitateBackend::backendProvider()
{


    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    static NetworkBackendProvider backend(qnam);

    QSettings settings;

    // Authentication
    QString user = settings.value("user").toString();
    QString password = settings.value("password").toString();
    Authenticator *auth = new BasicHttpAuth(user, password);
    backend.setAuthenticator(auth);

    // Hostname
    QString hostname = settings.value("hostname").toString();
    backend.setHostname(hostname);

    QObject::connect(qnam, &QNetworkAccessManager::authenticationRequired,
                     auth, &Authenticator::onAuthenticationRequired, Qt::UniqueConnection);

    return &backend;
}

AbstractDownloader * const GravitateBackend::defaultDownloader()
{
    QNetworkAccessManager * qnam = new QNetworkAccessManager();
    static NetworkDownloader net_dl(qnam);

    QSettings settings;
    // Authentication
    QString user = settings.value("user").toString();
    QString password = settings.value("password").toString();
    Authenticator *auth = new BasicHttpAuth(user, password);
    net_dl.setAuthenticator(auth);

    QObject::connect(qnam, &QNetworkAccessManager::authenticationRequired,
                     auth, &Authenticator::onAuthenticationRequired, Qt::UniqueConnection);

    static CachedDownloader cache_dl(&net_dl, LocalFileCache::instance());

    return &cache_dl;
}

GravitateJobManager * const GravitateBackend::jobManager()
{
    // HACK: quick and dirty way to use different credentials
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    static NetworkBackendProvider schedulerBackend(qnam);

    QSettings settings;

    // Authentication
    QString user = settings.value("scheduler/user").toString();
    QString password = settings.value("scheduler/password").toString();
    Authenticator *auth = new BasicHttpAuth(user, password);
    schedulerBackend.setAuthenticator(auth);

    if(user.isEmpty())
    {
        qCritical() << "Scheduler user name is empty";
    }

    // Hostname
    QString hostname = settings.value("hostname").toString();
    schedulerBackend.setHostname(hostname);

    QObject::connect(qnam, &QNetworkAccessManager::authenticationRequired,
                     auth, &Authenticator::onAuthenticationRequired, Qt::UniqueConnection);


    static GravitateJobManager jobManager(&schedulerBackend);

    return &jobManager;
}

GravitateBackend::GravitateBackend()
{

}
