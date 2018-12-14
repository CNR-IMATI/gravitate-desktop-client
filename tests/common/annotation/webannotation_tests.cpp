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
#include "webannotation.h"
#include <QTemporaryFile>
#include <fstream>
/**
 * @brief Tests for the WebAnnotation class
 */
class WebAnnotationReadTests : public ::testing::Test
{
protected:
    void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).

        // Read annotation object in JSON-LD format
        temp = QTemporaryFile::createNativeFile(":/annotation/webannotation-sample.jsonld");
        auto fileName = temp->fileName().toStdString();

        WebAnnotationReader reader(fileName);
        anno = reader.readJson();
    }

    QTemporaryFile *temp;
    WebAnnotation anno;
};

TEST_F(WebAnnotationReadTests, ShouldHaveContext) {
    ASSERT_EQ(anno.context, "http://www.w3.org/ns/anno.jsonld");
}

TEST_F(WebAnnotationReadTests, ShouldHaveBodyId) {
    ASSERT_EQ(anno.body.id, "http://gravitate-project.eu/scheme/Eye");
}

TEST_F(WebAnnotationReadTests, ShouldHaveTargetSource) {
    ASSERT_EQ(anno.target.source.id, "https://gravitate-1.cytera.cyi.ac.cy/repository/Cyprus_Museum/D_292_clean.ply");
}

TEST_F(WebAnnotationReadTests, ShouldHaveTargetSelector) {
    ASSERT_EQ(anno.target.selector.id, "https://gravitate-1.cytera.cyi.ac.cy/repository/Cyprus_Museum/D_292_clean/Parts/selector1.json");
}

TEST_F(WebAnnotationReadTests, ShouldHaveTargetTypeSpecificResource) {
    ASSERT_EQ(anno.target.type, "SpecificResource");
}

TEST_F(WebAnnotationReadTests, ShouldHaveSelectorTypeThreeDSurface) {
    ASSERT_EQ(anno.target.selector.type, "ThreeDMeshSurfaceSelector");
}


