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
#ifndef GROUPPROXYMODEL_H
#define GROUPPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "clipboardmodel.h"

/**
 * @brief The GroupProxyModel class filters only the items of
 * type "Group" from the ClipboardModel.
 *
 * The setSourceModel() function will accept only arguments of
 * ClipboardModel* type.
 *
 */
class GroupProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit GroupProxyModel(QObject *parent = 0);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    // QAbstractProxyModel interface
public:
    void setSourceModel(QAbstractItemModel *sourceModel) override;

private:

    ClipboardModel * clipboardModel() const;
};

#endif // GROUPPROXYMODEL_H
