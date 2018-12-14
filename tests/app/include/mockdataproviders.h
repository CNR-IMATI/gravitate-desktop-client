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
#ifndef MOCKTHUMBNAILPROVIDER_H
#define MOCKTHUMBNAILPROVIDER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "thumbnailprovider.h"
#include <QUrl>
#include <QIcon>
#include <QPixmap>
#include "clipgroupsprovider.h"

class MockThumbnailProvider: public AbstractThumbnailProvider
{
    Q_OBJECT
public:
    explicit MockThumbnailProvider(QObject *parent = 0)
        : AbstractThumbnailProvider (parent)
    {
    }

    MOCK_METHOD1(requestArtefactThumbnail, void(const ArtefactId &artefactId));
    MOCK_METHOD1(getPixmap, QPixmap(const ArtefactId &artefactId));
    MOCK_METHOD1(hasThumbnail, bool(const ArtefactId &artefactId));
};

class MockClipGroupsProvider : public AbstractClipGroupsProvider
{
    Q_OBJECT
public:
    MOCK_METHOD0(requestClipboardContent, void());
    MOCK_METHOD1(requestGroupContent, void(const GroupId &groupId));
    MOCK_METHOD1(createGroup, void(const QString &name));
    MOCK_METHOD0(requestGroupsList, void());
    MOCK_METHOD1(removeResource, void(const QUrl &containerId));
    MOCK_METHOD2(addToGroup, void(const ArtefactId &artefactId, const GroupId &groupId));

};

#endif // MOCKTHUMBNAILPROVIDER_H
