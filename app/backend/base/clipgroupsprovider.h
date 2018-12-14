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
#ifndef CLIPGROUPSPROVIDER_H
#define CLIPGROUPSPROVIDER_H

#include <QObject>
#include <QUrl>
#include <QJsonArray>
#include "gresource.h"
#include "artefactid.h"

struct ClipboardItem
{
    QUrl containerId;
    GResource resource;

    bool operator <(const ClipboardItem &other)
    {
        return containerId.toString() < other.containerId.toString();
    }

    bool operator ==(const ClipboardItem &other)
    {
        return containerId.toString() == other.containerId.toString();
    }
};

Q_DECLARE_METATYPE(ClipboardItem)

class AbstractClipGroupsProvider : public QObject
{
    Q_OBJECT
public:
    explicit AbstractClipGroupsProvider(QObject *parent = nullptr)
        : QObject (parent) {}

signals:
    void receivedClipboardContent(const QList<ClipboardItem> &items);
    void receivedGroupsList(const QList<ClipboardItem> &items);
    void receivedGroupContent(const QList<ClipboardItem> &items, const GroupId &groupId);
    void groupCreated(const QString &name);
    void resourceRemoved(const QUrl &containerId);
    void addedToGroup(const ArtefactId &artefactId, const GroupId &groupId);

public slots:

    virtual void requestClipboardContent() = 0;
    virtual void requestGroupsList() = 0;
    virtual void requestGroupContent(const GroupId &containerId) = 0;
    virtual void createGroup(const QString &name) = 0;
    virtual void removeResource(const QUrl &containerId) = 0;
    virtual void addToGroup(const ArtefactId &artefactId, const GroupId &groupId) = 0;
};

#endif // CLIPGROUPSPROVIDER_H
