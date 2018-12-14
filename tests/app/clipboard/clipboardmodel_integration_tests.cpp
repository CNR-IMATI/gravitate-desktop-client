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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QSignalSpy>
#include <QSharedPointer>
#include <QResource>
#include <QFile>
#include <QVariant>
#include <QIcon>
#include "clipboardmodel.h"
#include "mockbackendprovider.h"
#include "qfakenetworkreply.h"
#include <QList>
#include "gravitatebackendconf.h"
#include "mockdownloader.h"
#include <QUrl>
#include <QSize>
#include "mockdataproviders.h"
#include "gravitateclipgroupsprovider.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::ReturnPointee;
using ::testing::DoAll;
using ::testing::ByRef;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::WithArgs;
using ::testing::NiceMock;

class ClipboardModelIntegrationTests : public ::testing::Test
{
public:
    ClipboardModelIntegrationTests()
        : clipGroups(&backend), model(&clipGroups)
    {
        model.setThumbnailProvider(&thumbnailProvider);

        EXPECT_CALL(backend, apiGetReply( QString(GravitateBackendConf::GroupsEndpoint) , _ ) )
            .WillRepeatedly(ReturnPointee(&groupsReply));

        EXPECT_CALL(backend, apiGetReply( QString(GravitateBackendConf::ContainersEndpoint) , _ ) )
            .WillRepeatedly(ReturnPointee(&containerReply));

        EXPECT_CALL(backend, apiGetReply( QString(GravitateBackendConf::ClipboardEndpoint) , _ ) )
            .WillRepeatedly(ReturnPointee(&clipboardReply));

        EXPECT_CALL(backend, sparqlQueryReply( _ ))
            .WillRepeatedly(Return(sparqlThumbnailReplyContent()));

        const QString iconFile = ":/images/test-image-128.png";

        QFile file(iconFile);
        file.open(QIODevice::ReadOnly);
        auto bytes = file.readAll();

        testPixmap = QPixmap(iconFile);

        ON_CALL(thumbnailProvider, requestArtefactThumbnail(_))
                .WillByDefault(WithArgs<0>(Invoke( [this] (const ArtefactId &artefactId) {
            emit thumbnailProvider.thumbnailReceived(testPixmap, artefactId);
        })));
    }


    GravitateClipGroupsProvider clipGroups;
    ClipboardModel model;
    MockBackendProvider backend;
    NiceMock<MockThumbnailProvider> thumbnailProvider;

    QPixmap testPixmap;

    QByteArray clipboardReply = "[]";
    QByteArray groupsReply = "[]";
    QByteArray containerReply = "[]";

    QByteArray clipboardReplyContent()
    {
        return
    R"([
        {
         "container_id": "http://localhost:10214/container/Uncategorized/397851eb-e2f1-44ef-aec7-4bcc3efe157a",
         "uri": "http://collection.britishmuseum.org/id/object/MCC6933",
         "label": "GOLD CASED KEYLESS LEVER POCKET WATCH\n\nCASE"
        },
        {
         "container_id": "http://localhost:10214/container/Uncategorized/e7a7ddc1-49df-499c-93a2-13eab1b1d6d8",
         "uri": "http://collection.britishmuseum.org/id/object/CBA266435",
         "label": "Banknote"
        },
        {
         "container_id": "http://localhost:10214/container/Uncategorized/b3afb922-5d06-4882-895d-82b8f2d82748",
         "uri": "http://collection.britishmuseum.org/id/object/COC231583",
         "label": "Coronation medal of Ghazi ud Din Haidar"
        },
        {
         "container_id": "http://localhost:10214/container/Uncategorized/1c0b6735-f7dc-4f91-8e23-cca553a16900",
         "uri": "http://collection.britishmuseum.org/id/object/JCF8939",
         "label": "Album of 9 fan paintings"
        }
    ])";
    }

    QByteArray groupReplyContent()
    {
        return R"([
               {
                 "id": "http://localhost:10214/container/setContainer/b09d3dee-ec0f-4497-919d-47601d4dcbce",
                 "label": "askdjfla"
               },
               {
                 "id": "http://localhost:10214/container/setContainer/caae2e83-aabd-4024-b922-1bb6896ba30b",
                 "label": "123123123"
               }
             ])";
    }

    QByteArray sparqlThumbnailReplyContent()
    {
        return R"({
               "head": { "vars": [ "id" , "thumbnail" ]
               } ,
               "results": {
                 "bindings": [
                   {
                     "id": { "type": "uri" , "value": "http://collection.britishmuseum.org/id/object/MCC6933" } ,
                     "thumbnail": { "type": "uri" , "value": "http://collection.britishmuseum.org/id/object/MCC6933/thumbnail.jpg" }
                   }
                 ]
               }
              })";
    }
};


TEST_F(ClipboardModelIntegrationTests, LoadClipboardGroupsEmptyOK)
{
    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    ASSERT_EQ(model.rowCount(), 0);
}

TEST_F(ClipboardModelIntegrationTests, LoadGroupsEmptyOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    ASSERT_EQ(model.rowCount(), 4);
}

TEST_F(ClipboardModelIntegrationTests, LoadClipboardEmptyOK)
{
    // Arrange
    groupsReply = groupReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    ASSERT_EQ(model.rowCount(), 2);
}

TEST_F(ClipboardModelIntegrationTests, ClipboardModelValidIndex)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    QModelIndex index = model.index(0,0);
    ASSERT_TRUE(index.isValid());
}

TEST_F(ClipboardModelIntegrationTests, ClipboardModelInvalidIndex)
{
    clipboardReply = R"([{
         "container_id": "http://localhost:10214/container/Uncategorized/397851eb-e2f1-44ef-aec7-4bcc3efe157a",
         "uri": "http://collection.britishmuseum.org/id/object/MCC6933",
         "label": "GOLD CASED KEYLESS LEVER POCKET WATCH\n\nCASE"
    }])";

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    QModelIndex index = model.index(1,0);
    ASSERT_FALSE(index.isValid());
}

TEST_F(ClipboardModelIntegrationTests, ClipboardItemContainerIdOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    QModelIndex index = model.index(0,0);
    EXPECT_EQ(model.containerId(index), QUrl("http://localhost:10214/container/Uncategorized/397851eb-e2f1-44ef-aec7-4bcc3efe157a"));
}

TEST_F(ClipboardModelIntegrationTests, ClipboardItemArtefactIdOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    QModelIndex index = model.index(0,0);
    EXPECT_EQ(model.artefactId(index) , QUrl("http://collection.britishmuseum.org/id/object/MCC6933"));
}

TEST_F(ClipboardModelIntegrationTests, ClipboardItemLabelOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    QModelIndex index = model.index(0,0);
    EXPECT_EQ(model.label(index), QString("GOLD CASED KEYLESS LEVER POCKET WATCH\n\nCASE"));
}

TEST_F(ClipboardModelIntegrationTests, ClipboardModelCallThumbnailProvider)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    EXPECT_CALL(thumbnailProvider, requestArtefactThumbnail(_))
            .Times(4);

    // Act
    model.loadClipboard();

    // This is needed to be sure that there are no pending events
    qApp->processEvents();
}

TEST_F(ClipboardModelIntegrationTests, DISABLED_ClipboardModelDecorationRoleOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    ON_CALL(thumbnailProvider, getPixmap(_))
            .WillByDefault(Return(testPixmap));

    // Act
    model.loadClipboard();

    ASSERT_TRUE(spy.wait(1000));

    // Assert
    auto index = model.index(0,0);
    ASSERT_TRUE(index.isValid());

    auto realIcon = qvariant_cast<QIcon>(model.data(index, Qt::DecorationRole));
    // To compare two pixmaps, convert to images first
    auto realImage = realIcon.pixmap(QSize(32, 32)).toImage();

    auto testIcon = QIcon(testPixmap);
    auto testImage = testIcon.pixmap(QSize(32, 32)).toImage();

    EXPECT_EQ(realImage, testImage);
}

TEST_F(ClipboardModelIntegrationTests, ClipboardModelLoadClipgroups)
{
    clipboardReply = clipboardReplyContent();
    groupsReply = groupReplyContent();

    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert

    ASSERT_TRUE(spy.wait(1000));

    ASSERT_EQ(model.rowCount(), 6);
}

TEST_F(ClipboardModelIntegrationTests, ClipboardModelChildGroupRowCount)
{
    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait(1000));

    QModelIndex groupIndex = model.index(0, 0, QModelIndex());

    ASSERT_EQ(model.rowCount(groupIndex),0);
}

TEST_F(ClipboardModelIntegrationTests, ClipboardModelParentIsRoot)
{
    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert

    ASSERT_TRUE(spy.wait(1000));

    QModelIndex childIndex = model.index(0, 0, QModelIndex());
    QModelIndex rootIndex = model.parent(childIndex);

    ASSERT_EQ(rootIndex, QModelIndex());
}



