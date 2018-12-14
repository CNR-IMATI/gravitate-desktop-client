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

#include <QTemporaryFile>
#include "reassemblyreader.h"

/**
 * @brief Automatically generated Google Test skeleton
 */
class ReAssemblyTests : public ::testing::Test
{
protected:
    void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        tempFile = QTemporaryFile::createNativeFile(":/data/sample-reassembly.json");
        tempFileName = tempFile->fileName();

        ASSERT_TRUE(reader.readJson(tempFileName.toStdString()));
    }

    void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
        tempFile->close();
        delete tempFile;
    }

    QTemporaryFile *tempFile;
    QString tempFileName;

    ReAssemblyReader reader;
};

TEST_F(ReAssemblyTests, GoodFileHasPairs) {
    // Act
    auto pairs = reader.pairs();

    // Assert
    EXPECT_EQ(pairs.size(), 16);
}

TEST_F(ReAssemblyTests, FileNamesOk) {

    // Arrange
    auto pairs = reader.pairs();
    ASSERT_GT(pairs.size(), 0);

    // Act
    auto p0 = pairs.at(0);

    // ASSERT
    EXPECT_EQ(p0.first.fileName, "Jar01_1.ply");
    EXPECT_EQ(p0.second.fileName, "Jar01_9.ply");
}

TEST_F(ReAssemblyTests, RotationMatrixOK) {

    // Arrange
    auto pairs = reader.pairs();
    ASSERT_GT(pairs.size(), 0);

    // Act
    auto p0 = pairs.at(0);

    auto R = p0.second.rotation;
    double testArray[] = {
        -0.3607326783,0.5244136235,-0.7712731593,
        -0.9316046502,-0.1630978935,0.3248258808,
        0.0445500895,0.8356969718,0.5473809102
    };

    for(int i = 0; i < 9; i++)
    {
        SCOPED_TRACE(i);
        EXPECT_DOUBLE_EQ(R[i], testArray[i]);
    }
}

TEST_F(ReAssemblyTests, TranslationVectorOk) {

    // Arrange
    auto pairs = reader.pairs();
    ASSERT_GT(pairs.size(), 0);

    // Act
    auto p0 = pairs.at(0);

    auto T = p0.second.translation;
    double testArray[] = {
        7.11932313,45.21320332,903.3555274
    };

    for(int i = 0; i < 3; i++)
    {
        SCOPED_TRACE(i);
        EXPECT_DOUBLE_EQ(T[i], testArray[i]);
    }
}





