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
#include "mockbackendprovider.h"
#include "artefact3dmetadata.h"
#include "metadata3drequest.h"
#include "qfakenetworkreply.h"
#include "qstringprint.h"
#include "sparqlreplybuilder.h"
#include <memory>
#include <QFile>
#include "backendfactorygravitate.h"
#include "metadata3drequestgravitate.h"

using ::testing::_;
using ::testing::Return;

class MetadataRequestTests : public ::testing::Test
{
public:
    MetadataRequestTests()
        :  req(&backend)
    {
        qRegisterMetaType<Artefact3DMetadata>();
    }

    void configureReplyFromResource(QString resource)
    {
        QFile file(resource);
        file.open(QIODevice::ReadOnly);
        auto data = file.readAll();

        EXPECT_CALL(backend, sparqlQueryReply(_))
                .WillOnce(Return(data));
    }

    QByteArray metadata3DReply();
    QByteArray metadata3DReplyWithout1M();

    QByteArray facetingReply();

    MockBackendProvider backend;
    Metadata3DRequestGravitate req;

    const ArtefactId artefactId = ArtefactId::fromUri(
                QUrl("http://collection.britishmuseum.org/id/object/GAA58781")
    );
};

QByteArray MetadataRequestTests::metadata3DReply()
{
    return SparqlReplyBuilder()
            .addVar("file", SparqlReplyBuilder::URI)
            .addVar("type", SparqlReplyBuilder::URI)
            .addRows(
    {
         {
             "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Intermediate_Models/1909_3-10_71_final.ply",
             "http://gravitate-project.eu/id/object/3D_model_file/final"
         },
         {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_clean.ply",
            "http://gravitate-project.eu/id/object/3D_model_file/clean"
         },
         {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_1M.ply",
            "http://gravitate-project.eu/id/object/3D_model_file/1m"
         },
         {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_100K.ply",
            "http://gravitate-project.eu/id/object/3D_model_file/100K"
         },
         {
           "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_50K.ply",
           "http://gravitate-project.eu/id/object/3D_model_file/50K"
         }
    }).getJson();
}

QByteArray MetadataRequestTests::metadata3DReplyWithout1M()
{
    return SparqlReplyBuilder()
            .addVar("file", SparqlReplyBuilder::URI)
            .addVar("type", SparqlReplyBuilder::URI)
            .addRows(
    {
         {
             "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Intermediate_Models/1909_3-10_71_final.ply",
             "http://gravitate-project.eu/id/object/3D_model_file/final"
         },
         {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_clean.ply",
            "http://gravitate-project.eu/id/object/3D_model_file/clean"
         },
         {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_100K.ply",
            "http://gravitate-project.eu/id/object/3D_model_file/100K"
         },
         {
           "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/1909_3-10_71_50K.ply",
           "http://gravitate-project.eu/id/object/3D_model_file/50K"
         }
    }).getJson();

}

QByteArray MetadataRequestTests::facetingReply()
{
    return SparqlReplyBuilder()
            .addVar("file", SparqlReplyBuilder::URI)
            .addVar("type", SparqlReplyBuilder::URI)
            .addRows(
    {
        {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/Parts/1909_3-10_71_clean.fct",
            "http://gravitate-project.eu/id/object/3D_faceted_model_file/clean"
        },
        {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/Parts/1909_3-10_71_1M.fct",
            "http://gravitate-project.eu/id/object/3D_faceted_model_file/1m"
        },
        {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/Parts/1909_3-10_71_100K.fct",
            "http://gravitate-project.eu/id/object/3D_faceted_model_file/100K"
        },
        {
            "https://gravitate-1.cytera.cyi.ac.cy/repository/British_Museum_Salamis/1909_3-10_71/1909_3-10_71_scan1/Processed_Models/Parts/1909_3-10_71_50K.fct",
            "http://gravitate-project.eu/id/object/3D_faceted_model_file/50K"
        },
    }).getJson();
}

TEST_F(MetadataRequestTests, Request3DMetadataIsValid)
{
    // Arrange
    EXPECT_CALL(backend, sparqlQueryReply(_))
            .WillOnce(Return(metadata3DReply()));


    QSignalSpy spy(&req, &Metadata3DRequest::receiveMetadata);

    // Act
    req.requestMetadata(artefactId);

    // Assert

    ASSERT_TRUE(spy.wait(1000));

    QList<QVariant> arguments = spy.takeFirst();
    Artefact3DMetadata a = qvariant_cast<Artefact3DMetadata>(arguments.at(0));

    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_ORIGINAL, MultiResProperty::MESH));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_FULL, MultiResProperty::MESH));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_1M, MultiResProperty::MESH));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_100K, MultiResProperty::MESH));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_50K, MultiResProperty::MESH));

}

TEST_F(MetadataRequestTests, Request3DMetadataWithout1MWithFallback)
{
    // Arrange
    EXPECT_CALL(backend, sparqlQueryReply(_))
            .WillOnce(Return(metadata3DReplyWithout1M()));

    QSignalSpy spy(&req, &Metadata3DRequest::receiveMetadata);

    // Act
    req.requestMetadata(artefactId);

    // Assert

    ASSERT_TRUE(spy.wait(1000));

    QList<QVariant> arguments = spy.takeFirst();
    Artefact3DMetadata a = qvariant_cast<Artefact3DMetadata>(arguments.at(0));



    ASSERT_EQ(a.getUrl(ModelResolution::RES_FULL, MultiResProperty::MESH, true),
              a.getUrl(ModelResolution::RES_1M, MultiResProperty::MESH, true));
}

TEST_F(MetadataRequestTests, ReturnsObbOK)
{
    SparqlReplyBuilder builder;
    auto replyContent = builder
            .addVar("file", SparqlReplyBuilder::URI)
            .addVar("type", SparqlReplyBuilder::URI)
            .addRow({
                "http://gravitate-1.cytera.cyi.ac.cy/repository/British Museum/1909 3-10 98/1909 3-10 98_scan1/Processed_Models/Properties/1909 3-10 98_obb.json",
                "http://gravitate-project.eu/id/object/minimal_bounding_box"
            })
            .getJson();

    // Arrange
    EXPECT_CALL(backend, sparqlQueryReply(_))
            .WillOnce(Return(replyContent));

    QSignalSpy spy(&req, &Metadata3DRequest::receiveMetadata);

    // Act
    req.requestMetadata(artefactId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));

    QList<QVariant> arguments = spy.takeFirst();
    auto a = qvariant_cast<Artefact3DMetadata>(arguments.at(0));

    ASSERT_FALSE(a.mbbUrl.isEmpty());
}

TEST_F(MetadataRequestTests, ReturnsFacetsOK)
{
    // Arrange
    EXPECT_CALL(backend, sparqlQueryReply(_))
            .WillOnce(Return(facetingReply()));

    QSignalSpy spy(&req, &Metadata3DRequest::receiveMetadata);

    // Act
    req.requestMetadata(artefactId);

    // Assert
    ASSERT_TRUE(spy.wait(1000));

    QList<QVariant> arguments = spy.takeFirst();
    auto a = qvariant_cast<Artefact3DMetadata>(arguments.at(0));

    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_FULL, MultiResProperty::FACETS));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_1M, MultiResProperty::FACETS));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_100K, MultiResProperty::FACETS));
    ASSERT_TRUE(a.hasUrl(ModelResolution::RES_50K, MultiResProperty::FACETS));
}
