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
#ifndef METADATAMODEL_H
#define METADATAMODEL_H

#include <QStandardItemModel>
#include <QVariant>
#include <QVariantList>
#include <QMap>
#include <QString>
#include "artefactid.h"

class MetadataChProvider;

class MetadataModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit MetadataModel(MetadataChProvider *provider, QObject *parent = 0);

    void requestArtefactMetadata(const ArtefactId &artefactId);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:

    void initKeyToLabelMap();

    QString keyToLabel(const QString key);

    void parseTree(QStandardItem *parent, const QVariantMap &varmap);

    QStandardItem* addCategory(const QString &name, QStandardItem *parentItem);
    void addMetadataItem(const QString &name, const QString &value, QStandardItem *parentItem);

    QMap<QString, QString> m_keyToLabelMap;

    MetadataChProvider *m_metadataProvider;
};

#endif // METADATAMODEL_H
