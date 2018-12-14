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
#include "annotationdialog.h"
#include "ui_annotationdialog.h"

AnnotationDialog::AnnotationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnnotationDialog)
{
    ui->setupUi(this);

    m_colorDialog = new QColorDialog();
    m_colorDialog->setOption(QColorDialog::NoButtons, true);
    m_colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);

    m_colorDialog->setVisible(true);

    ui->verticalLayout->insertWidget(0, m_colorDialog);

    //m_colorDialog->show();


}

AnnotationDialog::~AnnotationDialog()
{
    delete ui;
}

QString AnnotationDialog::label() const
{
    return ui->descriptionValue->text();
}

void AnnotationDialog::setLabel(const QString &label)
{
    ui->descriptionValue->setText(label);
}

QString AnnotationDialog::tag() const
{
    return ui->tagValue->text();
}

void AnnotationDialog::setTag(const QString &tag)
{
    ui->tagValue->setText(tag);
}

QColor AnnotationDialog::color() const
{
    return m_colorDialog->currentColor();
}
