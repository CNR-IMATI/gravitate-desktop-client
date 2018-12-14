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
#include "clipboardmodel.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileIconProvider>
#include <QPainter>
#include "gravitatebackendconf.h"
#include "sparqlresults.h"
#include "clipboardsynccommand.h"

ClipboardModel::ClipboardModel(AbstractClipGroupsProvider *clipGroups, QObject *parent)
    : QAbstractItemModel(parent),
      m_clipGroups(clipGroups),
      m_syncCommand(new FullSyncCommand(this))
{
    if(m_clipGroups == nullptr)
    {
        qCritical() << "ClipGroupsProvider is null";
        return;
    }

    connect(this, &ClipboardModel::error,
            [] (const QString &message) {
        qDebug() << tr("Error: %1").arg(message);
    });

    connect(m_clipGroups, &AbstractClipGroupsProvider::receivedGroupsList,
            this, &ClipboardModel::onReceiveGroupsList);

    connect(m_clipGroups, &AbstractClipGroupsProvider::receivedClipboardContent,
            this, &ClipboardModel::onReceiveClipboardContent);

    connect(m_clipGroups, &AbstractClipGroupsProvider::receivedGroupContent,
            this, &ClipboardModel::onReceiveGroupContent);

    connect(m_clipGroups, &AbstractClipGroupsProvider::resourceRemoved,
            this, &ClipboardModel::onResourceRemoved);

    connect(this, &ClipboardModel::itemDeleted,
            this, &ClipboardModel::removeItem);

    connect(this, &ClipboardModel::groupCreated,
            this, &ClipboardModel::loadClipboard);

    connect(this, &ClipboardModel::itemAdded,
            this, &ClipboardModel::loadClipboard);

    connect(this, &ClipboardModel::groupContentLoaded,
            this, &ClipboardModel::fetchThumbnails);

    reset();
}

void ClipboardModel::setThumbnailProvider(AbstractThumbnailProvider *thumbnailProvider)
{
    m_thumbnailProvider = thumbnailProvider;

    connect(m_thumbnailProvider, &AbstractThumbnailProvider::thumbnailReceived,
            this, &ClipboardModel::onThumbnailReceived);
}

void ClipboardModel::loadClipboard()
{
    if(m_clipGroups == nullptr)
    {
        qCritical() << "Clipboard provider has not been initialized";
        return;
    }

    m_syncCommand->begin();

    m_clipGroups->requestGroupsList();
    m_clipGroups->requestClipboardContent();

    m_syncCommand->end();
}

void ClipboardModel::createGroup(const QString &name)
{
    m_clipGroups->createGroup(name);
}

void ClipboardModel::addToGroup(const ArtefactId &artefactId, const GroupId &groupId)
{
    m_clipGroups->addToGroup(artefactId, groupId);
}

void ClipboardModel::addToClipboard(const ArtefactId &artefactId)
{
    qDebug() << tr("Adding resource %1 to clipboard...").arg(artefactId.toString());
    addToGroup(artefactId, GroupId::null());
}

void ClipboardModel::removeResource(const QModelIndex &index)
{
    qDebug() << tr("Removing resource %1...")
                .arg(label(index));

    if(!index.isValid())
    {
        qDebug() << tr("Cannot delete resource: invalid index");
        return;
    }

    QUrl uri = containerId(index);
    m_clipGroups->removeResource(uri);
}

QModelIndex ClipboardModel::findGroupIndex(const GroupId &groupId) const
{
    for(int i = 0; i < rowCount(); i++)
    {
        auto idx = index(i, 0);
        auto res = resource(idx);
        if(res.groupId == groupId)
        {
            return idx;
        }
    }
    return QModelIndex();
}

ArtefactId ClipboardModel::artefactId(const QModelIndex &index) const
{
    if(!index.isValid())
        return ArtefactId::null();

    ClipboardItem item = m_items.getNode(index.internalId());
    return item.resource.type == GResource::Type::ARTEFACT ?
                item.resource.artefactId : ArtefactId::null();
}

QUrl ClipboardModel::containerId(const QModelIndex &index) const
{
    if(!index.isValid())
        return QUrl();

    ClipboardItem item = m_items.getNode(index.internalId());
    return item.containerId;
}

GResource ClipboardModel::resource(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return GResource{
            ArtefactId::null(),
            GroupId::null(),
            GResource::Type::UNDEFINED,
            QString()
        };
    }

    ClipboardItem item = m_items.getNode(index.internalId());

    return item.resource;
}

GResource::Type ClipboardModel::itemType(const QModelIndex &index) const
{
    if(!index.isValid())
        return GResource::Type::UNDEFINED;
    ClipboardItem item = m_items.getNode(index.internalId());
    return item.resource.type;
}

QString ClipboardModel::label(const QModelIndex &index) const
{
    if(!index.isValid())
        return QString();
    ClipboardItem item = m_items.getNode(index.internalId());
    return item.resource.label;
}

QList<GResource> ClipboardModel::groups() const
{
    QList<GResource> l;
    for(int i = 0; i < rowCount(); i++)
    {
        auto idx = index(i, 0);
        if(!idx.isValid())
            continue;

        auto res = resource(idx);
        if(res.type == GResource::Type::GROUP)
        {
            l.append(res);
        }
    }
    return l;
}

QList<ArtefactId> ClipboardModel::getArtefacts(const GroupId &groupId) const
{
    auto groupIndex = findGroupIndex(groupId);
    if(!groupIndex.isValid())
        return QList<ArtefactId>();

    auto l = QList<ArtefactId>();

    for(int i = 0; i < rowCount(groupIndex); i++)
    {
        auto idx = index(i, 0, groupIndex);
        if(!idx.isValid())
            continue;

        auto res = resource(idx);

        if(res.type == GResource::Type::ARTEFACT)
        {
            l.append(res.artefactId);
        }
    }
    return l;
}

QVariant ClipboardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
    // FIXME: Implement me!
}

QModelIndex ClipboardModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    quintptr parentId;
    if(!parent.isValid())
        parentId = m_items.rootId();
    else
        parentId = parent.internalId();

    quintptr childId = m_items.child(parentId, row);
    return createIndex(row, column, childId);
}

QModelIndex ClipboardModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    quintptr nodeId = index.internalId();
    quintptr parentId = m_items.parent(nodeId);

    if(parentId == m_items.rootId())
        return QModelIndex();

    return createIndex(m_items.row(parentId), 0, parentId);
}

bool ClipboardModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return true;
    ClipboardItem item = m_items.getNode(parent.internalId());
    return item.resource.type == GResource::Type::GROUP;
}

int ClipboardModel::rowCount(const QModelIndex &parent) const
{  
    quintptr parentId;
    if(parent.isValid())
        parentId = parent.internalId();
    else
        parentId = m_items.rootId();

    return m_items.childCount(parentId);
}

int ClipboardModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ClipboardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ClipboardItem item = m_items.getNode(index.internalId());
    auto res = item.resource;

    switch(role)
    {
    case Qt::DisplayRole:
        return res.label;
    case Qt::DecorationRole:
    {
        QFileIconProvider p;
        if(res.type == GResource::Type::ARTEFACT)
        {
            if(m_thumbnailProvider->hasThumbnail(res.artefactId))
            {
                return QIcon(m_thumbnailProvider->getPixmap(res.artefactId));
            }
            return p.icon(QFileIconProvider::File);
        }
        else if (res.type == GResource::Type::GROUP)
        {
            return p.icon(QFileIconProvider::Folder);
        }
        // Otherwise, return an "empty" icon
        return QVariant();
    }
    case Qt::ToolTipRole:
    {
        if(res.type == GResource::Type::ARTEFACT)
            return res.artefactId.uri;
        return QVariant();
    }
    default:
        return QVariant();
    }
}

void ClipboardModel::loadGroupItems(const GroupId &groupId, const QModelIndex &parent)
{
    if(groupId.isNull())
    {
        emit error("Group id is empty");
        return;
    }

    m_requestedGroupsMap.insert(groupId, parent);
    m_clipGroups->requestGroupContent(groupId);
}

void ClipboardModel::onReceiveResources(const QList<ClipboardItem> items, const QModelIndex &parent)
{
    m_syncCommand->execute(items, parent);
}

void ClipboardModel::fetchThumbnails(const QModelIndex &parent)
{   
    if(m_thumbnailProvider == nullptr)
    {
        qCritical() << "ThumbnailProvider not initialized";
        return;
    }

    quintptr parentId = parent.isValid() ?
                parent.internalId() : m_items.rootId();
    ClipboardItem parentItem = m_items.getNode(parentId);

    for(int i = 0; i < rowCount(parent); i++)
    {
        QModelIndex idx = index(i, 0, parent);
        ClipboardItem item = m_items.getNode(idx.internalId());

        if(item.resource.type == GResource::Type::ARTEFACT)
        {
            auto id = item.resource.artefactId;
            m_thumbnailProvider->requestArtefactThumbnail(id);

            // Associate artefact id with QModelIndex
            m_requestedThumbnailMap.insert(id, idx);
        }
    }
}

void ClipboardModel::reset()
{
    beginResetModel();
    m_items.clear();
    m_items.insertRoot({QUrl(), GResource()});
    endResetModel();
}

void ClipboardModel::removeItem(const QModelIndex &index)
{
    quintptr nodeId = index.internalId();
    int row = m_items.row(nodeId);

    beginRemoveRows(index, row, row);
    m_items.remove(nodeId);
    endRemoveRows();
}

void ClipboardModel::fetchMore(const QModelIndex &parent)
{
    if(!canFetchMore(parent))
        return;

    m_fetchedNodes.insert(parent.internalId());

    ClipboardItem item = m_items.getNode(parent.internalId());

    auto groupId = GroupId::fromUri(item.containerId);

    loadGroupItems( groupId, parent );
}

bool ClipboardModel::canFetchMore(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return false;

    ClipboardItem item = m_items.getNode(parent.internalId());
    return item.resource.type == GResource::Type::GROUP && !m_fetchedNodes.contains(parent.internalId());
}

void ClipboardModel::onThumbnailReceived(const QPixmap &pixmap, const ArtefactId &artefactId)
{
    auto index = m_requestedThumbnailMap.take(artefactId);
    if(index.isValid())
    {
        emit dataChanged(index, index, {Qt::DecorationRole});
    }
}

// New Functions
void ClipboardModel::onReceiveGroupsList(const QList<ClipboardItem> &items)
{
    auto parent = QModelIndex();
    onReceiveResources(items, parent);
}

void ClipboardModel::onReceiveClipboardContent(const QList<ClipboardItem> &items)
{
    auto parent = QModelIndex();
    onReceiveResources(items, parent);
}

void ClipboardModel::onReceiveGroupContent(const QList<ClipboardItem> &items, const GroupId &groupId)
{
    auto parent = m_requestedGroupsMap.take(groupId);

    if(!parent.isValid())
        return;
    onReceiveResources(items, parent);
}

void ClipboardModel::onResourceRemoved(const QUrl &groupId)
{
    // Find the index of the resource being removed
    // Remove the resource
    // Emit the corresponding signal

}
