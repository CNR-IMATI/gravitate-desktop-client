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
#ifndef DATASETFETCHMODEL_H
#define DATASETFETCHMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QUrl>
#include <QPixmap>
#include "artefact3dmetadata.h"
#include "enums.h"

class AbstractDashboard;

class DatasetFetchModel : public QAbstractTableModel
{
    struct Item
    {
        ArtefactId artefactId;
        QString inventoryId;
        QString description;
        bool selected;
    };

    Q_OBJECT

    const int m_pixmapHeight = 150;

public:
    explicit DatasetFetchModel(AbstractDashboard *dashboard, QObject *parent = nullptr);

    QList<ArtefactId> selectedArtefacts();

    void loadArtefactList(const QList<ArtefactId> &artefacts, int itemsPerPage = -1, int page = -1);

    void fetchData(const QList<ArtefactId> &artefacts, const QList<ModelResolution> &resolutions, const QList<MultiResProperty> &property);

public slots:
    void setAllSelected(bool selected);

signals:
    void currentFileProgress(quint64 bytesRead, quint64 bytesTotal, const QString &fileName);

    void completedFileProgress(int filesCompleted, int filesTotal, const QString &currentFile);

    void downloadFinished(int completed);

private slots:
    void onItemListReceived(const QVariantList &items);

private:

    AbstractDashboard *m_dashboard;

    QList<Item> m_itemList;

    bool m_wasCanceled = false;

    void startDownload(const QList<QUrl> &files);

    void cancelDownload();

    bool is3DFile(const QUrl &url);


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // DATASETFETCHMODEL_H
