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
#ifndef CONCRETEBACKENDPROVIDER_H
#define CONCRETEBACKENDPROVIDER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QByteArray>
#include "backendprovider.h"
#include "authenticator.h"


class NetworkBackendProvider : public BackendProvider
{
    Q_OBJECT

    //Q_PROPERTY(QSharedPointer<Authenticator> authenticator READ authenticator WRITE setAuthenticator)

public:
    explicit NetworkBackendProvider(QNetworkAccessManager *manager, QObject *parent = 0);

    void setAuthenticator(Authenticator * const auth);

    Authenticator *authenticator() const;

    bool hasAuthenticator() const;

    const QString &hostname() const;
    void setHostname(QString hostname);

    QNetworkReply *apiGet(const QString &route, const QUrlQuery &query = QUrlQuery()) override;

    QNetworkReply *apiPost(const QString &route,
                           const QUrlQuery &query = QUrlQuery(),
                           const QByteArray &data = QByteArray(),
                           const QString &contentTypeHeader = "application/x-www-form-urlencoded") override;

    QNetworkReply *apiDelete(const QString &route, const QUrlQuery &query = QUrlQuery()) override;

    QNetworkReply *sparqlQuery(const QString &queryString) override;

    QNetworkReply *apiPostJson(const QJsonDocument &doc, const QString &route, const QUrlQuery &query) override;


signals:
    void networkReplyReceived(const QByteArray &data);

protected:
    QNetworkAccessManager *manager_;

private:
    QNetworkRequest createHttpRequest(const QString &route, const QUrlQuery &query = QUrlQuery()) const;

    QNetworkRequest createSparqlRequest() const;

    QNetworkRequest createContainerListingRequest();

    QNetworkRequest createContainerUpdateRequest();


    QUrl endpoint(const QString &route) const;

    QString hostname_;

    Authenticator *m_auth;

};

#endif // CONCRETEBACKENDPROVIDER_H
