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
#include "repofix.h"
#include <QString>

struct QudtParams
{
    QString in;
    QString out;
};

/**
 * @brief Automatically generated Google Test skeleton
 */
class FixQudtTests : public ::testing::Test,
    public ::testing::WithParamInterface<QudtParams>
{
};

TEST_P(FixQudtTests, TestResult) {
    auto params = GetParam();

    auto res = RepoFix::fixQudtUrl(params.in);

    ASSERT_EQ(res, params.out);
}

INSTANTIATE_TEST_CASE_P(RepoFix, FixQudtTests, ::testing::Values(
    QudtParams{ "", ""},
    QudtParams{ "invalidUrl", "invalidUrl"},
    QudtParams{ "http://qudt.org/vocab/unit#", "http://qudt.org/vocab/unit#"},
    QudtParams{ "http://qudt.org/vocab/unit#m", "http://qudt.org/vocab/unit/M"},
    QudtParams{ "http://qudt.org/vocab/unit#cm2", "http://qudt.org/vocab/unit/CM2"}
));
