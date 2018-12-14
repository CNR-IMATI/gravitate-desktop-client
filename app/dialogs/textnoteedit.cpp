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
#include "textnoteedit.h"
#include "ui_textnoteedit.h"
#include <QPushButton>

TextNoteEdit::TextNoteEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextNoteEdit)
{
    ui->setupUi(this);

    auto okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    connect(ui->titleEdit, &QLineEdit::textChanged,
            [this, okButton] {
        auto empty = ui->titleEdit->text().isEmpty();
        okButton->setEnabled(!empty);
    });
}

TextNoteEdit::~TextNoteEdit()
{
    delete ui;
}

void TextNoteEdit::setGroupName(const QString &name)
{
    ui->groupNameLabel->setText(name);
}

void TextNoteEdit::setTitle(const QString &title)
{
    ui->titleEdit->setText(title);
}

void TextNoteEdit::setCurrentText(const QString &text)
{
    ui->textEdit->setText(text);
}

QString TextNoteEdit::groupName()
{
    return ui->groupNameLabel->text();
}

QString TextNoteEdit::title()
{
    return ui->titleEdit->text();
}

QString TextNoteEdit::text()
{
    return ui->textEdit->toPlainText();
}
