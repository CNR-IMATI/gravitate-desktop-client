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
#include "clipboardsynccommand.h"
#include "gresource.h"

namespace {
    bool itemComparison(const ClipboardItem &i1, const ClipboardItem &i2)
    {
        return i1.resource.type == GResource::Type::GROUP &&
                i2.resource.type == GResource::Type::ARTEFACT;
    }
}

FullSyncCommand::FullSyncCommand(ClipboardModel *model)
    : m_model(model)
{

}

void FullSyncCommand::begin()
{
    m_model->reset();
}

void FullSyncCommand::end()
{

}

void FullSyncCommand::execute(const QList<ClipboardItem> &items, const QModelIndex &parent)
{
    if(m_model == nullptr)
        return;

    if(items.size() == 0)
    {
        emit m_model->groupContentLoaded(parent);
        return;
    }

    quintptr parentId = parent.isValid() ?
        parent.internalId() : m_model->m_items.rootId();

    m_model->beginInsertRows( parent , 0, items.size() - 1);
    for(auto i : items)
    {
        m_model->m_items.insert(i, parentId);
    }
    m_model->m_items.sortChildren(parentId, &itemComparison);
    m_model->endInsertRows();
    emit m_model->groupContentLoaded(parent);

}

BufferedSyncCommand::BufferedSyncCommand(ClipboardModel *model)
    : m_model(model)
{

}

void BufferedSyncCommand::begin()
{

}

void BufferedSyncCommand::end()
{

}

void BufferedSyncCommand::execute(const QList<ClipboardItem> &items, const QModelIndex &parent)
{
    if(m_model == nullptr)
        return;

    QList<BufferItem> diffItems;

    //    // If the items list is empty, we are done
    //    if(items.size() == 0)
    //    {
    //        return;
    //    }

    quintptr parentId = parent.isValid() ?
                parent.internalId() : m_model->m_items.rootId();

    //    QList<ClipboardItem> currentList;
    //    for( int i = 0; i < m_items.childCount(parentId); i++)
    //    {
    //        quintptr childId = m_items.child(parentId, i);
    //        currentList.append(m_items.getNode(childId));
    //    }

    //    int i = startIndex;
    //    for( ClipboardItem item : items)
    //    {
    //        if(!currentList.contains(item))
    //        {
    ////            qDebug() << "ADDED" << item.label;
    //            buffer.append({i, item});
    //        }
    //        else
    //        {
    ////            qDebug() << "SKIPPED" << item.label;
    //        }
    //        i++;
    //    }

    //    for(int i = startIndex; i <= endIndex; i++)
    //    {
    //        if(i >= m_items.childCount(parentId))
    //        {
    //            qDebug("WARNING: requesting an item out of index in the tree");
    //            break;
    //        }
    //        ClipboardItem item = currentList.at(i);
    //        if( !items.contains(item) )
    //        {
    ////            qDebug() << "DELETED" << item.label;
    //            beginRemoveRows(parent, i, i);
    //            quintptr childId = m_items.child(parentId, i);
    //            m_items.remove(childId);
    //            endRemoveRows();
    //        }
    //    }

}
