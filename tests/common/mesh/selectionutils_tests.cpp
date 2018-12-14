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
#include "annotation/selectionutils.h"
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>
#include <random>

class TestSelectionUtils : public ::testing::Test
{
public:
    TestSelectionUtils()
    {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> dist(0, 4096);

        for(int i = 0; i < 1000; i++)
        {
            values.push_back(dist(generator));
        }
    }

protected:
    std::vector<unsigned int> values;

    void verify(const std::vector<unsigned int> &values, vtkIdTypeArray *array)
    {
        ASSERT_EQ(values.size(), array->GetNumberOfValues());

        for(int i = 0; i < array->GetNumberOfValues(); i++)
        {
            ASSERT_EQ(values[i], array->GetValue(i));
        }
    }

    void verify(const std::vector<unsigned int> &values, vtkIdList *list)
    {
        ASSERT_EQ(values.size(), list->GetNumberOfIds());

        for(int i = 0; i < list->GetNumberOfIds(); i++)
        {
            ASSERT_EQ(values[i], list->GetId(i));
        }
    }
};

TEST_F(TestSelectionUtils, TestGetIdTypeArray)
{
    auto array = SelectionUtils::toIdTypeArray(values);

    verify(values, array);
}

TEST_F(TestSelectionUtils, TestGetIdList)
{
    auto list = SelectionUtils::toIdList(values);

    verify(values, list);
}
