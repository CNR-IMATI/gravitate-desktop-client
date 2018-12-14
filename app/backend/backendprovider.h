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
#ifndef BACKENDPROVIDER_H
#define BACKENDPROVIDER_H

/**
 * @brief The BackendProvider class is an abstract class that represents a
 * generic provider for the backend.
 *
 */

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QByteArray>
#include "sparqlresults.h"

class BackendProvider : public QObject
{
    Q_OBJECT

public:
    virtual ~BackendProvider() {}

    virtual QNetworkReply *apiGet(const QString &route, const QUrlQuery &query = QUrlQuery()) = 0;
    virtual QNetworkReply *apiPost(const QString &route,
                                   const QUrlQuery &query = QUrlQuery(),
                                   const QByteArray &data = QByteArray(),
                                   const QString &contentTypeHeader = "application/x-www-form-urlencoded") = 0;

    virtual QNetworkReply *apiPostJson(const QJsonDocument &doc,
                                       const QString &route,
                                       const QUrlQuery &query = QUrlQuery()) = 0;

    virtual QNetworkReply *apiDelete(const QString &route, const QUrlQuery &query = QUrlQuery()) = 0;
    virtual QNetworkReply *sparqlQuery(const QString &queryString) = 0;

protected:
    explicit BackendProvider(QObject *parent = nullptr) :
        QObject(parent) {}

};

#endif // BACKENDPROVIDER_H
