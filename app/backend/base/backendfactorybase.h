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
#ifndef ABSTRACTBACKENDREQUESTFACTORY_H
#define ABSTRACTBACKENDREQUESTFACTORY_H

#include "metadata3drequest.h"
#include <memory>
#include <QObject>
#include "clipgroupsprovider.h"
#include "thumbnailprovider.h"
#include "metadatamodel.h"
#include "loginprovider.h"
#include "itemlistproviderbase.h"

class BackendProvider;

class BackendFactoryBase
{
public:

    static BackendFactoryBase *instance();

    virtual std::unique_ptr<MetadataChProvider> createMetadataChProvider() = 0;

    virtual std::unique_ptr<Metadata3DRequest> createMetadata3DRequest() = 0;

    virtual std::unique_ptr<AbstractClipGroupsProvider> createClipGroupsProvider() = 0;

    virtual std::unique_ptr<AbstractThumbnailProvider> createThumbnailProvider() = 0;

    virtual std::unique_ptr<ItemListProviderBase> createItemListProvider() = 0;

    virtual std::unique_ptr<LoginProvider> createLoginProvider() = 0;

    virtual void setHostname(const QString &hostname);

    virtual void setAuthenticator(Authenticator *auth);

    virtual ~BackendFactoryBase() {}

private:
    static BackendFactoryBase *createFactory();
};





#endif // ABSTRACTBACKENDREQUESTFACTORY_H
