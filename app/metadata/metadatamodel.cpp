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
#include "metadatamodel.h"
#include "sparqlresults.h"
#include <QVariant>
#include <QPair>
#include <QString>
#include <QUrlQuery>
#include "stringmanipulation.h"
#include "nlohmann/json.hpp"
#include <QFile>
#include <exception>
#include <QDebug>
#include "metadatachprovider.h"

using json = nlohmann::json;

MetadataModel::MetadataModel(MetadataChProvider *provider, QObject *parent)
    : QStandardItemModel(parent), m_metadataProvider(provider)
{
    initKeyToLabelMap();

    connect(m_metadataProvider, &MetadataChProvider::received,
            [this] (const QVariantMap &metadataTree) {
        parseTree(invisibleRootItem(), metadataTree);
    });
}

void MetadataModel::requestArtefactMetadata(const ArtefactId &artefactId)
{
    clear();
    m_metadataProvider->requestMetadata(artefactId);
}

QVariant MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
             return QVariant();

     if (orientation == Qt::Horizontal) {
         switch (section) {
             case 0:
                 return tr("Property");

             case 1:
                 return tr("Value");

             default:
                 return QVariant();
         }
     }
     return QVariant();
}

int MetadataModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

void MetadataModel::initKeyToLabelMap()
{
    try {
        auto fileName = ":/metadata/metadata-api-conf.json";

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            qCritical() << "Could not open file" << fileName;
            return;
        }

        auto data = file.readAll().toStdString();
        json j = json::parse(data);

        auto dict = j.at("labels");

        if(!dict.is_object())
        {
            qCritical() << "'labels' is not an object";
            return;
        }

        for(auto it : dict.items())
        {
            auto key = QString::fromStdString(it.key());
            auto value = QString::fromStdString(it.value());

            m_keyToLabelMap.insert(key, value);
        }

    } catch (json::exception &e) {
        qCritical() << e.what();
    }


}

QString MetadataModel::keyToLabel(const QString key)
{
    return m_keyToLabelMap.value(key, key);
}

void MetadataModel::parseTree(QStandardItem *parent, const QVariantMap &varmap)
{
    for(auto key : varmap.keys())
    {
        auto value = varmap.value(key);
        auto prettyName = keyToLabel(key);

        if(value.isNull())
        {
            addMetadataItem(prettyName, "null", parent);
        }
        else if(value.canConvert<QVariantMap>())
        {
            auto cat = addCategory(prettyName, parent);
            parseTree(cat, value.toMap());
        }
        else if (value.canConvert<QString>())
        {
            addMetadataItem(prettyName, value.toString(), parent);
        }
        else
        {
            addMetadataItem(prettyName, "invalid", parent);
        }
    }
}

QStandardItem* MetadataModel::addCategory(const QString &name, QStandardItem *parentItem)
{
    auto item = new QStandardItem(name);
    parentItem->appendRow(item);
    return item;
}

void MetadataModel::addMetadataItem(const QString &name, const QString &value, QStandardItem *parentItem)
{
    QList<QStandardItem*> row = {
        new QStandardItem(name),
        new QStandardItem(value)
    };
    parentItem->appendRow(row);
}
