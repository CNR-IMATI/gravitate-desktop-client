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

#include "gravitatejobmanager.h"
#include "mockbackendprovider.h"
#include "gravitatebackendconf.h"
#include <QSignalSpy>
#include "qfakenetworkreply.h"
#include "gravitatebackendconf.h"
#include <QMetaType>
#include <QTestEventLoop>
#include <QFile>
#include <QString>

using ::testing::_;
using ::testing::Return;

/**
 * @brief Automatically generated Google Test skeleton
 */
class JobManagerTests : public ::testing::Test
{
protected:
    JobManagerTests()
        : manager(&backend)
    {
        // You can do set-up work for each test here.
        QFile f1(":/api/sample-job-started-reply.json");
        f1.open(QIODevice::ReadOnly);
        jobStartedReply = QString("{ \"id\": \"%1\"}").arg(jobId).toUtf8();
//        jobStartedReply = f1.readAll();

        QFile f2(":/demo/D_292_100K_Features.json");
        f2.open(QIODevice::ReadOnly);
        jobResultReply = f2.readAll();

        QFile successFile(":/api/feature-extraction-success-log.txt");
        successFile.open(QIODevice::ReadOnly);
        jobSuccessLog = successFile.readAll();

        QFile failureFile(":/api/feature-extraction-failure-log.txt");
        failureFile.open(QIODevice::ReadOnly);
        jobFailureLog = failureFile.readAll();
    }

    virtual ~JobManagerTests() {
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

    GravitateJobManager manager;
    MockBackendProvider backend;

    QByteArray jobStartedReply;
    QByteArray jobResultReply;
    QByteArray jobSuccessLog;
    QByteArray jobFailureLog;

    AlgorithmId algoId = AlgorithmId::fromString("585bdf79a7b11b0007f7b71c");
    JobId jobId = JobId::fromString("585bdff5a7b11b0007f7b724");
    QByteArray defaultPayload = R"({
                                "intParam" = 10,
                                "doubleParam" = 4.0,
                                "stringParam" = "A string"
                            })";
};

TEST_F(JobManagerTests, ParseJobId)
{
    JobId testJobId;

    ASSERT_TRUE(parseJobId(jobStartedReply, testJobId));

    ASSERT_EQ(testJobId, jobId);

}

TEST_F(JobManagerTests, TestStartJobReturnId) {

    // Arrange
    EXPECT_CALL(backend, apiPostReply( GravitateBackendConf::SchedulerJobsEndpoint, _ ) )
        .WillRepeatedly(Return(jobStartedReply));

    auto payload = defaultPayload;

    // Act
    auto testJobId = manager.startJob(algoId, payload);

    // Assert
    ASSERT_EQ(testJobId, jobId);
}

TEST_F(JobManagerTests, TestJobFinishedReply) {

    EXPECT_CALL(backend, apiPostReply( GravitateBackendConf::SchedulerJobsEndpoint, _ ) )
        .WillRepeatedly(Return(jobStartedReply));

    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::JobLog(jobId), _ ) )
        .WillRepeatedly(Return(jobSuccessLog));

    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::JobsResults(jobId), _ ) )
        .WillRepeatedly(Return(jobResultReply));

    QSignalSpy spy(&manager, &JobManagerBase::jobFinished);

    manager.startJob(algoId, defaultPayload);

    ASSERT_TRUE(spy.wait(2000));



//    QByteArray data;
//    ASSERT_TRUE(manager.getJobResults(jobId, data));
}

//TEST_F(JobManagerTests, DISABLED_TestJobResultNotFound) {

//    backend.statusCode = 404;

//    EXPECT_CALL(backend, apiGetReply( GravitateBackendConf::JobsResults(jobId), _ ) )
//        .WillRepeatedly(Return(""));

//    QByteArray data;
//    ASSERT_FALSE(manager.getJobResults(jobId, data));
//}
