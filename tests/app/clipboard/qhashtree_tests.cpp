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
#include <QString>
#include "qhashtree.h"

class QHashTreeTests : public ::testing::Test
{
public:
    QHashTreeTests()
    {
        quintptr root = tree.insertRoot(1);
        tree.insert(2, root);
        tree.insert(3, root);
    }

    QHashTree<int> tree;
};

TEST(QHashTreeInit, TestCreateTree)
{
    // Arrange
    QHashTree<int> t;

    // Act
    t.insertRoot(1);

    // Assert
    ASSERT_TRUE(t.contains(t.rootId()));
}

TEST_F(QHashTreeTests, TestGetNodeSuccess)
{
    // Arrange
    quintptr root = tree.rootId();

    // Act
    quintptr A = tree.child(root, 0);
    quintptr B = tree.child(root, 1);

    // Assert
    ASSERT_EQ(tree.getNode(root), 1);
    ASSERT_EQ(tree.getNode(A), 2);
    ASSERT_EQ(tree.getNode(B), 3);
}

TEST_F(QHashTreeTests, TestChildCount)
{
    // Arrange
    // Act
    quintptr root = tree.rootId();

    // Assert
    ASSERT_EQ(tree.childCount(root), 2);
}

TEST_F(QHashTreeTests, TestRow)
{
    // Arrange
    quintptr root = tree.rootId();
    quintptr A = tree.child(root, 0);
    quintptr B = tree.child(root, 1);
    // Act

    int idx0 = tree.row(A);
    int idx1 = tree.row(B);
    // Assert

    ASSERT_EQ(idx0, 0);
    ASSERT_EQ(idx1, 1);
}

TEST_F(QHashTreeTests, TestParentCall)
{
    // Arrange
    quintptr root = tree.rootId();
    quintptr A = tree.child(root, 0);
    // Act

    quintptr parent = tree.parent(A);

    // Assert
    ASSERT_EQ(root, parent);
}


