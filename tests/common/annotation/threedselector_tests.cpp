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
#include "threedselector.h"
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

/**
 * @brief Tests for the ThreeDSelector class
 */
class ThreeDSelectorWriterTests : public ::testing::Test
{
protected:
    ThreeDSelectorWriterTests()
    {
        // You can do set-up work for each test here.


        indicesNoneCompression = {0, 1, 2, 3, 4, 19, 12, 14, 15};
        indicesRleCompression = {0, 5, 12, 1, 14, 2, 19, 1};


        json j;
        j["compression"] = "None";
        j["indices"] = indicesNoneCompression;

        outJson = j.dump();
    }

    std::vector<size_t> indicesNoneCompression;
    std::vector<size_t> indicesRleCompression;
    std::string outJson;
};

TEST_F(ThreeDSelectorWriterTests, CompressionNoneIndices) {

    // Arrange
    ThreeDSurfaceSelector surfaceSelector;

    surfaceSelector.indices = indicesNoneCompression;

    ThreeDSurfaceSelectorWriter writer(surfaceSelector, IndexCompression::NONE);

    std::stringstream ss;
    writer.saveJson(ss);

    ASSERT_EQ(ss.str(), outJson);
}

TEST_F(ThreeDSelectorWriterTests, CompressionNoneAttributeNone) {

//    // Arrange
//    ThreeDSurfaceSelector surfaceSelector;

//    surfaceSelector.indices = indices;

//    ThreeDSurfaceSelectorWriter writer(surfaceSelector, IndexCompression::NONE);

//    std::stringstream ss;
//    writer.saveJson(ss);

//    ASSERT_EQ(ss.str(), outJson);
}


