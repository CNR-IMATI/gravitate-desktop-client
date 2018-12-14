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
#ifndef MOCKBACKENDPROVIDER_H
#define MOCKBACKENDPROVIDER_H

#include <gmock/gmock.h>
#include "backendprovider.h"
#include "qstringprint.h"
#include "qfakenetworkreply.h"

class MockBackendProvider : public BackendProvider
{
    Q_OBJECT
public:
    MockBackendProvider()
    {
        qRegisterMetaType<QFakeNetworkReply*>();
    }

    QNetworkReply *apiGet(const QString &route, const QUrlQuery &query) override
    {
        return createReply(apiGetReply(route, query));
    }

    QNetworkReply *apiPost(const QString &route,
                           const QUrlQuery &query,
                           const QByteArray &data,
                           const QString &contentType) override
    {
        return createReply(apiPostReply(route, query));
    }

    QNetworkReply *apiPostJson(const QJsonDocument &doc,
                               const QString &route,
                               const QUrlQuery &query) override
    {
        return createReply(apiPostJsonReply(doc, route, query));
    }


    QNetworkReply *apiDelete(const QString &route, const QUrlQuery &query) override
    {
        return createReply(apiDeleteReply(route, query));
    }

    QNetworkReply *sparqlQuery(const QString &queryString) override
    {
        return createReply(sparqlQueryReply(queryString));
    }

    MOCK_METHOD2(apiGetReply, QByteArray (const QString &route, const QUrlQuery &query));
    MOCK_METHOD2(apiPostReply, QByteArray (const QString &route, const QUrlQuery &query));
    MOCK_METHOD3(apiPostJsonReply, QByteArray (const QJsonDocument &doc, const QString &route, const QUrlQuery &query));
    MOCK_METHOD2(apiDeleteReply, QByteArray (const QString &route, const QUrlQuery &query));
    MOCK_METHOD1(sparqlQueryReply, QByteArray (const QString &queryString));

    int statusCode = 200;
    double replyDelay = 0;

signals:
    void replyFinished(QFakeNetworkReply *reply);

private slots:
    void onReplyFinished()
    {
        auto reply = static_cast<QFakeNetworkReply*>(sender());
        emit replyFinished(reply);
    }

private:
    QNetworkReply *createReply(const QByteArray &content)
    {
        auto reply = QFakeNetworkReply::fromData(content)->finish(replyDelay);
        reply->setHttpStatusCode(statusCode);
        connect(reply, &QNetworkReply::finished,
                this, &MockBackendProvider::onReplyFinished);
        //        connect(reply, &QNetworkReply::finished,
        //                this, [this, reply] {
        //            emit replyFinished(reply);
        //        });

        // This is to ensure that the QFakeNetworkReply is destroyed, but not
        // before the test has finished
        connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
        m_fakeReplies.append(reply);

        return reply;
    }

    QList<QFakeNetworkReply*> m_fakeReplies;

};

#endif // MOCKBACKENDPROVIDER_H
