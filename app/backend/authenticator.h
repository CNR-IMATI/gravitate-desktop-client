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
#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>

/**
 * @brief The Authenticator class is an abstract class which handles the user authentication
 * when performing a HTTP request.
 *
 * The implementation of the addAuthenticationHeader member function depends on the authentication
 * method used, such as HTTP Basic or a token-based authentication
 */
class Authenticator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Adds an authentication header to the QNetworkRequest object
     * @param req
     */
    virtual QNetworkRequest addAuthenticationHeader(QNetworkRequest req) const = 0;
    virtual ~Authenticator() {}

public slots:
    virtual void onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *auth);

};

class BasicHttpAuth : public Authenticator
{
    Q_OBJECT

    Q_PROPERTY(QString user READ user)
    Q_PROPERTY(QString password READ password)

public:
    BasicHttpAuth(const QString &user, const QString &password);

    QString user() const { return m_user; }
    QString password() const { return m_password; }

    QNetworkRequest addAuthenticationHeader(QNetworkRequest req) const override;

public slots:
    void onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *auth) override;

private:
    bool credentialsAlreadyUsed(const QString& user, const QString& password);

    QString m_user;
    QString m_password;

    QString m_latestUser;
    QString m_latestPassword;
};


#endif // AUTHENTICATOR_H
