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
#ifndef THESAURUSMODEL_H
#define THESAURUSMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QVariantMap>
#include "term.h"

class QStandardItem;

class ThesaurusModel : public QStandardItemModel
{
    struct Node
    {
        QStandardItem *item;
        Term term;
    };

    Q_OBJECT
public:
    explicit ThesaurusModel(QObject *parent = nullptr);

    bool readJson(const QByteArray &data, QString *errorMsg = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    Term term(const QModelIndex &index);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:

public slots:

private:

    void parseSubtree(const QVariantMap &map, QStandardItem *parent);

    QList<Node> m_items;

};


#endif // THESAURUSMODEL_H
