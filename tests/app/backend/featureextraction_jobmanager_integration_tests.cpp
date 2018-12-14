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
#include <QSignalSpy>
#include <QFile>

#include "mockbackendprovider.h"
#include "featureextractionalgorithm.h"
#include "gravitatejobmanager.h"
#include "gravitatebackendconf.h"

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;

/**
 * @brief Automatically generated Google Test skeleton
 */
class FeatureExtractionJobManagerIntegrationTests : public ::testing::Test
{
protected:
    FeatureExtractionJobManagerIntegrationTests() :
        jobManager(&backend),
        algo(&jobManager)
    {
        // You can do set-up work for each test here.
        params.modelUrl = "https://example.org/D_292_100K.ply";
        params.facetUrl = "https://example.org/D_292_100K.fct";
        params.curveId  = "GP";
        params.parD = 10;
        params.parN = 55;
        params.minPts = 30;
        params.eps = 0.5;
        params.useFacet = true;
        params.erosion = 5;

        jobStartedReply = R"({"id":"1234"})";
        jobId = "1234";

        backend.replyDelay = 500;

        QFile replyFile(":/demo/D_292_100K_Features.json");
        replyFile.open(QIODevice::ReadOnly);
        jobResultReply = replyFile.readAll();

        QFile successFile(":/api/feature-extraction-success-log.txt");
        successFile.open(QIODevice::ReadOnly);
        jobSuccessLog = replyFile.readAll();

        ON_CALL(backend, apiPostReply( GravitateBackendConf::SchedulerJobsEndpoint, _ ) )
            .WillByDefault(Return(jobStartedReply));

        ON_CALL(backend, apiGetReply( GravitateBackendConf::JobsResults(jobId), _ ) )
            .WillByDefault(Return(jobResultReply));

    }

    virtual ~FeatureExtractionJobManagerIntegrationTests() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
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

    QByteArray jobStartedReply;
    JobId jobId;
    QByteArray jobResultReply;
    QByteArray jobSuccessLog;

    NiceMock<MockBackendProvider> backend;
    GravitateJobManager jobManager;
    FeatureExtractionAlgorithm algo;
    FeatureExtractionParams params;
};

TEST_F(FeatureExtractionJobManagerIntegrationTests, OnRunVerifyJobStarted)
{
    EXPECT_CALL(backend, apiPostReply( GravitateBackendConf::SchedulerJobsEndpoint, _ ) );

    QSignalSpy spy(&algo, &FeatureExtractionAlgorithm::finished);

    // Act
    bool result = algo.run(params);

    ASSERT_TRUE(spy.wait());

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(FeatureExtractionJobManagerIntegrationTests, OnJobFinishedEmitFinishedSignal)
{
    // Arrange
    EXPECT_CALL(backend, apiPostReply( GravitateBackendConf::SchedulerJobsEndpoint, _ ) );

    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::JobsResults(jobId), _ ) );

    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::JobLog(jobId), _ ) )
        .WillRepeatedly(Return(jobSuccessLog));

    QSignalSpy spy(&algo, &FeatureExtractionAlgorithm::finished);
    ASSERT_TRUE(spy.isValid());

    algo.run(params);

    ASSERT_TRUE(spy.wait());

    // Assert
    ASSERT_EQ(spy.count(), 1);
}
