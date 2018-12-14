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
#include "thumbnailprovider.h"
#include "mockbackendprovider.h"
#include "mockdownloader.h"
#include "qfakenetworkreply.h"
#include "sparqlreplybuilder.h"
#include "gravitatethumbnailprovider.h"

#include <QBuffer>
#include <QFile>
#include <QSignalSpy>

using ::testing::_;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Return;

/**
 * @brief Automatically generated Google Test skeleton
 */
class ThumbnailProviderTests : public ::testing::Test
{
protected:
    void setupDefaultPixmap()
    {
        QFile file(":/images/test-image-128.png");
        file.open(QIODevice::ReadOnly);
        defaultPixmapData = file.readAll();
    }

    ThumbnailProviderTests()
        : thumbnailProvider(&backend, &downloader) {
        // You can do set-up work for each test here.

        setupDefaultPixmap();

        auto jsonReply = R"({
          "http://example.com/artefacts/artefact01": "http://example.com/images/image01.png"
        })";

        EXPECT_CALL(backend, apiPostJsonReply(_,_,_))
            .WillRepeatedly(Return( jsonReply ));

        EXPECT_CALL(downloader, getFile(_, _))
            .WillRepeatedly(
                    DoAll(SetArgPointee<1>(true),
                    Return(defaultPixmapData)
                )
            );

        EXPECT_CALL(backend, sparqlQueryReply(_))
                .WillRepeatedly(Return(queryReply()));
    }

    QByteArray queryReply();

    GravitateThumbnailProvider thumbnailProvider;
    MockBackendProvider backend;
    MockDownloader downloader;
    QByteArray defaultPixmapData;
    ArtefactId artefactId = ArtefactId::fromUri(QUrl("http://example.com/artefacts/artefact01"));

};

QByteArray ThumbnailProviderTests::queryReply()
{
    return SparqlReplyBuilder()
            .addVar("id", SparqlReplyBuilder::URI)
            .addVar("thumbnail", SparqlReplyBuilder::URI)
            .addVar("name", SparqlReplyBuilder::LITERAL)
            .addRow({
                "http://example.com/artefacts/artefact01",
                "http://example.com/images/image01.png",
                "Head Fragment"
            })
            .getJson();
}

TEST_F(ThumbnailProviderTests, GetPixmapOK)
{
    // Arrange
    QSignalSpy spy(&thumbnailProvider, &GravitateThumbnailProvider::thumbnailReceived);

    thumbnailProvider.requestArtefactThumbnail(artefactId);

    // Act
    // Assert
    ASSERT_TRUE(spy.wait(1000));

    auto args = spy.takeFirst();
    auto p1 = qvariant_cast<QPixmap>(args.at(0));

    QPixmap p2;
    p2.loadFromData(defaultPixmapData);

    ASSERT_EQ(p1.toImage(), p2.toImage());
}
