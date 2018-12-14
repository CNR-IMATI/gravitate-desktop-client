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
#include "backendfactorygravitate.h"
#include "gravitatesparql.h"
#include "gravitatebackend.h"
#include "gravitateclipgroupsprovider.h"
#include "gravitatethumbnailprovider.h"
#include "gravitatebackendconf.h"
#include "gravitateloginprovider.h"
#include "gravitateitemlistprovider.h"
#include "metadata3drequestgravitate.h"
#include "gravitatemetadatachprovider.h"
#include "networkbackendprovider.h"
#include <QSettings>

// Metadata3DRequest

BackendFactoryGravitate::BackendFactoryGravitate()
{
    m_backend = createBackendProvider();
    m_downloader = GravitateBackend::defaultDownloader();
}

std::unique_ptr<MetadataChProvider> BackendFactoryGravitate::createMetadataChProvider()
{
    return std::unique_ptr<MetadataChProvider>(new GravitateMetadataChProvider(m_backend));
}

std::unique_ptr<Metadata3DRequest> BackendFactoryGravitate::createMetadata3DRequest()
{
    return std::unique_ptr<Metadata3DRequestGravitate>(new Metadata3DRequestGravitate(m_backend));
}

std::unique_ptr<AbstractClipGroupsProvider> BackendFactoryGravitate::createClipGroupsProvider()
{
    return std::unique_ptr<AbstractClipGroupsProvider>(new GravitateClipGroupsProvider(m_backend));
}

std::unique_ptr<AbstractThumbnailProvider> BackendFactoryGravitate::createThumbnailProvider()
{
    return std::unique_ptr<AbstractThumbnailProvider>(new GravitateThumbnailProvider(m_backend, m_downloader));
}

std::unique_ptr<ItemListProviderBase> BackendFactoryGravitate::createItemListProvider()
{
    return std::unique_ptr<ItemListProviderBase>(new GravitateItemListProvider(m_backend));
}

std::unique_ptr<LoginProvider> BackendFactoryGravitate::createLoginProvider()
{
    return std::unique_ptr<LoginProvider>(new GravitateLoginProvider());
}

BackendProvider *BackendFactoryGravitate::createBackendProvider()
{
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    auto backend = new NetworkBackendProvider(qnam);

    QSettings settings;

    // Authentication
    QString user = settings.value("user").toString();
    QString password = settings.value("password").toString();
    Authenticator *auth = new BasicHttpAuth(user, password);
    backend->setAuthenticator(auth);

    // Hostname
    QString hostname = settings.value("hostname").toString();
    backend->setHostname(hostname);

    QObject::connect(qnam, &QNetworkAccessManager::authenticationRequired,
                     auth, &Authenticator::onAuthenticationRequired, Qt::UniqueConnection);

    return backend;
}


void BackendFactoryGravitate::setHostname(const QString &hostname)
{
    ((NetworkBackendProvider*) m_backend)->setHostname(hostname);
}

void BackendFactoryGravitate::setAuthenticator(Authenticator *auth)
{
    auto backend = (NetworkBackendProvider*) m_backend;
    backend->setAuthenticator(auth);
}
