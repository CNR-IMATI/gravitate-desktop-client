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
#include "clipboardmodel.h"
#include "mockdataproviders.h"

namespace {

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::ReturnPointee;
using ::testing::DoAll;
using ::testing::ByRef;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::InvokeArgument;
using ::testing::WithArgs;
using ::testing::Assign;
using ::testing::NiceMock;

using ClipboardItemList = QList<ClipboardItem>;

QUrl groupIdUrl(int subId);
QUrl artefactContainerUrl(int groupId, int subId);
QUrl artefactIdUrl(int subId);

class ClipboardModelTests : public ::testing::Test
{
public:
    ClipboardModelTests()
        : model(&clipGroups)
    {
    }

    void SetUp()
    {

    }

    void loadClipboard()
    {
//        EXPECT_CALL(clipGroups, requestClipboardContent());
//        EXPECT_CALL(clipGroups, requestGroupsList());

        model.loadClipboard();
    }

    void setClipboardContent(const ClipboardItemList list)
    {
        EXPECT_CALL(clipGroups, requestClipboardContent())
                .WillOnce(
                    DoAll(
                        Assign(&clipboardContent, list),
                        InvokeWithoutArgs(this, &ClipboardModelTests::emitReceivedClipboardContent))
                    );

    }

    void setGroupList(const ClipboardItemList list)
    {
        EXPECT_CALL(clipGroups, requestGroupsList())
                .WillOnce(
                    DoAll(
                        Assign(&groupsList, list),
                        InvokeWithoutArgs(this, &ClipboardModelTests::emitReceivedGroupsList))
                    );

    }

    void setGroupContent(const ClipboardItemList list, int subId)
    {
        auto url = groupIdUrl(subId);
        auto groupId = GroupId::fromUri(url);

        EXPECT_CALL(clipGroups, requestGroupContent(groupId))
                .WillOnce(
                    DoAll(
                        Assign(&groupContents[subId], list),
                        Assign(&currentSubId, subId),
                        InvokeWithoutArgs(this, &ClipboardModelTests::emitReceivedGroupContent)
                    )
                );
    }

    void emitReceivedClipboardContent()
    {
        emit clipGroups.receivedClipboardContent(clipboardContent);
    }

    void emitReceivedGroupsList()
    {
        emit clipGroups.receivedGroupsList(groupsList);
    }

    void emitReceivedGroupContent()
    {
        auto url = groupIdUrl(currentSubId);
        auto groupId = GroupId::fromUri(url);
        emit clipGroups.receivedGroupContent(groupContents[currentSubId], groupId);
    }

    MockClipGroupsProvider clipGroups;
    //NiceMock<MockClipGroupsProvider> clipGroups;
    ClipboardModel model;

    ClipboardItemList clipboardContent;
    ClipboardItemList groupsList;
    QMap<int, ClipboardItemList> groupContents;

    int currentSubId = 0;

};

QList<ClipboardItem> createItemList(QList<int> indices, int subId = 0)
{
    QList<ClipboardItem> list;
    for(int i : indices)
    {
        auto artefact_uri = artefactIdUrl(subId);
        auto container_uri = artefactContainerUrl(subId, i);

        GResource res {
            ArtefactId::fromUri(artefact_uri),
            GroupId::fromUri(container_uri),
            GResource::Type::ARTEFACT,
            QString("Example item %1").arg(i)
        };

        list.append({ container_uri, res });
    }

    return list;
}

QUrl groupIdUrl(int subId)
{
    return QUrl(QString("http://example.org/container/group/%1")
                .arg(subId));
}

QUrl artefactContainerUrl(int groupId, int subId)
{
    return QUrl(QString("http://example.org/container/artefact/%1/%2")
                .arg(groupId).arg(subId));
}

QUrl artefactIdUrl(int id)
{
    return QUrl(QString("http://example.org/id/%1").arg(id));
}

QList<ClipboardItem> createGroupList(QList<int> indices)
{


    QList<ClipboardItem> list;
    for(int i : indices)
    {
        auto container_uri = groupIdUrl(i);

        GResource res {
            ArtefactId::null(),
            GroupId::fromUri(container_uri),
            GResource::Type::GROUP,
            QString("Example group %1").arg(i)
        };

        list.append({ container_uri, res });
    }
    return list;
}

TEST_F(ClipboardModelTests, RequestClipboardContent)
{
    // Arrange

    // Act
    loadClipboard();

    // Assert
}

TEST_F(ClipboardModelTests, RequestClipboardContentOneItem)
{
    // Arrange
    setClipboardContent(createItemList({0}));

    // Act
    loadClipboard();

    // Assert
    ASSERT_EQ(model.rowCount(), 1);
}

TEST_F(ClipboardModelTests, RequestClipboardContentOneGroup)
{
    // Arrange
    setGroupList(createGroupList({0}));

    // Act
    loadClipboard();

    // Assert
    ASSERT_EQ(model.rowCount(), 1);
}

TEST_F(ClipboardModelTests, RequestClipboardContentOneGroupOneItem)
{
    // Arrange
    setClipboardContent(createItemList({0}));
    setGroupList(createGroupList({1}));

    // Act
    loadClipboard();

    // Assert
    ASSERT_EQ(model.rowCount(), 2);
}

TEST_F(ClipboardModelTests, RequestClipboardContentItemOrdering)
{
    // Arrange
    setGroupList(createGroupList({1,2}));
    setClipboardContent(createItemList({0}));

    // Act
    loadClipboard();

    auto i0 = model.index(0, 0);
    auto group0Id = model.containerId(i0);
    EXPECT_EQ(group0Id, QUrl("http://example.org/container/group/1"));

    auto i1 = model.index(1, 0);
    auto group1Id = model.containerId(i1);
    EXPECT_EQ(group1Id, QUrl("http://example.org/container/group/2"));

    auto i2 = model.index(2, 0);
    auto itemId = model.containerId(i2);
    EXPECT_EQ(itemId, QUrl("http://example.org/container/artefact/0/0"));
}

TEST_F(ClipboardModelTests, RequestGroupContent)
{
    int subId = 1;
    setGroupList(createGroupList({1,2}));
    setGroupContent(createItemList({0}, subId), subId);

    loadClipboard();

    auto i0 = model.index(0, 0);
    auto group0Id = model.containerId(i0);
    EXPECT_TRUE(i0.isValid());
    EXPECT_EQ(group0Id, QUrl("http://example.org/container/group/1"));

    model.fetchMore(i0);

    auto i1 = model.index(0, 0, i0);
    auto group1Id = model.containerId(i1);
    EXPECT_TRUE(i1.isValid());
    EXPECT_EQ(group1Id, QUrl("http://example.org/container/artefact/1/0"));
}

TEST_F(ClipboardModelTests, ClipboardUpdatedAddedItem)
{
    setClipboardContent(createItemList({0}));

    loadClipboard();

    EXPECT_EQ(model.rowCount(), 1);

    setClipboardContent(createItemList({0,1}));

    loadClipboard();

    EXPECT_EQ(model.rowCount(), 2);
}


TEST_F(ClipboardModelTests, ClipboardUpdatedRemovedItem)
{
    setClipboardContent(createItemList({0,1}));

    loadClipboard();

    EXPECT_EQ(model.rowCount(), 2);

    setClipboardContent(createItemList({0}));

    loadClipboard();

    EXPECT_EQ(model.rowCount(), 1);
}

}
