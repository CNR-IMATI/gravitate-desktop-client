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
#include "datasetfetchmodel.h"
#include "abstractdashboard.h"
#include "backendprovider.h"
#include "abstractdownloader.h"
#include "localfilecache.h"
#include "gravitatesparql.h"
#include "gravitaterepoconf.h"
#include <QEventLoop>
#include <QRegularExpression>
#include <QProgressDialog>
#include "backendfactorybase.h"
#include "qxtsignalwaiter.h"

namespace {



const int COL_SELECTED = 0;
const int COL_THUMBNAIL    = 1;
const int COL_ARTEFACT_ID  = 2;
const int COL_INVENTORY_ID = 3;
const int COL_DESCRIPTION  = 4;

const int COLUMN_COUNT = 5;

QMap<int, QString> m_sectionLabels;

}


DatasetFetchModel::DatasetFetchModel(AbstractDashboard *dashboard, QObject *parent)
    : QAbstractTableModel(parent),
      m_dashboard(dashboard)
{
    m_sectionLabels[COL_SELECTED] = tr("Selected");
    m_sectionLabels[COL_ARTEFACT_ID] = tr("Artefact ID");
    m_sectionLabels[COL_INVENTORY_ID] = tr("Inventory ID");
    m_sectionLabels[COL_THUMBNAIL] = tr("Thumbnail");
    m_sectionLabels[COL_DESCRIPTION] = tr("Description");
}

QList<ArtefactId> DatasetFetchModel::selectedArtefacts()
{
    QList<ArtefactId> l;

    for(auto item : m_itemList)
    {
        if(item.selected)
            l.append(item.artefactId);
    }

    return l;
}

void DatasetFetchModel::loadArtefactList(const QList<ArtefactId> &artefacts, int itemsPerPage, int page)
{
    auto p = BackendFactoryBase::instance()->createItemListProvider();

    connect(p.get(), &ItemListProviderBase::itemsReceived,
            this, &DatasetFetchModel::onItemListReceived);

    p->requestItems(artefacts);
    QxtSignalWaiter::wait(p.get(), SIGNAL(itemsReceived(const QVariantList&)), 5000);
}

// NOTE: this function has been disabled, because a proper redesign is needed
// (i.e. encapsulate the request in a backend class, such as Metadata3DRequest)
// Purpose: remove dependency from BackendProvider
void DatasetFetchModel::fetchData(const QList<ArtefactId> &artefacts, const QList<ModelResolution> &resolutions, const QList<MultiResProperty> &property)
{
    qWarning() << "Function currently disabled:" << Q_FUNC_INFO;
//    auto queryString = GravitateSparql::metadataQuery(artefacts);
//    auto backend = m_dashboard->backendProvider();
//    QNetworkReply *reply = backend->sparqlQuery(queryString);
//    connect(reply, &QNetworkReply::finished,
//        [=] {
//        //using namespace GravitateRepo;

//        if(reply->error() != QNetworkReply::NoError)
//        {
//            reply->deleteLater();
//            qCritical() << reply->errorString();
//            return;
//        }

//        SparqlResults results;
//        results.readJson(reply->readAll());

//        QList<QUrl> toDownload;

//        while(results.next())
//        {
//            auto file = results.value("file").toUrl();
//            auto type = results.value("type").toString();

//            if(is3DFile(file))
//                toDownload.append(file);
//        }

//        startDownload(toDownload);
//    });
}

void DatasetFetchModel::setAllSelected(bool selected)
{
    auto length = m_itemList.size();
    if(length == 0)
    {
        qDebug() << "Error: length = 0";
        return;
    }

    for(int i = 0; i < length; i++)
    {
        setData(index(i, 0), QVariant::fromValue<bool>(selected),
                Qt::CheckStateRole);
    }

}

void DatasetFetchModel::onItemListReceived(const QVariantList &items)
{
    beginResetModel();
    m_itemList.clear();
    endResetModel();

    beginInsertRows(index(0,0), 0, items.count() - 1);

    for(auto entry : items)
    {
        auto row = entry.toMap();

        Item i;
        i.artefactId = ArtefactId::fromUri(row.value("id").toUrl());
        i.inventoryId = row.value("inventoryId").toString();
        i.description = row.value("description").toString();
        i.selected = false;

        m_itemList.append(i);
    }

    endInsertRows();
}

void DatasetFetchModel::startDownload(const QList<QUrl> &files)
{
    int total = files.size();
    int completed = 0;

    auto downloader = m_dashboard->downloader();
    auto cache = LocalFileCache::instance();

    for(auto url : files)
    {
        bool ok;
        auto data = downloader->getFile(url, &ok);
        if(!ok)
            continue;

        if(m_wasCanceled)
        {
            m_wasCanceled = false;
            return;
        }

        cache->insert(url, data);
        auto currentFile = url.fileName();

        completed++;
        emit completedFileProgress(completed, total, currentFile);
    }
    emit downloadFinished(completed);

}

void DatasetFetchModel::cancelDownload()
{
    m_wasCanceled = true;
}

bool DatasetFetchModel::is3DFile(const QUrl &url)
{
    QString extensions=(".txt|.json|.ply");
    for(auto ext : extensions.split("|"))
    {
        if(url.toString().endsWith(ext))
            return true;
    }
    return false;
}

int DatasetFetchModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_itemList.size();
}

int DatasetFetchModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return COLUMN_COUNT;
}

QVariant DatasetFetchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    auto item = m_itemList.at(row);

    switch (role) {
    case Qt::DisplayRole:
        if(col == COL_ARTEFACT_ID)
            return item.artefactId.toString();
        else if(col == COL_INVENTORY_ID)
            return item.inventoryId;
        else if(col == COL_DESCRIPTION)
            return item.description;
        else
            return QVariant();
    case Qt::DecorationRole:
        if(col == COL_THUMBNAIL)
        {
            auto thumb = m_dashboard->thumbnailProvider();
            return thumb->getPixmap(item.artefactId)
                    .scaledToHeight(m_pixmapHeight, Qt::SmoothTransformation);
        }
        else return QVariant();
    case Qt::CheckStateRole:
        if(col == COL_SELECTED)
            return item.selected ? Qt::Checked : Qt::Unchecked;
        else return QVariant();
    }
    return QVariant();
}


QVariant DatasetFetchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        if(section >= columnCount(QModelIndex()))
            return QVariant();

        return m_sectionLabels.value(section);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}


bool DatasetFetchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    auto row = index.row();
    auto col = index.column();

    if(col == 0 && role == Qt::CheckStateRole)
    {
        auto selected = value.toBool();
        m_itemList[row].selected = selected;
        emit dataChanged(index, index);
        return true;
    }

    return false;
}


Qt::ItemFlags DatasetFetchModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractTableModel::flags(index);

    auto col = index.column();
    if(col == 0)
        flags = flags | Qt::ItemIsUserCheckable;

    return flags;
}
