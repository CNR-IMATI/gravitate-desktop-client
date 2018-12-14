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

#include "dialogs/parameterselectiondialog.h"

/**
 * @brief Automatically generated Google Test skeleton
 */
class ParameterDialogTests : public ::testing::Test
{
protected:
    ParameterDialogTests() {
        // You can do set-up work for each test here.
    }

    virtual ~ParameterDialogTests() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    static void SetUpTestCase() {
        // Called before the first test in this test case.
    }

    static void TearDownTestCase() {
        // Called after the last test in this test case.
    }

    ParameterSelectionDialog dialog;
};
TEST_F(ParameterDialogTests, TestIntParameterNonExistentReturnQVariant) {

    ASSERT_EQ(dialog.value("non_existent_parameter"), QVariant());
}


TEST_F(ParameterDialogTests, TestIntParameterValueOK) {

    dialog.addIntParameter("param", "Int Parameter", 0, 10);
    dialog.setValue("param", 5);

    QVariant value = dialog.value("param");

    ASSERT_EQ(value.toInt(), 5);
}

TEST_F(ParameterDialogTests, TestDoubleParameterValueOK) {

    dialog.addDoubleParameter("param", "Double Parameter", -10.0, 100.0, 0.1);
    dialog.setValue("param", 25.0);

    QVariant value = dialog.value("param");

    ASSERT_EQ(value.toDouble(), 25.0);
}

TEST_F(ParameterDialogTests, TestBoolParameterValueOK) {

    dialog.addBoolParameter("param", "Bool Parameter");
    dialog.setValue("param", true);

    QVariant value = dialog.value("param");

    ASSERT_EQ(value.toBool(), true);
}

TEST_F(ParameterDialogTests, TestListParameterValueOK) {

    QStringList values;
    values << "Item 1" << "Item 2" << "Item 3";

    dialog.addListParameter("param", "Parameter", values);
    dialog.setValue("param", "Item 2");

    QVariant value = dialog.value("param");

    ASSERT_EQ(value.toString(), "Item 2");
}


