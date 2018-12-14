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
#include "abstractdownloader.h"
#include "mocknetworkaccessmanager.h"
#include "qfakenetworkreply.h"
#include <QTemporaryFile>

using ::testing::_;
using ::testing::Return;

/**
 * @brief Automatically generated Google Test skeleton
 */
class TestDownloader : public ::testing::Test
{
protected:
    TestDownloader()
        : net_dl(&qnam)
    {
        // You can do set-up work for each test here.
    }

    MockNetworkAccessManager qnam;
    NetworkDownloader net_dl;
};

TEST_F(TestDownloader, NetworkDownloaderTestOK) {

    QUrl url("http://www.example.com/thumbnail.png");

    bool ok;

    QFile file(":/images/test-image-128.png");
    file.open(QIODevice::ReadOnly);
    auto orig_data = file.readAll();

    QFakeNetworkReply* reply = QFakeNetworkReply::fromResource(":/images/test-image-128.png");

    EXPECT_CALL(qnam, createRequest(QNetworkAccessManager::GetOperation, _, _))
            .WillOnce(Return(reply->finish()));

    auto data = net_dl.getFile(url, &ok);

    EXPECT_TRUE(ok);
    EXPECT_EQ(orig_data, data);
}
