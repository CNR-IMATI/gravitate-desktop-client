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
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QString user();
    QString password();
    QString hostname();

    /**
     * @brief requestTimeout returns the timeout setting for the request
     * @return timeout setting (in milliseconds)
     */
    int requestTimeout();


signals:
    void error(const QString &errorString);

    void configFileOpenRequested();

public slots:
    virtual void accept();

    void setUser(const QString &user);
    void setPassword(const QString &password);
    void setHostname(const QString &hostname);
    void setRequestTimeout(int milliseconds);

private slots:
    void setLoadingState(bool loading);

    void validateInput();

    void openConfigFile();

private:
    Ui::LoginDialog *ui;

    void initConfigMenu();
    void initValidators();
    bool isLoginSuccessful();
    void setInputEnabled(bool enabled);

    QString m_user;
    QMovie *m_loadingSpinner;
    int m_timeoutMs;
};

#endif // LOGINDIALOG_H
