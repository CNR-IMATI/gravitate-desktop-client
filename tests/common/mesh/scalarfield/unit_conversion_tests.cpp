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
#include "math_utils.h"
#include "qudt_units.h"
#include "unitofmeasure.h"


struct UnitOfMeasureParams
{
    qudt::Unit source;
    qudt::Unit target;
    double value;
    double result;
};

/**
 * @brief Automatically generated Google Test skeleton
 */
class UnitConversionTest : public ::testing::Test,
    public ::testing::WithParamInterface<UnitOfMeasureParams>
{
};

TEST_P(UnitConversionTest, ResultOK)
{
    auto param = GetParam();

    bool ok;
    double result = convertUnit(param.value, param.source, param.target, &ok);

    ASSERT_TRUE(ok);
    ASSERT_DOUBLE_EQ(result, param.result);
}

INSTANTIATE_TEST_CASE_P(UnitConversion, UnitConversionTest, ::testing::Values(
    UnitOfMeasureParams{ qudt::METER, qudt::METER, 1.0, 1.0 },
    UnitOfMeasureParams{ qudt::METER, qudt::CENTIMETER, 1.0, 100.0 },
    UnitOfMeasureParams{ qudt::CENTIMETER, qudt::METER, 1.0, 0.01 },
    UnitOfMeasureParams{ qudt::MILLIMETER, qudt::CENTIMETER, 1.0, 0.1 }
));
