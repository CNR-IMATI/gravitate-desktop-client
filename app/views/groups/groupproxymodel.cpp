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
#include "groupproxymodel.h"

GroupProxyModel::GroupProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

ClipboardModel * GroupProxyModel::clipboardModel() const
{
    return qobject_cast<ClipboardModel*>(sourceModel());
}

bool GroupProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    ClipboardModel *model = clipboardModel();

    QModelIndex index = model->index(source_row, 0, source_parent);

    return model->itemType(index) == GResource::Type::GROUP;
}

void GroupProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    ClipboardModel *model = qobject_cast<ClipboardModel*>(sourceModel);
    if(model == nullptr)
    {
        qCritical() << "Model is not of ClipboardModel type";
        return;
    }

    QSortFilterProxyModel::setSourceModel(sourceModel);

    connect(model, &ClipboardModel::rowsRemoved,
            [this] {
        beginResetModel();
        endResetModel();
    });

    connect(model, &ClipboardModel::rowsInserted,
            [this] {
        beginResetModel();
        endResetModel();
    });
}
