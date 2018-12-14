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
#ifndef QFAKENETWORKREPLY_H
#define QFAKENETWORKREPLY_H

#include <QNetworkReply>
#include <QString>
#include "qstringprint.h"

/**
 * @brief The QFakeNetworkReply class
 *
 * Inspired by: https://blogs.kde.org/2010/08/28/implementing-reusable-custom-qnetworkreply
 */

class QFakeNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    QFakeNetworkReply();
    ~QFakeNetworkReply();

    static QFakeNetworkReply *fromResource(const QString &resource);

    static QFakeNetworkReply *create(const QString &replyContent = QString());
    static QFakeNetworkReply *fromData(const QByteArray &content= QByteArray());

    using QNetworkReply::setRequest;
    using QNetworkReply::setHeader; // Changes the scope of the setHeader method to public (was protected)
    void setHttpStatusCode( int code, const QByteArray &statusText = QByteArray() );
    void setContentType( const QByteArray &contentType );

    void setContent( const QString &content );
    void setContent( const QByteArray &content );

    qint64 bytesAvailable() const;
    bool isSequential() const;

    QFakeNetworkReply *finish(int msec = 0);


public slots:
    void abort();

    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen);

    // QNetworkReply interface

private:
    struct QFakeNetworkReplyPrivate *d;
};

#endif // QFAKENETWORKREPLY_H
