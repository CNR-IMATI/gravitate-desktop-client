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
#include <QUrlQuery>
#include "qstringprint.h"
#include "gravitateclipgroupsprovider.h"
#include "artefactid.h"

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

class ClipGroupsProviderTests : public ::testing::Test
{
public:
    ClipGroupsProviderTests()
        : clipGroups(&backend)
    {
        qRegisterMetaType<ClipboardItem>();

        ON_CALL(backend, apiGetReply( GravitateBackendConf::GroupsEndpoint , _ ) )
            .WillByDefault(ReturnPointee(&groupsReply));

        ON_CALL(backend, apiGetReply( GravitateBackendConf::ContainersEndpoint , _ ) )
            .WillByDefault(ReturnPointee(&containerReply));

        ON_CALL(backend, apiGetReply( GravitateBackendConf::ClipboardEndpoint , _ ) )
            .WillByDefault(ReturnPointee(&clipboardReply));
    }

    static QList<ClipboardItem> toItemList(const QVariant &variant)
    {
        return qvariant_cast<QList<ClipboardItem>>(variant);
    }

    static int itemListSize(const QVariant &list)
    {
        return toItemList(list).size();
    }

    NiceMock<MockBackendProvider> backend;
    GravitateClipGroupsProvider clipGroups;

    QByteArray clipboardReply = "[]";
    QByteArray groupsReply = "[]";
    QByteArray containerReply = "[]";
    GroupId groupId = GroupId::fromUri(QUrl("http://www.example.org/containers/1"));

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

TEST_F(ClipGroupsProviderTests, RequestClipboardContentShouldCallApiGet)
{
    // Arrange
    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::ClipboardEndpoint , _ ));

    // Act
    clipGroups.requestClipboardContent();

    // Assert
}

TEST_F(ClipGroupsProviderTests, LoadClipboardShouldEmitSignalReceived)
{
    // Arrange
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedClipboardContent);

    // Act
    clipGroups.requestClipboardContent();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
}

TEST_F(ClipGroupsProviderTests, LoadClipboardShouldReturnEmptyList)
{
    // Arrange
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedClipboardContent);

    // Act
    clipGroups.requestClipboardContent();

    // Assert
    ASSERT_TRUE(spy.wait(1000));

    auto list = spy.takeFirst().at(0);
    ASSERT_EQ(itemListSize(list), 0);
}

TEST_F(ClipGroupsProviderTests, LoadClipboardShouldReturnFullList)
{
    // Arrange
    clipboardReply = clipboardReplyContent();
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedClipboardContent);

    // Act
    clipGroups.requestClipboardContent();

    // Assert
    ASSERT_TRUE(spy.wait(1000));

    auto list = spy.takeFirst().at(0);
    ASSERT_EQ(itemListSize(list), 4);
}

TEST_F(ClipGroupsProviderTests, RequestGroupsListShouldCallApiGet)
{
    // Arrange
    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::GroupsEndpoint , _ ));

    // Act
    clipGroups.requestGroupsList();

    // Assert
}


TEST_F(ClipGroupsProviderTests, LoadGroupsListShouldEmitSignalReceived)
{
    // Arrange
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedGroupsList);

    // Act
    clipGroups.requestGroupsList();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
}

TEST_F(ClipGroupsProviderTests, LoadGroupsListShouldReturnEmptyList)
{
    // Arrange
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedGroupsList);

    // Act
    clipGroups.requestGroupsList();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    auto list = spy.takeFirst().at(0);
    ASSERT_EQ(itemListSize(list), 0);
}

TEST_F(ClipGroupsProviderTests, LoadGroupsListShouldReturnFullList)
{
    // Arrange
    groupsReply = groupReplyContent();
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedGroupsList);

    // Act
    clipGroups.requestGroupsList();

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    auto list = spy.takeFirst().at(0);
    ASSERT_EQ(itemListSize(list), 2);
}

TEST_F(ClipGroupsProviderTests, RequestGroupContentShouldCallApiGet)
{
    // Arrange
    QUrlQuery q;
    q.addQueryItem("uri", groupId.uri.toString());

    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::ContainersEndpoint , q ));

    // Act
    clipGroups.requestGroupContent(groupId);

    // Assert
}

TEST_F(ClipGroupsProviderTests, LoadGroupShouldEmitSignalReceived)
{
    // Arrange
    //clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedGroupContent);

    // Act
    clipGroups.requestGroupContent(groupId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));
}

TEST_F(ClipGroupsProviderTests, LoadGroupShouldReturnEmptyList)
{
    // Arrange
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedGroupContent);

    // Act
    clipGroups.requestGroupContent(groupId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    auto list = spy.takeFirst().at(0);
    ASSERT_EQ(itemListSize(list), 0);
}

TEST_F(ClipGroupsProviderTests, LoadGroupShouldReturnFullList)
{
    // Arrange
    containerReply = clipboardReplyContent();

    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::receivedGroupContent);

    QUrl containerId("http://www.example.org/containers/1");

    // Act
    clipGroups.requestGroupContent(groupId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));
    auto list = spy.takeFirst().at(0);
    ASSERT_EQ(itemListSize(list), 4);
}

TEST_F(ClipGroupsProviderTests, CreateGroupCallsApiPost)
{
    // Arrange
    QString groupName = "Example 1";

    QUrlQuery q;
    q.addQueryItem("name", groupName);

    EXPECT_CALL(backend, apiPostReply( GravitateBackendConf::GroupsEndpoint , q));

    // Act
    clipGroups.createGroup(groupName);

    // Assert
}

TEST_F(ClipGroupsProviderTests, CreateGroupEmitsGroupCreated)
{
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::groupCreated);

    // Arrange
    QString groupName = "Example 1";

    // Act
    clipGroups.createGroup(groupName);

    // Assert
}

TEST_F(ClipGroupsProviderTests, RemoveResourceCallsApiDelete)
{
    // Arrange
    QUrlQuery q;
    q.addQueryItem("uri", groupId.uri.toString());

    EXPECT_CALL(backend, apiDeleteReply(GravitateBackendConf::ContainersEndpoint, q));

    // Act
    clipGroups.removeResource(groupId.uri);

    // Assert
}

TEST_F(ClipGroupsProviderTests, RemoveResourceEmitsResourceRemoved)
{
    // Arrange
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::resourceRemoved);

    // Act
    clipGroups.removeResource(groupId.uri);

    // Assert
    ASSERT_TRUE(spy.wait(1000));
}

TEST_F(ClipGroupsProviderTests, AddToGroupEmitsArtefactAdded)
{
    auto artefactId = ArtefactId::fromUri(QUrl("http://www.example.org/artefact/1"));
    QSignalSpy spy(&clipGroups, &GravitateClipGroupsProvider::addedToGroup);

    // Act
    clipGroups.addToGroup(artefactId, groupId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));

}



/*
TEST_F(ClipGroupsProviderTests, LoadClipboardEmptyOK)
{
    // Arrange
    groupsReply = groupReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait());
    ASSERT_EQ(model.rowCount(), 2);
}

TEST_F(ClipGroupsProviderTests, ClipboardModelValidIndex)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait());
    QModelIndex index = model.index(0,0);
    ASSERT_TRUE(index.isValid());
}

TEST_F(ClipGroupsProviderTests, ClipboardModelInvalidIndex)
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
    ASSERT_TRUE(spy.wait());
    QModelIndex index = model.index(1,0);
    ASSERT_FALSE(index.isValid());
}

TEST_F(ClipGroupsProviderTests, ClipboardItemContainerIdOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait());
    QModelIndex index = model.index(0,0);
    EXPECT_EQ(model.containerId(index), QUrl("http://localhost:10214/container/Uncategorized/397851eb-e2f1-44ef-aec7-4bcc3efe157a"));
}

TEST_F(ClipGroupsProviderTests, ClipboardItemArtefactIdOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait());
    QModelIndex index = model.index(0,0);
    EXPECT_EQ(model.artefactId(index) , QUrl("http://collection.britishmuseum.org/id/object/MCC6933"));
}

TEST_F(ClipGroupsProviderTests, ClipboardItemLabelOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait());
    QModelIndex index = model.index(0,0);
    EXPECT_EQ(model.label(index), QString("GOLD CASED KEYLESS LEVER POCKET WATCH\n\nCASE"));
}

TEST_F(ClipGroupsProviderTests, ClipboardModelCallThumbnailProvider)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    EXPECT_CALL(thumbnailProvider, requestArtefactThumbnail(_))
            .Times(4);

    // Act
    model.loadClipboard();

    // This is needed to be sure that there are no pending events
    QCoreApplication::processEvents();
}

TEST_F(ClipGroupsProviderTests, ClipboardModelDecorationRoleOK)
{
    // Arrange
    clipboardReply = clipboardReplyContent();

    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    ON_CALL(thumbnailProvider, get(_))
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

TEST_F(ClipGroupsProviderTests, ClipboardModelLoadClipgroups)
{
    clipboardReply = clipboardReplyContent();
    groupsReply = groupReplyContent();

    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert

    ASSERT_TRUE(spy.wait());

    ASSERT_EQ(model.rowCount(), 6);
}

TEST_F(ClipGroupsProviderTests, ClipboardModelChildGroupRowCount)
{
    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert
    ASSERT_TRUE(spy.wait());

    QModelIndex groupIndex = model.index(0, 0, QModelIndex());

    ASSERT_EQ(model.rowCount(groupIndex),0);
}

TEST_F(ClipGroupsProviderTests, ClipboardModelParentIsRoot)
{
    // Arrange
    QSignalSpy spy(&model, &ClipboardModel::groupContentLoaded);

    // Act
    model.loadClipboard();

    // Assert

    ASSERT_TRUE(spy.wait());

    QModelIndex childIndex = model.index(0, 0, QModelIndex());
    QModelIndex rootIndex = model.parent(childIndex);

    ASSERT_EQ(rootIndex, QModelIndex());
}
*/
