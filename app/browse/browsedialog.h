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
#ifndef DATASETFETCHDIALOG_H
#define DATASETFETCHDIALOG_H

#include <QDialog>
#include "artefact3dmetadata.h"

class QMenu;
class QStringList;
class ClipboardModel;
class DatasetFetchModel;

namespace Ui {
class BrowseDialog;
}

class BrowseDialog : public QDialog
{    
    Q_OBJECT

public:

    explicit BrowseDialog(QWidget *parent = 0);
    ~BrowseDialog();

    QList<ArtefactId> selectedArtefacts();
    QList<MultiResProperty> selectedProperties();
    QList<ModelResolution> selectedResolutions();

    void setClipboardModel(ClipboardModel *clipboardModel);

    void setDatasetFetchModel(DatasetFetchModel *model);

    void addDataset(const QString &name, const QStringList &artefacts);

private slots:
    void onCurrentIndexChanged(const QString &name);

    void onStartDownload();

    void onAddSelectedToExistingGroup();

private:
    Ui::BrowseDialog *ui;

    ClipboardModel *m_clipboardModel;
    DatasetFetchModel *m_datasetFetchModel;

    QMenu *m_addToMenu;

    QMenu *createAddToMenu();

    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // DATASETFETCHDIALOG_H
