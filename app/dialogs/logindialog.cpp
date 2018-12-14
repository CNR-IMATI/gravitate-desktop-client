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
#include "logindialog.h"
#include "ui_logindialog.h"
#include <QSettings>

#include <QMovie>
#include <QTimer>
#include <QDialogButtonBox>
#include <QPushButton>
#include "networkbackendprovider.h"
#include "authenticator.h"
#include "gravitatebackendconf.h"
#include <QRegularExpressionValidator>
#include "loginprovider.h"
#include "backendfactorybase.h"
#include <memory>
#include <QEventLoop>
#include <QDesktopServices>
#include <QMenu>

bool LoginDialog::isLoginSuccessful()
{
    setLoadingState(true);

    auto loginProvider = BackendFactoryBase::instance()->createLoginProvider();

    //auto auth = new BasicHttpAuth(user(), password());

    bool ok = loginProvider->tryLoginSync(hostname(), new BasicHttpAuth(user(), password()));

    if(!ok)
    {
        emit error(loginProvider->errorString());
    }

    return ok;
}

void LoginDialog::setInputEnabled(bool enabled)
{
    ui->LoginControlsContainer->setEnabled(enabled);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    initValidators();

    initConfigMenu();

    ui->loadingLabel->sizePolicy().setRetainSizeWhenHidden(true);

    m_loadingSpinner = new QMovie("://images/ajax-loader.gif",
                                  QByteArray(), this);
    connect(this, &LoginDialog::error,
            [] (const QString &errorString){
        qCritical().noquote() << tr("Error: %1").arg(errorString);
    });

    connect(this, &LoginDialog::error,
            ui->loadingLabel, &QLabel::setText);

    setRequestTimeout(5000);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::user()
{
    return ui->usernameEdit->text().trimmed();
}

QString LoginDialog::password()
{
    return ui->passwordEdit->text();
}

QString LoginDialog::hostname()
{
    return ui->hostnameEdit->text().trimmed();
}

int LoginDialog::requestTimeout()
{
    return m_timeoutMs;
}

void LoginDialog::accept()
{
    ui->loadingLabel->setText("");
    setLoadingState(true);
    if(!isLoginSuccessful())
    {
        setLoadingState(false);
        return;
    }
    //QDialog::accept();
    QDialog::accept();
}

void LoginDialog::setUser(const QString &user)
{
    ui->usernameEdit->setText(user.trimmed());
}

void LoginDialog::setPassword(const QString &password)
{
    ui->passwordEdit->setText(password);
}

void LoginDialog::setHostname(const QString &hostname)
{
    ui->hostnameEdit->setText(hostname.trimmed());
}

void LoginDialog::setRequestTimeout(int milliseconds)
{
    m_timeoutMs = milliseconds;
}

void LoginDialog::setLoadingState(bool loading)
{
    setInputEnabled(!loading);

    if(loading)
    {
        ui->loadingLabel->setMovie(m_loadingSpinner);
        m_loadingSpinner->start();
    }
    else
    {
        m_loadingSpinner->stop();
    }
}

void LoginDialog::validateInput()
{
    bool valid = ui->hostnameEdit->hasAcceptableInput() &&
                 ui->usernameEdit->hasAcceptableInput() &&
                 ui->passwordEdit->hasAcceptableInput();

    auto okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(valid);
}

void LoginDialog::openConfigFile()
{
    QSettings settings;

    auto url = QUrl::fromLocalFile(settings.fileName());

    if(url.isEmpty())
    {
        emit error("No location found for the settings file");
        return;
    }

    QDesktopServices::openUrl(url);
}

void LoginDialog::initConfigMenu()
{
    auto menu = new QMenu();
    menu->addAction(tr("Open Configuration File"), this, &LoginDialog::configFileOpenRequested);

    ui->configButton->setMenu(menu);

    connect(this, &LoginDialog::configFileOpenRequested,
            this, &LoginDialog::openConfigFile);
}

void LoginDialog::initValidators()
{
    QRegularExpression hostRegex(R"(^(https?:\/\/)?([\dA-Za-z\.-]+)\.([A-Za-z\.]{2,6})([\/\w \.-]*)*\/?$)");
    ui->hostnameEdit->setValidator( new QRegularExpressionValidator(hostRegex) );
    connect(ui->hostnameEdit, &QLineEdit::textChanged,
            this, &LoginDialog::validateInput );

    QRegularExpression userRegex(R"(.+)");
    ui->usernameEdit->setValidator( new QRegularExpressionValidator(userRegex));
    connect(ui->usernameEdit, &QLineEdit::textChanged,
            this, &LoginDialog::validateInput );

    QRegularExpression passRegex(R"(.+)");
    ui->passwordEdit->setValidator( new QRegularExpressionValidator(passRegex));
    connect(ui->passwordEdit, &QLineEdit::textChanged,
            this, &LoginDialog::validateInput );


}
