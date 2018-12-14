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
#include "gravitateclipgroupsprovider.h"
#include "gravitatebackendconf.h"

GravitateClipGroupsProvider::GravitateClipGroupsProvider(BackendProvider *backend, QObject *parent)
    : AbstractClipGroupsProvider (parent),
      m_backend(backend)
{
}

void GravitateClipGroupsProvider::requestClipboardContent()
{
    QNetworkReply *reply = m_backend->apiGet(GravitateBackendConf::ClipboardEndpoint);
    handleSetReply(reply, false);
}

void GravitateClipGroupsProvider::requestGroupsList()
{
    QNetworkReply *reply = m_backend->apiGet(GravitateBackendConf::GroupsEndpoint);
    handleSetReply(reply, true);
}

void GravitateClipGroupsProvider::requestGroupContent(const GroupId &groupId)
{
    if(groupId.isNull())
    {
        qCritical() << "Cannot load group content: container ID is empty";
        return;
    }


    QUrlQuery q;
    q.addQueryItem("uri", groupId.uri.toString());
    // If groupId is not empty, fetch items from group groupId
    auto reply = m_backend->apiGet(GravitateBackendConf::ContainersEndpoint, q);

    handleSetReply(reply, false, groupId);
}

void GravitateClipGroupsProvider::createGroup(const QString &name)
{
    QUrlQuery query;
    query.addQueryItem("name", name);

    QNetworkReply *reply = m_backend->apiPost(GravitateBackendConf::GroupsEndpoint, query);

    connect(reply, &QNetworkReply::finished,
        [=] {
        if(reply->error() != QNetworkReply::NoError)
        {
            qCritical() << "Error in the creation of the group";
        }
        else
        {
            qDebug() << "Group" << name << "created successfully";
            emit groupCreated(name);
        }
        reply->deleteLater();
    });
}

void GravitateClipGroupsProvider::removeResource(const QUrl &containerId)
{
    QUrlQuery query;
    query.addQueryItem("uri", containerId.toString());

    QNetworkReply *reply = m_backend->apiDelete(GravitateBackendConf::ContainersEndpoint, query);

    connect(reply, &QNetworkReply::finished,
            [this, reply, containerId] {
        if(reply->error() == QNetworkReply::NoError)
        {
            emit resourceRemoved(containerId);
        }
        reply->deleteLater();
    });

}

void GravitateClipGroupsProvider::addToGroup(const ArtefactId &artefactId, const GroupId &groupId)
{
    QUrlQuery query;
    QString endpoint;

    if(groupId.isNull())
    {
        endpoint = GravitateBackendConf::ClipboardEndpoint;
    }
    else
    {
        endpoint = GravitateBackendConf::ContainersEndpoint;
        query.addQueryItem("container_uri", groupId.uri.toString());
    }

    query.addQueryItem("fragment_iri", artefactId.toString());

    QNetworkReply *reply = m_backend->apiPost(endpoint, query);

    connect(reply, &QNetworkReply::finished,
            [=] {
        if(reply->error() == QNetworkReply::NoError)
        {
            emit addedToGroup(artefactId, groupId);
        }
        reply->deleteLater();
    });
}

void GravitateClipGroupsProvider::handleSetReply(QNetworkReply *reply, bool isGroupList, const GroupId &containerId)
{
    Q_ASSERT(reply != nullptr);

    bool isClipboard = containerId.isNull();

    auto errorSignal = QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error);

    connect(reply, errorSignal,
            [=] (QNetworkReply::NetworkError code) {
        qCritical() << reply->errorString();
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::finished,
            [=] () {
        if(reply->error() != QNetworkReply::NoError)
        {
            return;
        }

        QByteArray payload = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(payload);
        auto arr = doc.array();

        QList<ClipboardItem> items;

        if(isGroupList)
        {
            items = parseGroups(arr);
            emit receivedGroupsList(items);
        }
        else
        {
            items = parseItems(arr);
            if(isClipboard)
            {
                emit receivedClipboardContent(items);
            }
            else
            {
                emit receivedGroupContent(items, containerId);
            }
        }

        reply->deleteLater();
    });
}

QList<ClipboardItem> parseItems(const QJsonArray &array)
{
    QList<ClipboardItem> list;
    for( const QJsonValue &value: array)
    {
        const QJsonObject obj = value.toObject();

        // Group ID
        auto container_id = QUrl(obj["container_id"].toString());
        // Artefact ID
        auto uri = QUrl(obj["uri"].toString());
        // Short description
        auto label = obj["label"].toString();

        ClipboardItem item{
            container_id,
            {
                ArtefactId::fromUri(uri),
                GroupId::fromUri(container_id),
                GResource::Type::ARTEFACT,
                label
            }
        };
        list.append(item);
    }
    return list;
}

QList<ClipboardItem> parseGroups(const QJsonArray &array)
{
    QList<ClipboardItem> list;
    for( const QJsonValue &value: array)
    {
        QJsonObject obj = value.toObject();

        auto containerId = QUrl(obj["id"].toString());
        auto label = obj["label"].toString();

        ClipboardItem item{
            containerId,
            {
                // TODO fix ArtefactId initialization
                ArtefactId::fromUri(QUrl()),
                GroupId::fromUri(containerId),
                GResource::Type::GROUP,
                label
            }
        };

        list.append(item);
    }
    return list;
}
