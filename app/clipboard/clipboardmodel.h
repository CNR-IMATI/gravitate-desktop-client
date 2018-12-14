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
#ifndef CLIPBOARDMODEL_H
#define CLIPBOARDMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QPixmap>
#include <QHash>
#include "abstractdownloader.h"
#include "qhashtree.h"
#include <QSet>
#include "gresource.h"
#include "thumbnailprovider.h"
#include "clipgroupsprovider.h"

class ClipboardSyncCommand;

/**
 * @brief The ClipboardModelNew class
 *
 * This is a model for accessing and manipulating the Clipboard
 *
 * The Clipboard is a space where the user can store individual items and
 * groups
 *
 *
 */
class ClipboardModel : public QAbstractItemModel
{

    Q_OBJECT

    friend class FullSyncCommand;
    friend class BufferedSyncCommand;

protected:

    struct BufferItem
    {
        int pos;
        ClipboardItem item;
    };

    QSet<quintptr> m_fetchedNodes;

    QHashTree<ClipboardItem> m_items;

public:
    explicit ClipboardModel(AbstractClipGroupsProvider *clipGroups, QObject *parent = 0);

    void setThumbnailProvider(AbstractThumbnailProvider *thumbnailProvider);

    // CUSTOM MANIPULATION FUNCTIONS
    /**
     * @brief loadClipboard loads the clipboard from the backend
     */
    void loadClipboard();

    // Group Manipulation functions

    void createGroup(const QString &name);

    void addToGroup(const ArtefactId &artefactId, const GroupId &groupId);

    void addToClipboard(const ArtefactId &artefactId);

    void removeResource(const QModelIndex &index);

    // Query functions

    QModelIndex findGroupIndex(const GroupId &groupId) const;

    ArtefactId artefactId(const QModelIndex &index) const;

    QUrl containerId(const QModelIndex &index) const;

    GResource resource(const QModelIndex &index) const;

    GResource::Type itemType(const QModelIndex &index) const;

    QString label(const QModelIndex &index) const;

    QList<GResource> groups() const;

    QList<ArtefactId> getArtefacts(const GroupId &groupId) const;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Data fetching (for expanding a node)

    bool hasChildren(const QModelIndex &parent) const override;

    void fetchMore(const QModelIndex &parent) override;

    bool canFetchMore(const QModelIndex &parent) const override;

    void onThumbnailReceived(const QPixmap &pixmap, const ArtefactId &artefactId);

signals:
    void error(const QString &message);

    void groupContentLoaded(const QModelIndex &parent);

    void itemDeleted(const QModelIndex &index);

    void groupCreated(const QString &name);

    void itemAdded(const ArtefactId &artefactId, const GroupId &groupId);

private slots:
    void removeItem(const QModelIndex &index);

    void onReceiveGroupsList(const QList<ClipboardItem> &items);

    void onReceiveClipboardContent(const QList<ClipboardItem> &items);

    void onReceiveGroupContent(const QList<ClipboardItem> &items, const GroupId &groupId);

    void onResourceRemoved(const QUrl &groupId);

private:

    void loadGroupItems(const GroupId &groupId, const QModelIndex &parent);

    void fetchThumbnails(const QModelIndex &parent);

    void reset();

    AbstractClipGroupsProvider *m_clipGroups;

    AbstractThumbnailProvider *m_thumbnailProvider = nullptr;

    ClipboardSyncCommand * m_syncCommand;

    QMap<ArtefactId, QModelIndex> m_requestedThumbnailMap;

    QMap<GroupId, QModelIndex> m_requestedGroupsMap;

    void onReceiveResources(const QList<ClipboardItem> items, const QModelIndex &parent);


};



#endif // CLIPBOARDMODEL_H
