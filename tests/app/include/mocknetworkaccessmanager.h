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
#ifndef MOCKNETWORKACCESSMANAGER_H
#define MOCKNETWORKACCESSMANAGER_H

#include <gmock/gmock.h>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QIODevice>
#include "qfakenetworkreply.h"
#include "qstringprint.h"

using ::testing::_;
using ::testing::Return;

class MockNetworkAccessManager
        : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit MockNetworkAccessManager(QObject *parent = 0)
        : QNetworkAccessManager(parent)
    {
        ON_CALL(*this, createRequest(_, _, _))
            .WillByDefault(Return( QFakeNetworkReply::create("")->finish() ));
    }

    MOCK_METHOD3(createRequest, QNetworkReply* (Operation op, const QNetworkRequest &request, QIODevice *outgoingData));
    MOCK_METHOD1(get, QNetworkReply* (const QNetworkRequest &request));
};

#endif // MOCKNETWORKACCESSMANAGER_H
