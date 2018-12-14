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

#include "facet.h"

class FacetTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        // Arrange
        facetFile = QTemporaryFile::createNativeFile("://data/sample-facet.fct");
        ASSERT_TRUE(reader.readFct(facetFileName()));
    }

    void TearDown()
    {
        facetFile->close();
        delete facetFile;
    }

    std::string facetFileName()
    {
        return facetFile->fileName().toStdString();
    }

    QTemporaryFile *facetFile;
    FacetReader reader;
};

TEST_F(FacetTest, FacetFileOK)
{
    // Arrange
    // Act
    // Assert
}

TEST_F(FacetTest, FacetNumOK)
{
    // Arrange
    // Act
    std::vector<Facet> facets = reader.getFacets();
    // Assert
    ASSERT_EQ(facets.size(), 5);

}

TEST_F(FacetTest, FacetTypeOK)
{
    // Arrange
    // Act
    std::vector<Facet> facets = reader.getFacets();

    // Assert
    ASSERT_EQ(facets[0].type, Facet::FacetType::EXTERNAL);
    ASSERT_EQ(facets[1].type, Facet::FacetType::INTERNAL);
    ASSERT_EQ(facets[2].type, Facet::FacetType::FRACTURE);
    ASSERT_EQ(facets[3].type, Facet::FacetType::FRACTURE);
    ASSERT_EQ(facets[4].type, Facet::FacetType::FRACTURE);
}
