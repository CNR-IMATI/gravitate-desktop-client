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
#include "gravitateextrametadata.h"
#include "qstringprint.h"
#include "qudt_units.h"

/**
 * @brief Tests for the GravitateExtraMetadataTests class
 */
class GravitateExtraMetadataTests : public ::testing::Test
{
protected:
    GravitateExtraMetadataTests()
    {
        // You can do set-up work for each test here.
        metadata.readCsv(":/data/sample-extra-metadata.csv");
    }

    GravitateExtraMetadata metadata;
};

TEST_F(GravitateExtraMetadataTests, TestInventoryId) {

    auto info = metadata.get(ArtefactId::fromUri((QUrl("http://www.ashmolean.org/id/object/AN1909.840"))));

    ASSERT_EQ(info.inventoryId, QString("09_840"));

    // Arrange
}

TEST_F(GravitateExtraMetadataTests, TestThickness) {

    auto info = metadata.get(ArtefactId::fromUri(QUrl("http://www.ashmolean.org/id/object/AN1909.840")));

    ASSERT_DOUBLE_EQ(info.thickness, 1.3);

    // Arrange
}

TEST_F(GravitateExtraMetadataTests, TestUnit) {

    auto info = metadata.get(ArtefactId::fromUri(QUrl("http://www.ashmolean.org/id/object/AN1909.840")));

    ASSERT_EQ(info.unit, qudt::Unit::CENTIMETER);

    // Arrange
}
