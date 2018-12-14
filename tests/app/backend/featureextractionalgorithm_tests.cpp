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
#include "featureextractionalgorithm.h"
#include "mockjobmanager.h"
#include "gravitatebackendconf.h"
#include <QSignalSpy>
#include "qfakenetworkreply.h"
#include "gravitatebackendconf.h"
#include <QMetaType>
#include <QTestEventLoop>
#include <QFile>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;

/**
 * @brief Automatically generated Google Test skeleton
 */
class FeatureExtractionAlgorithmTests : public ::testing::Test
{
protected:
    FeatureExtractionAlgorithmTests()
        : algo(&jobManager)
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

        m_jobId = "1234";

        QFile f(":/demo/D_292_100K_Features.json");
        f.open(QIODevice::ReadOnly);
        m_result = f.readAll();
        //m_algoId = "featureExtraction";

    }

    virtual ~FeatureExtractionAlgorithmTests() {
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

    void onJobStartInvoked()
    {
        emit jobManager.jobStarted(m_jobId);
    }

    MockJobManager jobManager;
    FeatureExtractionParams params;
    FeatureExtractionAlgorithm algo;

    JobId m_jobId;
    QByteArray m_result;
};

TEST_F(FeatureExtractionAlgorithmTests, TestFormatParams)
{
    auto bytes = formatParams(params);
    auto expected = QByteArray(
        "https://example.org/D_292_100K.ply "
        "https://example.org/D_292_100K.fct "
        "GP 10 55 30 0.5 true 5");

    ASSERT_EQ(bytes, expected);
}

TEST_F(FeatureExtractionAlgorithmTests, OnRunVerifyJobStarted)
{
    EXPECT_CALL(jobManager, startJob(_,_)).WillOnce(Return(m_jobId));

    // Act
    bool result = algo.run(params);

    // Assert
    ASSERT_TRUE(result);
}

TEST_F(FeatureExtractionAlgorithmTests, OnJobFinishedEmitFinishedSignal)
{
    // Arrange
    QSignalSpy spy(&algo, &FeatureExtractionAlgorithm::finished);
    ASSERT_TRUE(spy.isValid());

    EXPECT_CALL(jobManager, startJob(_,_)).WillOnce(Return(m_jobId));

    algo.run(params);

    // Act
    //
    // We want to test that the FeatureExtractionAlgorithm::finished
    // is emitted as a result of a finished job
    emit jobManager.jobFinished(m_jobId, m_result);

    // Assert
    ASSERT_EQ(spy.count(), 1);
}

TEST_F(FeatureExtractionAlgorithmTests, OnJobSuccededFinishPositive)
{
    // Arrange
    QSignalSpy spy(&algo, &FeatureExtractionAlgorithm::finished);
    ASSERT_TRUE(spy.isValid());

    EXPECT_CALL(jobManager, startJob(_,_)).WillOnce(Return(m_jobId));

    algo.run(params);

    // Act
    //
    // We want to test that the FeatureExtractionAlgorithm::finished
    // is emitted as a result of a finished job
    emit jobManager.jobFinished(m_jobId, m_result);

    // Assert
    ASSERT_EQ(spy.count(), 1);
    auto arguments = spy.takeFirst();
    bool success = arguments.value(0).toBool();

    EXPECT_TRUE(success);
}


TEST_F(FeatureExtractionAlgorithmTests, OnJobFailedFinishNegative)
{
    // Arrange
    QSignalSpy spy(&algo, &FeatureExtractionAlgorithm::finished);
    ASSERT_TRUE(spy.isValid());

    EXPECT_CALL(jobManager, startJob(_,_)).WillOnce(Return(m_jobId));

    algo.run(params);

    // Act
    //
    // We want to test that the FeatureExtractionAlgorithm::finished
    // is emitted as a result of a finished job
    emit jobManager.jobFailed(m_jobId);

    // Assert
    ASSERT_EQ(spy.count(), 1);
    auto arguments = spy.takeFirst();
    bool success = arguments.value(0).toBool();

    EXPECT_FALSE(success);
}


