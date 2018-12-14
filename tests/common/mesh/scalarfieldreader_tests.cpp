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
#include "scalarfieldreader.h"
#include "math_utils.h"
#include <QFile>
#include <QTemporaryFile>

class ScalarFieldTest : public ::testing::Test
{
protected:
    void SetUp()
    {
        sampleCurvatureFile = QTemporaryFile::createNativeFile("://data/sample_curvature.txt");
        sampleCielabFile = QTemporaryFile::createNativeFile("://data/sample_cielab.txt");
    }

    void TearDown()
    {
        sampleCurvatureFile->close();
        delete sampleCurvatureFile;

        sampleCielabFile->close();
        delete sampleCielabFile;
    }

    std::string curvatureFileName()
    {
        return sampleCurvatureFile->fileName().toStdString();
    }

    std::string cielabFileName()
    {
        return sampleCielabFile->fileName().toStdString();
    }

    QTemporaryFile* sampleCurvatureFile;
    QTemporaryFile* sampleCielabFile;
};

TEST_F(ScalarFieldTest, TestOpenCurvatureFile)
{
    ScalarFieldReader reader(1);
    ASSERT_TRUE(reader.readTxt(curvatureFileName()));

}

TEST_F(ScalarFieldTest, TestOpenCielabFile)
{
    // Arrange
    // Act
    ScalarFieldReader reader(3);

    // Assert
    ASSERT_TRUE(reader.readTxt(cielabFileName()));
}


TEST_F(ScalarFieldTest, TestCurvatureReadOK)
{
    // Arrange
    ScalarFieldReader reader(1);

    // Act
    reader.readTxt(curvatureFileName());

    // Assert
    ASSERT_EQ(reader.getColumn(0).size(), 1000);
}

TEST_F(ScalarFieldTest, TestCielabReadOK)
{
    // Arrange
    ScalarFieldReader reader(3);

    // Act
    reader.readTxt(cielabFileName());

    // Assert
    ASSERT_EQ(reader.getColumn(0).size(), 1000);
    ASSERT_EQ(reader.getColumn(1).size(), 1000);
    ASSERT_EQ(reader.getColumn(2).size(), 1000);
}
