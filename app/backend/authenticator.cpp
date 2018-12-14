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
#include "authenticator.h"

BasicHttpAuth::BasicHttpAuth(const QString &user, const QString &password)
    : m_user(user), m_password(password)
{}

QNetworkRequest BasicHttpAuth::addAuthenticationHeader(QNetworkRequest req) const
{
    QString userpassString = QString("%1:%2").arg(user()).arg(password());
    req.setRawHeader("Authorization", "Basic " + userpassString.toUtf8().toBase64());
    return req;
}

void BasicHttpAuth::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *auth)
{
    if(credentialsAlreadyUsed(this->m_user, this->m_password))
    {
        reply->abort();
    }

    auth->setUser(this->m_user);
    auth->setPassword(this->m_password);
}

bool BasicHttpAuth::credentialsAlreadyUsed(const QString &user, const QString &password)
{
    if(user == this->m_latestUser && password == this->m_latestPassword)
    {
        return true;
    }

    this->m_latestUser = user;
    this->m_latestPassword = password;
    return false;
}

void Authenticator::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *auth)
{

}
