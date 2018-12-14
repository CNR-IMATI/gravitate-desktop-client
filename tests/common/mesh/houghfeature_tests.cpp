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
#include "houghfeature.h"

#include <QDebug>
#include <QTemporaryFile>

#include <iostream>
#include <fstream>

/**
 * @brief This class tests the Hough Feature deserialization
 */
class HoughFeatureTests : public ::testing::Test
{
protected:
    void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        sampleHoughFeaturesFile = QTemporaryFile::createNativeFile(":/data/sample-features.json");

        auto fileName = sampleHoughFeaturesFile->fileName().toStdString();

        HoughFeatureReader reader;

        features = reader.readJson(fileName);

        f0_out = {
            {2.033, 1.3309368303333582},
            {
                {-0.71997022537657651, -19.906174263034309, 3.522278210579282},
                {0.28870509003616351, -17.885740425114168, 1.7737985839061914}
            },
            {
                {0.0323404529789898, -19.216126465280954, 2.4211233932166674},
                {-1.053657362294768, -17.813945835959583, 3.4148976271232141}
            }
            // NOTE: vertices vector is not initialized
        };
    }

    void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
        sampleHoughFeaturesFile->close();
        delete sampleHoughFeaturesFile;
    }

    std::string houghStdFileName()
    {
        return sampleHoughFeaturesFile->fileName().toStdString();
    }

    QTemporaryFile *sampleHoughFeaturesFile;
    std::vector<HoughFeature> features;

    HoughFeature f0_out;
};

TEST_F(HoughFeatureTests, TestBasicRead) {

    // Test
    ASSERT_EQ(features.size(), 5);
}

TEST_F(HoughFeatureTests, TestParam)
{
    // Test
    auto f = features[0];
    EXPECT_DOUBLE_EQ(f.params[0], f0_out.params[0]);
    EXPECT_DOUBLE_EQ(f.params[1], f0_out.params[1]);
}

TEST_F(HoughFeatureTests, TestAxes)
{
    auto f = features[0];


    for(int i = 0; i < 3; i++)
    {
        SCOPED_TRACE(i);

        EXPECT_DOUBLE_EQ(f.axis1.p1[i], f0_out.axis1.p1[i]);
        EXPECT_DOUBLE_EQ(f.axis1.p2[i], f0_out.axis1.p2[i]);
        EXPECT_DOUBLE_EQ(f.axis2.p1[i], f0_out.axis2.p1[i]);
        EXPECT_DOUBLE_EQ(f.axis2.p2[i], f0_out.axis2.p2[i]);
    }
}


