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
#include "gtest/gtest.h"
#include "metadatamodel.h"
#include "mockbackendprovider.h"
#include "backendfactorygravitate.h"
#include "gravitatemetadatachprovider.h"
#include <QFile>
#include <QSignalSpy>

using ::testing::_;
using ::testing::Return;

/**
 * @brief Automatically generated Google Test skeleton
 */
class GravitateChMetadataProviderTests : public ::testing::Test
{
protected:
    GravitateChMetadataProviderTests()
        : provider(&backend)
    {
        // You can do set-up work for each test here.
    }

    virtual ~GravitateChMetadataProviderTests() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        query.addQueryItem("uri", artefactId.uri.toString());

        QFile f(":/api/sample-metadata-reply.json");
        ASSERT_TRUE(f.open(QIODevice::ReadOnly));

        sampleMetadataReply = f.readAll();
    }

    void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    static void SetUpTestCase() {
        // Called before the first test in this test case.
    }

    static void TearDownTestCase() {
        // Called after the last test in this test case.
    }

    const ArtefactId artefactId = ArtefactId::fromUri(
                QUrl("http://www.mcw.gov.cy/AyiaIrini/id/object/AI_1249")
    );
    const QString route = "/gravapi/fragmentmetadata/ch";
    QUrlQuery query;

    MockBackendProvider backend;
    GravitateMetadataChProvider provider;

    QByteArray sampleMetadataReply;

    const QList<QString> categoriesNames = {
        "3D Model",
        "Cultural Heritage Collection",
        "Cultural Heritage Object"
    };
};

TEST_F(GravitateChMetadataProviderTests, ApiCallOK) {

    // Arrange
    EXPECT_CALL(backend, apiGetReply(route, query))
            .WillOnce(Return(sampleMetadataReply));

    QSignalSpy spy(&backend, &MockBackendProvider::replyFinished);

    // Act
    provider.requestMetadata(artefactId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));
}

TEST_F(GravitateChMetadataProviderTests, DISABLED_RootCategoriesOK) {

    // Arrange
    EXPECT_CALL(backend, apiGetReply(route, query))
            .WillOnce(Return(sampleMetadataReply));

    QSignalSpy spy(&backend, &MockBackendProvider::replyFinished);

    // Act
    provider.requestMetadata(artefactId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));

//    ASSERT_EQ(m_model.rowCount(), 3);
//
//    for(int i = 0; i < m_model.rowCount(); i++)
//    {
//        auto index = m_model.index(i, 0, QModelIndex());
//        ASSERT_TRUE(index.isValid());
//        auto result = index.data(Qt::DisplayRole).toString();
//        auto expectedResult = categoriesNames[i];
//        EXPECT_EQ(result , expectedResult);
//    }
}
