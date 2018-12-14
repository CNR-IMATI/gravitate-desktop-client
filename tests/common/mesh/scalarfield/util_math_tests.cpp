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
#include "math_utils.h"
#include <QVector>

struct MadParams
{
    QVector<double> vector;
    double result;
};

class MeanAbsoluteDeviationTest : public ::testing::Test,
                public ::testing::WithParamInterface<MadParams>
{
};

TEST(ClampTest, ResultOK)
{
    ASSERT_DOUBLE_EQ(clamp(5.0, 0.0, 10.0), 5.0);
    ASSERT_DOUBLE_EQ(clamp(-1.0, 0.0, 10.0), 0.0);
    ASSERT_DOUBLE_EQ(clamp(20.0, 0.0, 10.0), 10.0);
}

TEST_P(MeanAbsoluteDeviationTest, ResultOK)
{
    MadParams params = GetParam();
    ASSERT_DOUBLE_EQ(mean_absolute_deviation(params.vector), params.result);
}

INSTANTIATE_TEST_CASE_P(MathTests, MeanAbsoluteDeviationTest, ::testing::Values(
                            MadParams{{0.0, 0.0, 0.0, 0.0, 0.0}, 0.0},
                            MadParams{{1.0, 1.0, 1.0, 1.0, 1.0}, 0.0},
                            MadParams{{0.0, 1.0, 2.0, 3.0, 4.0}, 1.2},
                            MadParams{{1.0, 2.0, 3.0, 4.0, 5.0}, 1.2},
                            MadParams{{-2.0,-1.0, 0.0, 1.0, 2.0}, 1.2},
                            MadParams{{2.0, 2.0, 3.0, 4.0, 14.0}, 3.6}
                        ));
