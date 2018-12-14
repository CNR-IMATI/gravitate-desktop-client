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
#include <QTemporaryFile>

#include <fstream>
#include "boundingbox.h"

/**
 * @brief Automatically generated Google Test skeleton
 */
class BoundingBoxTests : public ::testing::Test
{
protected:

    QTemporaryFile *file = nullptr;
    std::string fileName;

    BoundingBox bb;

    BoundingBoxTests() {
        file = QTemporaryFile::createNativeFile(":/data/sample-mbb.json");
        fileName = file->fileName().toStdString();
    }
};

TEST_F(BoundingBoxTests, TestReadFile) {

    ASSERT_TRUE(bb.readJson(fileName));
}

TEST_F(BoundingBoxTests, TestVerticesOK) {
    std::vector<double> vertices = {
        6.103424,-11.450842,12.192950,
        6.507577,-15.306635,4.502751,
        4.369658,0.678966,6.020064,
        4.773811,-3.176826,-1.670135,
        -1.680121,-12.503063,12.311465,
        -1.275969,-16.358856,4.621266,
        -3.413887,-0.373257,6.138580,
        -3.009734,-4.229049,-1.551619
    };

    ASSERT_TRUE(bb.readJson(fileName));
    ASSERT_EQ(bb.vertices, vertices);
}

TEST_F(BoundingBoxTests, TestLengthOK)
{
    std::vector<double> length = {8.612180,13.720157,7.855240};

    ASSERT_TRUE(bb.readJson(fileName));
    ASSERT_EQ(bb.length, length);
}

TEST_F(BoundingBoxTests, TestDiamOK)
{
    double diam = 18.003254;

    ASSERT_TRUE(bb.readJson(fileName.c_str()));
    ASSERT_DOUBLE_EQ(bb.diam, diam);
}

