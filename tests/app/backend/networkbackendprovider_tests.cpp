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
#include "mocknetworkaccessmanager.h"
#include "networkbackendprovider.h"
#include "qfakenetworkreply.h"

using ::testing::_;
using ::testing::Return;

class NetworkBackendProviderTest : public ::testing::Test
{
public:
    NetworkBackendProviderTest()
        : backend(&qnam)
    {
    }

    NetworkBackendProvider backend;
    MockNetworkAccessManager qnam;
};

TEST_F(NetworkBackendProviderTest, apiGetCallsQNAMGet)
{
    // Arrange
    EXPECT_CALL(qnam, createRequest(QNetworkAccessManager::GetOperation, _, _));

    // Act
    backend.apiGet(QString(""));

    // Assert
}

TEST_F(NetworkBackendProviderTest, apiPostCallsQNAMPost)
{
    // Arrange
    EXPECT_CALL(qnam, createRequest(QNetworkAccessManager::PostOperation, _, _));

    // Act
    backend.apiPost(QString(""));

    // Assert
}

TEST_F(NetworkBackendProviderTest, apiDeleteCallsQNAMDelete)
{
    // Arrange
    EXPECT_CALL(qnam, createRequest(QNetworkAccessManager::DeleteOperation, _, _));

    // Act
    backend.apiDelete(QString(""));

    // Assert
}
