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
#include "thesaurusmodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QDebug>

ThesaurusModel::ThesaurusModel(QObject *parent)
    : QStandardItemModel (parent)
{

}

bool ThesaurusModel::readJson(const QByteArray &data, QString *errorMsg)
{
    QJsonParseError err;

    auto doc = QJsonDocument::fromJson(data, &err);

    if(err.error != QJsonParseError::NoError)
    {
        qCritical() << err.errorString();
        if(errorMsg != nullptr)
            *errorMsg = err.errorString();

        return false;
    }

    auto map = doc.object().toVariantMap();

    if(map.isEmpty())
    {
        qWarning() << "The JSON document was empty";
        return false;
    }

    parseSubtree(map, invisibleRootItem());
    return true;
}

void ThesaurusModel::parseSubtree(const QVariantMap &map, QStandardItem *parent)
{
    if(parent == nullptr)
    {
        qCritical() << "[ERROR] Parent item not set";
        return;
    }

    auto uri = map.value("uri").toString();
    auto label = map.value("label", "N/A").toString();
    auto desc = map.value("description", "N/A").toString();
    auto children = map["children"];

    Term t;
    t.id = uri.toStdString();
    t.label = label.toStdString();
    t.description = desc.toStdString();

    auto item = new QStandardItem();
    item->setData(QVariant::fromValue(t));
    parent->appendRow(item);

    for(auto c : children.toList())
    {
        if(!c.canConvert<QVariantMap>())
        {
            qCritical() << "Not a valid QVariantMap object:" << c;
            continue;
        }

        parseSubtree(c.toMap(), item);
    }
}


QVariant ThesaurusModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        auto item = itemFromIndex(index);
        auto term = item->data().value<Term>();
        return QString::fromStdString(term.label);
    }

    return QVariant();
}

Term ThesaurusModel::term(const QModelIndex &index)
{

    if(!index.isValid())
        return Term();

    auto item = itemFromIndex(index);
    if(item == nullptr)
        return Term();

    return item->data().value<Term>();
}

Qt::ItemFlags ThesaurusModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
