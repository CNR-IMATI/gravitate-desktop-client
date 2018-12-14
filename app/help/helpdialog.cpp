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
#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QDir>
#include <QMessageBox>
#include <QRegularExpression>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    QDir appdir(QCoreApplication::applicationDirPath());
    auto collectionFile = appdir.absoluteFilePath("docs/help/desktop-help.qhc");

    ui->helpBrowser->setCollectionFile(collectionFile);

    connect(ui->helpBrowser, &QTextBrowser::sourceChanged,
            [this] {
        auto title = tr("Help: %1").arg(ui->helpBrowser->documentTitle());
        this->setWindowTitle(title);
    });

    initButtons();
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::showHelpForKeyword(const QString &keyword)
{
    if(!ui->helpBrowser->isValid())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("The help browser component is not properly initialized. For further information, "
                                 "read the error messages in the console."));
        return;
    }

    bool ok = ui->helpBrowser->setHelpForKeyword(keyword);
    if(!ok)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Could not find a help page for the keyword \"%1\"").arg(keyword));
        return;
    }

    show();
}

void HelpDialog::showHelpForUrl(const QUrl &url)
{
    QRegularExpression re("help:\\/\\/gravitate-manual\\/([\\w\\-]+)");
    auto match = re.match(url.toString());

    if(!match.hasMatch())
        return;

    auto keyword = match.captured(1);

    showHelpForKeyword(keyword);
}

void HelpDialog::initButtons()
{
    connect(ui->homeButton, &QAbstractButton::clicked,
            ui->helpBrowser, &QTextBrowser::home);

    connect(ui->backwardButton, &QAbstractButton::clicked,
            ui->helpBrowser, &QTextBrowser::backward);
    connect(ui->helpBrowser, &QTextBrowser::backwardAvailable,
            ui->backwardButton, &QAbstractButton::setEnabled);

    connect(ui->forwardButton, &QAbstractButton::clicked,
            ui->helpBrowser, &QTextBrowser::forward);
    connect(ui->helpBrowser, &QTextBrowser::forwardAvailable,
            ui->forwardButton, &QAbstractButton::setEnabled);

}
