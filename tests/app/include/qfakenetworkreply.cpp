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
#include "qfakenetworkreply.h"
#include <QTimer>
#include <QtGlobal>
#include <QFile>
#include <QDebug>

struct QFakeNetworkReplyPrivate
{
    QByteArray data;
    int offset;
};

QFakeNetworkReply::QFakeNetworkReply()
{
    d = new QFakeNetworkReplyPrivate;
}

QFakeNetworkReply::~QFakeNetworkReply()
{
    delete d;
}

QFakeNetworkReply *QFakeNetworkReply::QFakeNetworkReply::fromResource(const QString &resource)
{
    QFakeNetworkReply *reply = new QFakeNetworkReply();
    QFile f(resource);
    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() << tr("Resource '%1' not found").arg(resource);
        return nullptr;
    }

    reply->setContent(f.readAll());

    return reply;
}

QFakeNetworkReply *QFakeNetworkReply::create(const QString &replyContent)
{
    return QFakeNetworkReply::fromData(replyContent.toUtf8());
}

QFakeNetworkReply *QFakeNetworkReply::fromData(const QByteArray &content)
{
    QFakeNetworkReply *reply = new QFakeNetworkReply();
    reply->setContent(content);
    return reply;
}

void QFakeNetworkReply::setHttpStatusCode(int code, const QByteArray &statusText)
{
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, code );
    if( statusText.isNull() )
        return;
    setAttribute(QNetworkRequest::HttpReasonPhraseAttribute, statusText );

}

void QFakeNetworkReply::setContentType(const QByteArray &contentType)
{
    setHeader(QNetworkRequest::ContentTypeHeader, contentType);
}

void QFakeNetworkReply::setContent(const QString &content)
{
    setContent(content.toUtf8());
}

void QFakeNetworkReply::setContent(const QByteArray &content)
{
    d->data = content;
    d->offset = 0;

    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));
}

qint64 QFakeNetworkReply::bytesAvailable() const
{
    return d->data.size() - d->offset;
}

bool QFakeNetworkReply::isSequential() const
{
    return true;
}

QFakeNetworkReply *QFakeNetworkReply::finish(int msec)
{
    QTimer::singleShot( msec, this, &QNetworkReply::readyRead);
    QTimer::singleShot( msec, this, &QNetworkReply::finished);

    return this;
}

void QFakeNetworkReply::abort()
{
    // Do nothing
}

qint64 QFakeNetworkReply::readData(char *data, qint64 maxlen)
{
    if (d->offset >= d->data.size())
        return -1;


    qint64 number = qMin(maxlen, bytesAvailable());
    memcpy(data, d->data.constData() + d->offset, number);
    d->offset += number;

    return number;

}
